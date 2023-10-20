#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Define a structure for a node in the linked list
struct Node {
    int data_int;
    float data_float;
    char data_char;
    struct Node* next;
};

// Function to insert a new node at the end of the linked list
void insert(struct Node** head, int int_val, float float_val, char char_val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data_int = int_val;
    newNode->data_float = float_val;
    newNode->data_char = char_val;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to display the linked list
void display(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("(%d, %.2f, %c) ", current->data_int, current->data_float, current->data_char);
        current = current->next;
    }
    printf("\n");
}

int main() {

    int size = 100;

    //initialize linked list
    struct Node* head = NULL;
    srand(time(NULL));

    // Generate and insert random values into the linked list
    for (int i = 0; i < size; i++) {
        int randomInt = rand() % 100;
        float randomFloat = (float)rand() / RAND_MAX * 100.0;
        char randomChar = 'A' + (rand() % 26);
        insert(&head, randomInt, randomFloat, randomChar);
    }

    printf("Linked List: ");
    display(head);

    // Clean up the linked list
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
