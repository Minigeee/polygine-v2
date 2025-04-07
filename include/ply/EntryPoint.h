#pragma once

#include <ply/core/Platform.h>

#ifdef PLY_PLATFORM_WINDOWS
    #define PLY_MAIN_DECL int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow)
    #define PLY_MAIN_CALL main(__argc, __argv)
#elif PLY_PLATFORM_ANDROID
    #define PLY_MAIN_DECL int SDL_main(int argc, char *argv[])
    #define PLY_MAIN_CALL main(argc, argv)
#else
    #define PLY_MAIN_DECL int main(int argc, char *argv[])
    #define PLY_MAIN_CALL main(argc, argv)
#endif

// Forward declaration of the actual main function
extern int main(int argc, char *argv[]);
