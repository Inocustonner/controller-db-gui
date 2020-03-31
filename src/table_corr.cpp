#include "tables.hpp"
#include "DbTable.hpp"

#include <nana/gui/place.hpp>
void admin_table()
{
	using namespace nana;
	constexpr size_t width = 600, height = 300;
	form main_form;
	API::track_window_size(main_form, { width, height }, false);
	
	DbTable table(main_form);

	place layout(main_form);
	layout.div("<margin=[0] table>");
	layout.field("table") << table;	
	layout.collocate();

	main_form.show();
	nana::exec();
}
