#pragma once

/*
* Handles the removal of a component(eg brand, quantity, season) from the database
*/

#include "app_basic.h"

/*Delete a location*/
int delete_location(mongocxx::collection products, std::string location);

/*Delete a age group*/
int delete_age_group(mongocxx::collection products, std::string age_group);

/*Delete a season*/
int delete_season(mongocxx::collection products, std::string season);

/*Delete a brand*/
int delete_brand(mongocxx::collection products, mongocxx::collection stock, std::string mproduct, std::string brand);

/*Delete a quantity*/
int delete_quantity(mongocxx::collection products, mongocxx::collection stock, std::string mproduct, std::string brand, std::string quantity);

/*Delete a product*/
int delete_product(mongocxx::collection products, mongocxx::collection stock, std::string mproduct);
