#pragma once

#include "Config.h"
// ��ȡ���̾��
#include <TlHelp32.h>


// DLLע��
INT_PTR InjectDll(DWORD pid);

// ����Ƿ��Ѿ�ע��
INT_PTR CheckInject(DWORD dwProcessid);

// ж��DLLע��
INT_PTR UnjectDll();

