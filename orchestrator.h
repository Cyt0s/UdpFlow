//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_ORCHESTRATOR_H
#define UDPFLOW_ORCHESTRATOR_H

#include "pcap_processing/data_types/session_data.h"
#include "pcap_processing/data_types/session_metadata.h"
#include "pcap_processing/wrappers/pcap_wrapper.h"
#include "pcap_processing/wrappers/netinet_wrapper.h"
#include "pcap_processing/output_methods/json_output.h"
#include "pcap_processing/output_methods/file_operations.h"
#include <string.h>
#include <arpa/inet.h>

int orchestrate_run(char* output_file , char** input_files ,size_t number_of_input_files,char* static_src_file);

int parse_pcap(char *arg,struct session_metadata *metadata,unsigned int index);

int get_all_data_from_session(pcap_t *handler, struct session_metadata *metadata,unsigned int index);

int parse_the_first_packet(struct pcap_pkthdr *pcap_header,u_char *packet,struct session_metadata *metadata,
                           struct ether_header *eth_header, struct ip *ip_header, struct udphdr *udp_header,size_t len);

struct session_data *extract_payload(u_char *packet,struct session_data *lastdata,size_t packet_size);

udpflow_res_t file_operation(char* input_file ,FILE* output_file,char* static_src_file,int flag,unsigned int index);

void convet_time_t(char* buffer,time_t time_b);

void create_sample(struct session_metadata *meta);



#endif //UDPFLOW_ORCHESTRATOR_H
