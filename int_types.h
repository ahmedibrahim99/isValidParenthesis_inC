#ifndef _INT_TYPES_H
#define _INT_TYPES_H

/*------------------------Section: Includes-------------------------*/
#include <stdint.h>
#include <string.h>

/*------------------------Section: Macros---------------------------*/
#define INT24_SIGN_BIT_INDEX (23U)

/*------------------------Section: Function-Like Macros-------------*/
/* 1- int24_t Function-Like Macros */

/*General read: no alignment restrictions*/
#define INT24_READ(INT24) \
((int32_t)(\
( (((unsigned char *)(INT24))[0]) \
+ ((((unsigned char *)(INT24))[1])<<8U) \
+(((((unsigned char *)(INT24))[2])&0x7F)<<16U)\
-(((((unsigned char *)(INT24)[2])&0x80) == 0)?(0):(1<<INT24_SIGN_BIT_INDEX)))))

/*Read from the word-aligned address INT24 */
/*Casting to 'int32_t' would perform sign extension*/
#define INT24_READ_WORD_ALIGNED(INT24) \
((int32_t)((((int24_cast_t *)(INT24))->val) \
-((((((int24_cast_t *)(INT24))->sign))== 0)?(0):(1<<INT24_SIGN_BIT_INDEX))))

/*General write: no alignment restrictions*/
#define INT24_WRITE(INT24,VAL) do{\
    (INT24)[0] =  (VAL)&0xFF; \
    (INT24)[1] = ((VAL)&0xFF00)   >>8U; \
    (INT24)[2] = ((VAL)&0xFF0000) >>16U; \
}while(0);

/*Write to the word-aligned address INT24.
VAL is a literal integer value or a variable from the caller scope*/
#define INT24_WRITE_WORD_ALIGNED(INT24,VAL) do{\
    (((uint24_cast_t *)(INT24))->val) = (VAL);\
}while(0);

/*General copy from address ADDR to address INT24 with no alignment restrictions*/
#define INT24_WRITE_FROM_ADDR(INT24, ADDR) do{\
/* if((INT24) && (ADDR)) */ \
    memcpy((void *)(INT24), (const void *)(ADDR), 3U);\
}while(0);


/*Copy 24-bit value from the word-aligned address ADDR
to the word-aligned address INT24 with alignment restrictions on both addresses*/
#define INT24_WRITE_FROM_ADDR_WORD_ALIGNED(INT24,ADDR) do{\
    (((uint24_cast_t *)(INT24))->val) =  ((*((uint32_t *)(ADDR)))&0x00FFFFFFU);\
}while(0);

/* 2- uint24_t Function-Like Macros */

/*General read: no alignment restrictions*/
#define UINT24_READ(UINT24) \
 ((((unsigned char *)(UINT24))[0]) \
+((((unsigned char *)(UINT24))[1])<<8U)\
+((((unsigned char *)(UINT24))[2])<<16U))

/*Read from the word-aligned address UINT24 */
#define UINT24_READ_WORD_ALIGNED(UINT24) (((uint24_cast_t *)(UINT24))->val)

/*General write: no alignment restrictions*/
#define UINT24_WRITE(UINT24, VAL) do{\
    (UINT24)[0] = (((VAL)>>0) &0xFF);\
    (UINT24)[1] = (((VAL)>>8) &0xFF);\
    (UINT24)[2] = (((VAL)>>16)&0xFF);\
}while(0);

/*Write to the word-aligned address UINT24.
VAL is a literal integer value or a variable from the caller scope*/
#define UINT24_WRITE_WORD_ALIGNED(UINT24, VAL) do{\
    ((uint24_cast_t *)(UINT24))->cast = (VAL);\
}while(0);

/*General copy from address ADDR to address UINT24 with no alignment restrictions*/
#define UINT24_WRITE_FROM_ADDR(UINT24, ADDR) do{\
/* if((UINT24) && (ADDR)) */ \
    memcpy((void *)(UINT24), (const void *)(ADDR), 3U);\
}while(0);

/*Copy 24-bit value from the word-aligned address ADDR
to the word-aligned address UINT24 with word-alignment restrictions on both addresses*/
#define UINT24_WRITE_FROM_ADDR_WORD_ALIGNED(UINT24,ADDR) do{\
    (((uint24_cast_t *)(UINT24))->val) =  ((*((uint32_t *)(ADDR)))&0x00FFFFFFU);\
}while(0);
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
