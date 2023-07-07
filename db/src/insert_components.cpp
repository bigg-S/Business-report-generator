
#include "insert_components.h"
#include "manage_components_arrays.h"
#include "new_docs.h"

bsoncxx::array::view _products;
bsoncxx::array::view _brands;
bsoncxx::array::view _brand_name;
bsoncxx::array::view _quantities;
bsoncxx::array::view _seasons;
bsoncxx::array::view _age_group;
bsoncxx::array::view _genders;
bsoncxx::array::view _locations;

std::string _path;

int insert_location(mongocxx::collection products, std::string location)
{
	insert_item_into_array(products, "Locations", "Components", location);

	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(document{} << "name" << "Components" << finalize);
	bsoncxx::document::view mview = my_doc.value().view();
	_products = mview["Products"].get_array();
	for (const bsoncxx::array::element& p : _products)
	{
		_brands = mview["Brands"][p.get_string().value.to_string()].get_array();
		for (const bsoncxx::array::element& b : _brands)
		{
			_quantities = mview["Quantities"][b.get_string().value.to_string()].get_array();
			for (const bsoncxx::array::element& q : _quantities)
			{
				_seasons = mview["Seasons"].get_array();
				for (const bsoncxx::array::element& s : _seasons)
				{
					_age_group = mview["Age Groups"].get_array();
					for (const bsoncxx::array::element& a : _age_group)
					{
						_genders = mview["Genders"].get_array();
						for (const bsoncxx::array::element& g : _genders)
						{
							_path = "Brands." + b.get_string().value.to_string() + "." +
								q.get_string().value.to_string() + "." +
								s.get_string().value.to_string() + "." +
								a.get_string().value.to_string() + "." +
								g.get_string().value.to_string() + "." + location;
   							products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));
						}
					}
				}
			}
		}
	}
	return 1;
}

int insert_gender(mongocxx::collection products, std::string gender)
{
	insert_item_into_array(products, "Genders", "Components", gender);
	bsoncxx::document::value filter = document{} << "name" << "Components" << finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(filter.view());
	bsoncxx::document::view mview = my_doc.value().view();
	_products = mview["Products"].get_array();
	for (const bsoncxx::array::element& p : _products)
	{
		_brands = mview["Brands"][p.get_string().value.to_string()].get_array();
		for (const bsoncxx::array::element& b : _brands)
		{
			_quantities = mview["Quantities"][b.get_string().value.to_string()].get_array();
			for (const bsoncxx::array::element& q : _quantities)
			{
				_seasons = mview["Seasons"].get_array();
				for (const bsoncxx::array::element& s : _seasons)
				{
					_age_group = mview["Age Groups"].get_array();
					for (const bsoncxx::array::element& a : _age_group)
					{
						_path = "Brands." + b.get_string().value.to_string() + "." +
							q.get_string().value.to_string() + "." +
							s.get_string().value.to_string() + "." +
							a.get_string().value.to_string() + "." + gender + "." + "Total";
						products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));
						_locations = mview["Locations"].get_array();
						for (const bsoncxx::array::element& l : _locations)
						{
							_path = "Brands." + b.get_string().value.to_string() + "." +
								q.get_string().value.to_string() + "." +
								s.get_string().value.to_string() + "." +
								a.get_string().value.to_string() + "." + gender + "." + l.get_string().value.to_string();
							products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

						}
						
					}
				}
			}
		}
	}
	return 1;
}

int insert_age_group(mongocxx::collection products, std::string age_group)
{
	insert_item_into_array(products, "Age Groups", "Components", age_group);
	bsoncxx::document::value filter = document{} << "name" << "Components" << finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(filter.view());
	bsoncxx::document::view mview = my_doc.value().view();
	_products = mview["Products"].get_array();
	for (const bsoncxx::array::element& p : _products)
	{
		_brands = mview["Brands"][p.get_string().value.to_string()].get_array();
		for (const bsoncxx::array::element& b : _brands)
		{
			
			_quantities = mview["Quantities"][b.get_string().value.to_string()].get_array();
			for (const bsoncxx::array::element& q : _quantities)
			{
				_seasons = mview["Seasons"].get_array();
				for (const bsoncxx::array::element& s : _seasons)
				{
					_path = "Brands." + b.get_string().value.to_string() + "." +
						q.get_string().value.to_string() + "." +
						s.get_string().value.to_string() + "." + age_group + "." + "Total";
					products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

					_genders = mview["Genders"].get_array();
					for (const bsoncxx::array::element& g : _genders)
					{
						_path = "Brands." + b.get_string().value.to_string() + "." +
							q.get_string().value.to_string() + "." +
							s.get_string().value.to_string() + "." + age_group + "." + g.get_string().value.to_string() + "." + "Total";
						products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

						_locations = mview["Locations"].get_array();
						for (const bsoncxx::array::element& l : _locations)
						{
							_path = "Brands." + b.get_string().value.to_string() + "." +
								q.get_string().value.to_string() + "." +
								s.get_string().value.to_string() + "." + age_group + "." + g.get_string().value.to_string() + "." + l.get_string().value.to_string();
							products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

						}
					}
				}
			}
		}
	}
	return 1;
}

int insert_season(mongocxx::collection products, std::string my_season)
{
	insert_item_into_array(products, "Seasons", "Components", my_season);
	bsoncxx::document::value filter = document{} << "name" << "Components" << finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(filter.view());
	bsoncxx::document::view mview = my_doc.value().view();
	_products = mview["Products"].get_array();
	for (const bsoncxx::array::element& p : _products)
	{
		
		_brands = mview["Brands"][p.get_string().value.to_string()].get_array();
		for (const bsoncxx::array::element& b : _brands)
		{
			
			_quantities = mview["Quantities"][b.get_string().value.to_string()].get_array();
			for (const bsoncxx::array::element& q : _quantities)
			{	
				
				_path = "Brands." + b.get_string().value.to_string() + "." + q.get_string().value.to_string() + "." + my_season +  "." + "Total";
				products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

				_age_group = mview["Age Groups"].get_array();
				for (const bsoncxx::array::element& a : _age_group)
				{
					_path = "Brands." + b.get_string().value.to_string() + "." + q.get_string().value.to_string() + "." + my_season + "." + a.get_string().value.to_string() +  "." + "Total";
					products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

					_genders = mview["Genders"].get_array();
					for (const bsoncxx::array::element& g : _genders)
					{
						_path = "Brands." + b.get_string().value.to_string() + "." + q.get_string().value.to_string() + "." + my_season + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + "Total";
						products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));

						_locations = mview["Locations"].get_array();
						for (const bsoncxx::array::element& l : _locations)
						{
							_path = "Brands." + b.get_string().value.to_string() + "." +
								q.get_string().value.to_string() + "." + my_season + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + l.get_string().value.to_string();
							products.update_one(make_document(kvp("name", p.get_string().value.to_string())), make_document(kvp("$set", make_document(kvp(_path, 0)))));
						}
					}
				}
				
			}
		}
	}
	return 1;
}

int insert_quantity(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name, std::string quantity, double selling_price, double buying_price, int num_of_items)
{
	int status = insert_quantity_into_components(products, stock, product_name, brand_name, quantity, selling_price, buying_price, num_of_items);

	if (status == 0)
		return 0;
	bsoncxx::document::value filter = document{} << "name" << "Components" << finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(filter.view());
	bsoncxx::document::view mview = my_doc.value().view();
	
	_path = "Brands." + product_name + "_" + brand_name + "." + quantity + "." + "Total";
	products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

	_seasons = mview["Seasons"].get_array();
	for (const bsoncxx::array::element& s : _seasons)
	{
		_path = "Brands." + product_name + "_" + brand_name + "." + quantity + "." + s.get_string().value.to_string() + "." + "Total";
		products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

		_age_group = mview["Age Groups"].get_array();
		for (const bsoncxx::array::element& a : _age_group)
		{
			_path = "Brands." + product_name + "_" + brand_name + "." + quantity + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "."  + "Total";
			products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

			_genders = mview["Genders"].get_array();
			for (const bsoncxx::array::element& g : _genders)
			{
				_path = "Brands." + product_name + "_" + brand_name + "." + quantity + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + "Total";
				products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

				_locations = mview["Locations"].get_array();
				for (const bsoncxx::array::element& l : _locations)
				{
					_path = "Brands." + product_name + "_" + brand_name + "." + quantity + "." +
						s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + l.get_string().value.to_string();
					products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));
				}
			}
		}
	}
	return 1;
			
}

int insert_brand(mongocxx::collection products, mongocxx::collection stock, std::string brand, std::vector<std::string> quantities, std::vector<double> selling_price, std::vector<double> buying_price, std::vector<int> number_of_items,  std::string product_name)
{
	int status = insert_brand_into_components(products, stock, brand, quantities, selling_price, buying_price, number_of_items, product_name);
	if (status == 0)
		return 0;

	brand = product_name + "_" + brand;
	bsoncxx::document::value filter = document{} << "name" << "Components" << finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(filter.view());
	bsoncxx::document::view mview = my_doc.value().view();

	_path = "Brands." + brand + "." + "Total";
	products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

	for (std::string q : quantities)
	{
		_path = "Brands." + brand + "." + q + "." + "Total";
		products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

		_seasons = mview["Seasons"].get_array();
		for (const bsoncxx::array::element& s : _seasons)
		{
			_path = "Brands." + brand + "." + q + "." + s.get_string().value.to_string() + "." + "Total";
			products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

			_age_group = mview["Age Groups"].get_array();
			for (const bsoncxx::array::element& a : _age_group)
			{
				_path = "Brands." + brand + "." + q + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + "Total";
				products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

				_genders = mview["Genders"].get_array();
				for (const bsoncxx::array::element& g : _genders)
				{
					_path = "Brands." + brand + "." + q + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + "Total";
					products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

					_locations = mview["Locations"].get_array();
					for (const bsoncxx::array::element& l : _locations)
					{
						_path = "Brands." + brand + "." + q + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." + g.get_string().value.to_string() + "." + l.get_string().value.to_string();
						products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));
					}
				}
			}
		}
	}
	return 1;
}


int insert_product(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::vector<std::string> brand_names, std::vector<std::vector<std::string>> qty_names, std::vector<std::vector<double>> selling_price, std::vector<std::vector<double>> buying_price, std::vector<std::vector<int>> number_of_items)
{
	create_new_product_doc(products, product_name);
	create_new_product_doc(stock, product_name);

	insert_product_into_components(products, stock, product_name, brand_names, qty_names, selling_price, buying_price, number_of_items);

	bsoncxx::stdx::optional<bsoncxx::document::value> my_doc = products.find_one(document{} << "name" << "Components" << finalize);
	bsoncxx::document::view mview = my_doc.value().view();
	
	_path = "Brands.Total";
	products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

	for (std::string b : brand_names)
	{		
		_path = "Brands." + b + "."  + "Total";
		products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

		for (std::vector<std::string> q : qty_names)
		{
			for(std::string qty : q)
			{
				_path = "Brands." + b + "." + qty + "." + "Total";
				products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

				_seasons = mview["Seasons"].get_array();
				
				for (const bsoncxx::array::element& s : _seasons)
				{
					_path = "Brands." + b + "." + qty + "." + s.get_string().value.to_string() + "." + "Total";
					products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

					_age_group = mview["Age Groups"].get_array();
					for (const bsoncxx::array::element& a : _age_group)
					{
						_path = "Brands." + b + "." + qty + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "."  + "Total";
						products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

						_genders = mview["Genders"].get_array();
						for (const bsoncxx::array::element& g : _genders)
						{
							_path = "Brands." + b + "." + qty + "." + s.get_string().value.to_string() + "." + a.get_string().value.to_string() + "." +	g.get_string().value.to_string() + "." + "Total";
							products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));

							_locations = mview["Locations"].get_array();
							for (const bsoncxx::array::element& l : _locations)
							{
								_path = "Brands." + b + "." +
									qty + "." +
									s.get_string().value.to_string() + "." +
									a.get_string().value.to_string() + "." +
									g.get_string().value.to_string() + "." + l.get_string().value.to_string();
								products.update_one(make_document(kvp("name", product_name)), make_document(kvp("$set", make_document(kvp(_path, 0)))));
							}
								
						}
					}
				}
			}
		}
	}
	return 1;
	
}
