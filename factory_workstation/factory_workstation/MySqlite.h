//======================================================================================
//MySqlite.h
#pragma once
#include "sqlite3.h"
#include <string>
#include "stdafx.h"

#define SQL_CREATE_TABLE "Create table if not exists device_info( id INTEGER PRIMARY KEY AUTOINCREMENT, SN TEXT NOT NULL, \
TIME TEXT NOT NULL,Result INTEGER,SOCF TEXT,RebootEnt TEXT,Voltage TEXT,Current TEXT,Version TEXT,Status TEXT NOT NULL)"

#define SQL_INSERT_DATA "Insert into device_info (id, SN, TIME, Result, SOCF, RebootEnt, Voltage, Current, Version, Status) values (NULL, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s')"

#define SQL_QUERY_DATA  "select * from device_info where SN = '%s' order by id desc limit 0, 1"
#define SQL_TABLE_NAME  "device_info"
#define SQL_DB_NAME     "myDatabase.db"

typedef struct SQLINFO{
	CString SN;
	CString Time;
	int Result;
	CString SOCF;
	CString RebootEnt;
	CString Voltage;
	CString Current;
	CString Version;
	CString Status;
}SqlInfo,*SqlIPtr;

#define SQL_CREATE_CHARGE_TABLE "Create table if not exists device_info( id INTEGER PRIMARY KEY AUTOINCREMENT, SN TEXT NOT NULL, \
TIME TEXT NOT NULL,Current TEXT,Status TEXT NOT NULL)"
#define SQL_INSERT_CHARGE_DATA "Insert into device_info (id, SN, TIME, Current, Status) values (NULL, '%s', '%s', '%s', '%s')"
#define SQL_DB_CHARGE_NAME_FZ     "charge_fz.db"
#define SQL_DB_CHARGE_NAME_JN     "charge_jn.db"

typedef struct SQLINFO_charge{
	CString SN;
	CString Time;
	CString Current;
	CString Status;
}SqlInfo_charge,*SqlIPtr_charge;

class MySqliteInfo
{
public:
	bool sql_flag;//sql´ò¿ª×´Ì¬
	void OpenSqliteInit();
	int InsertSqlite(SqlIPtr arr);
	int ReadSqlite(char *sn, SqlIPtr arr);
	int ReadSqlite_with_time(CString sn, CString time, CString minutes, SqlIPtr arr);
	bool IsTableExist(const std::string& strTableName);

	void OpenSqliteInit_charge(const char* name);
	int InsertSqlite_charge(SqlIPtr_charge arr);
private:
	sqlite3 *m_pDB;
	SqlInfo m_sqlinfo;
};
