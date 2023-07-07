

#include "admin_ui.h"
#include "insert_components.h"
#include "retrieve_data.h"
#include "delete_components.h"
#include "imgui_internal.h"
#include <regex>

#include <algorithm>
#include <agents.h>
#include <marketer_ui/marketer_ui.h>
#include "regular_ui/regular_user_ui.h"
#include "login_ui/login_ui.h"
#include "change_password.h"

bool admin_window = false;
bool showing_admin_extensions = false;
bool log_out = false;

AdminUi::ShowAdminWindow::ShowAdminWindow(AddorRemoveItemWindow& add_or_remove_item, AddorRemoveOthersWindow& add_or_remove_others, AddorRemoveAgentsWindow& add_or_remove_agents, PassWord& change_password)
{
	this->add_or_remove_item = &add_or_remove_item;
	this->add_or_remove_others = &add_or_remove_others;
	this->add_or_remove_agents = &add_or_remove_agents;
	this->change_password = &change_password;
}

int AdminUi::ShowAdminWindow::create_show_admin_window()
{
	
	if (showing_admin_extensions == false)
	{
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Products", ImVec2(115.0, 20.0)))
			this->add_or_remove_item->show_window = true;

		if (this->add_or_remove_item->show_window)
			this->add_or_remove_item->create_add_or_remove_items_window();


		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Others", ImVec2(115.0, 20.0)))
			this->add_or_remove_others->show_window = true;

		if (this->add_or_remove_others->show_window)
			this->add_or_remove_others->create_add_or_remove_others_window();


		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Agents", ImVec2(115.0, 20.0)))
			this->add_or_remove_agents->show_window = true;

		if (this->add_or_remove_agents->show_window)
			this->add_or_remove_agents->create_add_or_remove_agents_window();
	}
	

	if (finance_window != true && marketer_window != true && regular_user_window != true)
	{
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Finance", ImVec2(115.0, 20.0)))
			finance_window = true;

		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Reports", ImVec2(115.0, 20.0)))
			marketer_window = true;

		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Regular", ImVec2(115.0, 20.0)))
			regular_user_window = true;

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(6 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(6 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(6 / 7.0f, 0.8f, 0.8f));	
		
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Log Out", ImVec2(115.0, 20.0)))
		{
			login_window = true;
			admin_window = false;
			showing_admin_extensions = false;
		}
			
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.8f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.8f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.8f / 7.0f, 0.8f, 0.8f));

		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Change Password", ImVec2(115.0, 20.0)))
			this->change_password->show_window = true;
		ImGui::PopStyleColor(3);

		if (this->change_password->show_window)
			this->change_password->create_password_window();

		
	}
	else
		showing_admin_extensions = true;

	if (showing_admin_extensions == true)
	{
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(50.0, 0.0);
		if (ImGui::Button("Back", ImVec2(115.0, 20.0)))
		{
			showing_admin_extensions = false;
			finance_window = false;
			marketer_window = false;
			regular_user_window = false;
		}
	}


	return 1;
}


AdminUi::AddorRemoveItemWindow::AddorRemoveItemWindow(AddItemWindow& add_item, RemoveItemWindow& remove_item)
{
	this->add_item    = &add_item;
	this->remove_item = &remove_item;
}

int AdminUi::AddorRemoveItemWindow::create_add_or_remove_items_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Add Or Remove Item", &this->show_window, this->window_flags);

	if (this->add_item->show_window)
		this->add_item->create_add_items_window();

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Add Item", ImVec2(145.0, 20.0)))
		this->add_item->show_window = true;

	if (this->remove_item->show_window)
		this->remove_item->create_remove_items_window();

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Remove Item", ImVec2(145.0, 20.0)))
		this->remove_item->show_window = true;	


	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}

AdminUi::AddItemWindow::AddItemWindow(AddProductWindow& add_product, AddNewBrandWindow& add_brand, AddNewQuantityWindow& add_quantity)
{
	this->add_product	= &add_product;
	this->add_brand     = &add_brand;
	this->add_quantity  = &add_quantity;
}

int AdminUi::AddItemWindow::create_add_items_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Add Item", &this->show_window, this->window_flags);

	if (this->add_product->show_window)
		 this->add_product->create_add_product_window();

	if (this->add_product->success == true)
		this->add_product->notify_sale_status(this->add_product->success);

	if (this->add_product->finish)
	{
		this->add_product->show_window = false;
		this->add_product->finish = false;
	}
	
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Product", ImVec2(145.0, 20.0)))
		this->add_product->show_window = true;


	if (this->add_brand->brand_win.show_window)
		this->add_brand->create_add_new_brand_window();

	if (this->add_brand->brand_win.success == true)
		this->add_brand->brand_win.notify_sale_status(this->add_brand->brand_win.success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Brand", ImVec2(145.0, 20.0)))
		this->add_brand->brand_win.show_window = true;

	if (this->add_quantity->quantity_win.show_window)
		this->add_quantity->create_add_new_quantity_window();
	
	if (this->add_quantity->quantity_win.success == true)
		this->add_quantity->notify_sale_status(this->add_quantity->quantity_win.success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Quantity", ImVec2(145.0, 20.0)))
			this->add_quantity->quantity_win.show_window = true;
	

	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}

AdminUi::AddProductWindow::AddProductWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];
}

char AdminUi::AddProductWindow::c_product_name[128] = "";

int AdminUi::AddProductWindow::create_add_product_window()
{

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 250), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 180), ImGuiCond_Once);

	ImGui::Begin("Add Product", &this->show_window, this->window_flags);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Product Name: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint("###", "name", this->c_product_name, IM_ARRAYSIZE(this->c_product_name));

	

	//handle any error
	if (this->input_error == true)
		this->admin_error_message(this->input_error);
		
	//handle any brand window error
	if (this->brand_win.input_error == true)
		this->admin_error_message(this->brand_win.input_error);
	else if (this->brand_win.success == true)
		this->notify_sale_status(this->brand_win.success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(20.0, 0.0);

	if (this->brand_win.finish == true && this->brand_win.input_error == false)
	{
		//check for a duplicate
		std::vector<std::string> previous_brands = this->brand_names;
		previous_brands.pop_back();
		bool status = check_for_duplicate(previous_brands, this->brand_names[this->brand_names.size() - 1]);

		if (status == true)
		{
			this->brand_names[this->brand_names.size() - 1] = ""; //this will ensure that it is poped back from the vector in a later if statement 
			this->input_error = true;
		}

		this->brand_win.show_window = false;
		this->brand_win.finish = false;
		this->brand_index++;
	}

	if(this->brand_win.show_window == true)
		this->brand_win.create_add_brand_window(this->product_name, this->brand_names[brand_index], this->qty_names[brand_index], this->selling_price[brand_index], this->buying_price[brand_index], this->number_of_items[brand_index], "Product");
	else if (this->brand_win.show_window == false && this->brand_names.size() != 0 && this->brand_names[this->brand_names.size() - 1].compare("") == 0)
	{
		this->brand_names.pop_back();
		this->qty_names.pop_back();
		this->selling_price.pop_back();
		this->buying_price.pop_back();
		this->number_of_items.pop_back();

		if(this->brand_index != 0)
			this->brand_index--;
	}


	if (ImGui::Button("Add Brand", ImVec2(120, 0)) && this->brand_win.show_window == false)
	{
		this->product_name = this->c_product_name;
		if (this->product_name.compare("") != 0)
		{
			std::vector<std::string> m_quantities = {};
			this->qty_names.push_back(m_quantities);

			std::vector<double> prices = {};
			this->selling_price.push_back(prices);

			this->buying_price.push_back(prices);

			std::vector<int> nums = {};
			this->number_of_items.push_back(nums);

			this->brand_names.push_back("");

			this->brand_win.show_window = true;
		}
		else
		{
			this->input_error = true;
		}
		
		
	}
	

	ImGui::SameLine(150.0, 0.0);

	if (ImGui::Button("Finish", ImVec2(120, 0)))
	{
		this->product_name = this->c_product_name;

		bool found_unwanted_char = check_unwanted_characters(this->product_name);

		if ((this->product_name.compare("") == 0) || (this->brand_names.size() == 0) || found_unwanted_char == true)
		{
			this->input_error = true;
		}
		else
		{			
			bool status = check_for_duplicate(v_products, this->product_name);

			if (status == true)
			{
				this->input_error = true;
			}
			else
			{
				insert_product(this->products_collection, this->stock, this->product_name, this->brand_names, this->qty_names, this->selling_price, this->buying_price, this->number_of_items);
				this->success = true;

				this->brand_index = 0;
				this->finish = true;
			}

		}

	}


	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}

AdminUi::AddBrandWindow::AddBrandWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];

	this->mset_products();
}

char AdminUi::AddBrandWindow::c_brand_name[128] = "";
char AdminUi::AddBrandWindow::c_n_brand_name[128] = "";
int AdminUi::AddBrandWindow::create_add_brand_window(std::string product_name, std::string &brand, std::vector<std::string> &quantities, std::vector<double> &selling_price, std::vector<double> &buying_price, std::vector<int> &number_of_items, std::string caller)
{
	ImVec2 size = ImVec2(300, 190);
	std::string label = "Add Brand";
	std::string quantity_caller = "Brand";
	if (caller.compare("Product") != 0)
	{
		size = ImVec2(350, 350);
		label = "Add New Brand";
		quantity_caller = "New Brand";
	}
		

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 350), ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);

	ImGui::Begin(label.c_str(), &this->show_window, this->window_flags);

	if (caller.compare("Product") != 0)
	{		
		ImGui::SameLine(90.0, 0.0);
		this->mset_products();
		this->create_listbox_filter(this->products, "Product", this->selected_product, '_', ImVec2(210, 150));
	}


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Brand Name: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);
	if (caller.compare("Product") == 0)
		ImGui::InputTextWithHint("###", "name", this->c_brand_name, IM_ARRAYSIZE(this->c_brand_name));
	else
		ImGui::InputTextWithHint("###", "name", this->c_n_brand_name, IM_ARRAYSIZE(this->c_n_brand_name));


	//handle any error
	if (this->quantity_win.input_error == true)
		this->admin_error_message(this->quantity_win.input_error);
	
	

	if (this->quantity_win.finish == true && this->quantity_win.minput_error == false)
	{
		//check for a duplicate
		std::vector<std::string> previous_quantities = quantities;
		previous_quantities.pop_back();
		bool status = check_for_duplicate(previous_quantities, quantities[quantities.size() - 1]);

		if (status == true)
		{
			quantities[quantities.size() - 1] = ""; //this will ensure that it is poped back from the vector in a later if statement
			this->quantity_win.input_error = true;
		}			

		this->quantity_win.show_window = false;
		this->quantity_win.finish = false;
		this->quantity_index++;
	}

	if (this->quantity_win.show_window == true)
		quantity_win.create_add_quantity_window(quantities[quantity_index], selling_price[quantity_index], buying_price[quantity_index], number_of_items[quantity_index], quantity_caller);
	else if (this->quantity_win.show_window == false &&  quantities.size() != 0 && quantities[quantities.size() - 1].compare("") == 0)
	{
		quantities.pop_back();
		selling_price.pop_back();
		buying_price.pop_back();
		number_of_items.pop_back();

		if(this->quantity_index != 0)
			this->quantity_index--;
	}


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(20.0, 0.0);

	if (ImGui::Button("Add Quantity", ImVec2(120, 0)) && this->quantity_win.show_window == false)
	{
		if (caller.compare("Product") == 0)
			this->brand_name = this->c_brand_name;
		else
			this->brand_name = this->c_n_brand_name;

		

		if (this->brand_name.compare("") != 0 )
		{
			quantities.push_back("");
			selling_price.push_back(0.0);
			buying_price.push_back(0.0);
			number_of_items.push_back(0);

			this->quantity_win.show_window = true;

			this->brand_name = this->selected_product + "_" + this->brand_name;
		}
		else
			this->input_error = true;

		
				
	}

	ImGui::SameLine(150.0, 0.0);

	if (ImGui::Button("Finish", ImVec2(120, 0)))
	{
		if (caller.compare("Product") == 0)
		{
			this->brand_name = this->c_brand_name;
			brand = product_name + "_" + this->brand_name;
		}			
		else
		{
			this->brand_name = this->c_n_brand_name;
			brand = this->selected_product + "_" + this->brand_name;
		}
		

		bool found_unwanted_char = check_unwanted_characters(this->brand_name);

		if ((this->brand_name.compare("") == 0) || quantities.size() == 0 || found_unwanted_char == true)
		{
			this->input_error = true;			
		}
		else
		{
			if (caller.compare("Product") != 0)
			{
				if (this->selected_product.compare("All Products") != 0)
				{
					std::vector<std::string> v_brands = get_product_brands_array(this->products_collection, this->selected_product);
					bool status = check_for_duplicate(v_brands, this->brand_name);
					
					if (status == false)
					{
						insert_brand(this->products_collection, this->stock, brand, quantities, selling_price, buying_price, number_of_items, this->selected_product);
						this->success = true;
					}						
					else
						this->input_error = true;
				}
				else
					this->input_error = true;
				
			}

			quantity_index = 0;
			this->finish = true;
		}		

	}


	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}

AdminUi::AddQuantityWindow::AddQuantityWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];
	this->mset_products();

}

int AdminUi::AddQuantityWindow::a_current_brand_index = 0;
char AdminUi::AddQuantityWindow::c_quantity_name[128] = "";
char AdminUi::AddQuantityWindow::c_new_brand_quantity_name[128] = "";
char AdminUi::AddQuantityWindow::c_new_quantity_name[128] = "";

int AdminUi::AddQuantityWindow::create_add_quantity_window(std::string &quantity, double &selling_price, double &buying_price, int &number_of_items, std::string caller)
{

	ImVec2 size = ImVec2(300, 350);
	std::string label = "Add Quantity";
	if (caller.compare("New Brand") == 0)
	{		
		label = "Add New Brand Quantity ";
	}
	else if (caller.compare("New Quantity") == 0)
	{
		size = ImVec2(400, 500);
		label = "Add New Quantity ";
	}


	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);

	ImGui::Begin(label.c_str(), &this->show_window, this->window_flags);

	if (caller.compare("New Quantity") == 0)
	{
		ImGui::SameLine(120.0, 0.0);
		this->mset_products();
		this->create_listbox_filter(this->products, "Products", this->selected_product, '_', ImVec2(210, 150));

		
		ImVec2 combo_size = ImVec2(230, 50);

		ImGui::NewLine();
		
		if (this->selected_product.compare("All Products") != 0)
		{
			this->brands_array = get_product_brands_array(this->products_collection, this->selected_product);

			mset_brands(this->brands_array);

			//free before reallocating
			delete[]  this->p_brands;
			this->p_brands = NULL;

			//we are reallocating to make sure the size of this buffer is enough to hold the new string
			this->p_brands = new char[this->brands.size() + 1];
			memset(this->p_brands, 0, this->brands.size());
			strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
			this->prepare_items_pointer(this->brands, '_', this->p_brands);
		}
		

		create_combo(this->p_brands, "Brand", this->a_current_brand_index, combo_size);
		
	}

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Quantity Name: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);

	if(caller.compare("Brand") == 0)
		ImGui::InputTextWithHint("###", "name", this->c_quantity_name, IM_ARRAYSIZE(this->c_quantity_name));
	else if (caller.compare("New Brand") == 0)
		ImGui::InputTextWithHint("###", "name", this->c_new_brand_quantity_name, IM_ARRAYSIZE(this->c_new_brand_quantity_name));
	else if (caller.compare("New Quantity") == 0)
		ImGui::InputTextWithHint("###", "name", this->c_new_quantity_name, IM_ARRAYSIZE(this->c_new_quantity_name));

	ImGui::NewLine();
	ImGui::Text("Buying Price: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);

	ImGui::PushID("buying price");
	ImGui::InputFloat("##", &this->buying_price, 0);
	ImGui::PopID();

	ImGui::NewLine();
	ImGui::Text("Selling Price: ");

	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);

	ImGui::PushID("selling price");
	ImGui::InputFloat("##", &this->selling_price, 0);
	ImGui::PopID();

	ImGui::NewLine();
	ImGui::Text("Number: ");
	
	ImGui::SameLine(100.0, 0.0);
	ImGui::SetNextItemWidth(150);

	ImGui::InputInt("##", &this->amount, 0);
	
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(90.0, 0.0);

	
	if (ImGui::Button("Finish", ImVec2(120, 0)))
	{
		int current_brand_index = 1;

		if (caller.compare("Brand") == 0)
			this->quantity_name = this->c_quantity_name;
		else if (caller.compare("New Brand") == 0)
			this->quantity_name = this->c_new_brand_quantity_name;
		else if (caller.compare("New Quantity") == 0)
		{
			current_brand_index = a_current_brand_index;
			this->quantity_name = this->c_new_quantity_name;
		}
				
		
		bool found_unwanted_char = check_unwanted_characters(this->quantity_name);

		quantity = this->quantity_name;
		buying_price = this->buying_price;
		selling_price = this->selling_price;
		number_of_items = this->amount;
																																							  
		if (this->quantity_name.compare("") == 0 || this->buying_price <= 0 || this->selling_price <=0 || this->amount < 0 || found_unwanted_char == true || current_brand_index == 0)
		{
			this->input_error = true;
		}
		else
		{
			if (caller.compare("New Quantity") == 0)
			{
				std::vector<std::string> v_quantites = get_brand_quantities_array(this->products_collection, this->selected_product + "_" + this->brands_array[a_current_brand_index - 1]);
				bool status = check_for_duplicate(v_quantites, this->quantity_name);

				if (status == false)
				{
					insert_quantity(this->products_collection, this->stock, this->selected_product, this->brands_array[a_current_brand_index - 1], this->quantity_name, this->selling_price, this->buying_price, this->amount);
					this->success = true;
				}					
				else
					this->input_error = true;
			}
			this->finish = true;
		}

	}


	ImGui::PopStyleColor();
	ImGui::End();
	return 1;
}

int AdminUi::AddQuantityWindow::mset_brands(std::vector<std::string> vbrands)
{
	this->brands = "Select_";

	for (std::string brand: vbrands)
		this->brands += brand + "_";
	return 1;
}

int AdminUi::AddQuantityWindow::mset_products()
{
	this->v_products = get_products_array(this->products_collection);

	this->products = "All Products_";

	for (std::string m_product : v_products)
	{
		this->products += m_product + "_";
	}

	return 1;
}

int AdminUi::AddQuantityWindow::admin_error_message(bool& error_flag)
{
	ImGui::OpenPopup("Error");
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Fill In The Details or,\nThe Item already exists! or, \nRemove Unwanted Character(s)");
		ImGui::Separator();

		ImGui::SameLine(60.0f, 0.0f);
		if (ImGui::Button("OK", ImVec2(90, 0)))
		{
			error_flag = false;
			ImGui::CloseCurrentPopup();

		}

		ImGui::EndPopup();
	}

	return 1;
}


AdminUi::AddNewBrandWindow::AddNewBrandWindow(mongocxx::database db)
{
	this->brand_win.products_collection = db["products"];;
	this->brand_win.stock = db["stock"];

	this->brand_win.mset_products();
}


int AdminUi::AddNewBrandWindow::create_add_new_brand_window()
{
	
	//handle any error
	if (this->brand_win.input_error == true)
		this->brand_win.admin_error_message(this->brand_win.input_error);
	
	
	if (this->brand_win.finish == true && this->brand_win.input_error == false)
	{
		this->brand_win.show_window = false;
		this->brand_win.finish = false;
	}

	this->brand_win.create_add_brand_window(this->selected_product, this->brand_name, this->n_qty_names, this->n_selling_price, this->n_buying_price, this->n_number_of_items, "New");

	
	return 1;
}


bool AdminUi::AddQuantityWindow::check_unwanted_characters(std::string entered_string)
{

	for (char c : entered_string)
	{
		auto iterator = std::find(this->unwanted_chars.begin(), this->unwanted_chars.end(), c);
		
		if (iterator != this->unwanted_chars.end())
			return true;
	}

	return false;
}


AdminUi::AddNewQuantityWindow::AddNewQuantityWindow(mongocxx::database db)
{
	this->quantity_win.products_collection = db["products"];;
	this->quantity_win.stock = db["stock"];

	this->quantity_win.mset_products();
}


int AdminUi::AddNewQuantityWindow::create_add_new_quantity_window()
{

	if (this->quantity_win.finish == true && this->quantity_win.input_error == false)
	{		
		this->quantity_win.show_window = false;
		this->quantity_win.finish = false;
	}
	

	this->quantity_win.create_add_quantity_window(this->quantity_name, this->n_selling_price, this->n_buying_price, this->n_number_of_items, "New Quantity");

	//handle any error
	if (this->quantity_win.input_error == true)
		this->admin_error_message(this->quantity_win.input_error);
	

	return 1;
}

bool AdminUi::AddQuantityWindow::check_for_duplicate(std::vector<std::string> items, std::string item)
{
	std::vector<std::string> items2;
	items2.reserve(items.size());

	std::transform(items.begin(), items.end(), std::back_inserter(items2), 
			[](const std::string& in)
			{
				std::string out;
				out.reserve(in.size());
				std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
				return out;

			});

	std::transform(item.begin(), item.end(), item.begin(), 
			[](unsigned char c)
			{
				return std::tolower(c);
			});

	auto count = std::count(items2.begin(), items2.end(), item);

	if (count > 0)
		return true;

	return false;
}



AdminUi::RemoveItemWindow::RemoveItemWindow(RemoveProductWindow& remove_product, RemoveBrandWindow& remove_brand, RemoveQuantityWindow& remove_quantity)
{
	this->remove_product = &remove_product;
	this->remove_brand = &remove_brand;
	this->remove_quantity = &remove_quantity;
}

int AdminUi::RemoveItemWindow::create_remove_items_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Remove Item", &this->show_window, this->window_flags);

	if (this->remove_product->show_window)
		this->remove_product->create_remove_product_window();

	if (this->remove_product->success == true)
	{
		this->remove_product->notify_sale_status(this->remove_product->success);
		this->remove_product->show_window = false;
	}		


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Product", ImVec2(145.0, 20.0)))
		this->remove_product->show_window = true;


	if (this->remove_brand->show_window)
		this->remove_brand->create_remove_brand_window();

	if(this->remove_brand->success == true)
	{
		this->remove_brand->notify_sale_status(this->remove_brand->success);
		this->remove_brand->show_window = false;
	}
	
	
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Brand", ImVec2(145.0, 20.0)))
		this->remove_brand->show_window = true;

	if (this->remove_quantity->show_window)
		this->remove_quantity->create_remove_quantity_window();

	if (this->remove_quantity->success == true)
		this->remove_quantity->notify_sale_status(this->remove_quantity->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Quantity", ImVec2(145.0, 20.0)))
		this->remove_quantity->show_window = true;
		

	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}


AdminUi::RemoveProductWindow::RemoveProductWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];
	
}

int AdminUi::RemoveProductWindow::create_remove_product_window()
{	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 350), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);

	ImGui::Begin("Remove Product", &this->show_window, this->window_flags);

	ImGui::NewLine();
	ImGui::SameLine(80.0, 0.0);
	this->mset_products();
	this->create_listbox_filter(this->products, "Product", this->selected_product, '_', ImVec2(210, 150));
		
	if (this->confirm == this->ACCEPTED)
	{
		delete_product(this->products_collection, this->stock, this->selected_product);

		this->success = true;

		this->show_confirm_window = false;
		this->confirm = this->NONE;

		this->selected_product = "All Products";
		
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if(this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
			this->admin_error_message(this->input_error);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(100.0, 0.0);

	if (ImGui::Button("Remove", ImVec2(120, 0)))
	{	
		if ((this->selected_product.compare("All Products") == 0))
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

int AdminUi::AddQuantityWindow::admin_confirm_message(int& confirm_flag)
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


 int AdminUi::RemoveBrandWindow::r_brand_current_item = 0;
AdminUi::RemoveBrandWindow::RemoveBrandWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];
	
}

int AdminUi::RemoveBrandWindow::create_remove_brand_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiCond_Once);

	ImGui::Begin("Remove Brand", &this->show_window, this->window_flags);

	
	ImGui::SameLine(80.0, 0.0);
	this->mset_products();

	this->create_listbox_filter(this->products, "Products", this->selected_product, '_', ImVec2(210, 150));


	ImVec2 combo_size = ImVec2(230, 50);
	ImGui::NewLine();

	if (this->selected_product.compare("All Products") != 0)
	{
		this->brands_array = get_product_brands_array(this->products_collection, this->selected_product);

		mset_brands(this->brands_array);
	}
	else
	{
		this->brands = "Select_";
	}

	//free before reallocating
	delete[]  this->p_brands;
	this->p_brands = NULL;

	//we are reallocating to make sure the size of this buffer is enough to hold the new string
	this->p_brands = new char[this->brands.size() + 1];
	memset(this->p_brands, 0, this->brands.size());
	strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
	this->prepare_items_pointer(this->brands, '_', this->p_brands);

	ImGui::SameLine(60.0, 0.0);
	create_combo(this->p_brands, "Brand", this->r_brand_current_item, combo_size);
	
	if (this->confirm == this->ACCEPTED)
	{
		delete_brand(this->products_collection, this->stock, this->selected_product, this->brands_array[r_brand_current_item - 1]);
		
		this->success = true;

		this->show_confirm_window = false;
		this->confirm = this->NONE;

		this->selected_product = "All Products";
		r_brand_current_item = 0;
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
		this->admin_error_message(this->input_error);
		
	
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(70.0, 0.0);
	if (ImGui::Button("Remove", ImVec2(120, 0)))
	{
		if (this->selected_product.compare("All Products") == 0  || r_brand_current_item == 0)
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

int AdminUi::RemoveQuantityWindow::r_quantity_brand_current_item = 0;
int AdminUi::RemoveQuantityWindow::r_quantity_current_item = 0;
AdminUi::RemoveQuantityWindow::RemoveQuantityWindow(mongocxx::database db)
{
	this->products_collection = db["products"];
	this->stock = db["stock"];

}


int AdminUi::RemoveQuantityWindow::create_remove_quantity_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiCond_Once);

	ImGui::Begin("Remove Quantity", &this->show_window, this->window_flags);


	ImGui::SameLine(70.0, 0.0);
	this->mset_products();
	this->create_listbox_filter(this->products, "Products", this->selected_product, '_', ImVec2(210, 150));
		

	if (this->selected_product.compare("All Products") != 0)
	{
		this->brands_array = get_product_brands_array(this->products_collection, this->selected_product);

		mset_brands(this->brands_array);
	}
	else
	{
		this->brands = "Select_";		
	}

	if (r_quantity_brand_current_item != 0)
	{
		this->quantities_array = get_brand_quantities_array(this->products_collection, this->selected_product + "_" + this->brands_array[r_quantity_brand_current_item - 1]);

		mset_quantities(this->quantities_array);
	}
	else
	{
		this->quantities = "Select_";
	}

	//free before reallocating
	delete[]  this->p_brands;
	this->p_brands = NULL;

	//we are reallocating to make sure the size of this buffer is enough to hold the new string
	this->p_brands = new char[this->brands.size() + 1];
	memset(this->p_brands, 0, this->brands.size());
	strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
	this->prepare_items_pointer(this->brands, '_', this->p_brands);

	ImVec2 combo_size = ImVec2(230, 50);
	ImGui::NewLine();
	ImGui::SameLine(60.0, 0.0);
	create_combo(this->p_brands, "Brand", this->r_quantity_brand_current_item, combo_size);
	
	delete[] this->p_quantities;
	this->p_quantities = NULL;

	//we are reallocating to make sure the size of this buffer is enough to hold the current string
	this->p_quantities = new char[this->quantities.size() + 1];
	memset(this->p_quantities, 0, this->quantities.size());
	strncpy_s(this->p_quantities, this->quantities.size() + 1, this->quantities.c_str(), this->quantities.size());
	this->prepare_items_pointer(this->quantities, '_', this->p_quantities);

	ImGui::NewLine();
	ImGui::SameLine(60.0, 0.0);
	create_combo(this->p_quantities, "Quantity", this->r_quantity_current_item, combo_size);

	if (this->confirm == this->ACCEPTED)
	{
		delete_quantity(this->products_collection, this->stock, this->selected_product, this->brands_array[r_quantity_brand_current_item - 1], this->quantities_array[r_quantity_current_item - 1]);

		this->success = true;

		this->show_confirm_window = false;
		this->confirm = this->NONE;

		this->selected_product = "All Products";
		r_quantity_current_item = 0;
		r_quantity_brand_current_item = 0;
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
		this->admin_error_message(this->input_error);


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(100.0, 0.0);
	if (ImGui::Button("Remove", ImVec2(120, 0)))
	{																 //no item selected, current selected item is Select
		if (this->selected_product.compare("All Products") == 0 || r_quantity_brand_current_item == 0 || r_quantity_current_item == 0)
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

int AdminUi::AddQuantityWindow::mset_quantities(std::vector<std::string> vquantities)
{
	this->quantities = "Select_";

	for (std::string quantity : vquantities)
		this->quantities += quantity + "_";
	return 1;

}

AdminUi::AddOthersWindow::AddOthersWindow(mongocxx::database db)
{
	this->products_collection = db["products"];

	//we are reallocating to make sure the size of this buffer is enough to hold the new string
	this->p_others = new char[this->others.size() + 1];
	memset(this->p_others, 0, this->others.size());
	strncpy_s(this->p_others, this->others.size() + 1, this->others.c_str(), this->others.size());
	this->prepare_items_pointer(this->others, '_', this->p_others);
	
}

AdminUi::AddOthersWindow::~AddOthersWindow()
{
	delete[] this->p_others;
	this->p_others = NULL;

}

int AdminUi::AddOthersWindow::others_current_item = 0;
char AdminUi::AddOthersWindow::other_name[128] = "";
int AdminUi::AddOthersWindow::create_add_others_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once);

	ImGui::Begin("Add Others", &this->show_window, this->window_flags);
		
	ImVec2 combo_size = ImVec2(230, 50);
	ImGui::SameLine(40.0, 0.0);
	create_combo(this->p_others, "Others", this->others_current_item, combo_size);
		

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(30.0, 0.0);
	ImGui::Text("Name: ");

	ImGui::SameLine(70.0, 0.0);
	ImGui::SetNextItemWidth(150);
	ImGui::InputTextWithHint("###", "name", this->other_name, IM_ARRAYSIZE(this->other_name));

	if (this->confirm == this->ACCEPTED)
	{
		if (this->v_others[others_current_item - 1].compare("Season") == 0)
		{
			insert_season(this->products_collection, this->s_other_name);
		}
		else if (this->v_others[others_current_item - 1].compare("Age Group") == 0)
		{
			insert_age_group(this->products_collection, this->s_other_name);
		}
		else if (this->v_others[others_current_item - 1].compare("Location") == 0)
		{
			insert_location(this->products_collection, this->s_other_name);
		}
		
		
		this->success = true;

		this->show_confirm_window = false;
		this->confirm = this->NONE;

		this->others_current_item = 0;
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
		this->admin_error_message(this->input_error);


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(70.0, 0.0);
	if (ImGui::Button("Add", ImVec2(120, 0)))
	{
		this->s_other_name = this->other_name;
		
		if (others_current_item == 0 || this->s_other_name.compare("") == 0)
		{
			this->input_error = true;

		}
		else
		{
			std::vector<std::string> current_others;
			bool exists = false;
			if (this->v_others[others_current_item - 1].compare("Season") == 0)
			{
				current_others = get_seasons_array(this->products_collection);
				
				exists = check_for_duplicate(current_others, this->s_other_name);
			}
			else if (this->v_others[others_current_item - 1].compare("Age Group") == 0)
			{
				current_others = get_age_groups_array(this->products_collection);

				exists = check_for_duplicate(current_others, this->s_other_name);
			}			
			else if (this->v_others[others_current_item - 1].compare("Location") == 0)
			{
				current_others = get_locations_array(this->products_collection);

				exists = check_for_duplicate(current_others, this->s_other_name);
			}

			if(exists == false)
				this->show_confirm_window = true;
			else
				this->input_error = true;
		}

	}

	ImGui::PopStyleColor();
	ImGui::End();
	
	return 1;
}

int AdminUi::RemoveOthersWindow::r_others_current_item = 0;
int AdminUi::RemoveOthersWindow::r_other_selected_current_item = 0;
AdminUi::RemoveOthersWindow::RemoveOthersWindow(mongocxx::database db)
{
	this->products_collection = db["products"];

	//we are reallocating to make sure the size of this buffer is enough to hold the new string
	this->p_others = new char[this->others.size() + 1];
	memset(this->p_others, 0, this->others.size());
	strncpy_s(this->p_others, this->others.size() + 1, this->others.c_str(), this->others.size());
	this->prepare_items_pointer(this->others, '_', this->p_others);

	//we are reallocating to make sure the size of this buffer is enough to hold the new string
	this->p_selected_others = new char[this->selected_others.size() + 1];
	memset(this->p_selected_others, 0, this->selected_others.size());
	strncpy_s(this->p_selected_others, this->selected_others.size() + 1, this->selected_others.c_str(), this->selected_others.size());
	this->prepare_items_pointer(this->selected_others, '_', this->p_selected_others);

}

AdminUi::RemoveOthersWindow::~RemoveOthersWindow()
{
	delete[] this->p_others;
	this->p_others = NULL;

	delete[] this->p_selected_others;
	this->p_selected_others = NULL;

}

int AdminUi::RemoveOthersWindow::create_remove_others_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once);

	ImGui::Begin("Remove Others", &this->show_window, this->window_flags);

	ImVec2 combo_size = ImVec2(230, 50);
	ImGui::SameLine(40.0, 0.0);
	create_combo(this->p_others, "Others", this->r_others_current_item, combo_size);


	if (r_others_current_item != 0)
	{

		if (r_others_current_item == 1)
		{
			this->v_selected_others = get_seasons_array(this->products_collection);

			
		}
		else if (r_others_current_item == 2)
		{
			this->v_selected_others = get_age_groups_array(this->products_collection);

			
		}
		else if (r_others_current_item == 3)
		{
			this->v_selected_others = get_locations_array(this->products_collection);
			
		}

		mset_others(this->v_selected_others);

		delete[] this->p_selected_others;
		this->p_selected_others = NULL;

		//we are reallocating to make sure the size of this buffer is enough to hold the new string
		this->p_selected_others = new char[this->selected_others.size() + 1];
		memset(this->p_selected_others, 0, this->selected_others.size());
		strncpy_s(this->p_selected_others, this->selected_others.size() + 1, this->selected_others.c_str(), this->selected_others.size());
		this->prepare_items_pointer(this->selected_others, '_', this->p_selected_others);


	}
	ImGui::NewLine();
	ImGui::SameLine(40.0, 0.0);
	create_combo(this->p_selected_others, "Other", this->r_other_selected_current_item, combo_size);
	
	if (this->confirm == this->ACCEPTED)
	{

		if (r_others_current_item  == 1)
		{
			delete_season(this->products_collection, this->v_selected_others[this->r_other_selected_current_item - 1]);
		}
		else if (r_others_current_item == 2)
		{
			delete_age_group(this->products_collection, this->v_selected_others[this->r_other_selected_current_item - 1]);
		}
		else if (r_others_current_item == 3)
		{
			delete_location(this->products_collection, this->v_selected_others[this->r_other_selected_current_item - 1]);
		}
		

		this->success = true;

		this->show_confirm_window = false;
		this->confirm = this->NONE;

		this->r_others_current_item = 0;
		this->r_other_selected_current_item = 0;
	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
		this->admin_error_message(this->input_error);


	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(70.0, 0.0);
	if (ImGui::Button("Remove", ImVec2(120, 0)))
	{

		if (r_others_current_item == 0 || r_other_selected_current_item == 0)
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

AdminUi::AddorRemoveOthersWindow::AddorRemoveOthersWindow(AddOthersWindow& add_other, RemoveOthersWindow& remove_other)
{
	this->add_other = &add_other;
	this->remove_other = &remove_other;
}

int AdminUi::AddorRemoveOthersWindow::create_add_or_remove_others_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Add Or Remove Other", &this->show_window, this->window_flags);

	if (this->add_other->show_window)
		this->add_other->create_add_others_window();

	if (this->add_other->success == true)
		this->add_other->notify_sale_status(this->add_other->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Add Other", ImVec2(145.0, 20.0)))
		this->add_other->show_window = true;

	if (this->remove_other->show_window)
		this->remove_other->create_remove_others_window();

	if (this->remove_other->success == true)
		this->remove_other->notify_sale_status(this->remove_other->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Remove Other", ImVec2(145.0, 20.0)))
		this->remove_other->show_window = true;


	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}

int AdminUi::RemoveOthersWindow::mset_others(std::vector<std::string> v_s_others)
{
	this->selected_others = "Select_";

	for (std::string selected_other : v_s_others)
		this->selected_others += selected_other + "_";
	return 1;
}


char AdminUi::AddAgentWindow::agent_name[128] = "";
char AdminUi::AddAgentWindow::agent_phone[13] = "";
char AdminUi::AddAgentWindow::agent_location[128] = "";

AdminUi::AddAgentWindow::AddAgentWindow(mongocxx::database db)
{
	this->agents = db["Agents"];
	this->agent_sales = db["AgentSales"];
}

int AdminUi::AddAgentWindow::create_add_agent_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));
	
	ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_Once);

	ImGui::Begin("Agent Details", &this->show_window, this->window_flags);

	
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Agent Name    : ");

	ImGui::SameLine(115.0, 0.0);
	ImGui::SetNextItemWidth(160);
	ImGui::PushID("Agent Name");
	ImGui::InputTextWithHint("###", "name", this->agent_name, IM_ARRAYSIZE(this->agent_name));	
	ImGui::PopID();

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Phone         : ");

	ImGui::SameLine(115.0, 0.0);
	ImGui::SetNextItemWidth(160);
	ImGui::PushID("phone");
	ImGui::InputTextWithHint("###", "phone", this->agent_phone, IM_ARRAYSIZE(this->agent_phone));
	ImGui::PopID();

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::Text("Agent Location: ");

	ImGui::SameLine(115.0, 0.0);
	ImGui::SetNextItemWidth(160);
	ImGui::PushID("Agent Location");
	ImGui::InputTextWithHint("###", "location", this->agent_location, IM_ARRAYSIZE(this->agent_location));
	ImGui::PopID();

	
	if (this->confirm == this->ACCEPTED)
	{
		AGENT agent;
		agent.agent_name = this->s_agent_name;
		agent.agent_location = this->s_agent_location;
		agent.phone = this->s_agent_phone;

		this->s_agent_id = add_new_agent(this->agents, this->agent_sales, agent);

		this->success = true;

		this->show_id = true;
		this->show_confirm_window = false;
		this->confirm = this->NONE;

	}
	else if (this->confirm == this->REJECTED)
	{
		this->show_confirm_window = false;
		this->confirm = this->NONE;
	}

	if (this->show_confirm_window == true)
		this->admin_confirm_message(this->confirm);
	else if (this->input_error)
		this->admin_error_message(this->input_error);

	if(this->show_id)
		ImGui::TextColored(ImVec4(0.543f, 0.931f, 0.886f, 0.784f), std::string("Agent ID: " + this->s_agent_id).c_str());

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(90.0, 0.0);

	if (ImGui::Button("Add", ImVec2(120, 0)))
	{
		this->s_agent_name = this->agent_name;
		this->s_agent_location = this->agent_location;

		this->s_agent_phone = this->agent_phone;
		
		size_t phone_digits = this->s_agent_phone.size();
		bool is_number = std::regex_match(this->s_agent_phone, std::regex("[0-9]+"));			

		if (this->s_agent_name.compare("") == 0 || this->s_agent_location.compare("") == 0 || phone_digits < 10 || phone_digits > 13 || is_number == false)
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

int AdminUi::RemoveAgentWindow::remove_current_filter_type_index = 0;
AdminUi::RemoveAgentWindow::RemoveAgentWindow(mongocxx::database db)
{
	this->agents = db["Agents"];
	this->agent_sales = db["AgentSales"];

	table_flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Reorderable;
	//TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	this->agent_details = get_agent_details(this->agents);

	this->ids = this->agent_details[0];
	this->names = this->agent_details[1];
	this->phones = this->agent_details[2];
	this->locations = this->agent_details[3];

	this->filtered_ids = this->ids;
	this->filtered_names = this->names;
	this->filtered_phones = this->phones;
	this->filtered_locations = this->locations;

	this->p_admin_filter_types = new char[this->admin_filter_types.size() + 1];
	memset(this->p_admin_filter_types, 0, this->admin_filter_types.size());
	strncpy_s(this->p_admin_filter_types, this->admin_filter_types.size() + 1, this->admin_filter_types.c_str(), this->admin_filter_types.size());
	this->prepare_filter_types_pointer(this->admin_filter_types, '_', this->p_admin_filter_types);
}

int AdminUi::RemoveAgentWindow::create_remove_agent_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_Once);

	ImGui::Begin("Remove Agents", &this->show_window, this->window_flags | ImGuiWindowFlags_AlwaysAutoResize);

	this->agent_details = get_agent_details(this->agents);

	this->ids = this->agent_details[0];
	this->names = this->agent_details[1];
	this->phones = this->agent_details[2];
	this->locations = this->agent_details[3];

	if (this->names.size() != 0)
	{

		ImGuiContext& g = *GImGui;
		ImGuiWindow* parent_window = g.CurrentWindow;
		ImRect mrect = parent_window->Rect();

		float new_xcenter = mrect.GetCenter().x;

		// description
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine((new_xcenter - (350 / 2)) - mrect.Min.x, 0.0);

		create_search_filter_combo(this->p_admin_filter_types, "Filter Criteria", this->remove_current_filter_type_index, ImVec2(350, 30));

		//filter the agents after a search
		if (this->remove_current_filter_type_index != 0)
		{
			this->create_admin_agents_filter();
		}
		else
		{
			this->filtered_ids = this->ids;
			this->filtered_names = this->names;
			this->filtered_phones = this->phones;
			this->filtered_locations = this->locations;
		}

		// description
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine(200.0, 0.0);
		ImGui::TextColored(ImVec4(0.543f, 0.931f, 0.886f, 0.784f), "Agents Details");
		/*Table*/


		this->draw_admin_agents_table(this->TEXT_BASE_HEIGHT);


		if (this->confirm == this->ACCEPTED)
		{
			for (int i = 0; i < this->agents_count; i++)
			{
				this->agent_name = this->names[this->checked_agents[i]];
				this->agent_id = this->ids[this->checked_agents[i]];

				remove_agent(this->agents, this->agent_sales, this->agent_name, std::stoi(this->agent_id));
			}
			

			this->filtered_ids = this->ids;
			this->filtered_names = this->names;
			this->filtered_phones = this->phones;
			this->filtered_locations = this->locations;

			this->success = true;

			this->show_confirm_window = false;
			this->confirm = this->NONE;

			this->checked_agents.clear();
			remove_current_filter_type_index = 0;
		}
		else if (this->confirm == this->REJECTED)
		{
			this->show_confirm_window = false;
			this->confirm = this->NONE;
		}

		if (this->show_confirm_window == true)
			this->admin_confirm_message(this->confirm);
		else if (this->input_error)
			this->admin_error_message(this->input_error);


		this->agents_count = (int)this->checked_agents.size();
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::SameLine((new_xcenter - (115 / 2)) - mrect.Min.x, 0.0);	//button to be centered
		if (ImGui::Button("Remove", ImVec2(115.0, 20.0)))
		{
			if (this->agents_count == 0)
			{
				this->input_error = true;
			}
			else
			{
				this->show_confirm_window = true;

			}

		}
	}
	else
	{
		ImGui::Dummy(ImVec2(0.0, 10.0));
		ImGui::NewLine();
		ImGui::Text(std::string("No Agents Found").c_str());
		ImGui::Dummy(ImVec2(0.0, 10.0));
	}
	

	ImGui::PopStyleColor();
	ImGui::End();


	return 1;
}

int AdminUi::RemoveAgentWindow::create_admin_agents_filter()
{
	// description
	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(150.0, 0.0);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
	ImGui::BeginChild("##", ImVec2(200, 35), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	static ImGuiTextFilter filter;
	filter.Draw("Search");

	//Get number of items
	size_t count = this->names.size();

	this->filtered_ids.clear();
	this->filtered_names.clear();
	this->filtered_phones.clear();
	this->filtered_locations.clear();

	for (int item_num = 0; item_num < count; item_num++)
	{
		if (this->remove_current_filter_type_index == 1)
		{
			if (filter.PassFilter(this->ids[item_num].c_str()))
			{
				this->filtered_ids.push_back(this->ids[item_num]);
				this->filtered_names.push_back(this->names[item_num]);
				this->filtered_phones.push_back(this->phones[item_num]);
				this->filtered_locations.push_back(this->locations[item_num]);
			}
		}
		else if (this->remove_current_filter_type_index == 2)
		{
			if (filter.PassFilter(this->names[item_num].c_str()))
			{
				this->filtered_ids.push_back(this->ids[item_num]);
				this->filtered_names.push_back(this->names[item_num]);
				this->filtered_phones.push_back(this->phones[item_num]);
				this->filtered_locations.push_back(this->locations[item_num]);
			}
		}
		else if (this->remove_current_filter_type_index == 3)
		{
			if (filter.PassFilter(this->phones[item_num].c_str()))
			{
				this->filtered_ids.push_back(this->ids[item_num]);
				this->filtered_names.push_back(this->names[item_num]);
				this->filtered_phones.push_back(this->phones[item_num]);
				this->filtered_locations.push_back(this->locations[item_num]);
			}
		}

		else if (this->remove_current_filter_type_index == 4)
		{
			if (filter.PassFilter(this->locations[item_num].c_str()))
			{
				this->filtered_ids.push_back(this->ids[item_num]);
				this->filtered_names.push_back(this->names[item_num]);
				this->filtered_phones.push_back(this->phones[item_num]);
				this->filtered_locations.push_back(this->locations[item_num]);
			}
		}
		

	}

	ImGui::PopStyleColor();
	ImGui::EndChild();

	return 1;
}

int AdminUi::RemoveAgentWindow::draw_admin_agents_table(const float& m_TEXT_BASE_HEIGHT)
{
	bool* checked_statuses = new bool[this->filtered_names.size()];
	memset(checked_statuses, 0, this->filtered_names.size());
	for (int index : this->checked_agents)
		checked_statuses[index] = true;

	ImVec2 outer_size = ImVec2(0.0f, m_TEXT_BASE_HEIGHT * this->filtered_names.size() * 10);
	if (ImGui::BeginTable("table_scrolly", 5, this->table_flags, outer_size))
	{
		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Phone", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Check", ImGuiTableColumnFlags_None);

		ImGui::TableHeadersRow();

		std::vector<int>::iterator position;
		bool clicked;
		bool present;

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin((int)this->filtered_names.size());
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 5; column++)
				{
					ImU32 cell_bg_color = ImGui::GetColorU32(this->colum_colors[column]);
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

					ImGui::TableSetColumnIndex(column);
					if(column == 0)
					{
						ImGui::Text(this->filtered_ids[row].c_str());
					}
					else if (column == 1)
					{
						ImGui::Text(this->filtered_names[row].c_str());
					}
					else if (column == 2)
					{
						ImGui::Text(this->filtered_phones[row].c_str());
					}
					else if(column == 3)
					{
						ImGui::Text(this->filtered_locations[row].c_str());
					}					
					else
					{
						position = std::find(this->checked_agents.begin(), this->checked_agents.end(), row);
						present = (position != this->checked_agents.end()) ? true : false;

						ImGui::PushID(row);
						clicked = ImGui::Checkbox("##", &checked_statuses[row]);
						ImGui::PopID();

						//we update the data when the checkbox is clicked and it is currently not active to avoid adding and 
						//erasing at the same instance and vice versa
						if (clicked && present == true && !ImGui::IsItemActive())
						{
							this->checked_agents.erase(position);
						}
						else if (clicked && present == false && !ImGui::IsItemActive())
						{
							this->checked_agents.push_back(row);
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

AdminUi::AddorRemoveAgentsWindow::AddorRemoveAgentsWindow(AddAgentWindow& add_agent, RemoveAgentWindow& remove_agent)
{
	this->add_agent = &add_agent;
	this->remove_agent = &remove_agent;
}


int AdminUi::AddorRemoveAgentsWindow::create_add_or_remove_agents_window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

	ImGui::SetNextWindowPos(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 250), ImGuiCond_Once);

	ImGui::Begin("Add Or Remove Agent", &this->show_window, this->window_flags);

	if (this->add_agent->show_window)
		this->add_agent->create_add_agent_window();

	if (this->add_agent->success == true)
		this->add_agent->notify_sale_status(this->add_agent->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Add Agent", ImVec2(145.0, 20.0)))
		this->add_agent->show_window = true;

	if (this->remove_agent->show_window)
		this->remove_agent->create_remove_agent_window();

	if (this->remove_agent->success == true)
		this->remove_agent->notify_sale_status(this->remove_agent->success);

	ImGui::Dummy(ImVec2(0.0, 10.0));
	ImGui::NewLine();
	ImGui::SameLine(50.0, 0.0);
	if (ImGui::Button("Remove Agent", ImVec2(145.0, 20.0)))
		this->remove_agent->show_window = true;


	ImGui::PopStyleColor();
	ImGui::End();

	return 1;
}


