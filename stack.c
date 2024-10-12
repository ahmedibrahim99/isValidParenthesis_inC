/*------------------------Section: Includes--------------------------------*/
#include "stack.h"

/*------------------------Section: Global Variables------------------------*/


/*------------------------Section: Static Helper Functions Prototypes------*/


/*------------------------Section: API Interface Functions Definitions-----*/

void stack_push  (stack_t *topAddr, char data){
    /*Allocate new node*/
    stackNode_t *newNode = (stackNode_t *)malloc(sizeof(stackNode_t));

    if(newNode == NULL){ /*newNode is not allocated*/
        printf("Heap Overflow\n");
    }
    else if( topAddr!=  NULL ){ /* (topAddr) != NULL (stack exists) && newNode is allocated*/
        /*Place data in newNode*/
        newNode->data = data;
        /*Offset = offset to next node if stack is not empty, = 0 if stack is empty*/
        /*Offset = Old_Top_Address - New_Top_Address, if stack is not empty*/
        INT24_WRITE_WORD_ALIGNED( newNode->offsetToNext , (stack_isEmpty(*topAddr) == 0)?((*topAddr) - newNode ):0 );
    }
    else{ /* (*topAddr) equals NULL but newNode is allocated */
        free(newNode);
        newNode = NULL;
    }
    /*Update the stack top address*/
    (*topAddr) = newNode; /* The stack may become NULL if newNode not allocated */
}

stackData_t stack_pop   (stack_t *topAddr){
    /*Initially invalid node*/
    stackNode_t retNode = { .data = -1 , .offsetToNext = {0} };
    int32_t offset = 0;

    /*Check for a non-empty stack. Can not pop from empty stack*/
    if(topAddr != NULL && stack_isEmpty(*topAddr) == 0 ){
        /*Place data in retNode*/
        retNode.data = (*topAddr)->data;
        /*Keep the offset from stack top to its next node*/
        offset = INT24_READ_WORD_ALIGNED((*topAddr)->offsetToNext);
        /*free the stack top node*/
        free((*topAddr));
        /*Update the stack top address*/
        /*If no next node after the stack top, modify the stack top by NULL
         to prevent access to a freed node and to mark the stack as empty*/
        /*Else stack top = top + top's offsetToNext */
        (*topAddr) = (offset != 0)?( (*topAddr) + offset ):(NULL);
    }
    else{/*Nothing*/}
    return retNode.data;
}

stackData_t stack_peak  (stack_t top){
    /*Initially invalid node*/
    stackNode_t retNode = { .data = -1 , .offsetToNext = {0} };

    /*Check for a non-empty stack. Can not pop from empty stack*/
    if(stack_isEmpty(top) == 0 ){
        /*Place data in retNode*/
        retNode = (*top);
    }
    else{/*Nothing*/}

    return retNode.data;
}

void stack_print (stack_t top){
    /*If stack is not Empty*/
    if(stack_isEmpty(top) == 0){
        int32_t offset = 0;
        /*Repeat while node @ address top != 0*/
        while( (offset = INT24_READ_WORD_ALIGNED(top->offsetToNext)) != 0 ){
            printf("%c , ",top->data);
            top +=  offset ; /*GO to the next node*/
        }
        printf("%c\n", top->data); /*The last node with offset = 0 (Does not have a next node)*/
    }
    else{/*Nothing*/}
}

void stack_free  (stack_t top){
    /*If stack is not Empty*/
    if(stack_isEmpty(top) == 0){
        stackNode_t *next = NULL;
        int32_t offset = 0;
        /*Free nodes while they have an offset to next != 0*/
        while( (offset = INT24_READ_WORD_ALIGNED(top->offsetToNext)) != 0 ){
            next = top + offset;
            free(top);
            top = next;
        }
        free(top); /*The last node with offset = 0 (Does not have a next node)*/
    }
    else{/*Nothing*/}
}

/*------------------------Section: Static Helper Functions Definitions-----*/
