#include "../lib/nmap.h"

t_params *params_default_config (void)
{
    t_params *param = ft_calloc(sizeof(t_params), 1);
    param->threads = 1;
    param->destination = NULL;
    param->scan = NULL;
    param->ip_list = NULL;
    param->ports = NULL;
    return param;
}

void free_params(t_params *param)
{

    if (param->destination)
        free (param->destination);
    if (param->scan)
    {
        ft_lstiter(*param->scan,free);
        ft_lstclear(param->scan);
        free (param->scan);
    }
    if (param->ip_list)
    {
        ft_lstiter(*param->ip_list,free);
        ft_lstclear(param->ip_list);
        free (param->ip_list);
    }
    if (param->ports)
    {
        ft_lstiter(*param->ports,free);
        ft_lstclear(param->ports);
        free (param->ports);
    }
    free(param);
    return;
}

static bool check_integer(char *str)
{
    char *aux_c;
    int aux_i;

    aux_i = ft_atoi(str);
    aux_c = ft_itoa(aux_i);
    if (!ft_strncmp(str, aux_c, ft_strlen(str)) == 0)
	{
		free(aux_c);
		return (false);
	}
    free(aux_c);
    return true;
}

static t_port* create_port (char *str)
{
    t_port  *ret;
    char    **ports = NULL;
    bool    create = false;
    int     count = 0;

    ret = ft_calloc(1,sizeof(t_port));
    if (ft_strchr(str,'-') != NULL)
    {
        ret->range = true;
        ports = ft_split(str,'-');
        while (ports[count])
            count++;
                
        if (count == 2)
        {
            if (check_integer(ports[0]) && check_integer(ports[1]))
            {
                create = true;
                ret->port_min = ft_atoi(ports[0]);
                ret->port_max = ft_atoi(ports[1]);
            }
        }
    }
    else
    {
        if (check_integer(str))
        {
            ret->port_nbr = ft_atoi(str);
            create = true;
        }
    }

    if (ports)
    {
        for(int i = 0; ports[i] != NULL; i++)
            free(ports[i]);
        free(ports);
    }

    if(!create)
    {
        free(ret);
        return NULL;
    }
    return ret;
}

static t_list **extract_ports(char* str)
{
    t_list **lst_ports = ft_calloc(1,sizeof(t_list*));
    char **ports = NULL ;
    t_port *aux = NULL;
    

    printf("PUERTOS %s\n",str);
    ports = ft_split(str,',');
    for(int i = 0; ports[i] != NULL; i++)
    {
        aux = create_port(ports[i]);
        if (aux)
            ft_lstadd_back(lst_ports,ft_lstnew(aux));
        else
        {
            ft_lstclear(lst_ports);
            free(lst_ports);
            lst_ports = NULL;
            break;
        }
    }

    for(int i = 0; ports[i] != NULL; i++)
        free(ports[i]);
    free(ports);

    return lst_ports;

}

t_list **default_ports(void)
{
    t_list **lst_ports = ft_calloc(1,sizeof(t_list*));
    t_port *aux;

    aux = create_port("0-1023");
    if (aux)
        ft_lstadd_back(lst_ports,ft_lstnew(aux));

    return lst_ports;
}

static t_list **extract_ip_lists(char *filename)
{
    t_list **lst_ips = ft_calloc(1,sizeof(t_list*));
    char *aux;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return NULL;
    aux = get_next_line(fd);    
    while (aux)
    {
        ft_lstadd_back(lst_ips,ft_lstnew(aux));
        aux = get_next_line(fd);
    }
    close(fd);
    return lst_ips;

}

static t_scan get_scan(char *str)
{
    const char *scan_names[] = {"SYN","NUL","FIN","XMAS","ACK","UDP"};
    int num_scans = sizeof(scan_names) / sizeof(scan_names[0]);

    for (int i = 0; i < num_scans; i++)
    {
        if (strcmp(str, scan_names[i]) == 0)
            return (t_scan)i;
    }

    return SCAN_UNKNOWN;
}

static t_list **extract_scan(char *str)
{
    t_list **lst_scans = ft_calloc(1,sizeof(t_list*));
    char **scans = NULL;
    t_scan *new_aux;
    t_scan aux;
    
    scans = ft_split(str,'/');
    for(int i = 0; scans[i] != NULL; i++)
    {
        aux = get_scan(scans[i]);
        new_aux = malloc(sizeof(t_scan));
        if (!new_aux)
            return NULL;
        *new_aux = aux;
        ft_lstadd_back(lst_scans, ft_lstnew(new_aux));
    }

    for(int i = 0; scans[i] != NULL; i++)
        free(scans[i]);
    free(scans);

    return lst_scans;
}

//fucntions related to each flag
void apply_help(t_flag *flag, t_params *params)
{
    (void)flag;
    params->help = true;
}

void apply_ports(t_flag *flag, t_params *params)
{
    params->ports = extract_ports(flag->value.str_value);
}

void not_apply_ports(t_flag *flag, t_params *params)
{
    (void)flag;
    params->ports = default_ports();
}

void apply_ip(t_flag *flag, t_params *params)
{
    params->destination = ft_strdup(flag->value.str_value);
}

void apply_file(t_flag *flag, t_params *params)
{
    params->ip_list = extract_ip_lists(flag->value.str_value);
}

void apply_speedup(t_flag *flag, t_params *params)
{
    params->threads = flag->value.int_value;
}

void apply_scan(t_flag *flag, t_params *params)
{
    params->scan = extract_scan(flag->value.str_value);
}


t_list *flags_config (void)
{
    t_list *flag = NULL;
    flag = set_up_flag(flag, "--help", FLAG_NONE, apply_help,NULL);
    flag = set_up_flag(flag, "--ports", FLAG_STRING, apply_ports,not_apply_ports);
    flag = set_up_flag(flag, "--ip", FLAG_STRING, apply_ip,NULL);
    flag = set_up_flag(flag, "--speedup", FLAG_INTEGER, apply_speedup,NULL);
    flag = set_up_flag(flag,"--file",FLAG_STRING,apply_file,NULL);
    flag = set_up_flag(flag,"--scan",FLAG_STRING,apply_scan,NULL);
    return flag;
}


