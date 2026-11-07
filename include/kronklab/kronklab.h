/*
** FREE PROJECT, 2026
** TESTYCROUSTY
** File description:
** Single-Header test framework
*/
#ifndef __kronklab_H
    #define __kronklab_H
    #include <assert.h>
    #include <stddef.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>

    #define klRed "\033[31m"
    #define klGreen "\033[32m"
    #define klYellow "\033[33m"
    #define klNormal "\033[0m"

    #define klError(msg, ...) printf(klRed "ERROR" klNormal ": " msg, ##__VA_ARGS__)
    #define klInfo(msg, ...) printf(klYellow "INFO" klNormal ": " msg, ##__VA_ARGS__)
    #define klGood(msg, ...) printf(klGreen "GOOD" klNormal ": " msg, ##__VA_ARGS__)

    #define Assert(condition, msg, ...) \
    do { \
        if (!(condition)) { \
            klError("Failed at %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
            exit(1); \
        } \
    } while(0)

    #define AssertEq(a, b, msg, ...) Assert(a == b, msg, ##__VA_ARGS__)
    #define AssertNe(a, b, msg, ...) Assert(a != b, msg, ##__VA_ARGS__)
    #define AssertLe(a, b, msg, ...) Assert(a <= b, msg, ##__VA_ARGS__)
    #define AssertGe(a, b, msg, ...) Assert(a >= b, msg, ##__VA_ARGS__)
    #define AssertNull(ptr, msg, ...) Assert(ptr == NULL, msg, ##__VA_ARGS__)
    #define AssertNotNull(ptr, msg, ...) Assert(ptr != NULL, msg, ##__VA_ARGS__)
    #define AssertStrEq(str1, str2, msg, ...) AssertEq(strcmp(str1, str2), 0, msg, ##__VA_ARGS__)
    #define AssertStrNe(str1, str2, msg, ...) AssertNe(strcmp(str1, str2), 0, msg, ##__VA_ARGS__)

#ifdef _WIN32
    #define __kl_SECTION ".kl$m"
#else
    #define __kl_SECTION "__kl_funcStruct"
#endif

    /* Places a static registry entry in section `sec` (MSVC and GCC/Clang spellings). */
    #ifdef _MSC_VER
        #define __kl_PLACE(sec) __pragma(section(sec, read)) __declspec(allocate(sec)) __declspec(align(8))
    #else
        #define __kl_PLACE(sec) __attribute__((section(sec), aligned(8), used, retain))
    #endif

    typedef void (*__kl_testFunc)(void);

    enum __kl_testType {
        __kl_test,
        __kl_bench
    };

    struct __kl_funcStruct {
        char suite[32];
        char name[32];
        enum __kl_testType type;
        __kl_testFunc func;
    };

    #define Test(suite, name) \
        void suite##_##name##_impl(void); \
        __kl_PLACE(__kl_SECTION) \
        static const struct __kl_funcStruct suite##_##name##_meta = { \
            #suite, #name, __kl_test, &suite##_##name##_impl \
        }; \
        void suite##_##name##_impl(void)

    #define Bench(suite, name) \
        void suite##_##name##_impl(void); \
        __kl_PLACE(__kl_SECTION) \
        static const struct __kl_funcStruct suite##_##name##_meta = { \
            #suite, #name, __kl_bench, &suite##_##name##_impl \
        }; \
        void suite##_##name##_impl(void)

#ifndef _WIN32
    extern struct __kl_funcStruct __start___kl_funcStruct;
    extern struct __kl_funcStruct __stop___kl_funcStruct;
#endif

    typedef uint64_t timestamp;

    timestamp kl_monotonic(void);
    timestamp kl_high_monotonic(void);

    #define BenchStart() \
        timestamp __kl_now_start = kl_high_monotonic();
    #define BenchEnd() \
        timestamp __kl_now_end = kl_high_monotonic() - __kl_now_start;
    #define BenchSeconds() __kl_now_end / 1000000
    #define BenchMilliSeconds() __kl_now_end / 1000
    #define BenchMicroSeconds() __kl_now_end
    #define BenchReport() klGood("-- %zu us elapsed\n", BenchMilliSeconds());

#endif /* __kronklab_H */
