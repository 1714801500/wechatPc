#pragma once
#include "stdafx.h"

// �˳���¼
void DoLogout();
// ��ȡ��¼״̬
int CheckLoginStatus();
// ��ȡ��¼״̬�͵�¼��ά��
bool SendLoginStatus(bool sendQrcode = TRUE);
// ��ת����ά�����
void GotoQrCode();