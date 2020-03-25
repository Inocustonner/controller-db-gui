#pragma once
#include <P7/P7_Trace.h>
// include windows
#define LOG(format, ...) Odbc_Logger::pTrace->P7_TRACE(Odbc_Logger::hModule, L"[\n" format L"\n]", __VA_ARGS__)
#define ERR_LOG(format, ...) Odbc_Logger::pTrace->P7_ERROR(Odbc_Logger::hModule, L"[\n" format L"\n]", __VA_ARGS__)
#define LOG_LINE(format, ...) Odbc_Logger::pTrace->P7_TRACE(Odbc_Logger::hModule, L"\t" format, __VA_ARGS__)
namespace Odbc_Logger
{
	extern IP7_Client *pClient;
	extern IP7_Trace *pTrace;
	extern IP7_Trace::hModule hModule;

	bool init_logger();
	void free_logger();

	void log(const wchar_t *format, ...);
	void err_log(const wchar_t *format, ...);
};
