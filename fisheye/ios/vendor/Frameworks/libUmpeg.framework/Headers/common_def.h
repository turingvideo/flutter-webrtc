#ifndef _COM_DEF_H
#define _COM_DEF_H

#ifndef VS2008B
    #include <stdint.h>
#else
    #ifndef uint8_t
        #define uint8_t         unsigned char
    #endif
    #ifndef int64_t
        #define int64_t         long long
    #endif
#endif

#ifndef HANDLE
    typedef void*               HANDLE;
#endif

#ifdef WIN32
    #define STDCALL             __stdcall
#else
    #define STDCALL
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

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


/*********************** 通用成功 ***********************************/
#define ERR_UMPEG_SUCCESS                   0x00000000      /* 执行成功 */

/*********************** 通用错误 ***********************************/
#define ERR_UMPEG_FAIL                      0x00000001      /* 执行失败 */
#define ERR_UMPEG_INVALID_PARAM             0x00000002      /* 输入参数非法 */
#define ERR_UMPEG_NO_MEMORY                 0x00000003      /* 系统内存不足 */
#define ERR_UMPEG_UNSUPPORT_FUNC            0x00000004      /* 功能不支持 */
#define ERR_UMPEG_UNSUPPORT_FMT             0x00000005      /* 不支持的格式 */
#define ERR_UMPEG_NEED_MORE_DATA            0x00000006      /* 需要更多输入数据 */
#define ERR_UMPEG_GET_MORE_DATA             0X00000007      /* 需要继续获取输出数据 */

#define ERR_UMPEG_NULL_PTR                  0x00000008      /* 空指针错误 */
#define ERR_UMPEG_DUP_INIT                  0x00000009      /* 重复初始化 */
#define ERR_UMPEG_ALLOC_FAIL                0x00000010      /* 资源分配失败 */

/*********************** 解码模块 ***********************************/
#define ERR_UMPEG_DECODE_FAIL               0x00000101      /* 解码失败 */
#define ERR_UMPEG_HWDATA_TO_MEM_FAIL        0x00000102      /* 硬件码数据拷贝内存帧失败 */

#define ERR_UMPEG_UNKNOWN_HWDEV             0x00000103      /* 未知硬件加速设备 */

#define ERR_UMPEG_FIND_DEC_FAIL             0x00000104      /* 查找解码器失败，检查库编译参数是否启用 */
#define ERR_UPMEG_CREATE_HWDEV_FAIL         0x00000105      /* 创建硬件加速设备失败 */
#define ERR_UMPEG_INPUTDATA_INVALID         0X00000106      /* 输入数据异常 */
#define ERR_UMPEG_DECODE_AGAIN              0x00000107      /* 资源暂时不可用,稍后重试 */
#define ERR_UMPEG_ZXDECODE_UNVALIABLE       0x00000108      /* 中星微的Svac解码库不可用(使用vnplayer解码) */
#define ERR_UMPEG_HWDEC_CONGESTED           0x00000109      /* 硬件解码器拥塞(缓冲区持续满),建议切软解 */

/*********************** 编码模块 ***********************************/
#define ERR_UMPEG_ENCODE_FAIL               0x00000201      /* 编码失败 */
#define ERR_UMPEG_FIND_ENC_FAIL             0x00000202      /* 查找编码器失败，检查库编译参数是否启用 */
#define ERR_UMPEG_UNKNOWN_SAMPLE_FMT        0x00000203      /* 不支持的采样格式 */

/*********************** 录制模块 ***********************************/
#define ERR_UMPEG_RECORD_REACH_LIMIT        0x00000301      /* 录制达到预设大小限制 */

/*********************** RTP模块 ***********************************/
#define ERR_UMPEG_RTP_UNKNOWN_STREAM_TYPE   0x00000401      /* 未知媒体流 */
#define ERR_UMPEG_RTP_PACKET_LOSE           0x00000402      /* 丢包 */
#define ERR_UMPEG_RTP_UNSUPPORT_PAYLOAD     0x00000403      /* 不支持的数据类型 */

/*********************** 解析模块 ***********************************/
#define ERR_UMPEG_DEMUX_FSEEK_FAIL          0x00000501      /* 位置查找失败 */

#define ERR_UMPEG_OPEN_INPUT_FAIL           0x00000502      /* 输入源打开失败 */
#define ERR_UMPEG_GET_INFO_FAIL             0x00000503      /* 查找属性信息失败 */
#define ERR_UMPEG_BSF_INIT_FAIL             0x00000504      /* Bit Stream Filter初始化失败 */
#define ERR_UMPEG_READ_EOF                  0x00000505      /* 文件读取结束 */

/*********************** 封装模块 ***********************************/
#define ERR_UMPEG_FIND_MUX_FAIL             0x00000601      /* 查找封装器失败，检查库编译参数是否启用 */


#define UMPEG_INVALID_VALUE            (-1)       /* 无效值 */
#define UMPEG_ONE_SECOND_MS            1000       /* 一秒钟含有的毫秒数 */
#define UMPEG_FRAME_TIME_DIFF          40         /* 相邻两帧之间的时间间隔 */
/**
* @enum tagUmpegLogLevelE
* @brief 运行日志等级枚举类型
* @attention 无
*/
typedef enum tagUmpegLogLevelE
{
    UMPEG_LOG_CLOSE        = 0,               /* 不打印 */
    UMPEG_LOG_FATAL        = 1,               /* fatal级别 */
    UMPEG_LOG_ERROR        = 2,               /* error级别 */
    UMPEG_LOG_WARN         = 3,               /* warn级别 */
    UMPEG_LOG_INFO         = 4,               /* info级别 */
    UMPEG_LOG_DEBUG        = 5                /* debug级别 */
}UMPEG_LOG_LEVEL_E;

/**
* @enum tagUmpegMediaTypeE
* @brief 媒体数据类型枚举
* @attention 音频|视频
*/
typedef enum tagUmpegMediaTypeE
{
    UMPEG_TYPE_VIDEO      = 0,        /* 视频数据 */
    UMPEG_TYPE_AUDIO      = 1,        /* 音频数据 */
    UMPEG_TYPE_MAX
}UMPEG_MEDIA_TYPE_E;

/**
* @enum tagUmpegEncodeFormatE
* @brief 数据编码格式枚举
* @attention 无
*/
typedef enum tagUmpegEncodeFormatE
{
    UMPEG_ENC_FMT_H264    = 0,        /* H.264 */
    UMPEG_ENC_FMT_H265    = 1,        /* H.265 */
    UMPEG_ENC_FMT_MJPEG   = 2,        /* MJPEG */
    UMPEG_ENC_FMT_BMP     = 3,        /* BMP   */
    UMPEG_ENC_FMT_G711A   = 4,        /* G711A */
    UMPEG_ENC_FMT_G711U   = 5,        /* G711U */
    UMPEG_ENC_FMT_AAC     = 6,        /* AAC   */
    UMPEG_ENC_FMT_MP2     = 7,        /* MP2   */
    UMPEG_ENC_FMT_MPEG2   = 8,        /* MPEG2 */
    UMPEG_ENC_FMT_MPEG4   = 9,        /* MPEG4 */
    UMPEG_ENC_FMT_SVAC    = 10,       /* SVAC Video */
    UMPEG_ENC_FMT_AUDIO_SVAC = 11,    /* SVAC Audio */
    UMPEG_ENC_FMT_G722 = 12,          /* G.722(ADPCM), 标准采样率8kHz，GB/T 28181-2022 */
    UMPEG_ENC_FMT_G723_1 = 13,        /* G.723.1, 默认采样率8kHz */
    UMPEG_ENC_FMT_G726 = 14,          /* G726, 默认采样率8kHz */
    UMPEG_ENC_FMT_G729 = 15,          /* G729, 默认采样率8kHz */
    UMPEG_ENC_FMT_MAX
}UMPEG_ENC_FMT_E;

/**
* @enum tagUmpegFileFormatE
* @brief 数据封装格式枚举
* @attention 无
*/
typedef enum tagUmpegFileFormatE
{
    UMPEG_FILE_FMT_JPEG   = 0,        /* jpeg */
    UMPEG_FILE_FMT_BMP    = 1,        /* bmp  */
    UMPEG_FILE_FMT_MP4    = 2,        /* mp4  */
    UMPEG_FILE_FMT_TS     = 3,        /* ts   */
    UMPEG_FILE_FMT_MAX
}UMPEG_FILE_FMT_E;

/**
* @brief Muxer类型
*/
typedef enum tagUmpegMuxerTypeE
{
    UMPEG_MUXER_TYPE_NONE = 0,
    UMPEG_MUXER_TYPE_FILE,            /* 直接写入到文件 */
    UMPEG_MUXER_TYPE_IOBUF            /* 写入到预分配的缓存区 */
}UMPEG_MUXER_TYPE_E;

/**
* @enum tagUmpegFrameTypeE
* @brief 帧类型枚举
* @attention 无
*/
typedef enum tagUmpegFrameTypeE
{
    UMPEG_FRAME_TYPE_I    = 0,        /* I帧 */
    UMPEG_FRAME_TYPE_P,               /* P帧 */
    UMPEG_FRAME_TYPE_SI,              /* SI帧 */
    UMPEG_FRAME_TYPE_SP,              /* SP帧 */
    UMPEG_FRAME_TYPE_MAX
}UMPEG_FRAME_TYPE_E;

/**
 * @enum tagUmpegRtpPayloadE
 * @brief 媒体负载枚举类型
 * @attention 无
 */
typedef enum tagUmpegRtpPayloadE
{
    UMPEG_RTP_PAYLOAD_NULL        = 0,        /* 无编码数据 */
    UMPEG_RTP_PAYLOAD_H264,                   /* H.264 */
    UMPEG_RTP_PAYLOAD_H265,                   /* H.265 */
    UMPEG_RTP_PAYLOAD_MP1L2,                  /* MPEG1 Layer2 */
    UMPEG_RTP_PAYLOAD_JPEG,                   /* JPEG（MJPEG） */
    UMPEG_RTP_PAYLOAD_G711A,                  /* G.711A */
    UMPEG_RTP_PAYLOAD_G711U,                  /* G.711U */
    UMPEG_RTP_PAYLOAD_PCM,                    /* PCM音频44.1k采样率 */
    UMPEG_RTP_PAYLOAD_AAC,                    /* AAC */
    UMPEG_RTP_PAYLOAD_METADATA,               /* 元数据 */
	UMPEG_RTP_PAYLOAD_MPEG4,                  /* MPEG4  */
    UMPEG_RTP_PAYLOAD_MPEG2,                  /* MPEG2  */
    UMPEG_RTP_PAYLOAD_MAX_NUM
}UMPEG_RTP_PAYLOAD_E;

/**
* @struct tagUmpegPixelFmtE
* @brief    像素格式类型
* @attention 无
*/
typedef enum tagUmpegPixelFmtE
{
    UMPEG_PIXFMT_YUV420P    = 0,            /* YUV420格式 */
    UMPEG_PIXFMT_YUYV422,                   /* YUV422格式 packed*/
    UMPEG_PIXFMT_YUV422P,                   /* YUV422格式 planar*/
    UMPEG_PIXFMT_YUV444P,                   /* YUV444格式 */
    UMPEG_PIXFMT_NV12,                      /* NV12格式 */
    UMPEG_PIXFMT_BGR24,                     /* BGR24格式 */
    UMPEG_PIXFMT_RGB24,                     /* RGB24格式 */
    UMPEG_PIXFMT_RGBA,                      /* RGBA格式 */
    UMPEG_PIXFMT_ARGB,                      /* ARGB格式 */
    UMPEG_PIXFMT_RGB565,                    /* RGB565格式 */
    UMPEG_PIXFMT_BGRA,                      /* BGRA格式 */
    UMPEG_PIXFMT_DXVA2_VLD,                 /* data[3]指向一个LPDIRECT3DSURFACE9指针*/
    UMPEG_PIXFMT_D3D11,                     /* data[0]指向一个ID3D11Texture2D指针，data[1]为纹理索引 */
    UMPEG_PIXFMT_DRM_PRIME,                 /* DRM硬件像素格式,RK解码后为此格式 */
    UMPEG_PIXFMT_MAX
}UMPEG_PIXEL_FMT_E;

/**
* @struct tagUmpegSampleFmtE
* @brief    音频采样格式类型
* @attention 无
*/
typedef enum tagUmpegSampleFmtE
{
    UMPEG_SAMPLE_FMT_U8     = 0,            /* unsigned 8 bits */
    UMPEG_SAMPLE_FMT_S16,                   /* signed 16 bits */
    UMPEG_SAMPLE_FMT_S32,                   /* signed 32 bits */
    UMPEG_SAMPLE_FMT_FLT,                   /* float */
    UMPEG_SAMPLE_FMT_DBL,                   /* double */

    UMPEG_SAMPLE_FMT_U8P,                   /* unsigned 8 bits, 平面格式 */
    UMPEG_SAMPLE_FMT_S16P,                  /* signed 16 bits, 平面格式 */
    UMPEG_SAMPLE_FMT_S32P,                  /* signed 32 bits, 平面格式 */
    UMPEG_SAMPLE_FMT_FLTP,                  /* float, 平面格式 */
    UMPEG_SAMPLE_FMT_DBLP,                  /* double, 平面格式 */
    UMPEG_SAMPLE_FMT_S64,                   /* signed 64 bits */
    UMPEG_SAMPLE_FMT_S64P,                  /* signed 64 bits, 平面格式 */
    
    UMPEG_SAMPLE_FMT_MAX
}UMPEG_SAMPLE_FMT_E;

/**
* @enum tagUmpegHwDevTypeE
* @brief 硬件加速类型枚举
* @attention 无
*/
typedef enum tagUmpegHwDevTypeE
{
    UMPEG_HWDEV_TYPE_NONE     = 0,        /* 非硬件加速 */
    UMPEG_HWDEV_TYPE_DXVA2,               /* DXVA2硬件加速 */
    UMPEG_HWDEV_TYPE_D3D11VA,             /* D3D11VA硬件加速 */
    UMPEG_HWDEV_TYPE_ROCKCHIP,            /* RK硬件加速 */
    UMPEG_HWDEV_TYPE_OMX,                 /* HW OMX硬件加速 */
    UMPEG_HWDEV_TYPE_MAX
}UMPEG_HWDEV_TYPE_E;

/**
* @struct tagUmpegHwDevCapS
* @brief 硬件加速能力集
* @attention 无
*/
typedef struct tagUmpegHwDevCapS
{
    int nH264_DXVA2;                 /* 支持h264_dxva2硬件加速 */
    int nH264_D3D11VA;               /* 支持h264_d3d11va硬件加速 */
    int nH265_DXVA2;                 /* 支持h265_dxva2硬件加速 */
    int nH265_D3D11VA;               /* 支持h265_d3d11va硬件加速 */
    int nH264_ROCKCHIP;              /* 支持h264_Rockchip硬件加速 */
    int nH265_ROCKCHIP;              /* 支持h265_Rockchip硬件加速 */
}UMPEG_HWDEV_CAP_S;

/**
* @struct tagUMDecoderInitParamS
* @brief 解码器初始化参数结构体
* @attention 无
*/
typedef struct tagUMDecoderInitParamS
{
    UMPEG_ENC_FMT_E eEncFmt;            /* 数据编码格式 */
    UMPEG_HWDEV_TYPE_E eHwDevType;      /* 硬件加速类型 */
    int nWidth;                         /* 视频图像宽度 */
    int nHeight;                        /* 视频图像高度 */
    int nThreadCount;                   /* 解码线程数 */
    char szDevice[16];                  /* 硬件设备标识符（传""使用默认设备） */
    int nSampleRate;                    /* 采样率 */
    int nChannels;                      /* 通道数 */
}UMDECODER_INIT_PARAM_S;

/**
* @struct tagUMDecFrameS
* @brief 待解码帧数据结构体
* @attention 无
*/
typedef struct tagUMDecFrameS
{
    uint8_t *pcData;                         /* 帧数据 */
    int nDataLen;                            /* 帧数据长度 */
    long long llTimestamp;                   /* 时间戳 */
    unsigned int nFrameType;                 /* 帧类型 0-I帧 1-P帧 */
}UMDECODER_FRAME_S;

/**
* @struct tagUMDecRawS
* @brief 解码一帧数据信息
* @attention 无
*/
typedef struct tagUMDecRawS
{
    void *pFrame;                       /* 帧信息（释放硬解数据时解码器内部需根据该信息释放资源） */
    uint8_t *pcData[4];                 /* PIC数据|PCM数据 */
    int nLineSize[4];                   /* 跨距数组（视频时表示一行数据的长度，音频时表示数据总长度） */
    UMPEG_PIXEL_FMT_E ePixFmt;          /* 视频像素格式 */
    int nWidth;                         /* 视频图像宽度 */
    int nHeight;                        /* 视频图像高度 */
    int nChannel;                       /* 音频通道数 */
    int nSampleRate;                    /* 音频采样率 */
    int nSamples;                       /* 音频采样点数 */
    UMPEG_SAMPLE_FMT_E eSampleFmt;      /* 音频采样格式 */
    long long llTimestamp;              /* 时间戳 */
}UMDECODER_RAW_S;

/**
* @enum tagUmpegEncoderModeE
* @brief 编码模式
* @attention 无
*/
typedef enum tagUmpegEncoderModeE
{
    UM_ENC_MODE_NULL         = 0,       /**< 默认无 */
    UM_ENC_MODE_CBR,                    /**< CBR */
    UM_ENC_MODE_CVBR,                   /**< CVBR */
    UM_ENC_MODE_VBR                     /**< VBR */
}UMPEG_ENCODER_MODE_E;

/**
* @struct tagUMEncoderInitParamS
* @brief 编码器初始化参数结构体
* @attention 无
*/
typedef struct tagUMEncoderInitParamS
{
    UMPEG_ENC_FMT_E eEncFmt;                 /* 目标编码格式 */
    UMPEG_HWDEV_TYPE_E eHwDevType;           /* 硬件加速类型 */
    int nThreadCount;                        /* 编码线程数 */
    int nWidth;                              /* 视频|图片宽度 */
    int nHeight;                             /* 视频|图片高度 */
    int nFrameRate;                          /* 视频帧率 */
    unsigned int nGopSize;                   /* 视频编码组大小 */
    unsigned int nQpValue;                   /* 视频编码QP值 */
    UMPEG_ENCODER_MODE_E nEncMode;           /* 编码模式 */
    int nSampleRate;                         /* 音频采样率 */
    int nChannels;                           /* 音频声道数 */
    int nBitRate;                            /* 码率大小 */
}UMENCODER_INIT_PARAM_S;

/**
* @struct tagUMEncRawS
* @brief 待编码数据结构体
* @attention 无
*/
typedef struct tagUMEncRawS
{
    uint8_t *pcData[4];                 /* PIC数据|PCM数据 */
    int nLineSize[4];                   /* 跨距数组（视频时表示一行数据的长度，音频时表示数据总长度）*/
    long long llTimestamp;              /* 时间戳 */
}UMENCODER_RAW_S;

/**
* @struct tagUMEncFrameS
* @brief 编码后帧数据信息结构体
* @attention 无
*/
typedef struct tagUMEncFrameS
{
    uint8_t *pcData;                         /* 帧数据 */
    int nDataLen;                            /* 帧数据长度 */
    UMPEG_FRAME_TYPE_E eFrameType;           /* 帧类型 */
    long long llTimestamp;                   /* 时间戳 */
}UMENCODER_FRAME_S;

/**
* @struct tagUMRecorderInitParamS
* @brief 录制器初始化结构体
* @attention 无
*/
typedef struct tagUMRecorderInitParamS
{
    int nMaxFileSize;                   /* 文件大小上限（单位：KB） */
    UMPEG_FILE_FMT_E eFileFmt;          /* 文件格式 */
    UMPEG_ENC_FMT_E eVideoEncFmt;       /* 视频编码格式 */
    UMPEG_ENC_FMT_E eAudioEncFmt;       /* 音频编码格式 */
    UMPEG_MUXER_TYPE_E eMuxerType;      /* 封装器类型 */
    const char *pcFilename;             /* 文件名 */
    int nWidth;                         /* 视频宽度 */
    int nHeight;                        /* 视频高度 */
    int nFrameRate;                     /* 视频帧率 */
    int nSampleRate;                    /* 音频采样率 */
    int nChannels;                      /* 音频声道数 */
}UMRECORDER_INIT_PARAM_S;

/**
* @struct tagUMRecorderFrameS
* @brief 录制器输入数据结构体
* @attention 无
*/
typedef struct tagUMRecorderFrameS
{
    uint8_t *pcData;                         /* 帧数据 */
    int nDataLen;                            /* 帧数据长度 */
    UMPEG_MEDIA_TYPE_E eMediaType;           /* 数据类型（视频|音频） */
    UMPEG_FRAME_TYPE_E eFrameType;           /* 帧类型（视频需要） */
    int64_t nTimestamp;                      /* 时间戳 */
}UMRECORDER_FRAME_S;

/**
* @struct tagUMRecorderFrameS
* @brief 录制器录制后数据结构体
* @attention 无
*/
typedef struct tagUMRecoderDataS
{
    uint8_t *pcData;                         /* 输出缓存区指针 */
    int nDataLen;                            /* 输出缓存区长度 */
}UMRECORDER_DATA_S;


/**
* @struct tagPictureInitInfoS
* @brief 截图模块初始化参数
* @attention 无
*/
typedef struct tagUMCapturerInitParamS
{
    UMPEG_FILE_FMT_E eFileFmt;        /* 数据封装格式 */
    int nWidth;                       /* 图像宽度 */
    int nHeight;                      /* 图像高度 */
}UMCAPTURER_INIT_PARAM_S;

/**
* @struct tagUMCapturerRawS
* @brief 截图模块输入参数结构体
* @attention 需要传入YUVJ420P格式的数据
*/
typedef struct tagUMCapturerRawS
{
    uint8_t *pcData[4];                 /* PIC数据|PCM数据 */
    int nLineSize[4];                   /* 跨距数组（视频时表示一行数据的长度，音频时表示数据总长度）*/
}UMCAPTURER_RAW_S;

/**
* @struct tagUMCapturerDataS
* @brief  截图数据结构体
* @attention 无
*/
typedef struct tagUMCapturerDataS
{
    uint8_t *pcData;                         /* 截图数据指针 */
    int nDataLen;                            /* 截图数据长度 */
}UMCAPTURER_DATA_S;

/**
* @struct tagUMPackagerInitParamS
* @brief  打包器初始化参数结构体
* @attention 无
*/
typedef struct tagUMPackagerInitParamS
{
    UMPEG_RTP_PAYLOAD_E ePayload;            /* 媒体类型 */
    char* pcSrcData;                         /* 待打包数据 */
    unsigned int nDataLen;                   /* 待打包数据长度 */
    unsigned int nSequenceNum;               /* 序列号 */
    long long    llTimestamp;                /* 时间戳 */
}UMPACKAGER_INIT_PARAM_S;

/**
* @struct tagUMPackagerDataS
* @brief  打包后数据结构
* @attention 无
*/
typedef struct tagUMPackagerDataS
{
    char *pcData;                            /* 打包数据 */
    unsigned int nDataLen;                   /* 打包数据长度 */
}UMPACKAGER_DATA_S;

/**
* @struct tagUMParserCtrlTypeE
* @brief  控制类型枚举
* @attention 无
*/
typedef enum tagUMParserCtrlTypeE
{
    UMPARSER_CTRL_TIME_FORWARD= 0,           /* 按时间戳，帧进查找 */
    UMPARSER_CTRL_TIME_BACKWARD,             /* 按时间戳，帧退查找 */
    UMPARSER_CTRL_BYTE_BACKWARD,             /* 按字节，帧退查找 */
    UMPARSER_CTRL_BYTE_FORWARD,              /* 按字节，帧进查找 */
    UMPARSER_CTRL_MAX
}UMPARSER_CTRL_TYPE_E;

/**
* @struct tagUMParserVideoFrameS
* @brief  待解析帧数据
* @attention 无
*/
typedef struct tagUMParserVideoFrameS
{
    UMPEG_ENC_FMT_E eEncFmt;                 /* 帧编码格式 */
    uint8_t *pcData;                         /* 帧数据 */
    int nDataLen;                            /* 帧数据长度 */
}UMPARSER_VIDEO_IFRAME_S;

/**
* @struct tagUMParserVideoInfoS
* @brief 解析后帧数据信息结构体
* @attention 无
*/
typedef struct tagUMParserVideoInfoS
{
    int nWidth;                         /* 视频参数：宽度 */
    int nHeight;                        /* 视频参数：高度 */
    int nFrameRate;                     /* 视频参数：帧率 */
}UMPARSER_VIDEO_INFO_S;

/**
* @struct tagUMParserInfoS
* @brief  解析信息结构体
* @attention 无
*/
typedef struct tagUMParserInfoS
{
    unsigned int unFileTime;                 /* 文件时长 */
    unsigned int unFileLength;               /* 文件长度 */
    UMPARSER_CTRL_TYPE_E eSeekType;          /* 文件控制类型 */
    const char* pcDemuxerName;               /* 解封装器名称 */
    int nTimeBaseNum;                        /* 视频时间基分子 */
    int nTimeBaseDen;                        /* 视频时间基分母 */
    int nAudioTimeBaseNum;                   /* 音频时间基分子 */
    int nAudioTimeBaseDen;                   /* 音频时间基分母 */
    unsigned int unBeginTimestamp;           /* 视频起始时间戳 */
    UMPEG_ENC_FMT_E eEncVideoFmt;            /* 视频编码格式 */
    UMPEG_ENC_FMT_E eEncAudioFmt;            /* 音频编码格式 */
    int nWidth;                              /* 图像宽度 */
    int nHeight;                             /* 图像高度 */
    int nFrameRate;                          /* 视频帧率 */
    int nSampleRate;                         /* 音频采样率 */
    int nChannels;                           /* 音频声道数 */
    int nBitRate;                            /* 码率大小 */
    int nFrameNum;                           /* 帧个数 */
}UMPARSER_INFO_S;

/**
* @struct tagUMParserInfoS
* @brief  解析后数据结构体
* @attention 无
*/
typedef struct tagUMParserFrameS
{
    uint8_t *pcData;                         /* 帧数据 */
    int nDataLen;                            /* 帧数据长度 */
    UMPEG_MEDIA_TYPE_E eMediaType;           /* 媒体数据类型（视频|音频） */
    UMPEG_FRAME_TYPE_E eFrameType;           /* 帧类型（视频需要） */
    long long llTimestamp;                   /* 时间戳 */
}UMPARSER_FRAME_S;

/**
* @struct tagUMParserCtrlS
* @brief  对解析后文件进行控制操作
* @attention 无
*/
typedef struct tagUMParserCtrlS
{
    UMPARSER_CTRL_TYPE_E eSeekType;              /* 控制类型 */
    long long llPos;                             /* 控制参数 */
}UMPARSER_CTRL_S;

/**
* @struct tagUMScalerAlgorithmE
* @brief    缩放算法
* @attention 无
*/
typedef enum tagUMScalerAlgorithmE
{
    UMSCALER_BICUBIC = 0,             /* 双三次插值算法 */
    UMSCALER_BILINEAR,                /* 双线性插值算法 */
    UMSCALER_POINT,                   /* 点采样算法 */
    UMSCALER_NEAREST,                 /* 最近邻插值算法 */
    UMSCALER_MAX
}UMSCALER_ALGORITHM_E;

/**
* @struct tagUMScalerInitParamS
* @brief    缩放器初始化信息
* @attention 无
*/
typedef struct tagUMScalerInitParamS
{
    UMPEG_PIXEL_FMT_E eSrcPixFmt;       /* 源像素格式 */
    UMPEG_PIXEL_FMT_E eDstPixFmt;       /* 目标像素格式 */
    UMSCALER_ALGORITHM_E eSCAlgorithm;  /* 缩放算法 */
    int nSrcWidth;                      /* 源图片宽度 */
    int nSrcHeight;                     /* 源图片高度 */
    int nDstWidth;                      /* 目标图片宽度 */
    int nDstHeight;                     /* 目标图片高度 */
}UMSCALER_INIT_PARAM_S;

/**
* @struct tagUMSclSrcRawS
* @brief    源图片数据
* @attention 无
*/
typedef struct tagUMSclSrcRawS
{
    uint8_t *pcData[4];                 /* PIC数据 */
    int nLineSize[4];                   /* 跨距数组 */
}UMSCALER_SRCRAW_S;

/**
* @struct tagUMSclDestRawS
* @brief    目标图片数据
* @attention 无
*/
typedef struct tagUMSclDestRawS
{
    uint8_t *pcData[4];                 /* PIC数据 */
    int nLineSize[4];                   /* 跨距数组 */
}UMSCALER_DESTRAW_S;

/**
* @struct tagUmpegChannelLayoutE
* @brief    音频声道布局
* @attention 无
*/
typedef enum tagUmpegChannelLayoutE
{
    UMPEG_CHANNEL_LAYOUT_MONO = 0,      /* 单声道 */
    UMPEG_CHANNEL_LAYOUT_STEREO,        /* 立体声（双声道）*/
    UMPEG_CHANNEL_LAYOUT_MAX
}UMPEG_CHANNEL_LAYOUT_E;

/**
* @struct tagUMResampleInitParamS
* @brief    重采样器初始化参数结构体
* @attention 无
*/
typedef struct tagUMResamplerInitParamS
{
    UMPEG_CHANNEL_LAYOUT_E eSrcChannelLayout;    /* 源声道格式 */
    UMPEG_SAMPLE_FMT_E     eSrcSampleFmt;        /* 源采样格式 */
    int nSrcSampleRate;                          /* 源采样率 */
    UMPEG_CHANNEL_LAYOUT_E eDstChannelLayout;    /* 目标声道格式 */
    UMPEG_SAMPLE_FMT_E     eDstSampleFmt;        /* 目标采样格式 */
    int nDstSampleRate;                          /* 目标采样率 */
}UMRESAMPLER_INIT_PARAM_S;

/**
* @struct tagUMResamplerSrcRawS
* @brief    重采样前音频数据结构体
* @attention 无
*/
typedef struct tagUMResamplerSrcRawS
{
    uint8_t *pcData[4];                 /* 音频原数据 */
    int nSampleNum;                     /* 每个声道的采样点数 */
}UMRESAMPLER_SRC_RAW_S;

/**
* @struct tagUMResamplerDstRawS
* @brief    重采样后音频数据结构体
* @attention 无
*/
typedef struct tagUMResamplerDstRawS
{
    uint8_t *pcData[4];                 /* 音频重采样后数据 */
    int nSampleNum;                     /* 每个声道的采样点数 */
    int nLineSize;                      /* 每个plane的LineSize */
}UMRESAMPLER_DST_RAW_S;

/**
* @struct tagTranscoderInitParamS
* @brief 转码模块输入参数结构体
* @attention 无
*/
typedef struct tagTranscoderInitParamS
{
    UMPEG_ENC_FMT_E eSrcEncFmt;                  /* 源编码格式 */
    UMPEG_CHANNEL_LAYOUT_E eSrcChannelLayout;    /* 源声道格式 */
    UMPEG_SAMPLE_FMT_E     eSrcSampleFmt;        /* 源采样格式 */
    int nSrcSampleRate;                          /* 源采样率 */
    int nSampleNum;                              /* 源数据每个声道的采样点数 */
    UMPEG_ENC_FMT_E eDstEncFmt;                  /* 目标编码格式 */
    UMPEG_CHANNEL_LAYOUT_E eDstChannelLayout;    /* 目标声道格式 */
    UMPEG_SAMPLE_FMT_E     eDstSampleFmt;        /* 目标采样格式 */
    int nDstSampleRate;                          /* 目标采样率 */
    UMPEG_HWDEV_TYPE_E eHwDevType;               /* 硬件加速类型 */
    int nDecThreadCount;                         /* 解码线程数 */
    int nEncThreadCount;                         /* 编码线程数 */
    int nBitRate;                                /* 码率大小 */
}TRANSCODER_INIT_PARAM_S;


/**
* @brief  记录日志的回调函数类型定义 
* @param  eLevel            日志级别, 对应# UMPEG_LOG_LEVEL_E 枚举中的值
* @param  pcFileName        代码文件名
* @param  nFileLine         代码所在行数
* @param  pcFunc            所在函数名
* @param  pcFormat          格式字符串
* @param  vl                变参列表
* @return void
* @note
*/
typedef void (STDCALL* UMPEG_WRITE_LOG_CALLBACK_PF)(UMPEG_LOG_LEVEL_E eLevel,
                                                     const char *pcFileName,
                                                     int nFileLine,
                                                     const char *pcFunc,
                                                     const char *pcFormat,
                                                     va_list vl);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _COM_DEF_H */