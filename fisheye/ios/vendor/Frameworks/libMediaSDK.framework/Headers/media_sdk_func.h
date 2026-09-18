#ifndef __MEDIA_SDK_FUNC_H__
#define __MEDIA_SDK_FUNC_H__

#ifdef  __cplusplus
extern "C"{
#endif

#include "media_sdk_com_def.h"


/*************************************************************************************
MediaSDK
***************************************************************************************/

/**
* 设置播放器日志参数 日志文件由播放器管理 \n
* @param [IN] ulLogLevel                日志级别，参考# MEDIASDK_LOGLEVEL_E
         [IN] pcLogPath                 日志文件路径
         [IN] ulLogFileSize             日志文件大小(单位 KB)
         [IN] ulLogFileNum              日志文件数量上限，达上限后满覆盖
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetLocalLogParam(IN ULONG_32 ulLogLevel, IN CHAR *pcLogPath, IN ULONG_32 ulLogFileSize, IN ULONG_32 ulLogFileNum);

/**
* 注册播放器内运行日志的回调函数，日志通过回调再写入文件 \n
* @param [IN] pfnWriteLogFunc           播放器内运行日志回调函数指针。
         [IN] ulLogLevel                日志级别，参考# MEDIASDK_LOGLEVEL_E
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) 为了防止日志信息记录遗漏，用户应在调用播放器其他接口函数之前调用该函数；
* -    2) 如果用户不调用该函数设置运行日志的回调函数，则播放器内部不记录任何日志信息；
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetWriteLogCB(IN MEDIASDK_WRITE_LOG_CALLBACK_PF pfnWriteLogFunc,IN ULONG_32 ulLogLevel);

/**
* 初始化播放器SDK资源 \n
* @param [IN] 无
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_Init(void);

/**
* 释放播放器SDK资源，在结束使用播放器SDK之前最后调用 \n
* @param [IN] 无
* @return 无
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_Cleanup(void);

/**
* 设置播放器资源号最大数量 \n
* @param [IN] ulMaxPortNum              资源号最大数量
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) 最大通道号数量不超过512
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetMaxPortNum(IN ULONG_32 ulMaxPortNum);

/**
* 创建资源号，用于后续业务标识 \n
* @param [IN] 无。
* @return 
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CreatPortRes(INOUT ULONG_32 *pulPort);

/**
* 销毁资源号，在停止业务后调用 \n
* @param [IN] pHandle                   播放句柄
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_DestroyPortRes(IN ULONG_32 ulPort);

/**
* 注册消息或异常消息回调函数 \n
* @param [IN] ulPort                    资源号
* @param [IN] pfRunInfoFunc             消息或异常消息回调函数指针
* @param [IN] pUserParam                用户参数
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) 不允许在设置给播放器库的回调函数中调用播放器库接口
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRunInfoCB(IN ULONG_32 ulPort, 
                                                       IN MEDIASDK_RUN_INFO_CALLBACK_PF pfnRunInfoFunc, 
                                                       IN VOID *pUserParam);

/**
* 设置用户名密码 \n
* @param [IN] ulPort                    资源号
* @param [IN] pcUser                    用户名
* @param [IN] pcPassword                交互密码
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) pcUser和pcPassword长度限制128字节
* -    2) 启流前调用：启流需要用户名密码、并且未通过启流URL携带，需通过该接口提前设置
* -    3) 流运行时调用：若流运行过程中交互密码变更，需通过该接口设置
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetUserPassword(IN ULONG_32 ulPort, 
                                                          IN CHAR *pcUser, 
                                                          IN CHAR *pcPassword);

/**
* 媒体启流接口 \n
* @param [IN] ulPort                    资源号
* @param [IN] ulStreamType              启流业务类型，对应# MEDIASDK_STREAM_TYPE_E 枚举中的值。
* @param [IN] pStreamParam              启流参数指针（所存放的参数与启流业务类型有关）
* @param [IN] bAsync                    异步标志（BOOL_TRUE: 异步 BOOL_FALSE: 同步）
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -     1) 参数pStreamParam所存放参数数据由启流业务类型而定，用户需要根据启流类型，填写对应启流参数。
* -        当ulStreamType值为# MEDIASDK_STREAM_RTSP_REALPLAY时，用户需将# MEDIASDK_RTSP_REALPLAY_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_RTSP_PLAYBACK 时，用户需将# MEDIASDK_RTSP_PLAYBACK_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_RTSP_DOWNLOAD 时，用户需将# MEDIASDK_RTSP_DOWNLOAD_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_RTSP_VOICE 时，用户需将# MEDIASDK_RTSP_VOICE_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_SMC_REALPLAY 时，用户需将# MEDIASDK_SMC_REALPLAY_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_SMC_PLAYBACK 时，用户需将# MEDIASDK_SMC_PLAYBACK_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_SMC_DOWNLOAD 时，用户需将# MEDIASDK_SMC_DOWNLOAD_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_SMC_VOICE 时，用户需将# MEDIASDK_SMC_VOICE_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_RTMP_REALPLAY 时，用户需将# MEDIASDK_RTMP_REALPLAY_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_HLS_PLAYBACK 时，用户需将# MEDIASDK_HLS_PLAYBACK_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_NET_REALPLAY 时，用户需将# MEDIASDK_NET_REALPLAY_S 结构体类型指针转换成pStreamParam传下来。
* -        当ulStreamType值为# MEDIASDK_STREAM_NET_VOICE 时，用户需将# MEDIASDK_NET_VOICE_S 结构体类型指针转换成pStreamParam传下来。
* -     2) 同步启流：接口启流成功后返回，通过返回值判断启流是否成功，可能存在耗时长的情况
* -        异步启流：接口创建启流线程后返回、通过上报判断是启流是否成功，接口不会出现阻塞
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StartStream(IN ULONG_32 ulPort, IN ULONG_32 ulStreamType, IN VOID *pStreamParam, IN BOOL_T bAsync);

/**
* @brief  回放上墙
* @param  [IN] ulPort                   资源号
* @param  [IN] pstTvParam               电视墙参数
* @param  [IN] pstToTvInfo              服务器响应墙信息
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note   1) 该接口需要在启流接口后调用
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetStreamToTv(IN ULONG_32 ulPort, IN MEDIASDK_TV_PARAM_S *pstTvParam, OUT MEDIASDK_TO_TV_INFO_S *pstToTvInfo);

/**
* 关闭媒体流但不停止渲染 \n
* @param [IN] ulPort                    资源号
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note     特别注意此接口适用于仅关闭网络连接但不关闭画面的场景,最终停止全部业务需要调用MediaSDK_StopStream
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CloseStream(IN ULONG_32 ulPort);

/**
* 媒体停流接口 \n
* @param [IN] ulPort                    资源号
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StopStream(IN ULONG_32 ulPort);

/**
* 播放媒体文件 \n
* @param  [IN] ulPort                   资源号
* @param  [IN] pFileParam               媒体文件参数。
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
  * -    1) 该接口适用于文件播放
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_PlayMediaFile(IN ULONG_32 ulPort, MEDIASDK_MEDIAFILE_S* pstFileParam);

/**
* 停止播放媒体文件 \n
* @param [IN] ulPort                    资源号
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
  * -    1) 该接口适用于文件播放
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StopPlayMediaFile(IN ULONG_32 ulPort);

/**
* 开始本地录像 \n
* @param [IN] ulPort                    资源号
* @param [IN] pstInitParam              录像初始化参数
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) 播放器将根据媒体流格式在文件名后面追加对应的后缀名。如TS流录像文件的后缀为.ts；
* -    2) 当播放处于暂停播放、快速播放、慢速播放或单帧播放状态时，不支持启动本地录像；
* -    3) 启动录像后，在录像过程中，通过#MediaSDK_SetPlayerRunMsgCB设置的回调函数上报消息# MEDIASDK_RUN_INFO_RECORD_VIDEO；携带的错误码：
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StartRecord(IN ULONG_32 ulPort, IN MEDIASDK_RECORD_INIT_PARAM_S *pstInitParam);

/**
* 停止本地录像 \n
* @param [IN] ulPort                    资源号
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StopRecord(IN ULONG_32 ulPort);

/**
* 截图/抓拍 \n
* @param [IN] ulPort                    资源号
* @param [IN] pcFileName                保存图像的文件路径，包括文件名（不带后缀名）。
* @param [IN] ulPicFormat               图像格式，需从# MEDIASDK_PIC_FORMATE_E 枚举中取值。
* @param [IN] eSaveType                 保存类型，需从# MEDIASDK_SNATCH_SAVE_TYPE_E 枚举中取值。
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SnatchOnce(IN ULONG_32 ulPort,
                                                     IN const CHAR *pcFileName,
                                                     IN ULONG_32 ulPicFormat,
                                                     IN ULONG_32 ulSaveType);

/**
* 暂停/恢复播放 \n
* @param [IN] ulPort                    资源号
* @param [IN] bPause                    BOOL_TRUE: 暂停, BOOL_FLASE: 恢复
* @param [IN] bTimeFlag                 BOOL_TRUE: 携带, BOOL_FLASE: 不携带
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
* -    1) 暂停时，bTimeFlag参数无效，传BOOL_FALSE即可
* -    2) 恢复时，bTimeFlag参数仅对RTSP协议启流有效，其他流类型/文件播放无效，传BOOL_FALSE即可
* -    3) RTSP流恢复时，IPC设备bTimeFlag传BOOL_FALSE，RTSP交互不携带RANGE字段
* -    4) RTSP流恢复时，NVR设备bTimeFlag传BOOL_TRUE，RTSP交互携带RANGE字段
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_PausePlay(IN ULONG_32 ulPort,IN BOOL_T bPause, BOOL_T bTimeFlag);

/**
* 跳转指定回放时间 \n
* @param [IN] ulPort                    资源号
* @param [IN] tiPlayTime                播放时间（单位: s）
* @return:
* -         ERR_MEDIASDK_SUCCEED        成功
* -         其他详见错误码信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPlayTime(IN ULONG_32 ulPort,IN TIME_T tiPlayTime);

/**
* 支持带倍速的回放跳转 \n
* @param [IN] ulPort                资源号
* @param [IN] ulPlayTime            播放时间（单位: s）
* @param [IN] ulSpeed               播放速度(见# MEDIASDK_PLAY_SCALE_E)
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
  * -    1) 该接口仅用于Rtsp流播放
  * -    2) NVR单帧时时间传-1
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_PlayTimeWithSpeed(IN ULONG_32 ulPort,IN TIME_T tiPlayTime,IN ULONG_32 ulSpeed);

/**
 * 设置播放速度 \n
 * @param [IN] ulPort                资源号
 * @param [IN] ulSpeed               播放速度(见# MEDIASDK_PLAY_SCALE_E)
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SpeedPlay(IN ULONG_32 ulPort, IN ULONG_32 ulSpeed);

/**
* 设置麦克风音量 \n
* @param [IN] ulPort        资源号
* @param [IN] lVolume       麦克风音量
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -    1) 该接口函数支持Windows和Linux。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetMicVolume(IN ULONG_32 ulPort, IN LONG_32 lVolume);

/**
* 获取麦克风音量 \n
* @param [IN] ulPort        资源号
* @param [IN] plVolume      麦克风音量
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -    1) 该接口函数支持Windows和Linux。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetMicVolume(IN ULONG_32 ulPort, INOUT LONG_32 *plVolume);

/*************************************************************************************
Audio play(need port and stream/file)
***************************************************************************************/
/**
 * 打开音频播放 \n
 * @param [IN] ulPort        资源号
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
*/
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OpenSound(IN ULONG_32 ulPort);

/**
 * 关闭音频播放 \n
 * @param [IN] ulPort        资源号
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
*/
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CloseSound(IN ULONG_32 ulPort);

/**
 * 获取声音播放音量 \n
 * @param [IN] ulPort                     资源号
 * @param [INOUT] plVolume                音量
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 该接口函数支持Windows和Linux。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetSoundVolume(IN ULONG_32 ulPort,INOUT LONG_32 *plVolume);

/**
 * 设置声音播放音量 \n
 * @param [IN] ulPort               资源号
 * @param [IN] lVolume              音量(0-255，默认128）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 该接口函数支持Windows和Linux。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetSoundVolume(IN ULONG_32 ulPort,IN LONG_32 lVolume);

/*************************************************************************************
call back(need port and stream/file)
***************************************************************************************/
/**
* 设置拼帧前媒体流数据回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnSourceDataCBFun       拼帧前媒体流数据回调函数的指针
* @param [IN] bContinue                是否继续进行后面的拼帧、解码和显示操作
* @param [IN] pUserParam               用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -     1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -     2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetSourceDataCB(IN ULONG_32 ulPort,
                                        IN MEDIASDK_SOURCE_DATA_CALLBACK_PF pfnSourceDataCBFun,
                                        IN BOOL_T bContinue,
                                        IN VOID* pUserParam);

/**
* 设置拼帧后视频数据回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnParseVideoDataCBFun    拼帧后视频数据回调函数的指针
* @param [IN] bContinue                 是否继续进行解码和显示操作
* @param [IN] lUserParam                用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -     1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -    2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetParseVideoDataCB(IN ULONG_32 ulPort,
                                            IN MEDIASDK_PARSE_VIDEO_DATA_CALLBACK_PF pfnParseVideoDataCBFun,
                                            IN BOOL_T bContinue,
                                            IN VOID* pUserParam);

/**
* 设置拼帧后音频数据回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnParseAudioDataCBFun    拼帧后音频数据回调函数的指针
* @param [IN] bContinue                 是否继续进行解码和播放操作
* @param [IN] lUserParam                用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -     1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -     2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetParseAudioDataCB(IN ULONG_32 ulPort,
                                            IN MEDIASDK_PARSE_AUDIO_DATA_CALLBACK_PF pfnParseAudioDataCBFun,
                                            IN BOOL_T bContinue,
                                            IN VOID* pUserParam);

/**
* 设置解码后视频媒体流数据回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnDecodeVideoDataCBFun   解码后视频媒体流数据回调函数的指针
* @param [IN] bContinue                 是否继续进行后面的显示操作
* @param [IN] lUserParam                用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -    1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -    2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDecodeVideoDataCB(IN ULONG_32 ulPort,
                                             IN MEDIASDK_DECODE_VIDEO_DATA_CALLBACK_PF pfnDecodeVideoDataCBFun,
                                             IN BOOL_T bContinue,
                                             IN VOID* pUserParam);

/**
* 设置解码后音频媒体流数据回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnDecodeAudioDataCBFun   解码后音频媒体流数据回调函数的指针
* @param [IN] bContinue                 是否继续进行后面的播放操作
* @param [IN] lUserParam                用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -     1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -     2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDecodeAudioDataCB(IN ULONG_32 ulPort,
                                             IN MEDIASDK_DECODE_AUDIO_DATA_CALLBACK_PF pfnDecodeAudioDataCBFun,
                                             IN BOOL_T bContinue,
                                             IN VOID* pUserParam);

/**
* 设置画图回调函数 \n
* @param [IN]  ulPort               资源号
* @param [IN] pfnDrawCBFun              画图回调函数的指针
* @param [IN] lUserParam                用户设置参数
* @return:
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
* -     1) 该函数在启流/文件播放之前或之后调用，在停流/停止文件播放时自动失效;
* -     2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDrawMediaCB(IN ULONG_32 ulPort,
                                       IN MEDIASDK_DRAW_CALLBACK_PF pfnDrawCBFun,
                                       IN VOID* pUserParam);

/**
 * 获取网络/文件流信息 \n
 * @param [IN]    ulPort                资源号
 * @param [INOUT] pstStreamInfo         流信息
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 */

MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetStreamInfo(IN ULONG_32 ulPort, INOUT MEDIASDK_STREAMINFO_S *pstStreamInfo);

/**
 * 获当前播放时间 \n
 * @param [IN] ulPort                   资源号
 * @param [INOUT] pdulPlayTime          当前播放时间，UTC时间
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
* -    1) 适用于回放流以及文件播放
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetPlayTime(IN ULONG_32 ulPort, INOUT DULONG *pdulPlayTime);

 /**
 * 设置播放图像流畅性或实时性 \n
 * @param [IN] ulPort                资源号
 * @param [IN] ulFluency             播放图像是实时性优先还是流畅性优先，需从# MEDIASDK_PICTURE_FLUENCY_E 枚举中取值
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 此接口必须在获取资源号之后设置才有效;
 * -    2) 如果用户不调用该函数设置播放图像流畅性，则播放器内部默认使用均衡模式。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPictureFluency(IN ULONG_32 ulPort, IN ULONG_32 ulFluency);

/**
 * 重置丢包率 \n
 * @param [IN] ulPort                   资源号
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_ResetLostPacketRate(IN ULONG_32 ulPort);

 /**
 * 暂停/恢复输入流播放（实况暂停/恢复） \n
 * @param [IN] ulPort    资源号
 * @param [IN] bPause    暂停标志（BOOL_TRUE暂停，BOOL_FALSE恢复）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 调用该接口后解码库内部正常解析码流，但不解码播放
 * -    2) 该接口仅用于网络流的暂停，不支持文件暂停
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_PauseLive(IN ULONG_32 ulPort, IN BOOL_T bPause);

/**
* 获取Mp4文件的绝对时间 \n
* @param [IN] fMp4File              MP4文件。
* @param [INOUT] pulRealTime        绝对时间
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息
* @note
 * -    1) 该接口适用于文件播放  改掉名字和注释
*/
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_MP4GetRealTime(IN const CHAR *pcFileName, INOUT ULONG_32 *pulRealTime);

/**
 * 获取文件播放总时长 \n
 * @param [IN] ulPort                   资源号
 * @param [INOUT] pulTotalTime          存放文件总的时间长度的指针（单位: s）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 该接口适用于文件播放
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetFileTime(IN ULONG_32 ulPort, INOUT ULONG_32 *pulTotalTime);

/**
 * 水印校验 \n
 * @param [IN] ulPort                   资源号
 * @param [IN] pcFilename               媒体文件名，建议长度260字节
 * @param [INOUT] pstMarkInfo           校验结果，具体说明参考 # MEDIASDK_MARK_INFO_S 说明
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 需要通过MediaSDK_CreatPortRes新建一个通道资源进行处理
 * -    2) 该接口属于同步调用，会阻塞线程，建议创建一个线程调用，校验过程中可调用MediaSDK_CheckMarkCancel取消
 * -    3) MEDIASDK_MARK_INFO_S下的pstUnmarkTimeList需要用户调用进行MediaSDK_FreeMarkInfo释放，否则会出现内存泄漏
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CheckMark(IN ULONG_32 ulPort,IN const CHAR *pcFilename,INOUT MEDIASDK_MARK_INFO_S *pstMarkInfo);

/**
 * 水印校验取消 \n
 * @param [IN] ulPort                   资源号
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CheckMarkCancel(IN ULONG_32 ulPort);

/**
 * 释放水印资源 \n
 * @param [INOUT] pstMarkInfo           校验结果，具体说明参考 # MEDIASDK_MARK_INFO_S 说明
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_FreeMarkInfo(INOUT MEDIASDK_MARK_INFO_S *pstMarkInfo);

/**
 * 设置窗口显示比例 \n
 * @param [IN] ulPort                   资源号
 * @param [IN] hWnd                     播放窗口句柄
 * @param [IN] lRenderScale             视频显示比例（见# MEDIASDK_RENDER_SCALE_E ）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 可以在播放过程中调用；
 * -    2) 指定窗口句柄必须是已被绑定好的，否则调用失败；
 * -    3) 该接口函数支持Windows和Linux。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRenderScale(IN ULONG_32 ulPort,
                                                        IN HANDLE hWnd,
                                                        IN LONG_32 lRenderScale);

/**
 * 获取窗口显示比例 \n
 * @param [IN] ulPort                   资源号
 * @param [IN] hWnd                     播放窗口句柄
 * @param [INOUT] plRenderScale         视频显示比例（参考# MEDIASDK_RENDER_SCALE_E ）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 可以在播放过程中调用；
 * -    2) 指定窗口句柄必须是已被绑定好的，否则调用失败；
 * -    3) 该接口函数支持Windows和Linux。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetRenderScale(IN ULONG_32 ulPort,
                                                        IN HANDLE hWnd,
                                                        INOUT LONG_32 *plRenderScale);

/**
 * 数字放大（设置显示区域） \n
 * @param [IN] ulPort                   资源号
 * @param [IN] hWnd                     播放窗口句柄
 * @param [IN] pstRect                  基于原始图片的放大区域（万分比），从上到下，从左到右均为0-10000（NULL为全部显示）
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 以原图片的长宽为基准的万分比（请区别于窗口显示的区域），
           例如显示图像的右下半部分，则left = 5000，right = 10000， top = 5000，bottom = 10000
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDigitalZoom(IN ULONG_32 ulPort,
                                                        IN HANDLE hWnd,
                                                        IN MEDIASDK_RECT_S *pstRect);

 /**
 * 设置录像文件大小 \n
 * @param [IN] ulPort        资源号
 * @param [IN] ulFileSize    录像文件大小（单位MB，范围从10MB到4000MB）
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息
 * @note
 * -    1) 建议在录像开始前设置
 * -    2) 无论是否设置，当大小达到最大值4000MB时，会自动分段
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRecordSize(IN ULONG_32 ulPort, IN ULONG_32 ulFileSize);

 /**
 * 设置录像文件时长 \n
 * @param [IN] ulPort        资源号
 * @param [IN] ulTime        录像文件时长（单位:分钟）
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 建议在录像开始前设置
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRecordDivByTime( IN ULONG_32 ulPort, IN ULONG_32 ulTime);

 /**
 * 获取通道录像状态 \n
 * @param [IN]    ulPort         资源号
 * @param [INOUT] pbRecording    是否还在录像
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetRecordStatus(IN ULONG_32 ulPort, INOUT BOOL_T *pbRecording);

 /**
 * 获取正在录像文件大小 \n
 * @param [IN]    ulPort                 资源号
 * @param [INOUT] pulRecordFileSize      录像文件大小
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口仅支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetRecordFileSize( IN ULONG_32 ulPort, INOUT DULONG *pulRecordFileSize);

 /**
 * 获取鱼眼矫正参数\n
 * @param [IN]    ulPort             资源号
 * @param [INOUT] pstCenterPoint     鱼眼图像圆的圆心
 * @param [INOUT] pulRadius          鱼眼图像圆的半径
 * @param [INOUT] pParm              预留鱼眼镜头参数
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetFishEyeParam(IN ULONG_32 ulPort,
                                                          INOUT MEDIASDK_POINT_S *pstCenterPoint,
                                                          INOUT ULONG_32 *pulRadius,
                                                          INOUT VOID *pParm);

/**
* 判断当前码流是否为鱼眼码流\n
* @param [IN]    ulPort             资源号
* @return 返回如下值：
* -         BOOL_TRUE           是鱼眼码流
* -         BOOL_FALSE          不是鱼眼码流
* @note
* -    1) 该接口需要在启流之后调用
*/
MEDIASDK_EXPORT BOOL_T STDCALL MediaSDK_IsFishEyeStream(IN ULONG_32 ulPort);

/**
 * 设置模式和安装位置\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulPtzMode             PTZ模式，参考# MEDIASDK_PTZ_MODE_E
 * @param [IN] ulFixMode             安装位置，参考# MEDIASDK_FIX_MODE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPtzAndFixMode(IN ULONG_32 ulPort,
                                                            IN ULONG_32 ulPtzMode,
                                                            IN ULONG_32 ulFixMode);

 /**
 * 获取模式和安装位置\n
 * @param [IN]    ulPort                 资源号
 * @param [INOUT] pulPtzMode             PTZ模式，参考# MEDIASDK_PTZ_MODE_E
 * @param [INOUT] pulFixMode             安装位置，参考# MEDIASDK_FIX_MODE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetPtzAndFixMode(IN ULONG_32 ulPort,INOUT ULONG_32 *pulPtzMode,INOUT ULONG_32 *pulFixMode);

 /**
 * 设置解码回调方式\n
 * @param [IN] ulPort                资源号
 * @param [IN] lRectify              设置解码回调类型
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) lRectify参考枚举MEDIASDK_VIDEO_CB_TYPE_E，默认为解码后数据直接回调
        2) 该接口在设置解码后回调MediaSDK_SetDecodeVideoDataCB后使用
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetVideoCallBackMode(IN ULONG_32 ulPort, IN ULONG_32 lRectify);

 /**
 * 鼠标位移操作\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulFlags               与实际标志位，暂时无意义
 * @param [IN] pstPoint              位移坐标
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OnMouseMove(IN ULONG_32 ulPort, IN ULONG_32 ulFlags, IN MEDIASDK_POINT_S *pstPoint);

 /**
 * 鼠标左键按下\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulFlags               与实际标志位，暂时无意义
 * @param [IN] pstPoint              位移坐标
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OnLButtonDown(IN ULONG_32 ulPort, IN ULONG_32 ulFlags, IN MEDIASDK_POINT_S *pstPoint);

 /**
 * 鼠标左键弹起\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulFlags               与实际标志位，暂时无意义
 * @param [IN] pstPoint              位移坐标
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OnLButtonUp(IN ULONG_32 ulPort, IN ULONG_32 ulFlags, IN MEDIASDK_POINT_S *pstPoint);

 /**
 * 鼠标中键滚动\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulFlags               与实际标志位，暂时无意义
 * @param [IN] sDelta                鼠标滚轮偏移度
 * @param [IN] pstPoint              位移坐标
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口暂不支持Linux
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OnMouseWheel(IN ULONG_32 ulPort, IN ULONG_32 ulFlags, IN SHORT sDelta, IN MEDIASDK_POINT_S *pstPoint);

 /**
 * 设置5ePtz显示模式\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulPtzMode             5ePtz显示模式，参考# MEDIASDK_PTZ_DISPLAY_MODE_E
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPtzDisplayMode(IN ULONG_32 ulPort, IN ULONG_32 ulPtzMode);

 /**
 * 设置手动模式保活时间\n
 * @param [IN] ulPort                资源号
 * @param [IN] ulKeepAliveTime       设置保活时间
 * @return：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPtzKeepAliveTime(IN ULONG_32 ulPort, IN ULONG_32 ulKeepAliveTime);

 /**
 * 设置元数据画图使能 \n
 * @param [IN] ulPort            播放通道号
 * @param [IN] bAble             使能标志（BOOL_TRUE使能，BOOL_FALSE关闭）
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetIVAEnable(IN ULONG_32 ulPort, IN BOOL_T bAble);

 /**
 * 设置元数据绘图显示类型参数 \n
 * @param [IN] ulPort        播放通道号
 * @param [IN] eShowParam    元数据显示类型参数，具体参考 MEDIASDK_IVA_SHOW_RULE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetIVAShowParam(IN ULONG_32 ulPort, IN MEDIASDK_IVA_SHOW_RULE_E eShowParam);

 /**
 * 使能智能标记 \n
 * @param [IN] ulPort            播放通道号
 * @param [IN] eTargetTag        目标标记显示类型参数，具体参考 MEDIASDK_TARGET_TAG_E
 * @param [IN] bEnable           是否显示目标标记类型

 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_EnableTargetTag(IN ULONG_32 ulPort, IN MEDIASDK_TARGET_TAG_E eTargetTag, IN BOOL_T bEnable);

 /**
 * 使能智数据OSD \n
 * @param [IN] ulPort            播放通道号
 * @param [IN] bEnable           使能参数（BOOL_TRUE使能，BOOL_FALSE关闭）
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 元数据使能开启时，调用该接口可设置显示OSD信息；
 * -    2) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_EnableIvaOsd(IN ULONG_32 ulPort, BOOL_T bEnable);

 /**
 * 设置POS信息使能。\n
 * @param [IN] ulPort                    播放通道号
 * @param [IN] bAble                     使能标志（BOOL_TRUE使能，BOOL_FALSE关闭）
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPosEnable(IN ULONG_32 ulPort, IN BOOL_T bAble);

 /**
 * 获取POS信息使能状态。\n
 * @param [IN] ulPort                    播放通道号
 * @param [OUT] bAble                    使能标志（BOOL_TRUE使能，BOOL_FALSE关闭）
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    3、该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetPosEnable(IN ULONG_32 ulPort, IN BOOL_T *pbAble);

 /**
 * POS信息配置。\n
 * @param [IN] ulPort                     资源号
 * @param [IN] pstPosInfo                  POS信息
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPosInfo(IN ULONG_32 ulPort, IN MEDIASDK_POS_INFO_S *pstPosInfo);

 /**
 * 设置智能元数据回调函数 \n
 * @param [IN] ulPort                    资源号
 * @param [IN] pfnIVADataCBFun           画图回调函数的指针
 * @param [IN] lUserParam                用户设置参数
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该函数在启流之前或之后调用，在停流时自动失效；
 * -    2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL；
 * -    3) IPC智能规则结果数据上报时，通过该接口设置回调。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetIvaDataCB(IN ULONG_32 ulPort, IN MEDIASDK_IVA_DATA_CALLBACK_PF pfnIVADataCBFun, IN VOID* pUserParam);

 /**
 * 设置元数据回调函数 \n
 * @param [IN] ulPort                    资源号
 * @param [IN] pfnIVADataCBFun           元数据回调的指针
 * @param [IN] lUserParam                用户设置参数
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该函数在启流之前或之后调用，在停流时自动失效；
 * -    2) 回调函数要尽快返回，如果要停止回调，可以把回调函数指针设为NULL；
 * -    3) SD卡、云台元数据上报时，通过该接口设置回调(目前仅移动端使用)。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetMetaDataCB(IN ULONG_32 ulPort, IN MEDIASDK_META_DATA_CALLBACK_PF pfnMetaDataCBFun, IN VOID* pUserParam);

 /**
 * 设置RSA公私钥\n
 * @param [IN] pstRsaInfo                RSA信息
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 用于加扰码流
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRsaKey(IN MEDIASDK_RSA_INFO_S *pstRsaInfo);

 /**
 * 获取RSA公私钥\n
 * @param [INOUT] pstRsaInfo             RSA信息
 * @return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 用于加扰码流
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetRsaKey(INOUT MEDIASDK_RSA_INFO_S *pstRsaInfo);

 /**
 * 设置文件播放密码\n
 * @param [IN] ulPort                    资源号
 * @param [IN] pcPassword                用户密码
 * @param [IN] lPasswordLen              用户密码长度
 *@return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口适用于加密文件播放
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetFilePassword(IN ULONG_32 ulPort, IN CHAR *pcPassword, IN LONG_32 lPasswordLen);

 /**
 * 设置图像质量\n
 * @param [IN] eQuality                  图像质量参数，具体参考 MEDIASDK_IMAGE_QUALITY_E
 *@return:
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPicQuality(IN MEDIASDK_IMAGE_QUALITY_E eQuality);

 /**
 * 设置结构化信息字号大小\n
 * @param [IN] ulPort            资源号
 * @param [IN] ulSize            结构化信息字号大小
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 该接口函数支持Windows
 * -    2）ulSize大小为万分比
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetIvaOsdSize(IN ULONG_32 ulPort, IN ULONG_32 ulSize);

 /**
 * 设置温度类型\n
 * @param [IN] ulPort            资源号
 * @param [IN] ulTempeType       温度类型（说明参考 #MEDIASDK_TEMPE_TYPE）
 * @param [IN] ulReserved1       预留参数1（供扩展）
 * @param [IN] ulReserved2       预留参数2（供扩展）
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 设置热成像温度类型；
 * -    2) 该接口函数支持Windows
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetTempeType(IN ULONG_32 ulPort, IN ULONG_32 ulTempeType,
                                                        IN ULONG_32 ulReserved1, IN ULONG_32 ulReserved2);

 /**
 * 设置热成像参数\n
 * @param [IN] ulPort             资源号
 * @param [IN] pstThermalParam    热成像参数结构体
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 * -    1) 元数据使能开启时，调用该接口可设置热成像业务参数
 * -    2) 该接口函数支持Windows
 * -    3) bShowTempeInfo默认开启，显示温度信息；若关闭，则仅显示画框
 * -    4) ulTempeInfoPlace控制【测温检测】信息显示位置，默认为0，显示在规则周边
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetThermalParam(IN ULONG_32 ulPort, IN MEDIASDK_THERMAL_PARAM_S *pstThermalParam);

 /**
 * 设置渲染模式 \n
 * @param [IN] eMode                     渲染模式参数，具体参考 MEDIASDK_RENDER_MODE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRenderMode(IN MEDIASDK_RENDER_MODE_E eMode);

 /**
 * GPU硬解使能 \n
 * @param [IN] bEnable                  BOOL_TRUE表示使能，BOOL_FALSE表示取消
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED       成功
 * -         其他详见错误码信息信息
 * @note
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_EnableGpuDecode(IN BOOL_T bEnable);

/**
* GPU硬解使能（扩展版） \n
* @param [IN] bEnable                  BOOL_TRUE表示使能，BOOL_FALSE表示取消
* @param [OUT] pstGpuAbleState         GPU使能状态信息，具体参考 MEDIASDK_GPU_ABLE_STATE_S
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED       成功
* -         其他详见错误码信息信息
* @note
* -    1) 与MediaSDK_EnableGpuDecode区别在于此接口会返回GPU支持哪些编码格式
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_EnableGpuDecodeEx(IN BOOL_T bEnable, OUT MEDIASDK_GPU_ABLE_STATE_S *pstGpuAbleState);

/**
 * 设置GPU模式 \n
 * @param [IN] eMode                     具体参考 MEDIASDK_GPU_MODE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetGpuMode(IN MEDIASDK_GPU_MODE_E eMode);

 /**
 * 影像调节 \n
 * @param [IN] ulPort                    资源号
 * @param [IN] pstAdjust                 影像调节参数指针，具体参考 MEDIASDK_VIDEO_ADJUST_S
 * @param [IN] bOpen                     是否打开影像调节
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 *      1、该函数需要在启流之后调用。
 * -    2、默认不启动影像调节。
 * -    3、该接口函数仅支持Windows。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetVideoAdjust(IN ULONG_32 ulPort, IN const MEDIASDK_VIDEO_ADJUST_S *pstAdjust, IN BOOL_T bOpen);

/**
* 获取影像调节参数 \n
* @param [IN] ulPort                    资源号
* @param [OUT] pstAdjust                影像调节参数指针，具体参考 MEDIASDK_VIDEO_ADJUST_S
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note
*      1、该函数需要在启流之后调用。
* -    2、该接口函数仅支持Windows。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_GetVideoAdjust(IN ULONG_32 ulPort, OUT MEDIASDK_VIDEO_ADJUST_S *pstAdjust);

/**
* 设置满屏水印显示
* @param [IN] ulPort        播放通道号
* @param [IN] pstWaterMark  满屏水印参数
* @return 返回如下结果：
* -         #ERR_MEDIASDK_SUCCEED                 成功
* -         #ERR_XP_FAIL_TO_SET_OSD             设置显示osd失败
* -         #ERR_XP_FAIL_TO_GET_PORT_RES        获得播放通道资源失败
* @note
* - 1、调用该接口开始满屏显示水印，直到调用IMOS_XP_StopWaterMark后满屏水印消失。
* - 2、该接口函数仅支持Windows。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_AddWaterMark(IN ULONG_32 ulPort,
                                                       IN const MEDIASDK_WATER_MARK_INFO_S *pstWaterMark);

/**
* 设置转封装码流输出的回调函数指针。\n
* @param [IN] ulPort                        资源号。
* @param [IN] pfnRecordDataCallback          媒体流数据回调函数的指针。
* @param [IN] lUserParam                    用户设置参数。
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetRecordDataCB(IN ULONG_32 ulPort,
                                                          IN MEDIASDK_RECORD_DATA_CALLBACK_PF pfnRecordDataCallback,
                                                          IN LONG_REAL lUserParam);

/**
* 设置码流丢包情况下拼帧策略。\n
* @param [IN] ulPacketLostSplitterPolicy  码流丢包情况下拼帧策略，需从#MEDIASDK_PKT_LOST_POLICY_E枚举中取值。
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note
* -    1、码流丢包情况下默认丢弃一个I帧组
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPacketLostSplitterPolicy(IN ULONG_32 ulPacketLostSplitterPolicy);

/**
* 设置是否启动抗丢包。\n
* @param [IN] bResistLost 是否启动抗丢包
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_ResistLost(IN BOOL_T bResistLost);

/**
* 设置播放视频的窗口句柄。\n
* @param [IN] ulPort    播放通道号。
* @param [IN] pWnd      播放窗口句柄。
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note 无。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPlayWnd(IN ULONG_32 ulPort, IN VOID* pWnd);

/**
* 设置通道的厂商信息。\n
* @param [IN] ulPort        播放通道号。
* @param [IN] pcDecoderTag  解码器标签。目前支持的厂商解码器有："h3c-v3"表示H3C V3版本解码器。
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note 如果用户没有调用该函数，则播放器内各通道默认使用H3C V3版本的解码器。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDecoderTag(IN ULONG_32 ulPort,
                                                        IN const CHAR *pcDecoderTag);

/**
* 设置指定播放通道的NAT类型。\n
* @param [IN] ulPort            播放通道号。
* @param [IN] ulNatMode         NAT类型，需从#MEDIASDK_NAT_MODE_E枚举中取值。
* @param [IN] pcNatSessionStr   NAT引流字符串。
* @return 返回如下错误码：
* -         #ERR_MEDIASDK_SUCCEED                 成功
* -         #ERR_MEDIASDK_NULL_PTR                输入参数无效
* -         #ERR_MEDIASDK_INVALID_PARAM           输入参数无效
* @note 引流字符串可以自行定义，但是收发双方必须保持一致；可以传空字符串，但指针不能为空。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetNatParam(IN ULONG_32 ulPort,
                                                           IN ULONG_32 ulNatMode,
                                                           IN const CHAR *pcNatSessionStr);

/**
* 设置sdk端通知报文的接收端地址 \n
* @param [IN] ulPort          播放通道号。
* @param [IN] pcFeedbackIP       抗丢包通知报文的接收端地址。
* @param [IN] usFeedbackPort      抗丢包通知报文的接收端监听端口。
* @return 返回如下错误码：
* -         #ERR_MEDIASDK_SUCCEED                 成功
* -         #ERR_MEDIASDK_NULL_PTR                输入参数无效
* -         #ERR_MEDIASDK_INVALID_PARAM           输入参数无效
* @note
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetFeedbackAddr(IN ULONG_32 ulPort,
                                                               IN const CHAR *pcFeedbackIP,
                                                               IN USHORT usFeedbackPort);

 /**
 * 设置KDM信息。\n
 * @param [IN] pcDevCode   设备编码。
 * @param [IN] pcDevIP     设备IP。
 * @param [IN] pcKDMAddr    KDM服务器IP。
 * @param [IN] ulKDMPort    KDM服务器端口。
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetKDMInfo(IN MEDIASDK_KDM_INFO_S *pstKDM);

 /**
 * 设置加扰的信息。\n
 * @param [IN] ulPort            播放通道号。
 * @param [IN] pcScrambleMode    加扰模式。
 * @param [IN] pcScrambleKey     加扰KEY(未异或摄像机编码)。
 * @param [IN] pcCameraCode      摄像机编码。
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note 引流字符串可以自行定义，但是收发双方必须保持一致；可以传空字符串，但指针不能为空。
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetScrambleInfo(IN ULONG_32 ulPort, IN MEDIASDK_SCRAMBLE_INFO_S *pstScramble);

/**
* 判断当前码流是否支持加扰 \\n
* @param [IN]    ulPort             资源号
* @return 返回如下值：
* -         BOOL_TRUE           支持加扰
* -         BOOL_FALSE          不支持加扰
* @note
* -    1) 该接口需要在启流之后调用
*/
MEDIASDK_EXPORT BOOL_T STDCALL MediaSDK_IsSupportScrambling(IN ULONG_32 ulPort);

/**
 * 设置解码模式。\n
 * @param [IN] ulPort                    播放通道号。
 * @param [IN] enDecodeFrameType         解码帧的类型。
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 */
 MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetDecodeFrameType(IN ULONG_32 ulPort, IN MEDIASDK_DECODE_FRAME_TYPE_E enDecodeFrameType);

/**
* 视频文件格式转封装\n
* @param [IN] ulPort                播放通道号。
* @param [IN] pcInputUrl            输入文件路径，如：扩展后缀名为"xxx.ts"
* @param [IN] pcOutPutUrl           输出文件路径，如：扩展后缀名为"xxx.mp4"
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
* @note
* -    1) 该接口函数支持全平台
* -    2）当前仅支持TS转封装成MP4
* -    3）通过上报ERR_REMUXING_FINISH通知转换成功
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_FileRemuxing(IN ULONG_32 ulPort, IN const CHAR * pcInputUrl, IN const CHAR *pcOutPutUrl);

/**
* 停止视频文件格式转封装\n
* @param [IN] ulPort              播放通道号。
* @return 返回如下错误码：
* -         ERR_MEDIASDK_SUCCEED         成功
* -         其他详见错误码信息信息
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_StopFileRemuxing(IN ULONG_32 ulPort);
/**
 * 设置缓存的帧数。\n
 * @param [IN] ulPort              播放通道号。
 * @param [IN] ulBuffer  帧数。
 * @return 返回如下错误码：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
 * @note 如果用户没有调用该函数，则播放器Mediasdk内各通道默认直接实况，不进行帧缓存。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetPacketBuffer(IN ULONG_32 ulPort, IN ULONG_32 ulBuffer);

/**
 * 设置缓存的帧数。\n
 * @param [IN] ulPort              播放通道号。
 * @param [IN] ulFrameNums  帧数。
 * @return 返回如下错误码：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
 * @note 如果用户没有调用该函数，则播放器Mediasdk内各通道默认直接实况，不进行帧缓存。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetCachedFrames(IN ULONG_32 ulPort, IN ULONG_32 ulFrameNums);

/**
 * 检查U盾是否可用。\n
 * @param [INOUT] pulUsable                    检查结果。1：可用, 0: 不可用。
 * @return 返回如下错误码：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
 * @note
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CheckUKeyUsable(INOUT ULONG_32 *pulUsable);

/**
 * 设置云服务器信息（Smc启流需要） \n
 * @param [IN] pcDomain                         域名
 * @param [IN] pcToken                          Token
 * @return:
 * -         ERR_MEDIASDK_SUCCEED               成功
 * -         其他详见错误码信息
 * @note     1) 该接口需要在初始化后、启流前调用
 *           2) 云服务信息变更时，需要调用接口设置更新
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetCloudServerInfo(IN CHAR *pcDomain, IN CHAR *pcToken);

/**
 * 输入音频数据 \n
 * @param [IN] ulPort                           播放通道号
 * @param [IN] pucData                          语音数据地址
 * @param [IN] ulDatalen                        语音数据长度
 * @return:
 * -         ERR_MEDIASDK_SUCCEED               成功
 * -         其他详见错误码信息
 * @note     1) 输入PCM音频数据长度需小于1600且为160倍数
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_InputVoiceData(IN ULONG_32 ulPort,
                                                         IN UCHAR *pucData,
                                                         IN ULONG_32 ulDatalen);


/**
 * 配置Hls相关选项
 * @param [IN] pcCachePath                      缓存路径
 * @param [IN] pcCacheLimit                     缓存大小上限（单位：MB）
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 * @note    1) 请在进行Hls起流之前调用；
 *          2) 如果未设置缓存路径，无法进行Hls起流；
 *          3) 默认缓存上限500MB，达到缓存上限后按照时间顺序删除最早的缓存；
 *          4) 缓存上限至少设置为100MB，低于该值按照100MB处理。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_ConfigHls(IN CONST CHAR *pcCachePath, IN ULONG_32 ulCacheLimit);

/**
 * 更新Hls播放参数
 * @param [IN] ulPort                           播放通道号
 * @param [IN] pstParam                         播放参数
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_UpdateHlsParam(IN ULONG_32 ulPort, IN MEDIASDK_HLS_PLAYBACK_S *pstParam);

/**
 * 设置网络自适应模式运行参数
 * @param [IN] ulPort                           播放通道号
 * @param [IN] eKey                             要设置的参数
 * @param [IN] ulValue                          参数值
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 * @note
 *          1) 该接口仅在通过MediaSDK_SetPictureFluency设置了MEDIASDK_PICTURE_NETADJUST后才可用。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetNetAdjustParam(IN ULONG_32 ulPort,
                                                            IN MEDIASDK_NETADJUST_KEY_E eKey,
                                                            IN ULONG_32 ulValue);

/**
 * 设置网络卡顿模块运行参数
 * @param [IN] ulPort                           播放通道号
 * @param [IN] eKey                             要设置的参数
 * @param [IN] ulValue                          参数值
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetNetStuckParam(IN ULONG_32 ulPort,
                                                           IN MEDIASDK_NETSTUCK_KEY_E eKey,
                                                           IN ULONG_32 ulValue);

 /**
 * 设置属性信息语言类型 \n
 * @param [IN] eLanguage                  具体参考 MEDIASDK_LANGUAGE_E
 * @return 返回如下错误码：
 * -         ERR_MEDIASDK_SUCCEED         成功
 * -         其他详见错误码信息信息
 * @note
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetSEILanguageType(IN MEDIASDK_LANGUAGE_E eLanguage);

/**
 * 设置流参数。\n
 * @param [IN] ulPort              播放通道号。
 * @param [IN] eStreamOptType      流参数类型。
 * @param [IN] dulOptVal            流参数值。
 * @return 返回如下错误码：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
 * @note 起流之前设置流需要的额外参数。
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetStreamOpt(IN ULONG_32 ulPort, IN MEDIASDK_STREAM_OPT_TYPE_E eStreamOptType, IN DULONG dulOptVal);

/**
* 关闭降噪功能 \n
* @param [IN] ulPort                    播放通道号
* @param [IN] bEnable                   是否使能降噪功能（TRUE表示关闭，打开请选择FALSE）
 * @return 返回如下错误码：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
* @note
* -    1) 调用此接口会稍微影响音量
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CloseNoiseReduction(IN ULONG_32 ulPort, IN BOOL_T bClose);


/**
* RTSP 通信模块  设置鉴权信息 \n
* @param [in] ULONG ulSessionNo     会话编号
* @param [in] MEDIASDK_AUTH_INFO_S *pcAuthprizationInfo  鉴权信息
* @return 返回如下结果：
 * -         #ERR_MEDIASDK_SUCCEED             成功
 * -         #ERR_MEDIASDK_INVALID_PARAM       输入参数非法
 * -         #ERR_MEDIASDK_PORT_INVALID        获得播放窗口的资源失败
* @note  
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_SetAuthprizationInfo(IN ULONG ulPort, IN MEDIASDK_AUTH_INFO_S *pcAuthprizationInfo);


/**
* 设置包缓存队列中的缓冲帧数(设置播放模式为自定义时生效)。\n
* @param [IN] ulPort                    播放通道号。
* @param [IN] ulNum                     包缓存队列中的缓冲帧数。
* @return:
* -         BOOL_TRUE         成功
* -         BOOL_FALSE        失败，请通过 NDPlayer_GetLastError 获取错误的详细信息
* @note
* -    1）包缓存队列的缓冲帧数设置范围是[6-25];
* -    2) 调用该函数前要设置播放模式为自定义-缓冲帧数可配或者网络抖动自适应模式。
*/
MEDIASDK_EXPORT BOOL_T STDCALL MediaSDK_SetPacketCacheFrameNum( IN ULONG_32 ulPort, IN ULONG_32 ulCacheFrameNum );

/**
* 仅对PCM音频进行编码，返回编码后数据，不打包不发送 \n
* @param [IN]  ulPort                     播放通道号
* @param [IN]  enVoiceEncType             音频编码类型
* @param [IN]  ulSampleRate               采样率
* @param [IN]  pucDataBuf                 PCM数据缓冲区
* @param [IN]  ulDataLen                  PCM数据长度（须小于1600且为160的倍数）
* @param [OUT] pucEncodedDataBuf          [输出] 编码后数据缓冲区（由调用者分配）
* @param [INOUT] pulEncodedDataLen        [输入] 编码缓冲区大小 / [输出] 编码后数据实际长度
* @return:
* -         ERR_MEDIASDK_SUCCEED               成功
* -         ERR_MEDIASDK_NULL_PTR              指针参数为空
* -         ERR_MEDIASDK_INVALID_PARAM         参数非法（编码类型不支持/数据长度非法/采样率非法/编码类型与已创建编码器不一致）
* -         ERR_MEDIASDK_GET_PORT_RES_FAIL     获取端口资源失败
* -         ERR_MEDIASDK_FAIL                   编码器创建失败或编码失败
* @note
* -    1) 无需启流，CreatPortRes后即可调用；
* -    2) 输入PCM数据长度须小于1600且为160的倍数；
* -    3) pucEncodedDataBuf由调用者分配，建议大小1500字节以上；
* -    4) 支持G711U/G711A/PCMA/AAC编码，编码类型由enVoiceEncType参数指定；
* -    5) 不允许在设置给播放器库的回调函数中调用该接口；
* -    6) 同一端口上，编码器在首次调用时按指定编码类型创建，后续调用编码类型须一致，
*         若需更换编码类型，需先DestroyPortRes释放资源再CreatPortRes。
*/
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_VoiceDataEncode(
    IN  ULONG_32  ulPort,
    IN  MEDIASDK_AUDIO_VOICE_TYPE_E enVoiceEncType,
    IN  ULONG_32  ulSampleRate,
    IN  const UCHAR *pucDataBuf,
    IN  ULONG_32  ulDataLen,
    OUT UCHAR     *pucEncodedDataBuf,
    INOUT ULONG_32 *pulEncodedDataLen);

/**
 * 打开输入流
 * @param [IN] ulPort                           播放通道号
 * @param [IN] eType                            数据类型 (具体见 MEDIASDK_INPUTDATA_TYPE_E）
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_OpenInputStream(IN ULONG_32 ulPort, IN MEDIASDK_INPUTDATA_TYPE_E eType);


/**
 * 帧数据输入接口
 * @param [IN] ulPort                           播放通道号
 * @param [IN] pstFrameData                     传入数据结构体指针,类型为VOID
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_InputData(IN ULONG_32 ulPort, IN VOID *pstData);



/**
 * 关闭输入流
 * @param [IN] ulPort                           播放通道号
 * @return:
 *          ERR_MEDIASDK_SUCCEED                成功
 *          其他详见错误码信息
 */
MEDIASDK_EXPORT ULONG_32 STDCALL MediaSDK_CloseInputStream(IN ULONG_32 ulPort);

#ifdef  __cplusplus
}
#endif /* end of __cplusplus */

#endif /* __MEDIA_SDK_FUNC_H__ */

