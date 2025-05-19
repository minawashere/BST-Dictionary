#include <stdio.h>
#include <stdlib.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct
{
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

Node* construct_tree(int key)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    return new_node;
}

void destruct_tree(Node* node)
{
    if (node == NULL)
        return;
    if (node->left)
        destruct_tree((Node*)node->left);
    if (node->right)
        destruct_tree((Node*)node->right);
    free(node);
}

Node* search_tree(Node* root, const int key)
{
    if (root == NULL) return NULL;
    if (root->key == key)
        return root;
    if (key < root->key)
        return search_tree((Node*)root->left, key);
    if (key > root->key)
        return search_tree((Node*)root->right, key);
    return NULL;
}

Node* minimum(Node* root)
{
    if (root == NULL)
        return NULL;
    // while (current!=NULL)
    // {
    //     if (current->left == NULL)
    //         return current;
    //     current = (Node*)current->left;
    // }
    if (root->left == NULL)
        return root;
    return minimum((Node*)root->left);
}


Node* maximum(Node* root)
{
    if (root == NULL)
        return NULL;
    Node* current = root;
    while (current->right != NULL)
    {
        if (current->right == NULL)
            return current;
        current = (Node*)current->right;
    }
    return NUll;
}

int height(const Node* root)
{
    if (root == NULL)
        return 0;
    return root->height;
}

int get_balance(const Node* root)
{
    if (root == NULL)
        return 0;
    return height((Node*)root->left) - height((Node*)root->right);
}

Node* rotate_right(Node* old_root)
{
    Node* new_root = (Node*)old_root->left;
    Node* t2 = (Node*)new_root->right;
    new_root->right = (struct Node*)old_root;
    old_root->left = (struct Node*)t2;
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));
    old_root->height = 1 + max(height(old_root->left), height(old_root->right));
    return new_root;
}


Node* rotate_left(Node* old_root)
{
    Node* new_root = (Node*)old_root->right;
    Node* t2 = (Node*)new_root->left;

    new_root->left = (struct Node*)old_root;
    old_root->right = (struct Node*)t2;

    new_root->height = 1 + max(height(new_root->left), height(new_root->right));
    old_root->height = 1 + max(height(old_root->left), height(old_root->right));
    return new_root;
}

Node* insert_node(Node* root, const int key)
{
    if (root == NULL)
        return construct_tree(key);
    if (key < root->key)
        root->left = (struct Node*)insert_node((Node*)root->left, key);
    if (key > root->key)
        root->right = (struct Node*)insert_node((Node*)root->right, key);
    root->height = 1 + max(height(root->right), height(root->left));
    return root;
}

Node* delete_node(Node* root, const int key)
{
    if (root == NULL)
        return NULL;
    if (key > root->key)
        root->right = (struct Node*)delete_node((Node*)root->right, key);
    else if (key < root->key)
        root->left = (struct Node*)delete_node((Node*)root->left, key);
    else if (root->key == key)
    {
        if (root->left == NULL && root->right == NULL)
        {
            destruct_tree(root);
            return NULL;
        }
        if (root->right == NULL)
        {
            Node* temp = (Node*)root->left;
            free(root);
            return temp;
        }
        if (root->left == NULL)
        {
            Node* temp = (Node*)root->right;
            free(root);
            return temp;
        }

        const Node* temp = minimum((Node*)root->right);
        if (temp)
        {
            root->key = temp->key;
            root->right = (struct Node*)delete_node((Node*)root->right, temp->key);
        }
    }

    return root;
}

void print_tree(const Node* root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        print_tree((Node*)root->left);
    printf("%d ", root->key);
    if (root->right != NULL)
        print_tree((Node*)root->right);
}


int main()
{
    Node* root = construct_tree(15);
    root = insert_node(root, 10);
    root = insert_node(root, 5);
    root = insert_node(root, 20);
    root = insert_node(root, 8);
    root = insert_node(root, 11);
    print_tree(root);
    printf("\n");
    root = delete_node(root, 20);
    print_tree(root);
    destruct_tree(root);
    return 0;
}
