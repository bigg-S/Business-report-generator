

#include "update_stock.h"

#include "app_common.h"
#include "compute_costs.h"


/*Holder of a found document*/
bsoncxx::stdx::optional<bsoncxx::document::value> s_product;

/*Holder of a view of a document*/
bsoncxx::document::view s_product_view;

/*Holder of an elemnt*/
bsoncxx::document::element s_current_amount;

/*Order of navigation to a specific part in a document with subdocuments*/
std::string s_navigation_path;

int update_stock(mongocxx::collection stock, std::string navigation_path, bsoncxx::document::element current_amount, SALE sale, MODE mMode)
{
    int current_value;
    if (mMode == UPDATE)
    {
        current_value = current_amount.get_int32();
        current_value -= sale.item_count;
    }
    else
        current_value = 0;

    // Update an embedded document in a single document.
                              /*The top level document to be updated*/    /*The update*/
    stock.update_one(make_document(kvp("name", sale.product_name)), make_document(kvp("$set", make_document(kvp(navigation_path, current_value)))));

    return 1;

}

int update_stock_amountOf_item(mongocxx::collection stock, SALE sale, MODE mMode)
{
    
    s_navigation_path = "Brands." + sale.brand + "." + sale.quantity + ".Total" ;

    s_product = stock.find_one(document{} << "name" << sale.product_name << finalize);
    s_product_view = s_product.value().view();
    s_current_amount = s_product_view["Brands"][sale.brand][sale.quantity]["Total"];

    update_stock(stock, s_navigation_path, s_current_amount, sale, mMode);

    update_stock_quantity_total(stock, s_product_view, sale, mMode);
    update_stock_brand_total(stock, s_product_view, sale, mMode);
    update_stock_product_total(stock, s_product_view, sale, mMode);

    return 1;

}



int update_stock_quantity_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    
    s_navigation_path = "Brands." + sale.brand + "." + sale.quantity + "." + "Total";
    s_current_amount = product_view["Brands"][sale.brand][sale.quantity]["Total"];

    update_stock(stock, s_navigation_path, s_current_amount, sale, mMode);

    return 1;

}

int update_stock_brand_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    
    s_navigation_path = "Brands." + sale.brand + "." + "Total";
    s_current_amount = product_view["Brands"][sale.brand]["Total"];

    update_stock(stock, s_navigation_path, s_current_amount, sale, mMode);

    return 1;

}

int update_stock_product_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode)
{
    s_navigation_path = "Brands.Total";
    s_current_amount = product_view["Brands"]["Total"];

    update_stock(stock, s_navigation_path, s_current_amount, sale, mMode);

    return 1;

}


int reset_stock(mongocxx::collection products, mongocxx::collection stock, SALE sale, MODE mMode)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view components_arrays_view = components_arrays.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> stock_products;
    bsoncxx::document::view stock_products_view;

    bsoncxx::array::view prodtcs, brands, quantities;

    prodtcs = components_arrays_view["Products"].get_array();

    std::string nav_path;
    bsoncxx::document::element current_amount;

    for (bsoncxx::array::element p : prodtcs)
    {
        stock_products = stock.find_one(document{} << "name" << p.get_string().value.to_string() << finalize);
        stock_products_view = stock_products.value().view();

        sale.product_name = p.get_string().value.to_string();
        update_stock_product_total(stock, stock_products_view, sale, mMode);

        brands = components_arrays_view["Brands"][p.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element b : brands)
        {
            sale.brand = b.get_string().value.to_string();
            update_stock_brand_total(stock, stock_products_view, sale, mMode);

            quantities = components_arrays_view["Quantities"][b.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element q : quantities)
            {
                sale.quantity  = q.get_string().value.to_string();
                nav_path       = "Brands." + sale.brand + "." + sale.quantity + ".Total";
                update_stock(stock, nav_path, current_amount, sale, mMode);
            }

        }

    }

    return 1;
}


int set_buying_price_total(mongocxx::collection stock, bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays, SALE sl)
{
    double mystockcosts = compute_total_cost(stock, components_arrays, sl, "BuyingPrice");

    stock.update_one(make_document(kvp("name", "BuyingPrice")), make_document(kvp("$set", make_document(kvp("Total", mystockcosts)))));

    return 1;
}


int update_buying_price_total(mongocxx::collection stock, double amount, MODE mMode)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> buying_price = stock.find_one(document{} << "name" << "BuyingPrice" << finalize);
    bsoncxx::document::view buying_price_view = buying_price.value().view();

    bsoncxx::document::element current_amount = buying_price_view["Total"];

    double new_amount = 0.0, current_price = 0.0;

    if (current_amount.type() == bsoncxx::type::k_double)
        current_price = current_amount.get_double();
    else if (current_amount.type() == bsoncxx::type::k_int64)
        current_price = (double)current_amount.get_int64();
    else if (current_amount.type() == bsoncxx::type::k_int32)
        current_price = (double)current_amount.get_int32();

    if (mMode == ADD)
        new_amount = current_price + amount;
    else if(mMode == REMOVE)
        new_amount = current_price - amount;    

    stock.update_one(make_document(kvp("name", "BuyingPrice")), make_document(kvp("$set", make_document(kvp("Total", new_amount)))));

    return 1;
}


int update_Tbuying_price_during_brand_removal(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name)
{
    
    bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view components_arrays_view = components_arrays.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> stock_products = stock.find_one(document{} << "name" << product_name << finalize);
    bsoncxx::document::view stock_products_view = stock_products.value().view();

    bsoncxx::array::view brand_quantities = components_arrays_view["Quantities"][brand_name].get_array();

    bsoncxx::document::element current_amount;
    int number_of_items = 0;

    bsoncxx::document::element buying_price;
    double price = 0.0;

    double total_buying_price = 0.0;

    for (bsoncxx::array::element q : brand_quantities)
    {
        current_amount = stock_products_view["Brands"][brand_name][q.get_string().value.to_string()]["Total"];
        number_of_items = current_amount.get_int32();

        buying_price = stock_products_view["Brands"][brand_name][q.get_string().value.to_string()]["BuyingPrice"];

        if (buying_price.type() == bsoncxx::type::k_double)
            price = buying_price.get_double();
        else if (buying_price.type() == bsoncxx::type::k_int64)
            price = (double)buying_price.get_int64();
        else if (buying_price.type() == bsoncxx::type::k_int32)
            price = (double)buying_price.get_int32();
        else
            return 0;

        total_buying_price += price * (double)number_of_items;
        
    }

    update_buying_price_total(stock, total_buying_price, REMOVE);

    return 1;
}

int update_Tbuying_price_during_quantity_removal(mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name)
{
    
    bsoncxx::stdx::optional<bsoncxx::document::value> stock_products = stock.find_one(document{} << "name" << product_name << finalize);
    bsoncxx::document::view stock_products_view = stock_products.value().view();

    bsoncxx::document::element current_amount;
    int number_of_items = 0;

    bsoncxx::document::element buying_price;
    double price = 0.0;

    double total_buying_price = 0.0;

    current_amount = stock_products_view["Brands"][brand_name][quantity_name]["Total"];
    number_of_items = current_amount.get_int32();

    buying_price = stock_products_view["Brands"][brand_name][quantity_name]["BuyingPrice"];

    if (buying_price.type() == bsoncxx::type::k_double)
        price = buying_price.get_double();
    else if (buying_price.type() == bsoncxx::type::k_int64)
        price = (double)buying_price.get_int64();
    else if (buying_price.type() == bsoncxx::type::k_int32)
        price = (double)buying_price.get_int32();
    else
        return 0;

    total_buying_price = price * (double)number_of_items;

    update_buying_price_total(stock, total_buying_price, REMOVE);

    return 1;
}
