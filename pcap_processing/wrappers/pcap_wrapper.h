//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_PCAP_WRAPPER_H
#define UDPFLOW_PCAP_WRAPPER_H

#include <pcap.h>

pcap_t *read_pcap_file(char *filename ,char* errBuffer);

#endif //UDPFLOW_PCAP_WRAPPER_H
