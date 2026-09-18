#ifndef _UPARSER_SDK_FUNC_H
#define _UPARSER_SDK_FUNC_H

#include "uparser_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
    #ifdef UPARSER_EXPORTS_DLL
        #define UPARSER_EXPORT        __declspec( dllexport )
    #else
        #define UPARSER_EXPORT        __declspec( dllimport )
    #endif
#else
    #define UPARSER_EXPORT        __attribute__((visibility("default")))
#endif

/**
 * @brief   打开一个解析拼帧器
 * @param   pstInitParam    解析拼帧器初始化参数
 * @param   phUParser       创建出的Parser句柄
 * @return  成功返回 ERR_UPARSER_SUCCESS，失败详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_Open(IN UPARSER_INIT_PARAM_S *pstInitParam, OUT HANDLE *phUParser);

/**
 * @brief   进行解析拼帧
 * @param   hUParser    解析拼帧器句柄
 * @param   pstIn       输入数据
 * @param   pstOut      拼帧完成数据
 * @return  成功返回 ERR_UPARSER_SUCCESS，
 *          需要更多数据返回 ERR_UPARSER_NEED_MOREPACKET，
 *          失败详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_ParserStream(IN HANDLE hUParser, IN UPARSER_PACKET_S *pstPkt,
                              OUT UPARSER_FRAME_S *pstFrame);

/**
 * @brief   为拼帧解析器中特定Parser设置参数
 * @param   hUParser  解析拼帧器句柄
 * @param   eKey    用于指定Parser类型和接口名的枚举
 * @param   pParam  要设置的参数，使用方法参考枚举 UPARSER_AUDIO_INFO_S 的注释
 * @return  成功返回 ERR_UPARSER_SUCCESS，失败详见错误码定义
 */
UPARSER_EXPORT ULONG_32 STDCALL UParser_SetParserOpt(IN HANDLE hUParser, IN UPARSER_OPT_KEY_E eKey, IN VOID *pParam);

/**
 * @brief   销毁拼帧解析器
 * @param   hUParser    拼帧解析器句柄
 * @return  成功返回 ERR_UPARSER_SUCCESS，失败详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_Close(IN HANDLE hUParser);

/**
 * @brief   设置UParser模块日志等级
 * @param   eLevel  日志等级
 * @return  成功返回 ERR_UPARSER_SUCCESS，失败详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_SetLogLevel(IN UMD_LOG_LEVEL_E eLevel);

/**
 * @brief   设置UParser模块日志回调函数
 * @param   pfLogCB 日志回调函数指针
 * @return  成功返回 ERR_UPARSER_SUCCESS，失败详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_SetLogCB(IN UMD_LOG_CB_PF pfLogCB);

/**
* @brief  设置UParser事件回调
* @param  [IN] hUParser             拼帧解析器句柄
* @param  [IN] pfCB                 回调函数指针
* @param  [IN] dlReserved           保留参数
* @param  [IN] pUserParam           用户参数
* @return 详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_SetEventCB(IN HANDLE hUParser, IN UPARSER_EVENT_CB_PF pfCB,
                                                   IN LONG_REAL dlReserved, VOID *pUserParam);

/**
* @brief  重置RTP视频流序号连续性检查
* @param  [IN] hUParser             拼帧解析器句柄
* @return 详见错误码定义
*/
UPARSER_EXPORT ULONG_32 STDCALL UParser_ResetSeqCheck(IN HANDLE hUParser);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _UPARSER_SDK_FUNC_H */