#pragma once
#include "OpCode.h"
#include "PJson.h"

using namespace rapidjson;


class Package
{
	// Construction
public:
	Package();	// standard constructor

	rapidjson::Document json;

private:
	char *appId = NULL;  // �ͻ��˵�Ӧ��ID���ַ������32λ
	char *appKey = NULL;  // �ͻ��˵�Ӧ��KEY���ַ������32λ
	
	char *uniqueId = NULL;  // ���ݰ�ΨһID���ַ������32λ
	char *timestamp = NULL;  // ����ʱ�䣬���뼶ʱ������ַ���,13λ
	char *wechatId = NULL;  // ����ָ���Ŀ��΢�ſͻ���ID��ÿ��һ��΢�ſͻ��˴���һ��ΨһID����Ŀ��ͻ�����Ϊnull���ַ�����null
	int opCode = 0;  // ����ָ�16������ֵ

	rapidjson::Value body;  // ҵ���������
public:
	/*==============================================================*/
	// ���
	void SetConText(char *jsonStr);
	// ���
	rapidjson::StringBuffer GetConText();
	// �����ıش�����
	bool Check();
	/*==============================================================*/
	// ��ȡ�ͻ��˵�Ӧ��ID
	char *GetAppId();
	// ���ÿͻ��˵�Ӧ��ID
	void SetAppId(char *appId);
	// ��ȡ�ͻ��˵�Ӧ��KEY
	char *GetAppKey();
	// ���ÿͻ��˵�Ӧ��KEY
	void SetAppKey(char *appKey);
	/*==============================================================*/
	// ��ȡ���ݰ�ΨһID
	char *GetUniqueId();
	// �������ݰ�Ψһ
	void SetUniqueId(char *uniqueId);
	/*==============================================================*/
	// ��ȡ����ʱ��
	char *GetTimestamp();
	// ���÷���ʱ��
	void SetTimestamp(char *timestamp);
	/*==============================================================*/
	// ��ȡ����ָ���Ŀ��΢�ſͻ���ID
	char *GetWechatId();
	// ���ý���ָ���Ŀ��΢�ſͻ���ID
	void SetWechatId(char *wechatId);
	/*==============================================================*/
	// ��ȡ����ָ��
	int GetOpCode();
	// ���ò���ָ��
	void SetOpCode(int opCode);
	/*==============================================================*/
	// ��ȡҵ���������
	rapidjson::Value *GetBody();
	// ����ҵ���������
	void SetBody(rapidjson::Value &body);
	/*==============================================================*/
};