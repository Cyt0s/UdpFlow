//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_SESSION_METADATA_H
#define UDPFLOW_SESSION_METADATA_H

#include "../../common.h"

struct session_metadata{
    __time_t start_time;
    __time_t end_time;
    size_t total_bytes_transfered;
    unsigned int total_packets_transfered;
    char src_mac[20];
    char dst_mac[20];
    char type[20];
    char src_ip[20];
    char dst_ip[20];
    char source_file[1024];
    unsigned int session_number;
    unsigned int src_port;
    unsigned int dst_port;
};

void metadata_cleanup(struct session_metadata** metadata);

udpflow_res_t create_session_metadata(struct session_metadata **metadata);

void sort_ips_and_ports(struct session_metadata *metadata);

long get_numeric_number_out_of_ip(char* ip);

#endif //UDPFLOW_SESSION_METADATA_H
