#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"


using namespace std;

// ��ʱ�����µ�Ӧ����Ϣ
//void SetAppTmp(char *newWechatId);
//void GetAppTmp(char *newWechatId);
//void FreeAppTmp();

// ��ʼ��
void WsClientInit();
// ��������
void WsClientSend(char *body);
// ��������
void WsClientRecvCallback(char *data);
// �������ӷ����
BOOL WsClientKeepConnect(int time);
// �������ӷ����
void WsClientReConnect(int time);
// ���ӷ����
BOOL WsClientConnect();
// �رշ��������
void WsClientClose();
// �������ݰ��÷���˹ر�����
void WsClientSafeClose();
// �������ְ�
void WsClientHandShake();


// WebSocketClient class
class WebSocketClient : public CHttpClientListener
{
private:
	// �Ƿ����ڽ������� - ���ְ���Ҫ��¼
	bool RecvState = FALSE;
	// ��ǰ���յ������� - ���ְ���Ҫ��¼
	char *RecvData = {0};
	// ��ǰ�İ����� - ���ְ���Ҫ��¼
	long long int RecvDataLength = 0;

	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) { return HR_OK; }

	virtual EnHttpParseResult OnHeadersComplete(IHttpClient* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HPR_OK; }
	virtual EnHttpParseResult OnMessageComplete(IHttpClient* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnParseError(IHttpClient* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) { return HPR_OK; }

	virtual EnHandleResult OnWSMessageHeader(IHttpClient* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen) { return HR_OK; }
	virtual EnHandleResult OnWSMessageBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnWSMessageComplete(IHttpClient* pSender, CONNID dwConnID);
	virtual CStringA GetHeaderSummary(IHttpClient* pHttpClient, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength);
};

