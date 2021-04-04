//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_NETINET_WRAPPER_H
#define UDPFLOW_NETINET_WRAPPER_H
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "../data_types/vlan.h"

int check_eth(u_char *packet);
struct ether_header *extract_eth(u_char *packet, int len);
int check_vlan(u_char *packet);
int check_ip(struct ether_header* eth);
struct ip *extract_ip(u_char *packet, int len);
int check_udp(struct ip *ip_header);
struct udphdr *extract_udp(u_char *packet, int len);
u_char *extract_udp_payload(u_char *packet, int len);


#endif //UDPFLOW_NETINET_WRAPPER_H
