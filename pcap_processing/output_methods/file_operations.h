//
// Created by root on 1/16/19.
//

#ifndef UDPFLOW_FILE_OPERATIONS_H
#define UDPFLOW_FILE_OPERATIONS_H

#include "../../common.h"
#include "../../errors.h"


udpflow_res_t create_file(FILE **fptr, char *file_name);

void write_packet_hex(FILE *fptr, u_char* packet, size_t buffer_size);

void write_packet_utf8(FILE *fptr, u_char* packet, size_t buffer_size);

#endif //UDPFLOW_FILE_OPERATIONS_H
