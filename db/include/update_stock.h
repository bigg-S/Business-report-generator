#pragma once

/*
* Handles the modification of the stock in the database
*/

#include "app_basic.h"


/*Modification implementation*/
int update_stock(mongocxx::collection stock, std::string navigation_path, bsoncxx::document::element current_amount, SALE sale, MODE mMode);

/*Modifies the amount of an item*/
int update_stock_amountOf_item(mongocxx::collection stock, SALE sale, MODE mMode);

/*Modifies the total of a quantity*/
int update_stock_quantity_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a brand*/
int update_stock_brand_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a product*/
int update_stock_product_total(mongocxx::collection stock, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Reset the contents of the stock*/
int reset_stock(mongocxx::collection products, mongocxx::collection stock, SALE sale, MODE mMode);

/*Set the value of the stock buying price*/
int set_buying_price_total(mongocxx::collection stock, bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays, SALE sl);

/*Update the value of the buying price total*/
int update_buying_price_total(mongocxx::collection stock, double amount, MODE mMode);

/*Update the value of the buying price total during the removal of a brand*/
int update_Tbuying_price_during_brand_removal(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name);

/*Update the value of the buying price total during the removal of a quantity*/
int update_Tbuying_price_during_quantity_removal(mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name);
