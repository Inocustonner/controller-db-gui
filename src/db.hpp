#pragma once
#include <odbc/Connection.h>
#include <odbc/Environment.h>
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <odbc/ResultSet.h>
#define TABLE_NAME "cars_table"
#define ON_EXCEPTION(col_name) "ON CONFLICT(" #col_name ") DO UPDATE SET "
