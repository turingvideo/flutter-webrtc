#ifndef _MEDIASDK_COM_DEF_H
#define _MEDIASDK_COM_DEF_H

#include "media_sdk_com_type.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* 函数标准调用约定 Standard function calling convention */
#ifdef WIN32
    #define STDCALL                 __stdcall
    #ifdef MEDIASDK_DLL_EXPORTS
        #define MEDIASDK_EXPORT     __declspec( dllexport )
    #else
        #define MEDIASDK_EXPORT     __declspec( dllimport )
    #endif
#else
    #ifdef i386
        #define STDCALL             __attribute__((stdcall))
    #else
        #define STDCALL
    #endif
    #define MEDIASDK_EXPORT         __attribute__((visibility("default")))
#endif

/*********************** 错误码定义规则 ***********************************
MediaSDK错误码范围定义在73000—74999之间，错误码对应16进制如下：
    73000: 0x0001 1D28
    74999: 0x0001 24F7
按下述方式划分类别：
    0x0001 1D28--0x0001 1D80
    0x0001 1D81--0x0001 1DFF
    0x0001 1E00--0x0001 1E80
    0x0001 1E81--0x0001 1EFF
    ...
*********************** 错误码定义规则 ***********************************/

#define ERR_MEDIASDK_SUCCEED                            0x00000000      /* 执行成功 */

/****************************************** 通用错误码（0x00011D28--0x00011D80） ********************************/
#define ERR_MEDIASDK_FAIL                               0x00011D28      /* 执行失败 */
#define ERR_MEDIASDK_INVALID_PARAM                      0x00011D29      /* 参数非法 */
#define ERR_MEDIASDK_NULL_PTR                           0x00011D2A      /* 指针为空 */
#define ERR_MEDIASDK_NO_MEMORY                          0x00011D2B      /* 系统内存不足 */
#define ERR_MEDIASDK_PORT_INVALID                       0x00011D2C      /* 非法通道号 */
#define ERR_MEDIASDK_NOT_SUPPORT                        0x00011D2D      /* 功能不支持 */
#define ERR_MEDIASDK_UNSUPPORT_FMT                      0x00011D2E      /* 不支持的格式 */
#define ERR_MEDIASDK_GET_MORE_DATA                      0x00011D2F      /* 需要继续获取输出数据 */
#define ERR_MEDIASDK_SYS_FAIL                           0x00011D30      /* 系统方法调用失败 */
#define ERR_MEDIASDK_CREATE_THREAD_FAIL                 0x00011D31      /* 线程创建失败 */
#define ERR_MEDIASDK_LOAD_DLL_FAILED                    0x00011D32      /* 加载动态库失败 */
#define ERR_MEDIASDK_STREAM_IN_STARTING                 0x00011D33      /* 异步启流中 */
#define ERR_MEDIASDK_GPU_DECODE_NOT_ENABLE              0x00011D34      /* 未使能硬解 */
#define ERR_MEDIASDK_WND_IS_NOT_VISIBLE                 0x00011D35      /* 窗口不可见 */
#define ERR_MEDIASDK_STREAM_ALREADY_START               0x00011D36      /* 已启动流操作 */
#define ERR_MEDIASDK_RECORD_ALREADY_START               0x00011D37      /* 已启动录像操作 */
#define ERR_MEDIASDK_FILE_ALREADY_PLAY                  0x00011D38      /* 已启动文件播放操作 */
#define ERR_MEDIASDK_NOT_GET_STREAM_INFO                0x00011D39      /* 未获取到流信息 */
#define ERR_MEDIASDK_GET_PORT_RES_FAIL                  0x00011D3A      /* 获取播放通道资源失败 */
#define ERR_MEDIASDK_RES_NOT_EXIST                      0x00011D3B      /* 资源不存在 */
#define ERR_MEDIASDK_LOG_CLOSE                          0x00011D3C      /* 日志关闭 */
#define ERR_MEDIASDK_STREAM_IN_STOPPING                 0x00011D3D      /* 正在异步停流中 */
#define ERR_MEDIASDK_STREAM_IN_PAUSING                  0x00011D3E      /* 正在异步暂停中 */

/**************************************** 运行过程上报（0x00011D81--0x00011DFF） ********************************/
#define ERR_MEDIASDK_AUDIO_EXIST                        0x00011D81      /* 音频存在消息 */
#define ERR_MEDIASDK_VIDEO_EXIST                        0x00011D82      /* 视频存在消息 */
#define ERR_MEDIASDK_PLAYBACK_FINISH                    0x00011D83      /* 回放结束（从RTP解析） */
#define ERR_MEDIASDK_FIRSTFRAME_FINISH                  0x00011D84      /* 首帧解码完成 */
#define ERR_MEDIASDK_VIDEO_NET_STUCK                    0x00011D85      /* 检测到网络卡顿 */
#define ERR_MEDIASDK_MERGE_FINISH                       0x00011D86      /* 文件合并完成 */
#define ERR_MEDIASDK_DROP_CACHED_FRAMES                 0x00011D87      /* 缓存过多，主动丢包 */
#define ERR_MEDIASDK_RECORD_QUEUE_EMPTY                 0x00011D88      /* 录制队列帧数据为0 */
#define ERR_MEDIASDK_RECORD_RESOLUTION_OVER_LIMIT       0x00011D89	    /* 录制分辨率超出限制(超过4K不支持水印录制) */

/************************************** 音频模块错误码（0x00011E00--0x00011E80） ********************************/
/* ------------------------------音频采集错误码---------------------------- */
#define ERR_MEDIASDK_MIC_NOT_EXIST                      0x00011E00      /* 麦克风不存在 */
#define ERR_MEDIASDK_MIXER_OPEN_DEV_FAIL                0x00011E01      /* 打开混音器输入设备失败 */
#define ERR_MEDIASDK_MIXER_GET_DEV_INFO_FAIL            0x00011E02      /* 获取混音器输入设备信息失败 */
#define ERR_MEDIASDK_MIXER_GET_LINE_INFO_FAIL           0x00011E03      /* 获取指定的首个音频线路信息失败 */
#define ERR_MEDIASDK_MIXER_GET_LINE_CONTROL_FAIL        0x00011E04      /* 获取波形音频输入混音器音频标识失败 */
#define ERR_MEDIASDK_MIXER_SET_CONTROL_DETAIL_FAIL      0x00011E05      /* 设置控制信息失败 */
#define ERR_MEDIASDK_MIXER_GET_CONTROL_DETAIL_FAIL      0x00011E06      /* Mixer获取控制信息失败 */
#define ERR_MEDIASDK_CAP_DEVICE_CREATE_FAIL             0x00011E07      /* 耳麦不存在 */
#define ERR_MEDIASDK_CAP_BUFFER_CREATE_FAIL             0x00011E08      /* 音频捕获缓存创建失败 */
#define ERR_MEDIASDK_SET_NOTIFY_POSITION_FAIL           0x00011E09      /* 音频捕获设置通知位置失败 */
#define ERR_MEDIASDK_CAPTURE_FAIL                       0x00011E0A      /* 音频采集实例内部错误 */
#define ERR_MEDIASDK_MCM_SPEAKER_NOT_EXIST              0x00011E0B      /* 扬声器不存在 */
#define ERR_MEDIASDK_MCM_SPEAKER_MORE_THAN_ONE          0x00011E0C      /* 扬声器超过一个 */
/* ------------------------------音频播放错误码---------------------------- */
#define ERR_MEDIASDK_CREATE_DEV_FAIL                    0x00011E41      /* 音频播放实例创建失败 */
#define ERR_MEDIASDK_PLAY_DEVICE_CREATE_FAIL            0x00011E42      /* 音频播放设备初始化失败 */
#define ERR_MEDIASDK_SET_COORPERATIVA_LEVEL_FAIL        0x00011E43      /* 音频播放设备初始化设置失败 */
#define ERR_MEDIASDK_PLAYER_FAIL                        0x00011E44      /* 音频播放实例内部错误 */
#define ERR_MEDIASDK_GET_PLAY_VOLUME_FAIL               0x00011E45      /* 获取音频播放音量失败 */
#define ERR_MEDIASDK_SET_PLAY_VOLUME_FAIL               0x00011E46      /* 设置音频播放音量失败 */
#define ERR_MEDIASDK_LOCK_FAIL                          0x00011E47      /* 音频锁定缓冲区资源失败 */
#define ERR_MEDIASDK_UNLOCK_FAIL                        0x00011E48      /* 音频解锁缓冲区资源失败 */
#define ERR_MEDIASDK_LOOP_PLAY_FAIL                     0x00011E49      /* 循环播放音频失败 */
#define ERR_MEDIASDK_INPUT_DATA_ZERO                    0x00011E4A      /* 音频输入数据长度为0 */
#define ERR_MEDIASDK_POINTER_INVALID                    0x00011E4B      /* 音频句柄指针非法 */
#define ERR_MEDIASDK_GET_PLAY_POSITION_FAIL             0x00011E4C      /* 获取音频播放位置失败 */
#define ERR_MEDIASDK_SET_PLAY_POSITION_FAIL             0x00011E4D      /* 设置音频播放位置失败 */
#define ERR_MEDIASDK_PLAY_BUFFER_HIGH_LEVEL             0x00011E4E      /* 音频播放缓冲区高水位 */
#define ERR_MEDIASDK_PLAY_BUFFER_LOW_LEVEL              0x00011E4F      /* 音频播放缓冲区低水位 */

/************************************** 文件播放错误码（0x00011E81--0x00011EFF） ********************************/
#define ERR_MEDIASDK_GET_PIC_HW_FAIL                    0x00011E81      /* 获取图像宽高失败 */
#define ERR_MEDIASDK_DECODER_OUTPUT_FAIL                0x00011E82      /* 解码输出设置失败 */
#define ERR_MEDIASDK_FILESRC_END                        0x00011E83      /* 文件读取结束 */
#define ERR_MEDIASDK_FILESRC_NOT_OPEN                   0x00011E84      /* 文件没有打开 */
#define ERR_MEDIASDK_FILESRC_READ_FAIL                  0x00011E85      /* 读文件失败 */
#define ERR_MEDIASDK_FILESRC_DESTROY                    0x00011E86      /* 文件损坏 */
#define ERR_MEDIASDK_FILESRC_CLOSE_FAIL                 0x00011E87      /* 关闭文件失败 */
#define ERR_MEDIASDK_FILESRC_OPEN_FAIL                  0x00011E88      /* 打开文件操作失败 */
#define ERR_MEDIASDK_FILESRC_INDEX_FAIL                 0x00011E89      /* 文件索引失败 */
#define ERR_MEDIASDK_FILESRC_FSEEK_FAIL                 0x00011E8A      /* 文件跳播失败 */
#define ERR_MEDIASDK_FILESRC_FAIL_TO_BUILD_INDEX        0x00011E8B      /* 文件索引创建失败 */
#define ERR_MEDIASDK_FILESRC_FILE_READ_HEAD             0x00011E8C      /* 读取到文件开始 */
#define ERR_MEDIASDK_FILESRC_PAYLOAD_INVALID            0x00011E8D      /* 文件中无效的负载格式 */
#define ERR_MEDIASDK_FILESRC_CODEC_INVALID              0x00011E8E      /* 文件中无效的编码格式 */
#define ERR_MEDIASDK_FILESRC_WRITE_FRAME_FAIL           0x00011E8F      /* 写视频帧错误 */
#define ERR_MEDIASDK_FILESRC_WRITE_AUDIO_FAIL           0x00011E90      /* 写音频帧错误 */
#define ERR_MEDIASDK_FILESRC_SET_PLAYTIME_FAIL          0x00011E91      /* 设置读取位置失败 */
#define ERR_MEDIASDK_FILESRC_UNKNOWN_STREAM_TYPE        0x00011E92      /* 文件中无效码流格式 */
#define ERR_MEDIASDK_FILE_SPELL_FAIL                    0x00011E93      /* 拼帧失败 */
#define ERR_MEDIASDK_PACKET_OVERSIZE                    0x00011E94      /* 数据包过大 */
#define ERR_MEDIASDK_FILE_JUMP_OVER_LEN                 0x00011E95      /* 跳播无效 */
#define ERR_MEDIASDK_FILE_PAUSE                         0x00011E96      /* 文件处于暂停状态 */
#define ERR_MEDIASDK_FILE_STEP_READ_COMPLETE            0x00011E97      /* 单帧播放结束 */
#define ERR_MEDIASDK_FILE_FRAMERATE_INVALID             0x00011E98      /* 文件帧率不正确 */
#define ERR_MEDIASDK_FILE_PATH_FIND_FAILED              0x00011E99      /* 查找文件路径失败 */
#define ERR_MEDIASDK_FILE_READ_TOO_FAST                 0x00011E9A      /* 文件读取过快 */ 
#define ERR_MEDIASDK_FILE_READ_TOO_SLOW                 0x00011E9B      /* 文件读取过慢,需加快读取 */ 
#define ERR_MEDIASDK_FILE_JUMP_NOW                      0x00011E9C      /* 当前文件正在跳转 */
#define ERR_MEDIASDK_FILESRC_HAVE_OPEN                  0x00011E9D      /* 文件已经打开 */
#define ERR_MEDIASDK_HD_CAPACITY_NOT_ENOUGH             0x00011E9E      /* 硬盘空间不足，达到下限（100M） */
#define ERR_MEDIASDK_HD_CAPACITY_NOT_ENOUGH_MAX         0x00011E9F      /* 硬盘空间不足，达到上报阈值（500M）*/
#define ERR_MEDIASDK_HD_GETDISKSPACE_FAIL               0x00011EA0      /* 获得磁盘空间剩余空间失败 */
#define ERR_MEDIASDK_VIDEO_RECORD_PART                  0x00011EA1      /* 录像已分段 */
#define ERR_MEDIASDK_UCODE_NOT_SUPPORT_BACK             0x00011EA2      /* UCODE 文件不支持倒放 */
#define ERR_MEDIASDK_FILE_ENCRYPED                      0x00011EA3      /* 文件加密需要输入秘钥 */
#define ERR_MEDIASDK_RESOLUTION_CHANGE                  0x00011EA4      /* 播放通道的媒体流分辨率发生变化 */
#define ERR_MEDIASDK_TOO_LARGE                          0x00011EA5      /* 播放通道的录像文件太大 */
#define ERR_MEDIASDK_TOTALTIME_FAIL                     0x00011EA6      /* 获取文件时长失败 */

/****************************************** 画图元数据（0x00011F00--0x00011F80） ********************************/
#define ERR_MEDIASDK_INIT_CREATEDATA_FAIL               0x00011F00      /* 创建绘图整体数据结构失败 */
#define ERR_MEDIASDK_DRAW_NO_GRAPHICS                   0x00011F01      /* 未获得绘图器 */
#define ERR_MEDIASDK_DRAW_CREATEDIBSECTION_FAIL         0x00011F02      /* 创建与设备无关的位图失败 */
#define ERR_MEDIASDK_CONVERSION_NO_YUVMEMORY            0x00011F03      /* 未获得转YUV可用内存空间 */
#define ERR_MEDIASDK_CONVERSION_OUT_PICTURE             0x00011F04      /* 超过源图片区域 */
#define ERR_MEDIASDK_IVA_DATA_LEN_INVALID               0x00011F05      /* 画图数据长度不合法 */
#define ERR_MEDIASDK_IVA_DATA_POINTER_INVALID           0x00011F06      /* 画图数据指针非法 */
#define ERR_MEDIASDK_DRAW_LINE_FAIL                     0x00011F07      /* GDI PLUS画线失败 */
#define ERR_MEDIASDK_DRAW_POLYGON_FAIL                  0x00011F08      /* GDI PLUS画多边形失败 */
#define ERR_MEDIASDK_DRAW_LABEL_LINE_FAIL               0x00011F09      /* GDI PLUS画方向标志线失败 */
#define ERR_MEDIASDK_DRAW_RECT_LABEL_LINE_FAIL          0x00011F0A      /* GDI PLUS画矩形方向标志线失败 */
#define ERR_MEDIASDK_DRAW_FISHEYE_WND_FAIL              0x00011F0B      /* 根据坐标点绘制窗口边框失败 */

/***************************************** 四目全景PTZ（0x00011F81--0x00011FFF） ********************************/
#define ERR_MEDIASDK_PTZ_RESOUCE_FAIL                   0x00011F81      /* 四目全景 ptz资源错误 */
#define ERR_MEDIASDK_PTZ_STREAM_EXIST                   0x00011F82      /* 四目全景 ptz流存在，仅用于上报 */
#define ERR_MEDIASDK_PTZ_STREAM_NOT_EXIST               0x00011F83      /* 四目全景 ptz流不存在，仅用于上报 */
#define ERR_MEDIASDK_PTZ_GET_WNDID_FROM_POINT_FAIL      0x00011F84      /* 通过点坐标找PTZ窗口号失败 */
#define ERR_MEDIASDK_PTZ_NUM_INVALID                    0x00011F85      /* PTZ图像数量非法 */
#define ERR_MEDIASDK_PTZ_POINTER_INVALID                0x00011F86      /* PTZ指针非法 */
#define ERR_MEDIASDK_PTZ_CREATE_RES_FAIL                0x00011F87      /* PTZ创建图像处理对象失败 */
#define ERR_MEDIASDK_PTZ_IMAGE_STITCH_FAIL              0x00011F88      /* PTZ图像拼接失败 */
#define ERR_MEDIASDK_PTZ_IMAGE_PROCESS_FAIL             0x00011F89      /* PTZ图像处理失败 */
#define ERR_MEDIASDK_PTZ_RESULT_DATA_LEN_INVALID        0x00011F8A      /* PTZ规则信息长度非法 */
#define ERR_MEDIASDK_PTZ_NOT_STITCH_MODE                0x00011F8B      /* 非PTZ拼接模式 */

/******************************************** 鱼眼矫正（0x00012000--0x00012080） ********************************/
#define ERR_MEDIASDK_FISHEYE_RESOUCE_FAIL               0x00012000      /* 鱼眼资源错误 */
#define ERR_MEDIASDK_FISHEYE_GET_WNDID_FROM_POINT_FAIL  0x00012001      /* 通过点坐标找寻鱼眼PTZ窗口号失败 */
#define ERR_MEDIASDK_FISHEYE_ADJUST_NOT_NEED            0x00012002      /* 不需要鱼眼矫正 */
#define ERR_MEDIASDK_FISHEYE_ADJUST_FAIL                0x00012003      /* 鱼眼矫正失败 */
#define ERR_MEDIASDK_FISHEYE_STREAM_EXIST               0x00012004      /* 鱼眼流存在，仅用于上报 */
#define ERR_MEDIASDK_FISHEYE_STREAM_NOT_EXIST           0x00012005      /* 鱼眼流不存在，仅用于上报 */
#define ERR_MEDIASDK_FISHEYE_MODE_INVALID               0x00012006      /* 非法鱼眼模式（非ptz、安装模式） */
#define ERR_MEDIASDK_FISHEYE_POINTER_INVALID            0x00012007      /* 鱼眼数据指针非法 */
#define ERR_MEDIASDK_FISHEYE_PTZ_MODE_INVALID           0x00012008      /* 非法鱼眼ptz模式 */
#define ERR_MEDIASDK_FISHEYE_FIX_MODE_INVALID           0x00012009      /* 非法鱼眼安装模式 */

/********************************************* POS信息（0x00012081--0x000120FF） ********************************/
#define ERR_MEDIASDK_POS_STREAM_EXIST                   0x00012081      /* POS信息流存在，仅用于上报 */
#define ERR_MEDIASDK_POS_STREAM_NOT_EXIST               0x00012082      /* POS信息流不存在，仅用于上报 */

/**************************************** 流模块错误码（0x00012100--0x00012180） ********************************/
/* -------------------------------------错误码----------------------------------- */
#define ERR_MEDIASDK_URL_NOT_STANDARD                   0x00012100      /* URL不标准 */
#define ERR_MEDIASDK_URL_NO_USER                        0x00012101      /* URL未携带用户名 */
#define ERR_MEDIASDK_URL_NO_PASSWORD                    0x00012102      /* URL未携带密码 */
#define ERR_MEDIASDK_NEED_AUTH                          0x00012103      /* 需要鉴权 */
#define ERR_MEDIASDK_AUTH_TYPE_UNKNOWN                  0x00012104      /* 鉴权类型未知 */
#define ERR_MEDIASDK_AUTH_FAIL                          0x00012105      /* 鉴权失败 */
#define ERR_MEDIASDK_RTSP_MSG_PARSE_FAIL                0x00012106      /* RTSP消息解析失败 */
#define ERR_MEDIASDK_NET_FAIL                           0x00012107      /* 网络错误 */
#define ERR_MEDIASDK_NET_TIMEOUT                        0x00012108      /* 网络超时 */
#define ERR_MEDIASDK_INTERACTION_ERROR                  0x00012109      /* 交互错误 */
#define ERR_MEDIASDK_DEVICE_ERROR                       0x0001210A      /* 设备内部错误 */
#define ERR_MEDIASDK_STREAM_ASYNC_SUCCEED               0x0001210B      /* 异步流操作成功 */
#define ERR_MEDIASDK_ADDRESSINUSE                       0x0001210C      /* 网络端口被使用中 */
#define ERR_MEDIASDK_MEDIA_UNREADY                      0x0001210D      /* 媒体流未就绪 */
#define ERR_MEDIASDK_STREAM_ASYNC_STOP_SUCCEED          0x0001210E      /* 异步停流成功 */
#define ERR_MEDIASDK_HLS_NO_M3U8                        0x0001210F      /* 云存储回放未设置M3U8数据 */
#define ERR_MEDIASDK_HLS_NODE_NOT_EXIST                 0x00012110      /* 云存储回放未查询到指定播放节点 */
#define ERR_MEDIASDK_FLV_PARSE_FAIL                     0x00012111      /* FLV格式解析失败 */
#define ERR_MEDIASDK_RTMP_CONNECT_FAIL                  0x00012112      /* RTMP 连接失败 */
/* -------------------------------------事件码----------------------------------- */
#define ERR_MEDIASDK_STREAM_NUM_FULL                    0x00012141      /* 流数已满 */
#define ERR_MEDIASDK_STREAM_PLAY_COMPLETE               0x00012142      /* 播放完成 */
#define ERR_MEDIASDK_STREAM_THIRD_STOP                  0x00012143      /* 第三方停止流 */
#define ERR_MEDIASDK_STREAM_UPDATE                      0x00012144      /* 码流类型更新 */
#define ERR_MEDIASDK_STREAM_STOP                        0x00012145      /* 一体机国标流停止 */
#define ERR_MEDIASDK_STREAM_DEV_SLEEP                   0x00012146      /* 设备已休眠 */
#define ERR_MEDIASDK_STREAM_TYPE_MAIN                   0x00012147      /* 主流 */
#define ERR_MEDIASDK_STREAM_TYPE_SUB                    0x00012148      /* 辅流 */
#define ERR_MEDIASDK_STREAM_TYPE_THIRD                  0x00012149      /* 第三流 */
#define ERR_MEDIASDK_STREAM_OVER_ABILITY                0x0001214A      /* 一体机流规格满*/
#define ERR_MEDIASDK_STREAM_NOT_FOUND_RECORD            0x0001214B      /* 回放未查询到录像 */
#define ERR_MEDIASDK_STREAM_PLAY_ABNORMAL               0x0001214C      /* 播放失败 */
#define ERR_MEDIASDK_HLS_DOWNLOAD_FINISH                0x0001214D      /* 云存储回放：内部下载任务完成 */
#define ERR_MEDIASDK_HLS_DOWNLOAD_FAILED                0x0001214E      /* 云存储回放：内部下载出错 */
#define ERR_MEDIASDK_HLS_PARSE_DOMAIN_FAIL              0x0001214F      /* 云存储回放：解析M3U8内部域名出错 */
#define ERR_MEDIASDK_HLS_NETWORK_POOR                   0x00012150      /* 云存储回放：网络较差 */
#define ERR_MEDIASDK_HLS_PLAY_FINISH                    0x00012151      /* 云存储回放：播放完成 */
#define ERR_MEDIASDK_RTMP_INIT_FAIL                     0x00012152      /* RTMP初始化失败 */
#define ERR_MEDIASDK_STREAM_IS_THIRD_DEVICE             0x00012153      /* 该码流来自HIK/DH三方设备 */

/****************************************** 截图错误码（0x00012181--0x000121FF） ********************************/
#define ERR_MEDIASDK_SNAP_FINISH                        0x00012181      /* 截图结束 */
#define ERR_MEDIASDK_JPEG_ENC_FAIL                      0x00012182      /* JPEG编码失败 */
#define ERR_MEDIASDK_CREATE_ENCODER_FAIL                0x00012183      /* 创建编码器失败 */
#define ERR_MEDIASDK_SNAP_NOT_GET_DATA                  0x00012184      /* 未获取到图像数据 */


    /*@brief 路径最大长度 */
#define MEDIASDK_PATH_MAX_LEN                     256

/*@brief 最大通道数 */
#define MEDIASDK_MAXPORT_NUM                      512

    /*@brief 水印信息Max长度 */
#define MEDIASDK_WATRE_MARK_MAX_LEN                      512


    /* 一条规则的最大坐标点数 */
#define MEDIASDK_IVA_REPORT_COORD_NUM                    16



    /** @brief RTSP 流模式 仅视频 */
#define MEDIASDK_RTSP_STREAM_ONLY_VIDEO           0x8000
    /** @brief RTSP 流模式 仅音频 */
#define MEDIASDK_RTSP_STREAM_ONLY_AUDIO           0x4000
    /** @brief RTSP 流模式 视频+音频 */
#define MEDIASDK_RTSP_STREAM_VIDEOAUDIO           0x0000

    /*@brief URL地址长度 */
#define MEDIASDK_LEN_16                           16
#define MEDIASDK_LEN_48                           48
#define MEDIASDK_LEN_64                           64
#define MEDIASDK_LEN_128                          128
#define MEDIASDK_LEN_256                          256
#define MEDIASDK_LEN_512                          512
#define MEDIASDK_LEN_1024                         1024

/*@brief 通用CODE长度 */
#define MEDIASDK_CODE_LEN                           48             /* 平台支持的有效最大位数为47位 */

/*@brief 时间信息字符串长度 */
#define MEDIASDK_TIME_LEN                           32

/*@brief 录像文件大小 */
#define RECORD_FILE_MIN_SIZE                    10
#define RECORD_FILE_MAX_SIZE                    4000
#define ONE_MB_TO_B                             1048576

/*@brief 包列表长度 */
#define NET_PACKET_LIST_LEN                     5

/*@brief 录像时间 */
#define RECORD_1MIN_TO_SECOND                   60

/**
* @enum tagMediaSDKLogLevel
* @brief 运行日志等级枚举类型
* @attention 无
*/
typedef enum tagMediaSDKLogLevel
{
    MEDIASDK_LOG_CLOSE        = 0,       /**< 不打印 */
    MEDIASDK_LOG_FATAL        = 1,       /**< fatal级别 */
    MEDIASDK_LOG_ERROR        = 2,       /**< error级别 */
    MEDIASDK_LOG_WARN         = 3,       /**< warn级别 */
    MEDIASDK_LOG_INFO         = 4,       /**< info级别 */
    MEDIASDK_LOG_DEBUG        = 5        /**< debug级别 */
}MEDIASDK_LOGLEVEL_E;

/**
* @enum tagMediaSDKVideoFrameType
* @brief 视频帧类型的枚举定义
* @attention 无
*/
typedef enum tagMediaSDKVideoFrameType
{
    MEDIASDK_VIDEO_FRAME_I    = 0,        /**< 视频帧为I帧 */
    MEDIASDK_VIDEO_FRAME_P    = 1,        /**< 视频帧为P帧 */
    MEDIASDK_VIDEO_FRAME_B    = 2,        /**< 视频帧为B帧 */
    MEDIASDK_VIDEO_FRAME_UNKNOWN
}MEDIASDK_VIDEO_FRAME_TYPE_E;

/**
* @enum tagMediaSDKDecodeFrameType
* @brief 视频帧类型的枚举定义
* @attention 无
*/
typedef enum tagMediaSDKDecodeFrameType
{
    MEDIASDK_DECODE_FRAME_I    = 0,        /**< 只解码I帧 */
    MEDIASDK_DECODE_FRAME_P    = 1,        /**< 只解码P帧 */
    MEDIASDK_DECODE_FRAME_B    = 2,        /**< 只解码B帧 */
    MEDIASDK_DECODE_FRAME_ALL  = 3,        /**< 解码全部帧 */
    MEDIASDK_DECODE_FRAME_NONE = 4,        /**< 不解码任何帧 */
}MEDIASDK_DECODE_FRAME_TYPE_E;

/**
* @enum tagMediaSDKVideoEncodeType
* @brief 视频编码格式枚举定义
* @attention 无
*/
typedef enum tagMediaSDKVideoEncodeType
{
    MEDIASDK_VIDEO_ENCODE_MPEG2   = 0,      /**< 视频符合MPEG-2标准 */
    MEDIASDK_VIDEO_ENCODE_MPEG4   = 1,      /**< 视频符合MPEG-4标准 */
    MEDIASDK_VIDEO_ENCODE_H264    = 2,      /**< 视频符合H.264标准 */
    MEDIASDK_VIDEO_ENCODE_MJPEG   = 3,      /**< 视频符合MJPEG标准 */
    MEDIASDK_VIDEO_ENCODE_H265    = 4,      /**< 视频符合H.265标准 */
    MEDIASDK_VIDEO_ENCODE_SVAC    = 5,      /**< 视频符合SVAC标准 */
    MEDIASDK_VIDEO_ENCODE_NULL              /**< 无视频 */
}MEDIASDK_VIDEO_ENCODE_TYPE_E;

/**
* @enum tagMediaSDKAudioEncodeType
* @brief 音频编码格式枚举定义
* @attention 无
*/
typedef enum tagMediaSDKAudioEncodeType
{
    MEDIASDK_AUDIO_ENCODE_PCM8M      = 0,      /**< PCM，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_PCM16M     = 1,      /**< PCM，16K采样率 */
    MEDIASDK_AUDIO_ENCODE_G711U8M    = 2,      /**< G711U，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_G711U16M   = 3,      /**< G711U，16K采样率 */
    MEDIASDK_AUDIO_ENCODE_G711A8M    = 4,      /**< G711A，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_G711A16M   = 5,      /**< G711A，16K采样率 */
    MEDIASDK_AUDIO_ENCODE_AAC8M      = 6,      /**< AAC，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_AAC16M     = 7,      /**< AAC，16K采样率 */
    MEDIASDK_AUDIO_ENCODE_AAC32M     = 8,      /**< AAC，32K采样率 */
    MEDIASDK_AUDIO_ENCODE_AAC48M     = 9,      /**< AAC，48K采样率 */
    MEDIASDK_AUDIO_ENCODE_MP2        = 10,     /**< MP2 */
    MEDIASDK_AUDIO_ENCODE_SVAC       = 11,     /**< SVAC Audio 16位采样率 */
    MEDIASDK_AUDIO_ENCODE_G722_16M   = 12,     /**< G722，16K采样率 */
    MEDIASDK_AUDIO_ENCODE_G723_1_8M  = 13,     /**< G723.1，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_G726_8M    = 14,     /**< G726，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_G729_8M    = 15,     /**< G729，8K采样率 */
    MEDIASDK_AUDIO_ENCODE_NULL                 /**< 无音频 */
}MEDIASDK_AUDIO_ENCODE_TYPE_E;

/**
* @enum tagMediaSDKWaveFormatInfo
* @brief 解码后音频格式的枚举类型定义
* @attention 无
*/
typedef enum tagMediaSDKWaveFormatInfo
{
    MEDIASDK_WAVE_FORMAT_PCM8M16          = 0,        /**< PCM，8K采样率，单声道（M），16位采样 */
    MEDIASDK_WAVE_FORMAT_PCM16M16         = 1,        /**< PCM，16K采样率，单声道（M），16位采样 */
    MEDIASDK_WAVE_FORMAT_PCM441M16        = 2,        /**< PCM，44.1K采样率，单声道（M），16位采样 */
    MEDIASDK_WAVE_FORMAT_PCM11025M16      = 3,        /**< PCM，11.025K采样率，单声道（M），16位采样 */
    MEDIASDK_WAVE_FORMAT_PCM8_RIGHT       = 4,        /**< PCM，8K采样率，右声道 */
    MEDIASDK_WAVE_FORMAT_PCM_NORMAL       = 5,        /**< PCM，通用模式 */
    MEDIASDK_WAVE_FORMAT_PCM48000M16      = 6,        /**< PCM，48.0K采样率，单声道（M），16位采样 */
    MEDIASDK_WAVE_FORMAT_UNKNOWN
}MEDIASDK_WAVE_FORMAT_INFO_E;


/**
* @enum tagMediaSDKRenderScaleE
* @brief 视频图像显示比例枚举定义
* @attention 无
*/
typedef enum tagMediaSDKRenderScaleE
{
    MEDIASDK_RENDER_SCALE_FULL            = 0,        /**< 满窗格显示 */
    MEDIASDK_RENDER_SCALE_PROPORTION      = 1,        /**< 按照比例显示 */
    MEDIASDK_RENDER_SCALE_REAL            = 2         /**< 实际大小显示 */
}MEDIASDK_RENDER_SCALE_E;

/**
* @enum tagMediaSDKPictureFluencyE
* @brief 图像播放流畅性枚举类型定义
* @attention 无
*/
typedef enum tagMediaSDKPictureFluencyE
{
    MEDIASDK_PICTURE_REAL         = 0,                /**< 实时性优先 */
    MEDIASDK_PICTURE_FLUENCY      = 1,                /**< 流畅性优先 */
    MEDIASDK_PICTURE_BALANCE      = 2,                /**< 均衡 */
    MEDIASDK_PICTURE_USER_DEFINED = 3,                /**< 自定义-缓冲帧数可配 */
    MEDIASDK_PICTURE_NETADJUST    = 4,                /**< 网络抖动自适应模式*/
    MEDIASDK_PICTURE_MAX
}MEDIASDK_PICTURE_FLUENCY_E;

/**
* @enum tagMediaSDKPlayScale
* @brief 播放倍数枚举定义
* @attention 
*/
typedef enum tagMediaSDKPlayScale
{
    MEDIASDK_SCALE_QUARTER_FORWARD          = 0,            /**< 1/4倍速播放 */
    MEDIASDK_SCALE_HALF_FORWARD             = 1,            /**< 1/2倍速播放 */
    MEDIASDK_SCALE_1_FORWARD                = 2,            /**< 正常速度前进播放 */
    MEDIASDK_SCALE_2_FORWARD                = 3,            /**< 2倍速前进播放 */
    MEDIASDK_SCALE_4_FORWARD                = 4,            /**< 4倍速前进播放 */
    MEDIASDK_SCALE_8_FORWARD                = 5,            /**< 8倍速前进播放 */
    MEDIASDK_SCALE_16_FORWARD               = 6,            /**< 16倍速前进播放 */

    MEDIASDK_SCALE_16_BACKWARD              = 7,            /**< 16倍速后退播放 */
    MEDIASDK_SCALE_8_BACKWARD               = 8,            /**< 8倍速后退播放 */
    MEDIASDK_SCALE_4_BACKWARD               = 9,            /**< 4倍速后退播放 */
    MEDIASDK_SCALE_2_BACKWARD               = 10,           /**< 2倍速后退播放 */
    MEDIASDK_SCALE_1_BACKWARD               = 11,           /**< 正常速度后退播放 */
    MEDIASDK_SCALE_HALF_BACKWARD            = 12,           /**< 1/2倍速后退播放 */
    MEDIASDK_SCALE_QUARTER_BACKWARD         = 13,           /**< 1/4倍速后退播放 */

    MEDIASDK_SCALE_2_FORWARD_IFRAME         = 14,           /**< 2倍速前进播放(I帧) */
    MEDIASDK_SCALE_4_FORWARD_IFRAME         = 15,           /**< 4倍速前进播放(I帧) */
    MEDIASDK_SCALE_8_FORWARD_IFRAME         = 16,           /**< 8倍速前进播放(I帧) */
    MEDIASDK_SCALE_16_FORWARD_IFRAME        = 17,           /**< 16倍速前进播放(I帧) */

    MEDIASDK_SCALE_2_BACKWARD_IFRAME        = 18,           /**< 2倍速后退播放(I帧) */
    MEDIASDK_SCALE_4_BACKWARD_IFRAME        = 19,           /**< 4倍速后退播放(I帧) */
    MEDIASDK_SCALE_8_BACKWARD_IFRAME        = 20,           /**< 8倍速后退播放(I帧) */
    MEDIASDK_SCALE_16_BACKWARD_IFRAME       = 21,           /**< 16倍速后退播放(I帧) */

    MEDIASDK_SCALE_INTELLIGENT_FORWARD      = 22,           /**< 智能播放 */
    MEDIASDK_SCALE_1_FRAME_FORWARD          = 23,           /**< 单帧前进 */
    MEDIASDK_SCALE_I_FRAME_BACKWARD         = 24,           /**< 单帧(I帧)后退> */

    MEDIASDK_SCALE_40_FORWARD               = 25,           /**< 40倍速前进播放 */

    MEDIASDK_SCALE_32_FORWARD_IFRAME        = 26,           /**< 32倍速抽帧前进播放 */
    MEDIASDK_SCALE_32_BACKWARD_IFRAME       = 27,           /**< 32倍速抽帧后退播放 */
    MEDIASDK_SCALE_64_FORWARD_IFRAME        = 28,           /**< 64倍速抽帧前进播放 */
    MEDIASDK_SCALE_64_BACKWARD_IFRAME       = 29,           /**< 64倍速抽帧后退播放 */
    MEDIASDK_SCALE_128_FORWARD_IFRAME       = 30,           /**< 128倍速抽帧前进播放 */
    MEDIASDK_SCALE_128_BACKWARD_IFRAME      = 31,           /**< 128倍速抽帧后退播放 */
    MEDIASDK_SCALE_256_FORWARD_IFRAME       = 32,           /**< 256倍速抽帧前进播放 */
    MEDIASDK_SCALE_256_BACKWARD_IFRAME      = 33,           /**< 256倍速抽帧后退播放 */

    MEDIASDK_SCALE_32_FORWARD               = 34,           /**< 32倍速前进播放 */
    MEDIASDK_SCALE_32_BACKWARD              = 35,           /**< 32倍速后退播放 */
    MEDIASDK_SCALE_FORWORD_TOP_SPEED        = 36,           /**< 全速发流，用于下载功能 */
    MEDIASDK_SCALE_ONEEIGHTH_FORWARD        = 37,           /**< 1/8倍速播放 */

    /* Begin: add by wangjiannan w09479 2023-07-16 for 文昌新增三种倍速播放方式 */
    MEDIASDK_SCALE_THREE_QUARTERS_FORWARD   = 38,           /**< 3/4倍速前进播放 */
    MEDIASDK_SCALE_FIVE_FOURTHS_FORWARD     = 39,           /**< 5/4倍速前进播放 */
    MEDIASDK_SCALE_ONE_AND_HALF_FORWARD     = 40,           /**< 3/2倍速前进播放 */
    /* End: add by wangjiannan w09479 2023-07-16 for 文昌新增三种倍速播放方式 */
    /* Begin: add by wangjiannan w09479 2023-08-05 for 开平智慧城市32/64倍速回放 */
    MEDIASDK_SCALE_64_FORWARD               = 41,           /**< 64倍速前进播放 */
    MEDIASDK_SCALE_SUPER_FAST_FORWARD       = 42,           /**< 超快前进播放 */
    MEDIASDK_SCALE_AUTO_STEP_FORWARD        = 43,           /**< 连续单步前进 */
    MEDIASDK_SCALE_AUTO_STEP_BACKWARD       = 44,           /**< 连续单步后退 */

    /* XP倍速枚举 */
    MEDIASDK_SCALE_XP_16_BACKWARD  = 1000,      /**< 16倍速后退播放 */
    MEDIASDK_SCALE_XP_8_BACKWARD,               /**< 8倍速后退播放 */
    MEDIASDK_SCALE_XP_4_BACKWARD,               /**< 4倍速后退播放 */
    MEDIASDK_SCALE_XP_2_BACKWARD,               /**< 2倍速后退播放 */
    MEDIASDK_SCALE_XP_1_BACKWARD,               /**< 正常速度后退播放 */
    MEDIASDK_SCALE_XP_HALF_BACKWARD,            /**< 1/2倍速后退播放 */
    MEDIASDK_SCALE_XP_QUARTER_BACKWARD,         /**< 1/4倍速后退播放 */
    MEDIASDK_SCALE_XP_QUARTER_FORWARD,          /**< 1/4倍速播放 */
    MEDIASDK_SCALE_XP_HALF_FORWARD,             /**< 1/2倍速播放 */
    MEDIASDK_SCALE_XP_1_FORWARD,                /**< 正常速度前进播放 */
    MEDIASDK_SCALE_XP_2_FORWARD,                /**< 2倍速前进播放 */
    MEDIASDK_SCALE_XP_4_FORWARD,                /**< 4倍速前进播放 */
    MEDIASDK_SCALE_XP_8_FORWARD,                /**< 8倍速前进播放 */
    MEDIASDK_SCALE_XP_16_FORWARD,               /**< 16倍速前进播放 */
    MEDIASDK_SCALE_XP_SUPER_FAST_FORWARD,       /**< 超快前进播放 */
    MEDIASDK_SCALE_XP_ONEEIGHTH_FORWARD,        /**< 1/8倍速播放 */
    MEDIASDK_SCALE_XP_STEP_FORWARD,             /**< 单步前进 */
    MEDIASDK_SCALE_XP_STEP_BACKWARD,            /**< 单步后退 */
    MEDIASDK_SCALE_XP_AUTO_STEP_FORWARD,        /**< 连续单步前进 */
    MEDIASDK_SCALE_XP_AUTO_STEP_BACKWARD,       /**< 连续单步后退 */
    MEDIASDK_SCALE_XP_DOWNLOAD_1,               /**< 一倍速全帧下载 */
    MEDIASDK_SCALE_XP_DOWNLOAD_2,               /**< 二倍速全帧下载 */
    MEDIASDK_SCALE_XP_DOWNLOAD_4,               /**< 四倍速全帧下载 */
    MEDIASDK_SCALE_XP_DOWNLOAD_8,               /**< 八倍速全帧下载 */
    MEDIASDK_SCALE_XP_SAMPLE_DOWNLOAD_1,        /**< 一倍速抽帧下载 */
    MEDIASDK_SCALE_XP_SAMPLE_DOWNLOAD_2,        /**< 二倍速抽帧下载 */
    MEDIASDK_SCALE_XP_SAMPLE_DOWNLOAD_4,        /**< 四倍速抽帧下载 */
    MEDIASDK_SCALE_XP_SAMPLE_DOWNLOAD_8,        /**< 八倍速抽帧下载 */
    /* Begin: add by wangjiannan w09479 2023-07-16 for 文昌新增三种倍速播放方式 */
    MEDIASDK_SCALE_XP_THREE_QUARTERS_FORWARD,   /**< 3/4倍速前进播放 */
    MEDIASDK_SCALE_XP_FIVE_FOURTHS_FORWARD,     /**< 5/4倍速前进播放 */
    MEDIASDK_SCALE_XP_ONE_AND_HALF_FORWARD,     /**< 3/2倍速前进播放 */
    /* End: add by wangjiannan w09479 2023-07-16 for 文昌新增三种倍速播放方式 */
    /* Begin: add by wangjiannan w09479 2023-08-05 for 开平智慧城市32/64倍速回放 */
    MEDIASDK_SCALE_XP_32_FORWARD,               /**< 32倍速前进播放 */
    MEDIASDK_SCALE_XP_64_FORWARD,               /**< 64倍速前进播放 */
    /* End: add by wangjiannan w09479 2023-08-05 for 开平智慧城市32/64倍速回放 */

    MEDIASDK_SCALE_MAX
}MEDIASDK_PLAY_SCALE_E;

/**
* @enum tagMediaSDKRunInfoType
* @brief 独立播放器上报运行信息的类型的枚举定义
* @attention 无
*/
typedef enum tagMediaSDKRunInfoType
{
    MEDIASDK_RUN_INFO_RECORD_VIDEO        = 0,        /**< 本地录像过程中上报运行信息 */
    MEDIASDK_RUN_INFO_MEDIA_PROCESS       = 1,        /**< 视频媒体处理过程中的上报运行信息 */
    MEDIASDK_RUN_INFO_MEDIA_VOICE         = 2,        /**< 语音业务过程中上报运行信息 */
    MEDIASDK_RUN_INFO_STREAM              = 3,        /**< 网络流过程中上报运行信息 */
    MEDIASDK_RUN_INFO_SNATCH              = 4,        /**< 截图过程中的上报运行信息 */
    MEDIASDK_RUN_INFO_MAX
}MEDIASDK_RUN_INFO_TYPE_E;

/**
* @enum tagMediaSDKSourceDataFormat
* @brief 媒体数据流格式
* @attention 无
*/
typedef enum tagMediaSDKSourceDataFormat
{
    MEDIASDK_SOURCE_DATA_TS                = 0,        /**< TS封装码流 */
    MEDIASDK_SOURCE_DATA_ES12              = 1,        /**< RTP + 音/视频私有头（12字节） + ES码流 */
    MEDIASDK_SOURCE_DATA_ES12_TIME         = 2,        /**< RTP + 时间戳（8字节） + 音/视频私有头（12字节） + ES码流 */
    MEDIASDK_SOURCE_DATA_RTP_ES            = 3,        /**< RTP + ES码流 */
    MEDIASDK_SOURCE_DATA_HIK               = 4,        /**< 海康码流，回调的第一个包是海康40字节系统头 */
    MEDIASDK_SOURCE_DATA_DH                = 5,        /**< 大华码流 */
    MEDIASDK_SOURCE_DATA_RM                = 6,        /**< 锐明码流，回调的第一个包是锐明40字节系统头 */
    MEDIASDK_SOURCE_DATA_TDY_SWALLOW       = 7,        /**< TDY-SWALLOW */
    MEDIASDK_SOURCE_DATA_DMA_SWALLOW       = 8,        /**< DMA-SWALLOW */
    MEDIASDK_SOURCE_DATA_NANZI             = 9,        /**< 南自码流 */
    MEDIASDK_SOURCE_DATA_RTP3984           = 10,       /**< 国电的rfc3984码流 */
    MEDIASDK_SOURCE_DATA_RTP_PS            = 11,       /**< RTP+PS */
    MEDIASDK_SOURCE_DATA_RTP_H264          = 12,       /**< RTP+H264 */
    MEDIASDK_SOURCE_DATA_MOBOTIX           = 13,       /**< mobotix码流 */
    MEDIASDK_SOURCE_DATA_NONE                          /* 未识别码流格式 */
}MEDIASDK_SOURCE_DATA_FORMAT_E;


/** 
 * @enum tagMediaRtspExecuteType 
 * @brief RTSP启流类型
 * @attention 
 */ 
typedef enum tagMediaRtspExecuteType 
{ 
    MEDIASDK_RTSP_EXECUTE_TYPE_STANDARD   =0,             /**< 标准RTSP启流 */ 
    MEDIASDK_RTSP_EXECUTE_TYPE_FASTPLAY,                  /**< 快速启流，适合宇视IPC/NVR */ 
    MEDIASDK_RTSP_EXECUTE_TYPE_MAX 
}MEDIASDK_RTSP_EXECUTE_TYPE_E; 

/**
 * @enum tagMediaRtspStreamMode
 * @brief RTSP流模式
 * @attention 
 */
typedef enum tagMediaRtspStreamMode
{
    MEDIASDK_RTSP_MODE_VIDEO_AUDIO  = 0,                /* 流模式：视频+音频 */
    MEDIASDK_RTSP_MODE_ONLY_VIDEO,                      /* 流模式：仅视频 */
    MEDIASDK_RTSP_MODE_ONLY_AUDIO,                      /* 流模式：仅音频 */
    MEDIASDK_RTSP_MODE_TO_TV,                           /* 回放上墙 */
    MEDIASDK_RTSP_MODE_MAX
}MEDIASDK_RTSP_STREAM_MODE_E;

/**
* @enum tagMediaSDKStreamType
* @brief 启流业务的枚举定义
* @attention 无
*/
typedef enum tagMediaSDKStreamType
{
    MEDIASDK_STREAM_RTSP_REALPLAY        = 0,        /**< RTSP实况 */
    MEDIASDK_STREAM_RTSP_PLAYBACK        = 1,        /**< RTSP回放 */
    MEDIASDK_STREAM_RTSP_DOWNLOAD        = 2,        /**< RTSP下载 */
    MEDIASDK_STREAM_RTSP_VOICE           = 3,        /**< RTSP语音对讲 */
    MEDIASDK_STREAM_SMC_REALPLAY         = 4,        /**< SMC实况 */
    MEDIASDK_STREAM_SMC_PLAYBACK         = 5,        /**< SMC回放 */
    MEDIASDK_STREAM_SMC_DOWNLOAD         = 6,        /**< SMC下载 */
    MEDIASDK_STREAM_SMC_VOICE            = 7,        /**< SMC语音对讲 */
    MEDIASDK_STREAM_RTMP_REALPLAY        = 8,        /**< RTMP实况 */
    MEDIASDK_STREAM_HLS_PLAYBACK         = 9,        /**< HLS回放 */
    MEDIASDK_STREAM_NET_REALPLAY         = 10,       /**< 网络流实况 */
    MEDIASDK_STREAM_NET_VOICE            = 11,       /**< 网络流语音对讲/广播 */
    MEDIASDK_STREAM_FILE_INPUT           = 12,       /**< 文件输入流 */
    MEDIASDK_STREAM_DOWNLOAD_WATERMARK   = 13,       /**< 下载&录屏+水印 */
    MEDIASDK_STREAM_MAX
}MEDIASDK_STREAM_TYPE_E;


/**
 * @enum tagRMStreamType
 * @brief rtsp协议类型
 * @attention 
 */
typedef enum tagMediaSDKRtspType
{
    MEDIASDK_RTSP_RTP_TCP             = 0,                /**< RTP over TCP */
    MEDIASDK_RTSP_RTP_UDP,                                /**< RTP over UDP */
    MEDIASDK_RTSP_TS_TCP,                                 /**< TS over TCP */
    MEDIASDK_RTSP_TS_UDP,                                 /**< TS over UDP */
    MEDIASDK_RTSP_UNIJPEG_TCP,                            /**< 私有 Jpeg Protocol over TCP */
    MEDIASDK_RTSP_UNIMJPG_TCP,                            /**< 私有 Motion-Jpeg Protocol over TCP */
    MEDIASDK_RTSP_VOICE_TCP,                              /**< 语音对讲 */
    MEDIASDK_RTSP_VOICE_BROADCAST_TCP,                    /**< 语音广播 */
    MEDIASDK_RTSP_STREAM_BUT
}MEDIASDK_RTSP_TYPE_E;

/**
 * @enum  tagMediaSmcStreamProtoE
 * @brief SMC流协议类型
 * @attention 
 */ 
typedef enum tagMediaSmcStreamProtoE
{ 
    MEDIASDK_SMC_STREAM_PROTO_RTP_WS        = 0,        /**< RTP over WebSocket */
    MEDIASDK_SMC_STREAM_PROTO_UDT           = 1,        /**< UDT */
    MEDIASDK_SMC_STREAM_PROTO_MAX
}MEDIASDK_SMC_STREAM_PROTO_E;

/**
 * @enum  tagMediaSmcStreamTypeE
 * @brief SMC转发流类型
 * @attention 
 */ 
typedef enum tagMediaSmcStreamTypeE
{
    MEDIASDK_SMC_STREAM_TYPE_LIVE           = 0,        /**< 实况 */
    MEDIASDK_SMC_STREAM_TYPE_PLAYBACK       = 1,        /**< 回放 */
    MEDIASDK_SMC_STREAM_TYPE_VOICE          = 2,        /**< 对讲 */
    MEDIASDK_SMC_STREAM_TYPE_MAX
}MEDIASDK_SMC_STREAM_TYPE_E;

/**
* @enum  tagMediaSmcStreamQualityE
* @brief SMC视频清晰度
* @attention
*/
typedef enum tagMediaSmcStreamQualityE
{
    MEDIASDK_SMC_STREAM_QUALITY_MAIN        = 0,        /* 主流 */
    MEDIASDK_SMC_STREAM_QUALITY_SECOND      = 1,        /* 辅流 */
    MEDIASDK_SMC_STREAM_QUALITY_THIRD       = 2,        /* 三流 */
    MEDIASDK_SMC_STREAM_QUALITY_MAX
}MEDIASDK_SMC_STREAM_QUALITY_E;

/**
* @enum tagMediaSDKFileFormateE
* @brief 录像格式类型
* @attention 无
*/
typedef enum tagMediaSDKFileFormateE
{
    MEDIASDK_FILE_FORMATE_MP4             = 0,        /**< MP4格式的媒体文件(音频+视频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_TS,                         /**< TS 格式的媒体文件(音频+视频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_FMP4,                       /**< FMP4格式的媒体文件(音频+视频) */

    MEDIASDK_FILE_FORMATE_MP4_ADD_TIME,               /**< MP4格式的媒体文件(音频+视频) ,文件名根据系统时间自动生成 */
    MEDIASDK_FILE_FORMATE_TS_ADD_TIME,                /**< TS 格式的媒体文件(音频+视频) ,文件名根据系统时间自动生成 */

    MEDIASDK_FILE_FORMATE_AUDIO_TS,                   /**< TS 格式的音频文件(仅音频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_AUDIO_MP4,                  /**< MP4格式的音频文件(仅音频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_AUDIO_TS_ADD_TIME,          /**< TS 格式的音频文件(仅音频) ,文件名根据系统时间自动生成 */
    MEDIASDK_FILE_FORMATE_AUDIO_MP4_ADD_TIME,         /**< MP4格式的音频文件(仅音频) ,文件名根据系统时间自动生成 */
    MEDIASDK_FILE_FORMATE_MP4_ADD_RCD_TIME,           /**< MP4格式的媒体文件(音频+视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_TS_ADD_RCD_TIME,            /**< TS 格式的媒体文件(音频+视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_AUDIO_MP4_ADD_RCD_TIME,     /**< MP4格式的音频文件(仅音频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_AUDIO_TS_ADD_RCD_TIME,      /**< TS 格式的音频文件(仅音频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_VIDEO_MP4_ADD_RCD_TIME,     /**< MP4格式的视频文件(仅视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_VIDEO_TS_ADD_RCD_TIME,      /**< TS 格式的音频文件(仅视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_AVI,                        /**< AVI格式的媒体文件(音频+视频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_AVI_ADD_TIME,               /**< AVI格式的媒体文件(音频+视频) ,文件名根据系统时间自动生成 */
    MEDIASDK_FILE_FORMATE_AUDIO_AVI,                  /**< AVI格式的音频文件(仅音频)，文件名由接口指定 */
    MEDIASDK_FILE_FORMATE_AUDIO_AVI_ADD_TIME,         /**< AVI格式的音频文件(仅音频) ,文件名根据系统时间自动生成 */
    MEDIASDK_FILE_FORMATE_AVI_ADD_RCD_TIME,           /**< AVI格式的媒体文件(音频+视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_AUDIO_AVI_ADD_RCD_TIME,     /**< AVI格式的音频文件(仅音频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_VIDEO_AVI_ADD_RCD_TIME,     /**< AVI格式的视频文件(仅视频)，文件名根据录像开始结束时间自动生成 */
    MEDIASDK_FILE_FORMATE_HIK_PRIVATE,                /**< 海康私有格式 */
    MEDIASDK_FILE_FORMATE_DAHUA_PRIVATE,              /**< 大华私有格式 */
    MEDIASDK_FILE_FORMATE_MOBOTIX_PRIVATE,            /**< mobotix私有格式 */
    MEDIASDK_FILE_FORMATE_SVAC_PRIVATE,               /**< svac私有格式 */
    MEDIASDK_FILE_FORMATE_MAX
}MEDIASDK_FILE_FORMATE_E;



/**
* @enum tagMediaSDKPtzMode
* @brief 鱼眼设备矫正模式枚举
* @attention 无
*/
typedef enum tagMediaSDKPtzMode
{
    MEDIASDK_ORIGINAL                 = 0,        /**< 原始图像*/
    MEDIASDK_180                      = 1,        /**< 2*180度模式*/
    MEDIASDK_360_1PTZ                 = 2,        /**< 360度+1PTZ模式*/
    MEDIASDK_360_6PTZ                 = 3,        /**< 360度+6PTZ模式*/
    MEDIASDK_FISH_3PTZ                = 4,        /**< 鱼眼+3PTZ模式*/
    MEDIASDK_FISH_MID_ON_4PTZ         = 5,        /**< 鱼眼图像在中间且显示+4PTZ模式*/
    MEDIASDK_FISH_MID_OFF_4PTZ        = 6,        /**< 鱼眼图像在中间但不显示+4PTZ模式*/
    MEDIASDK_FISH_LEFT_4PTZ           = 7,        /**< 鱼眼左边+4PTZ模式*/
    MEDIASDK_FISH_8PTZ                = 8,        /**< 鱼眼+8PTZ模式*/
    MEDIASDK_PANORAMA                 = 9,        /**< 全景模式*/
    MEDIASDK_PR_3PTZ                  = 10,       /**< 全景+3PTZ模式*/
    MEDIASDK_PR_4PTZ                  = 11,       /**< 全景+4PTZ模式*/
    MEDIASDK_PR_8PTZ                  = 12,       /**< 全景+8PTZ模式*/
    MEDIASDK_1PTZ                     = 13,       /**< 1PTZ模式，全画面只有1个PTZ*/
    MEDIASDK_NOPR_4PTZ                = 14        /**< 4PTZ，没有鱼眼或者全景图像 */
}MEDIASDK_PTZ_MODE_E;

/**
* @enum tagMedaSDKFixMode
* @brief 鱼眼设备安装模式枚举
* @attention 无
*/
typedef enum tagMedaSDKFixMode
{
    MEDIASDK_TOP_INSTALLATION         = 0,        /**< 顶部安装*/
    MEDIASDK_BOTTOM_INSTALLATION      = 1,        /**< 底部安装*/
    MEDIASDK_SIDE_INSTALLATION        = 2         /**< 侧面安装*/
}MEDIASDK_FIX_MODE_E;

/**
* @enum tagMediaSDKPtzDisplayMode
* @brief 四目全景PTZ模式
* @attention 无
*/
typedef enum tagMediaSDKPtzDisplayMode
{
    MEDIASDK_PTZ_DISPLAY_ORIGINAL     = 0,        /**< 原始图像*/
    MEDIASDK_PTZ_DISPLAY_5PTZ         = 1         /**< 全景+5ePTZ模式*/
}MEDIASDK_PTZ_DISPLAY_MODE_E;

/**
 * @struct tagMediaSDKCheckStateE
 * @brief 水印校验枚举
 * @attention 无
 */
typedef enum tagMediaSDKCheckStateE
{
    MEDIASDK_CHECK_STATE_RIGHT               = 0,        /**< 校验匹配 */
    MEDIASDK_CHECK_STATE_NOMARK              = 1,        /**< 无水印信息可校验 */
    MEDIASDK_CHECK_STATE_MISMARCH            = 2         /**< 水印校验错误 */
}MEDIASDK_CHECK_STATE_E;

/**
* @enum tagMediaSDKVideoCBType
* @brief 解码后回调类型
* @attention 无
*/
typedef enum tagMediaSDKVideoCBType
{
    MEDIASDK_VIDEO_CB_DEFAULT         = 0,        /**< 解码后数据回调（默认） */
    MEDIASDK_VIDEO_CB_PROCESSED_DATA    = 1,        /**< 鱼眼矫正后数据回调 */
    MEDIASDK_VIDEO_CB_MAX
}MEDIASDK_VIDEO_CB_TYPE_E;



/**
* @enum tagMediaSDKIVARuleType
* @brief 规则类型
* @attention 无
*/
typedef enum tagMediaSDKIVARuleType
{
    MEDIASDK_IVA_RULE_LINE            = 0,        /* 线形规则（直线、折线） */
    MEDIASDK_IVA_RULE_POLY,                       /* 多边形规则（多边形、矩形） */
    MEDIASDK_IVA_RULE_POINT,                      /* 点 */
    MEDIASDK_IVA_RULE_FULL_SCREEN,                /* 全屏模式 */
    MEDIASDK_IVA_RULE_BUT
}MEDIASDK_IVA_RULE_TYPE_E;


/**
* @enum tagMediaSDKIVATrigType
* @brief 规则触发类型
* @attention 无
*/
typedef enum tagMediaSDKIVATrigType
{
    MEDIASDK_IVA_TRIG_NONE            = 0,        /* 无触发类型 */
    MEDIASDK_IVA_TRIG_BOTH            = 1,        /* 双向触发 */
    MEDIASDK_IVA_TRIG_CW              = 2,        /* 顺时针触发 */
    MEDIASDK_IVA_TRIG_CCW             = 3,        /* 逆时针触发 */
    MEDIASDK_IVA_TRIG_IN              = 4,        /* 进入触发 */
    MEDIASDK_IVA_TRIG_OUT             = 5,        /* 离开触发 */
    MEDIASDK_IVA_TRIG_BUT
}MEDIASDK_IVA_TRIG_TYPE_E;

/**
* @enum tagMediaSDKIVAColorType
* @brief 颜色类型
* @attention 无
*/
typedef enum tagMediaSDKIVAColorType
{
    MEDIASDK_IVA_COLOR_NONE           = 0,        /* 无色 不用做图 */
    MEDIASDK_IVA_COLOR_DEEP           = 1,        /* 深色 */
    MEDIASDK_IVA_COLOR_LIGHT          = 2,        /* 浅色 */
    MEDIASDK_IVA_COLOR_WHITE          = 3,        /* 白色 */
    MEDIASDK_IVA_COLOR_BLACK          = 4,        /* 黑色 */
    MEDIASDK_IVA_COLOR_RED            = 5,        /* 红色 */
    MEDIASDK_IVA_COLOR_GREEN          = 6,        /* 绿色 */
    MEDIASDK_IVA_COLOR_BLUE           = 7,        /* 蓝色 */
    MEDIASDK_IVA_COLOR_ORANGE         = 8,        /* 橙色 */
    MEDIASDK_IVA_COLOR_PURPLE         = 9,        /* 紫色 */
    MEDIASDK_IVA_COLOR_PINK           = 10,       /* 粉色 */
    MEDIASDK_IVA_COLOR_YELLOW         = 11,       /* 黄色 */
    MEDIASDK_IVA_COLOR_BROWN          = 12,       /* 棕色 */
    MEDIASDK_IVA_COLOR_GRAY           = 13,       /* 灰色 */
    MEDIASDK_IVA_COLOR_BUT
}MEDIASDK_IVA_COLOR_E;

/**
* @enum tagMediaSDKVideoRawType
* @brief 解码后回调数据类型
* @attention 无
*/
typedef enum tagMediaSDKVideoRawType
{
    MEDIASDK_VIDEO_RAW_YUV420 = 0,           /**< YV12数据格式（默认） */
    MEDIASDK_VIDEO_RAW_NV12,
    MEDIASDK_VIDEO_RAW_DXVA2_VLD,
    MEDIASDK_VIDEO_RAW_D3D11,
    MEDIASDK_VIDEO_RAW_DRM,
    MEDIASDK_VIDEO_RAW_RGBA,
    MEDIASDK_VIDEO_RAW_MAX
}MEDIASDK_VIDEO_RAW_TYPE_E;

/**
* @enum tagMediaSDKMetaDataType
* @brief 元数据类型
* @attention 无
*/
typedef enum tagMediaSDKMetaDataType
{
    MEDIASDK_METADATA_TYPE_RECORD,           /* SD卡相关元数据 */
    MEDIASDK_METADATA_TYPE_PTZ,              /* 云台相关元数据 */
}MEDIASDK_METADATA_TYPE_E;


/**
* @enum tagMediaSDKTempeType
* @brief 热成像温度类型
* @attention
* 华氏度 = 32 + 摄氏度 × 1.8
* 摄氏度 = (华氏度 - 32) ÷ 1.8
*/
typedef enum tagMediaSDKTempeType
{
    MEDIASDK_TEMPE_TYPE_CENTIGRADE = 0,       /*摄氏温度℃*/
    MEDIASDK_TEMPE_TYPE_FAHRENHEIT = 1        /*华氏温度℉*/
}MEDIASDK_TEMPE_TYPE;

/**
* @enum tagMediaSDKTempeInfoPlace
* @brief 热成像温度信息显示位置
* @attention 无
*/
typedef enum tagMediaSDKTempeInfoPlace
{
    MEDIASDK_TEMPE_INFO_RULE_SURROUND = 0,    /* 规则周边 */
    MEDIASDK_TEMPE_INFO_WND_LEFT = 1          /* 窗口左边 */
}MEDIASDK_TEMPE_INFO_PLACE;


/**
* @enum tagMediaSDKPicFormateE
* @brief 截图格式类型
* @attention 无
*/
typedef enum tagMediaSDKPicFormateE
{
    MEDIASDK_PIC_FORMATE_BMP              = 0,        /**< BMP */
    MEDIASDK_PIC_FORMATE_JPEG             = 1,        /**< JPEG */
    MEDIASDK_PIC_FORMATE_JPEG_AND_BMP     = 2,        /**< JPEG & BMP */
    MEDIASDK_PIC_FORMATE_MAX
}MEDIASDK_PIC_FORMATE_E;


/**
* @enum tagLanguage
* @brief 指定使用语言
* @attention 无
*/
typedef enum tagMediaSDKLanguage
{
    MEDIASDK_LANGUAGE_CHINESE     = 0,    /* 中文 */
    MEDIASDK_LANGUAGE_ENGLISH,            /* 英语 */
    MEDIASDK_LANGUAGE_MAX
}MEDIASDK_LANGUAGE_E;

/**
 * @enum tagMediaSDKNetAdjustKeyE
 * @brief 网络模块可设置参数的Key值
 * @attention 
* -   卡顿检测模块 注：只有超过卡顿检测阈值的时长才会被累加进卡顿累计时间
 */
 typedef enum tagMediaSDKNetAdjustKeyE
 {
     MEDIASDK_NETADJUST_KEY_DELAYLIMIT     = 0,    /**< 延时大小上限，单位second，默认5s */
     MEDIASDK_NETADJUST_KEY_SPEEDLIMIT     = 1,    /**< 播放加速上限，默认200%，详见 #MEDIASDK_NETADJUST_SPEED_E */
     MEDIASDK_NETMOD_KEY_UNKNOWN
 }MEDIASDK_NETADJUST_KEY_E;

 /**
 * @enum tagMediSDKNetAdjustSpeed
 * @brief 网络自适应模块速度枚举
 * @attention 无
 */
 typedef enum tagMediSDKNetAdjustSpeed
 {
     MEDIASDK_NETADJUST_SPEED_1          = 0,        /**< 1倍速 */
     MEDIASDK_NETADJUST_SPEED_2          = 1,        /**< 2倍速 */
     MEDIASDK_NETADJUST_SPEED_UNKNOWN
 }MEDIASDK_NETADJUST_SPEED_E;

 /**
 * @enum tagMediaSDKNetStuckKeyE
 * @brief 网络卡顿检测模块可设置的参数
 * @attention 
 *            1) 卡顿检测阈值：持续该时长未收到视频帧，才判断作为卡顿，单位：ms；
 *            2) 严重卡顿检测阈值：当持续该时长未接收到视频帧时，会直接上报卡顿事件，单位：ms；
 *            3) 频繁卡顿检测阈值：在检测周期内，超过卡顿检测阈值的卡顿时长累计超过该值时，会上报卡顿事件，单位：ms。
 */
typedef enum tagMediaSDKNetStuckKeyE
{
    MEDIASDK_NETSTUCK_KEY_BOTTOM_LIMIT  = 0,        /**< 卡顿检测阈值 */
    MEDIASDK_NETSTUCK_KEY_TOP_LIMIT     = 1,        /**< 严重卡顿检测阈值 */
    MEDIASDK_NETSTUCK_KEY_SUM_LIMIT     = 2,        /**< 频繁卡顿检测阈值 */
    MEDIASDK_NETSTUCK_KEY_UNKNOWN
}MEDIASDK_NETSTUCK_KEY_E;

 /**
 * @enum tagMediSDKIvaShowRule
 * @brief 元数据显示类型
 * @attention 无
 */
 typedef enum tagMediSDKIvaShowRule
 {
     MEDIASDK_SHOW_RULE          = 0,        /**< 显示规则线框 */
     MEDIASDK_SHOW_RESULT_TOUTH_RULE,        /**< 显示触发规则目标框 */
     MEDIASDK_SHOW_RESULT_UNTOUTH_RULE,      /**< 显示未触发规则目标框 */
     MEDIASDK_SHOW_TRACK                     /**< 显示目标框轨迹 */

 }MEDIASDK_IVA_SHOW_RULE_E;


/**
 * @enum tagMediSDKTargetTag
 * @brief 目标标记类型
 * @attention 无
 */
 typedef enum tagMediSDKTargetTag
 {
     MEDIASDK_TARGET_VEN          = 0,        /**< 目标标记——机动车 */
     MEDIASDK_TARGET_NONVEN,                  /**< 目标标记——非机动车 */
     MEDIASDK_TARGET_PERSON,                  /**< 目标标记——行人 */
 }MEDIASDK_TARGET_TAG_E;


/**
 * @enum tagMediSDKImageQuality
 * @brief 图片质量类型
 * @attention 无
 */
 typedef enum tagMediSDKImageQuality
 {
     MEDIASDK_IMAGE_HIGH          = 0,        /**< 高品质（不压缩） */
     MEDIASDK_IMAGE_MEDIUM,                   /**< 普通品质（三次差值压缩,抗锯齿效果） */
     MEDIASDK_IMAGE_LOW,                      /**< 低品质（双线性压缩,提升渲染性能） */
     MEDIASDK_IMAGE_AUTO,                     /**< 自适应 分辨率大于1080p不压缩，小于等于1080p时使用低品质进行双线性压缩 */
 }MEDIASDK_IMAGE_QUALITY_E;

/**
 * @enum tagMediSDKRenderMode
 * @brief 渲染模式类型
 * @attention 无
 */
 typedef enum tagMediSDKRenderMode
 {
     MEDIASDK_RENDER_ADAPTIVE          = 0,        /**< 自适应 */
     MEDIASDK_RENDER_DDRAW,                        /**< ddraw模式 */
     MEDIASDK_RENDER_D3D9,                         /**< d3d9模式 */
     MEDIASDK_RENDER_D3D11,                        /**< d3d11模式 */
     MEDIASDK_RENDER_SDL,                          /**< sdl模式 */
     MEDIASDK_RENDER_OPENGL,                       /**< opengl模式 */
 }MEDIASDK_RENDER_MODE_E;

/**
 * @enum tagMediSDKGPUMode
 * @brief GPU解码类型
 * @attention 无
 */
 typedef enum tagMediSDKGPUMode
 {
     MEDIASDK_GPU_ADAPTIVE          = 0,        /**< 自适应 */
     MEDIASDK_GPU_DXVA,                         /**< dxva模式 */
     MEDIASDK_GPU_D3D11VA,                      /**< d3d11va模式 */
     MEDIASDK_GPU_VAAPI,                        /**< vaapi模式 */
     MEDIASDK_GPU_OMX,                          /**< omx模式 */
 }MEDIASDK_GPU_MODE_E;


 /**
 * @enum tagMediaSDKOsdBkType
 * @brief OSD背景类型定义
 * @attention 无
 */
 typedef enum tagMediaSDKOsdBkType
 {
     MEDIASDK_OSD_BK_NORMAL      = 0,            /**< OSD普通背景。 */
     MEDIASDK_OSD_BK_WATERMARK   = 1,            /**< OSD水印背景。 */
     MEDIASDK_OSD_BK_PADDING     = 2,            /**< OSD填充背景 */ 
 }MEDIASDK_OSD_BK_TYPE_E;


 /**
 * @enum tagMediaSDKOsdType
 * @brief OSD类型定义
 * @attention 无
 */
 typedef enum tagMediaSDKOsdType
 {
     MEDIASDK_OSD_NORMAL         = 0,          /**< 普通OSD */
     MEDIASDK_OSD_SYSTIME        = 1,          /**< 系统时间OSD */
     MEDIASDK_OSD_VODTIME        = 2,          /**< 点播回放时间OSD */
     MEDIASDK_OSD_WATERMARK      = 3,          /**< 水印OSD*/
     MEDIASDK_OSD_TIMED          = 4,          /**< 匹配时间戳的OSD */ 
     MEDIASDK_OSD_CONTINUOUS     = 5,          /**< 持续显示的OSD */
 }MEDIASDK_OSD_TYPE_E;


/**
 * @enum tagMediaSDKOsdDensity
 * @brief 满屏水印密度类型
 * @attention 满屏水印时才涉及使用密度值
 */
 typedef enum tagMediaSDKOsdDensity
 {
     MEDIASDK_DENSITY_INVALID = 0,          /**< 不涉及密度使用 */
     MEDIASDK_DENSITY_SPARSE = 1,           /**< 稀疏 */
     MEDIASDK_DENSITY_NORMAL = 2,           /**< 常规 */
     MEDIASDK_DENSITY_CROWDED = 3           /**< 密集 */
 }MEDIASDK_OSD_DENSITY_E;


 /**
 * @enum tagMediaSDKPktLostPolicy
 * @brief 码流丢包情况下策略的枚举定义
 * @attention 无
 */
 typedef enum tagMediaSDKPktLostPolicy
 {
     MEDIASDK_PKT_LOST_NO_SPLITTER = 0 ,       /**< 码流丢包情况下丢弃处理 */
     MEDIASDK_PKT_LOST_CONTINUE_SPLITTER = 1   /**< 码流丢包情况下继续拼帧 */
 }MEDIASDK_PKT_LOST_POLICY_E;

 /**
 * @enum tagMediaSDKVoiceSvcType
 * @brief 语音业务类型枚举定义
 * @attention 无
 */
 typedef enum tagMediaSDKVoiceSvcType
 {
     MEDIASDK_VOICE_BROADCAST_INPUT    = 0,    /**< 语音广播业务类型，语音发送，发送音源等待用户输入音频 */
     MEDIASDK_VOICE_BROADCAST_CAPTURE  = 1,    /**< 语音广播业务类型，语音发送，发送音源通过麦克风等设备采集 */
     MEDIASDK_VOICE_TALKBACK_INPUT     = 2,    /**< 语音对讲业务类型，语音收发，发送音源等待用户输入音频 */
     MEDIASDK_VOICE_TALKBACK_CAPTURE   = 3,    /**< 语音对讲业务类型，语音收发，发送音源通过麦克风等设备采集 */
     MEDIASDK_VOICE_BE_BROADCASTED     = 4     /**< 被语音广播业务类型，语音收 */
 }MEDIASDK_VOICE_TYPE_E;

 /**
 * @enum tagMediaSDKAudioVoiceType
 * @brief 音频编码格式枚举定义
 * @attention 无
 */
 typedef enum tagMediaSDKAudioVoiceType
 {
     MEDIASDK_AUDIO_VOICE_G711U = 0,          /**< 音频符合G.711U律标准 */
     MEDIASDK_AUDIO_VOICE_MP2 = 1,            /**< 音频符合MP2编码标准 */
     MEDIASDK_AUDIO_VOICE_AAC = 2,            /**< 音频符合AAC编码标准 */
     MEDIASDK_AUDIO_VOICE_G711A = 3,          /**< 音频符合G.711A律标准 */
     MEDIASDK_AUDIO_VOICE_PCMA = 4,           /**< 音频符合PCMA律标准 */
     MEDIASDK_AUDIO_VOICE_MAX
 }MEDIASDK_AUDIO_VOICE_TYPE_E;

 /**
 * @enum tagMediaSDKWaveInSource
 * @brief 音频输入源设置
 * @attention 无
 */
 typedef enum tagMediaSDKWaveInSource
 {
     MEDIASDK_WAVEIN_SOURCE_DEVICE = 0,        /**< 麦克风作音频输入源 */
     MEDIASDK_WAVEIN_SOURCE_FILE = 1,          /**< 音频文件作音频输入源 */
     MEDIASDK_WAVEIN_SOURCE_INPUT = 2          /**< 用户输入音频数据作为音频输入源 */
 }MEDIASDK_WAVEIN_SOURCE_E;

 /**
 * @enum tagMediaSDKTransProtocolE
 * @brief 协议类型
 * @attention 无
 */
 typedef enum tagMediaSDKTransProtocolE
 {
     MEDIASDK_TRANS_PROTOCOL_TCP      = 0,      /**< TCP协议 */
     MEDIASDK_TRANS_PROTOCOL_UDP      = 1,      /**< UDP协议 */
     MEDIASDK_TRANS_PROTOCOL_MAX
 }MEDIASDK_TRANS_PROTOCOL_E;

/**
 * @enum tagMediaSDKStreamOptTypeE
 * @brief 起流额外流参数类型
 * @attention 无
 */
typedef enum tagMediaSDKStreamOptTypeE
{
     MEDIASDK_STREAM_OPT_TYPE_KEEPALIVE_MS      = 0,      /**< 保活间隔（单位: ms，此值传0时使用默认保活时间） */
     MEDIASDK_STREAM_OPT_TYPE_BUSICODE          = 1,      /**< RTSP协商时是否使用额外的业务码(主要用于跨域下载,此字段为false时跨域只下载I帧) */
     MEDIASDK_STREAM_OPT_TYPE_THIRD_DEVICE      = 2,      /**< 标识三方设备(启流前设置) */
     MEDIASDK_STREAM_OPT_TYPE_MAX
}MEDIASDK_STREAM_OPT_TYPE_E;

/**
 * @enum tagMediaSDKInputDataTypeE
 * @brief 输入数据类型枚举
 * @attention 无
 */
typedef enum tagMediaSDKInputDataTypeE
{
    MEDIASDK_INPUTDATA_PACKET          = 0,         /**< 包数据 */
    MEDIASDK_INPUTDATA_FRAME           = 1,         /**< 帧数据 */
    MEDIASDK_INPUTDATA_MAX
}MEDIASDK_INPUTDATA_TYPE_E;

/**
* @struct tagMediaSDKRunInfo
* @brief 上报消息的结构体定义。
* @attention 无
*/
typedef struct tagMediaSDKRunInfo
{
    ULONG_32    ulErrCode;                              /**< 上报的错误码 */
    DLONG       dlReserved;                             /**< 保留参数（eg：RTSP用户特殊参数） */
    CHAR        szFilePath[MEDIASDK_PATH_MAX_LEN];      /**< 字符串参数 */
}MEDIASDK_RUN_INFO_S;

/**
* @struct tagMediaSDKSourceData
* @brief 存放拼帧前媒体数据信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKSourceData
{
    UCHAR *pucData;                     /**< 拼帧前媒体流数据缓冲区指针 */
    ULONG_32 ulDataLen;                 /**< 媒体数据长度 */
    ULONG_32 ulSourceDataType;          /**< 媒体数据类型，参见媒体类型枚举定义# MEDIASDK_SOURCE_DATA_FORMAT_E */
    ULONG_32 ulReserved2;               /**< 保留参数 */
}MEDIASDK_SOURCE_DATA_S;

/**
* @struct tagMediaSDKParseVideoFrame
* @brief 存放拼帧后视频数据的指针和长度等信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKParseVideoFrame
{
    UCHAR *pucData;                     /**< 视频数据指针 */
    ULONG_32 ulDataLen;                 /**< 视频数据长度 */
    ULONG_32 ulVideoFrameType;          /**< 视频帧类型，从# MEDIASDK_VIDEO_FRAME_TYPE_E 中取值 */
    ULONG_32 ulVideoCodeFormat;         /**< 视频编码格式，从# MEDIASDK_VIDEO_ENCODE_TYPE_E 中取值 */
    ULONG_32 ulHeight;                  /**< 视频图像高度 */
    ULONG_32 ulWidth;                   /**< 视频图像宽度 */
    DLONG dlTimeStamp;                  /**< 时间戳，rtsp流中代表该帧发送时RTP包的时间戳，ts流中代表PCR，flv流中代表该帧时间戳*90(PCR采集基准)*/
    ULONG_32 ulAbsTime;                 /**< 绝对时间(Unix时间戳)，当前仅回放流存在 */
    ULONG_32 ulFrameRate;               /**< 视频帧率 */
	USHORT usFishEyeInfoX;              /**< 鱼眼矫正中心坐标X(万分比) */
    USHORT usFishEyeInfoY;              /**< 鱼眼矫正中心坐标Y(万分比) */
    ULONG_32 ulFishEyeInfoRadius;       /**< 鱼眼矫正半径R(以X轴为参考的万分比) */
    ULONG_32 ulReserved2;               /**< 保留参数 */
}MEDIASDK_PARSE_VIDEO_FRAME_S;

/**
* @struct tagMediaSDKParseAudioFrame
* @brief 存放拼帧后音频数据的指针和长度等信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKParseAudioFrame
{
    UCHAR *pucData;                     /**< 音频数据指针 */
    ULONG_32 ulDataLen;                 /**< 音频数据长度 */
    ULONG_32 ulAudioCodeFormat;         /**< 音频编码格式，从# MEDIASDK_AUDIO_ENCODE_TYPE_E 中取值 */
    DLONG dlTimeStamp;                  /**< 时间戳，rtsp流中代表该帧发送时RTP包的时间戳，ts流中代表PCR，flv流中代表该帧时间戳*90(PCR采集基准)*/
    ULONG_32 ulChannelNum;              /**< 音频声道数目 */
    ULONG_32 ulSampleRate;              /**< 音频采样率 */    
    ULONG_32 ulReserved1;               /**< 保留参数 */
    ULONG_32 ulReserved2;               /**< 保留参数 */
}MEDIASDK_PARSE_AUDIO_FRAME_S;

/**
* @struct tagMediaSDKDecodeVideoRaw
* @brief 存放解码后图像数据的指针和长度等信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKDecodeVideoRaw
{
    UCHAR *pucData[4];                          /**< 取YV12格式数据：pucData[0]:Y 平面指针,pucData[1]:U 平面指针,pucData[2]:V 平面指针 */
    ULONG_32 ulLineSize[4];                     /**< ulLineSize[0]:Y平面每行跨距, ulLineSize[1]:U平面每行跨距, ulLineSize[2]:V平面每行跨距 */
    ULONG_32 ulPicHeight;                       /**< 图片高度 */
    ULONG_32 ulPicWidth;                        /**< 图片宽度 */
    ULONG_32 ulFrameRate;                       /**< 帧率 */
    DLONG dlRenderTime;                         /**< 用于渲染的时间数据 */
    MEDIASDK_VIDEO_RAW_TYPE_E eDataType;        /**< 解码后数据格式，对应# MEDIASDK_VIDEO_RAW_TYPE_E 枚举中的值 */
}MEDIASDK_DECODE_VIDEO_RAW_S;

/**
 * @struct tagMediaSDKRectS
 * @brief 原始图像区域结构体定义
 * @attention
 */
typedef struct tagMediaSDKRectS
{
    LONG_32 lLeft;                      /**< x轴左点值[0,10000]  */
    LONG_32 lTop;                       /**< y轴顶点值[0,10000]  */
    LONG_32 lRight;                     /**< x轴右点值[0,10000] */
    LONG_32 lBottom;                    /**< y轴底点值[0,10000] */
} MEDIASDK_RECT_S;

/**
* @struct tagMediaSDKDecodeAudioRaw
* @brief 存放解码后音频数据的指针和长度等信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKDecodeAudioRaw
{
    CHAR *pcData;                       /**< 音频数据指针 */
    ULONG_32 ulDataLen;                 /**< 音频数据长度 */
    ULONG_32 ulWaveFormat;              /**< 解码后音频格式，对应# MEDIASDK_WAVE_FORMAT_INFO_E 枚举中的值 */
}MEDIASDK_DECODE_AUDIO_RAW_S;

/**
* @struct tagMediaSDKDrawInfo
* @brief 存放画图回调信息的结构体定义
* @attention 无
*/
typedef struct tagMediaSDKDrawInfo
{
    VOID* hWnd;                       /**< 窗口句柄 */
    VOID* hDc;                        /**< 画图设备上下文 */
}MEDIASDK_DRAW_INFO_S;

/**
 * @struct tagMediaSDKRtspRealPlayS
 * @brief rtsp实况启流结构体
 * @attention 无
 */
typedef struct tagMediaSDKRtspRealPlayS
{
    MEDIASDK_RTSP_TYPE_E enRtspType;                        /* 流协议类型 */
    MEDIASDK_RTSP_EXECUTE_TYPE_E enRtspExecuteType;         /* RTSP执行模式 */
    MEDIASDK_RTSP_STREAM_MODE_E enStreamMode;               /* RTSP流模式 */
    CHAR *pcURL;                                            /* 流URI,格式:rtsp://username:password@ip:port/xxxxxxxxxxxx*/
    CHAR *pcServerIp;                                       /* 服务器IP (跨Net时(包括P2P)传入，传入后优先用此IP进行交互)*/
    USHORT usServerPort;                                    /* 服务器端口(跨Net时(包括P2P)传入，传入后优先用此Port进行交互) */
    USHORT usRecvPort;                                      /* 接收端口（UDP收流/TCP带外收流使用） */
    BOOL_T bKeepAlive;                                      /* 是否需要保活 */
    VOID* pWnd;                                             /* 窗口句柄 如果不需要下层显示可传NULL*/
    LONG_REAL lUserParam;
}MEDIASDK_RTSP_REALPLAY_S;

/**
 * @struct tagMediaSDKRtspPlayBackS
 * @brief rtsp回放启流结构体
 * @attention 无
 */
typedef struct tagMediaSDKRtspPlayBackS
{
    MEDIASDK_RTSP_TYPE_E enRtspType;                        /* 流协议类型 */
    MEDIASDK_RTSP_EXECUTE_TYPE_E enRtspExecuteType;         /* RTSP执行模式 */
    MEDIASDK_RTSP_STREAM_MODE_E enStreamMode;               /* RTSP流模式 */
    CHAR *pcServerIp;                                       /* 服务器IP (跨Net时(包括P2P)传入，传入后优先用此IP进行交互)*/
    USHORT usServerPort;                                    /* 服务器端口(跨Net时(包括P2P)传入，传入后优先用此Port进行交互) */
    USHORT usRecvPort;                                      /* 接收端口（UDP收流/TCP带外收流使用） */
    BOOL_T bKeepAlive;                                      /* 是否需要保活 */
    MEDIASDK_PLAY_SCALE_E eSpeed;                           /* 播放速度 */
    TIME_T tiStartPlayTime;                                 /* 开始时间（单位: s） */
    TIME_T tiEndPlayTime;                                   /* 结束时间（单位: s） */
    CHAR *pcURL;                                            /* 流URI,格式:rtsp://username:password@ip:port/xxxxxxxxxxxx*/
    VOID* pWnd;                                             /* 窗口句柄 如果不需要下层显示可传NULL*/
    LONG_REAL lUserParam;
}MEDIASDK_RTSP_PLAYBACK_S;

/**
 * @struct tagMediaSDKSmcRealPlayS
 * @brief  smc实况启流结构体
 * @attention 无
 */
typedef struct tagMediaSDKSmcRealPlayS
{
    CHAR                            *pcSerialNo;        /* 设备序列号 */
    ULONG                           ulChannelNo;        /* 设备通道号 */
    MEDIASDK_SMC_STREAM_PROTO_E     eStreamProto;       /* 协议类型 */
    MEDIASDK_SMC_STREAM_QUALITY_E   eStreamQuality;     /* 视频清晰度 */
    LONG_REAL                       lUserParam;         /* 用户参数 */
}MEDIASDK_SMC_REALPLAY_S;

/**
 * @struct tagMediaSDKSmcPlayBackS
 * @brief  smc回放启流结构体
 * @attention 无
 */
typedef struct tagMediaSDKSmcPlayBackS
{
    CHAR                            *pcSerialNo;        /* 设备序列号 */
    ULONG                           ulChannelNo;        /* 设备通道号 */
    MEDIASDK_SMC_STREAM_PROTO_E     eStreamProto;       /* 协议类型 */
    MEDIASDK_SMC_STREAM_QUALITY_E   eStreamQuality;     /* 视频清晰度 */
    TIME_T                          tiStartPlayTime;    /* 回放开始时间 */
    TIME_T                          tiEndPlayTime;      /* 回放结束时间 */
    LONG_REAL                       lUserParam;         /* 用户参数 */
}MEDIASDK_SMC_PLAYBACK_S;

/**
 * @struct tagMediaSDKSmcVoiceS
 * @brief  smc对讲结构体
 * @attention 无
 */
typedef struct tagMediaSDKSmcVoiceS
{
    CHAR                            *pcSerialNo;        /* 设备序列号 */
    ULONG                           ulChannelNo;        /* 设备通道号 */
    MEDIASDK_SMC_STREAM_PROTO_E     eStreamProto;       /* 协议类型 */
    MEDIASDK_AUDIO_VOICE_TYPE_E     enVoiceType;        /* 音频编码格式 */
    ULONG_32                        ulSampleRate;       /* 采样率 */
    LONG_REAL                       lUserParam;         /* 用户参数 */
}MEDIASDK_SMC_VOICE_S;

/**
 * @enum tagMediaSDKOsdOverInfo
 * @brief OSD信息的结构体
 * @attention
 */
 typedef struct tagMediaSDKOsdOverInfo
 {
     UCHAR *pucBuf;                              /**< 数据指针 */
     ULONG_32 ulBufLen;                          /**< 数据长度 */
     ULONG_32 ulColor;                           /**< 颜色 */
     ULONG_32 ulPosX;                            /**< X轴百分比[0 ~ 100] */
     ULONG_32 ulPosY;                            /**< Y轴百分比[0 ~ 100] */
     ULONG_32 ulSize;                            /**< 字体大小 */
     ULONG_32 ulOsdType;                         /**< 具体定义见#MEDIASDK_OSD_TYPE_E  */
     ULONG_32 ulOsdBkType;                       /**< 具体定义见#MEDIASDK_OSD_BK_TYPE_E  */
     UCHAR ucOsdJudge[MEDIASDK_LEN_64];          /**< 特殊用途，一般不用传入 */
     UCHAR ucReserver[MEDIASDK_LEN_128];         /**< 保留字段，后续扩展的时候从该内存块中分配（当前SDK用户传入的该块数据必须为全0，便于后续接口功能扩展和SDK升级） */
 }MEDIASDK_OSD_OVER_INFO_S;

 
 /**
 * @enum tagMediaSDKWaterMarkInfo
 * @brief 全画面水印信息结构体
 * @attention
 */
 typedef struct tagMediaSDKWaterMarkInfo
 {
     MEDIASDK_OSD_OVER_INFO_S stWaterMarkBasicInfo;

     ULONG_32 ulRotateAngle;        /**< 字体旋转角度[0 ~ 180] */
     ULONG_32 ulTransparency;       /**< 字体透明度百分比[0 ~ 100] 100表示全透明 */
     ULONG_32 ulOsdDensity;         /**< 具体定义见#MEDIASDK_OSD_DENSITY_E  */
     BOOL_T   bShowSystemTime;      /**< 是否显示当前系统时间  */
 }MEDIASDK_WATER_MARK_INFO_S;

/**
 * @struct tagMediaSDKRtspDownloadS
 * @brief rtsp下载结构体
 * @attention 无
 */
typedef struct tagMediaSDKRtspDownloadS
{
    MEDIASDK_RTSP_TYPE_E enRtspType;                        /* 流协议类型 */
    MEDIASDK_RTSP_EXECUTE_TYPE_E enRtspExecuteType;         /* RTSP执行模式 */
    CHAR *pcURL;                                            /* 流URI,格式:rtsp://username:password@ip:port/xxxxxxxxxxxx*/
    CHAR *pcServerIp;                                       /* 服务器IP(跨Net时(包括P2P)传入，传入后优先用此IP进行交互) */
    USHORT usServerPort;                                    /* 服务器端口(跨Net时(包括P2P)传入，传入后优先用此Port进行交互) */
    USHORT usRecvPort;                                      /* 接收端口（UDP收流/TCP带外收流使用） */
    MEDIASDK_PLAY_SCALE_E eSpeed;                           /* 播放速度 */
    TIME_T tiStartPlayTime;                                 /* 开始时间（单位: s） */
    TIME_T tiEndPlayTime;                                   /* 结束时间（单位: s） */
    BOOL_T bKeepAlive;                                      /* 是否需要保活 */
    CHAR *pcFileName;                                       /* 录像文件存放路径，包括文件名（不带后缀名） */
    ULONG_32 ulMediaFileFormat;                             /* 录像文件的格式，需从# MEDIASDK_FILE_FORMATE_E 枚举中取值 */
    BOOL_T bDecryptInput;                                   /* 对输入数据进行解扰标识 */
    BOOL_T bEncryptOutput;                                  /* 对输出数据进行加密标识 */
    LONG_REAL lUserParam;
    ULONG_32 ulWaterMarkNum;                                /* 录屏下载时可能带有水印,如果不需要水印,则此值为0 */
    MEDIASDK_WATER_MARK_INFO_S *pstWaterMark;
}MEDIASDK_RTSP_DOWNLOAD_S;

/**
 * @struct tagMediaSDKRtspVoiceS
 * @brief rtsp对讲结构体
 * @attention 
 */
typedef struct tagMediaSDKRtspVoiceS
{
    MEDIASDK_RTSP_TYPE_E        enRtspType;                 /* 流协议类型 */
    CHAR*                       pcURL;                      /* 流URI,格式:rtsp://username:password@ip:port/xxxxxxxxxxxx*/
    CHAR*                       pcServerIp;                 /* 服务器IP(跨Net时(包括P2P)传入，传入后优先用此IP进行交互) */
    USHORT                      usServerPort;               /* 服务器端口(跨Net时(包括P2P)传入，传入后优先用此Port进行交互) */
    BOOL_T                      bKeepAlive;                 /* 是否需要保活 */
    MEDIASDK_AUDIO_VOICE_TYPE_E enVoiceType;                /* 音频编码格式 */
    ULONG_32                    ulSampleRate;               /* 采样率 */
    /* 音频采集的工作模式 */
    MEDIASDK_WAVEIN_SOURCE_E    enWaveInSource;             /* #MEDIASDK_WAVEIN_SOURCE_E枚举中取值 */
}MEDIASDK_RTSP_VOICE_S;

/**
 * @struct tagMediaSDKRtmpRealPlayS
 * @brief rtmp实况结构体
 * @attention 
 */
typedef struct tagMediaSDKRtmpRealPlayS
{
    CHAR        *pcURL;                                 /* 流URI,格式:rtsp://username:password@ip:port/xxxxxxxxxxxx*/

//待补充

}MEDIASDK_RTMP_REALPLAY_S;

/**
 * @struct tagMediaSDKHlsPlayBackS
 * @brief hls回放结构体
 * @attention 1) 如果tiSeekTime缺省，则从已设置的m3u8对应的第一个录像段开始播放；
 *            2) 如何tiEndTime缺省，则从会一直播放到已设置的m3u8对应的最后一个录像段。
 */
typedef struct tagMediaSDKHlsPlayBackS
{
    CHAR *pcM3u8;                                       /* 设置要播放的录像段的m3u8信息，填NULL为缺省，延后设置 */
    TIME_T tiSeekTime;                                  /* 要播放的时间点（单位：s），填0为缺省 */
    TIME_T tiEndTime;                                   /* 播放终止时间点（单位：s），填0为缺省 */
}MEDIASDK_HLS_PLAYBACK_S;


/**
 * @struct tagMediaSDKMediaFileS
 * @brief 媒体文件结构体
 * @attention 
 */
typedef struct tagMediaSDKMediaFileS
{

    CHAR *pcFilename;                                        /* 媒体文件名，建议长度不超过256字节*/
    VOID *pWnd;                                              /* 窗口句柄 如果不需要下层显示可传NULL*/

}MEDIASDK_MEDIAFILE_S;

/**
* @enum tagMediaSDKProtocol
* @brief 网络协议
* @attention 无
*/
typedef enum tagMediaSDKProtocol
{
    MEDIASDK_PROTOCOL_UDP = 0,                   /**< UDP协议 */
    MEDIASDK_PROTOCOL_TCP = 1,                   /**< TCP协议Client端*/
    MEDIASDK_PROTOCOL_TCP_SERVER = 2,            /**< TCP协议Server端*/
    MEDIASDK_PROTOCOL_TCP_SERVERANDUDP = 3,      /**< TCP协议Server端及UDP端 */
    MEDIASDK_PROTOCOL_WEBSOCKET_CLIENT = 4       /**< WebSocket协议Client端*/
}MEDIASDK_PROTOCOL_E;

/**
* @enum tagMediaSDKNatMode
* @brief NAT方式的枚举类型
* @attention 无
*/
typedef enum tagMediaSDKNatMode
{
    MEDIASDK_NAT_NONE = 0,                     /**< 无NAT的方式 */
    MEDIASDK_NAT_IN   = 1,                     /**< NAT IN方式 */
    MEDIASDK_NAT_OUT  = 2                      /**< NAT OUT方式 */
}MEDIASDK_NAT_MODE_E;

/**
* @enum tagMediaSDKPktQuePolicyE
* @brief 拼帧缓存/包缓存满时控制策略
* @attention 无
*/
typedef enum tagMediaSDKPktQuePolicyE
{
    MEDIASDK_PKT_QUE_POLICY_DROP           = 0,         /**< 丢弃 */
    MEDIASDK_PKT_QUE_POLICY_BLOCK          = 1,         /**< 阻塞 */
    MEDIASDK_PKT_QUE_POLICY_MAX
}MEDIASDK_PKT_QUE_POLICY_E;

/**
* @enum tagMediaSDKSnatchSaveTypeE
* @brief 截图时保存图片的类型
* @attention 无
*/
typedef enum tagMediaSDKSnatchSaveTypeE
{
    MEDIASDK_SNATCH_SAVE_NORMAL   = 0,         /**< 保存普通图片 */
    MEDIASDK_SNATCH_SAVE_CORRECTED = 1,        /**< 保存校正后的图片(鱼眼或ptz) */
}MEDIASDK_SNATCH_SAVE_TYPE_E;

 /**
 * @struct tagMediaSDKKdmInfo
 * @brief KDM加扰信息
 */
typedef struct tagMediaSDKScrambleInfo
{
    CHAR  szScrambleMode[MEDIASDK_LEN_16];          /* 加解密模式 */
    CHAR  szScrambleKey[MEDIASDK_LEN_512];          /* 加解密Key信息 */
    CHAR  szCameraCode[MEDIASDK_LEN_256];           /* 相机编码 */
}MEDIASDK_SCRAMBLE_INFO_S;

/**
 * @struct tagMediaSDKNetRealPlayS
 * @brief NET实况结构体
 * @attention 
 */
typedef struct tagMediaSDKNetRealPlayS
{
    ULONG_32     ulProtocol;                         /* NET拉流协议 需从#MEDIASDK_PROTOCOL_E枚举中取值 */
    CHAR         *pcLocalIP;                         /* 本地IP地址 */
    USHORT       usLocalPort;                        /* 本地监听端口 */
    CHAR         *pcRemoteIP;                        /* 可为空,远端IP地址 */
    USHORT       usRemotePort;                       /* 可为空,远端端口 */
    CHAR         *pcFeedbackIP;                      /* 可为空,抗丢包通知报文的接收端地址 */
    USHORT       usFeedbackPort;                     /* 可为空,抗丢包通知报文的接收端监听端口 */
    ULONG_32     ulNatMode;                          /* 可传MEDIASDK_NAT_NONE,NAT类型，需从#MEDIASDK_NAT_MODE_E枚举中取值。 */
    CHAR         *pcNatSessionStr;                   /* 可为空,引流字符串 */
    /* 幻影信息 */
    MEDIASDK_SCRAMBLE_INFO_S    stScrambleInfo;     /* 结构体内部字符串可为NULL也可为空字符串 */
}MEDIASDK_NET_REALPLAY_S;

/**
* @struct tagMediaSDKWaveInFileInfo
* @brief 设置音频文件作为输入源时，需要设置的额外参数信息。
* @attention 无
*/
typedef struct tagMediaSDKWaveInFileInfo
{
    CHAR szWaveFileName[MEDIASDK_LEN_256];      /**< 音频文件名称（包括路径信息） */
    BOOL_T bCyclePlay;                          /**< 是否循环播放音频文件 */
}MEDIASDK_WAVEIN_FILE_INFO_S;

/**
 * @struct tagMediaSDKNetVoiceS
 * @brief NET对讲结构体
 * @attention 
 */
typedef struct tagMediaSDKNetVoiceS
{
    ULONG_32     ulVoiceSvcType;                     /* 对讲类型 需从#MEDIASDK_VOICE_TYPE_E枚举中取值 */
    ULONG_32     ulProtocol;                         /* NET拉流协议 需从#MEDIASDK_PROTOCOL_E枚举中取值 */
    CHAR         *pcLocalIP;                         /* 本地IP地址 */
    USHORT       usLocalPort;                        /* 本地监听端口 */
    CHAR         *pcRemoteIP;                        /* 可为空,远端IP地址 */
    USHORT       usRemotePort;                       /* 可为空,远端端口 */
    ULONG_32     ulAudioEncType;                     /* NET拉流协议 需从#MEDIASDK_AUDIO_VOICE_TYPE_E枚举中取值 */
    ULONG_32     ulSampleRate;                       /* 采样率 */
    ULONG_32     ulChannels;                         /* 采样声道数目 */
    ULONG_32     ulNatMode;                          /* 可传MEDIASDK_NAT_NONE,NAT类型，需从#MEDIASDK_NAT_MODE_E枚举中取值。 */
    CHAR         *pcNatSessionStr;                   /* 可为空,引流字符串 */

    /* 音频采集的工作模式 */
    ULONG_32     ulWaveInSource;                    /* #MEDIASDK_WAVEIN_SOURCE_E枚举中取值 */
    VOID         *pParam;                           /* MEDIASDK_WAVEIN_SOURCE_FILE时传结构体MEDIASDK_WAVEIN_FILE_INFO_S的指针，其他场景传NULL */
}MEDIASDK_NET_VOICE_S;

/**
 * @struct tagMediaSDKStreamInfoS
 * @brief 媒体流信息结构体
 * @attention 
 */
typedef struct tagMediaSDKStreamInfoS
{

    LONG_32 lBitRate;                                          /* 码率*/
    LONG_32 lRecvPktNum;                                       /* 总包数*/
    LONG_32 lLostPktNum;                                       /* 丢包数*/
    LONG_32 lFrameRate;                                        /* 帧率*/
    LONG_32 lWidth;                                            /* 视频宽*/
    LONG_32 lHeight;                                           /* 视频高*/
    ULONG_32 ulVideoEncodeType;                                /* 视频编码格式，参考# MEDIASDK_VIDEO_ENCODE_TYPE_E*/
    ULONG_32 ulAudioEncodeType;                                /* 音频编码格式，参考# MEDIASDK_AUDIO_ENCODE_TYPE_E*/

}MEDIASDK_STREAMINFO_S;

/**
* @struct tagMediaSDKCheckMarkInfo
* @brief 不符合水印校验信息
* @attention
*/
typedef struct tagMediaSDKCheckMarkInfo
{
    ULONG_32 ulPlayTime;               /**< 该记录对应的时间点 */
    ULONG_32 ulState;                  /**< 水印校验，参考# PLAYER_CHECK_STATE_E */
}MEDIASDK_CHECK_MARK_INFO;

/**
* @struct tagMediaSDKMarkInfo
* @brief 水印校验结果信息
* @attention
* @note
* -    1、ulUnmarkCount记录不符合校验数量，如果为零则表示校验成功，pstUnmarkTimeList的数量也由ulUnmarkCount指定
* -    2、1个录像最多显示1000条水印校验不成的记录，即ulUnmarkCount值不大于1000
* -    3、不成功分为：没有水印信息，或校验错误，后者较为严重，建议提示具体时间
*/
typedef struct tagMediaSDKMarkInfo
{
    CHAR szMarkInfo[MEDIASDK_WATRE_MARK_MAX_LEN];            /**< 水印信息值 */
    ULONG ulUnmarkCount;                            /**< 不符合水印校验数量 */
    MEDIASDK_CHECK_MARK_INFO *pastUnmarkTimeList;     /**< 不符合水印列表 */
    BOOL_T bHaveWaterMark;                          /**< 是否有水印 */
}MEDIASDK_MARK_INFO_S;

/*********************** Player 元数据画图相关********************************/
/**
 * @struct tagMediaSDKPointS
 * @brief 点坐标
 * @attention
 */
typedef struct tagMediaSDKPointS
{
    USHORT usX;                     /**< 横坐标，万分比[0,10000] */
    USHORT usY;                     /**< 纵坐标，万分比[0,10000] */
}MEDIASDK_POINT_S;



/**
* @struct tagMediaSDKRsaInfo
* @brief RSA加解密信息
* @attention 无
*/
typedef struct tagMediaSDKRsaInfo
{
    ULONG_32 ulBit;                 /**< 密钥bit值 */
    ULONG_32 ulPub;                 /**< 公钥值 */
    CHAR cN[260];                   /**< n */
    CHAR cPri[260];                 /**< 私钥值 */
    CHAR cP[132];                   /**< p */
    CHAR cQ[132];                   /**< q */
    CHAR cDP[132];                  /**< dp */
    CHAR cDQ[132];                  /**< dq */
    CHAR cQP[132];                  /**< qp */
}MEDIASDK_RSA_INFO_S;

/**
 * @struct tagMediaSDKPosInfoS
 * @brief Pos配置信息结构体
 * @attention 无
 */
typedef struct tagMediaSDKPosInfoS
{
    MEDIASDK_RECT_S stArea;           /* 显示区域（万分比）*/
    ULONG_32 ulFontSize;            /* 字体大小 */
    ULONG_32 ulPosColor;            /* 字体颜色 */
    ULONG_32 ullKeepAliveTime;      /* 保活时间 */
    VOID *pParm;                    /* 预留参数 */
}MEDIASDK_POS_INFO_S;

/**
 * @struct tagMediaSDKIVARuleHead
 * @brief V2版本规则信息头
 * @attention 无
 */
typedef struct tagMediaSDKIVARuleHead
{
    ULONG_32 ulChannel;             /* 通道号 */
    BOOL_T bEnable;                 /* 使能状态 */
    USHORT usRuleNum;               /* 规则数量 */
    USHORT usDisplayHoldTime;       /* 规则持续显示时间 ms */
}MEDIASDK_IVA_RULE_HEAD_S;

/**
 * @struct tagMediaSDKIVAResultHead
 * @brief V2版本结果信息头
 * @attention 无
 */
typedef struct tagMediaSDKIVAResultHead
{
    ULONG_32 ulChannel;             /* 通道号 */
    BOOL_T bEnable;                 /* 使能状态 */
    USHORT usTargetNum;             /* 目标数量 */
    USHORT usDisplayHoldTime;       /* 持续显示时间 ms */
}MEDIASDK_IVA_RESULT_HEAD_S;


/**
 * @struct tagMediaSDKIVACoord
 * @brief 坐标点结构体
 * @attention 无
 */
typedef struct tagMediaSDKIVACoord
{
    USHORT usX;                     /* 坐标X  0-10000 万分比 */
    USHORT usY;                     /* 坐标Y  0-10000 万分比 */
}MEDIASDK_IVA_COORD_S;


/**
 * @struct tagMediaSDKIVARuleV2S
 * @brief V2版本规则信息
 * @attention 无
 */
typedef struct tagMediaSDKIVARuleV2S
{
    ULONG_32 ulRuleID;                                  /* 规则ID */
    ULONG_32 bTriggered;                                /* 规则是否被触发 */
    MEDIASDK_IVA_RULE_TYPE_E enRuleType;                  /* 规则类型 */
    MEDIASDK_IVA_TRIG_TYPE_E enTrigType;                  /* 规则触发类型 */
    MEDIASDK_IVA_COORD_S astCoord[MEDIASDK_IVA_REPORT_COORD_NUM];  /* 规则坐标点*/
    ULONG_32 ulCoordNum;                                /* 规则坐标点数量 */
    MEDIASDK_IVA_COLOR_E enLineColor;                     /* 规则线条颜色 */
    MEDIASDK_IVA_COLOR_E enFillColor;                     /* 规则填充颜色 */
    ULONG_32 ulLineAlpha;                               /* 线条透明度 0-100 */
    ULONG_32 ulFillAlpha;                               /* 填充透明度 0-100 */
    ULONG_32 ulAttachLen;                               /* 附属信息长度 保留 */
    ULONG_32 ulAttachType;                              /* 附属信息类型 保留 */
}MEDIASDK_IVA_RULE_V2_S;

/**
 * @struct tagMediaSDKIVAResultV2S
 * @brief V2版本结果信息
 * @attention 无
 */
typedef struct tagMediaSDKIVAResultV2S
{
    ULONG_32 ulTargetID;                                /* 目标ID*/
    ULONG_32 ulTrigRule;                                /* 触发规则信息 1bit代表1条规则 */
    MEDIASDK_IVA_COORD_S stStart;                         /* 目标起始坐标，起始坐标和结束坐标都为（0,0）时，表示跟踪结束*/
    MEDIASDK_IVA_COORD_S stEnd;                           /* 目标结束坐标 */
    MEDIASDK_IVA_COLOR_E enLineColor;                     /* 线条颜色 */
    ULONG_32 ulLineAlpha;                               /* 线条透明度 0-100 */
    ULONG_32 ulTrailHoldTime;                           /* 历史轨迹点显示时间 ms */
    ULONG_32 ulAttachLen;                               /* 附属信息长度 保留 */
    ULONG_32 ulAttachType;                              /* 附属信息类型 -0x01（全结构属性信息） */
    CHAR szMsgData[0];                                  /* 附属属性信息-新增定义 */
}MEDIASDK_IVA_RESULT_V2_S;


/**
 * @struct tagMediaSDKThermalParam
 * @brief 热成像参数结构体
 * @attention 
 */
typedef struct tagMediaSDKThermalParam
{

    BOOL_T bThermalDetect;                               /* 火点检测使能参数（BOOL_TRUE使能，BOOL_FALSE关闭）*/
    BOOL_T bTempeDetect;                                 /* 测温检测使能参数（BOOL_TRUE使能，BOOL_FALSE关闭）*/
    BOOL_T bExtremeTrace;                                /* 冷热点追踪使能参数（BOOL_TRUE使能，BOOL_FALSE关闭）*/
    BOOL_T bShowTempeInfo;                               /* 测温信息使能参数（BOOL_TRUE使能，BOOL_FALSE关闭）*/
    ULONG_32 ulTempeInfoPlace;                           /* 测温信息位置参数（说明参考 # MEDIASDK_TEMPE_INFO_PLACE）*/
    ULONG_32 ulReserved1;                                /* 预留参数1（供扩展）*/
    ULONG_32 ulReserved2;                                /* 预留参数2（供扩展）*/

}MEDIASDK_THERMAL_PARAM_S;



 /**
 * @struct tagMediaSDKGpuAbleState
 * @brief GPU使能状态结构体
 * @attention 无
 */
typedef struct tagMediaSDKGpuAbleState
{
    BOOL_T bGpuAbleH264;            /**< GPU使能-H.264*/
    BOOL_T bGpuAbleH265;            /**< GPU使能-H.265 */
    BOOL_T bGpuAbleDXVA;            /**< GPU使能-DXVA*/
    BOOL_T bGpuAbleD3D11VA;         /**< GPU使能-D3D11VA */
    BOOL_T bReserved1;              /**< 预留类型1 */
    BOOL_T bReserved2;              /**< 预留类型2 */
    BOOL_T bReserved3;              /**< 预留类型3 */
}MEDIASDK_GPU_ABLE_STATE_S;



/**
 * @struct tagMediaSDKVideoAdjust
 * @brief 影像设置参数
 * @attention 
 */
typedef struct tagMediaSDKVideoAdjust
{
    ULONG_32 ulContrast;                               /* 对比度取值范围为[0--199]，默认值为100 */
    ULONG_32 ulLuminance;                              /* 亮度取值范围为[0--199]，默认值为100 */
    ULONG_32 ulHue;                                    /* 色调取值范围为[0--359]，默认值为0 */
    ULONG_32 ulSaturation;                             /* 饱和度取值范围为[0--359]，默认值为100 */
    ULONG_32 ulGamma;                                  /* 伽玛取值范围为[0--99]，默认值为10 */

}MEDIASDK_VIDEO_ADJUST_S;

#pragma  pack(4)

 /**
 * @struct tagMediaSDKTripCamera
 * @brief 拌线任务数据
 */
 typedef struct tagMediaSDKTripCamera
 {
     CHAR szCamName[MEDIASDK_LEN_64];  /**  */
     CHAR szCamCode[MEDIASDK_CODE_LEN];
     DULONG lTimeStamp;
     ULONG_32 ulCamPersonCountIn;
     ULONG_32 ulCamPersonCountOut;
     ULONG_32 ulCamPersonCountLost;
 }MEDIASDK_TRIP_CAMERA_S;


 /**
 * @struct tagMediaSDKAreaCamera
 * @brief 区域任务数据
 */
 typedef struct tagMediaSDKAreaCamera
 {
     CHAR szCamName[MEDIASDK_LEN_64];
     CHAR szCamCode[MEDIASDK_CODE_LEN];
     DULONG lTimeStamp;
     DULONG lCamPersonCount;
 }MEDIASDK_ARERA_CAMERA_S;

 /**
 * @struct tagMediaSDKExecption
 * @brief 异常数据
 */
 typedef struct tagMediaSDKExecption
 {
     CHAR szCamName[MEDIASDK_LEN_64];
     CHAR szCamCode[MEDIASDK_CODE_LEN];
     ULONG_32 ulExceptionCode;
 }MEDIASDK_EXCEPTION_S;

 /**
 * @struct tagMediaSDKDpcInfo
 * @brief 实时推送数据
 */
 typedef struct tagMediaSDKDpcInfo
 {
     DLONG lNowTripPersonsIn;
     DLONG lNowTripPersonsOut;
     DLONG lNowTripPersonsTotal;
     DLONG lNowAreaPersonsTotal;
     CHAR szServerTime[MEDIASDK_TIME_LEN];
     CHAR szTaskID[MEDIASDK_CODE_LEN];
     ULONG_32 ulTaskType;
     ULONG_32 ulTripDataInLen;
     MEDIASDK_TRIP_CAMERA_S szTripDataIn[MEDIASDK_LEN_256];
     ULONG_32 ulTripDataOutLen;
     MEDIASDK_TRIP_CAMERA_S szTripDataOut[MEDIASDK_LEN_256];
     ULONG_32 ulAreaDataInLen;
     MEDIASDK_ARERA_CAMERA_S szAreaDataIn[MEDIASDK_LEN_256];
     ULONG_32 ulExFlag;
     ULONG_32 ulExceptionDataLen;
     MEDIASDK_EXCEPTION_S stExceptionData[MEDIASDK_LEN_256];
 }MEDIASDK_DPC_INFO_S;
#pragma pack()

 /**
 * @struct tagMediaSDKKdmInfo
 * @brief KDM信息
 */
 typedef struct tagMediaSDKKdmInfo
 {
     CHAR *pcDevCode;
     CHAR *pcDevIP;
     CHAR *pcKDMAddr;
     ULONG_32 ulKDMPort;
 }MEDIASDK_KDM_INFO_S;

/**
 * @struct  tagRecordInitParamS
 * @brief   录像初始化参数
 */
typedef struct tagRecordInitParamS
{
    CHAR szFileName[MEDIASDK_LEN_256];
    ULONG_32 ulMediaFileFormat;
    BOOL_T bDecryptInput;
    BOOL_T bEncryptOutput;
    BOOL_T bRecordReEncode;         /* 录像时重新编码 */
}MEDIASDK_RECORD_INIT_PARAM_S;

/**
 * @struct  tagMediaSDKTvParamS
 * @brief   电视墙参数
 */
typedef struct tagMediaSDKTvParamS
{
    CHAR        szTvIp[MEDIASDK_LEN_64];            /* 电视墙IP */
    USHORT      usTvPort;                           /* 电视墙端口 */
    MEDIASDK_TRANS_PROTOCOL_E   eProtocol;          /* 电视墙收流协议 */
}MEDIASDK_TV_PARAM_S;

/**
 * @struct  tagMediaSDKToTvInfoS
 * @brief   回放上墙信息
 */
typedef struct tagMediaSDKToTvInfoS
{
    CHAR                        szServerIp[MEDIASDK_LEN_64];    /* 服务器响应IP */
    USHORT                      usServerPort;                   /* 服务器响应端口 */
    MEDIASDK_TRANS_PROTOCOL_E   eProtocol;                      /* 服务器响应协议TCP/UDP */
}MEDIASDK_TO_TV_INFO_S;

/**
 * @struct  tagMediaSDKRationalS
 * @brief   分数
 */
typedef struct tagMediaSDKRationalS
{
    LONG_32     lNum;       /* 分子 */
    LONG_32     lDen;       /* 分母 */
}MEDIASDK_RATIONAL_S;

/**
* @struct tagUrtspAuthInfo
 * @brief 鉴权信息
* @attention 无
*/
typedef struct tagMediaSDKAuthInfo
{
   CHAR szNonce[64];                            /* Nonce值*/
   CHAR szRealm[32];                            /* Realm值*/
   CHAR szAlgorithm[32];                        /* 鉴权算法*/
}MEDIASDK_AUTH_INFO_S;

/**
 * @enum tagMediaSDKTypeE
 * @brief 媒体类型枚举
 * @attention 无
*/
typedef enum tagMediaSDKTypeE
{
    MEDIASDK_TYPE_NONE = 0,
    MEDIASDK_TYPE_VIDEO,
    MEDIASDK_TYPE_AUDIO,
    MEDIASDK_TYPE_PRIV,
    MEDIASDK_TYPE_BINARY,
}MEDIASDK_TYPE_E;

/**
 * @struct tagMeidaSDKFrameDataS
 * @brief 传入帧数据结构体
 * @attention 无
 * @note
 * -    1）输入视频帧时，采样率类型枚举可以传入 MEDIASDK_WAVE_FORMAT_UNKNOWN 代表未知类型
 */
typedef struct tagMeidaSDKFrameDataS
{
    UCHAR                               *pucData;                          /**< 缓存区地址 */
    LONG_32                             lDataLen;                          /**< 缓存区数据长度 */
    MEDIASDK_TYPE_E                     eMediaDataType;                    /**< 媒体流类型 */
    union
    {
        MEDIASDK_AUDIO_ENCODE_TYPE_E    enAudioEncodeType;                 /**< 视频枚举变量*/
        MEDIASDK_VIDEO_ENCODE_TYPE_E    enVideoEncodeType;                 /**< 音频枚举变量*/
    };
    MEDIASDK_WAVE_FORMAT_INFO_E         enSample;                          /**< 采样率类型枚举 ，仅在输入音频帧时有效*/
    LONG_32                             lFormatType;                       /**< 视频帧类型，仅在输入视频帧有效，具体见# MEDIASDK_VIDEO_FRAME_TYPE_E ）*/
    DULONG                              dulTimeStamp;                      /**< 时戳 */
    ULONG_32                            ulRecvTime;                        /**< 网络接收时间 */
    ULONG_32                            ulPlaybackTime;                    /**< 回放时间 */
    BOOL_T                              bNeedFrameCallback;                /**< 是否回调该帧数据 */
}MEDIASDK_FRAME_DATA_S;

/**
* 记录日志的回调函数类型定义 \n
* @param [IN] ulLogLevel        日志级别, 对应# MEDIASDK_LOGLEVEL_E 枚举中的值
* @param [IN] ulErrCode         日志信息错误码
* @param [IN] ulCmdModuleID     命令行模块ID
* @param [IN] pcFileName        代码文件名
* @param [IN] ulFileLine        代码所在行数
* @param [IN] pcLogDescription  日志描述字符串
* @return VOID
* @note
* -    1) MediaSDK内部记录日志级别;
* -    2) MediaSDK内部多线程调用该回调函数，用户记录日志处理中需要做线程同步处理;
* -    3) MediaSDK内部输出的日志描述字符串末尾没有带"\r\n"符。用户在写日志文件的时候需要做换行处理。
*/
typedef VOID (STDCALL* MEDIASDK_WRITE_LOG_CALLBACK_PF)(IN ULONG_32 ulLogLevel,
                                                       IN ULONG_32 ulErrCode,
                                                       IN ULONG_32 ulCmdModuleID,
                                                       IN const CHAR *pcFileName,
                                                       IN ULONG_32 ulFileLine,
                                                       IN const CHAR *pcLogDescription);

/**
* 消息或异常消息回调函数的指针类型 \n
* @param [IN] ulPort            通道号
* @param [IN] ulRunInfoType     消息或者异常消息类型，对应# MEDIASDK_RUN_INFO_TYPE_E 枚举中的值
* @param [IN] pParam            存放消息或异常消息数据的缓冲区指针（所存放的数据与消息或异常消息类型有关）
* @param [IN] pUserParam        用户参数
* @return VOID
* @note
* -     1) 用户需要确保该回调函数尽快返回
* -     2) 用户不能在该回调函数中直接调用播放器SDK的任何接口函数
* -     3) 参数pParam所指的缓冲区中存放的数据的类型视消息或者异常消息类型而定，用户需要根据消息或者异常消息类型对其做类型转换
* -        当ulRunInfoType值为：
* -        MEDIASDK_RUN_INFO_RECORD_VIDEO
* -        MEDIASDK_RUN_INFO_MEDIA_PROCESS
* -        MEDIASDK_RUN_INFO_MEDIA_VOICE
* -        MEDIASDK_RUN_INFO_STREAM
* -        MEDIASDK_RUN_INFO_ASYNC_START_STREAM
* -        用户需将pParam转换为# MEDIASDK_RUN_INFO_S 结构体类型指针
*/
typedef VOID (STDCALL* MEDIASDK_RUN_INFO_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                      IN ULONG_32 ulRunInfoType,
                                                      IN VOID *pParam,
                                                      IN VOID *pUserParam);

/**
* 拼帧前媒体流数据回调函数的指针类型 \n
* @param [IN] ulPort                资源号
* @param [IN] pstSourceData         存放拼帧前媒体数据信息缓冲区指针
* @param [IN] lUserParam            用户设置参数，即用户在调用# MediaSDK_SetSourceDataCB 函数时指定的用户参数
* @param [IN] lReserved             保留参数
* @return 无。
* @note  1) 用户应及时处理输出的媒体流数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL* MEDIASDK_SOURCE_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                         IN const MEDIASDK_SOURCE_DATA_S *pstSourceData,
                                                         IN VOID* pUserParam,
                                                         IN LONG_REAL lReserved);

/**
* 拼帧后视频流数据回调函数的指针类型 \n
* @param [IN] ulPort                资源号
* @param [IN] pstParseVideoData     存放拼帧后视频流数据信息缓冲区指针
* @param [IN] lUserParam            用户设置参数，即用户在调用# MediaSDK_SetParseVideoDataCB 函数时指定的用户参数
* @param [IN] lReserved             保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL *MEDIASDK_PARSE_VIDEO_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                              IN const MEDIASDK_PARSE_VIDEO_FRAME_S *pstParseVideoData,
                                                              IN VOID* pUserParam,
                                                              IN LONG_REAL lReserved);

/**
* 拼帧后音频流数据回调函数的指针类型 \n
* @param [IN] ulPort                资源号
* @param [IN] pstParseAudioData     存放拼帧后音频流数据信息缓冲区指针
* @param [IN] lUserParam            用户设置参数，即用户在调用# MediaSDK_SetParseAudioDataCB 函数时指定的用户参数
* @param [IN] lReserved             保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL *MEDIASDK_PARSE_AUDIO_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                              IN const MEDIASDK_PARSE_AUDIO_FRAME_S *pstParseAudioData,
                                                              IN VOID* pUserParam,
                                                              IN LONG_REAL lReserved);

/**
* 解码后视频流数据回调函数的指针类型 \n
* @param [IN] ulPort                资源号
* @param [IN] pDecodeVideoData      存放解码后视频流数据信息缓冲区指针
* @param [IN] lUserParam            用户设置参数，即用户在调用# MediaSDK_SetDecodeVideoDataCB 函数时指定的用户参数
* @param [IN] lReserved             保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的视频流数据，确保函数尽快返回，否则会影响播放器内的媒体流处理；
* -     2) pPictureData->ulDataType指明解码后的数据格式，需根据所述格式进行数据处理；
* -     3) 默认解码后视频数据是yv12格式。排列顺序“Y0-Y1-......”，“U0-U1-......”，“V0-V1-......”
*/
typedef VOID (STDCALL* MEDIASDK_DECODE_VIDEO_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                               IN const MEDIASDK_DECODE_VIDEO_RAW_S *pDecodeVideoData,
                                                               IN VOID* pUserParam,
                                                               IN LONG_REAL lReserved);

/**
* 解码后音频流数据回调函数的指针类型 \n
* @param [IN] ulPort         资源号
* @param [IN] pWaveData      存放解码后音频流数据信息缓冲区指针
* @param [IN] lUserParam     用户设置参数，即用户在调用# NDPlayer_SetDecodeAudioMediaDataCB 函数时指定的用户参数
* @param [IN] lReserved      保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的音频流数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL* MEDIASDK_DECODE_AUDIO_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                               IN const MEDIASDK_DECODE_AUDIO_RAW_S *pDecodeAudioData,
                                                               IN VOID* pUserParam,
                                                               IN LONG_REAL lReserved);

/**
* 画图回调函数的指针类型 \n
* @param [IN] ulPort          资源号
* @param [IN] pstDrawInfo     画图信息结构体指针
* @param [IN] lUserParam      用户设置参数，即用户在调用# NDPlayer_SetDecodeAudioMediaDataCB 函数时指定的用户参数
* @return 无。
* @note
*/
typedef VOID (STDCALL* MEDIASDK_DRAW_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                  IN const MEDIASDK_DRAW_INFO_S *pstDrawInfo,
                                                  IN VOID* pUserParam);

/**
* 用于智能元数据(SEI负载)回调函数的指针类型 \n
* @param [IN] ulPort         资源号
* @param [IN] pRuleHead      存放规则信息头
* @param [IN] pRuleData      存放规则信息缓冲区指针，MEDIASDK_IVA_RULE_V2_S类型，具体个数参考存放规则信息头的规则数量
* @param [IN] ulRuleLen      规则信息缓冲区长度
* @param [IN] pResultHead    存放结果信息头
* @param [IN] pResultData    存放结果信息缓冲区指针，MEDIASDK_IVA_RESULT_V2_S类型，具体个数参考存放结果信息头的目标数量
* @param [IN] ulResultLen    结果信息缓冲区长度
* @param [IN] lUserParam     用户设置参数，即用户在调用# MediaSDK_SetIvaDataCB 函数时指定的用户参数
* @param [IN] lReserved      保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL *MEDIASDK_IVA_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                      IN MEDIASDK_IVA_RULE_HEAD_S *pRuleHead,
                                                      IN const VOID *pRuleData,
                                                      IN ULONG_32 ulRuleLen,
                                                      IN MEDIASDK_IVA_RESULT_HEAD_S *pResultHead,
                                                      IN const VOID *pResultData,
                                                      IN ULONG_32 ulResultLen,
                                                      IN VOID* pUserParam,
                                                      IN LONG_REAL lReserved);

/**
* 用于元数据(RTP负载)回调函数的指针类型\n
* @param [IN] ulPort            资源号
* @param [IN] ulMetaDataType   上报的元数据的类型，由MEDIASDK_METADATA_TYPE_E确定
* @param [IN] pMetaData        上报的数据缓冲区指针
* @param [IN] ulDataLen        上报的数据缓冲区长度
* @param [IN] lUserParam       用户设置参数，即用户在调用MediaSDK_SetMetaDataCB函数时指定的用户参数
* @param [IN] lReserved        保留参数
* @return 无。
* @note
* -     1) 用户应及时处理输出的数据，确保函数尽快返回，否则会影响播放器内的媒体流处理
        2) 此回调函数用于上报SD卡、云台元数据，智能规则结果数据由MEDIASDK_METADATA_CALLBACK_PF进行上报
*/
typedef VOID (STDCALL *MEDIASDK_META_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                       IN MEDIASDK_METADATA_TYPE_E ulMetaDataType, 
                                                       IN const VOID *pMetaData,
                                                       IN ULONG_32 ulDataLen,
                                                       IN VOID* pUserParam,
                                                       IN LONG_REAL lReserved);




/**
* 录像数据回调函数的指针类型。\n
* @param [IN] ulPort                播放通道号
* @param [IN] pBuffer               存放媒体流数据缓冲区指针。
* @param [IN] ulBufSize             缓冲区大小。
* @param [IN] lUserParam            用户设置参数
* @param [IN] lReserved1            保留参数1
* @param [IN] lReserved2            保留参数2
* @return 无。
* @note
* -     用户应及时处理输出的媒体流数据，确保函数尽快返回，否则会影响播放器内的媒体流处理。
*/
typedef VOID (STDCALL* MEDIASDK_RECORD_DATA_CALLBACK_PF)(IN ULONG_32 ulPort,
                                                         IN const UCHAR *pBuffer,
                                                         IN ULONG_32 ulBufSize,
                                                         IN LONG_REAL lUserParam,
                                                         IN LONG_REAL lReserved1,
                                                         IN LONG_REAL lReserved2);



#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _MEDIASDK_COM_DEF_H */