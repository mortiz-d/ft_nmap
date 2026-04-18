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
#include "../../lib/flag.h"

void free_flag(void *content)
{
    t_flag *f;

    if (!content)
        return;

    f = (t_flag *)content;

    if (f->name)
        free(f->name);

    if (f->type == FLAG_STRING && f->value.str_value)
        free(f->value.str_value);

    free(f);
}

void free_flags(t_list **flags)
{
    if (!flags || !*flags)
        return;

    ft_lstiter(*flags, free_flag);
    ft_lstclear(flags);
}


t_flag *ft_flagnew(char *name, enum flag_type type, t_flag_apply apply,t_flag_not_apply not_apply, va_list args)
{
    t_flag *flag;

    flag = ft_calloc(sizeof(t_flag),1);
    if (!flag)
        return NULL;

    flag->name = ft_strdup(name);
    if (!flag->name)
    {
        free(flag);
        return NULL;
    }

    flag->type = type;
    flag->active = 0;

    flag->apply = apply;
    flag->not_apply = not_apply;

    flag->value.int_value = 0;

    if (type == FLAG_INTEGER)
        flag->value.int_value = va_arg(args, int);

    else if (type == FLAG_RANGE_INTEGER)
    {
        flag->value.range.min = va_arg(args, int);
        flag->value.range.max = va_arg(args, int);
    }

    else if (type == FLAG_STRING)
        flag->value.str_value = NULL;

    else if (type == FLAG_CHAR)
        flag->value.char_value = 0;

    else if (type == FLAG_BOOLEAN)
        flag->value.bool_value = 0;

    return flag;
}


t_list *set_up_flag(t_list *flag, char *name,enum flag_type type, t_flag_apply apply,t_flag_not_apply not_apply,...)
{
    t_list  *node;
    t_flag  *new_flag;
    va_list args;

    // printf("Creating %s\n", name);

    va_start(args, not_apply);
    new_flag = ft_flagnew(name, type, apply, not_apply, args);
    va_end(args);

    if (!new_flag)
        return flag;
    // printf("lstnew %s\n", name);
    node = ft_lstnew(new_flag);
    if (!node)
        return flag;
    // printf("adding to front %s\n", name);
    ft_lstadd_front(&flag, node);

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

t_list *find_flag(t_list *flag, char *c)
{
	t_list *aux;
    t_flag *f;

	aux = flag;
	while (aux)
	{
		f = (t_flag *)aux->content;

		if (is_exact_flag_word(f->name, c))
			return aux;

		aux = aux->next;
	}
	return NULL;
}

int valid_argument(char *str, t_flag *flag, enum flag_type type, int max_range, int min_range)
{
	char *aux_c;
	int aux_i;

	if (type == FLAG_BOOLEAN || type == FLAG_INTEGER)
	{
		aux_i = ft_atoi(str);
		aux_c = ft_itoa(aux_i);

		if (ft_strncmp(str, aux_c, ft_strlen(str)) != 0)
		{
			free(aux_c);
			printf("param: invalid argument: %s\n", str);
			return (0);
		}
		free(aux_c);

		if (type == FLAG_BOOLEAN)
		{
			if (aux_i > 1 || aux_i < 0)
				return (0);
			flag->value.bool_value = aux_i;
		}
		else
		{
			if (aux_i > max_range || aux_i < min_range)
			{
				printf("param: invalid argument: '%s': out of range: %d <= value <= %d\n",
					str, min_range, max_range);
				return (0);
			}
			flag->value.int_value = aux_i;
		}
		return (1);
	}

	if (type == FLAG_CHAR)
	{
		if (ft_strlen(str) == 1)
		{
			flag->value.char_value = str[0];
			return (1);
		}
	}

	if (type == FLAG_STRING)
	{
		flag->value.str_value = ft_strdup(str);
		return (1);
	}

	if (type == FLAG_NONE)
	{
		printf("param: invalid argument: '%s': flag doesnt require argument\n", str);
		return (0);
	}

	return (0);
}


int validate_argument(int argc, int *i, char **argv, t_flag *flag)
{
    int start = *i;
    if (!flag || !argv || !i)
        return 0;

    switch (flag->type)
    {
        case FLAG_NONE:
            break;

        case FLAG_INTEGER:
            if (*i + 1 >= argc)
                return needs_argument(argv[*i]);

            if (!ft_isnumber(argv[*i + 1]))
                return 0;

            flag->value.int_value = ft_atoi(argv[++(*i)]);
            break;

        case FLAG_STRING:
            if (*i + 1 >= argc)
                return needs_argument(argv[*i]);

            flag->value.str_value = ft_strdup(argv[++(*i)]);
            break;

        case FLAG_CHAR:
            if (*i + 1 >= argc)
                return needs_argument(argv[*i]);

            if (ft_strlen(argv[*i + 1]) != 1)
                return 0;

            flag->value.char_value = argv[++(*i)][0];
            break;

        case FLAG_BOOLEAN:
        {
            if (*i + 1 >= argc)
                return needs_argument(argv[*i]);

            if (!ft_isnumber(argv[*i + 1]))
                return 0;

            int v = ft_atoi(argv[++(*i)]);
            if (v != 0 && v != 1)
                return 0;

            flag->value.bool_value = v;
            break;
        }

        case FLAG_RANGE_INTEGER:
            if (*i + 2 >= argc)
                return needs_argument(argv[*i]);

            if (!ft_isnumber(argv[*i + 1]) || !ft_isnumber(argv[*i + 2]))
                return 0;

            flag->value.range.min = ft_atoi(argv[*i + 1]);
            flag->value.range.max = ft_atoi(argv[*i + 2]);
            *i += 2;
            break;

        default:
            return 0;
    }

    flag->active = 1;
    for (int j = start; j <= *i; j++)
        argv[j] = "\0";

    return 1;
}


//Procesado correcto 1 / Error 0
int process_flags(int argc, char **argv, t_list *flag,struct s_params *params)
{
	int i = 1;
	t_list *aux;
    t_flag *f;
    int ret = 1;

	while (i < argc)
	{
		aux = find_flag(flag,argv[i]);
        //if flag is found we need to extract its data from the argument
		if (aux)
		{
            f = aux->content;
            if (validate_argument(argc,&i,argv,f) && f->apply)
            {
                if (!f->apply(f,params))
                    ret = 0;
            }
            else
            {
                ret = 0;
                if (f->not_apply)
                    f->not_apply(f,params);
            }

		}
		i++;
	}
	return ret;
}
