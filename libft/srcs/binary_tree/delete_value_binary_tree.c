/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_value_binary_tree.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 19:54:50 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/23 23:36:35 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

t_binary_tree* ft_binarytree_delete(t_binary_tree* tree, int content)
{
  
    if (tree == NULL) 
        return tree;

    if (content < tree->content)
        tree->left = ft_binarytree_delete(tree->left, content);
    else if (content > tree->content)
        tree->right = ft_binarytree_delete(tree->right, content);
    else {

        // If content is present more than once, 
      	// decrement count and return
        if (tree->count > 1) {
            tree->count--;
            return tree;
        }

        // node with only one child or no child
        if (tree->left == NULL) {
            t_binary_tree* temp = tree->right;
            free(tree);
            return temp;
        }
        else if (tree->right == NULL) {
            t_binary_tree* temp = tree->left;
            free(tree);
            return temp;
        }

        // node with two children: Get the inorder 
      	// successor (smallest in the right subtree)
        t_binary_tree* temp = ft_binarytree_minValue(tree->right);

        // Copy the inorder successor's content to this node
        tree->content = temp->content;
        tree->count = temp->count;

        // Set the count to 0 to ensure successor 
      	// gets deleted by deleteNode call
        temp->count = 0;

        // Delete the inorder successor
        tree->right = ft_binarytree_delete(tree->right, temp->content);
    }
    return tree;
}