#pragma once

#include <string>
#include <vector>

#include "imgui.h"
#include "app_common.h"
#include "change_password.h"

extern bool regular_user_window;

//Regular user's user-interface
namespace RUserUi
{
	class Window
	{
		//destructor and constructor
		public:
			Window();
			~Window();
		/*properties*/
		public:

			//strings used by the window
			std::string products;
			std::string brands;
			std::string quantities;

			//common vectors used by the window
			std::vector<std::string> products_array;
			std::vector<std::string> brands_array;
			std::vector<std::string> quantities_array;
			std::vector<int> amounts_array;

			//vectors containing deatails about a product
			std::vector<std::vector<std::string>> brand_quantities_array;
			std::vector<std::vector<int>> brand_amounts_array;

			//vectors for all items in the database used by the window
			std::vector<std::vector<std::string>> all_brands_array;
			std::vector<std::vector< std::vector<std::string>>> all_quantities_array;
			std::vector<std::vector< std::vector<int>>> all_amounts_array;

			//pointers used by combo boxes
			char* p_brands;
			char* p_quantities;

			//selected items
			std::string selected_product;
			std::string selected_brand;
			std::string selected_quantity;

			bool show_window = false;
			bool product_changed = false;

			ImGuiWindowFlags window_flags;

			//database collections
			mongocxx::collection products_collection;
			mongocxx::collection stock;
			mongocxx::collection agents;

		/*methods*/
		public:
			//replaces the items separator with '\0'
			void prepare_items_pointer(const std::string& items, char items_separator, char*& items_pointer);

			//display all the products
			int  display_all_products();

			//display all the brands if product is selected
			int  display_all_brands(std::string product);

			//display all the quantities for a particular brand
			int  display_brand_quantities(std::string product, std::string brand);
			
			//display a single quantity
			int  display_quantity(std::string product, std::string brand, std::string quantity);


			//strings setters
			int set_products();
			int set_brands(const std::string& m_brands);
			int set_quantities(const std::string& m_quantities);

			//vector setters
			int set_products_array(const std::vector<std::string>& m_products);
			int set_brands_array(const std::vector<std::string>& m_brands);
			int set_quantities_array(const std::vector<std::string>& m_quantities);

			//getters
			std::string get_selected_product();
			std::string get_selected_brand(int selected_index);
			std::string get_selected_quantity(int selected_index);

			int reset_combo_items();

			/*---------------------------Entities------------------------*/
			//create a list box
			int create_listbox_filter(std::string items, std::string items_description, std::string& selected_item, char item_separator, ImVec2 size, std::string caller = "Regular User");
			//create a combo box
			int create_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size);
	};

	class StockWindow : public Window
	{
		//destructor and constructor
		public:
			StockWindow(mongocxx::database db);
			StockWindow() = default;
			~StockWindow() = default;
		/*properties*/
		public:
			static int stock_brand_current_item;
			static int stock_quantity_current_item;
		/*methods*/
		public:
			int create_stock_window();

	};

	class SellWindow : public Window
	{
		public:
			SellWindow(mongocxx::database db);
			SellWindow() = default;
			~SellWindow();

		/*properties*/
		public:
			//strings used by this window
			std::string seasons;
			std::string age_groups;
			std::string genders;
			std::string locations;

			//common vectors used by the window
			std::vector<std::string> seasons_array;
			std::vector<std::string> age_groups_array;
			std::vector<std::string> genders_array;
			std::vector<std::string> locations_array;


			//pointers used by combo boxes
			char* p_seasons;
			char* p_age_groups;
			char* p_genders;
			char* p_locations;

			//selected items for this window
			std::string selected_season;
			std::string selected_age_group;
			std::string selected_gender;
			std::string selected_location;

			static int sell_brand_current_item;
			static int sell_quantity_current_item;
			static int sell_season_current_item;
			static int sell_age_group_current_item;
			static int sell_gender_current_item;
			static int sell_location_current_item;

			static bool input_error;
			static bool success;

			//number of items to be sold
			static int number_of_items;

		/*methods*/
		public:
			int create_sell_window();

			//setters
			int set_seasons();
			int set_age_groups();
			int set_genders();
			int set_locations();

			int populate_brands(std::string product);
			int populate_quantities(std::string brand);

			//getters
			std::string get_selected_season();
			std::string get_selected_age_group();
			std::string get_selected_gender();
			std::string get_selected_location();
			int get_number_of_items();

			int handle_error(bool& err_flag);

			int notify_sale_status();
	};
		

	class AgentsWindow
	{
		//destructor and constructor
		public:
			AgentsWindow(mongocxx::database db);
			AgentsWindow() = default;
			~AgentsWindow();

		/*properties*/
		public:			
			
			/*vectors to store all data*/
			std::vector<std::string> ids;
			std::vector<std::string> names;
			std::vector<std::string> phones;
			std::vector<std::string> locations;

			/*vectors to store filtered data*/
			std::vector<std::string> filtered_ids;
			std::vector<std::string> filtered_names;
			std::vector<std::string> filtered_phones;
			std::vector<std::string> filtered_locations;

			/*
			index 0 = ids
				  1 = names
				  2 = phones
				  3 = locations
			*/
			std::vector<std::vector<std::string>> agent_details;

			std::string filter_types = "No Criteria Selected_Filter by Name_Filter by Phone_Filter by Location_";

			char* p_filter_types;

			static int current_filter_type_index;

			bool show_window = false;

			ImGuiWindowFlags window_flags;
			ImGuiTableFlags table_flags;

			mongocxx::collection agents;

			// Using those as a base value to create width/height that are factor of the size of our font
			//float TEXT_BASE_WIDTH = 0.0;
			float TEXT_BASE_HEIGHT = 0.0;

		/*methods*/
	   public:
		   int create_search_filter_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size);
		   //replaces the items separator with '\0'
		   void prepare_filter_types_pointer(const std::string& items, char items_separator, char*& items_pointer);

		   int create_agents_filter();

		   int draw_agents_table(const float& m_TEXT_BASE_HEIGHT);

		   int create_agents_window();
	};

	class ReturnSaleWindow : public AgentsWindow, public SellWindow
	{
		/*Constructor and Detructor*/
	public:
		ReturnSaleWindow(mongocxx::database db);
		ReturnSaleWindow() = default;
		~ReturnSaleWindow() = default;

		/*properties*/
	public:

		static int return_current_filter_type_index;
		std::vector<int> checked_sales;

		std::vector<std::vector<std::string>> sales_data;


		std::vector<std::string> sales_product_names;
		std::vector<std::string> sales_brands;
		std::vector<std::string> sales_quantities;
		std::vector<std::string> sales_seasons;
		std::vector<std::string> sales_age_groups;
		std::vector<std::string> sales_genders;
		std::vector<std::string> sales_locations;
		std::vector<std::string> sales_item_counts;
		std::vector<std::string> sales_dates_and_times;
		std::vector<std::string> sales_indices;

		std::vector<std::string> filtered_sales_product_names;
		std::vector<std::string> filtered_sales_brands;
		std::vector<std::string> filtered_sales_quantities;
		std::vector<std::string> filtered_sales_seasons;
		std::vector<std::string> filtered_sales_age_groups;
		std::vector<std::string> filtered_sales_genders;
		std::vector<std::string> filtered_sales_locations;
		std::vector<std::string> filtered_sales_item_counts;
		std::vector<std::string> filtered_sales_dates_and_times;


		size_t sales_count = 0;

		enum confirm_state { ACCEPTED = 0, REJECTED = 1, NONE = 2 };
		int confirm = confirm_state::NONE;
		bool  show_confirm_window = false;

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
			ImVec4(0.17f, 0.61f, 0.38f, 0.65f),
			ImVec4(0.28f, 0.11f, 0.88f, 0.65f)
		};

		std::string sales_filter_types = "No Criteria Selected_Filter by Name_Filter by Location_Filter by Date&Time_";
		char* p_sales_filter_types;

		mongocxx::collection products_collection;
		mongocxx::collection stock;

		bool success = false;
		bool input_error = false;

		SALE sale;

		bool show_window = false;
		ImGuiWindowFlags window_flags;
		ImGuiTableFlags table_flags;

		// Using those as a base value to create width/height that are factor of the size of our font
			//float TEXT_BASE_WIDTH = 0.0;
		float TEXT_BASE_HEIGHT = 0.0;

		/*methods*/
	public:
		int create_return_sale_window();
		int create_sales_data_filter();
		int regular_user_confirm_message(int& confirm_flag);
		int draw_sales_data_table(const float& m_TEXT_BASE_HEIGHT);
	};

	class ShowRegularUserWindow :public StockWindow, public ReturnSaleWindow
	{
		/*Constructor and destructor*/
	public:
		ShowRegularUserWindow(StockWindow& stock_win, SellWindow& sell_win, ReturnSaleWindow& return_sale_win, AgentsWindow& agents_win, PassWord& change_password);
		~ShowRegularUserWindow() = default;

		/*properties*/
	public:
		StockWindow* stock_win;
		SellWindow* sell_win;
		ReturnSaleWindow* return_sale_win;
		AgentsWindow* agents_win;
		PassWord* change_password;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_show_regular_user_window();
	};

	
}