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
	struct Header_Propts
	{
		enum class Type
		{
			Numeric,
			Text
		};
		enum class Locale
		{
			Eng,
			Rus
		};

		std::string dbheader;
		Type type;
		Locale locale;
	};

public:
	DbTable() = delete;

	template<typename ...Args>
	DbTable(Args&& ...args) : nana::group(std::forward<Args>(args)...),
							  table(*this),
							  upd_butt(*this), // edit_butt(*this), 
							  next_butt(*this), prev_butt(*this),
							  page_counter(*this),
							  env{ odbc::Environment::create() }
	{
		dbconn = env->createConnection();
		upd_butt.caption("Update"); // edit_butt.caption("Edit");
		prev_butt.caption("<"); next_butt.caption(">");
		page_counter.from((int)current_page);

		set_events();

		layout();
	}


	auto append_header(std::string&& header_name, std::string&& field_name = "",
		Header_Propts::Type ht = Header_Propts::Type::Text,
		Header_Propts::Locale locl = Header_Propts::Locale::Eng)
	{
		if (field_name.empty())
			field_name = header_name;

		corresp_fields.push_back( { field_name, ht, locl } );
		return table.append_header(header_name);
	}

	auto append_header(std::string&& header_name, Header_Propts::Locale locale)
	{
		corresp_fields.push_back({ header_name, Header_Propts::Type::Text, locale });
		return table.append_header(header_name);
	}

	auto append_header(std::string&& header_name, Header_Propts::Type type)
	{
		corresp_fields.push_back({ header_name, type, Header_Propts::Locale::Eng });
		return table.append_header(header_name);
	}


	bool connect_db(std::string_view conn_str);

	using func_t = void(DbTable&);

	void set_table_update_function(std::function<func_t> func);

	void set_table_add_function(std::function<func_t> func);
	void set_table_edit_function(std::function<func_t> func);

	void set_events();
	void layout();
	void update_table();

public:
	MyWidgets::Listbox table;
	MyWidgets::Button upd_butt, // edit_butt, 
		next_butt, prev_butt;
	MyWidgets::NumTextbox page_counter;

	size_t current_page = 1;
	size_t page_elems_limit = 10;
	odbc::ConnectionRef dbconn;

	odbc::EnvironmentRef env;
	
	std::function<func_t> update_table_func;
	std::function<func_t> add_table_func;
	std::function<func_t> edit_table_func;

	std::vector<Header_Propts> corresp_fields; // corresponding db field names to headers
};
