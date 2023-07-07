

#include "agent_sales.h"
#include "new_docs.h"

int create_new_agent_sale_doc(mongocxx::collection agent_sales, int64_t id)
{
	create_agent_sale_doc(agent_sales);

	agent_sales.update_one(make_document(kvp("id", 0)), make_document(kvp("$set", make_document(kvp("id", id)))));

	return 1;
}

int delete_new_agent_sale_doc(mongocxx::collection agent_sales, int64_t id)
{
	agent_sales.delete_one(make_document(kvp("id", id)));

	return 1;
}

int add_agent_sale(mongocxx::collection agent_sales, int64_t id, SALE sale)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> agent_sale_data = agent_sales.find_one(document{} << "id" << id << finalize);
	bsoncxx::document::view agent_sale_data_view = agent_sale_data.value().view();

	int current_sale_index = agent_sale_data_view["current_index"].get_int32();

	std::string path = std::to_string(current_sale_index);

	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Product", sale.product_name)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Brand", sale.brand)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Quantity", sale.quantity)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Season", sale.season)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Age Group", sale.age_group)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Gender", sale.gender)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Location", sale.location)))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(path + "." + "Item Count", sale.item_count)))));

	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp("current_index", current_sale_index + 1)))));

	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$push", make_document(kvp("indices", std::to_string(current_sale_index))))));

	return 1;
}	


int delete_agent_sale(mongocxx::collection agent_sales, int64_t id, int sale_index)
{
	
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$unset", make_document(kvp(std::to_string(sale_index), "")))));
	agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$pull", make_document(kvp("indices", std::to_string(sale_index))))));

	/*----------------------Reset current sale index to 0 if there are no sales in the document------------------------------------*/

	bsoncxx::stdx::optional<bsoncxx::document::value> agent_sale_data = agent_sales.find_one(document{} << "id" << id << finalize);
	bsoncxx::document::view agent_sale_data_view = agent_sale_data.value().view();

	bsoncxx::array::view indices = agent_sale_data_view["indices"].get_array();

	if (indices.empty())
		agent_sales.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp("current_index", 0)))));
	
	/*------------------------------------------------------------------------------------------------------------------------------*/

	return 1;
}


std::vector<std::vector<std::string>> get_agent_sales_arrays(mongocxx::collection agent_sales, int64_t id)
{
	std::vector<std::vector<std::string>> details;
	std::vector<std::string> data = {};
	for (int count = 0; count < 9; count++)
		details.push_back(data);

	bsoncxx::stdx::optional<bsoncxx::document::value> agent_sale_data = agent_sales.find_one(document{} << "id" << id << finalize);
	bsoncxx::document::view agent_sale_data_view = agent_sale_data.value().view();

	bsoncxx::array::view indices = agent_sale_data_view["indices"].get_array();
	int item_count = 0;
	for (bsoncxx::array::element index : indices)
	{
		details[0].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Product"].get_string().value.to_string());
		details[1].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Brand"].get_string().value.to_string());
		details[2].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Quantity"].get_string().value.to_string());
		details[3].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Season"].get_string().value.to_string());
		details[4].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Age Group"].get_string().value.to_string());
		details[5].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Gender"].get_string().value.to_string());
		details[6].push_back(agent_sale_data_view[index.get_string().value.to_string()]["Location"].get_string().value.to_string());

		item_count = agent_sale_data_view[index.get_string().value.to_string()]["Item Count"].get_int32();
		details[7].push_back(std::to_string(item_count));

		details[8].push_back(index.get_string().value.to_string());

	}

	return details;
}
