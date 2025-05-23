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

    struct Node* construct_tree(char* data)
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
        if (strcmp(key, root->data) == 0)
            return root;
        if (strcmp(key, root->data) < 0)
        {
            if (root->left)
                return search_tree(root->left, key);
            return root;
        }
        if (strcmp(key, root->data) > 0)
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
        
        if (strcmp(data, root->data) < 0)
            root->left = insert_node(root->left, data);
        else if (strcmp(data, root->data) > 0)
            root->right = insert_node(root->right, data);
        else
            return root;
        
        root->height = 1 + max(height(root->right), height(root->left));

        int balance = get_balance(root);
        
        if (balance > 1 && strcmp(data, root->left->data) < 0)
            return rotate_right(root);

        if (balance < -1 && strcmp(data, root->right->data) > 0)
            return rotate_left(root);

        if (balance > 1 && strcmp(data, root->left->data) > 0) {
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }

        if (balance < -1 && strcmp(data, root->right->data) < 0) {
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }

        return root;
    }

    struct Node* delete_node(struct Node* root, const char* key)
    {
        if (root == NULL)
            return NULL;
        if (strcmp(key, root->data) > 0)
            root->right = delete_node(root->right, key);
        else if (strcmp(key, root->data) < 0)
            root->left = delete_node(root->left, key);
        else if (strcmp(root->data , key) == 0)
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

    struct Node* predecessor(struct Node* root)
    {
        if (root == NULL) return NULL;
        return maximum(root->left);
    }
    struct Node* successor(struct Node* root)
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

    void toLowerCase(char *str) {
        for (int i = 0; str[i]; i++)
            str[i] = tolower((unsigned char)str[i]);
    }

    int main()
    {
        FILE* f = fopen("Dictionary.txt", "r");
        if (f == NULL) printf("File does not exist\n");

        char temp[100];
        fscanf(f, "%s", temp);
        toLowerCase(temp); //TODO MINA LOOK AT THIS I AM CONVERTING TO LOWER AHOO
        struct Node* treeRoot = construct_tree(temp);
        char x = 'a';

        while (x != EOF)
        {
            fscanf(f, "%s", temp);
            toLowerCase(temp); //TODO MINA LOOK AT THIS
            insert_node(treeRoot, temp);
            x = fgetc(f);
        }
        fclose(f);
        
        char input[200];
        printf("Enter a sentence: ");
        gets(input);
        toLowerCase(input);
        char* token = strtok(input, " ");
        while (token != NULL)
        {
            struct Node* tempNode = search_tree(treeRoot, token);
            insert_node(treeRoot, "i");
            if (strcmp(tempNode->data,token) != 0)
            {
                printf("Word '%s' -- ", token);   
                printf("Invalid word -- ");
                
                /*
                struct Node* node = construct_tree(token);
                FILE* f1 = fopen("Dictionary.txt", "r");
                if (f1 == NULL) printf("File does not exist\n");
                x = 'a';

                while (x != EOF)
                {
                    fscanf(f1, "%s", temp);
                    insert_node(node, StrToLower(temp));
                    x = fgetc(f1);
                }
                fclose(f1);
                */
                
                
                //printf("Suggestions: '%s', '%s', '%s'\n", tempNode->data, predecessor(node)->data, successor(node)->data);
                printf("Suggestions: '%s'\n", tempNode->data);

            }
            else
            {
                printf("Word '%s' -- ", token);   
                printf("Valid word\n");
            }
            token = strtok(NULL, " ");
        }
        

        
        /*struct Node* root = construct_tree("ahmed");
        root = insert_node(root, "mina");
        root = insert_node(root, "rabie");
        root = insert_node(root, "youssef");
        root = insert_node(root, "mahmoud");
        root = insert_node(root, "metawie");
        print_tree(root);
        printf("\n");
        root = delete_node(root, "mina");
        print_tree(root);
        destruct_tree(root);*/
        return 0;
    }