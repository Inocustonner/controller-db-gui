#include "AddForm.hpp"
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>

#include <iostream>
#include <nana/gui/widgets/group.hpp>

void on_add(MyWidgets::Listbox &table, odbc::ConnectionRef &dbconn)
{
	using namespace nana;
	// dialog form that ask for user input
	form ask_form;
	constexpr size_t width = 200, height = 200;
	API::track_window_size(ask_form, { width, height }, false);

	place layout(ask_form);
	layout.div("<><vert weight=90% <vert weight=100 fields> <weight=50> <> <weight=50 submit_butt>><>");

	const size_t cols = table.column_size();

	// create groups, labels and inputs
	std::vector<group> groups(cols);
	std::vector<label> labels(cols);
	std::vector<MyWidgets::NumTextbox> inputs(cols);

	for (size_t i = 0; i < cols; ++i)
	{
		groups[i].create(ask_form);
		groups[i].borderless(false);

		labels[i].create(groups[i]);
		labels[i].caption(table.column_at(i).text());
		labels[i].text_align(align::center);

		inputs[i].create(groups[i]);
		inputs[i].configure();

		groups[i].div("vert <horisontal <label><vert <input>>>");
		groups[i]["label"] << labels[i];
		groups[i]["input"] << inputs[i];
		groups[i].collocate();

		layout.field("fields") << groups[i];
	}

	// create submit button
	MyWidgets::Button subm_butt(ask_form);
	subm_butt.caption("Submit");

	

	layout.field("submit_butt") << subm_butt;

	layout.collocate();
	API::modal_window(ask_form);
}
