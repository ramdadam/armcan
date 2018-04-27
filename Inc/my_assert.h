/**
 * @file    my_assert.h
 * @brief   special ASSERT macro
 * @author  Dr. Klaus Schaefer schaefer@eit.h-da.de
 */

 #ifndef MY_ASSERT_H_
 #define MY_ASSERT_H_
 
 #undef ASSERT
 
 #ifdef DEBUG
 #define ASSERT(x) if((x)==0) asm("bkpt 0")
 #else
 #define ASSERT(x) ((void)(x))
 #endif
 
 #endif /* MY_ASSERT_H_ */
 