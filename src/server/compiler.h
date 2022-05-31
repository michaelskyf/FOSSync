#pragma once

/* Compiler specific quirks */

#ifdef __clang__
    #define typeof(x) __typeof__(x)
#else
    #define typeof(x) __typeof(x)
#endif

#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define static_assert(expr, ...) __static_assert(expr, ##__VA_ARGS__, #expr)
#define __static_assert(expr, msg, ...) _Static_assert(expr, msg)
