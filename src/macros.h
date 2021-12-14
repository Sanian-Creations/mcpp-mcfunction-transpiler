/*
  I switched from gcc 32bit to gcc 64bit halfway through making this, and I noticed that printing out size_t with printf caused trouble after the upgrade.
  
  32bit: size_t is and unsigned long int (4)
  64bit: size_t is and unsigned long long int (8)
  
  With size 4, %u works. With size 8, %llu should be used. However, the compiler I use doesn't support %llu, it seems to be a linux only thing. (One of these days I'm actually going to switch, MS is starting to piss me off) Luckily <inttypes.h> seems to have a macro that makes it work.
  
  Anyways, having to modify all strings in the project just to be able to use a different compiler is bothersome. So, if anyone else ever compiles this with 32bit, I want to save you the trouble of having to do that.

  If I did everything right, then this file should make it so that you can compile this with either a 64 bit or 32 bit compiler, the format used to print size_t dynamically adjusts itself to the environment.

*/

// https://stackoverflow.com/questions/1505582/determining-32-vs-64-bit-in-c

// Check windows
#if defined(_WIN32) || defined(_WIN64)
 #ifdef _WIN64
  #define ENVIRONMENT64
 #else
  #define ENVIRONMENT32
 #endif
#endif

// Check GCC
#ifdef __GNUC__
 #if defined(__x86_64__) || defined(__ppc64__)
  #define ENVIRONMENT64
 #else
  #define ENVIRONMENT32
 #endif
#endif

// define the suitable format-string for printing size_t
#include <inttypes.h>
#ifdef ENVIRONMENT64
 #define SIZE_T_FORMAT PRId64
#else 
 #define SIZE_T_FORMAT PRId32
#endif
