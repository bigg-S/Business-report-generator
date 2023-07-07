#pragma once

/*
* Handles the manipulation of the selling price in the database
*/

#include "app_basic.h"

/*Set the price of a particular quantity*/
int set_price(mongocxx::collection products, std::string mproduct, std::string brand, std::string quantity, double price, std::string price_type, int number_of_items);

/*Set the price for the quantities of a brand*/
int set_brand_quantiities_price(mongocxx::collection products, std::string mproduct, std::string brand, std::vector<std::string> quantities, std::vector<double> prices, std::string price_type, std::vector<int> number_of_items);

/*Set the price for the quantities of the brands of a product*/
int set_product_brands_quantiities_price(mongocxx::collection products, std::string mproduct, std::vector<std::string> brands, std::vector<std::vector<std::string>> quantities, std::vector<std::vector<double>> prices, std::string price_type, std::vector<std::vector<int>> number_of_items);

/*Change the buying or selling price of a quantity*/
int change_price(mongocxx::collection products, std::string mproduct, std::string brand, std::string quantity, double price, std::string price_type);