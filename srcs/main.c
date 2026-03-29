//#include <studio.h>
#include "../lib/nmap.h"

int main(int argc, char **argv) 
{  
    (void) argv;
    t_params *params;
    int i;

    if (argc < 2)
    {
        printf (PING_USSAGE_ERROR);
        return 0;
    }
    
    params = get_params(argc,argv);
    debug_params(params);
    
    // params->ttl = DEFAULT_TTL;
    // params->timeout_ms = DEFAULT_TIMEOUT;
    // params->payload_size = PAYLOAD_SIZE_DEFAULT;
    // params->interval = DEFAULT_INTERVAL_PING;
    // params->flags = calloc(1,sizeof(active_flags));

    // if (ping_check_flags(argc, argv, params->flags, params) == 0)
    // {
    //     free(params);
    //     return (0);
    // }
    
    // signal(SIGINT, handle_sigint);
    i = 1;
    while (i < argc)
    {
        printf("%i - %s\n",i,argv[i]);
        i++;
    }

    //
    free(params);
    return 0;
}