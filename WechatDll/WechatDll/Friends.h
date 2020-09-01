#pragma once
#include "Package.h"

// ͬ��Ӻ��ѵĲ�����v1
struct v1Info
{
	int fill = 0;
	wchar_t *v1;
	int v1Len;
	int maxV1Len;
	char fill2[0x41C] = { 0 };
	DWORD v2 = 0;
};
// ͬ��Ӻ��ѵĲ�����v2
struct v2Info
{
	char fill[0x24C] = {0};
	DWORD fille3 = 0x25;
	char fill4[0x40] = { 0 };
	wchar_t *v2;
	int v2Len;
	int maxV2Len;
	char fill2[0x8] = { 0 };
};
// �Ӻ��ѵĲ���
struct wechatText
{
	wchar_t *pStr;
	int strLen;
	int strMaxLen;
};
// ��������
struct userInfo
{
	wchar_t wxid[0x100] = { 0 };
	wchar_t username[0x100] = { 0 };
	wchar_t nickname[0x100] = { 0 };
	wchar_t remark[0x200] = { 0 };
	wchar_t avatar[0x200] = { 0 };
	wchar_t v1[0x100] = { 0 };
};

//ɾ�����ѽṹ��
struct DeleteUserStructWxid
{
	//���͵��ı���Ϣָ��
	wchar_t* pWxid;
	//�ַ�������
	DWORD length;
	//�ַ�����󳤶�
	DWORD maxLength;

	//��������ռλ����
	DWORD fill1;
	DWORD fill2;
};
//ͨ���Ƽ���Ƭ�Ӻ��ѽṹ��
struct AddUserFromCardStruct
{
	wchar_t* pStr;
	int strLen;
	int strMaxLen;
};
// ���ú��ѱ�ע�ṹ
struct SetRemarkStruct
{
	wchar_t *remark;
	int remarkLen;
	int maxRemarkLen;
};
// ��ѯ���ݿ�����б�
void selectFriendList();
// ���������б����
void ListFriendsList();
// ���ͺ����б�
void SendList(Package *package = NULL, int page = 1, int pageSize = 0, int total = 0);
// ͬ��Ӻ�������
void VerifyFriend(PWCHAR v1, PWCHAR v2);
// �Ӻ���
void AddFriend(PWCHAR wxid, PWCHAR message);
// ��ȡ��������
userInfo GetUserInfoByWxid(wchar_t* userwxid);
// ɾ������
void DeleteUser(wchar_t* wxid);
// ͨ���Ƽ���Ƭ��Ӻ���
void AddUserFromCard(wchar_t* v1, wchar_t* msg);
// ���ú��ѱ�ע
void SetRemark(wchar_t *wxid, wchar_t *remark);