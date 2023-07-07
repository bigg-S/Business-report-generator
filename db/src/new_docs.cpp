

#include "new_docs.h"

void create_stock_buying_price_doc(mongocxx::collection stock_products)
{
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "name" << "BuyingPrice"

		<< "Total" << 0.0

		<< bsoncxx::builder::stream::finalize;

	bsoncxx::document::view view = doc_value.view();

	bsoncxx::stdx::optional<mongocxx::result::insert_one> result = stock_products.insert_one(view);
};


void create_agent_doc(mongocxx::collection agents)
{
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "id" << 0

		<< "name" << ""

		<< "phone" << ""

		<< "location" << ""

		<< "points" << 0

		<< bsoncxx::builder::stream::finalize;

	bsoncxx::document::view mview = doc_value.view();	

 bsoncxx::stdx::optional<mongocxx::result::insert_one> result = agents.insert_one(mview);

};


void create_agent_sale_doc(mongocxx::collection agent_sales)
{
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "id" << 0
		<< "indices"
			<<bsoncxx::builder::stream::open_array
			<< bsoncxx::builder::stream::close_array
		<< "current_index" << 0
		
		<< bsoncxx::builder::stream::finalize;

	bsoncxx::document::view mview = doc_value.view();
	bsoncxx::stdx::optional<mongocxx::result::insert_one> result = agent_sales.insert_one(mview);
}



void create_new_product_doc(mongocxx::collection products, std::string product_name)
{
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "name" << product_name		

		<< bsoncxx::builder::stream::finalize; //used to obtain a bsoncxx::document::value instance (read only owning its own memory) and to use it you must obtain a bsoncxx::document::view using the view method

	bsoncxx::document::view view = doc_value.view();

	bsoncxx::stdx::optional<mongocxx::result::insert_one> result = products.insert_one(view);
}