#include "../lib/nmap.h"

void *scan_port(void *args){
    //castear args a lo que sea
    //hacer el scan
    return NULL;
}

void main_scan_logic(t_params* args){
    pthread_t thread;

    for(int i = 0; i < args->threads; ++i){
        pthread_create(&thread, NULL, scan_port, &arg);
    }
}
