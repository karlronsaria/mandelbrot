// Source:
//    https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
//    https://stackoverflow.com/users/6003870/padymko
//    
// Retrieved:
//    2020_06_11

#include <stdio.h>

/**
 * Determination a platform of an operation system
 * Fully supported supported only GNU GCC/G++, partially on Clang/LLVM
 */

#if defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows
	#define WINDOWS
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows" // Windows
	#define WINDOWS
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
	#define WINDOWS
#elif defined(__ANDROID__)
    #define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
	#define ANDROID
#elif defined(__linux__)
    #define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
	#define LINUX
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
    #include <sys/param.h>
	#define UNIX
    #if defined(BSD)
        #define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
    #endif
#elif defined(__hpux)
	#define HPUX
    #define PLATFORM_NAME "hp-ux" // HP-UX
#elif defined(_AIX)
	#define AIX
    #define PLATFORM_NAME "aix" // IBM AIX
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #define PLATFORM_NAME "ios" // Apple iOS
		#define IOS
    #elif TARGET_OS_IPHONE == 1
        #define PLATFORM_NAME "ios" // Apple iOS
		#define IOS
    #elif TARGET_OS_MAC == 1
        #define PLATFORM_NAME "osx" // Apple OSX
		#define OSX
    #endif
#elif defined(__sun) && defined(__SVR4)
    #define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
	#define SOLARIS
#else
    #define PLATFORM_NAME NULL
#endif

// Return a name of platform, if determined, otherwise - an empty string
const char* get_platform_name() {
    return PLATFORM_NAME == NULL ? "" : PLATFORM_NAME;
}

// #if defined(WINDOWS) && !defined(_CONSOLE)
// #include <Windows.h>
// #define __WINAPP
// #define MAIN_ENTRY \
// int wWinMain(HINSTANCE h, HINSTANCE prev_h, PWSTR argv, int n_cmd_show)
// #else
// #define MAIN_ENTRY \
// int main(int argc, char** argv)
// #endif

// #ifdef __WINAPP
// #define COMMAND_LINE_ARGUMENTS(args) \
// 	{ \
// 		int argc; \
// 		auto lpwstr = CommandLineToArgvW(GetCommandLine(), &argc); \
// 		for (int i = 0; i < argc; ++i) { \
// 			auto wstr = std::wstring(lpwstr[i]); \
// 			args.push_back(std::string(wstr.begin(), wstr.end())); \
// 		} \
// 	}
// #else
// #define COMMAND_LINE_ARGUMENTS(args) \
// 	{ \
// 		for (int i = 0; i < argc; ++i) { \
// 			args.push_back(std::string(argv[i])); \
// 		} \
// 	}
// #endif
