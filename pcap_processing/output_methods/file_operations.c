//
// Created by root on 1/16/19.
//

#include "file_operations.h"

udpflow_res_t create_file(FILE **fptr, char *file_name){
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    FILE *local_fptr = NULL;
    if(NULL == fptr || NULL == file_name){
        res = UDPFLOW_OUTPUT_CREATEFILE_BAD_PARAMETERS;
        DEBUG_PRINT("Bad parameters to open new file");
        goto lbl_cleanup;
    }

    local_fptr = fopen(file_name,"w");
    if(NULL == local_fptr){
        res = UDDFLOW_OUTPUT_CREATEFILE_COULDENT_CREATE_FILE;
        DEBUG_PRINT("Couldn't create the file");
        goto lbl_cleanup;
    }

    *fptr = local_fptr;
    res = UDPFLOW_SUCCESS;
    lbl_cleanup:
    return res;
}

void write_packet_hex(FILE *fptr, u_char* packet, size_t buffer_size){
    int i=0;
    u_char a;
    for(i;i<buffer_size;i++){
        a = *(packet+i);
        fprintf(fptr,"%x",a);
    }
}

void write_packet_utf8(FILE *fptr, u_char* packet, size_t buffer_size){
    int i=0;
    u_char a;
    for(i;i<buffer_size;i++){
        a = *(packet+i);
        if(0x0a == a)
            fprintf(fptr,"\\n");
        else if(0x22 == a)
            fprintf(fptr,"\\\"");
        else if(0x09 == a)
            fprintf(fptr,"\\t");
        else if(0x08 == a)
            fprintf(fptr,"\\b");
        else if(0x0d == a)
            fprintf(fptr,"\\r");
        else if(0x5c == a)
            fprintf(fptr,"\\\\");
        else if(0x0c == a)
            fprintf(fptr,"\\f");
        else
            fprintf(fptr,"%c",a);
    }
}