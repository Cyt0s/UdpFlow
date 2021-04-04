//
// Created by root on 1/9/19.
//

#ifndef UDPFLOW_JSON_OUTPUT_H
#define UDPFLOW_JSON_OUTPUT_H


#include "../../common.h"
#include "sys/queue.h"
#include "../data_types/session_metadata.h"
#include "../data_types/session_data.h"
#include "file_operations.h"

udpflow_res_t write_openning_json_file(FILE *fptr);

udpflow_res_t write_sperator_json_file(FILE *fptr);

udpflow_res_t write_ending_json_file(FILE *fptr);

udpflow_res_t write_to_json_row(FILE *fptr, struct session_metadata *metadata,int flag);

#endif //UDPFLOW_JSON_OUTPUT_H
