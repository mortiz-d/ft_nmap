/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:20:50 by mortiz-d          #+#    #+#             */
/*   Updated: 2026/01/10 19:08:51 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"


int	ft_flagsize(t_flag *lst)
{
	int	count;

	if (lst != 0)
	{
		count = 1;
		while (lst->next != 0)
		{	
			lst = lst->next;
			count++;
		}
		return (count);
	}
	return (0);
}


void	ft_flagclear(t_flag **lst)
{
	t_flag	*aux;
	t_flag	*aux2;

	aux = *lst;
	while (aux != 0)
	{
		aux2 = aux->next;
		aux->name = 0;
		if (aux->argument && aux->argument->s)
    		free(aux->argument->s);
		free(aux->argument);
		free(aux);
		aux = aux2;
	}
	*lst = 0;
}

t_flag	*ft_flaglast(t_flag *lst)
{
	if (lst != 0)
	{
		while (lst->next != 0)
		{
			lst = lst->next;
		}
		return (lst);
	}
	return (0);
}

void	ft_flagadd_back(t_flag **lst, t_flag *new)
{
	if (*lst != 0)
	{
		new->prev = ft_flaglast(*lst);
		ft_flaglast(*lst)->next = new;
	}
	else
		*lst = new;
}

void	ft_flagadd_front(t_flag **lst, t_flag *new)
{
	if (!lst || !new)
		return;
	if (*lst)
		(*lst)->prev = new;
	new->next = *lst;
	new->prev = NULL;
	*lst = new;
}


t_flag	*ft_flagnew(char * name, enum flag_type type,int min_range,int max_range)
{
	t_flag	*flag;
	t_argument *arg;

	flag = malloc(sizeof(t_flag));
	if (flag == 0)
		return (0);

	arg = malloc(sizeof(t_argument));
	if (arg == 0)
	{
		free(flag);
		return (0);
	}
	flag->name = name;
    flag->type = type;
    flag->min_range = min_range;
    flag->max_range = max_range;
	flag->argument = arg;
	flag->status = 0;
    if (max_range < min_range)
    {
        flag->max_range = min_range;
        flag->min_range = max_range;
    }

	flag->next = NULL;
	flag->prev = NULL;
	return (flag);
}


void visualize_flags(t_flag *flag)
{
	t_flag *aux;

	aux = flag;
	while (aux)
	{
		ft_printf("FLAG %p : %s range : %d - %d status : %d type : %d next %p \n",aux ,(aux)->name, (aux)->min_range,(aux)->max_range, (aux)->status, (aux)->type ,(aux)->next);
		aux = aux->next;
	}
	return;
}

t_flag *set_up_flag(t_flag *flag, char *name, enum flag_type type, int min_range, int max_range)
{
    t_flag *aux;

    aux = ft_flagnew(name, type, min_range, max_range);
	// printf("Adding new %s\n",name);
    if (!aux)
        return flag;

    if (flag == NULL)
        flag = aux;
    else
		ft_flagadd_front(&flag, aux);
        // ft_flagadd_back(&flag, aux);

    return flag;
}


int needs_argument(char *c)
{
	printf ("param : %s requires an argument\n",c);
	return (0);
}

int is_exact_flag_word(char *str, char *cmp)
{
	if (ft_strncmp(str,cmp,ft_strlen(cmp)) == 0 && ft_strlen(str) == ft_strlen(cmp))
		return (1);
	return (0);
} 

t_argument *get_flag_value(t_flag *start_flag, char *c)
{
	t_flag *aux;

	aux = find_flag(start_flag,c);
	if (!aux)
		return (NULL);

	
	return (aux->argument);

}

int check_flag_status(t_flag *start_flag, char *c)
{
	t_flag *aux;

	// ft_printf("find flag?\n");
	aux = find_flag(start_flag,c);
	// ft_printf("found flag\n");
	if (!aux)
		return (0);

	if (aux->status)
		return (1);
	return (0);

}

t_flag *find_flag(t_flag *flag, char *c)
{
	t_flag *aux;

	aux = flag;
	while (aux)
	{
		// ft_printf("Compara %s con %s\n",aux->name,c);
		if (is_exact_flag_word(aux->name,c))
		{
			// ft_printf("Es un match\n");
			return (aux);
		}
		aux = aux->next;
	}
	return NULL;

}

int valid_argument(char *str,t_flag *flag,enum flag_type type ,int max_range, int min_range)
{
	char *aux_c;
    int aux_i;

	if(type == FLAG_BOOLEAN || type == FLAG_INTEGER)
	{
		aux_i = ft_atoi(str);
    	aux_c = ft_itoa(aux_i);

		if (!ft_strncmp(str, aux_c, ft_strlen(str)) == 0)
		{
			free(aux_c);
			printf("param: invalid argument: %s\n",str);
			return (0);
		}
		free(aux_c);
		if (type == FLAG_BOOLEAN)
		{
			if (aux_i > 1 || aux_i < 0 )
				return (0);
		}
		else
		{
			if (aux_i > max_range || aux_i < min_range )
			{
				printf("param: invalid argument: '%s': out of range: %d <= value <= %d\n",str,min_range,max_range);
				return (0);
			}
		}
		flag->argument->i = ft_atoi(str);
		return (1);
	}
	
	if (type == FLAG_CHAR)
	{
		if (ft_strlen(str) == 1)
		{
			flag->argument->c = str[0];
			return (1);
		}
	}

	if (type == FLAG_STRING)
	{
		flag->argument->s = ft_strdup(str);
		return (1);
	}

	if (type == FLAG_NONE)
	{
		printf("param: invalid argument: '%s': flag doesnt require argument\n",str);
		return (0);
	}

	return (0);
}


//Procesado correcto 1 / Error 0
int process_flags(int argc, char **argv, t_flag *flag)
{
	int i = 1;
	t_flag *aux;

	while (i < argc)
	{
		aux = find_flag(flag,argv[i]);
		if (aux)
		{
			if ((i + 1) >= argc && aux->type != FLAG_NONE)
				return needs_argument(argv[i]);
			if (aux->type == FLAG_NONE)
			{
				aux->status = 1;
				argv[i][0] = '\0';
				continue;
			}else
			{
				if (valid_argument(argv[i+1], aux , aux->type, aux->max_range, aux->min_range))
				{
					// ft_printf("Argumento %s validado\n",aux->name);
					aux->status = 1;
					argv[i][0] = '\0';
					argv[i+1][0] = '\0';
					i++;
					// params->hops = ft_atoi(argv[i+1]);
				}
				else
					return 0;
				
			}

		}
		i++;
	}
	// params->flags = *flags;
	return 1;
}
