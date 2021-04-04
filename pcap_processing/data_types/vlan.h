//
// Created by root on 7/22/20.
//

#ifndef UDPFLOW_V_VLAN_H
#define UDPFLOW_V_VLAN_H

#define VLAN_HLEN 4

struct vlan_hdr{
    unsigned short TCI;
    unsigned short encap;
};

#endif //UDPFLOW_V_VLAN_H
