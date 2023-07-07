#pragma once

/*
* Handles the sales that an agent has done and are not yet cleared
*/

#include "app_basic.h"
/*Add a new agent sale document to store the sales made by an agent before clearance*/
int create_new_agent_sale_doc(mongocxx::collection agent_sales, int64_t id);

/*Delete an agent's sale document*/
int delete_new_agent_sale_doc(mongocxx::collection agent_sales, int64_t id);

/*Add an agent sale*/
int add_agent_sale(mongocxx::collection agent_sales, int64_t id, SALE sale);

/*Delete an agent sale*/
int delete_agent_sale(mongocxx::collection agent_sales, int64_t id, int sale_index);

/*Get sales data*/
/*
*vector @ index 0 = products
* 				1 = brands
* 				2 = quantities
* 				3 = seasons
* 				4 = age groups
* 				5 = genders
* 				6 = locations
* 				7 = item counts 				 
* 
*/
std::vector<std::vector<std::string>> get_agent_sales_arrays(mongocxx::collection agent_sales, int64_t id);