#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap *) malloc(sizeof(TreeMap));

    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key)) return;
    TreeNode *new = createTreeNode(key, value);
    TreeNode *parent = NULL;

    tree->current = tree->root;

    while(tree->current)
    {
        parent = tree->current;

        if(tree->lower_than(key, tree->current->pair->key)) tree->current = tree->current->left;
        else tree->current = tree->current->right;
    }

    new->parent = parent;

    if(!parent) tree->root = new;
    else
    {
        if(tree->lower_than(key, parent->pair->key)) parent->left = new;
        else parent->right = new;
    }

    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    while(x->left) x = x->left;
    return x;
}

void reemplazarNodo(TreeMap *tree, TreeNode *old, TreeNode *new)
{
    if(old->parent)
    {
        if(is_equal(tree, old->parent->left->pair->key, old->pair->key))
            old->parent->left = new;
        else old->parent->right = new;
    }
    else tree->root = new;

    if(new) new->parent = old->parent;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *aux = NULL;

    if(!node->left)
    {
        reemplazarNodo(tree, node, node->right);
    }
    else
    {
        if(!node->right)
        {
            reemplazarNodo(tree, node, node->left);
        }
        else
        {
            aux = minimum(node->right);
            //Si el nodo derecho de aux no es el hijo del nodo a eliminar
            if(!is_equal(tree, aux->parent->pair->key, node->pair->key))
            {
                reemplazarNodo(tree, aux, aux->right);
                aux->right = node->right;
                aux->right->parent = aux;               
            }
            reemplazarNodo(tree, node, aux);
            aux->left = node->left;
            aux->left->parent = aux;
        }
    }

    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;
    if(!tree->current) return NULL;
    
    while(tree->current && !is_equal(tree, tree->current->pair->key, key))
    {
        if(tree->lower_than(key, tree->current->pair->key))
        {
            tree->current = tree->current->left;
        }
        else
        {
            tree->current = tree->current->right;
        }
    }
    if (tree->current) return tree->current->pair;
    else return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
