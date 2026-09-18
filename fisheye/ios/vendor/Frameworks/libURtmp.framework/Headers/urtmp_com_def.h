#ifndef _URTMP_COM_H_
#define _URTMP_COM_H_

#include "umedia_com_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------错误码定义---------------------------------------------*/
#define ERR_URTMP_SUCCESS                       0x00000000      /* 执行成功 */
#define ERR_URTMP_FAIL                          0x00000001      /* 执行失败 */
#define ERR_URTMP_INVALID_PARAM                 0x00000002      /* 输入参数非法 */
#define ERR_URTMP_NO_MEMORY                     0x00000003      /* 系统内存不足 */
#define ERR_URTMP_NET_INIT_FAIL                 0x00000004      /* 网络初始化失败 */
#define ERR_URTMP_SESSION_FULL                  0x00000005      /* 会话号满 */
#define ERR_URTMP_CONNECT_FAIL                  0x00000006      /* RTMP 连接失败 */
#define ERR_URTMP_CREATETHREAD_FAIL             0x00000007      /* 创建线程失败 */
#define ERR_URTMP_RECV_DATA_NOTENOUGH           0x00000008      /* 接收数据不足 */
#define ERR_URTMP_RECV_DATA_ABNORMAL            0x00000009      /* 接收数据异常 */
#define ERR_URTMP_FLV_PARSE_FAIL                0x0000000A      /* FLV格式解析失败 */
#define ERR_URTMP_SESSION_INVALID               0x0000000B      /* 输入会话号非法 */
#define ERR_URTMP_REPORT_INIT_FAILED            0x0000000C      /* 上报模块初始化失败 */

/*--------------------------------------宏定义---------------------------------------------*/
#define URTMP_URL_MAXLEN    256                                 /* url最大长度 */

/*--------------------------------------枚举定义---------------------------------------------*/
/**
* @enum  tagURtmpEventE
* @brief 会话上报错误
* @attention
*/
typedef enum tagURtmpEventE
{
    URTMP_EVENT_NET_FAIL = 0,               /* 会话网络错误 */
    URTMP_EVENT_NET_TIMEOUT,                /* 会话网络超时 */
    URTMP_EVENT_STREAM_NUM_FULL,            /* 流数已经满 */
    URTMP_EVENT_STREAM_THIRDSTOP,           /* 第三方停止流 */
    URTMP_EVENT_RTMP_CONNECT_FAIL,          /* RTMP连接失败 */
    URTMP_EVENT_RTMP_INIT_FAIL,             /* RTMP初始化失败 */
    URTMP_EVENT_MAX
}URTMP_EVETN_E;

/*--------------------------------------结构体定义---------------------------------------------*/
/**
* @struct tagURtmpStreamParamS
* @brief  启流参数结构体
* @attention 无
*/
typedef struct tagURtmpStreamParamS
{
    CHAR szUrl[URTMP_URL_MAXLEN];           /* 流URL */
}URTMP_STREAM_PARAM_S;

/**
* @struct tagURtmpSrcDataS
* @brief  原始码流结构体
* @attention 无
*/
typedef struct tagURtmpSrcDataS
{
    CHAR *pcData;                           /* 缓存区地址 */
    ULONG_32 ulLen;                         /* 缓存区数据长度 */
}URTMP_SRC_DATA_S;

/*--------------------------------------回调函数定义---------------------------------------------*/
/**
* @brief 通道状态回调函数
* @param [IN] hHandle               通道句柄
* @param [IN] pUserParam            用户参数
* @param [IN] eEvent                会话事件码
* @return 无
* @note
*/
typedef VOID (*URTMP_EVENT_PF)(IN HANDLE hHandle, IN URTMP_EVETN_E eEvent, IN VOID *pUserParam);

/**
* @brief 原始码流数据回调函数
* @param [IN] hHandle               通道句柄
* @param [IN] pUserParam            用户参数
* @param [IN] pstSrcData            原始码流数据结构指针
* @return 无
* @note   1) 该函数内部实现必须简单 耗时短，比如只做简单拷贝
*/
typedef VOID (*URTMP_DATA_OUTPUT_PF)(IN HANDLE hHandle, IN URTMP_SRC_DATA_S *pstSrcData, IN VOID *pUserParam);

#ifdef __cplusplus
}
#endif

#endif /* end of _UTRMP_COM_H_ */