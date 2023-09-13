#include "helpers.h"

int main(int ac, char** av) {
    
    char* str;
    if (ac == 2) str = av[1];
    else {
        write(2, "invalid expression\n", 20);
        return 1;
    }
    
    // input string -------------> infix expression as array of strings    
    size_t len = rm_space_and_strlen(str);
    if (len < 1) {
        write(2, "parse error\n", 12);
        return 1;
    }

    if (infix_checker(str) != 0){
        write(2, "parse error\n", 12);
        return 1;
    }

    char** string_array = make_str_arr(str, len);

    // infix expression ----------> postfix using shunting yard
    char operator_stack[len];
    node* queue_head = NULL;
    shunting_yard(string_array, &queue_head, operator_stack);

    // evaluate postfix
    bool divide_by_zero = false;
    int result = eval_postfix(queue_head, len, &divide_by_zero);
    if (divide_by_zero) {
        write(2, "divide by zero\n", 15);
        return 1;
    }

    printf("%d\n", result);

    free(string_array);
    free_linked_list(queue_head);

    return 0;
}