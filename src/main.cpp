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

inline
bool is_ascii(wchar_t ch)
{
	return (int)ch < 128;
}

// create a window with filling form
// after accept this form sent to db and if call is succesful update append to table
void add_instance(const nana::arg_click& ei, nana::listbox &table)
{
	using namespace nana;

	form dlg(ei.window_handle);
	dlg.caption("Add instance");

	// create lables and corresponding textboxes
	// car id - int 8 digits long
	constexpr unsigned car_id_max_chars = 8; // max number len
	std::string_view car_id_str = "car id";	 // label
	label car_id_label(dlg, car_id_str.data());
	textbox car_id_txtb{ dlg };
	car_id_txtb.multi_lines(false); // make inline
	API::effects_edge_nimbus(car_id_txtb, effects::edge_nimbus::none); // disable glowing on select
	car_id_txtb.set_accept([txtb = &car_id_txtb, max_chars = car_id_max_chars](wchar_t ch) -> bool
						   {
							   if (iswdigit(ch) && std::string_view(txtb->caption()).length() < max_chars
								   || ch == keyboard::backspace) return true;
							   else return false;
						   });

	// gov num line - str 9 chars long
	constexpr unsigned gov_num_max_chars = 9;
	std::string_view gov_num_str = "gov num";
	label gov_num_label(dlg, gov_num_str.data());
	textbox gov_num_txtb{ dlg };
	gov_num_txtb.multi_lines(false);
	API::effects_edge_nimbus(gov_num_txtb, effects::edge_nimbus::none);
	gov_num_txtb.set_accept([txtb = &gov_num_txtb, max_chars = gov_num_max_chars](wchar_t ch) -> bool
						   {
							   if (std::string_view(txtb->caption().c_str()).length() < max_chars
								   && is_ascii(ch)
								   || ch == keyboard::backspace) return true;
							   else return false;
						   });
	
	// car weight line - int 8 digits long
	constexpr unsigned car_weight_max_chars = 8;
	std::string_view car_weight_str = "car weight";
	label car_weight_label(dlg, car_weight_str.data());
	textbox car_weight_txtb{ dlg };
	car_weight_txtb.multi_lines(false);
	API::effects_edge_nimbus(car_weight_txtb, effects::edge_nimbus::none);
	car_weight_txtb.set_accept([txtb = &car_weight_txtb, max_chars = car_weight_max_chars](wchar_t ch) -> bool
						   {
							   if (iswdigit(ch) && std::string_view(txtb->caption()).length() < max_chars
								   || ch == keyboard::backspace) return true;
							   else return false;
						   });
	
	// correction line - int 6 digits long
	constexpr unsigned correction_max_chars = 6;	
	std::string_view correction_str = "correction";
	label correction_label(dlg, correction_str.data());
	textbox correction_txtb{ dlg };
	correction_txtb.multi_lines(false);
	API::effects_edge_nimbus(correction_txtb, effects::edge_nimbus::none);
	correction_txtb.set_accept([txtb = &correction_txtb, max_chars = correction_max_chars](wchar_t ch) -> bool
					   {
						   if (L'-' == ch || iswdigit(ch) && std::string_view(txtb->caption()).length() < max_chars
							   || ch == keyboard::backspace) return true;
						   else return false;
					   });

	// submit form button
	button submit_button(dlg);
	submit_button.caption("Submit");
	submit_button.enable_focus_color(false);
	submit_button.edge_effects(false);

	submit_button.events().click([&dlg](){ dlg.close();  });

	place plc(dlg);
	plc.div("<vert <car_id> <gov_num> <car_weight> <correction> <submit_button>>");
	plc.field("car_id") << car_id_label << car_id_txtb;
	plc.field("gov_num") << gov_num_label << gov_num_txtb;
	plc.field("car_weight") << car_weight_label << car_weight_txtb;
	plc.field("correction") << correction_label << correction_txtb;
	plc.field("submit_button") << submit_button;
	plc.collocate();
	API::modal_window(dlg);
}


// called only once
void init_interface(nana::form *fm)
{
	using nana::listbox, nana::button;
	
	static listbox table(*fm, nana::rectangle(10, 60, 850, 200));
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
	fm.caption("db table");
	// compute font size

	init_interface(&fm);
	

	// this will attemp to use   
	// item_proxy cat_proxy::append (T &&t, bool set_value=false) 
	// to add a value any of char[4]  = "int"
	// lb.at(0).append({"double"});
	fm.show();
	nana::exec();
}

