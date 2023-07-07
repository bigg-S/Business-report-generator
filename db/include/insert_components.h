#pragma once
/*
* Handles the organisation's agents details, addition, removal etc.
*/

#include "app_basic.h"

/*Insert a location*/
int insert_location(mongocxx::collection products, std::string location);

/*Insert a gender*/
int insert_gender(mongocxx::collection  products, std::string gender);

/*Insert a age group*/
int insert_age_group(mongocxx::collection  products, std::string age_group);

/*Insert a season*/
int insert_season(mongocxx::collection  products, std::string season);

/*Insert a quantity*/
int insert_quantity(mongocxx::collection  products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity, double selling_price, double buying_price, int num_of_items);

/*Insert a brand*/
int insert_brand(mongocxx::collection  products, mongocxx::collection stock, std::string brand, std::vector<std::string> quantities, std::vector<double> selling_price, std::vector<double> buying_price, std::vector<int> number_of_items, std::string product_name);

/*Insert a product*/
int insert_product(mongocxx::collection  products, mongocxx::collection stock, std::string product_name, std::vector<std::string> brand_names, std::vector<std::vector<std::string>> qty_names, std::vector<std::vector<double>> selling_price, std::vector<std::vector<double>> buying_price, std::vector<std::vector<int>> number_of_items);
