#pragma once

#define MACRO_CONCAT_(a, b) a ## b
#define MACRO_CONCAT(a, b) MACRO_CONCAT_(a, b)
#define MACRO_ADDLINENUM(a) MACRO_CONCAT(a, __LINE__)
#define MACRO_SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(*(arr)))
#define MACRO_EXPAND(...) __VA_ARGS__
#define MACRO_EMPTY() 
#define MACRO_DEFER(id) id MACRO_EMPTY()

#define MACRO_HASARGS(...) \
    MACRO_EXPAND(MACRO_DEFER(MACRO_ARGCOUNT_HELPER)( \
        _, ##__VA_ARGS__, MACRO_HASARGS_RSEQ_N() \
    ))

#define MACRO_ARGCOUNT_HELPER(_0, \
	__1, __2, __3, __4, __5, __6, __7, __8, \
	__9, _10, _11, _12, _13, _14, _15, _16, \
	_17, _18, _19, _20, _21, _22, _23, _24, \
	_25, _26, _27, _28, _29, _30, _31, _32, N, ...) N

#define MACRO_NARGS_RSEQ_N() \
	32, 31, 30, 29, 28, 27, 26, 25,  \
	24, 23, 22, 21, 20, 19, 18, 17,  \
	16, 15, 14, 13, 12, 11, 10,  9,  \
	 8,  7,  6,  5,  4,  3,  2,  1,  0,

#define MACRO_OVERLOAD_HASARGS(base, ...) \
    MACRO_CONCAT(base, MACRO_HASARGS(__VA_ARGS__))