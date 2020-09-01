#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "SocketStruct.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include <cstdlib>
#include <ctime>
#include "HPSocket.h"


// ȫ�ִ���Socket�б�
SocketList *socketList;

// �����б�
void CreateSocketList()     /*����ÿһ���ڵ㶼��ָ��  SocketListPointStruct�ṹ��ָ�룬���Է���ֵ��SocketListPointStruct *,n��ָ����������Ľڵ���Ŀ*/
{
	SocketList *normal, *next;/*����ͷ�ڵ㣬�����ڵ㣬��β�ڵ�*/
	next = new SocketList;

	/*�����ڴ�*/
	socketList = next;/*�ʼ���һ���ڵ����ͷ�ڵ㣬ע����Ϊͨ��ָ�����ֱ�ӶԵ�ַ�ϵĶ������в�������ʱend��headָ��ͬһ����ַ����end��ָ���ַ���в�������ͬ�ڶ�head��ַ�����Ĳ���*/
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		normal = new SocketList;

		/* ���½ڵ�������ݣ�ע������ֻ������Ľڵ�������ݣ�head��������*/
		next->next = normal;/*��end�������½ڵ�*/
		next = next->next;
	}
	next->next = NULL;/*��������ָ��һ���µ�ַ*/
}
// ��ȡ����΢��ID�б��ַ������ö��Ÿ���
CString GetAllSocketWechatIdList()
{
	CString list;

	int number = 0;
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && strlen(p->wechatId) > 0) {
			if (number == 0) {
				list += p->wechatId;
			}
			else {
				list += ",";
				list += p->wechatId;
			}
			number++;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return list;
}
// ɾ��Socket
void DeleteSocket(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			if (p->RecvDataLength > 0) {
				// �ͷŷְ��������Դ
				p->RecvDataLength = 0;
				free(p->RecvData);
				p->RecvState = FALSE;
			}
			p->dwConnID = 0;
			sprintf_s(p->wechatId, 33, "%s", "");
			p->next = p->next->next;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// �����һ�����̣���΢��ID
void AddWechatProcess(int processId, char *wechatId) 
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p != NULL && p->processId == 0) {
			p->processId = processId;
			sprintf_s(p->wechatId, 33, "%s", wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// �����һ��Socket
void AddSocket(CONNID dwConnID, int processId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p != NULL && p->processId == processId) {
			p->dwConnID = dwConnID;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// ��ȡsocket��΢�ſͻ���ID - ����ID
void GetSocketWechatIdByProcessId(int processId, char *wechatId) 
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->processId == processId) {
			sprintf_s(wechatId, 33, "%s", p->wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// ��ȡsocket��΢�ſͻ���ID
void GetSocketWechatId(CONNID dwConnID, char *wechatId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			sprintf_s(wechatId, 33, "%s", p->wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// ��ȡsocket��΢�ſͻ���ID
CONNID GetSocketConnectId(char *wechatId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && !strncmp(p->wechatId, wechatId, 33)) {
			return p->dwConnID;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return 0;
}
// ��ȡsocket��΢�ſͻ���ID
bool GetSocketRecvState(CONNID dwConnID)
{
	bool RecvState = FALSE;
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			RecvState = p->RecvState;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return RecvState;
}
// ��ȡsocket����Ϣ���ݴ���
void GetSocketRecvData(CONNID dwConnID, char *recvData)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			p->RecvData[p->RecvDataLength] = '\0';
			sprintf_s(recvData, (size_t)(p->RecvDataLength + 1), "%s", p->RecvData);
			return;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return;
}
// �����Ϣ����
void AddSocketRecvData(CONNID dwConnID, const BYTE* pData, int iLength)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			// �·����ڴ�ռ�
			if (p->RecvState == FALSE) {
				if ((p->RecvData = (char *)malloc((iLength + 1) * sizeof(char))) == NULL) {
					return;
				}
				p->RecvState = TRUE;
				memcpy(p->RecvData, pData, iLength);
			}
			// ��̬�����ڴ��С
			else {
				p->RecvData = (char *)realloc(p->RecvData, (size_t)((p->RecvDataLength + iLength + 1) * sizeof(char)));
				if (p->RecvData == NULL) {
					free(p->RecvData);
					p->RecvState = FALSE;
					return;
				}
				// ׷���ڴ�
				for (int i = 0; i < iLength; i++) {
					p->RecvData[p->RecvDataLength + i] = pData[i];
				}
			}
			p->RecvDataLength += iLength;

			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// ��ȡsocket����Ϣ���ݴ��泤��
long long int GetSocketRecvDataLength(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			return p->RecvDataLength;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return 0;
}
// �ͷŷְ��������Դ
void FreeSocketRecvData(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			// �ͷŷְ��������Դ
			p->RecvDataLength = 0;
			free(p->RecvData);
			p->RecvState = FALSE;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}