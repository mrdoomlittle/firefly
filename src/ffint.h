# ifndef __ffly__int__h
# define __ffly__int__h
typedef unsigned char ff_u8_t;
typedef signed char ff_i8_t;
typedef signed char ff_s8_t;

typedef unsigned short int ff_u16_t;
typedef short int ff_i16_t;
typedef short int ff_s16_t;

typedef unsigned int ff_u32_t;
typedef int ff_i32_t;
typedef int ff_s32_t;

typedef long long unsigned ff_u64_t;
typedef long long int ff_i64_t;
typedef long long int ff_s64_t;

# ifdef __ff8
typedef ff_u8_t ff_uint_t;
typedef ff_i8_t ff_int_t;
# elif __ff16
typedef ff_u16_t ff_uint_t;
typedef ff_i16_t ff_int_t;
# elif __ff32
typedef ff_u32_t ff_uint_t;
typedef ff_i32_t ff_int_t;
# elif __ff64
typedef ff_u64_t ff_uint_t;
typedef ff_i64_t ff_int_t;
# else
typedef ff_u32_t ff_uint_t;
typedef ff_i32_t ff_int_t;
# endif
# ifdef __cplusplus
namespace ff {
typedef ff_u64_t u64_t;
typedef ff_i64_t i64_t;
typedef ff_s64_t s64_t;

typedef ff_u32_t u32_t;
typedef ff_i32_t i32_t;
typedef ff_s32_t s32_t;

typedef ff_u16_t u16_t;
typedef ff_i16_t i16_t;
typedef ff_s16_t s16_t;

typedef ff_u8_t u8_t;
typedef ff_i8_t i8_t;
typedef ff_s8_t s8_t;

typedef ff_uint_t uint_t;
typedef ff_int_t int_t;
}
# endif
# endif /*__ffly__int__h*/
