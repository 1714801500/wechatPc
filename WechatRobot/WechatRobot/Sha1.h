#pragma once

#ifndef _USER_SHA1_H
#define _USER_SHA1_H

#include <stdio.h>
#include <memory.h>



#define SHA1_SIZE_BYTE 20

/**
* @brief SHA1_String
*
* Detailed description.
* @param[in] inputString Ҫ���д�����޷����ַ���ָ��
* @param[in] len Ҫ�������Ϣ���ȣ�n<= strlen(p);
* @param[out] pOutSHA1Buf ���ժҪ��Ϣ������Ϊ20�� unsigned char ����160 bits
* @return int
*/
int SHA1_String(const unsigned char* inputString, unsigned long len, unsigned char* pOutSHA1Buf);

/**
* @brief SHA1_String_Compare
*
* Detailed description.
* @param[in] inputString
* @param[in] len
* @param[in] pOutSHA1Buf
* @return int
*/
int SHA1_String_Compare(const unsigned char* inputString, unsigned long len, const unsigned char* pOutSHA1Buf);

#endif /**< _USER_SHA1_H */
