#include "../lib/nmap.h"

t_params *params_default_config (void)
{
    t_params *param = ft_calloc(sizeof(t_params), 1);
    param->threads = 1;
    param->destination = NULL;
    param->scan = NULL;
    param->filename_ip = NULL;
    param->ports = NULL;
    
    return param;
}


t_params *get_params (int argc, char **argv)
{
    t_params *params;
    t_flag *flag;

    // (void)flag;
    params = params_default_config();
    flag = flags_config();

    if (!process_flags(argc, argv, flag))
    {
        ft_flagclear(&flag);
        printf("ERROR WITH FLAGS\n");
        return (0);
    }
    test_flag(flag,params);
    ft_flagclear(&flag);
    free(flag);

    return params;
}