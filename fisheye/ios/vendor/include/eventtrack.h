#ifndef _EVENTTRACK_H_
#define _EVENTTRACK_H_

#ifdef __ANDROID__

#ifdef __cplusplus
extern "C"
{
#endif

/* key-value 拆分 */
typedef struct TUNNEL_C_REPORT_LogSplitInfo
{
    char szSubKey[32];      /* 拆分的子Key */
    char szSplitStart[32];  /* szSubKey 的值在 value 中的起始位置 */
    char szSplitEnd[32];    /* szSubKey 的值在 value 中的终止位置 */
} TUNNEL_C_REPORT_LOG_SPLITINFO_S;

/* 添加 key:value, 注意区分是否是第一次添加, 如果是第一次添加需要先初始化头部信息 */
int TUNNEL_C_REPORT_AddLog(const char *pcLogkey, const char *pcLogValue, const TUNNEL_C_REPORT_LOG_SPLITINFO_S (*apstLogDivInfo)[2], size_t ulLogDivInfoSize);

#ifdef __cplusplus
}
#endif

#endif /* __ANDROID__ */

#endif /* _EVENTTRACK_H_ */
