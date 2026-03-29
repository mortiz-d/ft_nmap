/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_binary_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 18:44:28 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/23 20:47:39 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

t_binary_tree   *ft_binarytree_create(int content)
{
	t_binary_tree	*tree;

	tree = ft_calloc(1,sizeof(t_binary_tree));
	if (tree == NULL)
		return (NULL);
	tree->content = content;
	tree->count = 1;
	tree->right = NULL;
	tree->left = NULL;
	return (tree);
}