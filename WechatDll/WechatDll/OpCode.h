#pragma once

class OpCode
{
public:
	/** @var int ��Ӧָ�� */
	static const int OPCODE_READY = 0x0;  // ws�ͻ���׼�����
	static const int OPCODE_SUCCESS = 0x1;  // �����ɹ�
	static const int OPCODE_FAILURE = 0x2;  // ����ʧ��
	/** @var int ҵ��ָ�� */
	static const int OPCODE_WECHAT_OPEN = 0x10;  // �¿�һ��΢�ſͻ���
	static const int OPCODE_WECHAT_QUIT = 0x11;  // �˳�һ��΢��
	static const int OPCODE_WECHAT_GET_LOGIN_STATUS = 0x12;  // ��ȡ��¼״̬
	static const int OPCODE_MESSAGE_SEND_TEXT = 0x20;  // �����ı���Ϣ
	static const int OPCODE_MESSAGE_SEND_IMAGE = 0x21;  // ����ͼƬ��Ϣ
	static const int OPCODE_MESSAGE_SEND_FILE = 0x22;  // ���͸�����Ϣ
	static const int OPCODE_MESSAGE_SEND_CARD = 0x23;  // ������Ƭ��Ϣ
	static const int OPCODE_MESSAGE_SEND_XML = 0x24;  // ����xml��Ϣ
	static const int OPCODE_FRIEND_ADD = 0x30;  // wxid�Ӻ���
	static const int OPCODE_FRIEND_ADD_FROM_V1 = 0x31;  // v1�Ӻ���
	static const int OPCODE_FRIEND_DELETE = 0x32;  // ɾ������
	static const int OPCODE_FRIEND_VERIFY = 0x33;  // v1+v2ͬ���������
	static const int OPCODE_FRIEND_LIST = 0x34;  // �����б�
	static const int OPCODE_FRIEND_REMARK = 0x35;  // �޸ı�ע
	static const int OPCODE_ROOM_CREATE = 0x40;  // ����Ⱥ��
	static const int OPCODE_ROOM_EDIT_NAME = 0x41;  // �޸�Ⱥ����
	static const int OPCODE_ROOM_ANNOUNCEMENT = 0x42;  // ����Ⱥ����
	static const int OPCODE_ROOM_MEMBER_LIST = 0x43;  // ��ȡȺ��Ա�б�
	static const int OPCODE_ROOM_ADD_MEMBER = 0x44;  // ��������Ⱥ
	static const int OPCODE_ROOM_DELETE_MEMBER = 0x45;  // ɾ��Ⱥ��Ա
	static const int OPCODE_ROOM_AT_MEMBER = 0x46;  // ����Ⱥ��Ա
	static const int OPCODE_ROOM_QUIT = 0x47;  // �˳�Ⱥ��
	static const int OPCODE_TRANSFER_RECV = 0x50;  // �տ�
	static const int OPCODE_WECHAT_QRCODE = 0x90;  // ���ص�¼��ά���ַ
	static const int OPCODE_LOGIN_INFO = 0x91;  // ���ص�ǰ��¼��΢�ź���Ϣ
	static const int OPCODE_MESSAGE_RECEIVE = 0x92;  // ���ؽ��յ�΢����Ϣ
};