//
// Created by root on 12/30/18.
//


#include "session_data.h"


udpflow_res_t create_session_data(struct session_data **session,size_t size_of_payload){
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    struct session_data *local_session = NULL;
    if(NULL == session){
        res = UDPFLOW_DATA_BAD_PARMATERS;
        DEBUG_PRINT("Bad parameter at creating session_data");
        goto lbl_cleanup;
    }

    local_session = (struct session_data *)malloc(sizeof(*local_session));
    if(NULL == local_session){
        res = UDPFLOW_DATA_INIT_MALLOC_FAILURE_ALLOCATE_OBJECT;
        DEBUG_PRINT("Bad parameter at allocating new session_data");
        goto lbl_cleanup;
    }

    local_session->packet_payload = (u_char*)malloc(sizeof(u_char)*(size_of_payload+1));
    if(NULL == local_session->packet_payload){
        res = UDPFLOW_DATA_INIT_MALLOC_FAILURE_ALLOCATE_BUFFER;
        DEBUG_PRINT("Bad parameter at allocating new buffer at session_data");
        goto lbl_cleanup;
    }
    local_session->payload_size= size_of_payload ;
    *session = local_session;
    res = UDPFLOW_SUCCESS;
lbl_cleanup:
    if (res != UDPFLOW_SUCCESS){
        if(NULL != local_session->packet_payload){
            FREE(local_session->packet_payload)
        }
        FREE(local_session);
    }

    return res;
}

void session_data_cleanup(struct session_data **session){
    if(NULL == session || NULL == *session){
        return;
    }
    FREE((*session)->packet_payload);
    FREE(*session);
}

void append_new_session_data(struct session_data *newblock,struct session_data *lastblock){
    if(NULL == lastblock)
        LIST_INSERT_HEAD(&session_unified_data,newblock,pointers);
    else
        LIST_INSERT_AFTER(lastblock,newblock,pointers);
}

void session_data_list_cleanup(){
    struct session_data *n1 = LIST_FIRST(&session_unified_data),*n2 = NULL;
    while (NULL != n1) {
        n2 = LIST_NEXT(n1,pointers);
        session_data_cleanup(&n1);
        n1 = n2;
    }
    LIST_INIT(&session_unified_data);
}