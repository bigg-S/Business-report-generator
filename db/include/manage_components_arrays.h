#pragma once

/*
* Handles the deletion and insertion elements in the arrays contained in the components document
*/

#include "app_basic.h"

/*-------------------------------------------DELETE----------------------------*/

/*Delete from a common array*/
int delete_item_from_array(mongocxx::collection products, std::string array_name, std::string componets_doc_name, std::string component_name);

/*Delete a product from the products array*/
int delete_product_from_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name);

/*Delete a brand from the brands array*/
int delete_brand_from_components(mongocxx::collection products, mongocxx::collection stock, std::string brand_name, std::string product_name);

/*Delete a quantity*/
int delete_quantity_from_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name);


/*-------------------------------------------INSERT----------------------------*/

/*Insert into a common array*/
int insert_item_into_array(mongocxx::collection products, std::string array_name, std::string componets_doc_name, std::string component_name);

/*Insert a product into the products array*/
int insert_product_into_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::vector<std::string> mbrands, std::vector<std::vector<std::string>> myquantities, std::vector<std::vector<double>> my_SelingPrices, std::vector<std::vector<double>> my_BuyingPrices, std::vector<std::vector<int>> my_numbers_of_items);

/*insert a brand into the brands array*/
int insert_brand_into_components(mongocxx::collection products, mongocxx::collection stock, std::string brand_name, std::vector<std::string> mquantities, std::vector<double> my_SelingPrices, std::vector<double> my_BuyingPrices, std::vector<int> my_numbers_of_items, std::string product_name);

/*Insert a quantity*/
int insert_quantity_into_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name, double my_SelingPrice, double my_BuyingPrice, int my_number_of_items);