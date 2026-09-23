#ifndef _UHLS_SDK_FUNC_H
#define _UHLS_SDK_FUNC_H

#include "uhls_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
    #ifdef UHLS_EXPORTS_DLL
        #define UHLS_EXPORT        __declspec( dllexport )
    #else
        #define UHLS_EXPORT        __declspec( dllimport )
    #endif
#else
    #define UHLS_EXPORT        __attribute__((visibility("default")))
#endif

/**
 * @brief   UHLS模块全局初始化 
 * @param   pstParam    初始化参数
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHLS_GlobalInit(IN UHLS_GLOBAL_PARAM_S *pstParam);

/**
 * @brief   UHLS模块全局反初始化
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHLS_GlobaUninit(VOID);

/**
 * @brief   创建UHLS模块句柄
 * @param   phUHls  UHLS模块句柄
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_Open(OUT HANDLE *phUHls);

/**
 * @brief   销毁UHLS模块句柄
 * @param   hUls    UHLS模块句柄
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT VOID STDCALL UHls_Close(IN HANDLE hUHls);

/**
 * @brief   设置TS包回调函数
 * @param   hUHls   UHLS模块句柄
 * @param   pfCB  回调函数指针
 * @param   pUserParam  用户参数
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_SetTsPacketCB(IN HANDLE hUHls, IN UHLS_DATA_OUTPUT_CB_PF pfCB, IN VOID *pUserParam);

/**
 * @brief   设置事件回调函数
 * @param   hUHls   UHLS模块句柄
 * @param   pfCB   回调函数指针
 * @param   pUserParam  用户参数
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_SetEventReportCB(IN HANDLE hUHls, IN UHLS_REPORT_CB_PF pfCB, IN VOID *pUserParam);

/**
 * @brief   播放控制
 * @param   hUHls   UHLS模块句柄
 * @param   eCmd    控制命令枚举
 * @param   Param   控制命令的附加参数，详见 UHLS_CONTROL_E 注释
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_Control(IN HANDLE hUHls, IN UHLS_CONTROL_E eCmd, IN UHLS_CONTROL_PARAM Param);

/**
 * @brief   设置播放结束时间
 * @param   hUHls   UHLS模块句柄
 * @param   ulEnd   播放结束时间
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
 * @note    若不设置则会一直播放到通过UHls_LoadM3u8设置的最后一个录像段
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_SetEndTime(IN HANDLE hUHls, IN ULONG_32 ulEndTime);

/**
 * @brief   加载m3u8信息
 * @param   hUHls   UHLS模块句柄
 * @param   pcM3u8  要加载的m3u8信息
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_LoadM3u8(IN HANDLE hUHls, IN CONST CHAR *pcM3u8);

/**
 * @brief   获取当前播放时间
 * @param   hUHls   UHLS模块句柄
 * @param   pulCurTime  当前播放时间
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_CurTime(IN HANDLE hUHls, OUT ULONG_32 *pulCurTime);

/**
 * @brief   设置日志打印等级
 * @param   eLevel  日志等级
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_SetLogLevel(IN UMD_LOG_LEVEL_E eLevel);

/**
 * @brief   设置日志回调函数
 * @param   pfLogCB 日志回调函数指针
 * @return  成功返回ERR_UHLS_SUCCESS，失败详见错误码定义
*/
UHLS_EXPORT ULONG_32 STDCALL UHls_SetLogCB(IN UMD_LOG_CB_PF pfLogCB);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _UHLS_SDK_FUNC_H */
