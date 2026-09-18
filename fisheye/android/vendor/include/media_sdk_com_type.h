#ifndef __MEDIASDK_COM_TYPE_H__
#define __MEDIASDK_COM_TYPE_H__

#ifdef  __cplusplus
extern "C"{
#endif

#ifndef IN
#define     IN
#endif

#ifndef OUT
#define     OUT
#endif

#ifndef INOUT
#define     INOUT
#endif

#ifndef CHAR_DEF
#define CHAR_DEF
    typedef char                    CHAR;
#endif

#ifndef UCHAR_DEF
#define UCHAR_DEF
    typedef unsigned char           UCHAR;
#endif

/** @brief 表示宽字符结构 */
#ifndef WCHAR_DEF
#define WCHAR_DEF
    typedef wchar_t                 WCHAR;
#endif

#ifndef HANDLE_DEF
#define HANDLE_DEF
    typedef void*                   HANDLE;
#endif

#ifndef SHORT_DEF
#define SHORT_DEF
    typedef short                   SHORT;
#endif

#ifndef USHORT_DEF
#define USHORT_DEF
    typedef unsigned short          USHORT;
#endif

#ifndef LONG_32_DEF
#define LONG_32_DEF
    typedef int                     LONG_32;
#endif

#ifndef ULONG_32_DEF
#define ULONG_32_DEF
    typedef unsigned int            ULONG_32;
#endif

#ifndef LONG_DEF
#define LONG_DEF
    typedef long                    LONG;
#endif

#ifndef ULONG_DEF
#define ULONG_DEF
    typedef unsigned long           ULONG;
#endif

#ifndef TIME_DEF
#define TIME_DEF
    typedef long                    TIME_T;
#endif

#ifndef ULONG_REAL_DEF
#define ULONG_REAL_DEF
    #ifdef _WIN64
        typedef unsigned long long ULONG_REAL;
    #else
        typedef unsigned long ULONG_REAL;
    #endif
#endif

#ifndef LONG_REAL_DEF
#define LONG_REAL_DEF
    typedef long                    LONG_REAL;
#endif

#ifndef DLONG_DEF
#define DLONG_DEF
#if defined (_MSC_VER)
    typedef __int64                 DLONG;
#else
    typedef long long               DLONG;
#endif
#endif

#ifndef DULONG_DEF
#define DULONG_DEF
#if defined (_MSC_VER)
    typedef unsigned __int64        DULONG;
#else
    typedef unsigned long long      DULONG;
#endif
#endif

#ifndef DOUBLE_DEF
#define DOUBLE_DEF
    typedef double                  DOUBLE;
#endif

/** @brief 浮点数据类型 */
#ifndef FLOAT_DEF
#define FLOAT_DEF
    typedef float                   FLOAT;
#endif

#ifndef BOOL_T_DEF
#define BOOL_T_DEF
    typedef unsigned int            BOOL_T;
#endif

#ifndef BOOL_TRUE
#define BOOL_TRUE                   ((BOOL_T) 1)
#endif

#ifndef BOOL_FALSE
#define BOOL_FALSE                  ((BOOL_T) 0)
#endif

#ifndef VOID_DEF
#define VOID_DEF
    #define VOID                    void
#endif

#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif

#ifndef STATIC_DEF
#define STATIC_DEF
    #define STATIC                  static
#endif

#ifndef CONST_DEF
#define CONST_DEF
    #define CONST                   const
#endif

#ifdef _WIN64
    typedef unsigned long long UINTPTR_T;
#else
    typedef unsigned long UINTPTR_T;
#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of __MEDIASDK_COM_TYPE_H__ */