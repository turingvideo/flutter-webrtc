#ifndef _UMPEG_SDK_FUNC_H
#define _UMPEG_SDK_FUNC_H

#include "common_def.h"

/* 函数标准调用约定 */
#ifdef WIN32
    #ifdef UMPEG_DLL_EXPORTS
        #define UMPEG_EXPORT     __declspec(dllexport)
    #else
        #define UMPEG_EXPORT     __declspec(dllimport)
    #endif
#elif defined(__EMSCRIPTEN__)
    #include <emscripten.h>
    #include <emscripten/threading.h>
    #define UMPEG_EXPORT          EMSCRIPTEN_KEEPALIVE
#else
    #if __GNUC__ >= 4
        #define UMPEG_EXPORT          __attribute__((visibility("default")))
    #else
        #define UMPEG_EXPORT
    #endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

/**
* @brief  模块初始化 
* @note   调用该接口初始化整个模块
* @return 详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMPEG_Init(void);

/**
* @brief  模块反初始化
* @note   调用该接口初始化整个模块
* @return 详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMPEG_UnInit(void);

/**
* @brief  设置日志等级 
* @param  eLevel            日志等级，详见 UMPEG_LOG_LEVEL_E
* @return 详见错误码定义
* @note   1) 设置日志打印等级后，将会打印枚举值小于或等于当前等级的日志
*         2) 默认日志打印等级为UMPEG_LOG_INFO
*/
UMPEG_EXPORT int STDCALL UMLog_SetLevel(IN UMPEG_LOG_LEVEL_E eLevel);

/**
* @brief  设置日志回调
* @param  pfUMWriteLogFunc     回调函数名
* @return 详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMLog_SetLogCallback(IN UMPEG_WRITE_LOG_CALLBACK_PF pfUMWriteLogFunc);

/**    
* @brief   获取硬件加速能力集检测信息
* @param  pstHWDevCapInfo     硬件加速支持情况信息
* @return 详见错误码定义
* @note   通过UMHW_DEVCAP_S可知道当前支持硬件加速情况
*/
UMPEG_EXPORT int STDCALL UMPEG_GetHWDevCapInfo(OUT UMPEG_HWDEV_CAP_S* pstHWDevCapInfo);

/**
* @brief  创建一个解码器实例
* @param  pstInitParam       初始化参数
* @param  phDecoder          解码器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
* @note   1) 使用UMDecoder_Close进行关闭
*         2) 仅支持H264、H265视频解码，AAC音频解码，详见枚举 UMPEG_ENC_FMT_E
*         3) 当解码线程数量设置为1时，使用单线程解码
*         4) 由于目前Safari浏览器不支持Worker的嵌套创建，所以无法使用多线程解码
*         5）AAC解码输出FLTP格式
*/
UMPEG_EXPORT int STDCALL UMDecoder_Open(IN UMDECODER_INIT_PARAM_S * pstInitParam,
                                        OUT HANDLE *phDecoder);
/**
* @brief  解码一帧数据
* @param  hDecoder          解码器句柄
* @param  pstFrameData      输入待解码数据
* @param  pstDecRaw         解码后数据信息
* @return ERR_UMPEG_SUCCESS           成功
*         ERR_UMPEG_NEED_MORE_DATA    需要输入更多数据
*         失败详见错误码定义
* @note   1) 如果启用多线程解码，由于解码器内部会进行缓存，所以前几次接口调用无数据返回 
*         2) 调用时如果解码器内部还未完成解码，该接口会阻塞
*/
UMPEG_EXPORT int STDCALL UMDecoder_DecodeOneFrame(IN HANDLE hDecoder,
                                                  IN UMDECODER_FRAME_S *pstFrameData,
                                                  OUT UMDECODER_RAW_S *pstDecRaw);

/**
* @brief  把硬解显存数据拷贝到内存
* @param  hDecoder          解码器句柄
* @param  pstHwDecRaw       硬解码后数据结构体
* @param  pstSwDecRaw       拷贝到内存数据
* @note   1) 硬解输出后的解码数据在显存中，可以调该接口拷贝到内存中
*/
UMPEG_EXPORT int STDCALL UMDecoder_TransHwRawBuff(IN HANDLE hDecoder,
                                                  IN UMDECODER_RAW_S *pstHwDecRaw,
                                                  OUT UMDECODER_RAW_S *pstSwDecRaw);

/**
* @brief  释放硬件解码占用的显存
* @param  hDecoder          解码器句柄
* @param  pstDecRaw         解码后数据结构体
* @note   1) 硬解输出后的解码数据使用完成后，需要调用该接口释放资源，否则会造成资源泄露
*/
UMPEG_EXPORT int STDCALL UMDecoder_FreeHWRawBuff(IN HANDLE hDecoder, IN UMDECODER_RAW_S *pstDecRaw);

/**
* @brief  使用硬件进行图像缩放
* @param  hDecoder          解码器句柄
* @param  pstSrcDecRaw      解码后数据结构体
* @param  pstDstDecRaw      解码后数据结构体
* @param  pcRgaBuffer       外部传入的RGA缩放缓冲区（由调用方分配和管理生命周期）
* @param  pulRgaBufferSize  外部传入的RGA缓冲区大小（输入为当前大小，输出为需要的最大大小）
* @note   1) 渲染时缩放到窗口大小时使用此方法在gpu中进行缩放，减少cpu的消耗及数据量的传输
*         2) pcRgaBuffer由外部管理，解码器不负责分配和释放，避免跨线程生命周期问题
*/
UMPEG_EXPORT int STDCALL UMDecoder_ScaleHWRawFrame(IN HANDLE hDecoder, IN UMDECODER_RAW_S *pstSrcDecRaw, IN UMDECODER_RAW_S *pstDstDecRaw, IN char *pcRgaBuffer, INOUT unsigned int *pulRgaBufferSize);

/**
* @brief  销毁解码器实例
* @param  hDecoder          解码器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMDecoder_Close(IN HANDLE hDecoder);

/**
* @brief  创建一个编码器实例
* @param  pstInitInfo      入参结构体，详见UMENCODER_INIT_PARAM_S
* @param  pHEncoder        编码器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
* @note   1) 使用UMEncoder_Close进行关闭
*         2) 仅支持音频G711[A|U]格式编码，详见枚举 UMPEG_ENC_FMT_E;
*/
UMPEG_EXPORT int STDCALL UMEncoder_Open(IN UMENCODER_INIT_PARAM_S * pstInitParam,
                                           OUT HANDLE *phEncoder);

/**
* @brief  编码一帧数据
* @param  hEncoder         编码器句柄
* @param  pstRawData       输入待编码数据
* @param  pstFrameData     编码后帧数据信息结构体
* @return ERR_UMPEG_SUCCESS           成功
*         ERR_UMPEG_NEED_MORE_DATA    需要输入更多数据
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMEncoder_EncodeOneFrame(IN HANDLE hEncoder, 
                                                  IN UMENCODER_RAW_S *pstRawData, 
                                                  OUT UMENCODER_FRAME_S *pstFrameData);

/**
* @brief  销毁编码器实例
* @param  hEncoder         编码器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMEncoder_Close(IN HANDLE hEncoder);

/**
* @brief  创建一个录制器实例
* @param  pstInitInfo      入参结构体，详见 UMRECORDER_INIT_PARAM_S
* @param  phRecorder       录制器句柄
* @return 成功             ERR_UMPEG_SUCCESS
*         失败详见错误码定义
* @note   1) 适用于录制数据写入缓存区场景，缓存区由录制器内部分配
*         2) 使用 UMRecorder_Close 进行关闭
*         3) 支持MP4、TS格式文件的录制，详见枚举 UMPEG_FILE_FMT_E
*         4) 如果无音频数据，请将音频的编码格式设置为 UMPEG_ENC_FMT_MAX
*         5) 初始化时未配置音频编码格式时，后续调用 UMRecorder_PacketOneFrame 输入的音频数据也不会被录制
*/
UMPEG_EXPORT int STDCALL UMRecorder_Open(IN UMRECORDER_INIT_PARAM_S *pstInitParam,
                                         OUT HANDLE *phRecorder);

/**
* @brief  录制一帧数据
* @param  hRecorder        录制器句柄
* @param  pstRecorderData  入参结构体，详见RECODER_DATA_S
* @return ERR_UMPEG_SUCCESS               成功
*         ERR_UMPEG_RECORD_REACH_LIMIT    录制达到预设大小限制
*         失败详见错误码定义
* @note   1) 当接口返回ERR_UMPEG_RECORD_REACH_LIMIT，本次接口调用输入数据不会写入缓存区
*         2) 当接口返回ERR_UMPEG_RECORD_REACH_LIMIT，如果要继续录制文件，需调用 UMRecorder_GetData 取出缓存区保存的录像数据
*         3) 实际录像大小与预设值可能存在偏差
*         4）调用完 UMRecorder_GetData 后，如果要继续录制，时间戳不需要清0，模块内部会自行根据记录的时间戳进行处理
*/
UMPEG_EXPORT int STDCALL UMRecorder_RecordOneFrame(IN HANDLE hRecorder, IN UMRECORDER_FRAME_S *pstRecFrame);

/**
* @brief  获取录制数据
* @param  hRecorder        录制器句柄
* @param  pstRecData       
* @return 成功返回ERR_UMPEG_SUCCESS，失败详见错误码定义
* @note   1) 调用侧操作完pstRecData后才能再继续调用 UMRecorder_RecordOneFrame 进行录制
*         2) 调用该接口后，下次调用 UMRecorder_RecordOneFrame 时会清空录制器内部已录制的内容
*/
UMPEG_EXPORT int STDCALL UMRecorder_GetData(IN HANDLE hRecorder, OUT UMRECORDER_DATA_S *pstRecData);

/**
 * @brief  销毁录制器实例
 * @param  hRecorder        录制器句柄
 * @return ERR_UMPEG_SUCCESS           成功
 *         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMRecorder_Close(IN HANDLE hRecorder);

/**
* @brief  创建一个捕获器实例
* @param  pstInitParam       初始化参数结构体
* @param  phCapturer         捕获器句柄
* @return 成功               ERR_UMPEG_SUCCESS
*         失败详见错误码定义
* @note   1) 适用于截图数据写入缓存区场景，缓存区由截图模块内部分配
*         2) 使用 UMCapturer_Close 关闭
*         3) 支持JPEG、BMP格式文件的截图，详见枚举 UMPEG_FILE_FMT_E
*/
UMPEG_EXPORT int STDCALL UMCapturer_Open(IN UMCAPTURER_INIT_PARAM_S *pstInitParam,
                                         OUT HANDLE *phCapturer);

/**
 * @brief  截图一帧数据
 * @param  hCapturer         捕获器句柄
 * @param  pstCapRaw         输入结构体，详见MCAPTURER_RAW_S
 * @param  pstCapData        截图数据结构体，详见UMCAPTURER_DATA_S
 * @return 详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMCapturer_Snatch(IN HANDLE hCapturer,
                                           IN UMCAPTURER_RAW_S *pstCapRaw, 
                                           OUT UMCAPTURER_DATA_S *pstCapData);

/**
 * @brief  销毁捕获器实例
 * @param  hCapturer         捕获器句柄
 * @return ERR_UMPEG_SUCCESS           成功
 *         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMCapturer_Close(IN HANDLE hCapturer);

/**
* @brief  创建RTP打包器句柄
* @param  UMPACKAGER_INIT_PARAM_S     打包器初始化参数结构体
* @param  phPackager                  打包器句柄
* @return 成功返回Rtp打包器句柄，失败返回NULL
* @note   1) 使用 UMPackager_Close 进行关闭
*/
UMPEG_EXPORT int STDCALL UMPackager_Open(IN UMPACKAGER_INIT_PARAM_S *pstInitParam,
                                         OUT HANDLE *phPackager);

/**
* @brief  RTP打包
* @param  hPacket                 RTP打包器句柄
* @param  UMPACKAGER_DATA_S       打包后Rtp格式数据，详见UMPACKAGER_DATA_S
* @return 详见错误码定义
* @note 
*/
UMPEG_EXPORT int STDCALL UMPackager_PacketData(IN HANDLE hPacket, OUT UMPACKAGER_DATA_S *pstPacketData);

/**
* @brief  销毁RTP打包器句柄
* @param  hPacket           RTP打包器句柄
* @return ERR_UMPEG_SUCCESS           成功
 *         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMPackager_Close(IN HANDLE hPacket);

/**
* @brief   创建一个解析器实例
* @param   pcPath            传入路径（文件或者url）
* @param   phParser          解析器句柄
* @return  ERR_UMPEG_SUCCESS           成功
*          失败详见错误码定义
* @note    1）使用 UMParser_Close 关闭 
*          2）支持本地文件和HTTP网络文件
*/
UMPEG_EXPORT int STDCALL UMParser_Open(IN char* pcPath, HANDLE *phParser);

/**
* @brief   获取解析信息
* @param   hParser            解析器句柄
* @param   pstInfo            解析信息结构体，详见UMPARSER_INFO_S
* @return  成功返回ERR_UMPEG_SUCCESS，失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMParser_GetInfo(IN HANDLE hParser, OUT UMPARSER_INFO_S *pstInfo);

/**
* @brief   获取解析后数据
* @param   hParser            解析器句柄
* @param   pstFrameData       解析帧信息，详见UMPARSER_FRAME_S
* @return  成功返回ERR_UMPEG_SUCCESS，失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMParser_GetOneFrame(IN HANDLE hParser, OUT UMPARSER_FRAME_S *pstFrameData);

/**
* @brief   对解析的文件进行控制操作
* @param   hParser            解析器句柄
* @param   pstParCtrl         控制参数信息，详见UMPARSER_CTRL_S
* @return  成功返回ERR_UMPEG_SUCCESS，失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMParser_Control(IN HANDLE hParser, IN UMPARSER_CTRL_S *pstParCtrl);

/**
* @brief   获取当前位置
* @param   hParser            解析器句柄
* @param   pllCurPos          当前位置信息
* @return  成功返回ERR_UMPEG_SUCCESS，失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMParser_Tell(IN HANDLE hParser, OUT long long *pllCurPos);

/**
* @brief   解析一帧数据
* @param   pcSrcData            待解析数据，详见UMPARSER_VIDEO_IFRAME_S
* @param   pstVideoInfo         数据解析信息，详见UMPARSER_VIDEO_INFO_S
* @return  ERR_UMPEG_SUCCESS 成功
*          ERR_UMPEG_UNSUPPORT_FMT  不支持的格式，其他见错误码
* @note    1）目前该接口仅支持H264、H265编码格式的视频数据解析
*          2）传入的数据必须是I帧
*/
UMPEG_EXPORT int STDCALL UMParser_ParseVideoFrame(IN UMPARSER_VIDEO_IFRAME_S *pcSrcData, 
                                                  OUT UMPARSER_VIDEO_INFO_S *pstVideoInfo);


/**
* @brief  销毁解析器实例
* @param  hParser           解析器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMParser_Close(IN HANDLE hParser);

/**
* @brief   创建一个缩放器实例
* @param   pstInitParam     缩放器初始化信息参数        
* @param   phScaler         缩放器句柄
* @return  成功             ERR_UMPEG_SUCCESS
*          失败详见错误码定义
* @note    编码器使用完后需要调用 UMScaler_Close 进行销毁
*/
UMPEG_EXPORT int STDCALL UMScaler_Open(IN UMSCALER_INIT_PARAM_S *pstInitParam,
                                       OUT HANDLE *phScaler);


/**
* @brief   进行图片缩放
* @param   hScaler          缩放器句柄    
* @param   pstSrcData       源图片数据  
* @param   pstDstData       目标图片数据     
* @return  ERR_UMPEG_SUCCESS 成功， 其他见错误码
*/
UMPEG_EXPORT int STDCALL UMScaler_ImageSWS(IN HANDLE hScaler,
                                           IN UMSCALER_SRCRAW_S *pstSrcData, 
                                           OUT UMSCALER_DESTRAW_S *pstDstData);

/**
* @brief  销毁缩放器实例
* @param  hScaler           缩放器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMScaler_Close(IN HANDLE hScaler);

/**
* @brief   打开一个重采样器实例
* @param   pstInitParam     重采样器初始化信息参数        
* @param   phResampler      重采样器
* @return  成功             ERR_UMPEG_SUCCESS
*          失败详见错误码定义
* @note    编码器使用完后需要调用 UMResampler_Close 进行销毁
*/
UMPEG_EXPORT int STDCALL UMResampler_Open(IN UMRESAMPLER_INIT_PARAM_S *pstInitParam,
                                          OUT HANDLE *phResampler);

/**
* @brief   重采样一帧音频源数据
* @param   hResampler       重采样器句柄    
* @param   pstSrcData       重采样前音频数据  
* @param   pstDstData       重采样后音频数据     
* @return  ERR_UMPEG_SUCCESS 成功， 其他见错误码
*/
UMPEG_EXPORT int STDCALL UMResampler_ResampleOneFrame(IN  HANDLE hResampler,
                                              IN  UMRESAMPLER_SRC_RAW_S* pstSrcData,
                                              OUT UMRESAMPLER_DST_RAW_S* pstDstData);

/**
* @brief  销毁重采样器实例
* @param  hResampler           重采样器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMResampler_Close(IN HANDLE hResampler);


/**
* @brief   创建一个转码句柄
* @param   pstInitParam     转码句柄初始化信息参数        
* @param   phTranscoder     转码器句柄
* @return  成功             ERR_UMPEG_SUCCESS
*          失败详见错误码定义
* @note    编码器使用完后需要调用 UMTranscoder_Close 进行销毁
*/
UMPEG_EXPORT int STDCALL UMTranscoder_Open(IN TRANSCODER_INIT_PARAM_S *pstInitParam,
                                           OUT HANDLE *phTranscoder);

/**
* @brief   转码一帧音频源数据
* @param   hTranscoder            转码句柄    
* @param   pstSrcFrameData       转码前音频数据  
* @param   pstDstFrameData       转码后音频数据     
* @return  ERR_UMPEG_SUCCESS 成功， 其他见错误码
*/
UMPEG_EXPORT int STDCALL UMTranscoder_TranscodeOneFrame(IN HANDLE hTranscoder, 
                                                IN UMDECODER_FRAME_S* pstSrcFrameData, 
                                                OUT UMENCODER_FRAME_S* pstDstFrameData);

/**
* @brief  销毁转码句柄
* @param  hTranscoder           重采样器句柄
* @return ERR_UMPEG_SUCCESS           成功
*         失败详见错误码定义
*/
UMPEG_EXPORT int STDCALL UMTranscoder_Close(IN HANDLE hTranscoder);

#ifdef __cplusplus
}
#endif

#endif /* end of _UMPEG_SDK_FUNC_H */