#ifndef _GENERICSTACK_
#define _GENERICSTACK_

#ifdef GENERIC_TYPE
    #undef GENERIC_TYPE
#endif
#ifdef GENERIC_NAME
    #undef GENERIC_NAME
#endif

#ifndef _STACK_CHAR_
    #define _STACK_CHAR_
    #define GENERIC_TYPE char
    #define GENERIC_NAME char
    #include "stack.h"
#endif

#ifdef GENERIC_TYPE
    #undef GENERIC_TYPE
#endif
#ifdef GENERIC_NAME
    #undef GENERIC_NAME
#endif

#ifndef _STACK_INT_
    #define _STACK_INT_
    #define GENERIC_TYPE int
    #define GENERIC_NAME int
    #include "stack.h"
#endif

#endif