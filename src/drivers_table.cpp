#include "AddForm.hpp"
#include "tables.hpp"
#include "DbTable.hpp"
#include "prompt_db.hpp"

#include <nana/gui/place.hpp>
#include <string>
#include <iostream>
#include <array>
#include <windows.h>

static void on_upd(DbTable& dbtable) noexcept
{
	try
	{
		odbc::PreparedStatementRef psSelect =
			dbtable.dbconn->prepareStatement("SELECT id, fio FROM drivers OFFSET ? LIMIT ?");
		psSelect->setInt(1, (int)((dbtable.current_page - 1) * dbtable.page_elems_limit));
		psSelect->setInt(2, (int)dbtable.page_elems_limit);
		odbc::ResultSetRef rs = psSelect->executeQuery();

		dbtable.table.clear();
		auto cat = dbtable.table.at(0);
		dbtable.table.auto_draw(false);
		while (rs->next())
		{
			constexpr auto max_fio_len = 60;
			wchar_t A = L'À', a = L'à', IA = L'ß', ia = L'ÿ';
			std::array<char, max_fio_len + 1> fio_row = {};
			std::array<wchar_t, 1024> fio_char = {};
			rs->getStringData(2, fio_row.data(), std::size(fio_row));
			//std::for_each(std::begin(fio_row), std::end(fio_row), [](char& c) { if (c != '\0') c += 16; });
			MultiByteToWideChar(CP_OEMCP, 0, fio_row.data(), -1, fio_char.data(), 1024);
			//nana::to_wstring(fio_row.data())
			cat.append({ std::to_wstring(*rs->getInt(1)), std::wstring(fio_char.data()) });
		}

		dbtable.table.auto_draw(true);
	}
	catch (const odbc::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void drivers_table()
{
	using namespace nana;

	constexpr char* table_name = "drivers";

	global_table_name = table_name;

	constexpr size_t width = 600, height = 300;
	form main_form;
	API::track_window_size(main_form, { width, height }, false);

	DbTable table(main_form);
	table.append_header("id");
	table.append_header("fio");

	if (!table.connect_db(prompt_conn_str("drivers").c_str()))
	{
		return;
	}

	table.set_table_update_function(on_upd);
	table.set_table_add_function(add_dlg);

	place layout(main_form);
	layout.div("<margin=[0] table>");
	layout.field("table") << table;
	layout.collocate();

	main_form.show();
	nana::exec();
}
