#ifndef _UENCRYPT_SDK_FUNC_H
#define _UENCRYPT_SDK_FUNC_H

#include "umedia_com_type.h"

#ifdef  __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/* 函数标准调用约定 Standard function calling convention */
#ifdef WIN32
    #define STDCALL                 __stdcall
    #ifdef UENCRYPT_DLL_EXPORTS
        #define UENCRYPT_EXPORT     __declspec( dllexport )
    #else
        #define UENCRYPT_EXPORT     __declspec( dllimport )
    #endif
#else
    #ifdef i386
        #define STDCALL             __attribute__((stdcall))
    #else
        #define STDCALL
    #endif
    #define UENCRYPT_EXPORT        __attribute__((visibility("default")))
#endif


/**
* @brief  SHA256加密
* @param  pucData               输入待加密字符串
* @param  nLen                  输入待加密字符串长度
* @param  pucOut                输出加密字符串
* @return 无
* @note   1) sha256计算生成一个256位的哈希值，数据长度为32字节（通常会被表示成64个十六进制数字）
*         2) 出参空间由外部分配，需考虑终止符，为加密数据长度+1
*/
UENCRYPT_EXPORT VOID STDCALL UE_Sha256_Calc(IN CONST UCHAR *pucData, IN ULONG_32 nLen, OUT UCHAR *pucOut);

/**
* @brief  MD5加密
* @param  pucData               输入待加密字符串
* @param  nLen                  输入待加密字符串长度
* @param  pucOut                输出加密字符串
* @return 无
* @note   1) md5计算生成一个128位的哈希值，数据长度为16字节（通常会被表示成32个十六进制数字）
*         2) 出参空间由外部分配，需考虑终止符，为加密数据长度+1
*/
UENCRYPT_EXPORT VOID STDCALL UE_Md5_Calc(IN CONST UCHAR *pucData, IN ULONG_32 nLen, OUT UCHAR *pucOut);

/**
* @brief  BASE64编码
* @param  pcData                输入待编码字符串
* @param  nLen                  输入待编码字符串长度
* @param  pucOut                输出编码后字符串
* @return 编码后数据长度
* @note   1) Base64编码会将原始数据每3个字节转换为4个字符，不足3字节填充后按3字节处理
*         2) 出参空间分配可按上述规则分配（eg:可按入参长度2倍进行分配）
*/
UENCRYPT_EXPORT LONG_32 STDCALL UE_Base64_Enc(IN CONST CHAR *pcData, IN LONG_32 nLen, OUT CHAR *pcOut);

/**
* @brief  BASE64解码
* @param  pcData                输入待解码字符串
* @param  pucOut                输出解码后字符串
* @return 解码后数据长度
* @note   1) 每4个字符对应原始数据的3个字节
*         2) 出参空间分配可按上述规则分配（eg：可按入参长度进行分配）
*/
UENCRYPT_EXPORT LONG_32 STDCALL UE_Base64_Dec(IN CONST CHAR *pcData, OUT CHAR *pcOut);

#ifdef  __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _UENCRYPT_SDK_FUNC_H */
