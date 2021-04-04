//
// Created by root on 12/30/18.
//

#include "netinet_wrapper.h"

int check_eth(u_char *packet){
    return 1;
}
struct ether_header *extract_eth(u_char *packet, int len){
   struct ether_header *eth;
   eth = (struct ether_header*)(packet + len);
   return eth;
}

int check_vlan(u_char *packet){
    struct ether_header *eth;
    eth = (struct ether_header*)(packet);
    if( ETHERTYPE_VLAN !=ntohs(eth->ether_type))
        return 0;
    return 1;
}

int check_ip(struct ether_header* eth){
    if( ETHERTYPE_IP != ntohs(eth->ether_type))
        return 0;
    return 1;
}

struct ip *extract_ip(u_char *packet, int len){
    struct ip * ip_header;
    ip_header = (struct ip*)(packet + len);
    return ip_header;
}
int check_udp(struct ip *ip_header){
    if(IPPROTO_UDP != ip_header->ip_p)
        return 0;
    return 1;
}

struct udphdr *extract_udp(u_char *packet, int len){
    struct udphdr *udp_header;
    udp_header = (struct udphdr*)(packet + len);
    return udp_header;
}

u_char *extract_udp_payload(u_char *packet, int len){
    u_char *payload;
    payload = packet + len;
    return payload;
}
