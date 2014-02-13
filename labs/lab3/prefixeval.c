#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//prefix evaluator with unary minus

//stack type for storing values
typedef struct stack_sto {
    struct stack_sto *next;
    int val;
} stack_sto, *stk;

int pop(stk *s) { //pop item off the stack
    stk toBeDeleted = *s;
    int val = toBeDeleted -> val; //set the return value before memory gets freed
    *s = (*s) -> next;
    free((void *) toBeDeleted);
    return val;
}

void push(stk *s, int v) {
    stk newVal = (stk) malloc(sizeof(stack_sto));
    //set the new values
    newVal -> val = v;
    newVal -> next = *s;
    //update the stack pointer
    *s = newVal;
    return;
}

int main(int argc, char *argv[]) {
    stk values = NULL; //let it point to null originally
    //I'm going to read the argument list backwards, because it leads to a more natural processing order.
    int p1, p2;
    for(int i = argc - 1; i > 0; i--) {
        switch(argv[i][0]) { //switch on the first char of the arg value
            case '+':
                push(&values,(pop(&values) + pop(&values))); //pop two off, add, push back on
                break;
            case '*':
                push(&values, (pop(&values) * pop(&values))); //same as above
                break;
            case '/':
                //non-commutativity doesn't matter here. the stack ensures the correct order
                push(&values, (pop(&values) / pop(&values)));
                break;
            case '-':
                if(argv[i][1] == '\0') { //do this only if it's an op, not a unary neg
                    push(&values, (pop(&values) - pop(&values)));
                    break;
                }
            default: //by assumption of syntactic correctness, it's a num
                push(&values, (int) strtol(argv[i], NULL, 10));
        }
    }
    printf("%d\n", pop(&values));
    exit(0);
}
