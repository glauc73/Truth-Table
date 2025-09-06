#include <stdio.h>
#include <stdlib.h>

#define _MACRO_CONCAT(X, Y) X ## Y
#define CONCAT(X, Y) _MACRO_CONCAT(X, Y)

#ifndef GENERIC_NAME  
    #define GENERIC_NAME char
#endif
#ifndef GENERIC_TYPE
    #define GENERIC_TYPE char
#endif

#define STACK_TYPE CONCAT(stack_, GENERIC_NAME)

#define NEW_STACK CONCAT(new_, STACK_TYPE)
#define STACK_PUSH CONCAT(STACK_TYPE, _push)
#define STACK_POP CONCAT(STACK_TYPE, _pop)
#define DELETE_STACK CONCAT(delete_, STACK_TYPE)
#define ISEMPTY CONCAT(STACK_TYPE, _isempty)

typedef struct STACK_TYPE{
    GENERIC_TYPE data[256];
    int top;
    GENERIC_TYPE (*push)(struct STACK_TYPE*, GENERIC_TYPE);
    GENERIC_TYPE (*pop)(struct STACK_TYPE*);
}STACK_TYPE;

GENERIC_TYPE STACK_PUSH(STACK_TYPE* stk, GENERIC_TYPE x);
GENERIC_TYPE STACK_POP(STACK_TYPE* stk);

STACK_TYPE* NEW_STACK(){
    STACK_TYPE* stk = malloc(sizeof(STACK_TYPE));
    stk->top = -1;
    stk->push = CONCAT(STACK_TYPE, _push);
    stk->pop = CONCAT(STACK_TYPE, _pop);
    return stk;
}

GENERIC_TYPE STACK_PUSH(STACK_TYPE* stk, GENERIC_TYPE x){
    if(stk->top == 255)
        return EOF; 
    return stk->data[++stk->top] = x;
}   

GENERIC_TYPE STACK_POP(STACK_TYPE* stk){
    if(stk->top == -1)
        return -1;
    return stk->data[stk->top--];
}

int ISEMPTY(STACK_TYPE* stk){
    return stk->top == -1;
}

void DELETE_STACK(void *stk){
    if(stk) free(stk);
}
