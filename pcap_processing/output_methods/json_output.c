//
// Created by root on 1/9/19.
//

#include "json_output.h"

udpflow_res_t write_sperator_json_file(FILE *fptr){
    fputc(',',fptr);
    fputc('\n',fptr);
    return UDPFLOW_SUCCESS;
}

udpflow_res_t write_openning_json_file(FILE *fptr){
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    if( NULL == fptr){
        res = UDPFLOW_OUTPUT_JSON_OPENING_WRITE_BAD_PARAMETERS;
        goto lbl_cleanup;
    }

    fputc('[',fptr);
    fputc('\n',fptr);

    res = UDPFLOW_SUCCESS;
    lbl_cleanup:
    return res;
}

udpflow_res_t write_ending_json_file(FILE *fptr){
    udpflow_res_t res =UDPFLOW_UNINITIALIZED;
    if( NULL == fptr){
        res = UDPFLOW_OUTPUT_JSON_CLOSING_WRITE_BAD_PARAMETERS;
        goto lbl_cleanup;
    }

    fputc(']',fptr);
    fputc('\n',fptr);

    res=UDPFLOW_SUCCESS;
    lbl_cleanup:
    return res;
}

udpflow_res_t write_to_json_row(FILE *fptr, struct session_metadata *metadata,int flag){
    udpflow_res_t  res = UDPFLOW_UNINITIALIZED;
    struct session_data *data;


    if( NULL == metadata || LIST_EMPTY(&session_unified_data ) || NULL == fptr){
        res = UDPFLOW_OUTPUT_BAD_PARAMETERS;
        DEBUG_PRINT("Bad parameters for output method");
        goto lbl_cleanup;
    }
    if(flag){
        write_sperator_json_file(fptr);
    }
    fputc('{',fptr);
    fputc('\n',fptr);
    fprintf(fptr,"\"source_file\":\"%s\",\n\"protocol\":\"%s\",\n\"src_mac\":\"%s\",\n\"dst_mac\":\"%s\",\n\"start_time\":\"%ld\",\n\"end_time\":\"%ld\",\n\"size\":\"%zu\",\n\"packets\":\"%d\",\n"
                 "\"ip_a\":\"%s\",\n\"ip_b\":\"%s\",\n\"session_number\":\"%d\",\n\"port_a\":\"%d\",\n\"port_b\":\"%d\",\n"
            ,metadata->source_file,metadata->type,metadata->src_mac,metadata->dst_mac,metadata->start_time,metadata->end_time,metadata->total_bytes_transfered,
            metadata->total_packets_transfered,metadata->src_ip,metadata->dst_ip,metadata->session_number,metadata->src_port,metadata->dst_port);
    fprintf(fptr,"\"data\":\"");
    LIST_FOREACH(data,&session_unified_data,pointers){
        write_packet_utf8(fptr,data->packet_payload,data->payload_size);
    }
    fputc('\"',fptr);
    fputc('\n',fptr);
    //values
    fputc('}',fptr);

    res = UDPFLOW_SUCCESS;
    lbl_cleanup:
    return res;
}





