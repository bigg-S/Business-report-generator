
#pragma once

/*
* Handles the modification of the contents of the database
*/

#include "app_basic.h"

/*Modification implementation*/
int change(mongocxx::collection products, std::string navigation_path, bsoncxx::document::element current_amount, SALE sale, MODE mMode);

/*Modifies the amount of an item*/
int change_amountOf_item(mongocxx::collection products, SALE sale, MODE mMode);

/*Modifies the total of a gender*/
int change_gender_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of an age group*/
int change_age_group_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a season*/
int change_season_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a quantity*/
int change_quantity_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a brand*/
int change_brand_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Modifies the total of a product*/
int change_product_total(mongocxx::collection products, bsoncxx::document::view product_view, SALE sale, MODE mMode);

/*Reset the contents of the stock*/
int reset_sales(mongocxx::collection products, SALE sale, MODE mMode);

/*save the sale in the sales data document*/
int add_to_sales_details(mongocxx::collection products, SALE sale);

/*remove a sale data from the sales data document*/
int delete_sale_data(mongocxx::collection products, size_t sale_index);
