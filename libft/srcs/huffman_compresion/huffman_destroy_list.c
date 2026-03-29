/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman_destroy_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:58:27 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/24 15:56:26 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

void fd_huffman_destroy_list(t_list *lst)
{
    t_list *aux;
    if (!lst)
        return;
    aux = lst;

    while (aux)
    {
        free(aux->content);
        aux = aux->next;
    }
    ft_lstclear(&lst);
    return;
}