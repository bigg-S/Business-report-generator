

#include "delete_from_stock.h"


int delete_quantity_from_stock(mongocxx::collection stock, std::string mproduct, std::string brand, std::string quantity)
{
	std::string nav_path = "Brands." + brand + "." + quantity;
	stock.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));

	return 1;
}


int delete_brand_from_stock(mongocxx::collection stock, std::string mproduct, std::string brand)
{
	std::string nav_path = "Brands." + brand;
	stock.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));

	return 1;
}


int delete_product_from_stock(mongocxx::collection stock, std::string mproduct)
{
	stock.delete_one(make_document(kvp("name", mproduct)));

	return 1;
}