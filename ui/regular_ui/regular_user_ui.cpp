
#include "admin_ui/admin_ui.h"
#include "regular_ui/regular_user_ui.h"
#include "retrieve_data.h"
#include "agents.h"
#include "change_values.h"
#include "update_stock.h"
#include "login_ui/login_ui.h"
#include "marketer_ui/marketer_ui.h"

#include "imgui_internal.h"


bool regular_user_window = false;


RUserUi::ShowRegularUserWindow::ShowRegularUserWindow(StockWindow& stock_win, SellWindow& sell_win, ReturnSaleWindow& return_sale_win, AgentsWindow& agents_win, PassWord& change_password)
{
    this->stock_win = &stock_win;
    this->sell_win = &sell_win;
    this->return_sale_win = &return_sale_win;
    this->agents_win = &agents_win;
    this->change_password = &change_password;
}

int RUserUi::ShowRegularUserWindow::create_show_regular_user_window()
{
    /*------------------------------------------View Stock---------------------------------------------------*/

      ImGui::Dummy(ImVec2(0.0, 60.0));
      ImGui::NewLine();
      ImGui::SameLine(50.0, 0.0);
      if (ImGui::Button("View Stock", ImVec2(115.0, 20.0)))
          this->stock_win->show_window = true;

      if(this->stock_win->show_window)
          this->stock_win->create_stock_window();
      /*--------------------------------------------------------------------------------------------------------*/

      /*------------------------------------------Sell---------------------------------------------------*/

      ImGui::Dummy(ImVec2(0.0, 10.0));
      ImGui::NewLine();
      ImGui::SameLine(50.0, 0.0);
      if (ImGui::Button("Sell Product", ImVec2(115.0, 20.0)))
          this->sell_win->show_window = true;

      if (this->sell_win->show_window)
          this->sell_win->create_sell_window();
      /*--------------------------------------------------------------------------------------------------*/

       /*------------------------------------------Return---------------------------------------------------*/

      ImGui::Dummy(ImVec2(0.0, 10.0));
      ImGui::NewLine();
      ImGui::SameLine(50.0, 0.0);
      if (ImGui::Button("Return Product", ImVec2(115.0, 20.0)))
          this->return_sale_win->show_window = true;

      if (this->return_sale_win->show_window)
          this->return_sale_win->create_return_sale_window();
      /*--------------------------------------------------------------------------------------------------*/

      /*------------------------------------------Agents---------------------------------------------------*/

      ImGui::Dummy(ImVec2(0.0, 10.0));
      ImGui::NewLine();
      ImGui::SameLine(50.0, 0.0);
      if (ImGui::Button("View Agents", ImVec2(115.0, 20.0)))
          this->agents_win->show_window = true;

      if (this->agents_win->show_window)
          this->agents_win->create_agents_window();

      /*-------------------------------------------------------------------------------------------------*/
      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));

      ImGui::Dummy(ImVec2(0.0, 10.0));
      ImGui::NewLine();
      ImGui::SameLine(50.0, 0.0);
      if (ImGui::Button("Log Out", ImVec2(115.0, 20.0)))
      {
          login_window = true;
          regular_user_window = false;
          marketer_window = false;
          admin_window = false;
          finance_window = false;
          showing_admin_extensions = false;
      }
      ImGui::PopStyleColor(3);


      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));

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

RUserUi::Window::Window()
{
    this->selected_product = "All Products";
    this->selected_brand = "Select";
    this->selected_quantity = "Select";

    this->set_brands("Select_");
    this->set_quantities("Select_");


    this->p_brands = new char[this->brands.size() + 1];
    memset(this->p_brands, 0, this->brands.size());
    strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
    this->prepare_items_pointer(this->brands, '_', this->p_brands);

    this->p_quantities = new char[this->quantities.size() + 1];
    memset(this->p_quantities, 0, this->quantities.size());
    strncpy_s(this->p_quantities, this->quantities.size() + 1, this->quantities.c_str(), this->quantities.size());
    this->prepare_items_pointer(this->quantities, '_', this->p_quantities);

}

RUserUi::Window::~Window()
{
    delete[]  this->p_brands;
    this->p_brands = NULL;

    delete[] this->p_quantities;
    this->p_quantities = NULL;
}

void RUserUi::Window::prepare_items_pointer(const std::string& items, char items_separator, char*& items_pointer)
{
    size_t count = std::count(items.begin(), items.end(), items_separator);

    size_t offset = 0;
    for (int index = 0; index < count; index++)
    {
        offset = items.find_first_of(items_separator, (offset + 1));
        items_pointer[offset] = '\0';
    }

}

int RUserUi::Window::create_listbox_filter(std::string items, std::string items_description, std::string& selected_item, char item_separator, ImVec2 size, std::string caller)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
    ImGui::BeginChild(items_description.c_str(), size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    //selected item
    items_description += ":" + selected_item;
    ImGui::Text(items_description.c_str());

    static ImGuiTextFilter filter;
    filter.Draw("Search");

    //Get number of items
    size_t count = std::count(items.begin(), items.end(), item_separator);

    //store the items in a vector
    std::vector<std::string> items_array;
    std::string item = "";
    size_t end_of_item_name = 0;
    for (int item_num = 0; item_num < count; item_num++)
    {
        end_of_item_name = items.find_first_of(item_separator, (size_t)0);
        item = items.substr(0, end_of_item_name);

        if (filter.PassFilter(item.c_str()))
            items_array.push_back(item);

        items = items.substr(end_of_item_name + 1, items.size() - end_of_item_name);
    }

    items_array.push_back(" ");
    items_array.push_back(" ");

    int number_of_items = (int)count + 2;

    if (ImGui::ListBoxHeader("##", number_of_items))
    {
        for (std::string item : items_array)
        {
            if (ImGui::Selectable(item.c_str()))
            {
                if (item.compare(" ") != 0)
                {
                    selected_item = item;
                    if(caller.compare("Regular User") == 0)
                        this->product_changed = true;
                }

            }

        }

        ImGui::ListBoxFooter();
    }

    ImGui::PopStyleColor();
    ImGui::EndChild();


    return 1;
}

int RUserUi::Window::create_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size)
{
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
    ImGui::BeginChild(description.c_str(), size, true);

    ImGui::Combo(description.c_str(), &current_item, (const char*)my_items_names);

    ImGui::PopStyleColor();
    ImGui::EndChild();

    return 1;
}


int RUserUi::Window::display_all_products()
{
    int quantity_index = 0;

    this->products_array = get_products_array(this->products_collection);

    for (std::string product : this->products_array)
    {
        ImGui::Text(product.c_str());

        ImGui::Indent();

        this->brands_array = get_product_brands_array(this->products_collection, product);

        for (std::string brand : this->brands_array)
        {
            ImGui::BulletText(brand.c_str());

            ImGui::Indent();

            this->quantities_array = get_brand_quantities_array(this->products_collection, product + "_" + brand);
            this->amounts_array = get_brand_amounts_array(this->products_collection, this->stock, product, brand);

            quantity_index = 0;

            for (std::string quantity : this->quantities_array)
            {
                ImGui::BulletText(quantity.c_str());

                ImGui::Indent();
                    std::string number = "Remaining: " + std::to_string(this->amounts_array[quantity_index]);
                    ImGui::Text(number.c_str());
                ImGui::Unindent();

                quantity_index++;
            }

            ImGui::Unindent();
          
        }
        ImGui::Unindent();
       
    }

    return 1;
}

int RUserUi::Window::display_all_brands(std::string product)
{    
    int quantity_index = 0;

    ImGui::Text(product.c_str());

    this->brands = "Select_";
    ImGui::Indent();
    
    this->brands_array = get_product_brands_array(this->products_collection, product);

    for (std::string brand : this->brands_array)
    {
        this->brands.append(brand + "_");

        ImGui::BulletText(brand.c_str());

        ImGui::Indent();

        this->quantities_array = get_brand_quantities_array(this->products_collection, product + "_" + brand);
        this->amounts_array = get_brand_amounts_array(this->products_collection, this->stock, product, brand);

        quantity_index = 0;

        for (std::string quantity : this->quantities_array)
        {
            ImGui::BulletText(quantity.c_str());

            ImGui::Indent();
                std::string number = "Remaining: " + std::to_string(this->amounts_array[quantity_index]);
                ImGui::Text(number.c_str());
            ImGui::Unindent();

            quantity_index++;
        }

        ImGui::Unindent();
       
    }
    ImGui::Unindent();

    //free before reallocating
    delete[]  this->p_brands;
    this->p_brands = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the new string
    this->p_brands = new char[this->brands.size() + 1];
    memset(this->p_brands, 0, this->brands.size());
    strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
    this->prepare_items_pointer(this->brands, '_', this->p_brands);

    return 1;
}

int RUserUi::Window::display_brand_quantities(std::string product, std::string brand)
{
   
    this->quantities = "Select_";
    
    ImGui::Text(product.c_str());

    ImGui::Indent();

    ImGui::BulletText(brand.c_str());

    ImGui::Indent();

    int quantity_index = 0;

    this->quantities_array = get_brand_quantities_array(this->products_collection, product + "_" + brand);
    this->amounts_array = get_brand_amounts_array(this->products_collection, this->stock, product, brand);

    for (std::string quantity : this->quantities_array)
    {
        this->quantities.append(quantity + "_");
        ImGui::BulletText(quantity.c_str());

        ImGui::Indent();
        
        std::string number = "Remaining: " + std::to_string(this->amounts_array[quantity_index]);
        ImGui::Text(number.c_str());
       
        ImGui::Unindent();

        quantity_index++;
    }

    ImGui::Unindent();

    ImGui::Unindent();

    //free the memory before reallocating
    delete[] this->p_quantities;
    this->p_quantities = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the new string
    this->p_quantities = new char[this->quantities.size() + 1];
    memset(this->p_quantities, 0, this->quantities.size());
    strncpy_s(this->p_quantities, this->quantities.size() + 1, this->quantities.c_str(), this->quantities.size());
    this->prepare_items_pointer(this->quantities, '_', this->p_quantities);
    
    return 1;
}

int RUserUi::Window::display_quantity(std::string product, std::string brand, std::string quantity)
{
    ImGui::Text(product.c_str());

    ImGui::Indent();

    ImGui::BulletText(brand.c_str());

    ImGui::Indent();

    SALE sale;
    sale.product_name = product;
    sale.brand = sale.product_name + "_" + brand;
    sale.quantity = quantity;

    int amount = get_quantity_total(this->stock, sale);
    
    ImGui::BulletText(quantity.c_str());
    
    ImGui::Indent();
    
    std::string number = "Remaining: " + std::to_string(amount);
    ImGui::Text(number.c_str());
    
    ImGui::Unindent();    
   

    ImGui::Unindent();

    ImGui::Unindent();

    return 1;
}


int RUserUi::Window::set_products()
{
    std::vector<std::string> v_products = get_products_array(this->products_collection);

    this->products = "All Products_";

    for (std::string m_product : v_products)
    {
        this->products += m_product + "_";
    }
    
    return 1;
}

int RUserUi::Window::set_brands(const std::string& m_brands)
{
    this->brands = m_brands;

    return 1;
}


int RUserUi::Window::set_quantities(const std::string& m_quantities)
{
    this->quantities = m_quantities;

    return 1;
}

int RUserUi::Window::set_products_array(const std::vector<std::string>& m_products)
{
    this->products_array = m_products;

    return 1;
}

int RUserUi::Window::set_brands_array(const std::vector<std::string>& m_brands)
{
    this->brands_array = m_brands;

    return 1;
}

int RUserUi::Window::set_quantities_array(const std::vector<std::string>& m_quantities)
{
    this->quantities_array = m_quantities;

    return 1;
}


std::string RUserUi::Window::get_selected_product()
{
    return selected_product;
}

std::string RUserUi::Window::get_selected_brand(int selected_index)
{
    if (selected_index != 0)
        selected_brand = this->brands_array[selected_index - 1];
    else
        selected_brand = "";

    return selected_brand;
}


std::string RUserUi::Window::get_selected_quantity(int selected_index)
{
    if (selected_index != 0)
        selected_quantity = this->quantities_array[selected_index - 1];
    else
        selected_quantity = "";

    return selected_quantity;
}


int RUserUi::Window::reset_combo_items()
{
    this->brands = "Select_";
   
    delete[]  this->p_brands;
    this->p_brands = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the current string
    this->p_brands = new char[this->brands.size() + 1];
    memset(this->p_brands, 0, this->brands.size());
    strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
    this->prepare_items_pointer(this->brands, '_', this->p_brands);


    this->quantities = "Select_";

    delete[] this->p_quantities;
    this->p_quantities = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the current string
    this->p_quantities = new char[this->quantities.size() + 1];
    memset(this->p_quantities, 0, this->quantities.size());
    strncpy_s(this->p_quantities, this->quantities.size() + 1, this->quantities.c_str(), this->quantities.size());
    this->prepare_items_pointer(this->quantities, '_', this->p_quantities);

    return 1;
}

/*------------------------------Stock window----------------------------------*/

//static members
int RUserUi::StockWindow::stock_brand_current_item = 0;
int RUserUi::StockWindow::stock_quantity_current_item = 0;

RUserUi::StockWindow::StockWindow(mongocxx::database db)
{
    this->products_collection = db["products"];
    this->stock = db["stock"];
    this->agents = db["Agents"];

    this->set_products();

}

int RUserUi::StockWindow::create_stock_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(50, 60), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_Once);

    ImGui::Begin("Stock", &this->show_window, this->window_flags);

    ImGui::SameLine(170.0, 0.0);
    /*-----------------------------Product------------------------------------*/

    create_listbox_filter(this->products, "Products", this->selected_product, '_', ImVec2(210, 150));

    /*------------------------------------------------------------------------------*/

    ImVec2 combo_size = ImVec2(230, 50);

    ImGui::NewLine();
    ImGui::SameLine(160.0, 0.0);
    /*-----------------------------------Brand-------------------------------------*/
   
    create_combo(this->p_brands, "Brand", this->stock_brand_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/


    ImGui::NewLine();
    ImGui::SameLine(160.0, 0.0);
    /*---------------------------------------Quantity-------------------------------*/
    
    create_combo(this->p_quantities, "Quantity", this->stock_quantity_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::Separator();

        
    if (this->selected_product.compare("All Products") == 0)
    {       
        display_all_products();
        this->reset_combo_items();
        this->stock_brand_current_item = 0;
        this->stock_quantity_current_item = 0;
        
    }        
    else if (this->stock_brand_current_item == 0 && (product_changed == false))
    {
        display_all_brands(this->selected_product);
    }
    else if (product_changed == true)
    {
        display_all_brands(this->selected_product);
        this->stock_brand_current_item = 0;
        this->stock_quantity_current_item = 0;
        product_changed = false;
    }
    else if (this->stock_brand_current_item != 0 && this->stock_quantity_current_item == 0  && (product_changed == false))
    {
        this->get_selected_brand(this->stock_brand_current_item);
        display_brand_quantities(this->selected_product, this->selected_brand);
    }
    else if (this->stock_quantity_current_item != 0 && (product_changed == false))
    {
        this->get_selected_quantity(this->stock_quantity_current_item);
        display_quantity(this->selected_product, this->selected_brand, this->selected_quantity);
    }

    ImGui::Separator();

    ImGui::PopStyleColor();
    ImGui::End();

    return 1;
}


/*-------------------------------Sell window---------------------------------------*/

//initialize static members
int RUserUi::SellWindow::sell_brand_current_item = 0;
int RUserUi::SellWindow::sell_quantity_current_item = 0;
int RUserUi::SellWindow::sell_season_current_item = 0;
int RUserUi::SellWindow::sell_age_group_current_item = 0;
int RUserUi::SellWindow::sell_gender_current_item = 0;
int RUserUi::SellWindow::sell_location_current_item = 0;

bool RUserUi::SellWindow::input_error = false;
bool RUserUi::SellWindow::success = false;

int RUserUi::SellWindow::number_of_items = 0;

RUserUi::SellWindow::SellWindow(mongocxx::database db)
{
    this->products_collection = db["products"];
    this->stock = db["stock"];
    this->agents = db["Agents"];

    this->set_products();

    this->set_seasons();

    this->p_seasons = new char[this->seasons.size() + 1];
    memset(this->p_seasons, 0, this->seasons.size());
    strncpy_s(this->p_seasons, this->seasons.size() + 1, this->seasons.c_str(), this->seasons.size());
    this->prepare_items_pointer(this->seasons, '_', this->p_seasons);

    this->set_age_groups();

    this->p_age_groups = new char[this->age_groups.size() + 1];
    memset(this->p_age_groups, 0, this->age_groups.size());
    strncpy_s(this->p_age_groups, this->age_groups.size() + 1, this->age_groups.c_str(), this->age_groups.size());
    this->prepare_items_pointer(this->age_groups, '_', this->p_age_groups);

    this->set_genders();

    this->p_genders = new char[this->genders.size() + 1];
    memset(this->p_genders, 0, this->genders.size());
    strncpy_s(this->p_genders, this->genders.size() + 1, this->genders.c_str(), this->genders.size());
    this->prepare_items_pointer(this->genders, '_', this->p_genders);

    this->set_locations();

    this->p_locations = new char[this->locations.size() + 1];
    memset(this->p_locations, 0, this->locations.size());
    strncpy_s(this->p_locations, this->locations.size() + 1, this->locations.c_str(), this->locations.size());
    this->prepare_items_pointer(this->locations, '_', this->p_locations);
}

RUserUi::SellWindow::~SellWindow()
{
    delete [] this->p_seasons;
    this->p_seasons = NULL;

    delete[] this->p_age_groups;
    this->p_age_groups = NULL;

    delete[] this->p_genders;
    this->p_genders = NULL;

    delete[] this->p_locations;
    this->p_locations = NULL;
}

int RUserUi::SellWindow::set_seasons()
{
    this->seasons = "Select_";

    this->seasons_array = get_seasons_array(this->products_collection);

    for (std::string season : this->seasons_array)
        this->seasons += season + "_";

    return 1;
}

int RUserUi::SellWindow::set_age_groups()
{
    this->age_groups = "Select_";

    this->age_groups_array = get_age_groups_array(this->products_collection);

    for (std::string age_group : this->age_groups_array)
        this->age_groups += age_group + "_";

    return 1;
}


int RUserUi::SellWindow::set_genders()
{
    this->genders = "Select_";

    this->genders_array = get_genders_array(this->products_collection);

    for (std::string gender : this->genders_array)
        this->genders += gender + "_";

    return 1;
}

int RUserUi::SellWindow::set_locations()
{
    this->locations = "Select_";

    this->locations_array = get_locations_array(this->products_collection);

    for (std::string location : this->locations_array)
        this->locations += location + "_";

    return 1;
}


std::string RUserUi::SellWindow::get_selected_season()
{
    if (this->sell_season_current_item != 0)
        selected_season = this->seasons_array[this->sell_season_current_item - 1];
    else
        selected_season = "";

    return selected_season;
}

std::string RUserUi::SellWindow::get_selected_age_group()
{
    if (this->sell_age_group_current_item != 0)
        selected_age_group = this->age_groups_array[this->sell_age_group_current_item - 1];
    else
        selected_age_group = "";

    return selected_age_group;
}


std::string RUserUi::SellWindow::get_selected_gender()
{
    if (this->sell_gender_current_item != 0)
        selected_gender = this->genders_array[this->sell_gender_current_item - 1];
    else
        selected_gender = "";

    return selected_gender;
}

std::string RUserUi::SellWindow::get_selected_location()
{
    if (this->sell_location_current_item != 0)
        selected_location = this->locations_array[this->sell_location_current_item - 1];
    else
        selected_location = "";

    return selected_location;
}

int RUserUi::SellWindow::get_number_of_items()
{
    return this->number_of_items;
}

int RUserUi::SellWindow::populate_brands(std::string product)
{
    this->brands_array = get_product_brands_array(this->products_collection, product);

    this->brands = "Select_";

    for (std::string my_brand : this->brands_array)
    {
        this->brands += my_brand + "_";
    }

    delete[]  this->p_brands;
    this->p_brands = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the current string
    this->p_brands = new char[this->brands.size() + 1];
    memset(this->p_brands, 0, this->brands.size());
    strncpy_s(this->p_brands, this->brands.size() + 1, this->brands.c_str(), this->brands.size());
    this->prepare_items_pointer(this->brands, '_', this->p_brands);

    return 1;
}
int RUserUi::SellWindow::populate_quantities(std::string brand)
{
    this->quantities_array = get_brand_quantities_array(this->products_collection, brand);

    this->quantities = "Select_";

    for (std::string my_quantity : this->quantities_array)
    {
        this->quantities += my_quantity + "_";
    }

    delete[] this->p_quantities;
    this->p_quantities = NULL;

    //we are reallocating to make sure the size of this buffer is enough to hold the current string
    this->p_quantities = new char[this->quantities.size() + 1];
    memset(this->p_quantities, 0, this->quantities.size());
    strncpy_s(this->p_quantities, this->quantities.size() + 1, this->quantities.c_str(), this->quantities.size());
    this->prepare_items_pointer(this->quantities, '_', this->p_quantities);
    return 1;
}

int RUserUi::SellWindow::create_sell_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 120), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 530), ImGuiCond_Once);

    ImGui::Begin("Sell", &this->show_window, this->window_flags);

    ImGui::SameLine(170.0, 0.0);
    /*-----------------------------Product-----------------------------------------*/

    create_listbox_filter(this->products, "Product", this->selected_product, '_', ImVec2(210, 150));

    /*------------------------------------------------------------------------------*/

    ImVec2 combo_size = ImVec2(230, 50);

    ImGui::NewLine();
    /*-----------------------------------Brand--------------------------------------*/
    if (this->selected_product.compare("All Products") != 0)
        this->populate_brands(this->selected_product);
    else
    {
        this->reset_combo_items();
        this->sell_brand_current_item = 0;
        this->sell_quantity_current_item = 0;
    }
        
    create_combo(this->p_brands, "Brand", this->sell_brand_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::SameLine(250.0, 0.0);
    /*---------------------------------------Quantity--------------------------------*/
    if (this->sell_brand_current_item != 0)
    {
        this->get_selected_brand(this->sell_brand_current_item);
        this->populate_quantities(this->selected_product + "_" + this->selected_brand);
    }        
    
    create_combo(this->p_quantities, "Quantity", this->sell_quantity_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/


    ImGui::NewLine();
    /*---------------------------------------Season-------------------------------*/
    
    create_combo(this->p_seasons, "Season", this->sell_season_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::SameLine(250.0, 0.0);
    /*---------------------------------------Age Group-------------------------------*/
    
    create_combo(this->p_age_groups, "Age Group", this->sell_age_group_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::NewLine();
    /*---------------------------------------Gender-------------------------------*/
    
    create_combo(this->p_genders, "Gender", this->sell_gender_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::SameLine(250.0, 0.0);
    /*---------------------------------------Location-------------------------------*/
    
    create_combo(this->p_locations, "Location", this->sell_location_current_item, combo_size);

    /*-------------------------------------------------------------------------------*/

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(150.0, 0.0);
    ImGui::SetNextItemWidth(100);

    
    ImGui::InputInt("Number of Items", &this->number_of_items, 0);

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(200.0, 0.0);

    //handle any error
    if (this->input_error == true)
        this->handle_error(this->input_error);
    else if(this->success == true)
        this->notify_sale_status();
    
    if (ImGui::Button("Sell", ImVec2(100.0, 20.0)))
    {
        SALE sale;
        sale.product_name = this->get_selected_product();
        sale.brand = sale.product_name + "_" + this->get_selected_brand(this->sell_brand_current_item);
        sale.quantity = this->get_selected_quantity(this->sell_quantity_current_item);
        sale.season = this->get_selected_season();
        sale.age_group = this->get_selected_age_group();
        sale.gender = this->get_selected_gender();
        sale.location = this->get_selected_location();
        sale.item_count = this->get_number_of_items();

        int available_items = 0;
        if(sale.quantity.compare("") != 0)
           available_items = get_quantity_total(this->stock, sale);

        if ((sale.product_name.compare("") == 0) || (sale.brand.compare("") == 0) || (sale.quantity.compare("") == 0) || (sale.season.compare("") == 0)
            || (sale.age_group.compare("") == 0) || (sale.gender.compare("") == 0) || (sale.location.compare("") == 0) || (sale.item_count == 0) || available_items == 0 || sale.item_count > available_items)
        {
            this->input_error = true;
        }
        else
        {
            change_amountOf_item(this->products_collection, sale, UPDATE);
            update_stock_amountOf_item(this->stock, sale, UPDATE);

            this->success = true;
        }
            
    }
   

    ImGui::PopStyleColor();
    ImGui::End();

    return 1;
}


int RUserUi::SellWindow::handle_error(bool & err_flag)
{
    ImGui::OpenPopup("Error");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Please fill in all the details\nMake Sure the Product is in stock\nCheck if the Product was sold before return");
        ImGui::Separator();

        ImGui::SameLine(90.0f, 0.0f);
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            err_flag = false;
            ImGui::CloseCurrentPopup();

        }

        ImGui::EndPopup();
    }

    return 1;
}

int RUserUi::SellWindow::notify_sale_status()
{
    ImGui::OpenPopup("Success");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Success", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Operation successful");
        ImGui::Separator();

        ImGui::SameLine(20.0f, 0.0f);
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            this->success = false;
            ImGui::CloseCurrentPopup();

        }

        ImGui::EndPopup();
    }

    return 1;
}


int RUserUi::ReturnSaleWindow::return_current_filter_type_index = 0;
RUserUi::ReturnSaleWindow::ReturnSaleWindow(mongocxx::database db)
{
    this->products_collection = db["products"];
    this->stock = db["stock"];
   
    table_flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Reorderable;
    //TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    this->p_sales_filter_types = new char[this->sales_filter_types.size() + 1];
    memset(this->p_sales_filter_types, 0, this->sales_filter_types.size());
    strncpy_s(this->p_sales_filter_types, this->sales_filter_types.size() + 1, this->sales_filter_types.c_str(), this->sales_filter_types.size());
    this->prepare_filter_types_pointer(this->sales_filter_types, '_', this->p_sales_filter_types);
}

int RUserUi::ReturnSaleWindow::create_return_sale_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_Once);

    ImGui::Begin("Return Sale", &this->show_window, this->window_flags | ImGuiWindowFlags_AlwaysAutoResize);

    this->sales_data = get_sales_data(this->products_collection);

    this->sales_product_names = this->sales_data[0];
    this->sales_brands = this->sales_data[1];
    this->sales_quantities = this->sales_data[2];
    this->sales_seasons = this->sales_data[3];
    this->sales_age_groups = this->sales_data[4];
    this->sales_genders = this->sales_data[5];
    this->sales_locations = this->sales_data[6];
    this->sales_item_counts = this->sales_data[7];
    this->sales_indices = this->sales_data[8];
    this->sales_dates_and_times = this->sales_data[9];
    

    if (this->sales_product_names.size() != 0)
    {

        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect mrect = parent_window->Rect();

        float new_xcenter = mrect.GetCenter().x;

        // description
        ImGui::Dummy(ImVec2(0.0, 10.0));
        ImGui::NewLine();
        ImGui::SameLine((new_xcenter - (350 / 2)) - mrect.Min.x, 0.0);

        this->create_search_filter_combo(this->p_sales_filter_types, "Filter Criteria", this->return_current_filter_type_index, ImVec2(350, 30));

        //filter the agents after a search
        if (this->return_current_filter_type_index != 0)
        {
            this->create_sales_data_filter();
        }
        else
        {
            this->filtered_sales_product_names = this->sales_product_names;
            this->filtered_sales_brands = this->sales_brands;
            this->filtered_sales_quantities = this->sales_quantities;
            this->filtered_sales_seasons = this->sales_seasons;
            this->filtered_sales_age_groups = this->sales_age_groups;
            this->filtered_sales_genders = this->sales_genders;
            this->filtered_sales_locations = this->sales_locations;
            this->filtered_sales_item_counts = this->sales_item_counts;
            this->filtered_sales_dates_and_times = this->sales_dates_and_times;
        }

        // description
        ImGui::Dummy(ImVec2(0.0, 10.0));
        ImGui::NewLine();
        ImGui::SameLine(200.0, 0.0);
        ImGui::TextColored(ImVec4(0.543f, 0.931f, 0.886f, 0.784f), "Sales Data");
        /*Table*/


        this->draw_sales_data_table(this->TEXT_BASE_HEIGHT);


        if (this->confirm == this->ACCEPTED)
        {
            for (size_t i = 0; i < this->sales_count; i++)
            {
                
                this->sale.product_name = this->sales_product_names[this->checked_sales[i]];
                this->sale.brand = this->sale.product_name + "_" + this->sales_brands[this->checked_sales[i]];
                this->sale.quantity = this->sales_quantities[this->checked_sales[i]];
                this->sale.season = this->sales_seasons[this->checked_sales[i]];
                this->sale.age_group = this->sales_age_groups[this->checked_sales[i]];
                this->sale.gender = this->sales_genders[this->checked_sales[i]];
                this->sale.location = this->sales_locations[this->checked_sales[i]];
                this->sale.item_count   = std::stoi(this->sales_item_counts[this->checked_sales[i]]) * -1;


                change_amountOf_item(this->products_collection, sale, UPDATE);
                update_stock_amountOf_item(this->stock, sale, UPDATE);

                long m_index = std::stol(this->sales_indices[this->checked_sales[i]]);
                delete_sale_data(this->products_collection, size_t(m_index));//update the sales data
            }


            this->filtered_sales_product_names = this->sales_product_names;
            this->filtered_sales_brands = this->sales_brands;
            this->filtered_sales_quantities = this->sales_quantities;
            this->filtered_sales_seasons = this->sales_seasons;
            this->filtered_sales_age_groups = this->sales_age_groups;
            this->filtered_sales_genders = this->sales_genders;
            this->filtered_sales_locations = this->sales_locations;
            this->filtered_sales_item_counts = this->sales_item_counts;
            this->filtered_sales_dates_and_times = this->sales_dates_and_times;

            this->success = true;

            this->show_confirm_window = false;
            this->confirm = this->NONE;

            this->checked_sales.clear();
            return_current_filter_type_index = 0;
        }
        else if (this->confirm == this->REJECTED)
        {
            this->show_confirm_window = false;
            this->confirm = this->NONE;
        }

        if (this->show_confirm_window == true)
            this->regular_user_confirm_message(this->confirm);
        else if (this->input_error)
            this->handle_error(this->input_error);


        this->sales_count = this->checked_sales.size();
        ImGui::Dummy(ImVec2(0.0, 10.0));
        ImGui::NewLine();
        ImGui::SameLine((new_xcenter - (115 / 2)) - mrect.Min.x, 0.0);	//button to be centered
        if (ImGui::Button("Remove", ImVec2(115.0, 20.0)))
        {
            if(sales_count == 0)
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
        ImGui::Text(std::string("No Sales Found").c_str());
        ImGui::Dummy(ImVec2(0.0, 10.0));
    }


    ImGui::PopStyleColor();
    ImGui::End();


    return 1;
}



int RUserUi::ReturnSaleWindow::create_sales_data_filter()
{
    // description
    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(300.0, 0.0);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
    ImGui::BeginChild("##", ImVec2(200, 35), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    static ImGuiTextFilter filter;
    filter.Draw("Search");

    //Get number of items
    size_t count = this->sales_product_names.size();

    
    this->filtered_sales_product_names.clear();
    this->filtered_sales_brands.clear();
    this->filtered_sales_quantities.clear();
    this->filtered_sales_seasons.clear();
    this->filtered_sales_age_groups.clear();
    this->filtered_sales_genders.clear();
    this->filtered_sales_locations.clear();
    this->filtered_sales_item_counts.clear();
    this->filtered_sales_dates_and_times.clear();

    for (size_t item_num = 0; item_num < count; item_num++)
    {
        if (this->return_current_filter_type_index == 1)
        {
            if (filter.PassFilter(this->sales_product_names[item_num].c_str()))
            {
                this->filtered_sales_product_names.push_back(this->sales_product_names[item_num]);
                this->filtered_sales_brands.push_back(this->sales_brands[item_num]);
                this->filtered_sales_quantities.push_back(this->sales_quantities[item_num]);
                this->filtered_sales_seasons.push_back(this->sales_seasons[item_num]);
                this->filtered_sales_age_groups.push_back(this->sales_age_groups[item_num]);
                this->filtered_sales_genders.push_back(this->sales_genders[item_num]);
                this->filtered_sales_locations.push_back(this->sales_locations[item_num]);
                this->filtered_sales_item_counts.push_back(this->sales_item_counts[item_num]);
                this->filtered_sales_dates_and_times.push_back(this->sales_dates_and_times[item_num]);
            }
        }
        else if (this->return_current_filter_type_index == 2)
        {
            if (filter.PassFilter(this->sales_locations[item_num].c_str()))
            {
                this->filtered_sales_product_names.push_back(this->sales_product_names[item_num]);
                this->filtered_sales_brands.push_back(this->sales_brands[item_num]);
                this->filtered_sales_quantities.push_back(this->sales_quantities[item_num]);
                this->filtered_sales_seasons.push_back(this->sales_seasons[item_num]);
                this->filtered_sales_age_groups.push_back(this->sales_age_groups[item_num]);
                this->filtered_sales_genders.push_back(this->sales_genders[item_num]);
                this->filtered_sales_locations.push_back(this->sales_locations[item_num]);
                this->filtered_sales_item_counts.push_back(this->sales_item_counts[item_num]);
                this->filtered_sales_dates_and_times.push_back(this->sales_dates_and_times[item_num]);
            }
        }
        else if (this->return_current_filter_type_index == 3)
        {
            if (filter.PassFilter(this->sales_dates_and_times[item_num].c_str()))
            {
                this->filtered_sales_product_names.push_back(this->sales_product_names[item_num]);
                this->filtered_sales_brands.push_back(this->sales_brands[item_num]);
                this->filtered_sales_quantities.push_back(this->sales_quantities[item_num]);
                this->filtered_sales_seasons.push_back(this->sales_seasons[item_num]);
                this->filtered_sales_age_groups.push_back(this->sales_age_groups[item_num]);
                this->filtered_sales_genders.push_back(this->sales_genders[item_num]);
                this->filtered_sales_locations.push_back(this->sales_locations[item_num]);
                this->filtered_sales_item_counts.push_back(this->sales_item_counts[item_num]);
                this->filtered_sales_dates_and_times.push_back(this->sales_dates_and_times[item_num]);
            }
        }

       

    }

    ImGui::PopStyleColor();
    ImGui::EndChild();

    return 1;
}

int RUserUi::ReturnSaleWindow::draw_sales_data_table(const float& m_TEXT_BASE_HEIGHT)
{
    bool* checked_statuses = new bool[this->filtered_sales_product_names.size()];
    memset(checked_statuses, 0, this->filtered_sales_product_names.size());
    for (int index : this->checked_sales)
        checked_statuses[index] = true;

    ImVec2 outer_size = ImVec2(0.0f, m_TEXT_BASE_HEIGHT * this->filtered_sales_product_names.size() * 10);
    if (ImGui::BeginTable("table_scrolly", 11, this->table_flags, outer_size))
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
        ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Date&Time", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Check", ImGuiTableColumnFlags_None);

        ImGui::TableHeadersRow();

        std::vector<int>::iterator position;
        bool clicked;
        bool present;


        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin((int)this->filtered_sales_product_names.size());
        while (clipper.Step())
        {
            for (size_t row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 11; column++)
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
                        ImGui::Text(this->filtered_sales_product_names[row].c_str());
                    }
                    else if (column == 2)
                    {
                        ImGui::Text(this->filtered_sales_brands[row].c_str());
                    }
                    else if (column == 3)
                    {
                        ImGui::Text(this->filtered_sales_quantities[row].c_str());
                    }
                    else if (column == 4)
                    {
                        ImGui::Text(this->filtered_sales_seasons[row].c_str());
                    }
                    else if (column == 5)
                    {
                        ImGui::Text(this->filtered_sales_age_groups[row].c_str());
                    }
                    else if (column == 6)
                    {
                        ImGui::Text(this->filtered_sales_genders[row].c_str());
                    }
                    else if (column == 7)
                    {
                        ImGui::Text(this->filtered_sales_locations[row].c_str());
                    }
                    else if (column == 8)
                    {
                        ImGui::Text(this->filtered_sales_item_counts[row].c_str());
                    }
                    else if (column == 9)
                    {
                        ImGui::Text(this->filtered_sales_dates_and_times[row].c_str());
                    }
                    else
                    {
                        position = std::find(this->checked_sales.begin(), this->checked_sales.end(), row);
                        present = (position != this->checked_sales.end()) ? true : false;

                        ImGui::PushID((int)row);
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
                            this->checked_sales.push_back((int)row);
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

int RUserUi::ReturnSaleWindow::regular_user_confirm_message(int& confirm_flag)
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


/*-----------------------------Agents Window-------------------------------*/

//initialize static members
int RUserUi::AgentsWindow::current_filter_type_index = 0;


RUserUi::AgentsWindow::AgentsWindow(mongocxx::database db)
{
    this->agents = db["Agents"];

    table_flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Reorderable;
    //TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    this->agent_details = get_agent_details(this->agents);

    this->names = this->agent_details[1];
    this->phones = this->agent_details[2];
    this->locations = this->agent_details[3];

    this->filtered_names = this->names;
    this->filtered_phones = this->phones;
    this->filtered_locations = this->locations;

    this->p_filter_types = new char[this->filter_types.size() + 1];
    memset(this->p_filter_types, 0, this->filter_types.size());
    strncpy_s(this->p_filter_types, this->filter_types.size() + 1, this->filter_types.c_str(), this->filter_types.size());
    this->prepare_filter_types_pointer(this->filter_types, '_', this->p_filter_types);
}

RUserUi::AgentsWindow::~AgentsWindow()
{
    delete[] this->p_filter_types;
    this->p_filter_types = NULL;
}


int RUserUi::AgentsWindow::create_agents_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 460), ImGuiCond_Once);

    ImGui::Begin("Agents", &this->show_window, this->window_flags);

    // description
    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(100.0, 0.0);
    
    create_search_filter_combo(this->p_filter_types, "Filter Criteria", this->current_filter_type_index, ImVec2(350, 30));

    //filter the agents after a search
    if (this->current_filter_type_index != 0)
    {
        this->create_agents_filter();
    }
    else
    {
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
    this->draw_agents_table(this->TEXT_BASE_HEIGHT);

    ImGui::PopStyleColor();
    ImGui::End();


    return 1;
}

int RUserUi::AgentsWindow::draw_agents_table(const float& m_TEXT_BASE_HEIGHT)
{
    ImVec2 outer_size = ImVec2(0.0f, m_TEXT_BASE_HEIGHT * 90);
    if (ImGui::BeginTable("table_scrolly", 3, this->table_flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Phone", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin((int)this->filtered_names.size());
        while (clipper.Step())
        {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (column == 0)
                    {                        
                        ImGui::Text(this->filtered_names[row].c_str());
                    }
                    else if (column == 1)
                    {
                        ImGui::Text(this->filtered_phones[row].c_str());
                    }
                    else
                    {
                        ImGui::Text(this->filtered_locations[row].c_str());
                    }
                    
                }
            }
        }
        ImGui::EndTable();
    }
    return 1;
}

int RUserUi::AgentsWindow::create_agents_filter()
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
    
    this->filtered_names.clear();
    this->filtered_phones.clear();
    this->filtered_locations.clear();

    for (int item_num = 0; item_num < count; item_num++)
    {    
        if (this->current_filter_type_index == 1)
        {
            if (filter.PassFilter(this->names[item_num].c_str()))
            {
                this->filtered_names.push_back(this->names[item_num]);
                this->filtered_phones.push_back(this->phones[item_num]);
                this->filtered_locations.push_back(this->locations[item_num]);
            }                
        }
        else if (this->current_filter_type_index == 2)
        {
            if (filter.PassFilter(this->phones[item_num].c_str()))
            {
                this->filtered_names.push_back(this->names[item_num]);
                this->filtered_phones.push_back(this->phones[item_num]);
                this->filtered_locations.push_back(this->locations[item_num]);
            }
        }

        else if (this->current_filter_type_index == 3)
        {
            if (filter.PassFilter(this->locations[item_num].c_str()))
            {
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

int RUserUi::AgentsWindow::create_search_filter_combo(char* my_items_names, std::string  description, int& current_item, ImVec2 size)
{    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));
    
    ImGui::BeginChild(description.c_str(), size, true, ImGuiWindowFlags_NoScrollbar);

    ImGui::Combo(description.c_str(), &current_item, (const char*)my_items_names);

    ImGui::PopStyleColor();
    ImGui::EndChild();

    return 1;
}

void RUserUi::AgentsWindow::prepare_filter_types_pointer(const std::string& items, char items_separator, char*& items_pointer)
{
    size_t count = std::count(items.begin(), items.end(), items_separator);

    size_t offset = 0;
    for (int index = 0; index < count; index++)
    {
        offset = items.find_first_of(items_separator, (offset + 1));
        items_pointer[offset] = '\0';
    }

}