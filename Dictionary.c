#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dictnode {
    char word[128];
    char meaning[256];
    struct dictnode *lft, *rght;
};

struct dictnode *root = NULL;

struct dictnode *createnode(char *word, char *meaning) {
    struct dictnode *newnode = (struct dictnode *)malloc(sizeof(struct dictnode));
    strcpy(newnode->word, word);
    strcpy(newnode->meaning, meaning);
    newnode->lft = newnode->rght = NULL;
    return newnode;
}

void insert(char *word, char *meaning) {
    struct dictnode *parent = NULL, *current = root, *newnode = NULL;
    int result = 0;

    if (!root) {
        root = createnode(word, meaning);
        return;
    }

    while (current != NULL) {
        result = strcasecmp(word, current->word);
        if (result == 0) {
            printf("Word already present in the dictionary.\n");
            return;
        }
        parent = current;
        current = (result > 0) ? current->rght : current->lft;
    }

    newnode = createnode(word, meaning);
    if (result > 0) {
        parent->rght = newnode;
    } else {
        parent->lft = newnode;
    }
}

struct dictnode *findMin(struct dictnode *node) {
    while (node->lft != NULL) {
        node = node->lft;
    }
    return node;
}

struct dictnode *deleteNode(struct dictnode *root, char *word) {
    if (!root) {
        printf("Word not found in the dictionary.\n");
        return NULL;
    }

    int result = strcasecmp(word, root->word);
    if (result < 0) {
        root->lft = deleteNode(root->lft, word);
    } else if (result > 0) {
        root->rght = deleteNode(root->rght, word);
    } else {
        // Node with one or no children
        if (root->lft == NULL) {
            struct dictnode *temp = root->rght;
            free(root);
            return temp;
        } else if (root->rght == NULL) {
            struct dictnode *temp = root->lft;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        struct dictnode *temp = findMin(root->rght);

        // Copy the inorder successor's content to this node
        strcpy(root->word, temp->word);
        strcpy(root->meaning, temp->meaning);

        // Delete the inorder successor
        root->rght = deleteNode(root->rght, temp->word);
    }
    return root;
}

void search(char *word) {
    struct dictnode *current = root;
    int result = 0;

    while (current != NULL) {
        result = strcasecmp(word, current->word);
        if (result == 0) {
            printf("Word: %s Meaning: %s ", current->word, current->meaning);
            return;
        }
        current = (result > 0) ? current->rght : current->lft;
    }
    printf("Word not found in the dictionary.\n");
}

void inorderTraversal(struct dictnode *node) {
    if (node) {
        inorderTraversal(node->lft);
        printf("Word: %s Meaning: %s\n", node->word, node->meaning);
        inorderTraversal(node->rght);
    }
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    int choice;
    char word[128], meaning[256];

    while (1) {
        printf("\n------------****MENU****--------------\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Inorder Traversal\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline after the number

        switch (choice) {
            case 1:
                printf("Enter the word to insert: ");
                fgets(word, 128, stdin);
                trim_newline(word);
                printf("Enter the meaning: ");
                fgets(meaning, 256, stdin);
                trim_newline(meaning);
                insert(word, meaning);
                break;
            case 2:
                printf("Enter the word to delete: ");
                fgets(word, 128, stdin);
                trim_newline(word);
                root = deleteNode(root, word);
                break;
            case 3:
                printf("Enter the word to search: ");
                fgets(word, 128, stdin);
                trim_newline(word);
                search(word);
                break;
            case 4:
                inorderTraversal(root);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
