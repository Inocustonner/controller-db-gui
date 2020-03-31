#pragma once
#include "db.hpp"
#include "MyWidgets.hpp"
#include <nana/gui/widgets/group.hpp>
#include <type_traits>
#include <functional>
#include <string_view>
// #include <nana/gui/widgets/form.hpp> // in MyButt.hpp
// #include <nana/gui/widgets/button.hpp> // in MyButt.hpp

struct DbTable : public nana::group
{
public:
	DbTable() = delete;

	template<typename ...Args>
	DbTable(Args&& ...args) : nana::group(std::forward<Args>(args)...),
							  table(*this),
							  add_butt(*this), edit_butt(*this), next_butt(*this), prev_butt(*this),
							  page_counter(*this),
							  env{ odbc::Environment::create() }
	{
		dbconn = env->createConnection();
		add_butt.caption("Add"); edit_butt.caption("Edit");
		prev_butt.caption("<"); next_butt.caption(">");
		page_counter.from((int)current_page);

		set_events();

		layout();
	}

	template<typename ...Args>
	auto append_header(Args&& ...args)
	{
		return this->table.append_header(std::forward<Args>(args)...);
	}

	bool connect_db(std::string_view conn_str);

	auto &get_table() noexcept;
	
	const size_t get_current_page() const noexcept;
	
	using upd_func_t = void(MyWidgets::Listbox &table, odbc::ConnectionRef &dbconn, size_t current_page, size_t page_elems_limit);

	void set_table_update_function(std::function<upd_func_t> func);

private:

	friend void on_next_page_click(DbTable &dbtable);
	friend void on_prev_page_click(DbTable &dbtable);
	friend void on_add_click();
	friend void on_edit_click();

	void set_events();
	void layout();
	void update_table();

private:
	MyWidgets::Listbox table;
	MyWidgets::Button add_butt, edit_butt, next_butt, prev_butt;
	MyWidgets::NumTextbox page_counter;

	size_t current_page = 1;
	size_t page_elems_limit = 10;

	odbc::EnvironmentRef env;
	odbc::ConnectionRef dbconn;
	
	std::function<upd_func_t> update_table_func;
};
