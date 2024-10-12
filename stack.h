#ifndef _STACK_H
#define _STACK_H

/*------------------------Section: Includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "int_types.h"

/*------------------------Section: Macros----------------------------------*/
#define stack_isEmpty STACK_IS_EMPTY

/*------------------------Section: Function-Like Macros--------------------*/
#define STACK_IS_EMPTY(STK_TOP) ((STK_TOP) == (stack_t)NULL)

/*------------------------Section: Defined Types---------------------------*/
union listNode{
    struct{
        int24_t offsetToNext; /*Offset in terms of sizeof(listNode_t)*/
        char    data;
    };
    uint32_t    alignerMember; /*This is placed for alignment on 4-bytes*/
};
typedef union listNode listNode_t;

typedef listNode_t      stackNode_t;
typedef stackNode_t*    stack_t;

typedef char stackData_t;
/*------------------------Section: API Interface Functions Prototypes -----*/
void        stack_push  (stack_t *topAddr, char data);
stackData_t stack_pop   (stack_t *topAddr);
stackData_t stack_peek  (stack_t top);
void        stack_print (stack_t top);
void        stack_free  (stack_t top);

/*------------------------Section: Extern Variables Declarations ----------*/

#endif /*_STACK_H*/
