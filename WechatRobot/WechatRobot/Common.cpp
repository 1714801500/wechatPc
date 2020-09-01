#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "Md5.h"
#include "ReadConfig.h"
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include <TlHelp32.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <atlstr.h>

using namespace std;

ConfigStruct Setting;

LPVOID writeMemory(HANDLE hProcess, char *path)
{
	// �����ڴ�
	LPVOID dllAddress = VirtualAllocEx(hProcess, NULL, strlen(path), MEM_COMMIT, PAGE_READWRITE);
	if (dllAddress == NULL) {
		return NULL;
	}

	// д���ڴ�
	if (WriteProcessMemory(hProcess, dllAddress, path, strlen(path), NULL) == 0) {
		return NULL;
	}
	return dllAddress;
}

/*
����DLL
*/
INT_PTR loadLibrary(HANDLE hProcess, LPVOID dllAddress)
{
	HMODULE k32 = GetModuleHandle(L"Kernel32.dll");
	LPVOID loadAddress = GetProcAddress(k32, "LoadLibraryA");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadAddress, dllAddress, 0, NULL);
	if (hThread == NULL) {
		return FALSE;
	}
	return TRUE;
}

/*
���ҽ���ID
*/
DWORD ProcessNameFindPid(LPCSTR processName)
{
	// ��ȡ���̿���
	HANDLE processAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// ��������
	PROCESSENTRY32 processInfo = { 0 };
	processInfo.dwSize = sizeof(PROCESSENTRY32);

	do {
		if (strcmp(processName, (char *)processInfo.szExeFile) == 0) {
			return processInfo.th32ProcessID;
		}
	} while (Process32Next(processAll, &processInfo));

	return NULL;
}
// ��ȡDLL·��
void GetDllPath(char *dllPath, int dllSize)
{
	// ��ȡ��ǰĿ¼
	string strUrl = GetExePath();

	sprintf_s(dllPath, dllSize, "%s\\%s", strUrl.c_str(), DLL_NAME);
	if (_access(dllPath, 0) == -1) {
		return ;
	}
	return ;
}

//UTF-8��GB2312��ת��
char* UTF8ToGB2312(char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312��UTF-8��ת��
char* GB2312ToUTF8(char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

/*
����ת��
 */
char* UnicodeToUtf8(wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = (char*)malloc(len + 1);
	if (szUtf8 != 0) {
		memset(szUtf8, 0, len + 1);
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

/*
����ת��
*/
wchar_t* UTF8ToUnicode(char* str)
{
	int    textlen = 0;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (result != 0)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	}
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

// char*תwchar*
wchar_t *CharToWChar(char *cStr)
{
	size_t len = strlen(cStr) + 1;
	size_t converted = 0;
	wchar_t *wStr;
	wStr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, wStr, len, cStr, _TRUNCATE);
	return wStr;
}

// wchar*תchar*
CHAR *WcharToChar(wchar_t *wStr)
{
	size_t len = wcslen(wStr) + 1;
	size_t converted = 0;
	char *cStr;
	cStr = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, cStr, len, wStr, _TRUNCATE);
	return cStr;
}

/*
��ȡ�����
*/
int GetRand(int number)
{
	srand((unsigned int)time(NULL));//ͨ��ϵͳʱ���ʼ�����������//������������������,��ÿ��rand()��������ʼֵ��һ��.
	int n = random(number) % 181 + 20;// rand()Ϊ���������, ����[0,int�����ֵ)֮���������, ��ֵ��ϵͳ��ͬ����ͬ, һ��Ϊ2147483648, ����Ϊ����// ͨ��rand()%181+20ת��Ϊ[20,200]֮������� (ע:rand()%181����[0,180]֮�������)
	return n; //�����������
}

/*
��ȡ��ǰʱ��� - ���е�����
*/
long long CreateTimestamp()
{
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}

// ����Ψһֵ
string CreateUniqueId()
{
	long long intTimestamp = CreateTimestamp();
	char str[70] = {0};
	sprintf_s(str, sizeof(str), "%lld%d", intTimestamp, GetRand(8));
	

	MD5 *md5 = new MD5();
	string res = md5->encode(str);
	// ת��д
	transform(res.begin(), res.end(), res.begin(), ::toupper);
	return res;
	//sprintf_s(uniqueId, 33, "%s", res.c_str());
}
// ��ȡ��ǰִ���ļ���Ŀ¼
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�  
	string path = szFilePath;

	return path;
}
// ��ȡ�����ļ�
string ReadConfig(string key)
{
	string value = "";
	
	if (Setting.isRead == FALSE) {
		string strUrl = GetExePath();
		CHAR configPath[MAX_PATH] = { 0 };
		sprintf_s(configPath, "%s\\%s", strUrl.c_str(), CONFIG_PATH);
		if (_access(configPath, 0) == -1) {
			MessageBox(NULL, L"�����ļ������ڣ�", L"��ܰ��ʾ��", NULL);
			return value;
		}
		Config configSettings(configPath);
		Setting.ConfigSetting = configSettings;
		Setting.isRead = TRUE;
	}

	value = Setting.ConfigSetting.Read(key, value);

	return value;
}