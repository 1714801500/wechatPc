#pragma once
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

#define random(x) (rand()%x)

using namespace std;

// ��ȡDLL·��
void GetDllPath(char *dllPath, int dllSize);

// д���ڴ�
LPVOID writeMemory(HANDLE hProcess, char *path);

// ����DLL
INT_PTR loadLibrary(HANDLE hProcess, LPVOID dllAddress);

// ���ҽ���ID
DWORD ProcessNameFindPid(LPCSTR processName);

// UTF-8��GB2312��ת��
char* UTF8ToGB2312(char* utf8);
// GB2312��UTF-8��ת��
char* GB2312ToUTF8(char* gb2312);
// ����ת��
char* UnicodeToUtf8(wchar_t* unicode);
// ����ת��
wchar_t* UTF8ToUnicode(char* str);
// char*תwchar*
wchar_t *CharToWChar(char *cStr);
// wchar*תchar*
CHAR *WcharToChar(wchar_t *wStr);
// ��ȡ�����
int GetRand(int number);
// ��ȡʱ��� - ���е�����
long long CreateTimestamp();
// ����Ψһֵ
string CreateUniqueId();
// ��ȡ��ǰִ���ļ���Ŀ¼
string GetExePath(void);
// ��ȡ�����ļ�
string ReadConfig(string key);