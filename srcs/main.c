//#include <studio.h>
#include "../lib/nmap.h"

int free_all(t_list *flags, t_params *params)
{
    // ft_lstiter(flags,print_flag);
    free_flags(&flags);
    free_params(params);
    return 1;
}

int main(int argc, char **argv) 
{
    t_list *flags = NULL;
    t_params *params = NULL;
    t_list *ip_selected = NULL;
    char *aux;

    params =  params_default_config();
    flags = flags_config();

    if (!process_flags(argc, argv, flags,params))
    {
        return free_all(flags, params);
    }
    debug_params(params);

    if (params->ip_list == NULL)
    {
        printf("Error :No IPs where given to nmap\n");
        return free_all(flags, params);
    }
    ip_selected = *params->ip_list;
    while (ip_selected)
    {

        printf("argv->%s\n", (char *)ip_selected->content);
        aux = dns_lookup((char *)ip_selected->content);
        printf("ip->%s\n",aux);
        nmap(params,aux);
        free(aux);
        ip_selected = ip_selected->next;
    }

    // for (int i = 1; i < argc; i++)
    // {
    //     if (argv[i][0] == '\0')
    //     {
    //         i++;
    //         continue;
    //     }
        
    //     printf("argv->%s\n",argv[i]);
    //     //TODO:Cambiar la ip de argv a param->ip_list
    //     
    //     // int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    //     // if (sockfd < 0) {
    //     //     fprintf(stderr, PING_ROOT_ERROR);
    //     //     free(params);
    //     //     return 1;
    //     // }
        

    // }
    
    return free_all(flags, params);
}