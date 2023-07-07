#pragma once

/*
* Handles the deletion from the stock in the database
*/

#include "app_basic.h"

/*Delete a quantity from the stock*/
int delete_quantity_from_stock(mongocxx::collection stock, std::string mproduct, std::string brand, std::string quantity);

/*Delete a brand from the stock*/
int delete_brand_from_stock(mongocxx::collection stock, std::string mproduct, std::string brand);

/*/*Delete a product from the stock*/
int delete_product_from_stock(mongocxx::collection stock, std::string mproduct);