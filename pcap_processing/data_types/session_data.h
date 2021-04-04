//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_SESSION_DATA_H
#define UDPFLOW_SESSION_DATA_H

#include "../../errors.h"
#include "../../common.h"
#include <sys/queue.h>
struct session_data{
    size_t payload_size;
    u_char *packet_payload;
    LIST_ENTRY(session_data) pointers;
};

LIST_HEAD(sessiondatalist,session_data) session_unified_data;

//Session data operation
udpflow_res_t create_session_data(struct session_data **session, size_t size_of_payload);

void session_data_cleanup(struct session_data **session);

//List operations:

void append_new_session_data(struct session_data *newblock,struct session_data *lastblock);

void session_data_list_cleanup();

#endif //UDPFLOW_SESSION_DATA_H
