/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman_calculate_size.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:14:56 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/27 13:05:10 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../lib/libft.h"

size_t ft_huffman_calculate_bitsize(t_list *lst)
{
    t_list *aux;
    size_t bit_size;

    bit_size = 0;
    if (!lst)
        return (bit_size);
    aux = lst;

    while (aux)
    {
        bit_size += ((t_huffman_count_list *)aux->content)->count * ft_strlen(((t_huffman_count_list *)aux->content)->code);
        aux = aux->next;
    }

    return (bit_size);
}
