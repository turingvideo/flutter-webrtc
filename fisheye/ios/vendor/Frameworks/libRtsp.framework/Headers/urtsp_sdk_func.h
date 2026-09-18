#ifndef __URTSP_SDK_FUNC__
#define __URTSP_SDK_FUNC__

#include "URtsp/urtsp_com_def.h"

#ifdef  __cplusplus
extern "C"{
#endif

#ifdef WIN32
    #ifdef URTSP_EXPORTS_DLL
        #define URTSP_EXPORT        __declspec( dllexport )
    #else
        #define URTSP_EXPORT        __declspec( dllimport )
    #endif
#else
    #define URTSP_EXPORT     __attribute__((visibility("default")))
#endif

/**
* @brief  模块全局初始化
* @param  [IN] ULONG ulMaxSession   最大会话数
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_GlobalInit(IN ULONG ulMaxSession);

/**
* @brief  模块全局反初始化
* @return 无
* @note
*/
URTSP_EXPORT VOID STDCALL URtsp_GlobalUnInit();

/**
* @brief  设置日志等级
* @param  [IN] eLevel               日志等级，详见 UMD_LOG_LEVEL_E
* @return 详见错误码定义
* @note   1) 设置日志打印等级后，将会打印枚举值小于或等于当前等级的日志
*         2) 默认日志打印等级为UMD_LOG_INFO
*/
URTSP_EXPORT ULONG_32 STDCALL URtsp_SetLogLevel(IN UMD_LOG_LEVEL_E eLevel);

/**
* @brief  设置日志回调
* @param  [IN] pfCB                 回调函数指针
* @return 详见错误码定义
* @note   1) 该接口需要在URtsp_GlobalInit之前调用
*/
URTSP_EXPORT ULONG_32 STDCALL URtsp_SetLogCB(IN UMD_LOG_CB_PF pfCB);

/**
* @brief  获取空闲会话
* @param  [OUT] phHandle            会话句柄
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_Open(OUT HANDLE *phHandle);

/**
* @brief  获取空闲会话
* @param  [OUT] hHandle             会话句柄
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT VOID STDCALL URtsp_Close(OUT HANDLE hHandle);

/**
* @brief  启流
* @param  [IN] hHandle              会话句柄
* @param  [IN] pstStreamParam       启流参数
* @return 详见错误码定义
* @note   1) usRecvPort参数在udp启流/TCP带外时用到，为收流端口
*/
URTSP_EXPORT ULONG STDCALL URtsp_StartStream(IN HANDLE hHandle,  IN URTSP_STREAM_PARAM_S *pstStreamParam);

/**
* @brief  回放上墙
* @param  [IN] hHandle              会话句柄
* @param  [IN] pstTvParam           墙参数
* @param  [IN] pstToTvInfo          服务器响应墙信息
* @return 详见错误码定义
* @note   1) 该接口需要在启流接口后调用
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetStreamToTv(IN HANDLE hHandle, IN URTSP_TV_PARAM_S *pstTvParam, OUT URTSP_TO_TV_INFO_S *pstToTvInfo);

/**
* @brief  停流
* @param  [IN] hHandle              会话句柄
* @return 详见错误码定义
* @note   1) 
*/
URTSP_EXPORT ULONG STDCALL URtsp_StopStream(IN HANDLE hHandle);

/**
* @brief  流控
* @param  [IN] hHandle              会话句柄
* @param  [IN] eCtrl                流控类型
* @param  [IN] eScale               流倍速
* @param  [IN] tiPlayTime           流时间
* @return 详见错误码定义
* @note   1) 根据eCtrl类型填eScale和tiPlayTime参数；若eScale无需设置，填URTSP_SCALE_MAX；若tiPlayTime无需设置，填0
*         2) 暂停/恢复无需设置eScale和tiPlayTime
*         3) NVR单帧时间传-1，range带1970起始时间
*/
URTSP_EXPORT ULONG STDCALL URtsp_CtrlStream(IN HANDLE hHandle, IN URTSP_STREAM_CTRL_E eCtrl, IN URTSP_PLAY_SCALE_E eScale, TIME_T tiPlayTime);

/**
* @brief  设置交互密码
* @param  [IN] hHandle              会话句柄
* @param  [IN] pcUser               用户名
* @param  [IN] pcPassword           交互密码
* @return 详见错误码定义
* @note   1) 用户名、密码长度限制128字节
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetUserPassword(IN HANDLE hHandle, IN CHAR *pcUser, IN CHAR *pcPassword);

/**
* @brief  获取一体机直连设备IP和Port
* @param  [IN] hHandle              会话句柄
* @param  [OUT] pusPortS            设备起始端口
* @param  [OUT] pusPortE            设备结束端口
* @param  [OUT] pcIpAddr            设备IP
* @return 详见错误码定义
* @note   1) 设备IP由调用者分配存储空间，内存空间要求64字节
*/
URTSP_EXPORT ULONG STDCALL URtsp_GetPortWithIP(IN HANDLE hHandle,  OUT USHORT *pusPortS, OUT USHORT *pusPortE, OUT CHAR *pcIpAddr);

/**
* @brief  获取payload值和码流类型
* @param  [IN] hHandle              会话句柄
* @param  [INOUT] pulNum            媒体数量
* @param  [OUT] pstPayloadType      码流信息结构体指针
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_GetPayloadType(IN HANDLE hHandle,  INOUT ULONG *pulNum, OUT URTSP_PAYLOAD_TYPE_S *pstPayloadType);

/**
* @brief  获取音频信息
* @param  [IN] hHandle              会话句柄
* @param  [OUT] pstAudioInfo        音频信息结构
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_GetAudioInfo(IN HANDLE hHandle,  OUT URTSP_AUDIO_INFO_S *pstAudioInfo);

/**
* @brief  设置公私钥
* @param  [IN] pstRsaInfo           RSA信息
* @return 详见错误码定义
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetRsaKey(IN URTSP_RSA_INFO_S *pstRsaInfo);

/**
* @brief  查询此通道是否支持功能
* @param  [IN] ulSessionNo          会话号
* @param  [INOUT] pstFuncInfo       功能信息
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_IsFuncSupport(IN HANDLE hHandle,  INOUT URTSP_FUNC_SUP_S *pstFuncInfo);

/**
* @brief  获取UPNP映射后的端口和IP
* @param  [IN] pcProto              使用的协议TCP/UDP
* @param  [IN] pcRemoteIP           服务器IP
* @param  [IN] usLocalPort          局域网内部端口
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_GetUpnpExIPAndExPort(IN CONST CHAR *pcProto, IN CONST CHAR *pcRemoteIP, IN CONST USHORT usLocalPort);

/**
* @brief  清理映射表，删除已映射但未使用的端口
* @param  [IN] pcLocalIP            本地IP
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_FreeExternalPort(IN CHAR *pcLocalIP);

/**
* @brief  设置鉴权信息
* @param  [IN] hHandle              会话句柄
* @param  [IN] pstAuthInfo          鉴权信息
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetAuthInfo(IN HANDLE hHandle,  IN URTSP_AUTH_INFO_S *pstAuthInfo);

/**
* @brief  设置参数
* @param  [IN] hHandle              会话句柄
* @param  [IN] eOpt                 设置选项
* @param  [IN] ulParam              选项值
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetOptULONG(IN HANDLE hHandle,  IN URTSP_OPT_E eOpt, IN ULONG ulParam);

/**
* @brief  设置信号量名称
* @param  [IN] pcSignalName         信号量名称
* @return 详见错误码定义
* @note
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetSignalName(IN CHAR* pcSignalName);

/**
* @brief  设置通道事件回调
* @param  [IN] hHandle              会话句柄
* @param  [IN] pFunc                回调函数指针
* @param  [IN] pUserParam           用户参数
* @return 详见错误码定义
* @note   1) 该接口需要在URtsp_StartStream之前调用
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetEventCB(IN HANDLE hHandle, IN URTSP_EVENT_PF pFunc, VOID *pUserParam);

/**
* @brief  设置通道码流回调回调
* @param  [IN] hHandle              会话句柄
* @param  [IN] pFunc                回调函数指针
* @param  [IN] pUserParam           用户参数
* @return 详见错误码定义
* @note   1) 该接口需要在URtsp_StartStream之前调用
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetStreamCB(IN HANDLE hHandle, URTSP_DATA_OUTPUT_PF pFunc, VOID *pUserParam);

/**
* @brief  设置通道回调
* @param  [IN] hHandle              会话句柄
* @param  [IN] pcSendBuf            封装对讲数据
* @param  [IN] lSendLen             对讲数据长度
* @return 详见错误码定义
* @note   1) 该接口需要在URtsp_StartStream之前调用
*/
URTSP_EXPORT ULONG STDCALL URtsp_SendVoice(IN HANDLE hHandle,  IN CHAR *pcSendBuf, IN LONG_32 lSendLen);

/**
* @brief  设置RTSP消息字段
* @param  [IN] hHandle              会话句柄
* @param  [IN] pstRtspField         RTSP请求消息字段
* @return 详见错误码定义
* @note   1) 该接口需要在获取通道后，启流前调用，支持DESCRIBE和SETUP方法
*         2) 示例Key(Accept:)Value(application/sdp)
*         3) 如需设置多个请求字段，可多次调用设置
*/
URTSP_EXPORT ULONG STDCALL URtsp_SetRtspField(IN HANDLE hHandle,  URTSP_MSG_FIELD_S *pstRtspField);

#ifdef  __cplusplus
}
#endif /* end of __cplusplus */

#endif   /* __URTSP_SDK_FUNC__ */
