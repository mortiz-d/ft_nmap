#include "../lib/nmap.h"



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

static int add_port_range(t_list **lst, char* str )
{
    t_port *aux;
    char    **ports = NULL;
    int     count = 0;
    int     min, max, created;

    created = 0;
    ports = ft_split(str,'-');
    while (ports[count])
        count++;
    created = 0;     
    if (count == 2)
    {
        if (check_integer(ports[0]) && check_integer(ports[1]))
        {
            min = ft_atoi(ports[0]);
            max = ft_atoi(ports[1]);

            if (min < max)
            {
                for (int p = min; p <= max; p++)
                {
                    aux = ft_calloc(1, sizeof(t_port));
                    aux->port_nbr = p;
                    ft_lstadd_back(lst, ft_lstnew(aux));
                }
                created=1;
            }
            else
                printf("Error: Port range %s - %s is backwards are you sure?\n",ports[0],ports[1]);
        }
        else
            printf("Error: Cannot determine port range %s - %s\n",ports[0],ports[1]);
    }
    else
        printf("Error: Cannot determine port range '%s'\n",str);

    if (ports)
    {
        for(int i = 0; ports[i] != NULL; i++)
            free(ports[i]);
        free(ports);
    }

    return created;
}


static int create_port (t_list **lst,char *str)
{
    t_port  *aux;

    aux = ft_calloc(1,sizeof(t_port));
    
    if (!check_integer(str))
    {
        printf("Error: '%s' is not a number\n",str);
        free(aux);
        return 0;
    }
    aux->port_nbr = ft_atoi(str);
    if (aux->port_nbr < MIN_PORT_RANGE || aux->port_nbr > MAX_PORT_RANGE)
    {
        printf("Error: '%s' needs to be between port ranges %i - %i \n",str, MIN_PORT_RANGE, MAX_PORT_RANGE);
        free(aux);
        return 0;
    }

    ft_lstadd_back(lst,ft_lstnew(aux));
    return 1;

}

static int extract_ports(t_params *params, char* str)
{
    t_list **lst_ports = ft_calloc(1,sizeof(t_list*));
    char **ports = NULL;
    int error = 0;

    ports = ft_split(str,',');
    for(int i = 0; ports[i] != NULL && error == 0; i++)
    {
        if (ft_strchr(ports[i],'-'))
        {
            if (!add_port_range(lst_ports,ports[i]))
            {
                error = 1;
                break;
            }
        }
        else
        {
            if (!create_port(lst_ports,ports[i]))
            {
                error = 1;
                break;
            }
        }
    }
    params->ports = lst_ports;
    params->n_ports = ft_lstsize(*lst_ports);
    
    for(int i = 0; ports[i] != NULL; i++)
        free(ports[i]);
    free(ports);

    return !error;

}

static t_list *extract_ip_lists(char *filename)
{
    t_list *lst_ips = NULL;
    char *aux,*aux2;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return NULL;
    aux = get_next_line(fd);    
    while (aux)
    {
        aux2 = ft_strtrim(aux, " \t\r\n");
        free(aux);
        ft_lstadd_back(&lst_ips,ft_lstnew(aux2));
        aux = get_next_line(fd);
    }
    close(fd);
    return lst_ips;

}


static t_list *extract_ip(char *ip)
{
    printf("LEEO IP\n");
    if (!ip)
        return NULL;
    return ft_lstnew(ft_strdup(ip));
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
int apply_help(t_flag *flag, t_params *params)
{
    (void)flag;
    params->help = true;
    return 1;
}

int apply_ports(t_flag *flag, t_params *params)
{
    if (params->ports)
    {
        ft_lstiter(*params->ports,free);
        ft_lstclear(params->ports);
        free (params->ports);
    }

    if (!extract_ports(params,flag->value.str_value))
        return 0;
    return 1;
}


int apply_ip(t_flag *flag, t_params *params)
{
    if (!params->ip_list)
        params->ip_list = ft_calloc(1,sizeof(t_list*));
    ft_lstadd_back(params->ip_list ,extract_ip(flag->value.str_value));//  ft_strdup(flag->value.str_value);
    return 1;
}

int apply_file(t_flag *flag, t_params *params)
{
    if (!params->ip_list)
        params->ip_list = ft_calloc(1,sizeof(t_list*));
    ft_lstadd_back(params->ip_list ,extract_ip_lists(flag->value.str_value));
    return 1;
}

int apply_speedup(t_flag *flag, t_params *params)
{
    params->threads = flag->value.int_value;
    return 1;
}

int apply_scan(t_flag *flag, t_params *params)
{
    params->scan = extract_scan(flag->value.str_value);
    return 1;
}


t_list *flags_config (void)
{
    t_list *flag = NULL;
    flag = set_up_flag(flag, "--help", FLAG_NONE, apply_help,NULL);
    flag = set_up_flag(flag, "--ports", FLAG_STRING, apply_ports,NULL);
    flag = set_up_flag(flag, "--ip", FLAG_STRING, apply_ip,NULL);
    flag = set_up_flag(flag, "--speedup", FLAG_INTEGER, apply_speedup,NULL);
    flag = set_up_flag(flag,"--file",FLAG_STRING,apply_file,NULL);
    flag = set_up_flag(flag,"--scan",FLAG_STRING,apply_scan,NULL);
    return flag;
}

t_params *params_default_config (void)
{
    t_params *param = ft_calloc(sizeof(t_params), 1);
    param->threads = 1;
    param->destination = NULL;
    param->scan = NULL;
    param->ip_list = NULL;
    extract_ports(param,"0-1023");
    return param;
}
