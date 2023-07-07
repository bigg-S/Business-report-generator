

#include "login_ui.h"
#include "regular_ui/regular_user_ui.h"
#include "finance_ui/finance_ui.h"
#include "marketer_ui/marketer_ui.h"
#include "admin_ui/admin_ui.h"

bool login_window = true;

char LoginUi::LoginWindow::c_password[128] = "";
LoginUi::LoginWindow::LoginWindow(mongocxx::database db)
{
	this->login = db["LogInData"];
}

int LoginUi::LoginWindow::create_login_window()
{
	this->passwords = get_users_data(this->login);

	this->s_password = "";

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));	

	ImGui::Begin("Login", NULL, this->window_flags | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if(this->login_error)
	this->login_error_message(this->login_error);

	ImGui::SameLine(70.0, 0.0);
	ImGui::Text("Password: ");

	ImGui::SameLine(150.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint("###", "password", this->c_password, IM_ARRAYSIZE(this->c_password));

	ImGui::NewLine();
	ImGui::SameLine(120.0, 0.0);
	if (ImGui::Button("Log In", ImVec2(145.0, 20.0)))
	{
		this->s_password = this->c_password;
		memset(this->c_password, 0, 128);

		if (this->s_password.compare(this->passwords[0].password) == 0)
		{
			admin_window = true;
			login_window = false;
		}
		else if (this->s_password.compare(this->passwords[1].password) == 0)
		{
			finance_window = true;
			login_window = false;
		}
		else if (this->s_password.compare(this->passwords[2].password) == 0)
		{
			marketer_window = true;
			login_window = false;
		}
		else if (this->s_password.compare(this->passwords[3].password) == 0)
		{
			regular_user_window = true;
			login_window = false;
		}
		else
		{
			this->login_error = true;
		}
	}
		

	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}

int LoginUi::LoginWindow::login_error_message(bool& error_flag)
{
	ImGui::OpenPopup("Login Error");
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Login Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Wrong Password!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(90, 0)))
		{
			error_flag = false;
			ImGui::CloseCurrentPopup();

		}

		ImGui::EndPopup();
	}

	return 1;
}