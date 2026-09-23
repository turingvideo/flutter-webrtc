#ifndef _UHLS_COM_DEF_H
#define _UHLS_COM_DEF_H

#include "umedia_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 错误码 */
#define ERR_UHLS_SUCCESS                        0   /* 成功 */
#define ERR_UHLS_FAIL                           1   /* 执行失败 */
#define ERR_UHLS_INVALID_PARAM                  2   /* 参数非法 */
#define ERR_UHLS_ALLOC_FAIL                     3   /* 内存分配失败 */
#define ERR_UHLS_NO_M3U8                        4   /* 未加载M3U8 */
#define ERR_UHLS_NODE_NOT_EXIST                 5   /* 时间节点不存在 */


#define UHLS_CONTROL_PARAM ULONG_32     /* UHLS_CONTROL命令的参数 */

typedef enum tagUHlsControlE
{
    UHLS_CONTROL_PLAY        = 0,       /* 设置开始播放 */
    UHLS_CONTROL_RESUME         ,       /* 设置恢复播放 */
    UHLS_CONTROL_PAUSE          ,       /* 设置播放暂停 */
    UHLS_CONTROL_SPEED          ,       /* 设置播放速度，附加参数为枚举值 UHLS_SPEED_E  */
    UHLS_CONTROL_SEEK                   /* 设置跳转，附件参数为要跳转的时间戳 */
}UHLS_CONTROL_E;

typedef enum tagUHlsSpeedE
{
    UHLS_SPEED_1_8           = 0,       /* 8倍速慢放 */
    UHLS_SPEED_1_4              ,       /* 4倍速慢放 */
    UHLS_SPEED_1_2              ,       /* 2倍速慢放 */
    UHLS_SPEED_NORMAL           ,       /* 1倍速播放 */
    UHLS_SPEED_2                ,       /* 2倍速快放 */
    UHLS_SPEED_4                ,       /* 4倍速快放 */
    UHLS_SPEED_8                        /* 8倍速快放 */
}UHLS_SPEED_E;

typedef enum tagUHlsEventE
{
    UHLS_EVENT_DOWNLOAD_FINISH            = 0,     /**< 下载完成 */
    UHLS_EVENT_PARSE_DOMAIN_FAIL             ,     /**< 解析域名失败 */
    UHLS_EVENT_CONNECT_FAIL                  ,     /**< 连接失败 */
    UHLS_EVENT_CONNECT_TIMEOUT               ,     /**< 连接超时 */
    UHLS_EVENT_DOWNLOAD_TIMEOUT              ,     /**< 下载超时 */
    UHLS_EVENT_DOWNLOAD_FAIL                 ,     /**< 下载失败 */
    UHLS_EVENT_NETWORK_POOR                  ,     /**< 网络较差 */
    UHLS_EVENT_PLAY_FINISH                   ,     /**< 播放完成 */
    UHLS_EVENT_DISK_FULL                     ,     /**< 磁盘空间满 */
    UHLS_EVENT_AUTH_FAIL                     ,     /**< 鉴权失败 */
    UHLS_EVENT_CURRENT_TIME                  ,     /**< 当前播放时间，仅用于上报 */
    UHLS_EVENT_PRIOR_DISK_FULL               ,     /**< 磁盘预值满 */
    UHLS_EVENT_NODE_NOT_EXIST                ,     /**< 时间节点不存在 */
    UHLS_EVENT_NO_CACHE_PATH                 ,     /**< 未设置缓存路径 */
    UHLS_EVENT_MSG_SEND_FAIL                 ,     /**< 消息发送失败 */
    UHLS_EVENT_TASK_CANCELLED                ,     /**< 任务已取消 */
    UHLS_EVENT_STREAM_CONTINUE               ,     /**< 流继续播放 */
    UHLS_EVENT_CONNECT_FAIL_AND_PLAY_FINISH  ,     /**< 网络连接异常且当前缓存已播放完成 */
    UHLS_EVENT_UNKNOWN                             /**< 未知事件 */
}UHLS_EVENT_E;


/**
 * @struct      tagUHlsTSPacketS
 * @brief       读取到的TS包
 * @attention   无
*/
typedef struct tagUHlsTSPacketS
{
    UCHAR *pucData;
    ULONG_32 ulSize;
}UHLS_TS_PACKET_S;

/**
 * @struct      tagUHlsReportMsgS
 * @brief       上报事件
 * @attention   无
*/
typedef struct tagUHlsReportMsgS
{
    UHLS_EVENT_E eEvent;        /* 事件码 */
    DULONG      dulReserved;    /* 保留参数（携带数值型附加信息） */
    CONST CHAR* pcDescription;  /* 事件内容描述 */
}UHLS_REPORT_MSG_S;

/**
 * @struct      tagUHlsGlobalParamS
 * @brief       UHLS模块初始化参数
 * @attention   无
*/
typedef struct tagUHlsGlobalParamS
{
    ULONG_32 ulMaxCacheSize;    /* 最大缓存值，单位MB */
    CHAR *pcCachePath;          /* 缓存路径 */
}UHLS_GLOBAL_PARAM_S;

/**
 * @brief       TS包数据回调函数指针
 * @param       pstTsPacket Ts包数据
 * @param       pUserParam  用户参数
 * @attention   无
*/
typedef VOID(STDCALL *UHLS_DATA_OUTPUT_CB_PF)(IN UHLS_TS_PACKET_S *pstTsPacket,
                                              INOUT VOID *pUserParam);

/**
 * @brief       UHLS事件上报回调函数
 * @param       pstMsg      上报事件
 * @param       pUserParam  用户参数
 * @attention   无
*/
typedef VOID(STDCALL *UHLS_REPORT_CB_PF)(IN UHLS_REPORT_MSG_S *pstMsg,
                                         INOUT VOID *pUserParam);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _UHLS_COM_DEF_H */
