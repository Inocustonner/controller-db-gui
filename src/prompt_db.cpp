#include "db.hpp"
#include "prompt_db.hpp"
#include <nana/gui.hpp>
std::string prompt_conn_str(std::string dbname_str)
{
	using namespace nana;
	form fm;

	inputbox::text server("Server");
	inputbox::text port("Port");
	inputbox::text dbname("Database", dbname_str.c_str());
	inputbox::text uid("Uid");
	inputbox::text pwd("Pwd");

	inputbox inbox(fm, "Input database info", "Database prompt");
	// verify
	inbox.verify([&](window handle)
				 {
					 if (!server.value().empty() && !port.value().empty() && 
						 !dbname.value().empty() && !uid.value().empty() && 
						 !pwd.value().empty())
						 return true;
					 else
					 {
						 msgbox mb(handle, "Invalid input");
						 mb << "Not all fields are filled";
						 mb.show();
						 return false;
					 }
				 });
	// verify
	if (inbox.show_modal(server, port, dbname, uid,pwd))
	{
		constexpr char driver_str[] = "DRIVER={PostgreSQL ANSI}";
		constexpr char server_str[] = ";SERVER=";
		constexpr char port_str[] = ";PORT=";
		constexpr char db_str[] = ";DATABASE=";
		constexpr char uid_str[] = ";UID=";
		constexpr char pwd_str[] = ";PWD=";

		return driver_str +
			(server_str + server.value()) +
			port_str + port.value() +
			db_str + dbname.value() +
			uid_str + uid.value() +
			pwd_str + pwd.value();
		fm.close();
	}
	//API::modal_window(fm);
}
