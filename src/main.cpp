#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

#include <iostream>
#include <cwctype>				// iswdigit
#include <string_view>
#include <windows.h>


// create a window with filling form
// after accept this form sent to db and if call is succesful update append to table
void add_instance(const nana::arg_click& ei, nana::listbox &table)
{
	using namespace nana;

	form dlg(ei.window_handle);
	dlg.caption("Add instance");
	

	// create lables and corresponding textboxes
	std::string_view car_id_str = "car id";
	label car_id_label(dlg, car_id_str.data());
	
	textbox car_id_txtb{ std::move(create_dlg_textbox(dlg, 8, true)) };
	
	std::string_view gov_num_str = "gov num";
	label gov_num_label(dlg, gov_num_str.data());
	
	textbox gov_num_txtb = create_dlg_textbox(dlg, 9, false);
	
	std::string_view car_weight_str = "car weight";
	label car_weight_label(dlg, car_weight_str.data());
	
	textbox car_weight_txtb = create_dlg_textbox(dlg, 8, true);
	
	std::string_view correction_str = "correction";
	label correction_label(dlg, correction_str.data());

	textbox correction_txtb = create_dlg_textbox(dlg, 6, true);

	place plc(dlg);
	plc.div("<labels grid=[4, 2]>");
	plc.field("labels") << car_id_label << car_id_txtb << gov_num_label << gov_num_txtb << car_weight_label << car_weight_txtb << correction_label << correction_txtb;
	plc.collocate();
	API::modal_window(dlg);
}


// called only once
void init_interface(nana::form *fm)
{
	using nana::listbox, nana::button;
	
	static listbox table(*fm, nana::rectangle(10, 60, 800, 200));
	table.sortable(false);
	table.append_header("car id", 200);
	table.append_header("gov num", 200);
	table.append_header("car weight", 200);
	table.append_header("car weight correction", 200);
	

	static button add_button(*fm, nana::rectangle(10, 10, 100, 50));
	add_button.caption("Add");
	add_button.enable_focus_color(false);
	add_button.edge_effects(false);

	add_button.events().click([table = &table](const nana::arg_click& ei){ add_instance(ei, *table); });
}


#ifndef DEBUG_TABLE
INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
#else
int main()
#endif
{
	using nana::form, nana::listbox;


	auto screen_info = nana::screen();

	form fm;
	unsigned int wnd_height = 900, wnd_width = 300;
	fm.size({ wnd_height, wnd_width });

	// compute font size

	init_interface(&fm);
	

	// this will attemp to use   
	// item_proxy cat_proxy::append (T &&t, bool set_value=false) 
	// to add a value any of char[4]  = "int"
	// lb.at(0).append({"double"});
	fm.show();
	nana::exec();
}

