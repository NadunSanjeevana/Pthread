#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

node* head = NULL;

int Member(int value) {
    node* current = head;
    while (current != NULL && current->data < value) {
        current = current->next;
    }
    return (current != NULL && current->data == value);
}

int Insert(int value) {
    node* current = head;
    node* prev = NULL;
    node* new_node = malloc(sizeof(node));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    new_node->data = value;
    new_node->next = NULL;

    if (head == NULL || head->data >= value) {
        new_node->next = head;
        head = new_node;
        return 1;
    }

    while (current != NULL && current->data < value) {
        prev = current;
        current = current->next;
    }

    if (current == NULL || current->data > value) {
        new_node->next = current;
        prev->next = new_node;
        return 1;
    }

    free(new_node);
    return 0;
}

int Delete(int value) {
    node* current = head;
    node* prev = NULL;

    while (current != NULL && current->data < value) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->data == value) {
        if (prev == NULL) {
            head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        return 1;
    }
    return 0;
}

void InitializeList(int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;
        while (!Insert(value)) {
            value = rand() % 65536;
        }
    }
}

void FreeList() {
    while (head != NULL) {
        node* temp = head;
        head = head->next;
        free(temp);
    }
}
