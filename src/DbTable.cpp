#include "DbTable.hpp"
#include <iostream>

bool DbTable::connect_db(std::string_view conn_str)
{
	try
	{
		dbconn->connect(conn_str.data());
	}
	catch (odbc::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return dbconn->connected() && dbconn->isValid();
}

void on_resized(MyWidgets::Listbox &table, const nana::arg_resized &arg)
{
	int cols = table.column_size();
	for (int i = 0; i < cols; ++i)
	{
		table.column_at(i).width((arg.width - 9) / cols);		
	}
}


auto &DbTable::get_table() noexcept
{
	return table;
}


const size_t DbTable::get_current_page() const noexcept
{
	return current_page;
}


void DbTable::set_table_update_function(std::function<upd_func_t> func)
{
	update_table_func = func;
	update_table();
}


void on_next_page_click(DbTable &dbtable)
{
	if (dbtable.table.size_item(0) == dbtable.page_elems_limit)
	{
		dbtable.page_counter.from((int)(dbtable.current_page += 1));
		dbtable.update_table();
	}
}


void on_prev_page_click(DbTable &dbtable)
{
	if (dbtable.current_page > 1)
	{
		dbtable.page_counter.from((int)(dbtable.current_page -= 1));
		dbtable.update_table();
	}
}


void DbTable::set_events()
{
	this->next_butt.events().click(
								   [&]()
								   {
									   on_next_page_click(*this);
								   });

	this->prev_butt.events().click(
								   [&]()
								   {
									   on_prev_page_click(*this);
								   });

	this->table.events().resized(
								 [&](const nana::arg_resized &arg)
								 {
									 on_resized(this->table, arg);
								 });

	this->page_counter.events().key_char(
										[&](const nana::arg_keyboard &arg)
										{
											if (arg.key == nana::keyboard::enter)
											{
												current_page = page_counter.to_int();
												update_table();
											}
										});
}


void DbTable::layout()
{
	this->div("vert <weight=90% table> < <add_butt> <edit_butt> <page_num> <prev_butt> <next_butt> >");
	(*this)["table"] << table;
	(*this)["add_butt"] << add_butt;
	(*this)["edit_butt"] << edit_butt;
	(*this)["page_num"] << page_counter;
	(*this)["prev_butt"] << prev_butt;
	(*this)["next_butt"]<< next_butt;
	this->collocate();
}


void DbTable::update_table()
{
	if (update_table_func)
	{
		update_table_func(table, dbconn, current_page, page_elems_limit);
	}
}
