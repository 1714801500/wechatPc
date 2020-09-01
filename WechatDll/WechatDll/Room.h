#pragma once
#include <stdio.h>
#include <string>
using namespace std;

//Ⱥ�ӳ�Ա΢��ID�Ľṹ��
struct AddRoomUserWxid
{
	wchar_t* str;
	int strLen = 0;
	int maxLen = 0;
	char file[0x8] = { 0 };
};
// ��ȡȺ��Ա��΢��ID
struct GetRoomUserwxStr
{
	wchar_t * pStr;
	int strLen;
	int strMaxLen;
	int fill = 0;
	int fill2 = 0;
};
// �˳�Ⱥ�Ľṹ
struct QuiteRoomStructWxid
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
// �޸�Ⱥ���ƽṹ
struct SetRoomNameStruct
{
	wchar_t* pStr;
	int strLen;
	int strMaxLen;
	char full[0x8] = { 0 };
};
// ɾ��Ⱥ��Ա�ṹ
struct DelRoomIdStruct
{
	wchar_t* roomid;
	int roomidLen;
	int roomidMaxLen;
	int full = 0;
	int full2 = 0;
};
//���Ͱ�����Ϣ��Ҫ�����ݽṹ
class TEXT_WX
{
public:
	wchar_t* pWxid = nullptr;
	DWORD length = 0;
	DWORD maxLength = 0;
	DWORD fill1 = 0;
	DWORD fill2 = 0;
	wchar_t wxid[1024] = { 0 };

	TEXT_WX(wstring wsWxid)
	{
		const wchar_t* temp = wsWxid.c_str();
		wmemcpy(wxid, temp, wsWxid.length());
		length = wsWxid.length();
		maxLength = wsWxid.capacity();
		fill1 = 0;
		fill2 = 0;
		pWxid = wxid;
	}
};
class ROOM_AT
{
public:
	DWORD at_WxidList = 0;
	DWORD at_end1 = 0;
	DWORD at_end2 = 0;
};
class  TEXT_WXID
{
public:
	wchar_t* pWxid = nullptr;
	DWORD length = 0;
	DWORD maxLength = 0;
	DWORD fill1 = 0;
	DWORD fill2 = 0;
};


// ��ȡȺ��Ա�б�
void GetRoomUserList(wchar_t* chatroomwxid);
// ���Ⱥ��Ա
void AddRoomUser(wchar_t* chatroomwxid, wchar_t* wxid);
// ����Ⱥ����
void SetRoomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement);
// �˳�Ⱥ��
void QuitRoom(wchar_t* chatroomwxid);
// �޸�Ⱥ����
void SetRoomName(wchar_t* roomwxid, wchar_t* roomname);
// ���Ͱ�����Ϣ
void SendRoomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg);
// ɾ��Ⱥ��Ա
void DelRoomUser(wchar_t* roomid, wchar_t* memberwxid);
// ����Ⱥ��
void CreateRoom(wchar_t *wxid1, wchar_t *wxid2);