#ifndef _KERNEL_DEF
#define _KERNEL_DEF

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

// Integer definitions
typedef unsigned long long u64;
typedef signed long long   i64;
typedef unsigned long      u32;
typedef signed long        i32;
typedef unsigned short     u16;
typedef signed short       i16;
typedef unsigned char      u8;
typedef signed char        i8;

// typedef u32 size_t;

// Floating point definitions
typedef long double f128;
typedef double      f64;
typedef float       f32;

static inline u64 rdtsc() {
	u64 ret;
	__asm__ volatile( "rdtsc" : "=A"( ret ) );
	return ret;
}

#endif // _KERNEL_DEF
