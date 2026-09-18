MediaSDK 直接拉流 + 鱼眼矫正 + 输出 YUV

// 1. 初始化
MediaSDK_Init();

	// 2. 创建资源号（等同于播放通道）
	ULONG_32 ulPort;
	MediaSDK_CreatPortRes(&ulPort);

	// 3. 设置解码后YUV回调
	MediaSDK_SetDecodeVideoDataCB(ulPort, MyVideoCallback, BOOL_TRUE, pUserParam);

	// 4. 切换回调为矫正后数据（关键！）
	MediaSDK_SetVideoCallBackMode(ulPort, MEDIASDK_VIDEO_CB_PROCESSED_DATA);  // = 1

	// 5. 启流 —— 直接传 RTSP URL
	MEDIASDK_RTSP_REALPLAY_S stRealPlay = {0};
	stRealPlay.enRtspType = MEDIASDK_RTSP_RTP_TCP;     // 协议类型
	stRealPlay.enRtspExecuteType = ...;                  // RTSP执行模式
	stRealPlay.enStreamMode = MEDIASDK_RTSP_MODE_ONLY_VIDEO;
	stRealPlay.pcURL = "rtsp://admin:password@192.168.1.1/stream1";
	stRealPlay.pWnd = NULL;  // 不需要显示时传NULL
	MediaSDK_StartStream(ulPort, MEDIASDK_STREAM_RTSP_REALPLAY, &stRealPlay, BOOL_FALSE);

	// 6. 设置鱼眼矫正模式和安装方式（必须！）
	MediaSDK_SetPtzAndFixMode(ulPort, MEDIASDK_360_1PTZ, MEDIASDK_TOP_INSTALLATION);

	// 7. 回调中获取矫正后的YUV数据
	void STDCALL MyVideoCallback(ULONG_32 ulPort,
	  const MEDIASDK_DECODE_VIDEO_RAW_S *pDecodeVideoData,
	  void *pUserParam, LONG_REAL lReserved)
	{
	  // pDecodeVideoData->pucData[0] = Y平面
	  // pDecodeVideoData->pucData[1] = U平面
	  // pDecodeVideoData->pucData[2] = V平面
	  // pDecodeVideoData->ulPicWidth / ulPicHeight / eDataType
	  // 此处就是矫正后的YUV数据，可自行处理
	}

	// 8. 停止
	MediaSDK_StopStream(ulPort);
	MediaSDK_DestroyPortRes(ulPort);
MediaSDK_Cleanup();