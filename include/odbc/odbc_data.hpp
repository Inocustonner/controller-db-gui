#pragma once
#include <windows.h>
#include <sal.h>
#include <sql.h>
#include <sqlext.h>

// move only
// data var may contain only primitive types
struct Odbc_Data
{
private:
	void *data = nullptr;
	SQLLEN *ind_ptr = nullptr;
	
public:
	SQLSMALLINT type;
	SQLLEN size;				// data size
	
public:
	Odbc_Data() = default;
	Odbc_Data(const Odbc_Data &odata) = delete;
	Odbc_Data(SQLSMALLINT type, void *data);
	Odbc_Data(SQLSMALLINT type, SQLLEN size, void *data);

	Odbc_Data(Odbc_Data &&odata);

	Odbc_Data &operator=(const Odbc_Data &odata) = delete;
	Odbc_Data &operator=(Odbc_Data &&odata);
	~Odbc_Data();

	void *get_data() const;
	
	template<typename T>
	T get_data() const
	{
		return reinterpret_cast<T>(get_data());
	}

	friend struct Stmt;
};
