#pragma once

#include "app_common.h"
#include "retrieve_data.h"
#include "login_ui/login_ui.h"
#include "imgui.h"


class PassWord 
{
		/*Constructor and Detructor*/
	public:
		PassWord(mongocxx::database db);
		PassWord() = default;
		~PassWord() = default;
	
		/*properties*/
	public:
	
		//loging details in the order 0 admin, 1 finance, 2 marketer, 3 regular
		std::vector<UserData> passwords;
	
		mongocxx::collection login;
		std::string s_old_password = "";
		static char c_old_password[128];

		std::string s_new_password = "";
		static char c_new_password[128];
	
		bool password_error = false;
		bool password_success = false;
	
		bool show_window = false;
		ImGuiWindowFlags window_flags;
	
		/*methods*/
	public:
		int create_password_window();
		int password_error_message(bool& error_flag);
		int password_success_message(bool& error_flag);
};