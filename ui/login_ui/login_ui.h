#pragma once

#include "imgui.h"
#include "retrieve_data.h"

extern bool login_window;

namespace LoginUi
{
	class LoginWindow
	{
		/*Constructor and Detructor*/
	public:
		LoginWindow(mongocxx::database db);
		LoginWindow() = default;
		~LoginWindow() = default;

		/*properties*/
	public:		

		//loging details in the order 0 admin, 1 finance, 2 marketer, 3 regular
		std::vector<UserData> passwords;

		mongocxx::collection login;
		std::string s_password = "";
		static char c_password[128];

		bool login_error = false;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_login_window();
		int login_error_message(bool& error_flag);
		
	};
}