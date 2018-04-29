

#ifndef LIB_gEFM_UTIL_H
#define LIB_gEFM_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>


#if (defined(WIN32) && !defined(NO_API)  && !defined(STATIC_LIB))
#if LIB_EXPORTS
#  define LIB_EXTERN __declspec(dllexport)
#else
#  define LIB_EXTERN __declspec(dllimport)

#endif
#else
#  define LIB_EXTERN
#endif

#if __cplusplus
#  define BEGIN_C_DECLS extern "C" { //
#  define END_C_DECLS   } //
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif

#endif

extern int globalErrorCode;
extern char *errorCodeStrings[];

using namespace std;

void *gEFMsafeCalloc(std::string msg, size_t numberOfElements, size_t elementSize);


