#include "helpers.h"


int add(int a, int b){
    return a + b;
}

int subtract(int a, int b){
    return a - b;
}

int multiply(int a, int b){
    return a * b;
}

int divide(int a, int b){
    return a / b;
}

int modulo(int a, int b){
    return a % b;
}

const struct operator_info operator_hash[] = {
    {'+', 2, add},
    {'-', 2, subtract},
    {'*', 3, multiply},
    {'/', 3, divide},
    {'%', 3, modulo},
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

// Function to print the linked list -- FOR DEBUGGING
// void print_list(node* head) {
//     node* current = head;
//     while (current != NULL) {
//         printf("%s => ", current->data);
//         current = current->next;
//     }
//     printf("NULL\n");
// }

void free_linked_list(node* head) {
    node* current = head;
    while (current != NULL) {
        node* next = current->next;
        free(current);
        current = next;
    }
}

size_t rm_space_and_strlen(char* str) {

    char* src = str;
    char* dst = str;
    size_t len = 0;

    while (*src) {
        if (*src != ' ') {
            // check for invalid characters
            if (*src > '9') {
                len = -1;
                break;
            } else if (*src < '0') {
                if (*src != '+' && *src != '-' && *src != '*' && *src != '/' && *src != '%' && *src != '(' && *src != ')') {
                    len = -1;
                    break;
                }
            }
            *dst = *src;
            dst++;
            len++;
        }
        src++;
    }
    *dst = '\0';
    return len + 1;
}

int infix_checker(char* str){
    int i = 0;
    while(str[i + 1] != '\0'){
        // empty parentheses
        if (str[i] == '(' && str[i + 1] == ')') return -1;
        // three consecutive operators (ugly but it works)
        if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '%'){
            if (str[i + 1] == '+' || str[i + 1] == '-' || str[i + 1] == '/' || str[i + 1] == '*' || str[i + 1] == '%') {
                if (str[i - 1] == '+' || str[i - 1] == '-' || str[i - 1] == '/' || str[i - 1] == '*' || str[i - 1] == '%') return -1;
            }
        }
        i++;
    }

    return 0;
}

int my_atoi(char* str) {
    int result = 0;
    int sign = 1; // To handle negative numbers

    // Check for a sign character (+ or -)
    if (*str == '-') {
        sign = -1;
        str++; // Move to the next character
    }

    // Iterate through the string and convert characters to integers
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++; // Move to the next character
    }

    // Apply the sign
    return result * sign;
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

    for (int i = 0; i < (int) sizeof(operator_hash)/(int) sizeof(operator_hash[0]); i++) {
        if (operator == operator_hash[i].op) {
            return operator_hash[i].precedence;
        }
    }
    return 0;
}

int evaluate(int a, char operator, int b){
    int result;
    for (int i = 0; i < (int) sizeof(operator_hash) /(int) sizeof(operator_hash[0]); i++) {
        if (operator_hash[i].op == operator) {
            result = operator_hash[i].operation(a, b);
            break;
        }
    }
    return result;
}

bool compare_precedence(char op_1, char op_2) {

    if (precedence(op_2) >= precedence(op_1)) return true;
    return false;
}

void shunting_yard (char** string_array, node** output_queue, char* operator_stack) {
    int stk_index = 0;
    while (*string_array != NULL) { // while there are tokens to be read:
        
        char* token = *string_array; // read a token
        
        if (token[0] >= '0' && token[0] <= '9') { // INTEGER 
            enqueue(output_queue, token);


        } else if (*token == '(') { // LEFT PARENTHESIS
            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';


        } else if (*token == ')') { // RIGHT PARENTHESIS

            while (stk_index > 0 && operator_stack[stk_index - 1] != '(') {
                
                char temp[2] = {operator_stack[--stk_index], '\0'};
                char* copy = malloc(2);
                strcpy(copy, temp);

                enqueue(output_queue, copy);
                
                
                operator_stack[stk_index] = '\0';

                
            }
            // remove '('
            if (stk_index > 0 && operator_stack[stk_index - 1] == '(') {
                operator_stack[--stk_index] = '\0';
            }
                    
        } else if (*token == '+' || *token == '-' || *token == '*' || *token == '/' || *token == '%') { // OPERATOR

            while (operator_stack[stk_index - 1] != '(' && compare_precedence(*token, operator_stack[stk_index - 1])) {

                char temp[2] = {operator_stack[--stk_index], '\0'};
                char* copy = malloc(2);
                strcpy(copy, temp);

                enqueue(output_queue, copy);
               
                
                operator_stack[stk_index] = '\0';

            }

            operator_stack[stk_index++] = *token;
            operator_stack[stk_index] = '\0';

        }   


        string_array++;
    }

    // After while loop, pop everything from operator stack to output queue.
    while (stk_index > 0) {
        char temp[2] = {operator_stack[--stk_index], '\0'};
        char* copy = strdup(temp);

        enqueue(output_queue, copy);
        operator_stack[stk_index] = '\0';
    }
}

int eval_postfix(node* output_queue, size_t my_strlen, bool* zero_error){

    int* postfix_stack = (int*)malloc(my_strlen * sizeof(int));
    int stk_i = 0;
    
    // while output queue has stuff in it, push each item into the stack starting from the head until empty
    node* current = output_queue;
    while (current != NULL) {
        // INTEGER
        if (current->data[0] >= '0' && current->data[0] <= '9') {
            int push_me = my_atoi(current->data);

            postfix_stack[stk_i] = push_me;
            stk_i++;

        }

        // OPERATOR
        else {
            char operator = current->data[0];
 
            stk_i--;
            int b = postfix_stack[stk_i];
            stk_i--;
            int a = postfix_stack[stk_i];
            if (b == 0) {
                if (operator == '%' || operator == '/'){
                    *zero_error = true;
                    return -1;
                }
            }
            int result = evaluate(a, operator, b);
            postfix_stack[stk_i] = result;
            stk_i++;
        }
        
        current = current->next;
    }
    return postfix_stack[0];
}