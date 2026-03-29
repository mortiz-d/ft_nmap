/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_value_binary_tree.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 19:42:40 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/23 19:50:58 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

t_binary_tree* ft_binarytree_minValue(t_binary_tree *tree) 
{
    t_binary_tree * aux;
    
    if (!tree)
        return (NULL);

    aux = tree;
    while (aux && aux->left)
        aux = aux->left;

    return aux;
}

t_binary_tree* ft_binarytree_maxValue(t_binary_tree *tree) 
{
    t_binary_tree * aux;
    
    if (!tree)
        return (NULL);

    aux = tree;
    while (aux && aux->right)
        aux = aux->right;
    return aux;
}
