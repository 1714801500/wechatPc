#include <Windows.h>
#include <string>
#include <list>
#include "struct.h"
using namespace std;

//���ݿ���
struct DbHandle
{
	int handler;
	char path[0x1000];
};

typedef int(*sqlite3_callback)(void*, int, char**, char**);

typedef int(WINAPI* Sqlite3_exec)(
	DWORD,
	const char*,
	sqlite3_callback,
	void*,
	char**
);
// ����΢��sqlite���
void ListenDbHandle();
// ִ��SQL
int runSql(string dbName, string sqlStr, sqlite3_callback callBack);
// ��ȡ�������ݿ���
list<DbHandle> getDbHandleList();
// ����������ݿ���
VOID clearDbHandleList();
