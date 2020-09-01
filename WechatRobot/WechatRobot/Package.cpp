#include "stdafx.h"
#include "Common.h"
#include "PJson.h"
#include "OpCode.h"
#include "Package.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>

using namespace rapidjson;
using namespace std;

/** @var array ����Ĳ���ָ�� */
int allowOpCode[] = {
	/** @var int ��Ӧָ�� */
	0x0,  // ws�ͻ���׼�����
	0x1,  // �����ɹ�
	0x2,  // ����ʧ��
	/** @var int ҵ��ָ�� */
	0x10,  // �¿�һ��΢�ſͻ���
	0x11,  // �˳�һ��΢��
	0x12,  // ��ȡ��¼״̬
	0x20,  // �����ı���Ϣ
	0x21,  // ����ͼƬ��Ϣ
	0x22,  // ���͸�����Ϣ
	0x23,  // ������Ƭ��Ϣ
	0x24,  // ����xml��Ϣ
	0x30,  // wxid�Ӻ���
	0x31,  // v1�Ӻ���
	0x32,  // ɾ������
	0x33,  // v1+v2ͬ���������
	0x34,  // �����б�
	0x35,  // �޸ı�ע
	0x40,  // ����Ⱥ��
	0x41,  // �޸�Ⱥ����
	0x42,  // ����Ⱥ����
	0x43,  // ��ȡȺ��Ա�б�
	0x44,  // ��������Ⱥ
	0x45,  // ɾ��Ⱥ��Ա
	0x46,  // ����Ⱥ��Ա
	0x47,  // �˳�Ⱥ��
	0x50,  // �տ�
	0x90,  // ���ص�¼��ά���ַ
	0x91,  // ���ص�ǰ��¼��΢�ź���Ϣ
	0x92,  // ���ؽ��յ�΢����Ϣ
};

Package::Package()
{
	this->json.SetObject(); // ����ǰ��Document����Ϊһ��object��Ҳ����˵����Document��һ��Object���͵�domԪ��
}
// ���
void Package::SetConText(char *jsonStr)
{
	this->json.Parse(jsonStr);

	// �ͻ��˵�Ӧ��ID���ַ������32λ
	if (this->json["appId"].IsNull()) {
		this->SetAppId(NULL);
	}
	else {
		if (!this->json["appId"].IsString()) return;
		Value& appId = this->json["appId"];
		this->SetAppId((char*)appId.GetString());
	}

	// �ͻ��˵�Ӧ��KEY���ַ������32λ
	if (!this->json.HasMember("appKey")) return;
	if (this->json["appKey"].IsNull()) {
		this->SetAppKey(NULL);
	}
	else {
		if (!this->json["appKey"].IsString()) return;
		Value& appKey = this->json["appKey"];
		this->SetAppKey((char*)appKey.GetString());
	}

	if (!this->json.HasMember("package")) return;
	if (!this->json["package"].IsObject()) return;
	rapidjson::Value& packageObject = this->json["package"];

	// ���ݰ�ΨһID���ַ������32λ
	if (!packageObject.HasMember("uniqueId")) return;
	if (!packageObject["uniqueId"].IsString()) return;
	Value& uniqueId = packageObject["uniqueId"];
	this->SetUniqueId((char*)uniqueId.GetString());

	// ����ʱ�䣬���뼶ʱ������ַ�����13λ
	if (!packageObject.HasMember("timestamp")) return;
	if (!packageObject["timestamp"].IsString()) return;
	Value& timestamp = packageObject["timestamp"];
	this->SetTimestamp((char*)timestamp.GetString());

	// ����ָ���Ŀ��΢�ſͻ���ID��ÿ��һ��΢�ſͻ��˴���һ��ΨһID����Ŀ��ͻ�����Ϊnull���ַ�����null
	if (!packageObject.HasMember("wechatId")) return;
	if (packageObject["wechatId"].IsNull()) {
		this->SetWechatId(NULL);
	}
	else {
		if (!packageObject["wechatId"].IsString()) return;
		Value& wechatId = packageObject["wechatId"];
		this->SetWechatId((char*)wechatId.GetString());
	}
	
	// ����ָ�16������ֵ
	if (!packageObject.HasMember("opCode")) return;
	if (!packageObject["opCode"].IsInt()) return;
	Value& opCode = packageObject["opCode"];
	this->SetOpCode(opCode.GetInt());

	// ҵ���������
	if (!packageObject.HasMember("body")) return;
	Value& body = packageObject["body"];
	if (packageObject["body"].IsNull()) {
		body.SetNull();
		this->SetBody(body);
	}
	else {
		if (!packageObject["body"].IsObject()) return;
		//rapidjson::Value& bodyObject = packageObject["body"];
		this->SetBody(packageObject["body"]);
	}
}
// ���
rapidjson::StringBuffer Package::GetConText()
{
	// �ͻ��˵�Ӧ��ID���ַ������32λ
	char *appId = this->GetAppId();
	rapidjson::Value appIdString(rapidjson::kStringType);
	if (!appId) {
		string sAppId = ReadConfig("app_id");
		char cAppId[33] = {0};
		sprintf_s(cAppId, sizeof(cAppId), "%s", sAppId.c_str());
		appIdString.SetString(cAppId, 32);
	}
	else {
		appIdString.SetString(appId, 32);
	}
	if (this->json.HasMember("appId")) {
		this->json.RemoveMember("appId");
	}
	this->json.AddMember("appId", appIdString, this->json.GetAllocator());

	// �ͻ��˵�Ӧ��KEY���ַ������32λ
	char *appKey = this->GetAppKey();
	rapidjson::Value appKeyString(rapidjson::kStringType);
	if (!appKey) {
		string sAppKey = ReadConfig("app_key");
		char cAppKey[33] = { 0 };
		sprintf_s(cAppKey, sizeof(cAppKey), "%s", sAppKey.c_str());
		appKeyString.SetString(cAppKey, 32);
	}
	else {
		appKeyString.SetString(appKey, 32);
	}
	if (this->json.HasMember("appKey")) {
		this->json.RemoveMember("appKey");
	}
	this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());

	// ��ʼ��������
	rapidjson::Value packageObject(rapidjson::kObjectType);

	// ���ݰ�ΨһID���ַ������32λ
	char *uniqueId = this->GetUniqueId();
	rapidjson::Value uniqueIdString(rapidjson::kStringType);
	if (!uniqueId) {
		string uniqueId = CreateUniqueId();
		char cUniqueId[33] = { 0 };
		sprintf_s(cUniqueId, 33, "%s", uniqueId.c_str());
		uniqueIdString.SetString(cUniqueId, strlen(cUniqueId));
	}
	else {
		uniqueIdString.SetString(uniqueId, 32);
	}
	if (packageObject.HasMember("uniqueId")) {
		packageObject.RemoveMember("uniqueId");
	}
	packageObject.AddMember("uniqueId", uniqueIdString, this->json.GetAllocator());

	// ����ʱ�䣬���뼶ʱ�������ֵ
	char *timestamp = this->GetTimestamp();
	rapidjson::Value timestampString(rapidjson::kStringType);
	if (timestamp == NULL) {
		long long intTimestamp = CreateTimestamp();
		char sTimestamp[64] = { 0 };
		sprintf_s(sTimestamp, sizeof(sTimestamp), "%lld", intTimestamp);
		timestampString.SetString(sTimestamp, strlen(sTimestamp));
	}
	else {
		timestampString.SetString(timestamp, strlen(timestamp));
	}
	if (packageObject.HasMember("timestamp")) {
		packageObject.RemoveMember("timestamp");
	}
	packageObject.AddMember("timestamp", timestampString, this->json.GetAllocator());

	// ����ָ���Ŀ��΢�ſͻ���ID��ÿ��һ��΢�ſͻ��˴���һ��ΨһID����Ŀ��ͻ�����Ϊnull���ַ�����null
	char *wechatId = this->GetWechatId();
	if (wechatId == NULL) {
		rapidjson::Value wechatIdString(rapidjson::kNullType);
		wechatIdString.SetNull();
		if (packageObject.HasMember("wechatId")) {
			packageObject.RemoveMember("wechatId");
		}
		packageObject.AddMember("wechatId", wechatIdString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value wechatIdString(rapidjson::kStringType);
		wechatIdString.SetString(wechatId, strlen(wechatId));
		if (packageObject.HasMember("wechatId")) {
			packageObject.RemoveMember("wechatId");
		}
		packageObject.AddMember("wechatId", wechatIdString, this->json.GetAllocator());
	}

	// ����ָ�16������ֵ
	int opCode = this->GetOpCode();
	rapidjson::Value opCodeNumber(rapidjson::kNumberType);
	opCodeNumber.SetInt(opCode);
	if (packageObject.HasMember("opCode")) {
		packageObject.RemoveMember("opCode");
	}
	packageObject.AddMember("opCode", opCodeNumber, this->json.GetAllocator());

	// ҵ���������
	rapidjson::Value *bodyObject = this->GetBody();
	if (packageObject.HasMember("body")) {
		packageObject.RemoveMember("body");
	}
	packageObject.AddMember("body", *bodyObject, this->json.GetAllocator());

	// ��ֵ������
	if (this->json.HasMember("package")) {
		this->json.RemoveMember("package");
	}
	this->json.AddMember("package", packageObject, this->json.GetAllocator());

	// �����ַ���
	rapidjson::StringBuffer buffer;
	Writer<rapidjson::StringBuffer> writer(buffer);
	this->json.Accept(writer);

	return buffer;
}
// �����ıش�����
bool Package::Check()
{
	// ���Ӧ��ID
	char *appId = this->GetAppId();
	if (appId == NULL || strlen(appId) <= 0) {
		return FALSE;
	}

	// ���Ӧ��KEY
	char *appKey = this->GetAppKey();
	if (appKey == NULL || strlen(appKey) <= 0) {
		return FALSE;
	}

	// ������ָ��
	int opCode = this->GetOpCode();
	bool checkOpCodeResult = FALSE;
	for (int i = 0; i < sizeof(allowOpCode); i++) {
		if (opCode == allowOpCode[i]) {
			checkOpCodeResult = TRUE;
			break;
		}
	}
	if (checkOpCodeResult == FALSE) {
		return FALSE;
	}

	return TRUE;
}
/*==============================================================*/
// ��ȡ�ͻ��˵�Ӧ��ID
char *Package::GetAppId()
{
	return this->appId;
}
// ���ÿͻ��˵�Ӧ��ID
void Package::SetAppId(char *appId)
{
	this->appId = appId;
}
// ��ȡ�ͻ��˵�Ӧ��KEY
char *Package::GetAppKey()
{
	return this->appKey;
}
// ���ÿͻ��˵�Ӧ��KEY
void Package::SetAppKey(char *appKey)
{
	this->appKey = appKey;
}
/*==============================================================*/
// ��ȡ���ݰ�ΨһID
char *Package::GetUniqueId()
{
	return this->uniqueId;
}
// �������ݰ�Ψһ
void Package::SetUniqueId(char *uniqueId)
{
	this->uniqueId = uniqueId;
}
/*==============================================================*/
// ��ȡ����ʱ��
char *Package::GetTimestamp()
{
	return this->timestamp;
}
// ���÷���ʱ��
void Package::SetTimestamp(char *timestamp)
{
	this->timestamp = timestamp;
}
/*==============================================================*/
// ��ȡ����ָ���Ŀ��΢�ſͻ���ID
char *Package::GetWechatId()
{
	return this->wechatId;
}
// ���ý���ָ���Ŀ��΢�ſͻ���ID
void Package::SetWechatId(char *wechatId)
{
	this->wechatId = wechatId;
}
/*==============================================================*/
// ��ȡ����ָ��
int Package::GetOpCode()
{
	return this->opCode;
}
// ���ò���ָ��
void Package::SetOpCode(int opCode)
{
	this->opCode = opCode;
}
/*==============================================================*/
// ��ȡҵ���������
rapidjson::Value *Package::GetBody()
{
	return &body;
}
// ����ҵ���������
void Package::SetBody(rapidjson::Value &body)
{
	this->body = body;
}