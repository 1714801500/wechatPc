#pragma once

// �տ�ṹ��
struct CllectMoneyStruct
{
	wchar_t* ptransferid;
	int transferidLen;
	int transferidMaxLen;
	char full[0x8] = { 0 };
	wchar_t* pwxid;
	int wxidLen;
	int wxidMaxLen;
	char full2[0x8] = { 0 };
};

using namespace std;

// �տ�
void CollectMoney(wchar_t* wxid, wchar_t* transferid);
// �Զ��տ�
void AutoCollectMoney(wchar_t* wxid, wstring msg);