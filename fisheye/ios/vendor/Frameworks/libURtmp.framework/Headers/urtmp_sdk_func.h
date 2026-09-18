#ifndef __URTMP_SDK_FUNC_H__
#define __URTMP_SDK_FUNC_H__

#ifdef  __cplusplus
extern "C"{
#endif

#include "URtmp/urtmp_com_def.h"

/** 编译选项 */
#if defined(WIN32)
    #if defined(URTMP_DLL_EXPORTS)
        #define URTMP_EXPORT        __declspec( dllexport )
    #else
        #define URTMP_EXPORT        __declspec( dllimport )
    #endif
#else
    #define URTMP_EXPORT            __attribute__((visibility("default")))
#endif

/**
* @brief  模块全局初始化
* @param  [IN] ulMaxSession         最大会话数
* @return 详见错误码定义
* @note   1) 最大会话数量不超过128路
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_GlobalInit(IN ULONG ulMaxSession);

/**
* @brief  模块全局反初始化
*/
URTMP_EXPORT VOID STDCALL URtmp_GlobalUnInit(VOID);

/**
* @brief  设置日志等级
* @param  [IN] eLevel               日志等级，详见 UMD_LOG_LEVEL_E
* @return 详见错误码定义
* @note   1) 设置日志打印等级后，将会打印枚举值小于或等于当前等级的日志
*         2) 默认日志打印等级为UMD_LOG_INFO
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_SetLogLevel(IN UMD_LOG_LEVEL_E eLevel);

/**
* @brief  设置日志回调
* @param  [IN] pfCB                 回调函数指针
* @return 详见错误码定义
* @note   1) 该接口需要在URtmp_GlobalInit之前调用
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_SetLogCB(IN UMD_LOG_CB_PF pfCB);


/**
* @brief  打开RTMP会话
* @param  [OUT] phHandle            会话句柄
* @return 详见错误码定义
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_Open(OUT HANDLE *phHandle);

/**
* @brief  关闭RTMP会话
* @param  [IN] hHandle              会话句柄
* @return 详见错误码定义
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_Close(IN HANDLE hHandle);

/**
* @brief  启流
* @param  [IN] hHandle              会话句柄
* @param  [IN] pstParam             启流参数
* @return 详见错误码定义
* @note   1) 目前仅支持实况/TCP拉流
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_StartStream(IN HANDLE hHandle, IN URTMP_STREAM_PARAM_S *pstParam);

/**
* @brief  停流
* @param  hHandle                   会话句柄
*/
URTMP_EXPORT LONG_32 STDCALL URtmp_StopStream(IN HANDLE hHandle);

/**
 * @brief  设置会话事件回调
 * @param  [IN] hHandle             会话句柄
 * @param  [IN] pFunc               回调函数指针
 * @param  [IN] pUserParam          用户参数
 * @return 详见错误码定义
 * @return 详见错误码定义
 * @note   1) 该接口需要在URtmp_StartStream之前调用
 */
URTMP_EXPORT LONG_32 STDCALL URtmp_SetEventCB(IN HANDLE hHandle, IN URTMP_EVENT_PF pFunc, IN VOID *pUserParam);

/**
 * @brief  设置会话码流回调
 * @param  [IN] hHandle             会话句柄
 * @param  [IN] pFunc               回调函数指针
 * @param  [IN] pUserParam          用户参数
 * @return 详见错误码定义
 * @note   1) 该接口需要在URtmp_StartStream之前调用
 */
URTMP_EXPORT LONG_32 STDCALL URtmp_SetStreamCB(IN HANDLE hHandle, IN URTMP_DATA_OUTPUT_PF pFunc, IN VOID *pUserParam);

#ifdef  __cplusplus
}
#endif /* end of __cplusplus */

#endif /* __URTMP_SDK_FUNC_H__ */
