#pragma once

/*
* Handles the creation of new documents in the exsting collections of data from the stock in the database
*/

#include "app_basic.h"


void create_stock_buying_price_doc(mongocxx::collection stock_products);

void create_agent_doc(mongocxx::collection agents);

void create_agent_sale_doc(mongocxx::collection agent_sales);

void create_new_product_doc(mongocxx::collection products, std::string product_name);