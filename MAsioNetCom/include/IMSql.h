#pragma once
#include <comdef.h>
#include <vector>
#include <string>
#include "IAsioInterface.h"

interface IMSqlResult{
	virtual unsigned __stdcall Row() const = 0;
	virtual unsigned __stdcall Col() const = 0;
	virtual std::string __stdcall RowData(unsigned row) = 0;
	virtual std::string __stdcall CellData(unsigned row, unsigned col) = 0;
};

interface __declspec(uuid("12E0CFF9-8B8A-4326-A22D-8AEDE82C1C2D")) IMSql : public IUnknown
{
	//
	// 创建一个新对象
	//
	virtual IMSql* __stdcall CreateNewObj() = 0;

	virtual bool __stdcall OpenDB(const std::string& filename) = 0;
	virtual bool __stdcall DeleteDB(const std::string& filename) = 0;
	virtual bool __stdcall CloseDB() = 0;
	virtual bool __stdcall AddTable(const std::string& tablename, const std::string& desc) = 0;
	virtual bool __stdcall DeleteTable(const std::string& tablename) = 0;
	virtual bool __stdcall UseTable(const std::string& tablename) = 0;
	virtual bool __stdcall Insert(const std::string& value) = 0;
	virtual bool __stdcall Insert(const std::string& field, const std::string& value) = 0;
	virtual bool __stdcall ShowTables() = 0;
	virtual IMSqlResult* __stdcall GetData(const std::string& sql = "") = 0;
	virtual __int64 __stdcall Rows() = 0;
	virtual std::string __stdcall CurrentTable() const = 0;
	virtual std::string __stdcall CurrentDB() const = 0;

	//-----------------------------------------------------------
	// 删除指定项
	//------------------------------------------------------------
	virtual bool __stdcall DeleteItemBy(const std::string& key, const std::string& value) = 0;

	//---------------------------------------------------------------
	// 查找
	//---------------------------------------------------------------
	virtual IMSqlResult* __stdcall Find(const std::string& key, const std::string& value) = 0;

	virtual __int64 __stdcall Sum(const std::string& field, __int64 defaultvalue) = 0;
	virtual __int64 __stdcall Avg(const std::string& field, __int64 defaultvalue) = 0;
	virtual __int64 __stdcall Max(const std::string& field, __int64 defaultvalue) = 0;
	virtual __int64 __stdcall Min(const std::string& field, __int64 defaultvalue) = 0;

	virtual __int32 __stdcall Sum(const std::string& field, __int32 defaultvalue) = 0;
	virtual __int32 __stdcall Avg(const std::string& field, __int32 defaultvalue) = 0;
	virtual __int32 __stdcall Max(const std::string& field, __int32 defaultvalue) = 0;
	virtual __int32 __stdcall Min(const std::string& field, __int32 defaultvalue) = 0;

	virtual __int16 __stdcall Sum(const std::string& field, __int16 defaultvalue) = 0;
	virtual __int16 __stdcall Avg(const std::string& field, __int16 defaultvalue) = 0;
	virtual __int16 __stdcall Max(const std::string& field, __int16 defaultvalue) = 0;
	virtual __int16 __stdcall Min(const std::string& field, __int16 defaultvalue) = 0;

	virtual double __stdcall Sum(const std::string& field, double defaultvalue) = 0;
	virtual double __stdcall Avg(const std::string& field, double defaultvalue) = 0;
	virtual double __stdcall Max(const std::string& field, double defaultvalue) = 0;
	virtual double __stdcall Min(const std::string& field, double defaultvalue) = 0;

	virtual float __stdcall Sum(const std::string& field, float defaultvalue) = 0;
	virtual float __stdcall Avg(const std::string& field, float defaultvalue) = 0;
	virtual float __stdcall Max(const std::string& field, float defaultvalue) = 0;
	virtual float __stdcall Min(const std::string& field, float defaultvalue) = 0;

	virtual bool __stdcall UpdateItem(const std::string& field, const std::string& value,
		const std::string& byOther, const std::string& selectedvalue) = 0;
	virtual IMSqlResult* __stdcall Sort(const std::string& field, bool isless) = 0;
	virtual bool __stdcall Commit() = 0;
	virtual bool __stdcall RollBack() = 0;
	virtual bool __stdcall Begin() = 0;
	virtual bool __stdcall ExecSql(const std::string& sql) = 0;
	virtual std::string __stdcall GetLastError() = 0;
	virtual IMSqlResult* __stdcall GetResult() const = 0;
	virtual void __stdcall ShowInformation() const = 0;
};

//
// 默认提供的创建数据库操作对象的工厂函数
//
inline IMSql* CreateSqlite(){
	IMSql* pSql = NULL;
	::CoCreateInstance(
		ASIOCLSID,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMSql),
		(void**)&pSql
		);
	return pSql;
}