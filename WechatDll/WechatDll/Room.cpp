#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "WechatOffset.h"
#include "Common.h"
#include "Struct.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <tchar.h> 
#include <sstream>
#include <iomanip>
#include <strstream>
#include <list>
#include <iostream>
#include <map>
#include "Friends.h"
#include "Room.h"



/*==================================��ȡȺ��Ա�б�ʼ=======================================*/
void GetRoomUserList(wchar_t* chatroomwxid)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	//1.�Ȼ�ȡȺ��Ա��΢��ID
	
	//׼��������
	DWORD dwWxidArr = 0;	//����΢��ID���ݵĵ�ַ
	char buff[0x164] = { 0 };
	char userListBuff[0x174] = { 0 };

	//��������
	GetRoomUserwxStr pWxid = { 0 };
	pWxid.pStr = chatroomwxid;
	pWxid.strLen = wcslen(chatroomwxid);
	pWxid.strMaxLen = wcslen(chatroomwxid) * 2;
	char * asmWxid = (char *)&pWxid.pStr;

	//����call
	DWORD dwCall1 = winAddress + WX_GET_ROOM_USER_WXID_CALL1;
	DWORD dwCall2 = winAddress + WX_GET_ROOM_USER_WXID_CALL2;
	DWORD dwCall3 = winAddress + WX_GET_ROOM_USER_WXID_CALL3;
	DWORD dwCall4 = winAddress + WX_GET_ROOM_USER_WXID_CALL4;

	//��ȡȺ��Ա
	__asm {
		lea ecx, buff[16]
		call dwCall1
		lea eax, buff[16]
		push eax
		mov ebx, asmWxid
		push ebx
		call dwCall2
		mov ecx, eax
		call dwCall3
		lea eax, buff
		push eax
		lea ecx, buff[16]
		call dwCall4
		mov dwWxidArr, eax
	}

	//�õ�΢��ID
	wchar_t test[0x100] = { 0 };
	wchar_t tempWxid[0x100] = { 0 };
	DWORD userList = *((DWORD *)dwWxidArr);
	DWORD testTmp = dwWxidArr + 0xB0;
	int Len = *((int *)testTmp);

	for (int i = 0; i < Len; i++)
	{
		DWORD temWxidAdd = userList + (i * 20);
		swprintf_s(tempWxid, L"%s", (wchar_t*)*((LPVOID *)temWxidAdd));

		//2.��ͨ��΢��ID��ȡȺ��Ա��Ϣ
		userInfo user = GetUserInfoByWxid(tempWxid);
		MessageBox(NULL, user.wxid, L"��", NULL);
		// user=�û�����ṹ
	}
}
/*==================================��ȡȺ��Ա�б����=======================================*/

/*==================================���Ⱥ��Ա��ʼ=======================================*/
void AddRoomUser(wchar_t* chatroomwxid, wchar_t* wxid)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	DWORD dwCallAddr1 = winAddress + WX_ADD_ROOM_MEMBER_CALL1;
	DWORD dwCallAddr2 = winAddress + WX_ADD_ROOM_MEMBER_CALL2;
	DWORD dwCallAddr3 = winAddress + WX_ADD_ROOM_MEMBER_CALL3;
	DWORD dwCallAddr4 = winAddress + WX_ADD_ROOM_MEMBER_CALL4;

	DWORD dwParam = winAddress + WX_ADD_ROOM_MEMBER_PARAM1;
	DWORD dwParam2 = winAddress + WX_ADD_ROOM_MEMBER_PARAM2;
	
	//ȺID�ṹ��
	struct tempChatRoom
	{
		char fill2[0x8] = { 0 };
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char fill[0x8] = { 0 };
	};


	//������Ҫ������
	AddRoomUserWxid wxidStruct = { 0 };
	wxidStruct.str = wxid;
	wxidStruct.strLen = wcslen(wxid) * 2;
	wxidStruct.maxLen = wcslen(wxid) * 4;

	tempChatRoom chatroomStruct = { 0 };
	chatroomStruct.str = chatroomwxid;
	chatroomStruct.strLen = wcslen(chatroomwxid) * 2;
	chatroomStruct.maxLen = wcslen(chatroomwxid) * 4;

	char wxidBuff[0xC] = { 0 };
	char tempWxid[0x14] = { 0 };
	char tempBuff[0x14] = { 0 };

	char* pWxid = (char*)&wxidStruct.str;
	char* pChatRoom = (char*)&chatroomStruct.fill2;

	__asm {
		pushad;
		lea esi, wxidBuff;
		mov ecx, esi;
		mov eax, pWxid;
		push eax;
		call dwCallAddr1;
		push 0;
		push dwParam;
		lea ecx, tempWxid;
		call dwCallAddr2;
		sub esp, 0x14;
		mov ecx, pChatRoom;
		mov eax, esp;
		push eax;
		call dwCallAddr3;
		push esi;
		mov ecx, dwParam2;
		call dwCallAddr4;
		popad;
	}
}
/*==================================���Ⱥ��Ա����=======================================*/

/*==================================����Ⱥ���濪ʼ=======================================*/
void SetRoomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	//��ȡWeChatWin�Ļ�ַ
	DWORD callAdrress = winAddress + WX_SET_ROOM_ANNOUNCEMENT;
	//��װ����
	CHAR bufferA[0xD0] = { 0 };
	DWORD* bufA = (DWORD*)& bufferA;

	CHAR buffer[0xD0] = { 0 };
	DWORD* buf = (DWORD*)& buffer;

	buf[0] = (DWORD)chatroomwxid;
	buf[1] = wcslen(chatroomwxid);
	buf[2] = wcslen(chatroomwxid) * 2;
	buf[3] = 0;
	buf[4] = 0;

	buf[0 + 5] = (DWORD)Announcement;
	buf[1 + 5] = wcslen(Announcement);
	buf[2 + 5] = wcslen(Announcement) * 2;
	buf[3 + 5] = 0;
	buf[4 + 5] = 0;

	bufA[0] = (DWORD)& buffer;
	bufA[1] = bufA[0] + 0x60;
	bufA[2] = bufA[0] + 0x60;

	DWORD r_esp = 0;
	__asm
	{
		//�����ջ�Ĵ���
		mov r_esp, esp

		//654B85B1 | .  8D85 30FFFFFF lea eax, [local.52]    //  lea eax,dword ptr ss:[ebp-0xD0]
		//654B85B7 | .  50            push eax
		//654B85B8 | .E8 43FF2000   call WeChatWi.0F411450;  ����Ⱥ����
		lea eax, bufferA
		push eax
		call callAdrress

		//�ָ���ջ�Ĵ���
		mov eax, r_esp
		mov esp, eax
	}
}
/*==================================����Ⱥ�������=======================================*/



/*==================================�˳�Ⱥ�Ŀ�ʼ=======================================*/
void QuitRoom(wchar_t* chatroomwxid)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();
	DWORD dwCallAddr = winAddress + WX_QUIT_ROOM;

	//��������
	QuiteRoomStructWxid structWxid = { 0 };
	structWxid.pWxid = chatroomwxid;
	structWxid.length = wcslen(chatroomwxid);
	structWxid.maxLength = wcslen(chatroomwxid) * 2;

	DWORD* asmMsg = (DWORD*)&structWxid.pWxid;

	__asm {
		mov ebx, asmMsg;
		push ebx;
		call dwCallAddr;
	}
}
/*==================================�˳�Ⱥ�Ľ���=======================================*/

/*==================================�޸�Ⱥ���ƿ�ʼ=======================================*/
void SetRoomName(wchar_t* roomwxid, wchar_t* roomname)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();
	
	SetRoomNameStruct pRoomwxid = { 0 };
	pRoomwxid.pStr = roomwxid;
	pRoomwxid.strLen = wcslen(roomwxid) + 1;
	pRoomwxid.strMaxLen = (wcslen(roomwxid) + 1) * 2;

	SetRoomNameStruct pRoomname = { 0 };
	pRoomname.pStr = roomname;
	pRoomname.strLen = wcslen(roomname) + 1;
	pRoomname.strMaxLen = (wcslen(roomname) + 1) * 2;

	char* asmWxid = (char*)&pRoomwxid.pStr;
	char* asmRoomname = (char*)&pRoomname.pStr;

	DWORD dwCall1 = winAddress + WX_SET_ROOM_NAME;


	__asm
	{
		mov edx, asmRoomname;
		mov ecx, asmWxid;
		call dwCall1;
	}
}
/*==================================�޸�Ⱥ���ƽ���=======================================*/

/*==================================���Ͱ�����Ϣ��ʼ=======================================*/
void SendRoomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();
	DWORD callAddress_SendText = winAddress + SEND_MESSAGE_CALL;


	TEXT_WX wxId(chatroomid);
	wchar_t tempmsg[100] = { 0 };
	swprintf_s(tempmsg, L"@%s %s", membernickname, msg);
	TEXT_WX wxMsg(tempmsg);

	TEXT_WXID wxAtId;
	wxAtId.pWxid = memberwxid;
	wxAtId.length = wcslen(memberwxid);
	wxAtId.maxLength = wcslen(memberwxid) * 2;
	wxAtId.fill1 = 0;
	wxAtId.fill2 = 0;
	//DWORD* asmWxid = (DWORD*)& wxAtId.pWxid;

	ROOM_AT roomAt;
	roomAt.at_WxidList = (DWORD)& wxAtId.pWxid;
	roomAt.at_end1 = roomAt.at_WxidList + 5 * 4;
	roomAt.at_end2 = roomAt.at_end1;

	//����һ��������
	BYTE buff[0x81C] = { 0 };

	//ִ�л�����
	__asm
	{
		lea edx, wxId

		//���ݲ���
		push 0x1

		lea eax, roomAt
		push eax

		//΢����Ϣ����
		lea ebx, wxMsg

		push ebx
		lea ecx, buff

		//���ú���
		call callAddress_SendText

		//ƽ���ջ
		add esp, 0xC
	}
}
/*==================================���Ͱ�����Ϣ����=======================================*/


/*==================================ɾ��Ⱥ��Ա��ʼ=======================================*/
void DelRoomUser(wchar_t* roomid, wchar_t* memberwxid)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	//�õ�call�����ݵ�ַ
	DWORD dwCall1 = winAddress + WX_DEL_ROOM_USER_CALL1;
	DWORD dwCall2 = winAddress + WX_DEL_ROOM_USER_CALL2;
	DWORD dwCall3 = winAddress + WX_DEL_ROOM_USER_CALL3;

	//��װȺID�ṹ��
	DelRoomIdStruct roomiddata;
	roomiddata.roomid = roomid;
	roomiddata.roomidLen = wcslen(roomid) + 1;
	roomiddata.roomidMaxLen = (wcslen(roomid) + 1) * 2;

	//��װ΢��ID�ṹ��
	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)memberwxid;
	dwBuff[1] = wcslen(memberwxid);
	dwBuff[2] = wcslen(memberwxid) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;

	wchar_t datatbuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&datatbuffer;
	dwDatabuf[0] = (DWORD)& wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x14;
	dwDatabuf[2] = dwDatabuf[0] + 0x14;

	//����ɾ��Ⱥ��Աcall
	__asm
	{
		pushad;
		sub esp, 0x14;
		lea eax, roomiddata.roomid;
		mov ecx, esp;
		push eax;
		call dwCall1;
		lea edi, datatbuffer;
		push edi;
		call dwCall2;
		mov ecx, eax;
		call dwCall3;
		popad;
	}
}
/*==================================ɾ��Ⱥ��Ա����=======================================*/


/*==================================����Ⱥ�Ŀ�ʼ=======================================*/
void CreateRoom(wchar_t *wxid1, wchar_t *wxid2)
{
	// ��ȡ΢�Ż�ַ
	DWORD winAddress = GetWechatWinAddress();

	//����call
	DWORD dwParam = winAddress + WX_CREATE_ROOM_PARAM;
	DWORD dwCall = winAddress + WX_CREATE_ROOM_CALL;

	//��װ΢��ID�ṹ��
	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)wxid1;
	dwBuff[1] = wcslen(wxid1);
	dwBuff[2] = wcslen(wxid1) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;
	dwBuff[5] = (DWORD)wxid2;
	dwBuff[6] = wcslen(wxid2);
	dwBuff[7] = wcslen(wxid2) * 2;
	dwBuff[8] = 0;
	dwBuff[9] = 0;

	wchar_t databuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&databuffer;
	dwDatabuf[0] = (DWORD)&wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x28;
	dwDatabuf[2] = dwDatabuf[0] + 0x28;
	
	//��ȡȺ��Ա
	__asm {
		sub esp, 0x14
		mov ecx, dwParam
		lea esi, databuffer
		push esi
		call dwCall
	}
}
/*==================================����Ⱥ�Ľ���=======================================*/
