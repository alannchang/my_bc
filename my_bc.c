#include "helpers.h"

int main(int ac, char** av) {
    
    char* expression;
    if (ac == 2) expression = av[1];
    else {
        write(2, "invalid expression\n", 20);
        return 1;
    }
    
    // input string -------------> array of strings    
    size_t my_strlen = rm_space_and_strlen(expression);
    if (my_strlen < 1) {
        write(2, "parse error\n", 12);
        return 1;
    }

    if (infix_checker(expression) != 0){
        write(2, "parse error\n", 12);
        return 1;
    }

    char** string_array = make_str_arr(expression, my_strlen);

    // infix ----------> postfix using shunting yard
    char operator_stack[my_strlen];
    node* queue_head = NULL;
    shunting_yard(string_array, &queue_head, operator_stack);

    // evaluate postfix
    bool zero_error = false;
    int result = eval_postfix(queue_head, my_strlen, &zero_error);
    if (zero_error) {
        write(2, "divide by zero\n", 15);
        return 1;
    }
    printf("%d\n", result);

    free(string_array);
    free_linked_list(queue_head);

    return 0;
}