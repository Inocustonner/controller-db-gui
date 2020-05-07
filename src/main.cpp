#include "MyWidgets.hpp"
#include "tables.hpp"
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>

int ask_table()
{
	int ret;
	using namespace nana;
	// message box with options for choosing a table

	form ask_form;
	constexpr size_t width = 400, height = 200;
	ask_form.size({ width, height });
	
	MyWidgets::Button butt_user(ask_form), butt_admin(ask_form);
	butt_user.caption("user table"); butt_admin.caption("admin table");

	butt_user.events().click( [&ret, &ask_form](){ ret = 0; ask_form.close(); });
	butt_admin.events().click([&ret, &ask_form](){ ret = 1; ask_form.close(); });

	label info_label(ask_form);
	info_label.caption("choose the table");
	info_label.text_align(align::center);


	place layout(ask_form);
	layout.div("<vert <><label> <buttons gap=10>>");
	layout.field("label") << info_label;
	layout.field("buttons") << butt_user << butt_admin;
	layout.collocate();

	API::modal_window(ask_form);

	return ret;
}

#ifndef DEBUG_TABLE
INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
#else
int main()
#endif
{
	if (ask_table() == 0)
		user_table();
	else
		admin_table();
	return 0;
}
