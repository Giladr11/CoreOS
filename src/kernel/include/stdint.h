#ifndef STDINT_H
#define STDINT_H

#ifndef NULL
#define NULL ((void*)0) // Empty pointer
#endif 

typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef short int           int16_t;
typedef unsigned short int  uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef long long int       int64_t;
typedef unsigned long long  uint64_t;

typedef signed char         int_least8_t;
typedef unsigned char       uint_least8_t;
typedef short int           int_least16_t;
typedef unsigned short int  uint_least16_t;
typedef int                 int_least32_t;
typedef unsigned int        uint_32_t;
typedef long long int       int_least64_t;
typedef unsigned long long  uint_least64_t;

typedef int                 intptr_t;
typedef unsigned int        uintptr_t;

typedef long long int       intmax_t;
typedef unsigned long long  uintmax_t;              

#endif