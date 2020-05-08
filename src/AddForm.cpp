#include "AddForm.hpp"
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
// #include <nana/gui.hpp>

#include <iostream>

static void update_db(DbTable& dbtable,
					  std::vector<MyWidgets::NumTextbox>& inputs)
{
	// form a query string
	std::string query_str = "INSERT INTO "+ global_table_name +"(";
	for (auto it = std::cbegin(dbtable.corresp_fields);
		 it != std::cend(dbtable.corresp_fields); ++it)
	{
		query_str += it->dbheader + ",";
	}
	query_str.pop_back();		// remove redoudant comma
	query_str = query_str + ")" + " VALUES(";
	
	for (auto it = std::cbegin(inputs);
		 it != std::cend(inputs) - 1; ++it)
	{
		DbTable::Header_Propts::Type type = dbtable.corresp_fields[std::distance(std::cbegin(inputs), it)].type;
		if (type == DbTable::Header_Propts::Type::Numeric)
			query_str += it->caption() + ",";
		else if (type == DbTable::Header_Propts::Type::Text)
			query_str += '\'' + it->caption() + '\'' + ",";
	}

	DbTable::Header_Propts::Type type = dbtable.corresp_fields.crbegin()->type;
	if (type == DbTable::Header_Propts::Type::Numeric)
		query_str += std::crbegin(inputs)->caption() + ") " ON_EXCEPTION(id);
	else if (type == DbTable::Header_Propts::Type::Text)
		query_str += '\'' + std::crbegin(inputs)->caption() + '\'' + ") " ON_EXCEPTION(id);

	//query_str += (std::crbegin(inputs))->caption() + ") " ON_EXCEPTION(id);

	for (auto it = std::cbegin(dbtable.corresp_fields);
		 it != std::cend(dbtable.corresp_fields); ++it)
	{
		query_str += it->dbheader + "=EXCLUDED." + it->dbheader + ",";
	}
	query_str.pop_back();
	try
	{
		auto psInsertUpdate = dbtable.dbconn->prepareStatement(query_str.c_str());
		psInsertUpdate->executeUpdate();
		dbtable.update_table();
	}
	catch(const odbc::Exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void add_dlg(DbTable& dbtable)
{
	using namespace nana;
	// dialog form that ask for user input
	form ask_form;
	constexpr size_t width = 200, height = 300;
	API::track_window_size(ask_form, { width, height }, false);

	place layout(ask_form);
	layout.div("<><vert weight=90% <vert weight=200 fields> <weight=10> <> <weight=50 submit_butt>><>");

	const size_t cols = dbtable.table.column_size();

	// create groups, labels and inputs
	std::vector<group> groups(cols);
	std::vector<label> labels(cols);
	std::vector<MyWidgets::NumTextbox> inputs(cols);

	for (size_t i = 0; i < cols; ++i)
	{
		groups[i].create(ask_form);
		groups[i].borderless(false);

		// labels should be created in the exact order as they stored in dbtable.corresp_fields
		labels[i].create(groups[i]);
		labels[i].caption(dbtable.table.column_at(i).text());
		labels[i].text_align(align::center);

		inputs[i].create(groups[i]);
		inputs[i].configure();
		if (dbtable.corresp_fields[i].type == DbTable::Header_Propts::Type::Text)
			inputs[i].text_mode();

		groups[i].div("vert <horisontal <label><vert <input>>>");
		groups[i]["label"] << labels[i];
		groups[i]["input"] << inputs[i];
		groups[i].collocate();

		layout.field("fields") << groups[i];
	}

	// create submit button
	MyWidgets::Button subm_butt(ask_form);
	subm_butt.caption("Submit");
	subm_butt.events().click(
		[&](){ update_db(dbtable, inputs); ask_form.close(); }
		);

	layout.field("submit_butt") << subm_butt;

	layout.collocate();
	API::modal_window(ask_form);
}
