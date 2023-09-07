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
    const char operator;
    const int precedence;
};

const struct operator_info operator_hash[] = {
    {'+', 2},
    {'-', 2},
    {'*', 3},
    {'/', 3},
    {'%', 3},
};


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
void print_list(node* head) {
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

int precedence(char operator) {

    for (int i = 0; i < sizeof(operator_hash)/sizeof(operator_hash[0]); i++) {
        if (operator == operator_hash[i].operator) {
            return operator_hash[i].precedence;
        }
    }
    return 0;
}

bool compare_precedence(char operator_1, char operator_2) {

    if (precedence(operator_2) >= precedence(operator_1)) return true;
    return false;
}

void shunting_yard (char** string_array, node** output_queue, char* operator_stack) {
    int stk_index = 0;
    while (*string_array != NULL) { // while there are tokens to be read:
        
        char* token = *string_array; // read a token
        
        if (token[0] >= '0' && token[0] <= '9') { // INTEGER 
            enqueue(output_queue, token);
            print_list(*output_queue);

        } else if (*token == '(') { // LEFT PARENTHESIS
            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';
            printf("OPERATOR STACK: %s\n", operator_stack);

        } else if (*token == ')') { // RIGHT PARENTHESIS
            printf(") found!\n");
            while (stk_index > 0 && operator_stack[stk_index - 1] != '(') {
                
                char temp[2] = {operator_stack[--stk_index], '\0'};
                char* copy = strdup(temp);

                enqueue(output_queue, copy);
                print_list(*output_queue);                
                
                operator_stack[stk_index] = '\0';
                printf("OPERATOR STACK: %s\n", operator_stack);
                
            }
            // remove '('
            if (stk_index > 0 && operator_stack[stk_index - 1] == '(') {
                operator_stack[--stk_index] = '\0';
            }
                    
        } else if (*token == '+' || *token == '-' || *token == '*' || *token == '/' || *token == '%') { // OPERATOR

            while (operator_stack[stk_index - 1] != '(' && compare_precedence(*token, operator_stack[stk_index - 1])) {

                char temp[2] = {operator_stack[--stk_index], '\0'};
                char* copy = strdup(temp);

                enqueue(output_queue, copy);
                print_list(*output_queue);                
                
                operator_stack[stk_index] = '\0';
                printf("OPERATOR STACK: %s\n", operator_stack);
            }

            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';
            printf("OPERATOR STACK: %s\n", operator_stack);
        }   
        printf("--------\n");

        string_array++;
    }

    // After while loop, pop everything from operator stack to output queue.
    while (stk_index > 0) {
        char temp[2] = {operator_stack[--stk_index], '\0'};
        char* copy = strdup(temp);

        enqueue(output_queue, copy);
        print_list(*output_queue);

        operator_stack[stk_index] = '\0';
        printf("OPERATOR STACK: %s\n", operator_stack);
    }

}

int eval_postfix(node* output_queue, char** postfix_stack){
    // while output queue has stuff in it, push each item into the stack starting from the head until empty
    node* current = output_queue;
    while (current != NULL) {
        char* push_me = current->data; 
        printf("scan %s\n", push_me);
        



        current = current->next;
    }
    return 0;
}


int main(int ac, char** av) {
    
    // TEST CASES
    
    char case_1[] = "38 + 4 * ( 2 * 1 + 13 - 6 ) * 5 / 2";
    char case_2[] = "10 * ( 321 - 4 ) + 2 % 5";
    char case_3[] = "1+2*(3-42)/5";
    char case_4[] = "321()";
    char case_5[] = "312/0";
    char case_6[] = "-(-((-4)+-6))";
    char case_7[] = "1234567890";

    char* test = case_1;
    
    // input string -------------> infix expression
    
    // remove spaces and get strlen
    int my_strlen = rm_space_and_strlen(test);
    printf("strlen = %d\n", my_strlen); // TEST PRINT
    // create string array to separate out operators/parentheses and integers
    char** string_array = make_str_arr(test, my_strlen);
    for (int i = 0; string_array[i] != NULL; i++) {
        printf("%s", string_array[i]); // TEST PRINT
    }
    printf("\n-------------------------\n");

    // infix ---> postfix using shunting yard algorithm
    char operator_stack[my_strlen];
    node* q_head = NULL;
    shunting_yard(string_array, &q_head, operator_stack);

    // evaluate postfix expression using stack
    print_list(q_head);
    char** postfix_stack = (char **)malloc(my_strlen * sizeof(char *));
    eval_postfix(q_head, postfix_stack);




    
    return 0;
}