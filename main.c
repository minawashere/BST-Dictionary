#include <stdio.h>
    #include <stdlib.h>

    #define min(a,b) (((a) < (b)) ? (a) : (b))
    #define max(a,b) (((a) > (b)) ? (a) : (b))

    struct Node
    {
        int key;
        struct Node* left;
        struct Node* right;
        int height;
    };

    struct Node* construct_tree(int key)
    {
        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        new_node->key = key;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 1;
        return new_node;
    }

    void destruct_tree(struct Node* node)
    {
        if (node == NULL)
            return;
        if (node->left)
            destruct_tree(node->left);
        if (node->right)
            destruct_tree(node->right);
        free(node);
    }

    struct Node* search_tree(struct Node* root, const int key)
    {
        if (root == NULL) return NULL;
        if (root->key == key)
            return root;
        if (key < root->key)
            return search_tree(root->left, key);
        if (key > root->key)
            return search_tree(root->right, key);
        return NULL;
    }

    struct Node* minimum(struct Node* root)
    {
        if (root == NULL)
            return NULL;
        if (root->left == NULL)
            return root;
        return minimum(root->left);
    }

    struct Node* maximum(struct Node* root)
    {
        if (root == NULL)
            return NULL;
        struct Node* current = root;
        while (current->right != NULL)
        {
            if (current->right == NULL)
                return current;
            current = current->right;
        }
        return NULL;
    }

    int height(const struct Node* root)
    {
        if (root == NULL)
            return 0;
        return root->height;
    }

    int get_balance(const struct Node* root)
    {
        if (root == NULL)
            return 0;
        return height(root->left) - height(root->right);
    }

    struct Node* rotate_right(struct Node* old_root)
    {
        struct Node* new_root = old_root->left;
        struct Node* t2 = new_root->right;
        new_root->right = old_root;
        old_root->left = t2;
        new_root->height = 1 + max(height(new_root->left), height(new_root->right));
        old_root->height = 1 + max(height(old_root->left), height(old_root->right));
        return new_root;
    }

    struct Node* rotate_left(struct Node* old_root)
    {
        struct Node* new_root = old_root->right;
        struct Node* t2 = new_root->left;

        new_root->left = old_root;
        old_root->right = t2;

        new_root->height = 1 + max(height(new_root->left), height(new_root->right));
        old_root->height = 1 + max(height(old_root->left), height(old_root->right));
        return new_root;
    }

    struct Node* insert_node(struct Node* root, const int key)
    {
        if (root == NULL)
            return construct_tree(key);
        if (key < root->key)
            root->left = insert_node(root->left, key);
        if (key > root->key)
            root->right = insert_node(root->right, key);
        root->height = 1 + max(height(root->right), height(root->left));
        return root;
    }

    struct Node* delete_node(struct Node* root, const int key)
    {
        if (root == NULL)
            return NULL;
        if (key > root->key)
            root->right = delete_node(root->right, key);
        else if (key < root->key)
            root->left = delete_node(root->left, key);
        else if (root->key == key)
        {
            if (root->left == NULL && root->right == NULL)
            {
                destruct_tree(root);
                return NULL;
            }
            if (root->right == NULL)
            {
                struct Node* temp = root->left;
                free(root);
                return temp;
            }
            if (root->left == NULL)
            {
                struct Node* temp = root->right;
                free(root);
                return temp;
            }

            const struct Node* temp = minimum(root->right);
            if (temp)
            {
                root->key = temp->key;
                root->right = delete_node(root->right, temp->key);
            }
        }

        return root;
    }

    void print_tree(const struct Node* root)
    {
        if (root == NULL)
            return;
        if (root->left != NULL)
            print_tree(root->left);
        printf("%d ", root->key);
        if (root->right != NULL)
            print_tree(root->right);
    }

    int main()
    {
        struct Node* root = construct_tree(15);
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