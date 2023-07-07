
#include "admin_ui/admin_ui.h"
#include "login_ui/login_ui.h"

#include "marketer_ui/marketer_ui.h"
#include <imgui_internal.h>
#include <thread>

bool marketer_window = false;


MUserUi::ShowMarketerWindow::ShowMarketerWindow(GeneralReportWindow& gr_window, CustomizedReportWindow& cr_window, PassWord& change_password)
{
    this->gr_window = &gr_window;
    this->cr_window = &cr_window;
    this->change_password = &change_password;
}

int MUserUi::ShowMarketerWindow::create_show_marketer_window()
{
    ImGui::Dummy(ImVec2(0.0, 60.0));
    ImGui::NewLine();
    ImGui::SameLine(50.0, 0.0);
    if (ImGui::Button("General Report", ImVec2(115.0, 20.0)))
        this->gr_window->show_window = true;

    if (this->gr_window->show_window)
        this->gr_window->create_general_report_window();

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(50.0, 0.0);
    if (ImGui::Button("Custom Report", ImVec2(115.0, 20.0)))
        this->cr_window->show_window = true;

    if (this->cr_window->show_window)
        this->cr_window->create_customized_report_window();

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(5 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(5 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(5 / 7.0f, 0.8f, 0.8f));

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(50.0, 0.0);
    if (ImGui::Button("Log Out", ImVec2(115.0, 20.0)))
    {
        login_window = true;
        marketer_window = false;
        admin_window = false;
        regular_user_window = false;
        finance_window = false;
        showing_admin_extensions = false;

    }
      
    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f / 7.0f, 0.8f, 0.8f));

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

MUserUi::WordDocument::WordDocument(GeneralReportWindow *gr_window, CustomizedReportWindow* cr_window)
{
    if (gr_window != NULL)
    {
        this->gr_window = gr_window;
        this->file_name = gr_window->file_name;
    }
       
    else
    {
        this->file_name = cr_window->file_name;
        this->cr_window = cr_window;
    }

   
    
}

void MUserUi::WordDocument::initialize()
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
}

void MUserUi::WordDocument::uninitialize()
{
    pDoc->Close(&fls);
    pWordApp->Quit();

    if(this->cr_window != NULL)
        this->cr_window->saving = false; //stop showing the saving indicator circle
    else
        this->gr_window->saving = false;

}


int MUserUi::WordDocument::create_document(std::string caller)
{
    this->initialize();
    try
    {
        //Word
        pWordApp.CreateInstance(_uuidof(Word::Application));
        //pWordApp->Visible = VARIANT_TRUE;
        
        dptr = this->pWordApp->Documents;
        pDoc = dptr->Add();

        p_Paras = pDoc->Paragraphs;
        p_Para = p_Paras->Add();

       
        pDoc->ActiveWindow->Selection->InsertAfter("SALES REPORT\n");
        paraformat = pDoc->ActiveWindow->Selection->ParagraphFormat;
        paraformat->Alignment = Word::wdAlignParagraphCenter;

        if (caller.compare("General Report") == 0)
        {    
            
            pDoc->ActiveWindow->Selection->InsertAfter(this->gr_window->seasons_data.c_str());

            this->gr_window->age_groups_data = "\n" + this->gr_window->age_groups_data;
            pDoc->ActiveWindow->Selection->InsertAfter(this->gr_window->age_groups_data.c_str());

            this->gr_window->genders_data = "\n" + this->gr_window->genders_data;
            pDoc->ActiveWindow->Selection->InsertAfter(this->gr_window->genders_data.c_str());

            this->gr_window->locations_data = "\n" + this->gr_window->locations_data;
            pDoc->ActiveWindow->Selection->InsertAfter(this->gr_window->locations_data.c_str());

            
        }
        else
        {
            pDoc->ActiveWindow->Selection->InsertAfter(this->cr_window->customized_data.c_str());
        }

        pDoc->Sentences->Item(1)->Font->Size = 16;
        pDoc->Sentences->Item(1)->Bold = Word::wdToggle;

        this->file_name += ".docx";
        bstr_t nm = &this->file_name[0];
        variant_t fname = nm;

        pDoc->SaveAs(&fname);
        pWordApp->DisplayAlerts = Word::wdAlertsNone;

        this->uninitialize();
    }
    catch (_com_error& err)
    {
        printf("Word throws the error: %s\n", (char*)err.ErrorMessage());

        printf("Description: %s\n", (LPCSTR)err.Description());
    }
    CoUninitialize();

    return 1;
}

MUserUi::GeneralReportWindow::GeneralReportWindow(mongocxx::database db, GLFWwindow* window)
{
    this->sales_collection = db["products"];

    this->v_seasons = get_seasons_data(this->sales_collection);
    this->v_age_groups = get_age_groups_data(this->sales_collection);
    this->v_genders = get_genders_data(this->sales_collection);
    this->v_locations = get_locations_data(this->sales_collection);

    this->window = window;
    this->main_window = glfwGetWin32Window(this->window);


}


int MUserUi::GeneralReportWindow::create_general_report_window()
{
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(820, 600), ImGuiCond_Once);

    ImGui::Begin("General Report", &this->show_window, this->window_flags | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(270.0, 0.0);
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.5f, 1.0f), "NUMBER OF SALES IN DIFFERENT CATEGORIES");

    ImGui::NewLine();
    ImGui::SameLine(0.0, 12.0);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 255, 255, 255));
    ImGui::BeginChild("Sales Report", ImVec2(790, 470), true);

    this->display_report();

    if (this->saving)
        this->LoadingIndicatorCircle("Loading", 30.0f, ImVec4(0.0f, 0.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 1.0f, 1.0f), 15, 7.0f);


    ImGui::PopStyleColor();
    ImGui::EndChild();    

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(360.0, 0.0);

    if(this->saving)
        ImGui::BeginDisabled(true);
    if (ImGui::Button("Save", ImVec2(100.0, 20.0)))
    {       
        this->file_name = this->get_file_name(this->main_window);

        if (this->file_name.compare("") != 0)
        {
            std::thread save_thread(&MUserUi::GeneralReportWindow::create_doc, this);
            save_thread.detach();
            
        }
       
                    
        
    } 

    if(this->saving)
        ImGui::EndDisabled();


    ImGui::PopStyleColor();
    ImGui::End();

    return 1;
}


int MUserUi::GeneralReportWindow::display_report()
{
   
    //display seasons
    this->display_category_data(this->v_seasons, "SEASONS", this->seasons_data);

    ImGui::Separator();

    //display age groups
    this->display_category_data(this->v_age_groups, "AGE GROUPs", this->age_groups_data);

    ImGui::Separator();

    
    //display genders
    this->display_category_data(this->v_genders, "GENDERS", this->genders_data);

    ImGui::Separator();

    //display locations
    this->display_category_data(this->v_locations, "LOCATIONS", this->locations_data);

    ImGui::Separator();

    return 1;
}

int MUserUi::GeneralReportWindow::display_category_data(std::vector<CategoryData> v_category, std::string category_name, std::string &stored_category_data)
{
     std::string details = "**************************** "+ category_name +"*****************************************";

    stored_category_data = "";
    
    stored_category_data += details;
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());
    //display the seasons data
    for (int s_count = 0; s_count < v_category.size(); s_count++)
    {
        stored_category_data += "\n";

        ImGui::Dummy(ImVec2(0.0, 10.0));
        ImGui::NewLine();

        details = std::to_string(s_count + 1) + "." + v_category[s_count].category_name + ": " + std::to_string(v_category[s_count].category_total);

        stored_category_data += details;

        ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());

        ImGui::Indent();
        for (int p_count = 0; p_count < v_category[s_count].p_products.size(); p_count++)
        {

            details = std::to_string(p_count + 1) + "." + v_category[s_count].p_products[p_count].product_name + ":" +
                std::to_string(v_category[s_count].p_products[p_count].product_total);

            stored_category_data += "\n\t" + details;

            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());
            ImGui::Indent();

            for (int b_count = 0; b_count < v_category[s_count].p_products[p_count].p_brands.size(); b_count++)
            {
                details = std::to_string(b_count + 1) + "." + v_category[s_count].p_products[p_count].p_brands[b_count].brand_name + ":" +
                    std::to_string(v_category[s_count].p_products[p_count].p_brands[b_count].brand_total);

                stored_category_data += "\n\t\t" + details;

                ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());
                ImGui::Indent();

                for (int q_count = 0; q_count < v_category[s_count].p_products[p_count].p_brands[b_count].b_quantities.size(); q_count++)
                {
                    details = std::to_string(q_count + 1) + "." + v_category[s_count].p_products[p_count].p_brands[b_count].b_quantities[q_count].quantity_name + ":" +
                        std::to_string(v_category[s_count].p_products[p_count].p_brands[b_count].b_quantities[q_count].quantity_total);

                    stored_category_data += "\n\t\t\t"  + details;

                    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());

                }

                ImGui::Unindent();

            }

            ImGui::Unindent();

        }

        ImGui::Unindent();
    }

    stored_category_data += "\n";
    return 1;
}

std::string MUserUi::GeneralReportWindow::get_file_name(HWND hwnd)
{
    OPENFILENAMEA ofn; //common dialog box structure
    CHAR szFile[260];  // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = (LPSTR)"Word Document\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    // Display the Save dialog box.

    if (GetSaveFileNameA(&ofn) == TRUE)
        return szFile;
    else
        return "";
}

int MUserUi::GeneralReportWindow::LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color,
    const ImVec4& backdrop_color, const int circle_count, const float speed)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
        return 0;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
    const ImVec2 pos = window_center;
    
    const float circle_radius = indicator_radius / 10.0f;
    const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f,
                                pos.y + indicator_radius * 2.0f));

    ImGui::ItemSize(bb, g.Style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
    {
        return 0;
    }

    const float t = (float)g.Time;
    const auto degree_offset = 2.0f * IM_PI / circle_count;

    for (int i = 0; i < circle_count; ++i)
    {
        const auto x = indicator_radius * std::sin(degree_offset * i);
        const auto y = indicator_radius * std::cos(degree_offset * i);

        const auto growth = max(0.0f, std::sin(t * speed - i * degree_offset));

        ImVec4 color;
        color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
        color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
        color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
        color.w = 1.0f;

        window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
                                                 pos.y + indicator_radius - y),
                                                 circle_radius + growth * circle_radius,
                                                 ImGui::GetColorU32(color));
    }


    return 1;
}



void MUserUi::GeneralReportWindow::create_doc()
{   
    this->saving = true;
     
    MUserUi::WordDocument wrd_win(this, NULL);
    wrd_win.create_document("General Report");            
   
}


int MUserUi::CustomizedReportWindow::current_selected_brand = 0;
MUserUi::CustomizedReportWindow::CustomizedReportWindow(mongocxx::database db, GLFWwindow* window)
{
    this->sales_collection = db["products"];
   
    this->window = window;
    this->main_window = glfwGetWin32Window(this->window);

    this->v_products = get_products_sales_data(this->sales_collection);

    this->populate_products();
}

int MUserUi::CustomizedReportWindow::create_customized_report_window()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(60, 60, 60, 255));

    ImGui::SetNextWindowPos(ImVec2(100, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520, 620), ImGuiCond_Once);

    ImGui::Begin("Customized Report", &this->show_window, this->window_flags | ImGuiWindowFlags_NoScrollWithMouse);

    
    ImGui::SameLine(160.0, 0.0);
    /*-----------------------------Product------------------------------------*/
    this->populate_products();
    create_listbox_filter(this->products, "Products", this->selected_product, '_', ImVec2(210, 150));

    /*------------------------------------------------------------------------------*/

    ImVec2 combo_size = ImVec2(210, 50);

    ImGui::NewLine();
    ImGui::SameLine(160.0, 0.0);
    /*-----------------------------------Brand-------------------------------------*/

    create_combo(this->p_brands, "Brand", this->current_selected_brand, combo_size);

    /*-------------------------------------------------------------------------------*/


    ImGui::Separator();
    ImGui::SameLine(140.0, 0.0);
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.5f, 1.0f), "Select An Item To Customize");

    ImGui::NewLine();
    ImGui::SameLine(0.0, 12.0);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 255, 255, 255));
    ImGui::BeginChild("Customized Report", ImVec2(480, 300), true);

    if (this->selected_product.compare("All Products") == 0)
    {
        this->display_products_sales();
        this->reset_combo_items();
        this->current_selected_brand = 0;        

    }
    else if (this->current_selected_brand == 0 && (product_changed == false))
    {
        this->display_brands_sales();
    }
    else if (product_changed == true)
    {
        this->display_brands_sales();
        this->current_selected_brand = 0;       
        product_changed = false;
    }
    else if (this->current_selected_brand != 0 && (product_changed == false))
    {
        display_brand_quantities_sales();
    }
    
    if (this->saving)
        this->LoadingIndicatorCircle("Loading", 30.0f, ImVec4(0.0f, 0.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 1.0f, 1.0f), 15, 7.0f);

    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0.0, 10.0));
    ImGui::NewLine();
    ImGui::SameLine(200.0, 0.0);

    if (this->saving)
        ImGui::BeginDisabled(true);
    if (ImGui::Button("Save", ImVec2(100.0, 20.0)))
    {
        this->file_name = this->get_file_name(this->main_window);

        std::thread save_thread(&MUserUi::CustomizedReportWindow::create_custom_doc, this);
        save_thread.detach();

    }

    if (this->saving)
        ImGui::EndDisabled();

    ImGui::PopStyleColor();
    ImGui::End();
    return 1;
}

int MUserUi::CustomizedReportWindow::populate_products()
{
    this->products = "All Products_";
    this->products_array.clear();

    for (ProductData p_data : this->v_products)
    {
        this->products += p_data.product_name + "_";
        this->products_array.push_back(p_data.product_name);
    }

    return 1;
}

int MUserUi::CustomizedReportWindow::populate_brands(std::vector<BrandData> v_brands)
{
    this->brands = "Select_";
    this->brands_array.clear();

    for (BrandData b_data : v_brands)
    {
        this->brands += b_data.brand_name + "_";
        this->brands_array.push_back(b_data.brand_name);
    }

    return 1;
}

int MUserUi::CustomizedReportWindow::display_products_sales()
{    
    
    std::string details = "";

    this->customized_data = "";

    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Products Sales\n");
    ImGui::Indent();

    this->customized_data += "Products Sales\n";

    int product_index = 1;
    for ( ProductData product : this->v_products)
    {
        details = std::to_string(product_index) + ". " + product.product_name.c_str() + std::string(": ") + std::to_string(product.product_total) + "\n";
        this->customized_data +=  "\t" + details;
        ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());

        product_index++;
    }

    ImGui::Unindent();        
    
    return 1;
}

int MUserUi::CustomizedReportWindow::display_brands_sales()
{

    std::string details = "";

    this->customized_data = "";

    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Product Brands Sales\n");
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), this->selected_product.c_str());
    ImGui::Indent();

    this->customized_data += "Product Brands Sales\n" + this->selected_product + "\n";

    auto iterator  = std::find(this->products_array.begin(), this->products_array.end(), this->selected_product);
    
    if (iterator == this->products_array.end())
        return 0;
    else
        this->product_index = (int)(iterator - this->products_array.begin());

    this->populate_brands(this->v_products[this->product_index].p_brands);

    int brand_index = 1;
    for (BrandData brand : this->v_products[this->product_index].p_brands)
    {
        details = std::to_string(brand_index) + ". " + brand.brand_name.c_str() + std::string(": ") + std::to_string(brand.brand_total) + "\n";
        this->customized_data +=  "\t" + details;

        ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());

        brand_index++;
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

int MUserUi::CustomizedReportWindow::display_brand_quantities_sales()
{
    std::string details = "";
    this->customized_data = "";

    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Brand Quantities Sales\n");
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), this->v_products[this->product_index].p_brands[this->current_selected_brand - 1].brand_name.c_str());
    ImGui::Indent();

    this->customized_data += "Brand Quantities Sales\n" + this->v_products[this->product_index].product_name + "\n\t" + this->v_products[this->product_index].p_brands[this->current_selected_brand - 1].brand_name;

    int quantity_index = 1;
    for (QuantityData quantity : this->v_products[this->product_index].p_brands[this->current_selected_brand - 1].b_quantities)
    {
        details = std::to_string(quantity_index) + ". " + quantity.quantity_name.c_str() + std::string(": ") + std::to_string(quantity.quantity_total);
        this->customized_data += "\n\t\t" + details;
        ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), details.c_str());

        quantity_index++;
    }

    ImGui::Unindent();

    this->customized_data += "\n";
    return 1;
}


void MUserUi::CustomizedReportWindow::create_custom_doc()
{
    this->saving = true;

    MUserUi::WordDocument wrd_win(NULL, this);
    wrd_win.create_document("Customized Report");

}
