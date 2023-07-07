

#include "finance_ui.h"
#include "retrieve_data.h"
#include "compute_costs.h"
#include "manage_OperationalCosts.h"
#include "agents.h"
#include "agent_sales.h"
#include "change_values.h"
#include "update_stock.h"
#include "login_ui/login_ui.h"
#include "marketer_ui/marketer_ui.h"
#include "admin_ui/admin_ui.h"
#include "imgui_internal.h"

#include <algorithm>


bool finance_window = false;

FUserUi::ShowFinanceWindow::ShowFinanceWindow(Profit_Loss_Window& profit_or_loss, Expenses& expenses, ConfirmSaleWindow& confim_sale, PassWord& change_password)
{
	this->profit_or_loss = &profit_or_loss;
	this->expenses = &expenses;
	this->confim_sale = &confim_sale;
	this->change_password = &change_password;
}

int FUserUi::ShowFinanceWindow::create_show_finance_window()
{	
	  ImGui::Dummy(ImVec2(0.0, 60.0));
	  ImGui::NewLine();
	  ImGui::SameLine(50.0, 0.0);
	  if (ImGui::Button("Profit/Loss", ImVec2(115.0, 20.0)))
		  this->profit_or_loss->show_window = true;

	  if(this->profit_or_loss->show_window)
		  this->profit_or_loss->create_profit_loss_window();
	  
	  ImGui::Dummy(ImVec2(0.0, 10.0));
	  ImGui::NewLine();
	  ImGui::SameLine(50.0, 0.0);
	  if (ImGui::Button("Manage Expenses", ImVec2(115.0, 20.0)))
		  this->expenses->show_window = true;

	  if (this->expenses->show_window)
		  this->expenses->create_handle_expenses_window();
	  
	  
	  if (this->expenses->add->show_window)
		  this->expenses->add->create_expense_window();
	 
	  if (this->expenses->remove->show_window)
		  this->expenses->remove->remove_expense_window();
	  
	  if (this->expenses->set->show_window)
		  this->expenses->set->set_expenditure_window();


	  ImGui::Dummy(ImVec2(0.0, 10.0));
	  ImGui::NewLine();
	  ImGui::SameLine(50.0, 0.0);
	  if (ImGui::Button("Confirm Sale", ImVec2(115.0, 20.0)))
		  this->confim_sale->show_window = true;

	  if (this->confim_sale->show_window)
		  this->confim_sale->create_confim_sale_window();

	  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));

	  ImGui::Dummy(ImVec2(0.0, 10.0));
	  ImGui::NewLine();
	  ImGui::SameLine(50.0, 0.0);
	  if (ImGui::Button("Log Out", ImVec2(115.0, 20.0)))
	  {
		  login_window = true;
		  finance_window = false;
		  marketer_window = false;
		  admin_window = false;
		  regular_user_window = false;
		  showing_admin_extensions = false;
		  
	  }
		 
	  ImGui::PopStyleColor(3);

	  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f / 7.0f, 0.6f, 0.6f));
	  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f / 7.0f, 0.7f, 0.7f));
	  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f / 7.0f, 0.8f, 0.8f));

	  ImGui::Dummy(ImVec2(0.0, 10.0));
	  ImGui::NewLine();
	  ImGui::SameLine(50.0, 0.0);
	  if (ImGui::Button("Change Password", ImVec2(115.0, 20.0)))
		  this->change_password->show_window = true;
	  ImGui::PopStyleColor(3);

	  if (this->change_password->show_window)
		  this->change_password->create_password_window();

	  return 1;

}

FUserUi::Profit_Loss_Window::Profit_Loss_Window(mongocxx::database db)
{
	this->operational_costs = db["OperationalCosts"];
	this->products_collection = db["products"];

	this->components_arrays = get_components_arrays(this->products_collection);
		
}

int FUserUi::Profit_Loss_Window::create_profit_loss_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 300), ImGuiCond_Once);

    ImGui::Begin("Profit/Loss", &this->show_window, this->window_flags);

	// description
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(200.0, 0.0);
	ImGui::TextColored(ImVec4(0.543f, 0.931f, 0.886f, 0.784f), "Profit or Loss");

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(150.0, 0.0);

	this->expected_expenditure = get_expected_expenditure(this->operational_costs);
	SALE sale; // used as a helper to navigate through the documents	
	this->total_sales = compute_total_cost(this->products_collection, components_arrays, sale, "SellingPrice");
	this->total_expenditure = get_operational_costs_total(this->operational_costs);
	this->profit_loss = this->total_sales - this->total_expenditure;

	std::string s_expected_expenditure = "Expected Expenditure: " + this->truncate(this->expected_expenditure);
	ImGui::Text(s_expected_expenditure.c_str());

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(150.0, 0.0);
	std::string s_total_sales = "Total Sales         : " + this->truncate(this->total_sales);
	ImGui::Text(s_total_sales.c_str());

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(150.0, 0.0);
	std::string s_total_expenditure = "Total Expenditure   : " + this->truncate((float)this->total_expenditure);
	ImGui::Text(s_total_expenditure.c_str());

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(150.0, 0.0);
	std::string s_profit_loss = "Total Profit/Loss   : " + this->truncate(this->profit_loss);
	ImGui::Text(s_profit_loss.c_str());

    ImGui::PopStyleColor();
    ImGui::End();

	return 1;
}


std::string FUserUi::Profit_Loss_Window::truncate(double val)
{
	std::string s_val = "";

	s_val = std::to_string(val);

	size_t decimal_index = s_val.find_first_of('.', 0);

	s_val = s_val.substr(0, decimal_index + 3);

	return s_val;
}


char FUserUi::AddExpenseWindow::c_expense_name[128] = "";

FUserUi::AddExpenseWindow::AddExpenseWindow(mongocxx::database db)
{
	this->operational_costs = db["OperationalCosts"];
}

int FUserUi::AddExpenseWindow::create_expense_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once);

	ImGui::Begin("Add Expense", &this->show_window, this->window_flags);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();	
	ImGui::Text("Expense Name: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint("###", "Expense Name", this->c_expense_name, IM_ARRAYSIZE(this->c_expense_name));	
	
	
	ImGui::NewLine();
	ImGui::Text("Expense Cost: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputFloat("##", &cost, 0);	
	

	
	//handle any error
	if (this->input_error == true)
		this->handle_error(this->input_error);
	else if (this->success == true)
		this->notify_sale_status(this->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(90.0, 0.0);

	if (ImGui::Button("Add", ImVec2(120, 0)))
	{
		this->expense_name = this->c_expense_name;
		
		if ((this->expense_name.compare("") == 0) || this->cost <= 0)
		{
			this->input_error = true;
		}
		else
		{
			insert_expense(this->operational_costs, this->expense_name, this->cost);
			this->success = true;
		}
			
	}
		

	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}

int FUserUi::Window::handle_error(bool& error_flag)
{
	ImGui::OpenPopup("Error");
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please Fill in All The Details Correctly or\nMake Sure A Checkbox is Checked.");
		ImGui::Separator();

		ImGui::SameLine(90.0f, 0.0f);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			error_flag = false;
			ImGui::CloseCurrentPopup();

		}

		ImGui::EndPopup();
	}

	return 1;
}

int FUserUi::Window::notify_sale_status(bool& success_flag)
{
	ImGui::OpenPopup("Success");
	//Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Success", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Operation successful");
		ImGui::Separator();

		ImGui::SameLine(20.0f, 0.0f);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			success_flag = false;
			ImGui::CloseCurrentPopup();

		}

		ImGui::EndPopup();
	}

	return 1;
}



int FUserUi::RemoveExpenseWindow::expense_current_item = 0;

FUserUi::RemoveExpenseWindow::RemoveExpenseWindow(mongocxx::database db)
{
	this->operational_costs = db["OperationalCosts"];
}

int FUserUi::RemoveExpenseWindow::remove_expense_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(450, 350), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once);

	ImGui::Begin("Remove Expense", &this->show_window, this->window_flags);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	
	this->fetch_expenses();
	create_combo(this->p_expenses, "Expense", this->expense_current_item, ImVec2(270, 35));
	delete[] this->p_expenses;
	this->p_expenses = NULL;
	

	if (this->confirm == this->ACCEPTED)
	{
		delete_expense(this->operational_costs, this->selected_expense);
		this->success = true;
		this->expense_current_item = 0;

		this->show_confirm_window = false;
		this->confirm = this->NONE;		
		
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->finance_confirm_message(this->confirm);
	else if (this->input_error)
		this->handle_error(this->input_error);

			
	if (this->success == true)
		this->notify_sale_status(this->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(90.0, 0.0);


	if (ImGui::Button("Remove", ImVec2(120, 0)))
	{
		this->get_selected_expense(this->expense_current_item);
		if (this->expense_current_item == 0 || this->selected_expense.compare("") == 0)
		{
			this->input_error = true;
		}
		else
		{
			this->show_confirm_window = true;
		}

	}

	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}



int FUserUi::RemoveExpenseWindow::create_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size)
{
	ImGui::SameLine(0.0, 12.0);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
	ImGui::BeginChild(description.c_str(), size, true);

	ImGui::Combo(description.c_str(), &current_item, (const char*)my_items_names);

	ImGui::PopStyleColor();
	ImGui::EndChild();

	return 1;
}

void FUserUi::RemoveExpenseWindow::prepare_items_pointer(const std::string& items, char items_separator, char*& items_pointer)
{
	size_t count = std::count(items.begin(), items.end(), items_separator);

	size_t offset = 0;
	for (int index = 0; index < count; index++)
	{
		offset = items.find_first_of(items_separator, (offset + 1));
		items_pointer[offset] = '\0';
	}

}

int FUserUi::RemoveExpenseWindow::set_expenses()
{
	this->expense_names = "Select_";

	this->expenses_array = get_expenses_array(this->operational_costs);

	for (std::string expense : this->expenses_array)
		this->expense_names += expense + "_";

	return 1;
}

std::string FUserUi::RemoveExpenseWindow::get_selected_expense(int selected_index)
{
	if (selected_index != 0)
		selected_expense = this->expenses_array[selected_index - 1];
	else
		selected_expense = "";

	return selected_expense;
}

int FUserUi::RemoveExpenseWindow::fetch_expenses()
{
	this->set_expenses();

	this->p_expenses = new char[this->expense_names.size() + 1];
	memset(this->p_expenses, 0, this->expense_names.size());
	strncpy_s(this->p_expenses, this->expense_names.size() + 1, this->expense_names.c_str(), this->expense_names.size());
	this->prepare_items_pointer(this->expense_names, '_', this->p_expenses);

	return 1;
}

FUserUi::SetExpenditureWindow::SetExpenditureWindow(mongocxx::database db)
{
	this->operational_costs = db["OperationalCosts"];
}

int FUserUi::SetExpenditureWindow::set_expenditure_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(480, 320), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_Once);

	ImGui::Begin("Set Expected Expenditure", &this->show_window, this->window_flags);
	
	ImGui::NewLine();
	ImGui::Text("Expected Expenditure: ");

	ImGui::SameLine(160.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputFloat("##", &this->expenditure, 0);


	//handle any error
	if (this->input_error == true)
		this->handle_error(this->input_error);
	else if (this->success == true)
		this->notify_sale_status(this->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(110.0, 0.0);

	if (ImGui::Button("Set", ImVec2(120, 0)))
	{
		if (this->expenditure < 0)
		{
			this->input_error = true;
		}
		else
		{
			set_expected_expenditure(this->operational_costs, this->expenditure);
			this->success = true;
		}

	}


	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}


FUserUi::Expenses::Expenses(FUserUi::AddExpenseWindow& add, FUserUi::RemoveExpenseWindow& remove, FUserUi::SetExpenditureWindow& set)
{
	this->add = &add;
	this->remove = &remove;
	this->set = &set;
}

int FUserUi::Window::finance_confirm_message(int& confirm_flag)
{

	ImGui::OpenPopup("Confirm");
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirm", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are You Sure You Want To Complete This Action?");
		ImGui::Separator();

		ImGui::SameLine(20.0f, 0.0f);
		if (ImGui::Button("No", ImVec2(90, 0)))
		{
			confirm_flag = REJECTED;
			ImGui::CloseCurrentPopup();

		}

		ImGui::SameLine(200.0f, 0.0f);
		if (ImGui::Button("Yes", ImVec2(90, 0)))
		{
			confirm_flag = ACCEPTED;
			ImGui::CloseCurrentPopup();

		}


		ImGui::EndPopup();
	}

	return 1;
}


int FUserUi::Expenses::create_handle_expenses_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Manage Expenses", &this->show_window, this->window_flags);

	if (this->success == true)
		this->notify_sale_status(this->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Add Expense", ImVec2(145.0, 20.0)))
		this->add->show_window = true;

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Remove Expense", ImVec2(145.0, 20.0)))
		this->remove->show_window = true;


	if (this->confirm == this->ACCEPTED)
	{
		reset_expenses(this->add->operational_costs);
		this->success = true;
		
		this->show_confirm_window = false;
		this->confirm = this->NONE;

	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->finance_confirm_message(this->confirm);
	else if (this->input_error)
		this->handle_error(this->input_error);


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);

	if (ImGui::Button("Reset Expenses", ImVec2(145.0, 20.0)))
	{
		this->show_confirm_window = true;
	}

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);

	if (ImGui::Button("Expected Expendture", ImVec2(145.0, 20.0)))
		this->set->show_window = true;
		

	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}

/*----------------------------------------------Confirm Sales--------------------------------------------*/

FUserUi::ConfirmSaleWindow::ConfirmSaleWindow(mongocxx::database db, RUserUi::Window& window)
{
	this->agents = db["Agents"];
	this->agent_sales = db["AgentSales"];
	this->products_collection = db["products"];
	this->stock = db["stock"];

	this->p_window = &window;

	this->selected_agent = "All Agents";

	this->table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg;
}

int FUserUi::ConfirmSaleWindow::prepare_agents_details(std::string& m_agent_details, const std::string& agent_details_separator, const std::string& new_separator)
{
	if (m_agent_details.empty())
		return 0;

	size_t start_pos = 0;

	while ((start_pos = m_agent_details.find(agent_details_separator, start_pos)) != std::string::npos)
	{
		m_agent_details.replace(start_pos, agent_details_separator.length(), new_separator);
		//start_pos += new_separator.length(); //In case new_separator contains agents_details_separator, like replacing 'x' with 'xy'
	}

	return 1;
}


int FUserUi::ConfirmSaleWindow::create_confim_sale_window()
{

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(720, 460), ImGuiCond_Once);

	ImGui::Begin("Confirm Agent Sale", &this->show_window, this->window_flags | ImGuiWindowFlags_AlwaysAutoResize);
	
	//handle any error
	if (this->input_error == true)
		this->handle_error(this->input_error);
	else if (this->success == true)
		this->notify_sale_status(this->success);

	this->set_agents_details();

	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = g.CurrentWindow;
	ImRect mrect = parent_window->Rect();

	float new_xcenter = mrect.GetCenter().x;

	ImGui::SameLine((new_xcenter - (210 / 2)) - mrect.Min.x, 0.0); //listbox to be centered
	this->p_window->create_listbox_filter(this->agents_details, "Agents", this->selected_agent, '_', ImVec2(210, 150), "Confirm");

	if (this->selected_agent.compare("All Agents") != 0)
	{
		this->set_agent_sales();
		if (this->v_agent_sales[0].size() != 0) //if we have an agent sale for the selected agent , we display them and the confirm button
		{
			draw_sales_table(this->TEXT_BASE_HEIGHT);

			size_t sales_count = this->checked_sales.size();
			SALE sale;

			ImGui::Dummy(ImVec2(0.0, 10.0));
			ImGui::NewLine();
			ImGui::SameLine((new_xcenter - (115 / 2)) - mrect.Min.x, 0.0);	//button to be centered
			if (ImGui::Button("Confirm", ImVec2(115.0, 20.0)))
			{
				if (this->agent_id == 0 || sales_count == 0)
				{
					this->input_error = true;
				}
				else
				{
					for (int i = 0; i < sales_count; i++)
					{
						sale.product_name = this->v_agent_sales[0][this->checked_sales[i]];
						sale.brand = sale.product_name + "_" + this->v_agent_sales[1][this->checked_sales[i]];
						sale.quantity = this->v_agent_sales[2][this->checked_sales[i]];
						sale.season = this->v_agent_sales[3][this->checked_sales[i]];
						sale.age_group = this->v_agent_sales[4][this->checked_sales[i]];
						sale.gender = this->v_agent_sales[5][this->checked_sales[i]];
						sale.location = this->v_agent_sales[6][this->checked_sales[i]];
						sale.item_count = std::stoi(this->v_agent_sales[7][this->checked_sales[i]]);
					
						
						change_amountOf_item(this->products_collection, sale, UPDATE);//update the sold items collection
						update_stock_amountOf_item(this->stock, sale, UPDATE);//update the stock
						delete_agent_sale(this->agent_sales, this->agent_id, std::stoi(this->v_agent_sales[8][this->checked_sales[i]]));//update the agents sales data
					
						
					}
					this->success = true;
					this->checked_sales.clear();
				}

			}
		}			
		else //we notify the user that there are no sales for the selected agent
		{
			ImGui::Dummy(ImVec2(0.0, 10.0));
			ImGui::NewLine();
			ImGui::Text(std::string("No Pending Sales for " + this->selected_agent.substr(8, this->selected_agent.size())).c_str());
			ImGui::Dummy(ImVec2(0.0, 10.0));
		}	
			
	}	
	else  //No agent selected, give the user some guide
	{
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::Text("Select an Agent To View Their Pending Sales.");
		ImGui::Dummy(ImVec2(0.0, 10.0));
	}		
	
	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}

int FUserUi::ConfirmSaleWindow::set_agents_details()
{
	this->agents_details = retieve_agents_data(this->agents);

	this->prepare_agents_details(this->agents_details, "_", ": "); //the details are n the form id_name, id_name; first we replace the '_' with ': '
	this->prepare_agents_details(this->agents_details, ",", "_"); //then we replace the commas with '_' which are required by our list box

	this->agents_details = "All Agents_" + this->agents_details;

	return 1;
}

int FUserUi::ConfirmSaleWindow::set_agent_sales()
{
	int64_t agent_id = this->get_agent_id();
	this->v_agent_sales = get_agent_sales_arrays(this->agent_sales, agent_id);

	return 1;
}

int64_t FUserUi::ConfirmSaleWindow::get_agent_id()
{
	int64_t a_id = 0;
	std::string s_id = this->selected_agent.substr(0, 6);

	try
	{
		a_id = (int64_t)std::stoi(s_id);
	}
	catch (std::invalid_argument const& ex)
	{

		//no agent selected
		a_id = 0;
	}

	this->agent_id = a_id;

	return a_id;
}

int FUserUi::ConfirmSaleWindow::draw_sales_table(const float& m_TEXT_BASE_HEIGHT)
{
	bool* checked_statuses = new bool[this->v_agent_sales[0].size()];
	memset(checked_statuses, 0, this->v_agent_sales[0].size());
	for (int index : this->checked_sales)
		checked_statuses[index] = true;


	ImVec2 outer_size = ImVec2(0.0f, m_TEXT_BASE_HEIGHT * 90);
	if (ImGui::BeginTable("table_scrolly", 10, this->table_flags, outer_size))
	{
		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Product", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Brand", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Season", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Age Group", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Gender", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Check", ImGuiTableColumnFlags_None);

		ImGui::TableHeadersRow();
			

		std::vector<int>::iterator position;
		bool clicked;
		bool present;

		// using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin((int)this->v_agent_sales[0].size());
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();

				for (int column = 0; column < 10; column++)
				{
					ImU32 cell_bg_color = ImGui::GetColorU32(this->colum_colors[column]);					

					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

					ImGui::TableSetColumnIndex(column);
					if (column == 0)
					{
						ImGui::Text(std::to_string(row + 1).c_str());
					}
					else if (column == 1)
					{
						ImGui::Text(this->v_agent_sales[0][row].c_str());
					}
					else if (column == 2)
					{
						ImGui::Text(this->v_agent_sales[1][row].c_str());
					}
					else if (column == 3)
					{
						ImGui::Text(this->v_agent_sales[2][row].c_str());
					}
					else if (column == 4)
					{
						ImGui::Text(this->v_agent_sales[3][row].c_str());
					}
					else if (column == 5)
					{
						ImGui::Text(this->v_agent_sales[4][row].c_str());
					}
					else if (column == 6)
					{
						ImGui::Text(this->v_agent_sales[5][row].c_str());
					}
					else if (column == 7)
					{
						ImGui::Text(this->v_agent_sales[6][row].c_str());
					}
					else if (column == 8)
					{
						ImGui::Text(this->v_agent_sales[7][row].c_str());
					}
					else
					{						
						position = std::find(this->checked_sales.begin(), this->checked_sales.end(), row);
						present = (position != this->checked_sales.end()) ? true : false;
						
						ImGui::PushID(row);
						clicked = ImGui::Checkbox("##", &checked_statuses[row]);
						ImGui::PopID();
						
						//we update the data when the checkbox is clicked and it is currently not active to avoid adding and 
						//erasing at the same instance and vice versa
						if (clicked && present == true && !ImGui::IsItemActive()) 
						{
							this->checked_sales.erase(position);
						}
						else if (clicked && present == false && !ImGui::IsItemActive())
						{
							this->checked_sales.push_back(row);
						}
					}

				}
			}
		}
		ImGui::EndTable();
	}

	delete[] checked_statuses;
	checked_statuses = NULL;

	return 1;
}




