//#include <studio.h>
#include "../lib/nmap.h"

void free_all(t_list *flags, t_params *params)
{
    // ft_lstiter(flags,print_flag);
    free_flags(&flags);
    free_params(params);
}

int main(int argc, char **argv) 
{
    t_list *flags = NULL;
    t_params *params = NULL;
    // t_list *aux = NULL;

    params =  params_default_config();
    flags = flags_config();

    if (!process_flags(argc, argv, flags,params))
    {
        free_all(flags, params);
        return 0;
    }
    debug_params(params);

    free_all(flags, params);
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '\0')
        {
            i++;
            continue;
        }
        
        printf("argv->%s\n",argv[i]);
        //TODO:Cambiar la ip de argv a param->ip_list
        printf("ip->%s\n",dns_lookup(argv[i]));
        // int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        // if (sockfd < 0) {
        //     fprintf(stderr, PING_ROOT_ERROR);
        //     free(params);
        //     return 1;
        // }
        

    }
    
    
    return 0;
}