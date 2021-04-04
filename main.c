
#include "common.h"
#include "orchestrator.h"
#include <dirent.h>

int get_dir_count(char** argv);

void get_list_all_files_in_dirs(char** files, char** argv);

udpflow_res_t create_list_of_input_pcaps(char *** files,size_t number_of_input);

void copy_argv_to_input_pcaps(char** files, char** argv, size_t number_of_input);

void cleanup_list_of_input_pcaps(char** files, size_t number_of_input);

int main(int argc,char *argv[]) {
    int program_status = EXIT_FAILURE;
    udpflow_res_t res = UDPFLOW_UNINITIALIZED;
    char ** files = NULL;
    int number_of_files=0;

    if(4 > argc){
        res = UDPFLOW_FAILURE;
        printf("Invalid args to the program,should be: [dest json file path] [src_file name that will be in the json as \"src_file\"] [*paths of all files]\n");
        printf("Example: ./UdpFlow /home/cytos/Desktop/json_name.json /home/cytos/Desktop/src_file /home/cytos/Desktop/dir_with_pcaps\n");
        goto lbl_cleanup;
    }
    number_of_files = get_dir_count(argv);
    res = create_list_of_input_pcaps(&files,(size_t)number_of_files);

    if(UDPFLOW_SUCCESS != res){
        res = UDPFLOW_FAILURE;
        goto lbl_cleanup;
    }

    get_list_all_files_in_dirs(files,argv);
    //copy_argv_to_input_pcaps(files,argv,(size_t)number_of_files);

    res = orchestrate_run(argv[1],files,(size_t)number_of_files,argv[2]);
    if(UDPFLOW_SUCCESS != res){
        res = UDPFLOW_FAILURE;
        goto lbl_cleanup;
    }

lbl_cleanup:
    if(NULL != files){
       cleanup_list_of_input_pcaps(files,(size_t)number_of_files);
    }
    if (UDPFLOW_SUCCESS == res) {
        program_status = EXIT_SUCCESS;
    }

    return program_status;
}


udpflow_res_t create_list_of_input_pcaps(char *** files,size_t number_of_input) {
    int i = 0;
    char** files_local = NULL;
    files_local = (char**)malloc(number_of_input * sizeof(char*));
    for(i;i<number_of_input;i++){
        files_local[i] = (char*)malloc(1024 * sizeof(char));
    }
    *files = files_local;
    return UDPFLOW_SUCCESS;
}

void copy_argv_to_input_pcaps(char** files, char** argv, size_t number_of_input) {
    for (int i = 0; i < number_of_input; ++i) {
        strcpy(files[i],argv[i+3]);
    }
}

int get_dir_count(char** argv){
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(argv[3]); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

void get_list_all_files_in_dirs(char** files, char** argv){
    int i = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(argv[3]);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG){
                strcpy(files[i],argv[3]);
                strcat(files[i],"/");
                strcat(files[i],dir->d_name);
                i++;
            }
        }
        closedir(d);
    }
}

void cleanup_list_of_input_pcaps(char** files, size_t number_of_input) {
    int i = 0;
    for(i;i < number_of_input; i++){
        FREE(files[i]);
    }
    FREE(files);
}