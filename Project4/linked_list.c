#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

Node* head = NULL;  // Global head of the list

void initialize_list() {
    head = (Node*)malloc(sizeof(Node));
    head->value = 1;
    head->next = NULL;
    Node* temp = head;
    for (int i = 2; i <= NUM_NODES; i++) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->value = i;
        new_node->next = NULL;
        pthread_mutex_init(&new_node->lock, NULL);
        temp->next = new_node;
        temp = new_node;
    }
}

void display_list() {
    Node* temp = head;
    while (temp) {
        printf("%d -> ", temp->value);
        temp = temp->next;
    }
    printf("NULL\n");
    printf("----------------------------------\n");
}

void cleanup_list() {
    Node* temp = head;
    while (temp) {
        Node* next = temp->next;
        pthread_mutex_destroy(&temp->lock);
        free(temp);
        temp = next;
    }
}
