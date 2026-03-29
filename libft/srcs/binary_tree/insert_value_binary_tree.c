/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_value_binary_tree.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 19:08:15 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/23 20:48:14 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

t_binary_tree   *ft_binarytree_insert(t_binary_tree   *tree, int new_value)
{
    if (!tree)
    {
	    return ft_binarytree_create(new_value);
    }
    
    if (new_value == tree->content) {
        tree->count++;
        return tree;
    }

	if (new_value < tree->content)
    {
        tree->left = ft_binarytree_insert(tree->left, new_value);
    }
    else
    {
        tree->right = ft_binarytree_insert(tree->right, new_value);
    }

	return (tree);
}