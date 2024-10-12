#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

unsigned char isValidParenthesis(char *s);

int main(){
    printf("isValidParenthesis(\"%s\") = %s\n", "[[()]]{}", (isValidParenthesis("[[()]]{}")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "[[]]", (isValidParenthesis("[[]]")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "{}", (isValidParenthesis("{}")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "[}", (isValidParenthesis("[}")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "}", (isValidParenthesis("}")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "[", (isValidParenthesis("[")==true)?"true":"false" );

    printf("isValidParenthesis(\"%s\") = %s\n", "[[()]]{}[([([])])]", (isValidParenthesis("[[()]]{}[([([])])]")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "[[()]]{}[([(])])]", (isValidParenthesis("[[()]]{}[([(])])]")==true)?"true":"false" );
    printf("isValidParenthesis(\"%s\") = %s\n", "[[()]]{}[([()])])]", (isValidParenthesis("[[()]]{}[([()])])]")==true)?"true":"false" );

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
            //stack_print(top); /*Causes timeout*/
            if( s[i] == '(' || s[i] == '[' || s[i] == '{'){
                stack_push(&top , s[i]);
                if(pushCount == ~0){
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
