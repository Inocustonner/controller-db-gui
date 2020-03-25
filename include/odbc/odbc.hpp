#pragma once
#include <odbc/logger.hpp>
#include <odbc/odbc_data.hpp>
#include <vector>
#include <windows.h>
#include <sal.h>
#include <sql.h>
#include <sqlext.h>

#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "user32.lib")

template<int SQL_C_TYPE>
struct sql_ctype
{
	using type = void;
};
#define SQL_CTYPE(SQL_C_TYPE, ctype) template<> \
	struct sql_ctype<SQL_C_TYPE>{ using type = ctype; }

SQL_CTYPE(SQL_C_CHAR, unsigned char*);
SQL_CTYPE(SQL_C_WCHAR, wchar_t*);
SQL_CTYPE(SQL_C_SSHORT, short int);
SQL_CTYPE(SQL_C_USHORT, unsigned short int);
SQL_CTYPE(SQL_C_SLONG, long int);
SQL_CTYPE(SQL_C_ULONG, unsigned long int);
SQL_CTYPE(SQL_C_FLOAT, float);
SQL_CTYPE(SQL_C_DOUBLE, double);
SQL_CTYPE(SQL_C_BIT, unsigned char);
SQL_CTYPE(SQL_C_STINYINT, signed char);
SQL_CTYPE(SQL_C_UTINYINT, unsigned char);
SQL_CTYPE(SQL_C_SBIGINT, _int64);
SQL_CTYPE(SQL_C_UBIGINT, unsigned _int64);
SQL_CTYPE(SQL_C_BINARY, unsigned char*);
// interval types

template<int SQL_C_TYPE>
using sql_ctype_t = typename sql_ctype<SQL_C_TYPE>::type;

size_t sql_ctype_size(SQLSMALLINT ctype);
SQLSMALLINT sql_type_ctype(SQLSMALLINT type);
bool init_env();
void free_env();

struct Diag_Rec_Str
{
	const char *sql_state_str;
	const char *rec_str;
};

using Data_Matrix_t = std::vector<std::vector<Odbc_Data>>;

struct Stmt
{
private:
	HSTMT hStmt;
	const char *query;
	SQLSMALLINT cols_cnt = 0;
	SQLLEN rows_cnt = 0;
	RETCODE last_retcode;
	SQLINTEGER sql_status_code;

public:
	Stmt() = delete;
	Stmt(HSTMT hStmt);
	Stmt(HSTMT hStmt, const char *query);
	~Stmt();
	
	void set_query(const char *query);
	RETCODE exec();

	SQLSMALLINT get_cols_cnt();
	SQLLEN get_rows_cnt();
	Data_Matrix_t get_all_rows(); // data put in a line
	
	RETCODE get_last_retcode() const noexcept;
	SQLINTEGER get_status_code() const noexcept;
	
private:
	bool alloc_odbc_data();
	bool are_bindings_alloc = false;
	std::vector<Odbc_Data> bindings_cols;
};

struct Odbc
{
public:
	Odbc() = default;
	Odbc(const char *connection_string);
	~Odbc();

	void set_connection_string(const char *connection_string);
	bool connect();
	Stmt *exec_query(const char *query);
	RETCODE get_last_retcode() const noexcept;
	SQLINTEGER get_status_code() const noexcept;

private:
	HDBC hDbc = NULL;
	HSTMT hStmt = NULL;
	Stmt *stmt = nullptr;

	const char *connection_string = nullptr;
	bool connected = false;
	RETCODE last_retcode;
	SQLINTEGER sql_status_code;
};
