#ifndef _INT_TYPES_H
#define _INT_TYPES_H

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

#endif /*_INT_TYPES_H*/
