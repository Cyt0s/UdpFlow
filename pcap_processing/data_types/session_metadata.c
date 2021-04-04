//
// Created by root on 12/30/18.
//

#include "session_metadata.h"


udpflow_res_t create_session_metadata(struct session_metadata **metadata){
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    struct session_metadata *local_metadata = NULL;

    /* Parameter checks. */
    if (NULL == metadata) {
        res = UDPFLOW_METADATA_BAD_PARMATERS;
        DEBUG_PRINT("Bad parameters.");
        goto lbl_cleanup;
    }

    /* Allocate the test object. */
    local_metadata = (struct session_metadata *)malloc(sizeof(*local_metadata));
    if (NULL == local_metadata) {
        res = UDPFLOW_METADATA_INIT_MALLOC_FAILURE_ALLOCATE_OBJECT;
        DEBUG_PRINT("malloc() failed when allocating %lu bytes",
                    sizeof(*local_metadata));
        goto lbl_cleanup;
    }

    local_metadata->total_bytes_transfered = 0;
    local_metadata->total_packets_transfered = 0;
    /* Transfer ownership. */
    *metadata = local_metadata;


    /* Indicate success. */
    res = UDPFLOW_SUCCESS;

    lbl_cleanup:
    /* Free everything in case of failure. */
    if (UDPFLOW_SUCCESS != res) {
        FREE(*metadata)
    }

    return res;
}

void metadata_cleanup(struct session_metadata** metadata){
    if(NULL == metadata || NULL == *metadata){
        return;
    }
    FREE(*metadata);
}

void sort_ips_and_ports(struct session_metadata *metadata){
    char ip_src[20],ip_dst[20];
    int unsigned src_port = metadata->src_port,dst_port = metadata->dst_port;
    strcpy(ip_src,metadata->src_ip);
    strcpy(ip_dst,metadata->dst_ip);
    if(get_numeric_number_out_of_ip(ip_src) > get_numeric_number_out_of_ip(ip_dst)){
        strcpy(metadata->src_ip,ip_src);
        strcpy(metadata->dst_ip,ip_dst);
        metadata->dst_port = dst_port;
        metadata->src_port = src_port;
    }
    else{
        strcpy(metadata->src_ip,ip_dst);
        strcpy(metadata->dst_ip, ip_src);
        metadata->dst_port = src_port;
        metadata->src_port = dst_port;
    }
}

long get_numeric_number_out_of_ip(char* ip){
    long numeric_ip;
    char ip_without_periods[20];
    int i=0,numeric_ip_i=0;
    for (i;i>strlen(ip);i++){
        if(isdigit(*(ip+i)))
        {
            ip_without_periods[numeric_ip_i] = ip[i];
            numeric_ip_i++;
        }
    }
    numeric_ip = strtol(ip_without_periods,NULL,10);
    return numeric_ip;
}
