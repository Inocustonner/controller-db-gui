#include <odbc/odbc.hpp>

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/label.hpp>

#include <iostream>

Odbc database;
size_t page_index;
size_t page_rows_limit;

inline
void set_butt_props(nana::button &butt)
{
	butt.enable_focus_color(false);
	butt.edge_effects(false);
}

auto get_accept(nana::textbox &txtb, size_t max_chars)
{
	return [&txtb, max_chars = max_chars](wchar_t ch) -> bool
		   {
			   if (iswdigit(ch) && txtb.caption().length() < max_chars
				   || ch == nana::keyboard::backspace
				   || ch == nana::keyboard::paste
				   || ch == nana::keyboard::copy)
				   return true;
			   else
				   return false;
		   };
}


void set_one_line_num_textbox(nana::textbox &txtb)
{
	using namespace nana;
	txtb.multi_lines(false);
	API::effects_edge_nimbus(txtb, effects::edge_nimbus::none);
	txtb.text_align(align::right);
}


void table_on_resized(nana::listbox &table, const nana::arg_resized &a_r)
{
	table.auto_draw(false);
	table.column_at(0).width((a_r.width - 8) / 3);
	table.column_at(1).width((a_r.width - 8) / 3);
	table.column_at(2).width((a_r.width - 8) / 3);
	table.auto_draw(true);
}


void update_table(nana::listbox &table)
{
	char query_buffer[100] = {};
	snprintf(query_buffer, std::size(query_buffer),
			 "SELECT * FROM cars_table LIMIT %u OFFSET %u;", page_rows_limit, page_index * page_rows_limit);
	Stmt *stmt = database.exec_query(reinterpret_cast<const char*>(query_buffer));
	if (stmt == nullptr)
	{
		std::cout << "Failed to update table" << std::endl;
		return;
	}
	
	int rows_cnt = stmt->get_rows_cnt();
	int cols_cnt = stmt->get_cols_cnt();
	Data_Matrix_t rows_v = stmt->get_all_rows();
	if (std::size(rows_v))
	{
		table.clear();			// clear all rows from the table
		auto cat = table.at(0);
		table.auto_draw(false);
		// for (int row_i = 0; row_i < rows_cnt; ++row_i)
		// {
		// 	std::vector<Odbc_Data> &row_v = rows[row_i];
			
		// }
		// redraw current page
		for (auto &row_v : rows_v)
		{
			int car_id = *reinterpret_cast<int*>(row_v[0].get_data());
			int weight = *reinterpret_cast<int*>(row_v[1].get_data());
			int corr = *reinterpret_cast<const int*>(row_v[2].get_data());
			cat.append({ std::to_string(car_id),
						 std::to_string(weight),
						 std::to_string(corr) });

		}
		table.auto_draw(true);
	}
	else
	{
		std::cout << "Failed to update table" << std::endl;
		return;
	}
}


bool insert_into_table(const char *car_id, const char *car_weight, const char *corr)
{
	char query_buffer[100] = {};
	snprintf(query_buffer, std::size(query_buffer),
			 "INSERT INTO cars_table VALUES(%s, %s, %s);", car_id, car_weight, corr);
	Stmt *stmt = database.exec_query(reinterpret_cast<const char*>(query_buffer));
	if (stmt == nullptr)
	{
		std::cout << "Error occured" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}


void add_dialog(const nana::arg_click &a_c, nana::listbox &table)
{
	using namespace nana;
	form dialog_form(a_c.window_handle);

	label	car_id_l(dialog_form, "car number"),
			car_weight_l(dialog_form, "car weight"),
			corr_l(dialog_form, "correction");
	
	textbox car_id_txtb(dialog_form),
			car_weight_txtb(dialog_form),
			corr_txtb(dialog_form);

	set_one_line_num_textbox(car_id_txtb);
	car_id_txtb.set_accept(get_accept(car_id_txtb, 9));
	
	set_one_line_num_textbox(car_weight_txtb);
	car_weight_txtb.set_accept(get_accept(car_weight_txtb, 9));
	
	set_one_line_num_textbox(corr_txtb);
	corr_txtb.set_accept(get_accept(corr_txtb, 9));

	button butt_submit(dialog_form);
	set_butt_props(butt_submit);
	butt_submit.caption("Submit");
	butt_submit.events().click([&]()
							   {
								   if (insert_into_table(car_id_txtb.caption().c_str(), car_weight_txtb.caption().c_str(), corr_txtb.caption().c_str()))
									   update_table(table);
								   dialog_form.close();
							   });

	place layout(dialog_form);
	layout.div("<vert <car_id> <car_weight> <corr> <butt_submit>>");
	layout.field("car_id") << car_id_l << car_id_txtb;
	layout.field("car_weight") << car_weight_l << car_weight_txtb;
	layout.field("corr") << corr_l << corr_txtb;
	layout.field("butt_submit") << butt_submit;
	layout.collocate();
	API::modal_window(dialog_form);
}


bool db_connect()
{
	database.set_connection_string("DRIVER={PostgreSQL ANSI}; SERVER=localhost; PORT=5432; DATABASE=cars; UID=postgres; PWD=root;");
	return database.connect();
}


#ifndef DEBUG_TABLE
INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
#else
int main()
#endif
{
	page_index = 0;
	page_rows_limit = 10;

	using namespace nana;
	if (!init_env()
		|| !db_connect())
	{
		return -1;
	}

	// create form
	form main_form;
	constexpr size_t min_width = 600, min_height = 300;
	main_form.size({ 600, 300 });
	main_form.caption("db table");
	API::track_window_size(main_form, { min_width, min_height }, false);

	// create table
	listbox table(main_form);
	table.sortable(false);
	table.column_movable(false);
	table.column_resizable(false);
	table.is_single_enabled(false);
	table.append_header("car number");
	table.append_header("car weight");
	table.append_header("correction");
	table.events().resized([&table](const arg_resized &a_r){ table_on_resized(table, a_r); });
	update_table(table);

	// create page number textbox
	textbox txtb_page_num(main_form);
	set_one_line_num_textbox(txtb_page_num);
	txtb_page_num.set_accept(get_accept(txtb_page_num, 10));
	txtb_page_num.events().key_press([&table, &txtb_page_num](const arg_keyboard &a_k)
									 {
										 // todo : add input check
										 if (a_k.key == nana::keyboard::enter)
										 {
											 page_index = txtb_page_num.to_int() - 1;
											 update_table(table);
										 }
									 });
	
	// create buttons
	button butt_add(main_form), butt_prev(main_form), butt_next(main_form);
	butt_add.caption("Add");
	set_butt_props(butt_add);
	butt_add.events().click([&table](const arg_click &a_c){ add_dialog(a_c, table); });

	butt_prev.caption("<");
	set_butt_props(butt_prev);
	butt_prev.events().click([&table, &txtb_page_num]()
							 {
								 if (page_index > 0)
								 {
									 page_index--;
									 txtb_page_num.from((int)(page_index + 1));
									 update_table(table);
								 }
							 });
	
	butt_next.caption(">");
	set_butt_props(butt_next);
	butt_next.events().click([&table, &txtb_page_num]()
								 {
									 if (page_rows_limit == table.size_item(0))
									 {
										 page_index++;
										 txtb_page_num.from((int)(page_index + 1));
										 update_table(table);
									 }
								 });
   

	// configure layout
	place layout(main_form);
	txtb_page_num.from((int)(page_index + 1));
	layout.div("<vert <table> <weight=10% <butt_add><butt_prev><page_num><butt_next>> >");
	layout.field("table") << table;
	layout.field("butt_add") << butt_add;
	layout.field("butt_prev") << butt_prev;
	layout.field("page_num") << txtb_page_num;
	layout.field("butt_next") << butt_next;
	layout.collocate();
	
	main_form.show();
	exec();
	free_env();
}
