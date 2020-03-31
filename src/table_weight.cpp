#include "tables.hpp"
#include "DbTable.hpp"

#include <nana/gui/place.hpp>
#include <string>
#include <iostream>

void on_upd(MyWidgets::Listbox &table, odbc::ConnectionRef &dbconn, size_t current_page, size_t page_elems_limit)
{
	try
	{
		odbc::PreparedStatementRef psSelect =
			dbconn->prepareStatement("SELECT id, weight FROM cars_table OFFSET ? LIMIT ?");
		psSelect->setInt(1, (int)((current_page - 1) * page_elems_limit));
		psSelect->setInt(2, (int)page_elems_limit);
		odbc::ResultSetRef rs = psSelect->executeQuery();
		
		table.clear();
		auto cat = table.at(0);
		table.auto_draw(false);

		while (rs->next())
		{
			cat.append({ std::to_string(*rs->getInt(1)), std::to_string(*rs->getInt(2)) });
		}

		table.auto_draw(true);
	}
	catch (const odbc::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void user_table()
{
	using namespace nana;
	constexpr size_t width = 600, height = 300;
	form main_form;
	API::track_window_size(main_form, { width, height }, false);
	
	DbTable table(main_form);
	table.append_header("id");
	table.append_header("min weight");

	if (!table.connect_db("DRIVER={PostgreSQL ANSI}; SERVER=localhost; PORT=5432; DATABASE=cars; UID=postgres; PWD=root;"))
	{
		// message box everything is fd up
		return;
	}
	table.set_table_update_function(on_upd);

	place layout(main_form);
	layout.div("<margin=[0] table>");
	layout.field("table") << table;	
	layout.collocate();

	main_form.show();
	nana::exec();
}
