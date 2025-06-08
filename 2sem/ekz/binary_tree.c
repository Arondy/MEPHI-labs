#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"

Node *tree_find(Tree *tree, const int key)
{
    Node *ptr = tree->root;
    while (ptr != NULL)
    {
        if (ptr->key == key)
            return ptr;

        if (key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }

    return NULL;
}

int tree_insert(Tree *tree, const int key)
{
    Node *ptr = tree->root;
    Node *par = NULL;
    while (ptr != NULL)
    {
        if (ptr->key == key)
            return 1;

        par = ptr;

        if (key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }

    Node *new = malloc(sizeof *new);

    new->key = key;
    new->left = new->right = NULL;

    if (par == NULL)
        tree->root = new;
    else if (key < par->key)
        par->left = new;
    else
        par->right = new;

    return 0;
}

int tree_delete(Tree *tree, const int key)
{
    Node *ptr = tree->root;
    Node *par = NULL;
    while (ptr != NULL)
    {
        if (ptr->key == key)
            break;

        par = ptr;

        if (key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }

    if (ptr == NULL)
        return 1;

    if (ptr->right == NULL)
    {
        if (par == NULL)
            tree->root = ptr->left;
        else if (ptr == par->left)
            par->left = ptr->left;
        else /* ptr == par->right */
            par->right = ptr->left;
    }
    else
    {
        Node *right = ptr->right;
        if (right->left == NULL)
        {
            right->left = ptr->left;

            if (par == NULL)
                tree->root = right;
            else if (ptr == par->left)
                par->left = right;
            else /* ptr = par->right */
                par->right = right;
        }
        else
        {
            Node *succ = right->left;
            while (succ->left != NULL)
            {
                right = succ;
                succ = succ->left;
            }

            right->left = succ->right;
            succ->left = ptr->left;
            succ->right = ptr->right;

            if (par == NULL)
                tree->root = succ;
            else if (ptr == par->left)
                par->left = succ;
            else /* ptr == par->right */
                par->right = succ;
        }
    }

    free(ptr);

    return 0;
}