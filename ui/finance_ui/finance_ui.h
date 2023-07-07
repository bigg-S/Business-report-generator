#pragma once

#include <string>
#include <vector>

#include "imgui.h"
#include "app_common.h"
#include "regular_ui/regular_user_ui.h"
#include "change_password.h"


extern bool finance_window;

namespace FUserUi
{	

	class Profit_Loss_Window
	{
		/*Constructor and destructor*/
		public:
			Profit_Loss_Window(mongocxx::database db);
			Profit_Loss_Window() = default;
			~Profit_Loss_Window() = default;

		/*properties*/
		public:
			double expected_expenditure = 0.0;
			double total_sales = 0.0;
			double total_expenditure = 0.0;
			double profit_loss = 0.0;

			mongocxx::collection operational_costs;
			mongocxx::collection products_collection;

			bool show_window = false;
			
			ImGuiWindowFlags window_flags;

			//contains the arrays of the items in the data base eg. products, brands etc.
			bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays;
		/*methods*/
		public:
			int create_profit_loss_window();

			std::string truncate(double val);

	};

	class Window
	{
		public:
			Window() = default;
			~Window() = default;
			/*properties*/
		public:
			
			bool input_error = false;
			bool success = false;

			enum confirm_state { ACCEPTED = 0, REJECTED = 1, NONE = 2 };
			int confirm = confirm_state::NONE;

			bool  show_confirm_window = false;

			bool show_window = false;
			ImGuiWindowFlags window_flags;

			mongocxx::collection operational_costs;

			/*methods*/
		public:
			
			int handle_error(bool &error_flag);
			int notify_sale_status(bool &success_flag);
			int finance_confirm_message(int& confirm_flag);

	};


	class AddExpenseWindow : public Window
	{
		/*Constructor and destructor*/
		public:
			AddExpenseWindow(mongocxx::database db);
			AddExpenseWindow() = default;
			~AddExpenseWindow() = default;
			

		/*properties*/
	    public:
			std::string expense_name;
			static char c_expense_name[128];
			float cost = 0.0;

		/*methods*/
		public:
			int create_expense_window();

	};

	
	class RemoveExpenseWindow : public Window
	{
		/*Constructor and destructor*/
	public:
		RemoveExpenseWindow(mongocxx::database db);
		RemoveExpenseWindow() = default;
		~RemoveExpenseWindow() = default;

		/*properties*/
	public:
		std::string expense_names;
		std::string selected_expense;

		std::vector<std::string> expenses_array;

		//pointer used by combo box
		char* p_expenses;
		static int expense_current_item;

		/*methods*/
	public:
		int remove_expense_window();

		//create a combo box
		int create_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size);

		//replaces the items separator with '\0'
		void prepare_items_pointer(const std::string& items, char items_separator, char*& items_pointer);

		int set_expenses();

		std::string get_selected_expense(int selected_index);

		int fetch_expenses();
	};

	class SetExpenditureWindow : public Window
	{
		/*Constructor and destructor*/
	public:
		SetExpenditureWindow(mongocxx::database db);
		SetExpenditureWindow() = default;
		~SetExpenditureWindow() = default;

		/*properties*/
	public:
		float expenditure = 0.0;

		/*methods*/
	public:
		int set_expenditure_window();
	};

	class Expenses : public Window
	{
		/*Constructor and destructor*/
		public:
			Expenses(AddExpenseWindow &add, RemoveExpenseWindow &remove, SetExpenditureWindow& set);
			Expenses() = default;
			~Expenses() = default;

		/*properties*/
		public:
			AddExpenseWindow* add;
			RemoveExpenseWindow* remove;
			SetExpenditureWindow* set;

		/*methods*/
		public:
			int create_handle_expenses_window();

	};

	

	class ConfirmSaleWindow : public Window
	{
		//destructor and constructor
		public:
			ConfirmSaleWindow(mongocxx::database db, RUserUi::Window &window);
			ConfirmSaleWindow() = default;
			~ConfirmSaleWindow() = default;

		/*properties*/
		public:
			//strings used by the window
			std::string agents_details;
			
			/*
			*vector @ index 0 = products
			* 				1 = brands
			* 				2 = quantities
			* 				3 = seasons
			* 				4 = age groups
			* 				5 = genders
			* 				6 = locations
			* 				7 = item counts
			*
			*/
			std::vector<std::vector<std::string>> v_agent_sales;

			std::vector<int> checked_sales;
			//static bool* chacked_statuses;
			//selected agent
			std::string selected_agent;
			int64_t agent_id = 0;

			int i = 0;

			bool show_window = false;
			
			bool input_error = false;
			bool success = false;

			ImGuiWindowFlags window_flags;
			ImGuiTableFlags table_flags;
			std::vector<ImVec4> colum_colors =
			{
				ImVec4(0.25f, 0.8f, 0.11f, 0.65f),
				ImVec4(0.73f, 0.2f, 0.34f, 0.65f),
				ImVec4(0.26f, 0.6f, 0.2f, 0.65f),
				ImVec4(0.39f, 0.13f, 0.47f, 0.65f),
				ImVec4(0.8f, 0.55f, 0.22f, 0.65f),
				ImVec4(0.15f, 0.4f, 0.68f, 0.65f),
				ImVec4(0.69f, 0.7f, 0.4f, 0.65f),
				ImVec4(0.31f, 0.21f, 0.33f, 0.65f),
				ImVec4(0.45f, 0.38f, 0.72f, 0.65f),
				ImVec4(0.17f, 0.61f, 0.38f, 0.65f)
			};

			mongocxx::collection agents;
			mongocxx::collection agent_sales;
			mongocxx::collection products_collection;
			mongocxx::collection stock;

			RUserUi::Window* p_window;

			// Using those as a base value to create width/height that are factor of the size of our font
			//float TEXT_BASE_WIDTH = 0.0;
			float TEXT_BASE_HEIGHT = 0.0;

		/*methods*/
		public:
			int set_agents_details();

			int set_agent_sales();

			int64_t get_agent_id();

			int prepare_agents_details(std::string& m_agent_details, const std::string& agent_details_separator, const std::string& new_separator);
			
			int draw_sales_table(const float& m_TEXT_BASE_HEIGHT);
			
			int create_confim_sale_window();

	};


	class ShowFinanceWindow :public ConfirmSaleWindow, public Expenses , public Profit_Loss_Window
	{
		/*Constructor and destructor*/
		public:
			ShowFinanceWindow(Profit_Loss_Window& profit_or_loss, Expenses& expenses, ConfirmSaleWindow& confim_sale, PassWord& change_password);
			~ShowFinanceWindow() = default;

		/*properties*/
		public:
			Profit_Loss_Window* profit_or_loss;
			Expenses* expenses;
			ConfirmSaleWindow* confim_sale;
			PassWord* change_password;

			bool show_window = false;
			ImGuiWindowFlags window_flags;

		/*methods*/
		public:
			int create_show_finance_window();
	};
}