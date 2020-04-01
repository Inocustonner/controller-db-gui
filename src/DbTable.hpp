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

	// template<typename ...Args>
	// auto append_header(Args&& ...args)
	// {
	// 	return this->table.append_header(std::forward<Args>(args)...);
	// } 

	auto append_header(std::string&& header_name, std::string field_name = "")
	{
		if (field_name.empty())
			field_name = header_name;

		corresp_fields.push_back(field_name);
		return table.append_header(header_name);
	}

	bool connect_db(std::string_view conn_str);

	auto &get_table() noexcept;
	
	const size_t get_current_page() const noexcept;
	
	using upd_func_t = void(MyWidgets::Listbox &table, odbc::ConnectionRef &dbconn, size_t current_page, size_t page_elems_limit);

	void set_table_update_function(std::function<upd_func_t> func);

	struct add_arg
	{
		MyWidgets::Listbox &table;
		odbc::ConnectionRef &dbconn;
		std::vector<std::string> &corresp_fields;
	};

	using add_func_t = void(add_arg &);

	void set_table_add_function(std::function<add_func_t> func);
	void set_table_edit_function(std::function<add_func_t> func);
private:

	friend void on_next_page_click(DbTable &dbtable);
	friend void on_prev_page_click(DbTable &dbtable);
	friend void on_add_click(DbTable &dbtable);
	friend void on_edit_click(DbTable &dbtable);

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
	std::function<add_func_t> add_table_func;
	std::function<add_func_t> edit_table_func;

	std::vector<std::string> corresp_fields; // corresponding db field names to headers
};
