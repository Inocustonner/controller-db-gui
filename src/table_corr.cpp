#include "AddForm.hpp"
#include "tables.hpp"
#include "DbTable.hpp"
#include "prompt_db.hpp"

#include <nana/gui/place.hpp>
#include <string>
#include <iostream>

static void on_upd(DbTable& dbtable)
{
	try
	{
		odbc::PreparedStatementRef psSelect =
			dbtable.dbconn->prepareStatement("SELECT id, weight, corr, gn FROM cars_table OFFSET ? LIMIT ?");
		psSelect->setInt(1, (int)((dbtable.current_page - 1) * dbtable.page_elems_limit));
		psSelect->setInt(2, (int)dbtable.page_elems_limit);
		odbc::ResultSetRef rs = psSelect->executeQuery();
		
		dbtable.table.clear();
		auto cat = dbtable.table.at(0);
		dbtable.table.auto_draw(false);
		while (rs->next())
		{
			cat.append({ std::to_string(*rs->getInt(1)), 
				std::to_string(*rs->getInt(2)), 
				std::to_string(*rs->getInt(3)), 
				*rs->getString(4)});
		}

		dbtable.table.auto_draw(true);
	}
	catch (const odbc::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void admin_table()
{
	using namespace nana;
	constexpr char *table_name = "cars_table";

	global_table_name = table_name;

	constexpr size_t width = 600, height = 300;
	form main_form;
	API::track_window_size(main_form, { width, height }, false);
	
	DbTable table(main_form);
	table.append_header("id");
	table.append_header("weight", DbTable::Header_Propts::Type::Numeric);
	table.append_header("correction", "corr", DbTable::Header_Propts::Type::Numeric);
	table.append_header("gov number", "gn", DbTable::Header_Propts::Type::Text);

	//"DRIVER={PostgreSQL ANSI}; SERVER=localhost; PORT=5432; DATABASE=cars; UID=postgres; PWD=root;"
	if (!table.connect_db(prompt_conn_str("cars").c_str()))
	{
		// message box everything is fd up
		return;
	}
	table.set_table_update_function(on_upd);
	table.set_table_add_function(add_dlg); // in AddForm.hpp
	place layout(main_form);
	layout.div("<margin=[0] table>");
	layout.field("table") << table;	
	layout.collocate();

	main_form.show();
	nana::exec();
}
