#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

// linked list will be used for output queue
typedef struct l_list_node {
    char* data;
    struct l_list_node* next;
} node;

struct operator_info {
    const char op;
    const int precedence;
    int (*operation)(int, int);
};

int add(int a, int b);

int subtract(int a, int b);

int multiply(int a, int b);

int divide(int a, int b);

int modulo(int a, int b);


// Function to add a node to the end of the linked list
void enqueue(node** head, char* data);

// void print_list(node* head);

void free_linked_list(node* head);

size_t rm_space_and_strlen(char* str);

int my_atoi(char* str);

char** make_str_arr(char* str, int arr_len);

int precedence(char op);

int evaluate(int a, char op, int b);

bool compare_precedence(char operator_1, char operator_2);

void shunting_yard (char** string_array, node** output_queue, char* operator_stack);

int eval_postfix(node* output_queue, size_t my_strlen);

#endif