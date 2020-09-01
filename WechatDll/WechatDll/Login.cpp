#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "Common.h"
#include "WechatOffset.h"
#include "Send.h"
#include "Login.h"

// �˳���¼
void DoLogout()
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();
	DWORD dwCallAddr = winAddress + LOGOUT;

	__asm {
		pushad
		push 0x0
		call dwCallAddr;
		popad
	}
}

// ��ȡ��¼״̬
int CheckLoginStatus()
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();
	// ����Ƿ��Ѿ���½
	wchar_t isLogin[8] = { 0 };
	memcpy(isLogin, (void*)(winAddress + ISLOGIN), 1);
	if (isLogin[0] == 1) {
		// �����ѵ�¼
		return 1;
	}
	return 0;
}

bool SendLoginStatus(bool sendQrcode) 
{
	// ����Ƿ��Ѿ���½
	if (CheckLoginStatus() == 1) {
		// ֪ͨ�������˵�¼�ɹ�
		Send::SendLoginStatus(1);
		// �����ѵ�¼
		return TRUE;
	}

	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	// ��ȡ��ά��ֵ
	CHAR qrCodeUrl[0x100] = { 0 };
	DWORD pCode = 0;
	while (TRUE) {
		pCode = *((DWORD *)(winAddress + WX_QRCODE_URL_CODE));
		if (pCode && strlen((CHAR*)pCode) > 0) {
			sprintf_s(qrCodeUrl, "http://weixin.qq.com/x/%s", (CHAR*)pCode);
			if (sendQrcode == TRUE) {
				Send::SendLoginQrcode(0, qrCodeUrl);  // ���ͻط�������
			}
			break;
		}
	}

	// ����δ��¼
	return FALSE;
}

/*
��ת����ά�봰��
*/
void GotoQrCode()
{
	return;
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	// ����Ƿ��Ѿ���½
	if (CheckLoginStatus() == 1) {
		return;  // �Ѿ���½��������ת
	}
	DWORD dwCallAddr1 = winAddress + GOTO_QRCODE_1;
	DWORD dwCallAddr2 = winAddress + GOTO_QRCODE_2;

	__asm {
		pushad
		call dwCallAddr1;
		mov ecx, eax;
		call dwCallAddr2;
		popad
	}
}