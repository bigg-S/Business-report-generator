#pragma once

#include <string>
#include <vector>

#include <imgui.h>
#include "finance_ui/finance_ui.h"
#include "regular_ui/regular_user_ui.h"
#include "change_password.h"


extern bool admin_window;

extern bool showing_admin_extensions;

extern bool log_out;

namespace AdminUi
{
	
	class AddQuantityWindow : public FUserUi::Window, public RUserUi::Window
	{
		/*Constructor and Detructor*/
	public:
		AddQuantityWindow(mongocxx::database db);
		AddQuantityWindow() = default;
		~AddQuantityWindow() = default;

		/*properties*/
	public:
		std::string quantity_name;
		static char c_quantity_name[128];
		static char c_new_brand_quantity_name[128];
		static char c_new_quantity_name[128];

		float buying_price = 0.0;
		float selling_price = 0.0;
		int amount = 0;

		static int a_current_brand_index;
		
		bool finish = false;

		bool minput_error = false;
		bool msuccess = false;

				
		enum confirm_state { ACCEPTED = 0, REJECTED = 1, NONE = 2 };
		int confirm = confirm_state::NONE;

		bool  show_confirm_window = false;

		bool duplicate_error = false;

		mongocxx::collection  products_collection;
		mongocxx::collection stock;

		std::vector<std::string> v_products;
		std::vector<std::string> v_brands;

		std::vector<char> unwanted_chars = { '_', '\\', ';', '\'', ',','.', '/' };

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_quantity_window(std::string& quantities, double& selling_price, double& buying_price, int& number_of_items, std::string caller);
		int admin_error_message(bool& error_flag);
		int admin_confirm_message(int& confirm_flag);
		
		int mset_quantities(std::vector<std::string> vquantities);
		int mset_brands(std::vector<std::string> vbradns);
		int mset_products();
		
		bool check_unwanted_characters(std::string entered_string);
		bool check_for_duplicate(std::vector<std::string> items, std::string item);
	};

	class AddBrandWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		AddBrandWindow(mongocxx::database db);
		AddBrandWindow() = default;
		~AddBrandWindow() = default;

	public:
		/*properties*/
		
		std::string brand_name;
		static char c_brand_name[128];
		static char c_n_brand_name[128];


		int quantity_index = 0;

		bool finish = false;

		AddQuantityWindow quantity_win;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_brand_window(std::string product_name, std::string& brand, std::vector<std::string>& quantities, std::vector<double>& selling_price, std::vector<double>& buying_price, std::vector<int>& number_of_items, std::string caller);
	};



	class AddProductWindow : public AddBrandWindow
	{
		/*Constructor and Detructor*/
	public:
		AddProductWindow(mongocxx::database db);
		AddProductWindow() = default;
		~AddProductWindow() = default;

		/*properties*/
	public:
		std::string product_name;
		static char c_product_name[128];

		int brand_index = 0;
				
		std::vector<std::string> brand_names;
		std::vector<std::vector<std::string>> qty_names;
		std::vector<std::vector<double>> selling_price;
		std::vector<std::vector<double>> buying_price;
		std::vector<std::vector<int>> number_of_items;

		AddBrandWindow  brand_win;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_product_window();
	};


	class AddNewBrandWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		AddNewBrandWindow(mongocxx::database db);
		AddNewBrandWindow() = default;
		~AddNewBrandWindow() = default;

		/*properties*/
	public:
		std::string brand_name;
		

		int n_quantity_index = 0;

		std::vector<std::string> n_qty_names;
		std::vector<double> n_selling_price;
		std::vector<double> n_buying_price;
		std::vector<int> n_number_of_items;

		AddBrandWindow  brand_win;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_new_brand_window();
	};


	class AddNewQuantityWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		AddNewQuantityWindow(mongocxx::database db);
		AddNewQuantityWindow() = default;
		~AddNewQuantityWindow() = default;

		/*properties*/
	public:
		std::string quantity_name;
		static char c_n_quantity_name[128];
		
		double n_selling_price = 0.0;
		double n_buying_price = 0.0;
		int n_number_of_items = 0;

		AddQuantityWindow quantity_win;
		
		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_new_quantity_window();
	};
	

	class AddItemWindow
	{
		/*Constructor and Detructor*/
		public:
			AddItemWindow(AddProductWindow &add_product, AddNewBrandWindow&add_brand, AddNewQuantityWindow &add_quantity);
			~AddItemWindow() = default;

		/*properties*/
		public:
			AddProductWindow* add_product;
			AddNewBrandWindow* add_brand;
			AddNewQuantityWindow* add_quantity;

			bool show_window = false;
			ImGuiWindowFlags window_flags;

		/*methods*/
		public:
			int create_add_items_window();

	};

	class RemoveProductWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		RemoveProductWindow(mongocxx::database db);
		RemoveProductWindow() = default;
		~RemoveProductWindow() = default;

		/*properties*/
	public:
		
		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_remove_product_window();
	};

	class RemoveBrandWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		RemoveBrandWindow(mongocxx::database db);
		RemoveBrandWindow() = default;
		~RemoveBrandWindow() = default;

		/*properties*/
	public:
		std::string brand_name;
		static int r_brand_current_item;
		
		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_remove_brand_window();
	};

	class RemoveQuantityWindow : public AddQuantityWindow
	{
		/*Constructor and Detructor*/
	public:
		RemoveQuantityWindow(mongocxx::database db);
		RemoveQuantityWindow() = default;
		~RemoveQuantityWindow() = default;

		/*properties*/
	public:
		static int r_quantity_brand_current_item;
		static int r_quantity_current_item;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_remove_quantity_window();
	};

	
	class RemoveItemWindow
	{
		/*Constructor and Detructor*/
	public:
		RemoveItemWindow(RemoveProductWindow&  remove_product, RemoveBrandWindow& remove_brand, RemoveQuantityWindow& remove_quantity);
		~RemoveItemWindow() = default;

		/*properties*/
	public:
		RemoveProductWindow* remove_product;
		RemoveBrandWindow* remove_brand;
		RemoveQuantityWindow* remove_quantity;
		

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_remove_items_window();

	};

	class AddorRemoveItemWindow
	{
		/*Constructor and Detructor*/
	public:
		AddorRemoveItemWindow(AddItemWindow& add_item, RemoveItemWindow& remove_item);
		AddorRemoveItemWindow() = default;
		~AddorRemoveItemWindow() = default;

		/*properties*/
	public:
		AddItemWindow* add_item;
		RemoveItemWindow* remove_item;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_or_remove_items_window();

	};

	class AddOthersWindow : public AddQuantityWindow
	{
			/*Constructor and Detructor*/
		public:
			AddOthersWindow(mongocxx::database db);
			AddOthersWindow() = default;
			~AddOthersWindow();

			/*properties*/
		public:
			
			char* p_others;
			std::string others = "Select_Season_Age Group_Location_";
			std::vector<std::string> v_others = { "Season" , "Age Group",  "Location"};

			static int others_current_item;
			static char other_name[128];
			
			std::string s_other_name;
			

			bool show_window = false;
			ImGuiWindowFlags window_flags;

			/*methods*/
		public:
			int create_add_others_window();
	};


	class RemoveOthersWindow : public AddQuantityWindow
	{	
			/*Constructor and Detructor*/
		public:
			RemoveOthersWindow(mongocxx::database db);
			RemoveOthersWindow() = default;
			~RemoveOthersWindow();

			/*properties*/
		public:

			char* p_others;
			std::string others = "Select_Season_Age Group_Location_";

			char* p_selected_others;			
			std::string selected_others = "Select_";

			std::string selected_other;
			std::vector<std::string> v_selected_others;

			static int r_others_current_item;
			static int r_other_selected_current_item;

			std::string s_other_name;


			bool show_window = false;
			ImGuiWindowFlags window_flags;

			/*methods*/
		public:
			int create_remove_others_window();
			int mset_others(std::vector<std::string> v_s_others);
			
		
	};

	class AddorRemoveOthersWindow
	{
			/*Constructor and Detructor*/
		public:
			AddorRemoveOthersWindow(AddOthersWindow& add_other, RemoveOthersWindow& remove_other);
			AddorRemoveOthersWindow() = default;
			~AddorRemoveOthersWindow() = default;

			/*properties*/
		public:
			AddOthersWindow* add_other;
			RemoveOthersWindow* remove_other;

			bool show_window = false;
			ImGuiWindowFlags window_flags;

			/*methods*/
		public:
			int create_add_or_remove_others_window();

	};

	
	class RemoveAgentWindow : public AddQuantityWindow, public RUserUi::AgentsWindow
	{
		/*Constructor and Detructor*/
	public:
		RemoveAgentWindow(mongocxx::database db);
		RemoveAgentWindow() = default;
		~RemoveAgentWindow() = default;

		/*properties*/
	public:

		static int remove_current_filter_type_index;
		std::vector<int> checked_agents;

		std::string agent_name = "";
		std::string agent_id = "";

		int agents_count = 0;
		
		std::vector<ImVec4> colum_colors =
		{
			ImVec4(0.25f, 0.8f, 0.11f, 0.65f),
			ImVec4(0.73f, 0.2f, 0.34f, 0.65f),
			ImVec4(0.26f, 0.6f, 0.2f, 0.65f),
			ImVec4(0.39f, 0.13f, 0.47f, 0.65f),
			ImVec4(0.8f, 0.55f, 0.22f, 0.65f)
		};

		std::string admin_filter_types = "No Criteria Selected_Filter by ID_Filter by Name_Filter by Phone_Filter by Location_";

		char* p_admin_filter_types;

		mongocxx::collection agents;
		mongocxx::collection agent_sales;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_remove_agent_window();
		int create_admin_agents_filter();
		int draw_admin_agents_table(const float& m_TEXT_BASE_HEIGHT);
	};

	class AddAgentWindow : public RemoveAgentWindow
	{
		/*Constructor and Detructor*/
	public:
		AddAgentWindow(mongocxx::database db);
		AddAgentWindow() = default;
		~AddAgentWindow() = default;

		/*properties*/
	public:
		std::string s_agent_name = "";
		std::string s_agent_location = "";
		std::string s_agent_phone = "";
		std::string s_agent_id = "";

		static char agent_name[128];
		static char agent_phone[13];
		static char agent_location[128];

		bool show_id = false;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_agent_window();


	};


	class AddorRemoveAgentsWindow
	{
		/*Constructor and Detructor*/
	public:
		AddorRemoveAgentsWindow(AddAgentWindow& add_agent, RemoveAgentWindow& remove_agent);
		AddorRemoveAgentsWindow() = default;
		~AddorRemoveAgentsWindow() = default;

		/*properties*/
	public:
		AddAgentWindow* add_agent;
		RemoveAgentWindow* remove_agent;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_add_or_remove_agents_window();

	};

	class ShowAdminWindow :public AddorRemoveItemWindow, public AddorRemoveOthersWindow, public AddorRemoveAgentsWindow, public PassWord
	{
		/*Constructor and destructor*/
	public:
		ShowAdminWindow(AddorRemoveItemWindow& add_or_remove_item, AddorRemoveOthersWindow& add_or_remove_others, AddorRemoveAgentsWindow& add_or_remove_agents, PassWord& change_password);
		~ShowAdminWindow() = default;

		/*properties*/
	public:
		AddorRemoveItemWindow* add_or_remove_item;
		AddorRemoveOthersWindow* add_or_remove_others;
		AddorRemoveAgentsWindow* add_or_remove_agents;
		PassWord* change_password;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_show_admin_window();
	};

}