#pragma once


/*
* Handles the retrieval of data from the database
*/

#include "app_basic.h"

/*Get a location data*/
int get_location_data(mongocxx::collection products, SALE sale);

/*Get the gender total*/
int get_gender_total(mongocxx::collection products, SALE sale);

/*Get the age group total*/
int get_age_group_total(mongocxx::collection products, SALE sale);


/*Get the season total*/
int get_season_total(mongocxx::collection products, SALE sale);

/*Get the quantity total*/
int get_quantity_total(mongocxx::collection products, SALE sale);


/*Get the quantity unit cost*/
/*
*price_type: "BuyingPrice" or "SellingPrice"
*/
double get_quantity_unit_cost(mongocxx::collection products, SALE sale, std::string price_type);

/*Get the brand total*/
int get_brand_total(mongocxx::collection products, SALE sale);

/*Get the product total*/
int get_product_total(mongocxx::collection products, SALE sale);



/*Get a specific operational cost*/
double get_operational_costs_data(mongocxx::collection operational_costs, std::string operation_description);

/*Get the operational costs total*/
double get_operational_costs_total(mongocxx::collection operational_costs);

double get_expected_expenditure(mongocxx::collection operational_costs);

/*RETRIEVE DATA CONCERNING A CERTAIN ENTITY eg. genders*/

/*Get the all common entities(Locations, genders, etc) for a particular product and brand details
 *
 *Returns a string containing the details in the format,  "location0:data0,location1:data1,"
 */
std::string get_all_common_entity_totals(mongocxx::collection products, SALE sale, std::string entity);


/*Get all the quantities totals
 *
 *Data is in the format, "Kasuku_2Litres:30,Omo_10g:10"
 */
std::string get_all_quantites_totals(mongocxx::collection products, SALE sale);

/*Get all the brands totals
 *
 *Data is in the format, "Kasuku:30,Omo:10"
 */
std::string get_all_brands_totals(mongocxx::collection products, SALE sale);

/*Get all the products totals
 *
 *Data is in the format, "Cooking Oil:30,Bathing Soap:10"
 */
std::string get_all_products_totals(mongocxx::collection products, SALE sale);

/*Get all the operational costs totals details
 *
 *Data is in the format, "Electricity:2000.00,Transport:5000.00"
 */
std::string get_all_operational_costs_totals(mongocxx::collection operational_costs);

std::vector<std::string> get_products_array(mongocxx::collection products);

std::vector<std::string> get_product_brands_array(mongocxx::collection products, std::string product_name);

std::vector<std::string> get_brand_quantities_array(mongocxx::collection products, std::string brand_name);

std::vector<int> get_brand_amounts_array(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name);


std::vector<std::string> get_seasons_array(mongocxx::collection products);

std::vector<std::string> get_age_groups_array(mongocxx::collection products);

std::vector<std::string> get_genders_array(mongocxx::collection products);

std::vector<std::string> get_locations_array(mongocxx::collection products);

std::vector<std::string> get_expenses_array(mongocxx::collection operational_costs);

std::vector<std::vector<std::string>> get_sales_data(mongocxx::collection products);

//For the report generation

struct QuantityData
{
	std::string quantity_name = "";
	int quantity_total = 0;
};

struct BrandData
{
	std::string brand_name = "";
	int brand_total = 0;

	std::vector<QuantityData> b_quantities;
};

struct ProductData
{
	std::string product_name = "";
	int product_total = 0;

	std::vector<BrandData> p_brands;
};

struct CategoryData
{
	std::string category_name = "";
	int category_total = 0;

	std::vector<ProductData> p_products;
};

//retrieving data for report generation
std::vector<CategoryData> get_seasons_data(mongocxx::collection products);

std::vector<CategoryData> get_age_groups_data(mongocxx::collection products);
			
std::vector<CategoryData> get_genders_data(mongocxx::collection products);
			
std::vector<CategoryData> get_locations_data(mongocxx::collection products);

std::vector<ProductData> get_products_sales_data(mongocxx::collection products);


struct UserData
{
	std::string level = "";
	std::string password = "";
};

//loging details in the order 0 admin, 1 finance, 2 marketer, 3 regular
std::vector<UserData> get_users_data(mongocxx::collection login);

int set_password(mongocxx::collection login, std::string user, std::string new_password);