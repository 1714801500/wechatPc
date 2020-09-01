#pragma once
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <strstream>
#include <queue>
#include "Send.h"

using namespace std;

// ��Ϣ�ṹ��
struct wxMessageStruct
{
	wchar_t * text;
	DWORD textLength;
	DWORD textLen;
};
// ����XML��Ϣ�ṹ
struct SendXmlCardWxString
{
	wchar_t * pStr;
	DWORD strLen;
	DWORD strMaxLen;
	char fillbuff[0x8] = { 0 };
};
//����ͼƬ�Ľṹ��
struct SendImageWxidStruct
{
	wchar_t* pWxid;
	DWORD length = 0;
	DWORD maxLength = 0;
};
//����ͼƬ�Ľṹ��
struct SendImageFileStruct
{
	wchar_t* filePath;
	DWORD length = 0;
	DWORD maxLength = 0;
	char fillbuff[0x18] = { 0 };
};
//����xml�������ݽṹ��
struct SendXmlStruct
{
	wchar_t* content;
	DWORD strLen;
	DWORD strMaxLen;
	char fillbuff[0x8] = { 0 };
};
//����xml���½�����΢��ID�ṹ��
struct SendXmlWxidStruct
{
	wchar_t* wxid;
	DWORD wxidLen;
	DWORD wxidMaxLen;
	char file[0x8] = { 0 };
};
//����xml���·�����΢��ID�ṹ��
struct SendXmlFromWxidStruct
{
	wchar_t* wxid;
	DWORD wxidLen;
	DWORD wxidMaxLen;
	char file[0x8] = { 0 };
};
// ����xml���±���ͼƬ�ṹ��
struct SendXmlImageStruct
{
	wchar_t* filePath;
	DWORD pathLen;
	DWORD pathMaxLen;
	char file[0x18] = { 0 };
};
//���͸����ļ�΢��ID
struct SendAttachWxidStruct
{
	wchar_t* str;
	DWORD strLen = 0;
	DWORD maxLen = 0;
};
//���͸����ļ��ṹ��
struct SendAttachFileStruct
{
	wchar_t* str;
	DWORD strLen = 0;
	DWORD maxLen = 0;
	char file[0x18] = { 0 };
};

// �����ı���Ϣ
VOID SendTextMessage(wchar_t *wxid, wchar_t *message);
// ������Ϣ
VOID ListenMessage();
// ����XML��Ƭ
void SendXmlCardMessage(wchar_t* RecverWxid, wchar_t* xmlData);
// ����ͼƬ
void SendImageMessage(wchar_t* wxid, wchar_t* filepath);
// ����xml
void SendXmlMessage(DWORD type, wchar_t* recverWxid, wchar_t* fromWxid, wchar_t* filePath, wchar_t* xmlData);
// ���͸����ļ�
void SendFileMessage(wchar_t* wxid, wchar_t* filepath);