#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "Sha1.h"
#include "Base64.h"
#include "BufferPtr.h"
#include "OpCode.h"
#include "Package.h"
#include "SocketStruct.h"
#include "WebSocketClient.h"
#include "WebSocketListen.h"
#include "WechatService.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"


IHttpServer* pServer;
WebSocketServer *cServer;

// ��������
void WsServerSend(CONNID dwConnID, char *body)
{
	// ת��
	size_t bodyLength = strlen(body);
	// ��ʼ����Ϣ
	CBufferPtr* pBuffer = new CBufferPtr(bodyLength);
	pBuffer->Copy((BYTE*)body, bodyLength);
	// ��ȡ��ʶ
	BYTE iReserved, iOperationCode;
	pServer->GetWSMessageState(dwConnID, nullptr, &iReserved, &iOperationCode, nullptr, nullptr, nullptr);
	// ������Ϣ
	pServer->SendWSMessage(dwConnID, TRUE, iReserved, iOperationCode, pBuffer->Ptr(), (int)pBuffer->Size());
	// �ͷ���Ϣ��Դ
	pBuffer->Free();
}
// ��������
void WsServerRecvCallback(CONNID dwConnID, char *data)
{
	// ��ʼ�����ݰ�
	Package *package = new Package();
	package->SetConText(data);
	// ������ݰ���Ҫ����
	/* ���ﲻ���жϣ���Ϊdll��û��appid */
	/*
	if (package->Check() == FALSE) {
		MessageBox(NULL, L"�յ���������ݰ���", L"��ܰ��ʾ��", NULL);
		return;
	}
	*/
	int opCode = package->GetOpCode();

	// ΢���˳��ˣ��رս���
	if (opCode == OpCode::OPCODE_WECHAT_QUIT) {
		Value *pBody = package->GetBody();
		Value &body = *pBody;
		if (!body.HasMember("processId") || !body.HasMember("loginStatus")) {
			return;
		}
		Value& processId = body["processId"];
		Value& loginStatus = body["loginStatus"];

		// δ�˳�������
		if (loginStatus.GetInt() != 0) {
			return;
		}

		// �رս���
		CloseProcess(processId.GetInt());
		// �޸ķ��ط�������֪ͨ�˳���¼
		body.RemoveMember("processId");
		package->SetBody(body);
	}

	// ��ֵ΢�ſͻ���ID
	char wechatId[33] = { 0 };
	GetSocketWechatId(dwConnID, wechatId);

	if (strlen(wechatId)) {
		package->SetWechatId(wechatId);
	}
	rapidjson::StringBuffer context = package->GetConText();
	const char *json = context.GetString();
	// ���͸���������
	WsClientSend((char *)json);
}
// �����˿�
BOOL WsServerListen()
{
	CString strBindAddr = WEBSOCKET_LISTEN_IP_ADDRESS;
	const USHORT port = atoi(WEBSOCKET_LISTEN_IP_PORT);
	// ��������
	if (!pServer->Start(strBindAddr, port)) {
		MessageBox(NULL, L"����Websocket����ʧ�ܣ�", L"��ܰ��ʾ��", NULL);
		return FALSE;
	}
	return TRUE;
}
// ��ʼ��
void WsServerInit()
{
	// ����socket�����б�
	CreateSocketList();
	// ��ʼ��websocket
	cServer = new WebSocketServer;
	pServer = HttpServer_Creator::Create(cServer);
	// ��ʼ�����˿�
	WsServerListen();
}
// �رռ���
void WsServerClose()
{
	pServer->Stop();
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	// ɾ��Socket�������Ϣ
	DeleteSocket(dwConnID);

	CBufferPtr* pBuffer = nullptr;
	pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);

	if (pBuffer) delete pBuffer;

	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHttpParseResult WebSocketServer::OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
{
	if (enUpgradeType == HUT_HTTP_TUNNEL) {
		pSender->SendResponse(dwConnID, HSC_OK, "Connection Established");
	}
	else if (enUpgradeType == HUT_WEB_SOCKET) {
		// ��ȡ����ID�����Ҵ���socket����
		LPCSTR processIdPtr = nullptr;
		if (!pSender->GetHeader(dwConnID, "Process-Id", &processIdPtr)) {
			return HPR_ERROR;
		}
		CStringA processIdStr(processIdPtr);
		int processId = atoi(processIdStr.GetString());
		// ��ȡ΢��ID�����ߺ��������ӵ�
		LPCSTR wechatIdPtr = nullptr;
		if (pSender->GetHeader(dwConnID, "Wechat-Id", &wechatIdPtr)) {
			CStringA wechatIdStr(wechatIdPtr);
			AddWechatProcess(processId, (char *)wechatIdStr.GetString());
		}

		// ��ӽ�������
		AddSocket(dwConnID, processId);

		// ����ظ���Ϣ
		int iHeaderCount = 2;
		THeader header[] = { {"Connection", "Upgrade"},
							{"Upgrade", "WebSocket"},
							{nullptr, nullptr},
							{nullptr, nullptr},
							{nullptr, nullptr} };

		LPCSTR lpszAccept = nullptr;

		if (!pSender->GetHeader(dwConnID, "Sec-WebSocket-Key", &lpszAccept))
			return HPR_ERROR;

		CStringA strAccept;

		BYTE OutSHA1Buf[80];
		char key[40];

		CStringA strKey(lpszAccept);
		strKey.Append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

		// ����key
		int len = SHA1_String((unsigned char*)strKey.GetString(), strKey.GetLength(), OutSHA1Buf);
		base64_encode(OutSHA1Buf, 20, key);
		strAccept = key;

		header[2].name = "Sec-WebSocket-Accept";
		header[2].value = strAccept;
		++iHeaderCount;

		CStringA strFirst;
		LPCSTR lpszProtocol = nullptr;

		if (pSender->GetHeader(dwConnID, "Sec-WebSocket-Protocol", &lpszProtocol)) {
			int i = 0;
			CStringA strProtocol(lpszProtocol);
			strFirst = strProtocol.Tokenize(", ", i);

			if (!strFirst.IsEmpty()) {
				header[3].name = "Sec-WebSocket-Protocol";
				header[3].value = strFirst;
				++iHeaderCount;
			}
		}
		
		// ����΢��ID
		char wechatId[33] = { 0 };
		GetSocketWechatIdByProcessId(processId, wechatId);
		header[4].name = "Wechat-Id";
		header[4].value = wechatId;
		++iHeaderCount;

		pSender->SendResponse(dwConnID, HSC_SWITCHING_PROTOCOLS, nullptr, header, iHeaderCount);
		pSender->SetConnectionExtra(dwConnID, new CBufferPtr);
	}

	return HPR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketServer::OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	// ��������
	AddSocketRecvData(dwConnID, pData, iLength);

	return HR_OK;
}

EnHandleResult WebSocketServer::OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
{
	BYTE iOperationCode;
	pSender->GetWSMessageState(dwConnID, nullptr, nullptr, &iOperationCode, nullptr, nullptr, nullptr);

	// �ر�����
	if (iOperationCode == 0x8) {
		pSender->Disconnect(dwConnID);
		return HR_OK;
	}

	// ��������
	long long int RecvDataLength = GetSocketRecvDataLength(dwConnID);
	char *data;
	if ((data = (char *)malloc((size_t)(RecvDataLength + 1) * sizeof(char))) == NULL) {
		return HR_OK;
	}
	GetSocketRecvData(dwConnID, data);
	// ������յ�������
	WsServerRecvCallback(dwConnID, data);
	free(data);
	// �ͷŷְ��������Դ
	FreeSocketRecvData(dwConnID);

	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

CStringA WebSocketServer::GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength)
{
	CStringA SEP1;

	for (int i = 0; i < iSepCount; i++)
		SEP1 += lpszSep;

	CStringA SEP2(SEP1);
	SEP2 += lpszSep;

	CStringA strResult;

	USHORT usUrlFieldSet = pSender->GetUrlFieldSet(dwConnID);

	strResult.AppendFormat("%s[URL Fields]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "SCHEMA", pSender->GetUrlField(dwConnID, HUF_SCHEMA), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "HOST", pSender->GetUrlField(dwConnID, HUF_HOST), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "PORT", pSender->GetUrlField(dwConnID, HUF_PORT), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "PATH", pSender->GetUrlField(dwConnID, HUF_PATH), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "QUERY", pSender->GetUrlField(dwConnID, HUF_QUERY), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "FRAGMENT", pSender->GetUrlField(dwConnID, HUF_FRAGMENT), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "USERINFO", pSender->GetUrlField(dwConnID, HUF_USERINFO), CRLF);

	DWORD dwHeaderCount = 0;
	pSender->GetAllHeaders(dwConnID, nullptr, dwHeaderCount);

	strResult.AppendFormat("%s[Request Headers]%s", SEP1, CRLF);

	if (dwHeaderCount == 0)
		strResult.AppendFormat("%s(no header)%s", SEP2, CRLF);
	else
	{
		unique_ptr<THeader[]> headers(new THeader[dwHeaderCount]);
		//VERIFY(pSender->GetAllHeaders(dwConnID, headers.get(), dwHeaderCount));
		pSender->GetAllHeaders(dwConnID, headers.get(), dwHeaderCount);

		for (DWORD i = 0; i < dwHeaderCount; i++)
			strResult.AppendFormat("%s%s: %s%s", SEP2, headers[i].name, headers[i].value, CRLF);
	}

	DWORD dwCookieCount = 0;
	pSender->GetAllCookies(dwConnID, nullptr, dwCookieCount);

	strResult.AppendFormat("%s[Cookies]%s", SEP1, CRLF);

	if (dwCookieCount == 0)
		strResult.AppendFormat("%s(no cookie)%s", SEP2, CRLF);
	else
	{
		unique_ptr<TCookie[]> cookies(new TCookie[dwCookieCount]);
		//VERIFY(pSender->GetAllCookies(dwConnID, cookies.get(), dwCookieCount));
		pSender->GetAllCookies(dwConnID, cookies.get(), dwCookieCount);

		for (DWORD i = 0; i < dwCookieCount; i++)
			strResult.AppendFormat("%s%s: %s%s", SEP2, cookies[i].name, cookies[i].value, CRLF);
	}

	EnHttpUpgradeType enUpgType = pSender->GetUpgradeType(dwConnID);
	LPCSTR lpszUpgrade = enUpgType != HUT_NONE ? "true" : "false";
	LPCSTR lpszKeepAlive = pSender->IsKeepAlive(dwConnID) ? "true" : "false";

	strResult.AppendFormat("%s[Basic Info]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "Method", pSender->GetMethod(dwConnID), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsUpgrade", lpszUpgrade, CRLF);
	if (enUpgType != HUT_NONE)
		strResult.AppendFormat("%s%13s: %d%s", SEP2, "UpgradeType", enUpgType, CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsKeepAlive", lpszKeepAlive, CRLF);
	if (bWithContentLength)
		strResult.AppendFormat("%s%13s: %lld%s", SEP2, "ContentLength", pSender->GetContentLength(dwConnID), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "ContentType", pSender->GetContentType(dwConnID), CRLF);

	return strResult;
}
