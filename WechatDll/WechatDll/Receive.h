#pragma once
#include "Pjson.h"
#include "Package.h"

class Receive
{
public:
	// �������˳���Ϣ
	static bool SendLogout(Package *package = NULL);

protected:
	// ����
	// ��ȡ��¼����
	bool GetLoginInfo(Package *package);
	// ��ȡ��¼״̬
	bool GetLoginStatus(Package *package);
	// ��ȡ��¼״̬
	bool Logout(Package *package);
	// �����ı���Ϣ
	bool MessageSendText(Package *package);
	// ����ͼƬ��Ϣ
	bool MessageSendImage(Package *package);
	// ���͸�����Ϣ
	bool MessageSendFile(Package *package);
	// ������Ƭ��Ϣ
	bool MessageSendCard(Package *package);
	// ����xml��Ϣ
	bool MessageSendXml(Package *package);
	// wxid�Ӻ���
	bool FriendAdd(Package *package);
	// v1�Ӻ���
	bool FriendAddFromV1(Package *package);
	// ɾ������
	bool FriendDelete(Package *package);
	// v1+v2ͬ���������
	bool FriendVerify(Package *package);
	// �����б�
	bool FriendList(Package *package);
	// ���ú��ѱ�ע
	bool FriendRemark(Package *package);
	// ����Ⱥ��
	bool RoomCreate(Package *package);
	// �޸�Ⱥ����
	bool RoomEditName(Package *package);
	// ����Ⱥ����
	bool RoomAnnouncement(Package *package);
	// ��ȡȺ��Ա�б�
	bool RoomMemberList(Package *package);
	// ��������Ⱥ
	bool RoomAddMember(Package *package);
	// ɾ��Ⱥ��Ա
	bool RoomDeleteMember(Package *package);
	// ����Ⱥ��Ա
	bool RoomAtMember(Package *package);
	// �˳�Ⱥ��
	bool RoomQuit(Package *package);
	// �տ�
	bool TransferRecv(Package *package);
public:
	int Handle(Package *package);
};