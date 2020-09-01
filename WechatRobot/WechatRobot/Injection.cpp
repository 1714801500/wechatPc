// Injection.cpp : ����ע�뷽����
//
#include "stdafx.h"
#include "Injection.h"
#include "Config.h"
#include "Common.h"
#include <direct.h>
#include <stdio.h>


/*
ע��DLL
*/
INT_PTR InjectDll(DWORD pid)
{
	// ��ȡ΢�Ž��̵�PID
	if(pid == 0) {
		DWORD pid = ProcessNameFindPid(WECHAT_PROCESS_NAME);
		if (pid == 0) {
			return FALSE;
		}
	}
	
	// ��鵽�Ѿ�ע��Ͳ�Ҫ��ע����
	INT_PTR result = CheckInject(pid);
	if (result == TRUE || result == NULL) {
		// �򿪽���
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == NULL) {
			return FALSE;
		}
		// ��ȡDLL·��
		CHAR dllPath[MAX_PATH] = { 0 };
		GetDllPath(dllPath, MAX_PATH);

		if (strlen(dllPath) <= 0) {
			MessageBox(NULL, L"DLL�ļ������ڣ�", L"��ܰ��ʾ��", 0);
			return FALSE;
		}
		// д���ڴ�
		LPVOID dllAddress = writeMemory(hProcess, dllPath);
		if (dllAddress == NULL) {
			return FALSE;
		}
		// ����DLL
		if (loadLibrary(hProcess, dllAddress) == FALSE) {
			return FALSE;
		}
		CloseHandle(hProcess);
	}
	
	return TRUE;
}

/*
����Ƿ��Ѿ�ע��
*/
INT_PTR CheckInject(DWORD dwProcessid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	// ��ʼ��ģ����Ϣ�ṹ��
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// ����ģ����� 1 �������� 2 ����ID
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessid);
	// ��������Ч�ͷ���false
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	// ͨ��ģ����վ����ȡ��һ��ģ�����Ϣ
	if (!Module32First(hModuleSnap, &me32)) {
		//��ȡʧ����رվ��
		CloseHandle(hModuleSnap);
		return NULL;
	}
	do {
		if (wcscmp(me32.szModule, _T(DLL_NAME)) == 0) {
			return FALSE;
		}
	} while (Module32Next(hModuleSnap, &me32));
	return TRUE;
}

/*
ж��ע��
*/
INT_PTR UnjectDll()
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hProcess, &pe32) != TRUE) {
		return FALSE;
	}

	HMODULE Kernel32 = GetModuleHandle(L"Kernel32.dll");
	if (Kernel32 == 0) {
		return FALSE;
	}
	int wechatCount = 0;
	do {
		if (wcscmp(pe32.szExeFile, _T(WECHAT_PROCESS_NAME)) != 0) {
			continue;
		}
		DWORD dwPid = pe32.th32ProcessID;

		//����ģ��
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

		if (hSnap == INVALID_HANDLE_VALUE) {
			continue;
		}
		MODULEENTRY32 ME32 = { 0 };
		ME32.dwSize = sizeof(MODULEENTRY32);
		BOOL isNext = Module32First(hSnap, &ME32);
		BOOL flag = FALSE;
		while (isNext) {
			if (wcscmp(ME32.szModule, _T(DLL_NAME)) == 0) {
				flag = TRUE;
				break;
			}
			isNext = Module32Next(hSnap, &ME32);
		}
		if (flag != TRUE) {
			CloseHandle(hSnap);
			continue;
		}
		//��Ŀ�����
		HANDLE hPro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if (!hPro) {
			CloseHandle(hSnap);
			continue;
		}
		//��ȡFreeLibrary������ַ
		FARPROC pFun = GetProcAddress(Kernel32, "FreeLibraryA");
		//����Զ���߳�ִ��FreeLibrary
		HANDLE hThread = CreateRemoteThread(hPro, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, ME32.modBaseAddr, 0, NULL);
		if (hThread != NULL) {
			//WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		CloseHandle(hPro);
		CloseHandle(hSnap);
	} while (Process32Next(hProcess, &pe32));

	return TRUE;
}


