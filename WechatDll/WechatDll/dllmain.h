#pragma once
#include "Config.h"

// ���津���ص�
INT_PTR CALLBACK Dlgproc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
// ��ʾ����
DWORD WINAPI ShowDialog(HMODULE hModule);
// ���΢��
DWORD WINAPI Monitor(HMODULE hModule);
// ���WebSocket
DWORD WINAPI WebSocketClientMonitor(HMODULE hModule);