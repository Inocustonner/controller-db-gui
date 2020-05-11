#include "DbTable.hpp"
#include <iostream>
#include <nana/gui/widgets/menu.hpp>

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


void DbTable::set_table_update_function(std::function<func_t> func)
{
	update_table_func = func;
	update_table();
}


void DbTable::set_table_add_function(std::function<func_t> func)
{
	add_table_func = func;
}


void DbTable::set_table_edit_function(std::function<func_t> func)
{
	edit_table_func = func;
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


void on_add_click(DbTable &dbtable)
{
	if (dbtable.add_table_func)
	{
		dbtable.add_table_func(dbtable);
		dbtable.update_table();
	}
}


void on_edit_click(DbTable &dbtable)
{
	if (dbtable.edit_table_func)
	{
		dbtable.edit_table_func(dbtable);
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

	this->upd_butt.events().click(
		[&]()
		{
			on_add_click(*this); 
		});

	// this->edit_butt.events().click(
	// 	[&]()
	// 	{
	// 		on_edit_click(*this); 
	// 	});

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
	this->div("vert <weight=90% table> < <upd_butt>" // "<edit_butt>"
			  "<page_num> <prev_butt> <next_butt> >");
	(*this)["table"] << table;
	(*this)["upd_butt"] << upd_butt;
	// (*this)["edit_butt"] << edit_butt;
	(*this)["page_num"] << page_counter;
	(*this)["prev_butt"] << prev_butt;
	(*this)["next_butt"]<< next_butt;
	this->collocate();
}


void DbTable::update_table()
{
	if (update_table_func)
	{
		update_table_func(*this);
	}
}


static void delete_by_id(odbc::ConnectionRef& conn, const std::string& id) noexcept
{
	try
	{
		auto ps = conn->prepareStatement(("DELETE FROM " + global_table_name + " WHERE id=?").c_str());
		ps->setString(1, id);
		ps->executeQuery();
	}
	catch (const odbc::Exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void DbTable::onclick(const nana::arg_click& a_cl)
{
	if (std::size(this->table.selected()))
	{
		nana::listbox::item_proxy item = this->table.at(this->table.selected()[0]);
		auto delete_handler = [&item, this](const nana::menu::item_proxy&) { delete_by_id(dbconn, item.text(0)); update_table();  };

		nana::menu m;
		m.append("Delete", delete_handler);

		m.popup_await(*this, a_cl.mouse_args->pos.x, a_cl.mouse_args->pos.y);
	}
}
