//
// Created by root on 12/30/18.
//

#include <time.h>
#include <ctype.h>
#include "orchestrator.h"


int orchestrate_run(char* output_file , char** input_files ,size_t number_of_input_files,char* static_src_file) {
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    FILE *fptr = NULL;
    int i = 0;
    int is_first_line_written = 0;

    if(NULL == output_file || NULL == input_files || number_of_input_files<=0){
        res = UDPFLOW_ORCH_BAD_PARAMETER;
        goto lbl_cleanup;
    }

    res = create_file(&fptr,output_file);
    if(UDPFLOW_SUCCESS != res){
        res = UDPFLOW_ORCH_COULDENT_CREATE_FILE;
        goto lbl_cleanup;

    }

    res = write_openning_json_file(fptr);
    if(UDPFLOW_SUCCESS != res){
        res = UDPFLOW_ORCH_COULDENT_WRITE_OPENNING;
        goto lbl_cleanup;
    }

    for(i ; i < number_of_input_files ; i++){
        res = file_operation(input_files[i],fptr,static_src_file,is_first_line_written,i+1);
        if (UDPFLOW_SUCCESS != res){
            printf("PROBLEM WITH PCAP : %d", res);
        } else{
            is_first_line_written = 1;
        }
    }

    res = write_ending_json_file(fptr);

    if (UDPFLOW_SUCCESS != res){
        res = UDPFLOW_ORCH_COULDENT_WRITE_ENDING;
        goto lbl_cleanup;
    }

lbl_cleanup:
    if(NULL != fptr)
        fclose(fptr);
    return res;
}

udpflow_res_t file_operation(char* input_file ,FILE* output_file,char* static_src_file,int flag,unsigned int index){
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    struct session_metadata *metadata = NULL;
    res = create_session_metadata(&metadata);
    if(UDPFLOW_SUCCESS != res){
        goto lbl_cleanup;
    }
    stpcpy(metadata->source_file,static_src_file);
    res = parse_pcap(input_file,metadata,index);
    if (UDPFLOW_SUCCESS != res){
        goto lbl_cleanup;
    }
    res = write_to_json_row(output_file,metadata,flag);

    lbl_cleanup:
    metadata_cleanup(&metadata);
    session_data_list_cleanup();
    return res;
}

int parse_pcap(char *arg,struct session_metadata *metadata,unsigned int index) {
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    pcap_t *pcap_handler = NULL;
    char errBuffer[PCAP_ERRBUF_SIZE];
    pcap_handler = read_pcap_file(arg,errBuffer);
    if(NULL == pcap_handler){
        res = UDPFLOW_PCAP_READ_ERROR;
        printf("%s",errBuffer);
        goto lbl_cleanup;
    }

    res = get_all_data_from_session(pcap_handler,metadata, index);

lbl_cleanup:
    pcap_close(pcap_handler);

    return res;
}


int get_all_data_from_session(pcap_t *handler, struct session_metadata *metadata,unsigned int index) {
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    struct ether_header *eth_header = NULL;
    size_t additional_len = 0;
    struct ip *ip_header = NULL;
    struct udphdr *udp_header = NULL;
    struct pcap_pkthdr packet_header;
    const u_char *packet = NULL;
    time_t end_time;
    struct session_data *last_data = NULL;
    packet = pcap_next(handler,&packet_header);
    if (check_vlan(((u_char*) packet))){
        additional_len = VLAN_HLEN;
    }
    res = parse_the_first_packet(&packet_header, (u_char*)packet, metadata,eth_header,ip_header,udp_header,additional_len);
    if(UDPFLOW_SUCCESS != res){
        goto lbl_cleanup;
    }
    while(NULL != packet){
        metadata->total_bytes_transfered += packet_header.caplen;
        metadata->total_packets_transfered++;
        end_time = packet_header.ts.tv_sec;
        extract_payload(packet,last_data,packet_header.caplen);
        packet = pcap_next(handler,&packet_header);
    }
    metadata->session_number = index;
    metadata ->end_time = end_time;
    res = UDPFLOW_SUCCESS;

lbl_cleanup:
    return res;
}

struct session_data *extract_payload(u_char *packet,struct session_data *lastdata,size_t packet_size) {
    const u_char *payload;
    size_t additional_len=0;
    struct session_data *data;
    if(check_vlan(packet)){
        additional_len = 4;
    }
    size_t payload_size = packet_size - (sizeof(struct ether_header)+ sizeof(struct ip) + sizeof(struct udphdr)+additional_len);
    payload = extract_udp_payload(packet,sizeof(struct ether_header)+ sizeof(struct ip) + sizeof(struct udphdr)+additional_len);
    (void)create_session_data(&data,payload_size);
    memcpy(data->packet_payload,payload,payload_size);
    append_new_session_data(data,lastdata);
    return data;
}


int parse_the_first_packet(struct pcap_pkthdr *pcap_header,u_char *packet,struct session_metadata *metadata,
                           struct ether_header *eth_header, struct ip *ip_header, struct udphdr *udp_header,size_t len) {
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    metadata->start_time = pcap_header->ts.tv_sec;
    if(!check_eth(packet)){
        res = UDPFLOW_PARSE_FIRST_PACKET_NO_ETH;
        DEBUG_PRINT("First packet doesn't contain ethernet");
        goto lbl_cleanup;
    }
    eth_header = extract_eth(packet,0);
    sprintf(metadata->src_mac,"%02X:%02X:%02X:%02X:%02X:%02X",eth_header->ether_shost[0],eth_header->ether_shost[1],
            eth_header->ether_shost[2],eth_header->ether_shost[3],eth_header->ether_shost[4],eth_header->ether_shost[5]);
    sprintf(metadata->dst_mac,"%02X:%02X:%02X:%02X:%02X:%02X",eth_header->ether_dhost[0],eth_header->ether_dhost[1],
            eth_header->ether_dhost[2],eth_header->ether_dhost[3],eth_header->ether_dhost[4],eth_header->ether_dhost[5]);

    if(!check_ip(eth_header) && !len){
        res = UDPFLOW_PARSE_FIRST_PACKET_NO_IP;
        DEBUG_PRINT("First packet doesn't contain ip");
        goto lbl_cleanup;
    }

    ip_header = extract_ip(packet, sizeof(struct ether_header)+len);
    inet_ntop(AF_INET,&(ip_header->ip_src),metadata->src_ip,INET_ADDRSTRLEN);
    inet_ntop(AF_INET,&(ip_header->ip_dst),metadata->dst_ip,INET_ADDRSTRLEN);
    if(!check_udp(ip_header)){
        res = UDPFLOW_PARSE_FIRST_PACKET_NO_UDP;
        DEBUG_PRINT("First packet doesn't contain udp");
        goto lbl_cleanup;
    }
    udp_header = extract_udp(packet, sizeof(struct ether_header)+ sizeof(struct ip)+len);
    metadata->src_port = ntohs(udp_header->uh_sport);
    metadata->dst_port = ntohs(udp_header->uh_dport);
    sprintf(metadata->type,"udp");


    res = UDPFLOW_SUCCESS;

lbl_cleanup:
        return res;
}

void convet_time_t(char* buffer,time_t time_b){
    struct tm ts;
    ts = *localtime(&time_b);
    strftime(buffer, 80,"%B, %d %Y %H:%M:%S",&ts);
}

void create_sample(struct session_metadata *meta){
    strcpy(meta->dst_ip,"1.0.0.127");
    strcpy(meta->src_ip,"1.0.0.127");
    strcpy(meta->src_mac,"00:22:33:11:33");
    strcpy(meta->dst_mac,"00:11:22:33:44");
    meta->src_port=23;
    meta->dst_port=33;
    strcpy(meta->start_time,"ffff");
}
