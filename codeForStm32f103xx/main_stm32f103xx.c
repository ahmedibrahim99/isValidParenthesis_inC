/*--------------------------------------------------------------------
------------------------File : int_types.h----------------------------
--------------------------------------------------------------------*/

/*------------------------Section: Includes-------------------------*/
#include <stdint.h>
#include <string.h>

/*------------------------Section: Macros---------------------------*/
#define INT24_SIGN_BIT_INDEX (23U)

/*------------------------Section: Function-Like Macros-------------*/

#define INT24_READ(INT24) \
((signed long int)(\
( (((unsigned char *)(INT24))[0]) \
+ ((((unsigned char *)(INT24))[1])<<8U) \
+(((((unsigned char *)(INT24))[2])&0x7F)<<16U)\
-(((((unsigned char *)(INT24)[2])&0x80) == 0)?(0):(1<<INT24_SIGN_BIT_INDEX)))))

/*Casting to 'int32_t' would perform sign extension*/
#define INT24_READ_WORD_ALIGNED(INT24) \
((int32_t)((((int24_cast_t *)(INT24))->val) -\
((((((int24_cast_t *)(INT24))->sign))== 0)?(0):(1<<INT24_SIGN_BIT_INDEX))))

#define UINT24_WRITE(UINT24, VAL) do{\
    (UINT24)[0] = (((VAL)>>0) &0xFF);\
    (UINT24)[1] = (((VAL)>>8) &0xFF);\
    (UINT24)[2] = (((VAL)>>16)&0xFF);\
}while(0);

#define INT24_WRITE_FROM_ADDR(INT24, ADDR) do{\
/* if((INT24) && (ADDR)) */ \
    memcpy((void *)(INT24), (void *)(ADDR), 3U);\
}while(0);

#define INT24_WRITE_WORD_ALIGNED(INT24,VAL) \
(((uint24_cast_t *)(INT24))->val) = (VAL)

#define INT24_WRITE_FROM_ADDR_WORD_ALIGNED(INT24,ADDR) \
(((uint24_cast_t *)(INT24))->val) =  (*((uint32_t *)(ADDR)))

/*------------------------Section: Defined Types---------------------------*/
typedef unsigned char uint24_t  [3];
typedef unsigned char int24_t   [3];

/*These types are not allocated,
  but used as a cast to place the 24-bit values directly
  in word-aligned uint24_t/int24_t without array access
 (These types are word-aligned)*/
typedef struct{
    uint32_t val   :24;
    uint32_t        :8;  /*Reserved, Not used*/
}uint24_cast_t;

typedef struct{
    uint32_t val   :23;
    uint32_t sign   :1;
    uint32_t        :8;  /*Reserved, Not used*/
}int24_cast_t;

/*------------------------Section: API Interface Functions Prototypes -----*/

/*------------------------Section: Extern Variables Declarations ----------*/

/*--------------------------------------------------------------------
------------------------File : stack.h----------------------------
--------------------------------------------------------------------*/

/*------------------------Section: Includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>


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

/*--------------------------------------------------------------------
------------------------File : main.c----------------------------
--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned char isValidParenthesis(char *s);

int main(){
	volatile bool state  = true;
	
	state = isValidParenthesis("[[()]]{}");
	state = isValidParenthesis("[[]]");
	state = isValidParenthesis("{}");
	state = isValidParenthesis("[}");
	state = isValidParenthesis("{");
	state = isValidParenthesis("[");
	state = isValidParenthesis("[[()]]{}[([([])])]");
	state = isValidParenthesis("[[()]]{}[([(])])]");
	state = isValidParenthesis("[[()]]{}[([()])])]");
	
	while(1){
		
	}
    return 0;
}

unsigned char isValidParenthesis(char *s){
    //printf("sizeof(stackNode_t) = %u\n",sizeof(stackNode_t));
    bool state = true ;
    stack_t top = NULL;
    uint16_t pushCount = 0; /*  Incremented on push, decremented on pop().
                                states whether stack is empty or not*/
    if(s && strlen(s) >= 2){

        /*stack_push(&top , s[0]);*/ /*So that it's not empty.
                                    (No need, pushing into empty stack (NULL) is handled)*/

        for(int i = 0 ; i < strlen(s) ; i++){
            //stack_print(top); /*Causes timeout !*/
            if( s[i] == '(' || s[i] == '[' || s[i] == '{'){
                stack_push(&top , s[i]);
                if(pushCount == 0xFFFFU){ /*Max UINT16 Value*/
                    perror("StackOverflow\n");
                    exit(-1);
                }
                else{
                    pushCount++;
                }
            }
            else{
                char topData = (stack_pop(&top));
                //pushCount = ( (pushCount > 0)?(pushCount -1 ):(0) );
                pushCount--; /*This may underflow, which also encodes poping from empty stack on ),] or } */
                if( topData == '(' && s[i] == ')' ){
                    continue;
                }
                else if( topData == '[' && s[i] == ']' ){
                    continue;
                }
                else if( topData == '{' && s[i] == '}' ){
                    continue;
                }
                else{
                    state = false;
                    break;
                }
            }
        }
    }
    else{
        state = false; /*Stack Not empty after all*/
    }
    if(pushCount != 0){ /*Stack Not empty after all*/
        state = false;
    }
    stack_free(top);
    return state;
}

/*--------------------------------------------------------------------
------------------------File : stack.c---------------------------
--------------------------------------------------------------------*/

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

stackData_t stack_peek  (stack_t top){
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
