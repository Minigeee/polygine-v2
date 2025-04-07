#pragma once

// Platform detection macros
#if defined(_WIN32) || defined(__CYGWIN__)
    #define PLY_PLATFORM_WINDOWS 1
    #define PLY_PLATFORM_DESKTOP 1
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define PLY_PLATFORM_IOS 1
        #define PLY_PLATFORM_MOBILE 1
    #else
        #define PLY_PLATFORM_MACOS 1
        #define PLY_PLATFORM_DESKTOP 1
    #endif
#elif defined(__ANDROID__)
    #define PLY_PLATFORM_ANDROID 1
    #define PLY_PLATFORM_MOBILE 1
#elif defined(__linux__)
    #define PLY_PLATFORM_LINUX 1
    #define PLY_PLATFORM_DESKTOP 1
#else
    #error "Unsupported platform"
#endif