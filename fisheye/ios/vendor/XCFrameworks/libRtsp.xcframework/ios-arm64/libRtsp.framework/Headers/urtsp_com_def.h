#ifndef __URTSP_COM_DEF_H__
#define __URTSP_COM_DEF_H__

#include "umedia_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------错误码定义---------------------------------------------*/
/* 通用错误码 */
#define ERR_URTSP_SUCCESS                           0               /* 执行成功 */
#define ERR_URTSP_FAIL                              1               /* 执行失败 */
#define ERR_URTSP_INVALID_PARAM                     2               /* 输入参数非法 */
#define ERR_URTSP_NULL_PTR                          3               /* 输入指针为空 */
#define ERR_URTSP_NO_MEMORY                         4               /* 内存分配失败 */
#define ERR_URTSP_NOT_SUPPORT                       5               /* 功能暂不支持 */
#define ERR_URTSP_SYS_FAIL                          6               /* 系统方法失败 */
#define ERR_URTSP_SOCK_METHOD_FAIL                  7               /* socket方法失败 */
#define ERR_URTSP_INIT_DONE                         8               /* 已经初始化 */
#define ERR_URTSP_NO_SPARE_SESSION                  9               /* 无空闲会话 */ 
#define ERR_URTSP_INVALID_SESSION                   10              /* 输入会话非法 */
#define ERR_URTSP_SESSION_CLOSE                     11              /* 会话已关闭 */
#define ERR_URTSP_STREAM_IN_PROCESS                 12              /* 流已经处理中 */
#define ERR_URTSP_URL_NOT_STANDARD                  13              /* URL格式不标准 */
#define ERR_URTSP_URL_NO_USER                       14              /* URL未解析到用户名 */
#define ERR_URTSP_URL_NO_PASSWORD                   15              /* URL未携带到密码 */
#define ERR_URTSP_MSG_PARSE_FAIL                    16              /* RTSP消息解析失败 */
#define ERR_URTSP_UPNP_FAIL                         17              /* UPNP失败 */
#define ERR_URTSP_NEED_AUTH                         18              /* 需要鉴权  */
#define ERR_URTSP_GET_AUTH_FAIL                     19              /* 获取鉴权失败 */
#define ERR_URTSP_AUTH_FAIL                         20              /* 鉴权失败 */
#define ERR_URTSP_AUTH_TYPE_UNKNOWN                 21              /* 鉴权类型错误 */
#define ERR_URTSP_IP_VERSION_NOT_SUPPORT            22              /* IP协议版本在当前系统不支持 */
#define ERR_URTSP_NET_FAIL                          23              /* 网络错误 */
#define ERR_URTSP_NET_TIMEOUT                       24              /* 网络超时 */
#define ERR_URTSP_INTERACTION_ERROR                 25              /* 交互错误 */
#define ERR_URTSP_MEDIA_UNREADY                     26              /* 媒体流未就绪 */
#define ERR_URTSP_DISORDER_NEED_OUTPUT              27              /* 乱序整理队列数据需要输出 */
#define ERR_URTSP_DISORDER_NEED_WAIT                28              /* 乱序整理队列需要等待 */

/* 事件码 */
#define ERR_URTSP_STREAM_NUM_FULL                   1000            /* 流规格已满 */
#define ERR_URTSP_STREAM_PLAY_COMPLETE              1001            /* 播放完成 */
#define ERR_URTSP_STREAM_THIRD_STOP                 1002            /* 第三方停止流 */
#define ERR_URTSP_STREAM_UPDATE                     1003            /* 码流类型更新 */
#define ERR_URTSP_STREAM_STOP                       1004            /* 一体机国标流停流 */
#define ERR_URTSP_STREAM_DEV_SLEEP                  1005            /* 设备已休眠 */
#define ERR_URTSP_STREAM_TYPE_MAIN                  1006            /* 主流 */
#define ERR_URTSP_STREAM_TYPE_SUB                   1007            /* 辅流 */
#define ERR_URTSP_STREAM_TYPE_THIRD                 1008            /* 第三流 */
#define ERR_URTSP_STREAM_OVER_ABILITY               1009            /* 一体机流规格满 */
#define ERR_URTSP_STREAM_NOT_FOUND_RECORD           1010            /* 回放未查询到录像 */
#define ERR_URTSP_STREAM_PLAY_ABNORMAL              1011            /* 播放失败 */
#define ERR_URTSP_STREAM_OPTIONS_TIME               1012            /* OPTIONS上报时间戳 */
#define ERR_URTSP_STREAM_IS_THIRD_DEVICE            1013            /* 该码流来自HIK/DH三方设备 */

/* 设备响应错误码 */
#define ERR_URTSP_DEV_INTERNAL_ERROR                10000           /* 设备侧内部处理错误 */
#define ERR_URTSP_DEV_INNER_TIMEOUT                 10001           /* 内部处理超时 */
#define ERR_URTSP_DEV_STREAM_NOT_READY              10002           /* 流未就绪 */
#define ERR_URTSP_DEV_KEEP_ALIVE_FAIL               10003           /* 保活失败 */
#define ERR_URTSP_DEV_OVER_ABILITY                  10004           /* 回放能力不足 */
#define ERR_URTSP_DEV_SESSION_NOT_EXIST             10005           /* 会话不存在 */
#define ERR_URTSP_DEV_LIVE_EXIST                    10006           /* 实况业务已经建立 */
#define ERR_URTSP_DEV_LIVE_BUSY                     10007           /* 实况业务显示资源忙 */
#define ERR_URTSP_DEV_MULTICAST_DISABLE             10008           /* 组播使能关闭 */
#define ERR_URTSP_DEV_MULTICAST_PORT_USED           10009           /* 组播端口已被占用 */
#define ERR_URTSP_DEV_MULTICAST_PORT_EXHAUSTED      10010           /* 组播端口已耗尽 */
#define ERR_URTSP_DEV_MULTICAST_USER_NOT_EXIST      10011           /* 组播用户不存在 */
#define ERR_URTSP_DEV_CHANNEL_OFFLINE               10012           /* 通道不在线 */
#define ERR_URTSP_DEV_TALK_RES_INVALID              10013           /* 语音对讲资源编码无效 */
#define ERR_URTSP_DEV_TALK_RES_USED                 10014           /* 语音资源已被对讲使用 */
#define ERR_URTSP_DEV_TALK_EXIST                    10015           /* 语音对讲已存在 */
#define ERR_URTSP_DEV_TALK_NOT_EXIST                10016           /* 语音业务不存在 */
#define ERR_URTSP_DEV_TALK_BUILD_TIMEOUT            10017           /* 建立语音对讲业务超时 */
#define ERR_URTSP_DEV_TALK_FAIL                     10018           /* 语音对讲失败 */
#define ERR_URTSP_DEV_DESCRIBE_TIMEOUT              10019           /* describe超时（大GOP、磁盘读取数据慢导致） */
#define ERR_URTSP_DEV_VOD_NOT_FIND                  10020           /* 控制块不存在（资源已被释放后，仍进行回放控制操作） */
#define ERR_URTSP_DEV_INVALID_PLAY_PARAM            10021           /* 参数不正确（播放时间不在setup范围内，开始时间比结束时间晚、倍速不正确） */
#define ERR_URTSP_DEV_IPC_NOT_BIND                  10022           /* 通道未绑定，POE通道未接入IPC，非POE通道未添加IPC  */
#define ERR_URTSP_DEV_DISK_ABNOMAL                  10023           /* 磁盘异常 */
#define ERR_URTSP_DEV_UNDEF_ERROR                   10024           /* 未定义错误 */
#define ERR_URTSP_DEV_SUCCESS_CREATED               10025           /* SuccessCreated */
#define ERR_URTSP_DEV_SUCCESS_ACCEPTED              10026           /* SuccessAccepted */
#define ERR_URTSP_DEV_SUCCESS_NO_CONTENT            10027           /* SuccessNoContent */
#define ERR_URTSP_DEV_SUCCESS_PARTIAL_CONTENT       10028           /* SuccessPartialContent */
#define ERR_URTSP_DEV_SUCCESS_LOW_ON_STORAGE        10029           /* SuccessLowOnStorage */
#define ERR_URTSP_DEV_MULTIPLE_CHOICES              10030           /* MultipleChoices */
#define ERR_URTSP_DEV_REDIRECT_PERM_MOVED           10031           /* RedirectPermMoved */
#define ERR_URTSP_DEV_REDIRECT_TEMP_MOVED           10032           /* RedirectTempMoved */
#define ERR_URTSP_DEV_REDIRECT_SEE_OTHER            10033           /* RedirectSeeOther */
#define ERR_URTSP_DEV_REDIRECT_NOT_MODIFIED         10034           /* RedirectNotModified */
#define ERR_URTSP_DEV_USE_PROXY                     10035           /* UseProxy */
#define ERR_URTSP_DEV_BAD_REQUEST                   10036           /* 错误的请求 */
#define ERR_URTSP_DEV_UNAUTHORIZED                  10037           /* 未通过认证 */
#define ERR_URTSP_DEV_PAYMENT_REQUIRED              10038           /* 需要付费 */
#define ERR_URTSP_DEV_FORIBIDDEN                    10039           /* 禁止 */
#define ERR_URTSP_DEV_RECORD_NOT_FOUND              10040           /* 回放未查询到录像 */
#define ERR_URTSP_DEV_METHOD_NOT_ALLOWED            10041           /* 不允许该方法 */
#define ERR_URTSP_DEV_NOT_ACCEPTABLE                10042           /* 不接受 */
#define ERR_URTSP_DEV_PROXY_REQUIRED                10043           /* 代理需要认证 */
#define ERR_URTSP_DEV_REQUEST_TIMEOUT               10044           /* 请求超时 */
#define ERR_URTSP_DEV_CLIENT_CONFLICT               10045           /* ClientConflict */
#define ERR_URTSP_DEV_GONE                          10046           /* 不在服务器 */
#define ERR_URTSP_DEV_LENGTH_REQUIRED               10047           /* 需要长度 */
#define ERR_URTSP_DEV_PRECONDITION_FAILED           10048           /* 预处理失败 */
#define ERR_URTSP_DEV_ENTITY_TOO_LARGE              10049           /* 请求实体过长 */
#define ERR_URTSP_DEV_URI_TOO_LARGE                 10050           /* 请求-URI过长 */
#define ERR_URTSP_DEV_UNSUPPORTED_TYPE              10051           /* 媒体类型不支持 */
#define ERR_URTSP_DEV_NOT_UNDERSTOOD                10052           /* 不理解此参数 */
#define ERR_URTSP_DEV_CONFERENCE_NOT_FOUND          10053           /* 找不到会议 */
#define ERR_URTSP_DEV_BANDWIDTH_NOT_ENOUGH          10054           /* 带宽不足 */
#define ERR_URTSP_DEV_SESSION_NOT_FOUND             10055           /* 找不到会话 */
#define ERR_URTSP_DEV_METHOD_NOT_VALID              10056           /* 此状态下此方法无效 */
#define ERR_URTSP_DEV_HEADER_NOT_VALID              10057           /* 此头部域对该资源无效 */
#define ERR_URTSP_DEV_INVALID_RANGE                 10058           /* 无效范围 */
#define ERR_URTSP_DEV_PARAM_READ_ONLY               10059           /* 参数是只读的 */
#define ERR_URTSP_DEV_AO_NOT_ALLOWED                10060           /* 不允许合控制 */
#define ERR_URTSP_DEV_ONLY_AO_ALLOWED               10061           /* 只允许合控制 */
#define ERR_URTSP_DEV_UNSUPPORTED_TRANSPORT         10062           /* 传输方式不支持 */
#define ERR_URTSP_DEV_DESTINATION_UNREACHABLE       10063           /* 无法到达目的地址 */
#define ERR_URTSP_DEV_NOT_IMPLEMENTED               10064           /* 未实现 */
#define ERR_URTSP_DEV_BAD_GATEWAY                   10065           /* 网关错误 */
#define ERR_URTSP_DEV_SERVICE_UNAVAILABLE           10066           /* 无法得到服务 */
#define ERR_URTSP_DEV_VERSION_NOT_SUPPORTED         10067           /* 不支持此RTSP版本 */
#define ERR_URTSP_DEV_GATEWAY_TIMEOUT               10068           /* 网关超时 */
#define ERR_URTSP_DEV_OPTION_NOT_SUPPORTED          10069           /* 不支持选项 */

/* 宏定义 */
#define URTSP_LEN_256                               256
#define URTSP_LEN_512                               512
#define URTSP_LEN_64                                64


/*--------------------------------------回调函数定义---------------------------------------------*/

/**
* @brief  外部模块提供给RM模块的RTSP流数据输出回调函数
* @param  [IN] ulSessionNo 流数据
* @param  [IN] INT lBufLen RTSP 流数据大小
* @return VOID
* @note   1) 该函数内部实现必须简单 耗时短，比如只做简单拷贝
*/
typedef VOID (STDCALL *URTSP_DATA_OUTPUT_PF)(IN CHAR *pcDataBuf, IN ULONG_32 ulBufLen, IN VOID *pUserParam);

/**
* 外部模块提供给RM模块的会话状态报告回调函数 \n
* @param [in] ULONG ulSessionNo 会话编号
* @param [in] DULONG dulReserve 预留参数接口,对State进行说明
* @param [in] URTSP_EVENT_E enErrorType 错误状态类型
* @return ULONG
* @note 
*/
typedef VOID (STDCALL *URTSP_EVENT_PF)(IN ULONG ulState, IN DULONG dulReserve, IN VOID *pUserParam);


/*--------------------------------------枚举定义---------------------------------------------*/

/**
 * @enum tagRMStreamType
 * @brief 会话流类型
 * @attention 
 */
typedef enum tagRMStreamType
{
    URTSP_STREAM_TYPE_RTP_TCP       = 0,        /* RTP over TCP */
    URTSP_STREAM_TYPE_RTP_UDP,                  /* RTP over UDP */
    URTSP_STREAM_TYPE_TS_TCP,                   /* RTP over TCP */
    URTSP_STREAM_TYPE_TS_UDP,                   /* RTP over UDP */
    URTSP_STREAM_TYPE_ES_UDP,                   /* ES over UDP */
    URTSP_STREAM_TYPE_UNIJPEG_TCP,              /* 私有 Jpeg Protocol over TCP */
    URTSP_STREAM_TYPE_UNIMJPG_TCP,              /* 私有 Motion-Jpeg Protocol over TCP */
    URTSP_STREAM_TYPE_VOICE_TCP,                /* 语音对讲 */
    URTSP_STREAM_TYPE_BROADCAST_TCP,            /* 语音广播 */
    URTSP_STREAM_TYPE_MAX
}URTSP_STREAM_TYPE_E;

/**
 * @enum tagRMStreamUsage
 * @brief 会话流业务
 * @attention 
 */
typedef enum tagRMStreamUsage
{
    URTSP_STREAM_USAGE_REALPLAY       = 0,      /* 实况 */
    URTSP_STREAM_USAGE_PLAYBACK,                /* 回放 */
    URTSP_STREAM_USAGE_DOWNLOAD,                /* 下载 */
    URTSP_STREAM_USAGE_MAX
}URTSP_STREAM_USAGE_E;

/**
* @enum  tagURtspStreamCtrlE
* @brief 码流控制
* @attention
*/
typedef enum tagURtspStreamCtrlE
{
    URTSP_STREAM_CTRL_PAUSE = 0,                /* 暂停播放 */
    URTSP_STREAM_CTRL_RESUME,                   /* 恢复播放*/
    URTSP_STREAM_CTRL_SPEED,                    /* 设置倍速*/
    URTSP_STREAM_CTRL_TIME,                     /* 设置时间 */
    URTSP_STREAM_CTRL_SPEED_TIME,               /* 设置倍速+时间 */
    URTSP_STREAM_CTRL_MAX
}URTSP_STREAM_CTRL_E;

/**
 * @enum tagRMSessionOptionE
 * @brief RM Session的option
 * @attention
 */
typedef enum tagRMSessionOptionE
{
    URTSP_OPT_BUILDSTREAM_TIMEOUT = 0,          /* 建立媒体流连接超时时间(s)，目前仅IMCP_RM_StartStreamBySession和IMCP_RM_StartStreamV2可用 */
    URTSP_OPT_STREAM_GB,                        /* 国标流 */
    URTSP_OPT_MAX
}URTSP_OPT_E;

/**
* @enum tagRMPlayScale
* @brief 播放倍数枚举定义，高倍速建议使用抽帧（I帧）模式
* @attention 
*/
typedef enum tagRMPlayScale
{
    URTSP_SCALE_ONEEIGHTH_FORWARD = 0,          /* 1/8倍速播放 */
    URTSP_SCALE_QUARTER_FORWARD,                /* 1/4倍速播放 */
    URTSP_SCALE_HALF_FORWARD,                   /* 1/2倍速播放 */
    URTSP_SCALE_THREE_QUARTERS_FORWARD,         /* 3/4倍速前进播放 */
    URTSP_SCALE_1_FORWARD,                      /* 正常速度前进播放 */
    URTSP_SCALE_FIVE_FOURTHS_FORWARD,           /* 5/4倍速前进播放 */
    URTSP_SCALE_ONE_AND_HALF_FORWARD,           /* 3/2倍速前进播放 */
    URTSP_SCALE_2_FORWARD,                      /* 2倍速前进播放 */
    URTSP_SCALE_4_FORWARD,                      /* 4倍速前进播放 */
    URTSP_SCALE_8_FORWARD,                      /* 8倍速前进播放 */
    URTSP_SCALE_16_FORWARD,                     /* 16倍速前进播放 */
    URTSP_SCALE_32_FORWARD,                     /* 32倍速前进播放 */
    URTSP_SCALE_40_FORWARD,                     /* 40倍速前进播放 */
    URTSP_SCALE_64_FORWARD,                     /* 64倍速前进播放 */

    URTSP_SCALE_QUARTER_BACKWARD,               /* 1/4倍速后退播放 */
    URTSP_SCALE_HALF_BACKWARD,                  /* 1/2倍速后退播放 */
    URTSP_SCALE_1_BACKWARD,                     /* 正常速度后退播放 */
    URTSP_SCALE_2_BACKWARD,                     /* 2倍速后退播放 */
    URTSP_SCALE_4_BACKWARD,                     /* 4倍速后退播放 */
    URTSP_SCALE_8_BACKWARD,                     /* 8倍速后退播放 */
    URTSP_SCALE_16_BACKWARD,                    /* 16倍速后退播放 */
    URTSP_SCALE_32_BACKWARD,                    /* 32倍速后退播放 */

    URTSP_SCALE_2_FORWARD_IFRAME,               /* 2倍速前进播放(I帧) */
    URTSP_SCALE_4_FORWARD_IFRAME,               /* 4倍速前进播放(I帧) */
    URTSP_SCALE_8_FORWARD_IFRAME,               /* 8倍速前进播放(I帧) */
    URTSP_SCALE_16_FORWARD_IFRAME,              /* 16倍速前进播放(I帧) */
    URTSP_SCALE_32_FORWARD_IFRAME,              /* 32倍速前进播放(I帧)） */
    URTSP_SCALE_64_FORWARD_IFRAME,              /* 64倍速前进播放(I帧) */
    URTSP_SCALE_128_FORWARD_IFRAME,             /* 128倍速前进播放(I帧) */
    URTSP_SCALE_256_FORWARD_IFRAME,             /* 256倍速前进播放(I帧) */

    URTSP_SCALE_2_BACKWARD_IFRAME,              /* 2倍速后退播放(I帧) */
    URTSP_SCALE_4_BACKWARD_IFRAME,              /* 4倍速后退播放(I帧) */
    URTSP_SCALE_8_BACKWARD_IFRAME,              /* 8倍速后退播放(I帧) */
    URTSP_SCALE_16_BACKWARD_IFRAME,             /* 16倍速后退播放(I帧) */
    URTSP_SCALE_32_BACKWARD_IFRAME,             /* 32倍速后退播放(I帧) */
    URTSP_SCALE_64_BACKWARD_IFRAME,             /* 64倍速后退播放(I帧) */
    URTSP_SCALE_128_BACKWARD_IFRAME,            /* 128倍速后退播放(I帧) */
    URTSP_SCALE_256_BACKWARD_IFRAME,            /* 256倍速后退播放(I帧) */

    URTSP_SCALE_INTELLIGENT_FORWARD,            /* 智能播放 */
    URTSP_SCALE_FORWORD_TOP_SPEED,              /* 全速发流，用于下载功能 */

    URTSP_SCALE_1_FRAME_FORWARD,                /* 单帧前进 */
    URTSP_SCALE_I_FRAME_BACKWARD,               /* 单帧(I帧)后退> */

    URTSP_SCALE_STEP_FORWARD,                   /* 单步前进(XP单帧用该枚举，前端只支持单帧，中心录像支持逐帧) */
    URTSP_SCALE_STEP_BACKWARD,                  /* 单步后退(XP单帧用该枚举) */

    URTSP_SCALE_AUTO_STEP_FORWARD,              /* 连续单步前进 */
    URTSP_SCALE_AUTO_STEP_BACKWARD,             /* 连续单步后退 */
    URTSP_SCALE_DOWNLOAD_1,                     /* 一倍速全帧下载 */
    URTSP_SCALE_DOWNLOAD_2,                     /* 二倍速全帧下载 */
    URTSP_SCALE_DOWNLOAD_4,                     /* 四倍速全帧下载 */
    URTSP_SCALE_DOWNLOAD_8,                     /* 八倍速全帧下载 */
    URTSP_SCALE_SAMPLE_DOWNLOAD_1,              /* 一倍速抽帧下载 */
    URTSP_SCALE_SAMPLE_DOWNLOAD_2,              /* 二倍速抽帧下载 */
    URTSP_SCALE_SAMPLE_DOWNLOAD_4,              /* 四倍速抽帧下载 */
    URTSP_SCALE_SAMPLE_DOWNLOAD_8,              /* 八倍速抽帧下载 */

    URTSP_SCALE_MAX
}URTSP_PLAY_SCALE_E;

/** 
 * @enum tagRtspDealType 
 * @brief 启流类型
 * @attention 
 */ 
typedef enum tagRMRtspExecuteType 
{ 
    URTSP_EXECUTE_TYPE_STANDARD  =0,            /* 标准RTSP启流 */ 
    URTSP_EXECUTE_TYPE_FASTPLAY,                /* 快速启流，适合我司IPC/NVR */ 
    URTSP_EXECUTE_TYPE_MAX
}URTSP_EXECUTE_TYPE_E; 

/**
 * @enum tagURtspStreamMode
 * @brief 流模式
 * @attention 
 */
typedef enum tagURtspStreamMode
{
    URTSP_MODE_VIDEO_AUDIO  = 0,                /* 流模式：视频+音频 */
    URTSP_MODE_ONLY_VIDEO,                      /* 流模式：仅视频 */
    URTSP_MODE_ONLY_AUDIO,                      /* 流模式：仅音频 */
    URTSP_MODE_TO_TV,                           /* 回放上墙 */
    URTSP_MODE_MAX
}URTSP_STREAM_MODE_E;

/**
 * @enum tagRMPayloadTypeE
 * @brief  码流类型枚举
 * @attention
 */
typedef enum tagRMPayloadTypeE
{
    URTSP_PAYLOAD_TYPE_NONE    = 0,             /* 空 */
    URTSP_PAYLOAD_TYPE_G711U,                   /* G.711U */
    URTSP_PAYLOAD_TYPE_MP1L2,                   /* MPEG1 Layer2 */
    URTSP_PAYLOAD_TYPE_AAC,                     /* AAC */
    URTSP_PAYLOAD_TYPE_H264,                    /* H.264 */
    URTSP_PAYLOAD_TYPE_JPEG,                    /* JPEG(MJPEG) */
    URTSP_PAYLOAD_TYPE_G711A,                   /* G.711A */
    URTSP_PAYLOAD_TYPE_DATA,                    /* 自适应后端增强 */
    URTSP_PAYLOAD_TYPE_PCM,                     /* PCM音频44.1k采样率 */
    URTSP_PAYLOAD_TYPE_H265,                    /* H265 */
    URTSP_PAYLOAD_TYPE_SVAC,                    /* SVAC */
    URTSP_PAYLOAD_TYPE_NULL                     /* 无编码数据 */
}URTSP_PAYLOAD_TYPE_E;

/**
 * @enum tagRMSampleRateTypeE
 * @brief  采样率类型枚举
 * @attention
 */
typedef enum tagRMSampleRateTypeE
{
    URTSP_WAVE_FMT_PCM8M16          = 0,        /* PCM，8K采样率，单声道（M），16位采样 */
    URTSP_WAVE_FMT_PCM16M16         = 1,        /* PCM，16K采样率，单声道（M），16位采样 */
    URTSP_WAVE_FMT_PCM441M16        = 2,        /* PCM，44.1K采样率，单声道（M），16位采样 */
    URTSP_WAVE_FMT_PCM11025M16      = 3,        /* PCM，11.025K采样率，单声道（M），16位采样 */
    URTSP_WAVE_FMT_PCM8_RIGHT       = 4,        /* PCM，8K采样率，右声道 */
    URTSP_WAVE_FMT_PCM_NORMAL       = 5,        /* PCM，通用模式 */
    URTSP_WAVE_FMT_PCM48000M16      = 6,        /* PCM，48.0K采样率，单声道（M），16位采样 */
    URTSP_WAVE_FMT_UNKNOWN
}URTSP_SAMPLERATE_FMT_E;

/**
 * @enum tagRtspMethodE
 * @brief RTSP方法
 * @attention
 */
typedef enum tagURtspMethodE
{
    URTSP_METHOD_DESCRIBE           = 0,        /* DESCRIBE 方法 */
    URTSP_METHOD_SETUP              = 1,        /* SETUP    方法 */
    URTSP_METHOD_MAX
}URTSP_METHOD_E;

/**
 * @enum  tagURtspFuncE
 * @brief 通道功能是否支持枚举
 * @attention
 */
typedef enum tagURtspFuncE
{
    URTSP_FUNC_SCRAMBLING = 0,                  /* 加扰 */
    URTSP_FUNC_MAX                              /* 无 */
}URTSP_FUNC_E;

/**
 * @enum  tagURtspProtoE
 * @brief 通道功能是否支持枚举
 * @attention
 */
typedef enum tagURtspProtoE
{
    URTSP_PROTO_TCP,                            /* TCP协议 */
    URTSP_PROTO_UDP,                            /* UDP协议 */
    URTSP_PROTO_MAX
}URTSP_PROTO_E;

/**
 * @enum  tagURtspStreamPktTypeE
 * @brief 码流类型，判断码流为RTP+TS类型时去掉RTP头
 * @attention
 */
typedef enum tagURtspStreamPktTypeE
{
    URTSP_STREAM_PACKET_UNKOWN = 0,
    URTSP_STREAM_PACKET_TS = 1,
    URTSP_STREAM_PACKET_RTP_TS = 2,
    URTSP_STREAM_PACKET_RTP_OTHER = 3
}URTSP_STREAM_PKT_TYPE_E;

/*--------------------------------------结构体定义---------------------------------------------*/
/**
* @struct tagURtspTvParamS
* @brief  电视墙参数结构体
* @attention 无
*/
typedef struct tagURtspTvParamS
{
    CHAR szTvIp[64];                            /* 电视墙IP */
    USHORT usTvPort;                            /* 电视墙端口 */
    URTSP_PROTO_E eProto;                       /* 电视墙收流协议 */
}URTSP_TV_PARAM_S;

/**
* @struct tagURtspToTvInfoS
* @brief  回放上墙信息结构体
* @attention 无
*/
typedef struct tagURtspToTvInfoS
{
    CHAR szServerIp[64];                        /* 服务器响应IP */
    USHORT usServerPort;                        /* 服务器响应端口 */
    URTSP_PROTO_E eProto;                       /* 服务器响应协议TCP/UDP */
}URTSP_TO_TV_INFO_S;

/**
* @struct tagURtmpStreamParamS
* @brief  启流参数结构体
* @attention 无
*/
typedef struct tagURtspStreamParamS
{
    CHAR szUrl[URTSP_LEN_512];                  /* 流URL */
    URTSP_STREAM_TYPE_E eStreamType;            /* 流类型 */
    URTSP_STREAM_MODE_E eStreamMode;            /* 流模式 */
    URTSP_STREAM_USAGE_E eStreamUsage;          /* 流用途 */
    URTSP_EXECUTE_TYPE_E eRtspExecuteType;      /* RTSP执行模式 */
    CHAR szServerIp[URTSP_LEN_64];              /* 服务器IP*/
    USHORT usServerPort;                        /* 服务器端口 */
    CHAR szRecvIp[URTSP_LEN_64];                /* 收流IP，默认本机为"" */
    USHORT usRecvPort;                          /* 接收端口（UDP收流/TCP带外收流使用） */
    BOOL_T bKeepAlive;                          /* 是否需要保活 */
    URTSP_PLAY_SCALE_E eSpeed;                  /* 播放倍速 */
    TIME_T tiStartPlayTime;                     /* 播放起始时间 */
    TIME_T tiEndPlayTime;                       /* 播放结束时间 */
    ULONG_32 ulKeepAlivems;                     /* 保活间隔（单位: ms，此值传0时使用默认保活时间） */
    BOOL_T bExtraBusiCode;                      /* RTSP协商时是否使用额外的业务码(主要用于跨域下载,此字段为false时跨域只下载I帧) */
}URTSP_STREAM_PARAM_S;

/**
 * @struct tagRMRtspPayloadType
 * @brief EZR RTSP媒体流信息结构
 * @attention
 */
typedef struct tagRMRtspPayloadTypeS
{
    ULONG_32 ulType;                            /* 流类型（0：视频，1：音频）*/
    UCHAR ucPayloadType;                        /* payload值*/
    UCHAR ucReserved[3];                        /* 填充位，无意义*/
    URTSP_PAYLOAD_TYPE_E ePayloadType;          /* 码流类型*/
    ULONG ulSampleRate;                         /* 采样频率*/
}URTSP_PAYLOAD_TYPE_S;

/**
 * @struct tagRMRtspPayloadType
 * @brief EZR RTSP音频信息结构
 * @attention
 */
typedef struct tagRMRtspAudioInfoS
{
    UCHAR ucAudioType;                          /* 音频协议类型*/
    UCHAR ucFrequencyIndex;                     /* 音频采样率索引*/
    UCHAR ucChannelNum;                         /* 音频声道数*/
    URTSP_PAYLOAD_TYPE_E ucVoiceType;           /* 语音对讲下行音频码流类型*/
    URTSP_SAMPLERATE_FMT_E ulVoiceSampleRate;   /* 语音对讲下行采样频率*/
    UCHAR ucReserved;                           /* 填充位，无意义*/
}URTSP_AUDIO_INFO_S;

/**
* @struct tagUrtspAuthInfo
 * @brief EZR 鉴权信息
* @attention 无
*/
typedef struct tagUrtspAuthInfo
{
   CHAR szNonce[64];                            /* Nonce值*/
   CHAR szRealm[32];                            /* Realm值*/
   CHAR szAlgorithm[32];                        /* 鉴权算法*/
}URTSP_AUTH_INFO_S;

/**
 * @struct tagURtspRsaInfoS
 * @brief RSA加解密信息
 * @attention 无
*/
typedef struct tagURtspRsaInfoS
{
    ULONG_32 ulBit;                             /* 密钥bit值 */
    ULONG_32 ulPub;                             /* 公钥值 */
    CHAR cN[260];                               /* n */
    CHAR cPri[260];                             /* 私钥值 */
    CHAR cP[132];                               /* p */
    CHAR cQ[132];                               /* q */
    CHAR cDP[132];                              /* dp */
    CHAR cDQ[132];                              /* dq */
    CHAR cQP[132];                              /* qp */
}URTSP_RSA_INFO_S;

/**
 * @struct tagURtspFuncSupS
 * @brief 功能支持信息
 * @attention 无
*/
typedef struct tagURtspFuncSupS
{
    URTSP_FUNC_E eFuncType;                     /* 功能类型*/
    BOOL_T bSupport;                            /* 支持标志*/
}URTSP_FUNC_SUP_S;

/**
 * @struct tagURtspMsgFieldS
 * @brief rtsp消息字段描述信息
 * @attention 无
*/
typedef struct tagURtspMsgFieldS
{
    URTSP_METHOD_E eRtspMethod;                 /* RTSP方法 */
    CHAR *pcRtspFieldKey;                       /* RTSP请求字段键 */
    CHAR *pcRtspFieldValue;                     /* RTSP请求字段值 */
}URTSP_MSG_FIELD_S;

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of __URTSP_COM_DEF_H__ */