
#include "compute_costs.h"
#include "retrieve_data.h"


bsoncxx::array::view mbrands;
bsoncxx::array::view mquantities;
bsoncxx::array::view mproducts_array;

bsoncxx::stdx::optional<bsoncxx::document::value> get_components_arrays(mongocxx::collection products)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays = products.find_one(document{} << "name" << "Components" << finalize);

	return components_arrays;
}

double compute_total_quantity_cost(mongocxx::collection products, SALE sale, std::string price_type)
{
	double total_quantity_cost = 0.0;

	int quantity_count = get_quantity_total(products, sale);
	double quantity_unit_cost = get_quantity_unit_cost(products, sale, price_type);

	total_quantity_cost = quantity_count * quantity_unit_cost;

	return total_quantity_cost;

}

double compute_total_cost(mongocxx::collection products, bsoncxx::stdx::optional<bsoncxx::document::value> components_arrays, SALE sale, std::string price_type)
{
	double total_sales_costs = 0.0;
		
	bsoncxx::document::view components_arrays_view = components_arrays.value().view();
	
	mproducts_array = components_arrays_view["Products"].get_array();
	for (bsoncxx::array::element my_product : mproducts_array)
	{
		mbrands = components_arrays_view["Brands"][my_product.get_string().value.to_string()].get_array();

		for (bsoncxx::array::element my_brand : mbrands)
		{
			mquantities = components_arrays_view["Quantities"][my_brand.get_string().value.to_string()].get_array();

			for (bsoncxx::array::element my_quantity : mquantities)
			{
				sale.product_name = my_product.get_string().value.to_string();
				sale.brand        = my_brand.get_string().value.to_string();
				sale.quantity     = my_quantity.get_string().value.to_string();

				total_sales_costs += compute_total_quantity_cost(products, sale, price_type);
				
			}			
		}		
	}

	return total_sales_costs;
}








