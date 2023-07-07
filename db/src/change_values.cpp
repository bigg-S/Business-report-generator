
#include "app_common.h"

#include "change_values.h"
#include "new_docs.h"
#include <manage_OperationalCosts.h>

/*Holder of a found document*/
bsoncxx::stdx::optional<bsoncxx::document::value> product;

/*Holder of a view of a document*/
bsoncxx::document::view product_view;

/*Holder of an elemnt*/
bsoncxx::document::element current_amount;

/*Order of navigation to a specific part in a document with subdocuments*/
std::string navigation_path;

int change(mongocxx::collection products, std::string navigation_path, bsoncxx::document::element current_amount, SALE sale, MODE mMode)
{
    int current_value;
    if (mMode == UPDATE)
    {
        current_value = current_amount.get_int32();
        current_value += sale.item_count;
    }
    else
        current_value = 0;

    // Update an embedded document in a single document.
                              /*The top level document to be updated*/    /*The update*/
    products.update_one(make_document(kvp("name", sale.product_name)), make_document(kvp("$set", make_document(kvp(navigation_path, current_value)))));

    return 1;

}

int change_amountOf_item(mongocxx::collection products, SALE sale, MODE mMode)
{   
    navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + sale.season + "." + sale.age_group + "." + sale.gender + "." + sale.location;

    product = products.find_one(document{} << "name" << sale.product_name << finalize);
    product_view = product.value().view();
    current_amount = product_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group][sale.gender][sale.location];

    if(sale.item_count > 0)
        add_to_sales_details(products, sale);

    change(products, navigation_path, current_amount, sale, mMode);

    change_gender_total(products, product_view, sale, mMode);
    change_age_group_total(products, product_view, sale, mMode);
    change_season_total(products, product_view, sale, mMode);
    change_quantity_total(products, product_view, sale, mMode);
    change_brand_total(products, product_view, sale, mMode);
    change_product_total(products, product_view, sale, mMode);

    return 1;

}

int change_gender_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + sale.season + "." + sale.age_group + "." + sale.gender + "." + "Total";
    current_amount = product_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group][sale.gender]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}

int change_age_group_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + sale.season + "." + sale.age_group + "." + "Total";
    current_amount = product_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}

int change_season_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + sale.season + "." + "Total";
    current_amount = product_view["Brands"][sale.brand][sale.quantity][sale.season]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}

int change_quantity_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + "Total";
    current_amount = product_view["Brands"][sale.brand][sale.quantity]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}

int change_brand_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands." + sale.brand + "." + "Total";
    current_amount = product_view["Brands"][sale.brand]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}

int change_product_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    navigation_path = "Brands.Total";
    current_amount = product_view["Brands"]["Total"];

    change(products, navigation_path, current_amount, sale, mMode);

    return 1;

}


int reset_sales(mongocxx::collection products, SALE sale, MODE mMode)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view components_arrays_view = components_arrays.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> complete_sales;
    bsoncxx::document::view complete_sales_view;

    bsoncxx::array::view prodtcs, brands, quantities, seasons, age_groups, genders, locations;

    prodtcs = components_arrays_view["Products"].get_array();

    std::string nav_path;
    bsoncxx::document::element current_amount;
    
    for (bsoncxx::array::element p : prodtcs)
    {
        complete_sales = products.find_one(document{} << "name" << p.get_string().value.to_string() << finalize);
        complete_sales_view = components_arrays.value().view();

        sale.product_name = p.get_string().value.to_string();
        change_product_total(products, complete_sales_view, sale, mMode);

        brands = components_arrays_view["Brands"][p.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element b : brands)
        {
            sale.brand = b.get_string().value.to_string();
            change_brand_total(products, complete_sales_view, sale, mMode);

            quantities = components_arrays_view["Quantities"][b.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element q : quantities)
            {
                sale.quantity = q.get_string().value.to_string();
                change_quantity_total(products, complete_sales_view, sale, mMode);

                seasons = components_arrays_view["Seasons"].get_array();

                for (bsoncxx::array::element s : seasons)
                {
                    sale.season = s.get_string().value.to_string();
                    change_season_total(products, complete_sales_view, sale, mMode);

                    age_groups = components_arrays_view["Age Groups"].get_array();

                    for (bsoncxx::array::element a : age_groups)
                    {
                        sale.age_group = a.get_string().value.to_string();
                        change_age_group_total(products, complete_sales_view, sale, mMode);

                        genders = components_arrays_view["Genders"].get_array();

                        for (bsoncxx::array::element g : genders)
                        {
                            sale.gender = g.get_string().value.to_string();
                            change_gender_total(products, complete_sales_view, sale, mMode);

                            locations = components_arrays_view["Locations"].get_array();
                            for (bsoncxx::array::element l : locations)
                            {                                 
                                sale.location = l.get_string().value.to_string();
                                nav_path = "Brands." + sale.brand + "." + sale.quantity + "." + sale.season + "." + sale.age_group + "." + sale.gender + "."  + sale.location;
                                change(products, nav_path, current_amount, sale, mMode);
                            }
                        }
                    }
                }
            }

        }

    }
    return  1;
}

int reset_sales_data(mongocxx::collection products)
{
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << "SalesData"
        << "indices"
        << bsoncxx::builder::stream::open_array
        << bsoncxx::builder::stream::close_array
        << "current_index" << 0

        << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view mview = doc_value.view();

    products.replace_one(make_document(kvp("name", "SalesData")), mview);

    return 1;
}


int add_to_sales_details(mongocxx::collection products, SALE sale)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> sales_data = products.find_one(document{} << "name" << "SalesData" << finalize);
    bsoncxx::document::view sales_data_view = sales_data.value().view();

    int current_sale_index = sales_data_view["current_index"].get_int32();

    std::string path = std::to_string(current_sale_index);
    std::string t_date = get_expense_date();
    t_date = t_date.substr(0, t_date.size() - 1);

    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Product", sale.product_name + "_" + t_date)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Brand", sale.brand)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Quantity", sale.quantity)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Season", sale.season)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Age Group", sale.age_group)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Gender", sale.gender)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Location", sale.location)))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp(path + "." + "Item Count", sale.item_count)))));
    
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp("current_index", current_sale_index + 1)))));

    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$push", make_document(kvp("indices", std::to_string(current_sale_index))))));


    return 1;
}

int delete_sale_data(mongocxx::collection products, size_t sale_index)
{

    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$unset", make_document(kvp(std::to_string(sale_index), "")))));
    products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$pull", make_document(kvp("indices", std::to_string(sale_index))))));

    /*----------------------Reset current sale index to 0 if there are no sales in the document------------------------------------*/

    bsoncxx::stdx::optional<bsoncxx::document::value> sales_data = products.find_one(document{} << "name" << "SalesData" << finalize);
    bsoncxx::document::view sales_data_view = sales_data.value().view();

    bsoncxx::array::view indices = sales_data_view["indices"].get_array();

    if (indices.empty())
        products.update_one(make_document(kvp("name", "SalesData")), make_document(kvp("$set", make_document(kvp("current_index", 0)))));

    /*------------------------------------------------------------------------------------------------------------------------------*/

    return 1;
}


