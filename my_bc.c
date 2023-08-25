#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef struct node{
    char* data;
    struct node* next;
} node;

// Function to add a node to the end of the linked list
void enqueue(node** head, char* data) {
    node* newNode = (node*) malloc(sizeof(node));
    if (newNode == NULL) {
        // Handle memory allocation error
        return;
    }
    newNode->data = data;
    newNode->next = NULL; // New node will be the last node, so set its next to NULL
    if (*head == NULL) {
        // If the list is empty, make the new node the head
        *head = newNode;
    } else {
        // Otherwise, traverse the list to find the last node
        node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Add the new node to the end
        current->next = newNode;
    }
}

// Function to print the linked list
void print_queue(node* head) {
    node* current = head;
    while (current != NULL) {
        printf("%s => ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int rm_space_and_strlen(char* str) {

    char* src = str;
    char* dst = str;
    int len = 0;

    while (*src) {
        if (*src != ' ') {
            *dst = *src;
            dst++;
            len++;
        }
        src++;
    }
    *dst = '\0';
    return len + 1;
}

char** make_str_arr(char* str, int arr_len) {

    char** str_arr = (char **)malloc(arr_len * sizeof(char *));
    int i_str_arr = 0;
    
    char int_buff[arr_len];
    int_buff[0] = '\0';
    int i_int_buff = 0; 
    
    while (*str) {
        // if number character found
        if (*str >= '0' && *str <= '9') {
            int_buff[i_int_buff] = *str;
            i_int_buff++;
        } else { // if operator/parentheses found
            
            if (int_buff[0] != '\0') { // if preceded by a number
                // add contents of int_buff to string array
                int_buff[i_int_buff] = '\0';
                str_arr[i_str_arr] = malloc(i_int_buff * sizeof(char*));
                strcpy(str_arr[i_str_arr], int_buff);
                i_int_buff = 0;
                int_buff[0] = '\0';
                i_str_arr++;
                // add operator to string array
                str_arr[i_str_arr] = malloc(2);
                str_arr[i_str_arr][0] = *str;
                str_arr[i_str_arr][1] = '\0';
                i_str_arr++;
            } else {
                // add operator to string array
                str_arr[i_str_arr] = malloc(2);
                str_arr[i_str_arr][0] = *str;
                str_arr[i_str_arr][1] = '\0';
                i_str_arr++;
            }
        }
        str++;
    }
    if (int_buff[0] != '\0') {
        int_buff[i_int_buff] = '\0';
        str_arr[i_str_arr] = malloc(i_int_buff + 1 * sizeof(char*));
        strcpy(str_arr[i_str_arr], int_buff);
    }
    return str_arr;
}

void shunting_yard (char** string_array, node* output_queue, char* operator_stack) {
    int stk_index = 0;
    while (*string_array != NULL) { // while there are tokens to be read:
        
        char* token = *string_array; // read a token
        
        if (token[0] >= '0' && token[0] <= '9') { // number ---> output queue 
            enqueue(&output_queue, token);
            print_queue(output_queue);

        } else if (*token == '(') { // '(' ----> operator stack
            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';
            printf("OPERATOR STACK: %s\n", operator_stack);

        } else if (*token == ')') { // ')'
            printf(") found!\n");
            while (stk_index > 0 && operator_stack[stk_index - 1] != '(') {
                
                char temp[2] = {operator_stack[--stk_index], '\0'};
                char* copy = strdup(temp);

                enqueue(&output_queue, copy);
                print_queue(output_queue);                
                
                operator_stack[stk_index] = '\0';
                printf("OPERATOR STACK: %s\n", operator_stack);
                
            }
            // remove '('
            if (stk_index > 0 && operator_stack[stk_index - 1] == '(') {
                operator_stack[--stk_index] = '\0';
            }
                    
        } else {
            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';
            printf("OPERATOR STACK: %s\n", operator_stack);
        }   
        printf("--------\n");

        string_array++;
    }
}


int main(int ac, char** av) {
    
    // TEST CASES
    
    char case_1[] = "32 + 420 * ( 20 * 1 + 3 - 6 ) * 10 / 2";
    char case_2[] = "1 + 2 * (3 - 42) / 5";
    char case_3[] = "1+2*(3-42)/5";
    char case_4[] = "321()";
    char case_5[] = "312/0";
    char case_6[] = "-(-((-4)+-6))";
    char case_7[] = "1234567890";

    char* test = case_1;
    
    // input string -------------> infix expression
    
    // remove spaces and get strlen
    int strlen = rm_space_and_strlen(test);
    printf("%s\nstrlen = %d\n", test, strlen); // TEST PRINT
    // create string array to separate out operators/parentheses and integers
    char** string_array = make_str_arr(test, strlen);
    for (int i = 0; string_array[i] != NULL; i++) {
        printf("%s", string_array[i]); // TEST PRINT
    }
    printf("\n");

    // use shunting-yard algorithm to produce infix expression
    char operator_stack[strlen];
    node* head = NULL;
    node* current = NULL;
    shunting_yard(string_array, head, operator_stack);

    // infix expression ---------> postfix expression
    
    return 0;
}