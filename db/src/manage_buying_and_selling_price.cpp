

#include "manage_buying_and_selling_price.h"
#include "retrieve_data.h"
#include "update_stock.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BUYING and SELLING PRICE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int set_price(mongocxx::collection products, std::string mproduct, std::string brand, std::string quantity, double price, std::string price_type, int number_of_items)
{
	std::string nav_path = "Brands." + brand + "." + quantity + "." + price_type;
	products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$set", make_document(kvp(nav_path, price)))));

	if (price_type.compare("BuyingPrice") == 0)
	{
		nav_path = "Brands." + brand + "." + quantity + "." + "Total";
		products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$set", make_document(kvp(nav_path, number_of_items)))));

		SALE sale;
		sale.product_name = mproduct;
		sale.brand = sale.product_name + "_" + brand;
		int current_total = get_brand_total(products, sale);
		current_total += number_of_items;

		nav_path = "Brands." + brand +  "." + "Total";
		products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$set", make_document(kvp(nav_path, current_total)))));

		current_total = get_product_total(products, sale);
		current_total += number_of_items;

		nav_path = "Brands.Total";
		products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$set", make_document(kvp(nav_path, current_total)))));

		double amount = price * (double)number_of_items;
		update_buying_price_total(products, amount, ADD);
	}
		
	
	return 1;
}


int set_brand_quantiities_price(mongocxx::collection products, std::string mproduct, std::string brand, std::vector<std::string> quantities, std::vector<double> prices, std::string price_type, std::vector<int> number_of_items)
{
	int quantity_index = 0;
	for (std::string my_quantity : quantities)
	{
		set_price(products, mproduct, brand, my_quantity, prices[quantity_index], price_type, number_of_items[quantity_index]);
		quantity_index += 1;
	}

	return 1;
}

int set_product_brands_quantiities_price(mongocxx::collection products, std::string mproduct, std::vector<std::string> brands, std::vector<std::vector<std::string>> quantities, std::vector<std::vector<double>> prices, std::string price_type, std::vector<std::vector<int>> number_of_items)
{
	int brand_index = 0;
	int quantity_index = 0;

	for (std::string my_brand : brands)
	{
		quantity_index = 0;
		for (std::string my_quantity : quantities[brand_index])
		{
			set_price(products, mproduct, my_brand, my_quantity, prices[brand_index][quantity_index], price_type, number_of_items[brand_index][quantity_index]);
			quantity_index += 1;
		}

		brand_index += 1;
	}

	return 1;
	
}


int change_price(mongocxx::collection products, std::string mproduct, std::string brand, std::string quantity, double price, std::string price_type)
{
	std::string nav_path = "Brands." + brand + "." + quantity + "." + price_type;
	products.update_one(make_document(kvp("name", mproduct)), make_document(kvp("$set", make_document(kvp(nav_path, price)))));

	return 1;
}




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


