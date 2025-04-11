#pragma once

///////////////////////////////////////////////////////////
#define BIT_OPERATOR(x)                        \
    inline x operator&(x a, x b) {             \
        return (x)((uint32_t)a & (uint32_t)b); \
    }                                          \
    inline x operator|(x a, x b) {             \
        return (x)((uint32_t)a | (uint32_t)b); \
    }                                          \
    inline x operator~(x a) {                  \
        return (x)(~(uint32_t)a);              \
    }                                          \
    inline x& operator&=(x& a, x b) {          \
        return a = a & b;                      \
    }                                          \
    inline x& operator|=(x& a, x b) {          \
        return a = a | b;                      \
    }

///////////////////////////////////////////////////////////
#define NON_COPYABLE(x)              \
    x(const x&) = delete;            \
    x& operator=(const x&) = delete; \
    x(x&&) = default;                \
    x& operator=(x&&) = default;

///////////////////////////////////////////////////////////
#define NON_MOVABLE(x)               \
    x(const x&) = delete;            \
    x& operator=(const x&) = delete; \
    x(x&&) = delete;                 \
    x& operator=(x&&) = delete;

///////////////////////////////////////////////////////////
#define EXPAND(x) x

#ifndef DOXYGEN_SKIP

    ///////////////////////////////////////////////////////////
    #define __CONCAT(a, b) a##b

    ///////////////////////////////////////////////////////////
    #define NARGS_COUNT( \
        _1,              \
        _2,              \
        _3,              \
        _4,              \
        _5,              \
        _6,              \
        _7,              \
        _8,              \
        _9,              \
        _10,             \
        _11,             \
        _12,             \
        _13,             \
        _14,             \
        _15,             \
        _16,             \
        _17,             \
        _18,             \
        _19,             \
        _20,             \
        _21,             \
        _22,             \
        _23,             \
        _24,             \
        _25,             \
        _26,             \
        _27,             \
        _28,             \
        _29,             \
        _30,             \
        _31,             \
        _32,             \
        _33,             \
        _34,             \
        _35,             \
        _36,             \
        _37,             \
        _38,             \
        _39,             \
        _40,             \
        x,               \
        ...              \
    )                    \
        x

    ///////////////////////////////////////////////////////////
    #define NARGS_EXPAND_ARGS(...) \
        EXPAND(NARGS_COUNT(        \
            __VA_ARGS__,           \
            39,                    \
            38,                    \
            37,                    \
            36,                    \
            35,                    \
            34,                    \
            33,                    \
            32,                    \
            31,                    \
            30,                    \
            29,                    \
            28,                    \
            27,                    \
            26,                    \
            25,                    \
            24,                    \
            23,                    \
            22,                    \
            21,                    \
            20,                    \
            19,                    \
            18,                    \
            17,                    \
            16,                    \
            15,                    \
            14,                    \
            13,                    \
            12,                    \
            11,                    \
            10,                    \
            9,                     \
            8,                     \
            7,                     \
            6,                     \
            5,                     \
            4,                     \
            3,                     \
            2,                     \
            1,                     \
            0                      \
        ))

    ///////////////////////////////////////////////////////////
    #define RECURSIVE_0(func, x, ...)
    #define RECURSIVE_1(func, x, ...) func(x, 0)
    #define RECURSIVE_2(func, x, ...) \
        func(x, 1) EXPAND(RECURSIVE_1(func, __VA_ARGS__))
    #define RECURSIVE_3(func, x, ...) \
        func(x, 2) EXPAND(RECURSIVE_2(func, __VA_ARGS__))
    #define RECURSIVE_4(func, x, ...) \
        func(x, 3) EXPAND(RECURSIVE_3(func, __VA_ARGS__))
    #define RECURSIVE_5(func, x, ...) \
        func(x, 4) EXPAND(RECURSIVE_4(func, __VA_ARGS__))
    #define RECURSIVE_6(func, x, ...) \
        func(x, 5) EXPAND(RECURSIVE_5(func, __VA_ARGS__))
    #define RECURSIVE_7(func, x, ...) \
        func(x, 6) EXPAND(RECURSIVE_6(func, __VA_ARGS__))
    #define RECURSIVE_8(func, x, ...) \
        func(x, 7) EXPAND(RECURSIVE_7(func, __VA_ARGS__))
    #define RECURSIVE_9(func, x, ...) \
        func(x, 8) EXPAND(RECURSIVE_8(func, __VA_ARGS__))
    #define RECURSIVE_10(func, x, ...) \
        func(x, 9) EXPAND(RECURSIVE_9(func, __VA_ARGS__))
    #define RECURSIVE_11(func, x, ...) \
        func(x, 10) EXPAND(RECURSIVE_10(func, __VA_ARGS__))
    #define RECURSIVE_12(func, x, ...) \
        func(x, 11) EXPAND(RECURSIVE_11(func, __VA_ARGS__))
    #define RECURSIVE_13(func, x, ...) \
        func(x, 12) EXPAND(RECURSIVE_12(func, __VA_ARGS__))
    #define RECURSIVE_14(func, x, ...) \
        func(x, 13) EXPAND(RECURSIVE_13(func, __VA_ARGS__))
    #define RECURSIVE_15(func, x, ...) \
        func(x, 14) EXPAND(RECURSIVE_14(func, __VA_ARGS__))
    #define RECURSIVE_16(func, x, ...) \
        func(x, 15) EXPAND(RECURSIVE_15(func, __VA_ARGS__))
    #define RECURSIVE_17(func, x, ...) \
        func(x, 16) EXPAND(RECURSIVE_16(func, __VA_ARGS__))
    #define RECURSIVE_18(func, x, ...) \
        func(x, 17) EXPAND(RECURSIVE_17(func, __VA_ARGS__))
    #define RECURSIVE_19(func, x, ...) \
        func(x, 18) EXPAND(RECURSIVE_18(func, __VA_ARGS__))
    #define RECURSIVE_20(func, x, ...) \
        func(x, 19) EXPAND(RECURSIVE_19(func, __VA_ARGS__))
    #define RECURSIVE_21(func, x, ...) \
        func(x, 20) EXPAND(RECURSIVE_20(func, __VA_ARGS__))
    #define RECURSIVE_22(func, x, ...) \
        func(x, 21) EXPAND(RECURSIVE_21(func, __VA_ARGS__))
    #define RECURSIVE_23(func, x, ...) \
        func(x, 22) EXPAND(RECURSIVE_22(func, __VA_ARGS__))
    #define RECURSIVE_24(func, x, ...) \
        func(x, 23) EXPAND(RECURSIVE_23(func, __VA_ARGS__))
    #define RECURSIVE_25(func, x, ...) \
        func(x, 24) EXPAND(RECURSIVE_24(func, __VA_ARGS__))
    #define RECURSIVE_26(func, x, ...) \
        func(x, 25) EXPAND(RECURSIVE_25(func, __VA_ARGS__))
    #define RECURSIVE_27(func, x, ...) \
        func(x, 26) EXPAND(RECURSIVE_26(func, __VA_ARGS__))
    #define RECURSIVE_28(func, x, ...) \
        func(x, 27) EXPAND(RECURSIVE_27(func, __VA_ARGS__))
    #define RECURSIVE_29(func, x, ...) \
        func(x, 28) EXPAND(RECURSIVE_28(func, __VA_ARGS__))
    #define RECURSIVE_30(func, x, ...) \
        func(x, 29) EXPAND(RECURSIVE_29(func, __VA_ARGS__))
    #define RECURSIVE_31(func, x, ...) \
        func(x, 30) EXPAND(RECURSIVE_30(func, __VA_ARGS__))
    #define RECURSIVE_32(func, x, ...) \
        func(x, 31) EXPAND(RECURSIVE_31(func, __VA_ARGS__))

    ///////////////////////////////////////////////////////////
    #define CMP_0(op, x, y, ...)
    #define CMP_1(op, x, y, ...)
    #define CMP_2(op, x, y, ...) (x op y ? x : y)
    #define CMP_3(op, x, ...) \
        (EXPAND(CMP_2(op, __VA_ARGS__) op x ? CMP_2(op, __VA_ARGS__) : x))
    #define CMP_4(op, x, ...) \
        (EXPAND(CMP_3(op, __VA_ARGS__) op x ? CMP_3(op, __VA_ARGS__) : x))
    #define CMP_5(op, x, ...) \
        (EXPAND(CMP_4(op, __VA_ARGS__) op x ? CMP_4(op, __VA_ARGS__) : x))
    #define CMP_6(op, x, ...) \
        (EXPAND(CMP_5(op, __VA_ARGS__) op x ? CMP_5(op, __VA_ARGS__) : x))
    #define CMP_7(op, x, ...) \
        (EXPAND(CMP_6(op, __VA_ARGS__) op x ? CMP_6(op, __VA_ARGS__) : x))
    #define CMP_8(op, x, ...) \
        (EXPAND(CMP_7(op, __VA_ARGS__) op x ? CMP_7(op, __VA_ARGS__) : x))
    #define CMP_9(op, x, ...) \
        (EXPAND(CMP_8(op, __VA_ARGS__) op x ? CMP_8(op, __VA_ARGS__) : x))
    #define CMP_10(op, x, ...) \
        (EXPAND(CMP_9(op, __VA_ARGS__) op x ? CMP_9(op, __VA_ARGS__) : x))
    #define CMP_11(op, x, ...) \
        (EXPAND(CMP_10(op, __VA_ARGS__) op x ? CMP_10(op, __VA_ARGS__) : x))
    #define CMP_12(op, x, ...) \
        (EXPAND(CMP_11(op, __VA_ARGS__) op x ? CMP_11(op, __VA_ARGS__) : x))
    #define CMP_13(op, x, ...) \
        (EXPAND(CMP_12(op, __VA_ARGS__) op x ? CMP_12(op, __VA_ARGS__) : x))
    #define CMP_14(op, x, ...) \
        (EXPAND(CMP_13(op, __VA_ARGS__) op x ? CMP_13(op, __VA_ARGS__) : x))
    #define CMP_15(op, x, ...) \
        (EXPAND(CMP_14(op, __VA_ARGS__) op x ? CMP_14(op, __VA_ARGS__) : x))
    #define CMP_16(op, x, ...) \
        (EXPAND(CMP_15(op, __VA_ARGS__) op x ? CMP_15(op, __VA_ARGS__) : x))
    #define CMP_17(op, x, ...) \
        (EXPAND(CMP_16(op, __VA_ARGS__) op x ? CMP_16(op, __VA_ARGS__) : x))
    #define CMP_18(op, x, ...) \
        (EXPAND(CMP_17(op, __VA_ARGS__) op x ? CMP_17(op, __VA_ARGS__) : x))
    #define CMP_19(op, x, ...) \
        (EXPAND(CMP_18(op, __VA_ARGS__) op x ? CMP_18(op, __VA_ARGS__) : x))
    #define CMP_20(op, x, ...) \
        (EXPAND(CMP_19(op, __VA_ARGS__) op x ? CMP_19(op, __VA_ARGS__) : x))

#endif

///////////////////////////////////////////////////////////
#define NARGS(...) NARGS_EXPAND_ARGS(AUG(__VA_ARGS__))

///////////////////////////////////////////////////////////
#define FIRST_ARG(x, ...) x

///////////////////////////////////////////////////////////
#define NFIRST_ARGS(x, ...) EXPAND(__VA_ARGS__)

///////////////////////////////////////////////////////////
#define LOOP(expr, ...) \
    EXPAND(CONCAT(RECURSIVE_, NARGS(__VA_ARGS__))(expr, __VA_ARGS__))

///////////////////////////////////////////////////////////
#define LOOP_XTIMES(N, expr, ...) \
    EXPAND(CONCAT(RECURSIVE_, N)(expr, __VA_ARGS__))

///////////////////////////////////////////////////////////
#define MAX(...) EXPAND(CONCAT(CMP_, NARGS(__VA_ARGS__))(>, __VA_ARGS__))

///////////////////////////////////////////////////////////
#define MIN(...) EXPAND(CONCAT(CMP_, NARGS(__VA_ARGS__))(<, __VA_ARGS__))

///////////////////////////////////////////////////////////
/// \brief Concatenate two values
///
///////////////////////////////////////////////////////////
#define CONCAT(a, b) __CONCAT(a, b)

///////////////////////////////////////////////////////////
/// \brief Expand a list of arguments by prepending
///
///////////////////////////////////////////////////////////
#define AUG(...) _, __VA_ARGS__

///////////////////////////////////////////////////////////
/// \brief Stringify a value
///
///////////////////////////////////////////////////////////
#define STR(x) #x

#ifdef WIN32

    ///////////////////////////////////////////////////////////
    /// \brief File seperator value that depends on OS
    ///
    ///////////////////////////////////////////////////////////
    #define FILE_SEPERATOR '\\'
#else

    ///////////////////////////////////////////////////////////
    /// \brief File seperator value that depends on OS
    ///
    ///////////////////////////////////////////////////////////
    #define FILE_SEPERATOR '/'
#endif

///////////////////////////////////////////////////////////
/// \brief Expand an expression that contains a variadic template parameter
///
///////////////////////////////////////////////////////////
#define PARAM_EXPAND(expr)                       \
    {                                            \
        int _temp[] = {0, ((void)(expr), 0)...}; \
    }