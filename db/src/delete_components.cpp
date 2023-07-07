
#include <iostream>

#include "delete_components.h"
#include "manage_components_arrays.h"

bsoncxx::array::view brands;

bsoncxx::array::view quantities;

bsoncxx::array::view seasons;

bsoncxx::array::view age_groups;

bsoncxx::array::view genders;

bsoncxx::array::view locations;

std::string nav_path;

int delete_location(mongocxx::collection products, std::string location)
{
    delete_item_from_array(products, "Locations", "Components", location);

    bsoncxx::stdx::optional<bsoncxx::document::value> product = products.find_one(document{} << "name" << "Components" << finalize);

    bsoncxx::document::view product_view = product.value().view();

    bsoncxx::array::view products_array = product_view["Products"].get_array();
   
    for (bsoncxx::array::element b : products_array)
    {
        brands = product_view["Brands"][b.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element x : brands)
        {
            quantities = product_view["Quantities"][x.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element z : quantities)
            {
                seasons = product_view["Seasons"].get_array();

                for (bsoncxx::array::element q : seasons)
                {
                    age_groups = product_view["Age Groups"].get_array();

                    for (bsoncxx::array::element g : age_groups)
                    {
                        genders = product_view["Genders"].get_array();

                        for (bsoncxx::array::element a : genders)
                        {
                            nav_path = "Brands." + x.get_string().value.to_string() + "." + z.get_string().value.to_string() + "." +
                                q.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + location;

                                products.update_one(make_document(kvp("name", b.get_string().value.to_string())), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));
                        };
                    };
                };
            };
        };
    };
	return 1;
}


int delete_age_group(mongocxx::collection products, std::string age_group)
{
    delete_item_from_array(products, "Age Groups", "Components", age_group);
    bsoncxx::stdx::optional<bsoncxx::document::value> product = products.find_one(document{} << "name" << "Components" << finalize);

    bsoncxx::document::view product_view = product.value().view();

    bsoncxx::array::view prodtcs = product_view["Products"].get_array();

    for (bsoncxx::array::element b : prodtcs)
    {
        brands = product_view["Brands"][b.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element x : brands)
        {
            quantities = product_view["Quantities"][x.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element z : quantities)
            {
                seasons = product_view["Seasons"].get_array();

                for (bsoncxx::array::element q : seasons)
                {
                   nav_path = "Brands." + x.get_string().value.to_string() + "." + z.get_string().value.to_string() + "." +
                       q.get_string().value.to_string() + "." + age_group;

                   products.update_one(make_document(kvp("name", b.get_string().value.to_string())), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));
                   
                };
            };
        };
    };
    return 1;
}


int delete_season(mongocxx::collection products, std::string season)
{
    delete_item_from_array(products, "Seasons", "Components", season);

    bsoncxx::stdx::optional<bsoncxx::document::value> product = products.find_one(document{} << "name" << "Components" << finalize);

    bsoncxx::document::view product_view = product.value().view();

    bsoncxx::array::view prodtcs = product_view["Products"].get_array();

    for (bsoncxx::array::element b : prodtcs)
    {
        brands = product_view["Brands"][b.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element x : brands)
        {
            quantities = product_view["Quantities"][x.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element z : quantities)
            {
                    nav_path = "Brands." + x.get_string().value.to_string() + "." + z.get_string().value.to_string() + "." +  season;

                    products.update_one(make_document(kvp("name", b.get_string().value.to_string())), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));

            };
        };
    };
    return 1;
}


int delete_quantity(mongocxx::collection products, mongocxx::collection stock, std::string mproduct, std::string brand, std::string quantity)
{
    brand = mproduct + "_" + brand;
    delete_quantity_from_components(products, stock, mproduct, brand, quantity);

    nav_path = "Brands." + brand + "." + quantity;
    products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));
        
    return 1;
}


int delete_brand(mongocxx::collection products, mongocxx::collection stock, std::string mproduct, std::string brand)
{
    brand = mproduct + "_" + brand;

    delete_brand_from_components(products, stock, brand, mproduct);

    nav_path = "Brands." + brand;
    products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));

    return 1;
}

int delete_product(mongocxx::collection products, mongocxx::collection stock, std::string mproduct)
{
    delete_product_from_components(products, stock, mproduct);

    products.delete_one(make_document(kvp("name", mproduct)));

    return 1;
}