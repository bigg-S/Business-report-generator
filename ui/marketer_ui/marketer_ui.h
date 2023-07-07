#pragma once

#include "app_common.h"
#include "retrieve_data.h"
#include "regular_ui/regular_user_ui.h"
#include "change_password.h"

#include <imgui.h>

#include <glfw\glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

#include "stdafx.h"
#include <thread>
#include <Windows.h>

extern bool marketer_window;

namespace MUserUi
{

	class GeneralReportWindow
	{
		/*Constructors and Destructor*/
		public:
			GeneralReportWindow(mongocxx::database db, GLFWwindow* window);
			GeneralReportWindow() = default;
			~GeneralReportWindow() = default;

		/*Properties*/
		public:
			mongocxx::collection sales_collection;

			std::vector<CategoryData> v_seasons;
			std::vector<CategoryData> v_age_groups;
			std::vector<CategoryData> v_genders;
			std::vector<CategoryData> v_locations;

			std::string  file_name;
			bool saving = false;
						
			std::string seasons_data;
			std::string age_groups_data;
			std::string genders_data;
			std::string locations_data;

			bool show_window = false;
			ImGuiWindowFlags window_flags;

			Word::_ApplicationPtr pWordApp;

			GLFWwindow* window;
			HWND main_window;

		/*Methods*/
		public:
			int create_general_report_window();
			
			int display_report();

			int display_category_data(std::vector<CategoryData> v_category, std::string category_name, std::string& stored_category_data);

			std::string get_file_name(HWND hwnd);

			int LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color,
				const ImVec4& backdrop_color, const int circle_count, const float speed);

			void create_doc();
			
			
	};

	class CustomizedReportWindow : public GeneralReportWindow , public RUserUi::Window
	{
		/*Constructor and Destructor*/
		public:
			CustomizedReportWindow(mongocxx::database db, GLFWwindow* window);
			CustomizedReportWindow() = default;
			~CustomizedReportWindow() = default;

		/*properties*/
		public:
			static int current_selected_brand;
			int product_index;

			std::vector<ProductData> v_products;

			
			std::string customized_data = "";

			bool show_window = false;
			ImGuiWindowFlags window_flags;
		/*methods*/
		public:
			int create_customized_report_window();

			int populate_products();
			int populate_brands(std::vector<BrandData> v_brands);

			int display_products_sales();
			int display_brands_sales();
			int display_brand_quantities_sales();

			void create_custom_doc();

	};


	class WordDocument
	{
		/*Constructor and Destructor*/
	public:
		WordDocument(GeneralReportWindow *gr_window, CustomizedReportWindow* cr_window);
		WordDocument() = default;
		~WordDocument() = default;

		/*properties*/
	//private:

		_variant_t fls = false;

		std::string  file_name;

		Word::_ApplicationPtr pWordApp;
		Word::DocumentsPtr dptr;
		Word::_DocumentPtr pDoc;

		Word::ParagraphsPtr p_Paras;
		Word::ParagraphPtr p_Para;
		Word::RangePtr p_ParaRng;

		Word::_ParagraphFormatPtr paraformat;

		GeneralReportWindow* gr_window;
		CustomizedReportWindow* cr_window;

		/*methods*/
	public:
		void initialize();
		void uninitialize();

		int create_document(std::string caller);

	};

	class ShowMarketerWindow : public CustomizedReportWindow
	{
		/*Constructor and destructor*/
	public:
		ShowMarketerWindow(GeneralReportWindow& gr_window, CustomizedReportWindow& cr_window, PassWord& change_password);
		~ShowMarketerWindow() = default;

		/*properties*/
	public:
		GeneralReportWindow* gr_window;
		CustomizedReportWindow* cr_window;
		PassWord* change_password;

		bool show_window = false;
		ImGuiWindowFlags window_flags;

		/*methods*/
	public:
		int create_show_marketer_window();
	};
}
