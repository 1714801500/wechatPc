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

Package::Package()
{
	json.SetObject(); // ����ǰ��Document����Ϊһ��object��Ҳ����˵����Document��һ��Object���͵�domԪ��
}
// ���
void Package::SetConText(char *jsonStr)
{
	json.Parse(jsonStr);

	// �ͻ��˵�Ӧ��ID���ַ������32λ
	if (json["appId"].IsNull()) {
		this->SetAppId(NULL);
	}
	else {
		if (!json["appId"].IsString()) return;
		Value& appId = json["appId"];
		this->SetAppId((char*)appId.GetString());
	}

	// �ͻ��˵�Ӧ��KEY���ַ������32λ
	if (!json.HasMember("appKey")) return;
	if (json["appKey"].IsNull()) {
		this->SetAppKey(NULL);
	}
	else {
		if (!json["appKey"].IsString()) return;
		Value& appKey = json["appKey"];
		this->SetAppKey((char*)appKey.GetString());
	}

	if (!json.HasMember("package")) return;
	if (!json["package"].IsObject()) return;
	rapidjson::Value& packageObject = json["package"];

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
	if (!appId) {
		rapidjson::Value appIdString(rapidjson::kNullType);
		appIdString.SetNull();
		if (this->json.HasMember("appId")) {
			this->json.RemoveMember("appId");
		}
		this->json.AddMember("appId", appIdString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value appIdString(rapidjson::kStringType);
		appIdString.SetString(appId, strlen(appId));
		if (this->json.HasMember("appId")) {
			this->json.RemoveMember("appId");
		}
		this->json.AddMember("appId", appIdString, this->json.GetAllocator());
	}

	// �ͻ��˵�Ӧ��KEY���ַ������32λ
	char *appKey = this->GetAppKey();
	if (!appKey) {
		rapidjson::Value appKeyString(rapidjson::kNullType);
		appKeyString.SetNull();
		if (this->json.HasMember("appKey")) {
			this->json.RemoveMember("appKey");
		}
		this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value appKeyString(rapidjson::kStringType);
		appKeyString.SetString(appKey, strlen(appKey));
		if (this->json.HasMember("appKey")) {
			this->json.RemoveMember("appKey");
		}
		this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());
	}

	// ��ʼ��������
	rapidjson::Value packageObject(rapidjson::kObjectType);

	// ���ݰ�ΨһID���ַ������32λ
	char *uniqueId = this->GetUniqueId();
	rapidjson::Value uniqueIdString(rapidjson::kStringType);
	if (uniqueId == NULL) {
		string uniqueId = CreateUniqueId();
		char cUniqueId[33] = { 0 };
		sprintf_s(cUniqueId, 33, "%s", uniqueId.c_str());
		uniqueIdString.SetString(cUniqueId, strlen(cUniqueId));
	}
	else {
		uniqueIdString.SetString(uniqueId, strlen(uniqueId));
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
	// ������ָ��
	int opCode = this->GetOpCode();
	if (opCode <= 0) {
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