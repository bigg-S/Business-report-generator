

#include "manage_components_arrays.h"
#include "manage_buying_and_selling_price.h"
#include "delete_from_stock.h"
#include "update_stock.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DELETION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int delete_item_from_array(mongocxx::collection products,std::string array_name, std::string componets_doc_name, std::string component_name)
{
	products.update_one(make_document(kvp("name", componets_doc_name)), make_document(kvp("$pull", make_document(kvp(array_name, component_name)))));

	return 1;
}

int delete_product_from_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> components_arrrays = products.find_one(document{} << "name" << "Components" << finalize);
	bsoncxx::document::view components_arrrays_view = components_arrrays.value().view();

	/*--------------------------------Delete from products Array--------------------------*/
	bsoncxx::array::view my_products = components_arrrays_view["Products"].get_array();
	bool exists = false;
	for (bsoncxx::array::element my_product : my_products)
	{
		if (product_name.compare(my_product.get_string().value.to_string()) == 0)
		{
			exists = true;
			break;
		}
	}

	if (!exists)
		return 1;//Product does not exist, return

	delete_item_from_array(products, "Products", "Components", product_name);

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Update the quantities--------------------------*/	

	bsoncxx::array::view brands = components_arrrays_view["Brands"][product_name].get_array();
	std::string nav_path;
	for (bsoncxx::array::element my_brand : brands)
	{
		/*Update the total stock buying price*/
		update_Tbuying_price_during_brand_removal(products, stock, product_name, my_brand.get_string().value.to_string());

		nav_path = "Quantities." + my_brand.get_string().value.to_string();
		products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));
	}

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Update the brands--------------------------*/
	
	nav_path = "Brands." + product_name;
	products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$unset", make_document(kvp(nav_path  , "")))));
	
	/*------------------------------------------------------------------------------------*/

	/*---------------------------------Update the Stock------------------------------*/

	delete_product_from_stock(stock, product_name);

	/*------------------------------------------------------------------------------------*/

	return 1;
}

int delete_brand_from_components(mongocxx::collection products, mongocxx::collection stock, std::string brand_name, std::string product_name)
{
	/*Update the total stock buying price*/
	update_Tbuying_price_during_brand_removal(products, stock, product_name, brand_name);

	/*--------------------------------Update the brands--------------------------*/

	std::string nav_path = "Brands." + product_name;
	delete_item_from_array(products, nav_path, "Components", brand_name);

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Update the quantities--------------------------*/

	nav_path = "Quantities." + brand_name;
	products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));
	

	/*------------------------------------------------------------------------------------*/

	/*---------------------------------Update the Stock------------------------------*/

	delete_brand_from_stock(stock, product_name, brand_name);

	/*------------------------------------------------------------------------------------*/

	return 1;
}

int delete_quantity_from_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name)
{	
	/*Update the total stock buying price*/
	update_Tbuying_price_during_quantity_removal(stock, product_name, brand_name, quantity_name);

	std::string nav_path = "Quantities." + brand_name;
	delete_item_from_array(products, nav_path, "Components", quantity_name);

	/*---------------------------------Update the Stock------------------------------*/
	delete_quantity_from_stock(stock, product_name, brand_name, quantity_name);
	/*------------------------------------------------------------------------------------*/

	return 1;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INSERTION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int insert_item_into_array(mongocxx::collection products, std::string array_name, std::string componets_doc_name, std::string component_name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> components_arrrays = products.find_one(document{} << "name" << componets_doc_name.c_str() << finalize);
	bsoncxx::document::view components_arrrays_view = components_arrrays.value().view();
	bsoncxx::array::view my_items;

	int end_of_subdocument_name = (int)array_name.find_first_of(".");

	if (end_of_subdocument_name != -1) //array_name has two parts
	{
		std::string subdocument = array_name.substr(0, end_of_subdocument_name);
		std::string name_of_array = array_name.substr(end_of_subdocument_name + 1, array_name.size() - 1);

		my_items = components_arrrays_view[subdocument][name_of_array].get_array();
	}
	else
		my_items = components_arrrays_view[array_name].get_array();
	
	bool exists = false;
	for (bsoncxx::array::element my_item : my_items)
	{
		if (component_name.compare(my_item.get_string().value.to_string()) == 0)
		{
			exists = true;
			break;
		}
	}

	if (exists)
		return 0;//item exists, return

	products.update_one(make_document(kvp("name", componets_doc_name)), make_document(kvp("$push", make_document(kvp(array_name, component_name)))));

	return 1;
}


int insert_product_into_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::vector<std::string> mbrands, std::vector<std::vector<std::string>> myquantities, std::vector<std::vector<double>> my_SelingPrices, std::vector<std::vector<double>> my_BuyingPrices, std::vector<std::vector<int>> my_numbers_of_items)
{
	std::string nav_path;

	/*--------------------------------Insert into products Array--------------------------*/

	int status = insert_item_into_array(products, "Products", "Components", product_name);

	if (status == 0)
		return 1;//Product already exists

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Update the brands--------------------------*/

	nav_path = "Brands." + product_name;

	auto mbrands_array = bsoncxx::builder::stream::array();		
	for (auto mbrand : mbrands)
			mbrands_array << mbrand;
				
	products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$set", make_document(kvp(nav_path, mbrands_array)))));

	/*------------------------------------------------------------------------------------*/


	/*--------------------------------Update the quantities--------------------------*/

	int brand_index = 0;
	for (std::string my_brand : mbrands)
	{
		auto my_quantities_array = bsoncxx::builder::stream::array();
		for (auto my_quantity : myquantities[brand_index])
			my_quantities_array << my_quantity;

		nav_path = "Quantities." + my_brand;
		products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$set", make_document(kvp(nav_path, my_quantities_array)))));

		brand_index += 0;
	}

	/*------------------------------------------------------------------------------------*/


	/*--------------------------------Set the selling prices-------------------------------*/

	set_product_brands_quantiities_price(products, product_name, mbrands, myquantities, my_SelingPrices, "SellingPrice", my_numbers_of_items);

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Set the buying prices -------------------------------*/

	set_product_brands_quantiities_price(stock, product_name, mbrands, myquantities, my_BuyingPrices, "BuyingPrice", my_numbers_of_items);

	/*------------------------------------------------------------------------------------*/

	return 1;
}


int insert_brand_into_components(mongocxx::collection products, mongocxx::collection stock, std::string brand_name, std::vector<std::string> mquantities, std::vector<double> my_SelingPrices, std::vector<double> my_BuyingPrices, std::vector<int> my_numbers_of_items, std::string product_name)
{
	std::string nav_path = "Brands." + product_name;

	/*--------------------------------Check if the brand exists--------------------------*/

	bsoncxx::stdx::optional<bsoncxx::document::value> components_arrrays = products.find_one(document{} << "name" << "Components" << finalize);
	bsoncxx::document::view components_arrrays_view = components_arrrays.value().view();

	bsoncxx::array::view my_brands = components_arrrays_view["Brands"][product_name].get_array();
	bool exists = false;
	for (bsoncxx::array::element my_brand : my_brands)
	{
		if (brand_name.compare(my_brand.get_string().value.to_string()) == 0)
		{
			exists = true;
			break;
		}
	}

	if (exists)
		return 0;//Brand does exists, return

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Update the brands--------------------------*/

	products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$push", make_document(kvp(nav_path, brand_name)))));

	/*------------------------------------------------------------------------------------*/


	/*--------------------------------Update the quantities--------------------------*/	
	
	auto my_quantities_array = bsoncxx::builder::stream::array();
	for (auto my_quantity : mquantities)
		my_quantities_array << my_quantity;

	nav_path = "Quantities." + brand_name;
	products.update_one(make_document(kvp("name", "Components")), make_document(kvp("$set", make_document(kvp(nav_path, my_quantities_array)))));

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------set the selling prices-------------------------------*/

	set_brand_quantiities_price(products, product_name, brand_name, mquantities, my_SelingPrices, "SellingPrice", my_numbers_of_items);

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------set the buying prices-------------------------------*/

	set_brand_quantiities_price(stock, product_name, brand_name, mquantities, my_BuyingPrices, "BuyingPrice", my_numbers_of_items);

	/*------------------------------------------------------------------------------------*/
	
	return 1;
}


int insert_quantity_into_components(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity_name, double my_SelingPrice, double my_BuyingPrice, int my_number_of_items)
{
	brand_name = product_name + "_" + brand_name;

	std::string nav_path = "Quantities."  + brand_name;
	
	//Insert the quantity
	int status = insert_item_into_array(products, nav_path, "Components", quantity_name);

	if (status == 0)
		return 0;//Quantity already exists

	/*--------------------------------Set the selling price-------------------------------*/

	set_price(products, product_name, brand_name, quantity_name, my_SelingPrice, "SellingPrice", my_number_of_items);

	/*------------------------------------------------------------------------------------*/

	/*--------------------------------Set the buying-------------------------------*/

	set_price(stock, product_name, brand_name, quantity_name, my_BuyingPrice, "BuyingPrice", my_number_of_items);

	/*------------------------------------------------------------------------------------*/

	return 1;
}


