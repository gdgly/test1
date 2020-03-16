
////======================================================================================
////初始化
//	m_sqlite.OpenSqliteInit();
//
////======================================================================================
////读取
//void CMPlayerDlg::OnBnClickedButton3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	int len;
//	CString sItem;
//	SqlInfo info;
//	
//	len = m_sqlite.ReadSqlite("LXF01-1018-02099820", &info);
//	sItem.Format("%s   %s   %s", info.SN,info.Time,info.Status);
//	list.AddString(sItem);
//	UpdateData(FALSE);
//}
//
////======================================================================================
////写入
//	SqlInfo info;
//	info.SN = sn;
//	info.Time = sTime;
//	info.Result = _ttoi(sResult);
//	info.Current = "";
//	info.RebootEnt = "";
//	info.SOCF = "";
//	info.Voltage = "";
//	info.Version = "";
//	info.Status = "pass";
//
//	m_sqlite.InsertSqlite(&info);
//	
//======================================================================================	
//MySqlite.cpp
#include "stdafx.h"
#include "MySqlite.h"


int sqlite3_callback_func(void* pHandle, int iRet, char** szSrc, char** szDst)
{
	//... 
	if (1 == iRet)
	{
		int iTableExist = atoi(*(szSrc));  //此处返回值为查询到同名表的个数，没有则为0，否则大于0
		if (pHandle != nullptr)
		{
			int* pRes = (int*)pHandle;
			*pRes = iTableExist;
		}
		// szDst 指向的内容为"count(*)"
	}

	return 0; //返回值一定要写，否则下次调用 sqlite3_exec(...) 时会返回 SQLITE_ABORT
}

bool MySqliteInfo::IsTableExist(const std::string& strTableName)
{
	std::string strFindTable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" + strTableName + "'";

	char* sErrMsg = nullptr;

	//void *pHandle = ***;
	int nTableNums = 0;
	if (sqlite3_exec(m_pDB, strFindTable.c_str(), &sqlite3_callback_func, &nTableNums, &sErrMsg) != SQLITE_OK)
	{
		return false;
	}
	//回调函数无返回值，则此处第一次时返回SQLITE_OK， 第n次会返回SQLITE_ABORT

	return nTableNums > 0;
}

void MySqliteInfo::OpenSqliteInit_charge(const char* name)
{
	int rc;
	char * err_msg = NULL;

	rc = sqlite3_open(name, &m_pDB);// 打开数据库，不存在时创建  
	if (rc)
	{
		sql_flag = false;
		sqlite3_close(m_pDB);  //打开失败！   
		AfxMessageBox(_T("数据库打开失败，请检查后再操作！"), NULL, MB_ICONSTOP);
		return;
	}

	if (false == IsTableExist(SQL_TABLE_NAME)) {
		if (sqlite3_exec(m_pDB, SQL_CREATE_CHARGE_TABLE, NULL, NULL, &err_msg) != SQLITE_OK)
		{
			sql_flag = false;
			sqlite3_close(m_pDB);
			AfxMessageBox(_T("操作失败，数据库链表生成错误！"), NULL, MB_ICONSTOP);
			return;
		}
	}
	sql_flag = true;
}

int MySqliteInfo::InsertSqlite_charge(SqlIPtr_charge arr)
{
	char sql[256];
	int rc = 0;
	char * err_msg = NULL;
	sprintf_s(sql, SQL_INSERT_CHARGE_DATA, (LPSTR)(LPCSTR)arr->SN, (LPSTR)(LPCSTR)arr->Time,
		(LPSTR)(LPCSTR)arr->Current, (LPSTR)(LPCSTR)arr->Status);

	int exit = sqlite3_exec(m_pDB, sql, 0, 0, &err_msg);
	if (exit != SQLITE_OK) {
		return 0;
	}
	return 1;
}

void MySqliteInfo::OpenSqliteInit()
{
	int rc;
	char * err_msg = NULL;

	rc = sqlite3_open(SQL_DB_NAME, &m_pDB);// 打开数据库，不存在时创建  
	if (rc)
	{
		sql_flag = false;
		sqlite3_close(m_pDB);  //打开失败！   
		AfxMessageBox(_T("数据库打开失败，请检查后再操作！"), NULL, MB_ICONSTOP);
		return;
	}

	if (false == IsTableExist(SQL_TABLE_NAME)) {
		if (sqlite3_exec(m_pDB, SQL_CREATE_TABLE, NULL, NULL, &err_msg) != SQLITE_OK)
		{
			sql_flag = false;
			sqlite3_close(m_pDB);
			AfxMessageBox(_T("操作失败，数据库链表生成错误！"), NULL, MB_ICONSTOP);
			return;
		}
	}
	sql_flag = true;
}

//id, SN, TIME, Result, SOCF, RebootEnt, Voltage, Current, Version, Status
int MySqliteInfo::InsertSqlite(SqlIPtr arr)
{
	char sql[256];
	int rc = 0;
	char * err_msg = NULL;
	sprintf_s(sql, SQL_INSERT_DATA, (LPSTR)(LPCSTR)arr->SN, (LPSTR)(LPCSTR)arr->Time,arr->Result, (LPSTR)(LPCSTR)arr->SOCF,
		(LPSTR)(LPCSTR)arr->RebootEnt, (LPSTR)(LPCSTR)arr->Voltage, (LPSTR)(LPCSTR)arr->Current, 
		(LPSTR)(LPCSTR)arr->Version, (LPSTR)(LPCSTR)arr->Status);

	int exit = sqlite3_exec(m_pDB, sql, 0, 0, &err_msg);
	if (exit != SQLITE_OK) {
		return 0;
	}
	return 1;
}


//select * from sensor where address = 'aaaa::11:22ff:fe33:4461' order by id desc limit 0,1;
int MySqliteInfo::ReadSqlite(char *sn, SqlIPtr arr)
{
	int rc,len=0;
	sqlite3_stmt *stmt;
	CString Result;
	char sql[256];
	sprintf_s(sql, SQL_QUERY_DATA, sn);
	rc = sqlite3_prepare_v2(m_pDB, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK)
	{
		return 0;
	}
	rc = sqlite3_step(stmt);
	while (rc == SQLITE_ROW)
	{
		arr->SN = sqlite3_column_text(stmt, 1);
		arr->Time = sqlite3_column_text(stmt, 2);
		Result = sqlite3_column_text(stmt, 3);
		arr->Result = _ttoi(Result);
		arr->SOCF = sqlite3_column_text(stmt, 4);
		arr->RebootEnt = sqlite3_column_text(stmt, 5);
		arr->Voltage = sqlite3_column_text(stmt, 6);
		arr->Current = sqlite3_column_text(stmt, 7);
		arr->Version = sqlite3_column_text(stmt, 8);
		arr->Status = sqlite3_column_text(stmt, 9);
		rc = sqlite3_step(stmt);
		len++;
	}

	return len;
}

//int len;
//CString sItem,sTime;
//SqlInfo info;
//CTime tNow = CTime::GetCurrentTime();
////YYYY-MM-DD HH:MM:SS
//sTime.Format("%04d-%02d-%02d %02d:%02d:%02d", tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
//len = m_sqlite.ReadSqlite("LXF01-1018-02099820", sTime, "80",&info);
//
//"select * from device_info where SN = 'LXF01-1018-02099820' and strftime('%Y-%m-%d %H:%M:%S', '2018-11-15 11:26:00', '-10 minutes') > strftime('%Y-%m-%d %H:%M:%S',TIME) order by id desc limit 0, 1"
//"select * from device_info where SN = 'LXF01-1018-02099820' order by id desc limit 0, 1"
int MySqliteInfo::ReadSqlite_with_time(CString sn, CString time, CString minutes, SqlIPtr arr)
{
	int rc,len=0;
	sqlite3_stmt *stmt;
	CString Result;
	//char sql[256];
	CString str1 = "select * from device_info where SN = '";
	CString str2 = "' and strftime('%Y-%m-%d %H:%M:%S', '";
	CString str3 = "', '-";
	CString str4 = " hours') > strftime('%Y-%m-%d %H:%M:%S',TIME) order by id desc limit 0, 1";//此处改为了hour
	CString cssql = str1 + sn + str2 + time + str3 + minutes + str4;

	rc = sqlite3_prepare_v2(m_pDB, cssql, -1, &stmt, 0);
	if (rc != SQLITE_OK)
	{
		return 0;
	}
	rc = sqlite3_step(stmt);
	while (rc == SQLITE_ROW)
	{
		arr->SN = sqlite3_column_text(stmt, 1);
		arr->Time = sqlite3_column_text(stmt, 2);
		Result = sqlite3_column_text(stmt, 3);
		arr->Result = _ttoi(Result);
		arr->SOCF = sqlite3_column_text(stmt, 4);
		arr->RebootEnt = sqlite3_column_text(stmt, 5);
		arr->Voltage = sqlite3_column_text(stmt, 6);
		arr->Current = sqlite3_column_text(stmt, 7);
		arr->Version = sqlite3_column_text(stmt, 8);
		arr->Status = sqlite3_column_text(stmt, 9);
		rc = sqlite3_step(stmt);
		len++;
	}

	return len;
}
