#pragma once
#include <odbc/Connection.h>
#include <odbc/Environment.h>
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <odbc/ResultSet.h>

extern std::string global_table_name;
#define ON_EXCEPTION(col_name) "ON CONFLICT(" #col_name ") DO UPDATE SET "
