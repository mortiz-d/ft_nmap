/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:30:50 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/24 17:03:13 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"



t_list *ft_huffman_find(t_list *lst, char c)
{
    t_list *aux;
    if (!lst)
        return (NULL);
    aux = lst;

    while (aux)
    {
        // printf("Comparamos %c - %c\n", c ,((t_huffman_count_list *)aux->content)->c );
        if (((t_huffman_count_list *)aux->content)->c == c)
            return aux;
        aux = aux->next;
    }

    return (NULL);
}

t_list *ft_huffman_add_character(t_list *lst, char c)
{
    t_huffman_count_list *aux_huff;
    t_list *aux;

    if (!lst)
    {
        aux_huff = calloc(1,sizeof(t_huffman_count_list));
        aux_huff->c = c;
        aux_huff->count = 1;
        lst = ft_lstnew(aux_huff);
    }
    else
    {
        aux = ft_huffman_find(lst,c);
        if (!aux)
        {
            aux_huff = calloc(1,sizeof(t_huffman_count_list));
            aux_huff->c = c;
            aux_huff->count = 1;
            ft_lstadd_back(&lst,ft_lstnew(aux_huff));
        }
        else
        {
            ((t_huffman_count_list *)aux->content)->count++;

            while (aux->prev)
            {
                if (((t_huffman_count_list *)aux->content)->count > ((t_huffman_count_list *)aux->prev->content)->count)
                {
                    aux_huff = calloc(1,sizeof(t_huffman_count_list));
                    aux_huff->c = ((t_huffman_count_list *)aux->content)->c;
                    aux_huff->count = ((t_huffman_count_list *)aux->content)->count;

                    ((t_huffman_count_list *)aux->content)->c = ((t_huffman_count_list *)aux->prev->content)->c;
                    ((t_huffman_count_list *)aux->content)->count = ((t_huffman_count_list *)aux->prev->content)->count;

                    ((t_huffman_count_list *)aux->prev->content)->c = aux_huff->c;
                    ((t_huffman_count_list *)aux->prev->content)->count = aux_huff->count;
                    free(aux_huff);
                    aux = aux->prev;
                }
                else
                    break;
            
            }
        }
        
    }
    return (lst);
}

