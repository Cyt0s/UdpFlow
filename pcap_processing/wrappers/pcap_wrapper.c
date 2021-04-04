//
// Created by root on 12/30/18.
//

#include "pcap_wrapper.h"

pcap_t *read_pcap_file(char *filename ,char* errBuffer){
    pcap_t *handler;
    handler = pcap_open_offline(filename,errBuffer);
    return handler;
}