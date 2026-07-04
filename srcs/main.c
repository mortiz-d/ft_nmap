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

    params =  params_default_config();
    flags = flags_config();

    if (!process_flags(argc, argv, flags,params))
    {
        // printf("?\n");
        return free_all(flags, params);
    }

    generate_result_table(params);
    debug_params(params);
    if (params->help == 1)
    {
        // printf("?!\n");
        return free_all(flags, params);
    }


    if (params->ip_list == NULL)
    {
        printf("Error :No IPs where given to nmap\n");
        return free_all(flags, params);
    }
    main_scan_logic(params);

    print_result_table(params);
    return free_all(flags, params);
}