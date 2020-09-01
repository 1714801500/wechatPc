// Injection.cpp : ����΢�ŷ���ܿء�
//
#include "stdafx.h"
#include "WechatMultiOpen.h"
#include "WechatService.h"
#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <TlHelp32.h>
#include "WechatRobot.h"

INT_PTR StartWechat()
{
	char dllPath[MAX_PATH] = {0};
	GetDllPath(dllPath, MAX_PATH);

	if (strlen(dllPath) <= 0) {
		MessageBox(NULL, L"DLL�ļ������ڣ�", L"��ܰ��ʾ��", 0);
		return FALSE;
	}
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Tencent\\WeChat", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
		MessageBox(NULL, L"ע����ʧ�ܣ�", L"��ܰ��ʾ��", 0);
		return FALSE;
	}
	CHAR szProductType[MAX_PATH] = {};
	DWORD dwBufLen = MAX_PATH;
	if (RegQueryValueEx(hKey, L"InstallPath", NULL, NULL, (LPBYTE)szProductType, &dwBufLen) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		MessageBox(NULL, L"ע����ѯʧ�ܣ�", L"��ܰ��ʾ��", 0);
		return FALSE;
	}

	RegCloseKey(hKey);
	wchar_t weChatexe[MAX_PATH] = {0};
	swprintf_s(weChatexe, L"%s\\%s", (wchar_t*)szProductType, _T(WECHAT_PROCESS_NAME));

	if (_access(WcharToChar(weChatexe), 0) == -1)
	{
		MessageBox(NULL, L"WeChat.exe������ע���װĿ¼��", L"��ܰ��ʾ��", 0);
		return FALSE;
	}
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	// ����΢��
	CreateProcess(weChatexe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	
	ResumeThread(pi.hThread);

	// �������Ƿ������������ȴ�1��
	WaitForSingleObject(pi.hProcess, 1000);

	// �࿪����
	PatchWeChat();
	// ��ʼע��
	InjectDll(pi.dwProcessId);
	
	// ���ؽ���ID
	return pi.dwProcessId;
}

/**
 * ��������΢�Ž���
 * @param
 * @return BOOL
 */
INT_PTR CloseAllWeChat()
{
	HANDLE ProcesssAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 proessInfo = { 0 };
	proessInfo.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		if (wcscmp(_T(WECHAT_PROCESS_NAME), proessInfo.szExeFile) == 0) {
			CloseProcess(proessInfo.th32ProcessID);  // �رս���
		}
	} while (Process32Next(ProcesssAll, &proessInfo));

	return TRUE;
}

/**
 * ����ָ������
 * @param
 * @return BOOL
 */
INT_PTR CloseProcess(int processId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (hProcess != NULL) {
		TerminateProcess(hProcess, 0);
	}

	return TRUE;
}
