#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include <cstdlib>
#include <ctime>
#include "HPSocket.h"


typedef struct SocketList
{
	// ����ID
	int processId = 0;
	// ����ID
	CONNID dwConnID = 0;
	// ΢��ID
	char wechatId[33] = {0};

	// �Ƿ����ڽ������� - ���ְ���Ҫ��¼
	bool RecvState = FALSE;
	// ��ǰ���յ������� - ���ְ���Ҫ��¼
	char *RecvData = { 0 };
	// ��ǰ�İ����� - ���ְ���Ҫ��¼
	long long int RecvDataLength = 0;

	SocketList *next;
}SocketList;

// �����б�
void CreateSocketList();
// ��ȡ����΢��ID�б��ַ������ö��Ÿ���
CString GetAllSocketWechatIdList();
// ɾ��Socket
void DeleteSocket(CONNID dwConnID);
// �����һ�����̣���΢��ID
void AddWechatProcess(int processId, char *wechatId);
// �����һ��Socket
void AddSocket(CONNID dwConnID, int processId);
// ��ȡsocket��΢�ſͻ���ID - ����ID
void GetSocketWechatIdByProcessId(int processId, char *wechatId);
// ��ȡsocket��΢�ſͻ���ID
void GetSocketWechatId(CONNID dwConnID, char *wechatId);
// ��ȡsocket��΢�ſͻ���ID
CONNID GetSocketConnectId(char *wechatId);
// ��ȡsocket��΢�ſͻ���ID
bool GetSocketRecvState(CONNID dwConnID);
// ��ȡsocket����Ϣ���ݴ���
void GetSocketRecvData(CONNID dwConnID, char *recvData);
// �����Ϣ����
void AddSocketRecvData(CONNID dwConnID, const BYTE* pData, int iLength);
// ��ȡsocket����Ϣ���ݴ��泤��
long long int GetSocketRecvDataLength(CONNID dwConnID);
// �ͷŷְ��������Դ
void FreeSocketRecvData(CONNID dwConnID);