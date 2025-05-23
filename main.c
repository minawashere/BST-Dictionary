#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

struct Node
{
    char* data;
    struct Node* left;
    struct Node* right;
    int height;
};


int height(const struct Node* root)
{
    if (root == NULL)
        return 0;
    return root->height;
}

struct Node* construct_tree(const char* data)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = malloc(strlen(data) + 1);
    strcpy(new_node->data, data);
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

struct Node* search_tree(struct Node* root, const char* key)
{
    if (root == NULL) return NULL;
    if (strcasecmp(key, root->data) == 0)
        return root;
    if (strcasecmp(key, root->data) < 0)
    {
        if (root->left)
            return search_tree(root->left, key);
        return root;
    }
    if (strcasecmp(key, root->data) > 0)
    {
        if (root->right)
            return search_tree(root->right, key);
        return root;
    }
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

struct Node* find_predecessor(struct Node* root, struct Node* node)
{
    if (!node) return NULL;

    if (node->left)
        return maximum(node->left);

    struct Node* predecessor = NULL;
    struct Node* current = root;

    while (current)
    {
        if (strcasecmp(node->data, current->data) > 0)
        {
            predecessor = current;
            current = current->right;
        }
        else if (strcasecmp(node->data, current->data) < 0)
        {
            current = current->left;
        }
        else
        {
            break;
        }
    }
    return predecessor;
}

struct Node* find_successor(struct Node* root, struct Node* node)
{
    if (!node) return NULL;

    if (node->right)
        return minimum(node->right);

    struct Node* successor = NULL;
    struct Node* current = root;

    while (current)
    {
        if (strcasecmp(node->data, current->data) < 0)
        {
            successor = current;
            current = current->left;
        }
        else if (strcasecmp(node->data, current->data) > 0)
        {
            current = current->right;
        }
        else
        {
            break;
        }
    }
    return successor;
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
    old_root->height = 1 + max(height(old_root->left), height(old_root->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));
    return new_root;
}

struct Node* rotate_left(struct Node* old_root)
{
    struct Node* new_root = old_root->right;
    struct Node* t2 = new_root->left;

    new_root->left = old_root;
    old_root->right = t2;

    old_root->height = 1 + max(height(old_root->left), height(old_root->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));
    return new_root;
}

struct Node* insert_node(struct Node* root, char* data)
{
    if (root == NULL)
        return construct_tree(data);

    if (strcasecmp(data, root->data) < 0)
        root->left = insert_node(root->left, data);
    else if (strcasecmp(data, root->data) > 0)
        root->right = insert_node(root->right, data);
    else
        return root;

    root->height = 1 + max(height(root->right), height(root->left));

    int balance = get_balance(root);

    if (balance > 1 && strcasecmp(data, root->left->data) < 0)
        return rotate_right(root);

    if (balance < -1 && strcasecmp(data, root->right->data) > 0)
        return rotate_left(root);

    if (balance > 1 && strcasecmp(data, root->left->data) > 0)
    {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if (balance < -1 && strcasecmp(data, root->right->data) < 0)
    {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

struct Node* delete_node(struct Node* root, const char* key)
{
    if (root == NULL)
        return NULL;
    if (strcasecmp(key, root->data) > 0)
        root->right = delete_node(root->right, key);
    else if (strcasecmp(key, root->data) < 0)
        root->left = delete_node(root->left, key);
    else if (strcasecmp(root->data, key) == 0)
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
            root->data = temp->data;
            root->right = delete_node(root->right, temp->data);
        }
    }

    return root;
}

struct Node* predecessor(const struct Node* root)
{
    if (root == NULL) return NULL;
    return maximum(root->left);
}

struct Node* successor(const struct Node* root)
{
    if (root == NULL) return NULL;
    return minimum(root->right);
}

void print_tree(const struct Node* root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        print_tree(root->left);
    printf("%s \n", root->data);
    if (root->right != NULL)
        print_tree(root->right);
}

// void toLowerCase(char* str)
// {
//     for (int i = 0; str[i]; i++)
//         str[i] = tolower((unsigned char)str[i]);
// }

int main()
{
    int i = 0;
    FILE* f = fopen("Dictionary.txt", "r");
    if (!f)
    {
        printf("File does not exist\n");
        return 1;
    }

    struct Node* treeRoot = NULL;
    char temp[100];

    while (fscanf(f, "%99s", temp) == 1)
    {
        treeRoot = insert_node(treeRoot, temp);
        i++;
    }

    fclose(f);
    printf("-----------------------------------------\n");
    printf("Dictionary Loaded Successfully\n");
    printf("Tree size: %d nodes\n", i);
    printf("Height of the tree is: %d nodes\n", height(treeRoot));
    printf("-----------------------------------------\n");

    char input[200];
    printf("Enter a sentence: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    // toLowerCase(input);

    char* token = strtok(input, " ");
    while (token)
    {
        struct Node* tempNode = search_tree(treeRoot, token);

        if (!tempNode || strcasecmp(tempNode->data, token) != 0)
        {
            printf("Word '%s' -- Invalid. Suggestions: ", token);

            struct Node* A = tempNode;
            struct Node* B = A ? find_predecessor(treeRoot, A) : NULL;
            struct Node* C = A ? find_successor(treeRoot, A) : NULL;

            printf("'%s'", A ? A->data : "");
            if (B) printf(", '%s'", B->data);
            if (C) printf(", '%s'", C->data);
            printf("\n");
        }
        else
        {
            printf("Word '%s' -- Valid\n", token);
        }

        token = strtok(NULL, " ");
    }
    destruct_tree(treeRoot);
    return 0;
}
