#ifndef VARIADIC_H
#define VARIADIC_H

#ifdef __INTELLISENSE__

// Some variadic argument utilities for cross-platform variadic use
#define __COUNT_CONT__(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _X, ...) _X
#define __COUNT__(...) __COUNT_CONT__(, ## __VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __VA_ANY__(...) __COUNT_CONT__(, ## __VA_ARGS__, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE)
#define __VA_COMMA_1__ ,
#define __VA_COMMA_0__
#define __VA_COMMA_TRUE__ __VA_COMMA_1__
#define __VA_COMMA_FALSE__ __VA_COMMA_0__
#define __VA_COMMA_EXPAND_CONT__(HAS_ARGS, ...) __VA_COMMA_ ## HAS_ARGS ## __ __VA_ARGS__
#define __VA_COMMA_EXPAND__(HAS_ARGS, ...) __VA_COMMA_EXPAND_CONT__(HAS_ARGS, __VA_ARGS__)

#define __VA_COMMA__(...) __VA_COMMA_EXPAND__(__VA_ANY__(__VA_ARGS__), __VA_ARGS__)
#else
#define __VA_COMMA__(...) __VA_OPT__(,) __VA_ARGS__
#endif

#endif