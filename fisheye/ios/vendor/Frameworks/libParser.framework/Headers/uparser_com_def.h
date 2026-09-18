#ifndef _UPARSER_COM_DEF_H
#define _UPARSER_COM_DEF_H

#include "umedia_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_UPARSER_SUCCESS                     0   /* 成功 */
#define ERR_UPARSER_FAIL                        1   /* 执行失败 */
#define ERR_UPARSER_INVALID_PARAM               2   /* 参数非法 */
#define ERR_UPARSER_NEED_MOREPACKET             3   /* 需要更多包 */
#define ERR_UPARSER_LOST_PACKET                 4   /* 发生丢包 */
#define ERR_UPARSER_UNKNOWN_STREAM              5   /* 未知的流类型 */
#define ERR_UPARSER_ALLOC_FAIL                  6   /* 内存分配失败 */
#define ERR_UPARSER_PARSER_NOTFOUND             7   /* Parser未找到 */
#define ERR_UPARSER_PARSER_NOTSUPPORT           8   /* Parser不支持 */
#define ERR_UPARSER_UNKNOWN_PAYLOAD             9   /* 未知payload */

#define ERR_UPARSER_PACKET_LEN_WRONG            10  /* 包长度错误 */
#define ERR_UPARSER_GET_MORE_DATA               11  /* 需要继续获取输出数据 */

#define ERR_TS_PACKET_IN_THE_ROUGH              0x0000010B      /**< TS打包未完成 */
#define ERR_TS_SRCBUF                           0x0000010C      /**< TS包缓冲区溢出 */
#define ERR_TS_NO_SYN                           0x0000010D      /**< 没有同步字节0x47 */
#define ERR_TS_BUFOVER                          0x0000010E      /**< TS包缓冲区溢出 */
#define ERR_TS_CONTINUE                         0x0000010F      /**< TS拼帧继续下一个拼帧 */

#define ERR_TS_GET_PCR_FAIL                     0x00000162      /**< TS解析pcr失败 */
#define ERR_TS_PID_PARSE_FAIL                   0x00000163      /**< TS解析pid失败 */
#define ERR_TS_FIND_HEADER_FAIL                 0x00000164      /**< TS查找包头标准 */
#define ERR_FIRSTREND_FINISH                    0x00000165      /**< 首帧渲染完成*/ 

#define ERR_RTP_SYNC_BYTE_WRONG                 0xF000000E      /**< RTP解析：查找同步字段错误 */
#define ERR_RTP_DATA_LEN_INVALID                0xF000000F      /**< RTP解析：数据长度不合法 */
#define ERR_RTP_DATA_POINTER_INVALID            0xF0000010      /**< RTP解析：数据长度不合法 */
#define ERR_RTP_FIND_IFRAME_BY_NAL              0xF0000011      /**< RTP解析：通过NAL找到I帧 */
#define ERR_RTP_FIND_DATA_BY_NAL_FAIL           0xF0000012      /**< RTP解析：通过NAL查找数据(如sps、pps)失败 */

#define ERR_RTP_HEAD_WRONG                      0xF0000013      /**< RTP头解析错误 */
#define ERR_RTP_H264_BUFOVER                    0xF0000014      /**< RTP + H264缓冲区溢出 */
#define ERR_RTP_H264_PACK                       0xF0000015      /**< 解析H264包出错 */
#define ERR_RTP_H264_PACKET_LEN                 0xF0000016      /**< RTP + H264包长度出错 */

#define ERR_FLV_POINTER_INVALID                 0xF0000007      /**< FLV解析：输入内存非法 */
#define ERR_FLV_DATALEN_INVALID                 0xF0000008      /**< FLV解析：输入数据长度非法 */

#define UPARSER_OPT_VALUE void

/*@brief 最大媒体类型数 */
#define PLAYER_PAYLOAD_TYPE_NUM                 2
/*@brief 最大媒体类型数 */
#define UMD_PAYLOAD_TYPE_NUM                 2

/**
 * @enum        tagUParserUsageE
 * @brief       Parser用途定义
 * @attention   无
*/
typedef enum tagUParserUsageE
{
    UPARSER_USAGE_RTP_TS = 0,
    UPARSER_USAGE_RTP_ES,
    UPARSER_USAGE_RTP_PS,
    UPARSER_USAGE_TS,
    UPARSER_USAGE_PS,
    UPARSER_USAGE_FLV,
    UPARSER_USAGE_UNKNOWN
}UPARSER_USAGE_E; 


/**
 * @enum        tagUParserCodecTypeE
 * @brief       编码格式类型枚举
 * @attention   无
*/
typedef enum tagUParserCodecTypeE
{
    UPARSER_CODEC_TYPE_G711U,
    UPARSER_CODEC_TYPE_G711A,
    UPARSER_CODEC_TYPE_H264,
    UPARSER_CODEC_TYPE_H265,
    UPARSER_CODEC_TYPE_JPEG,
    UPARSER_CODEC_TYPE_AAC_LC,
    UPARSER_CODEC_TYPE_UNKNOWN
}UPARSER_CODEC_TYPE_E;

/**
* @enum tagUParserVideoEncodeTypeE
* @brief 视频编码格式枚举定义
* @attention 无
*/
typedef enum tagUParserVideoEncodeTypeE
{
    UPARSER_VIDEO_ENCODE_MPEG2   = 0,      /**< 视频符合MPEG-2标准 */
    UPARSER_VIDEO_ENCODE_MPEG4   = 1,      /**< 视频符合MPEG-4标准 */
    UPARSER_VIDEO_ENCODE_H264    = 2,      /**< 视频符合H.264标准 */
    UPARSER_VIDEO_ENCODE_MJPEG   = 3,      /**< 视频符合MJPEG标准 */
    UPARSER_VIDEO_ENCODE_H265    = 4,      /**< 视频符合H.265标准 */
    UPARSER_VIDEO_ENCODE_SVAC    = 5,      /**< 视频符合SVAC标准 */
    UPARSER_VIDEO_ENCODE_NULL              /**< 无视频 */
}UPARSER_VIDEO_ENCODE_TYPE_E;

/**
* @enum tagUParserAudioEncodeTypeE
* @brief 音频编码格式枚举定义
* @attention 无
*/
typedef enum tagUParserAudioEncodeTypeE
{
    UPARSER_AUDIO_ENCODE_G711U   = 0,      /**< 音频符合G.711U律标准 */
    UPARSER_AUDIO_ENCODE_MP2     = 1,      /**< 音频符合MP2编码标准 */
    UPARSER_AUDIO_ENCODE_AAC     = 2,      /**< 音频符合AAC编码标准 */
    UPARSER_AUDIO_ENCODE_G711A   = 3,      /**< 音频符合G.711A律标准 */
    UPARSER_AUDIO_ENCODE_PCM     = 4,      /**< 音频符合PCM律标准 */
    UPARSER_AUDIO_ENCODE_SVAC    = 5,      /**< 音频符合SVAC律标准 */
    UPARSER_AUDIO_ENCODE_G722    = 6,      /**< 音频符合G.722标准 */
    UPARSER_AUDIO_ENCODE_G723_1  = 7,      /**< 音频符合G.723.1标准 */
    UPARSER_AUDIO_ENCODE_G729    = 8,      /**< 音频符合G.729标准 */
    UPARSER_AUDIO_ENCODE_NULL              /**< 无音频 */
}UPARSER_AUDIO_ENCODE_TYPE_E;

/**
 * @enum        tagUParserMediaTypeE
 * @brief       媒体数据类型枚举
 * @attention   无
*/
typedef enum tagUParserMediaTypeE
{
    UPARSER_MEDIA_TYPE_AUDIO,
    UPARSER_MEDIA_TYPE_VIDEO,
    UPARSER_MEDIA_TYPE_METADATA,
    UPARSER_MEDIA_TYPE_UNKNOW
}UPARSER_MEDIA_TYPE_E;


/**
 * @brief       UParser可选设置参数
 * @enum        tagUParserOptKeyE
 * @attention   无
*/
typedef enum tagUParserOptKeyE
{
    UPARSER_OPT_KEY_RTP_AUDIO_INFO,     /* 为RTP_Parser设置AudioInfo, 参数类型为: UPARSER_AUDIO_INFO_S */
    UPARSER_OPT_KEY_RTP_PAYLOAD_TYPE,   /* 为RTP_Parser设置PayloadType映射列表，参数类型为: UPARSER_AUDIO_INFO_S */
    UPARSER_OPT_KEY_GB_USAGE,           /* 指定UParser用于解析国标流，eUsage只会在RTP_PS和PS中进行猜测 */
    UPARSER_OPT_KEY_UNKNOW
}UPARSER_OPT_KEY_E;

/**
 * @brief       设置拼帧器发生丢包时的拼帧策略
 * @enum        tagUParserSplitterPolicyE
 * @attention   仅对解析链包含PS的UParser生效
 */
typedef enum tagUParserSplitterPolicyE
{
    UPARSER_SPLITTER_POLICY_DROP        = 0,    /* 丢弃当前GOP剩余帧 */
    UPARSER_SPLITTER_POLICY_CONTINUE            /* 继续拼帧 */
}UPARSER_SPLITTER_POLICY_E;

/**
 * @brief       拼帧过程事件枚举
 * @enum        tagUParserEventE
 * @attention   无
 */
typedef enum tagUParserEventE
{
    UPARSER_EVENT_RTP_PLAYBACK_FINISH   = 0,    /* RTP回放结束事件 */
    UPARSER_EVENT_MAX
}UPARSER_EVENT_E;

/**
 * @struct tagPlayerPayloadTypeS
 * @brief 码流信息结构体
 * @attention 无
 */
typedef struct tagUParserPayloadTypeS
{
    UCHAR ucPayloadType;                                /**< payload值*/
    UCHAR ucReserved[3];                                /**< 填充位，无意义*/
    ULONG_32 ulType;                                    /**< 0视频，1音频*/
    union
    {
        UPARSER_VIDEO_ENCODE_TYPE_E enVideoPayloadType;  /**< 视频枚举变量*/
        UPARSER_AUDIO_ENCODE_TYPE_E enAudioPayloadType;  /**< 音频枚举变量*/
    };
    ULONG_32 ulSampleRate;                              /**< 采样率*/
}UPARSER_PAYLOAD_TYPE_S;

typedef struct tagUParserPayloadTypeListS
{
    ULONG_32 ulNum;                                     /**< 要输入的PayloadType数量 */
    UPARSER_PAYLOAD_TYPE_S *pstPayloadTypeList;    /**< 要输入的payloadType数组 */
}UPARSER_PAYLOAD_TYPE_LIST_S;

/**
 * @struct tagUParserAudioInfoS
 * @brief 音频信息结构体
 * @attention 无
 */
typedef struct tagUParserAudioInfoS
{
    UCHAR ucAudioType;                  /**< 音频协议类型*/
    UCHAR ucFrequencyIndex;             /**< 音频采样率索引*/
    UCHAR ucChannelNum;                 /**< 音频声道数*/
    UCHAR ucReserved;                   /**< 填充位，无意义*/
}UPARSER_AUDIO_INFO_S;

/**
 * @enum        tagUParserInitParamS
 * @brief       UParser初始化参数
 * @attention   无
*/
typedef struct tagUParserInitParamS
{
    UPARSER_USAGE_E eUsage;    /* 如果传UPARSER_USAGE_NONE,则内部根据输入的数据去尝试判断 */
    UPARSER_SPLITTER_POLICY_E eSplitterPolicy;  /* 发生丢包时后续的丢包策略 */
}UPARSER_INIT_PARAM_S;

/**
 * @struct      tagUParserInDataS
 * @brief       待解析的packet
 * @attention   无
*/
typedef struct tagUParserPacketS
{
    UCHAR *pucData;
    ULONG_32 ulSize;
    DULONG dulTimestamp;    /* 包时间，可缺省 */
}UPARSER_PACKET_S;

/**
 * @struct      tagUParserOutDataS
 * @brief       Parser拼帧完毕的数据帧
 * @attention   无
*/
typedef struct tagUParserFrameS
{
    UCHAR *pucData;
    ULONG_32 ulSize;
    DULONG dulTimestamp;
    DULONG dulExtTimestamp;                     /* RTP扩展字段中解析出的时间戳 */
    UMD_PAYLOAD_E ePayload;
    UMD_VIDEO_FRAME_TYPE_E eVideoFrameType;
    UMD_WAVE_FORMAT_E eWaveFormat;
    ULONG_32 ulWidth, ulHeight, ulFrameRate;
    ULONG_32 ulSampleRate, ulChannelNum;
    BOOL_T bDropPacket;                         /* 用来返回国标码流是否丢包 */
}UPARSER_FRAME_S;


/**
 * @struct      tagUParserEventS
 * @brief       UParser事件结构体
 * @attention   无
 */
typedef struct tagUParserEventS
{
    UPARSER_EVENT_E eEvent;     /* 事件码 */
    BOOL_T bHaveExtraMsg;       /* 是否包含额外消息 */
    const CHAR *pcExtraMsg;     /* 额外消息 */
}UPARSER_EVENT_S;

/**
 * @brief       事件回调函数指针类型
 * @param       [IN] pstEvent   事件
 * @param       [IN] dlReserved 保留参数
 * @param       [IN] pUserParam 用户参数
 */
typedef VOID(STDCALL *UPARSER_EVENT_CB_PF)(IN UPARSER_EVENT_S *pstEvent,
                                           IN LONG_REAL dlReserved,
                                           IN VOID *pUserParam);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _UPARSER_COM_DEF_H */