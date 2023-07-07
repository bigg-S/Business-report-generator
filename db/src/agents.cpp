
#include <time.h>
#include <math.h>

#include "agents.h"
#include "new_docs.h"
#include "manage_components_arrays.h"
#include "agent_sales.h"

long cretate_new_agent_id()
{
	int max = 9, min = 0;

	unsigned seed = (unsigned)time(0);

	int digit = 0;
	long id = 0;

	
	for (int count = 0; count <= 5; count++)
	{
		srand(seed * (count + 1));

		digit = (rand() % (max - min + 1)) + min;

		id += (long)pow(10, (5 - count)) * digit;
	}

	return id;
	
}

std::string add_new_agent(mongocxx::collection agents, mongocxx::collection agent_sales, AGENT agent)
{
	
	//create an agent id
	int64_t new_agent_id = cretate_new_agent_id();

	/*-------------------------------------------Check for id duplicate------------------------------------------------*/

	bsoncxx::stdx::optional<bsoncxx::document::value> agents_data = agents.find_one(document{} << "name" << "AgentsData" << finalize);
	bsoncxx::document::view agents_data_view = agents_data.value().view();

	bsoncxx::array::view ids = agents_data_view["IDs"].get_array();

	std::string s_id = std::to_string(new_agent_id);

	for (bsoncxx::array::element id : ids)
	{
		if (s_id.compare(id.get_string().value.to_string()) == 0)
			return 0;//Id already exists
		
	}

	/*---------------------------------------------------------------------------------------------------------------*/
	
	/*------------------------------------------Update the agents data-----------------------------------------------*/
	if (s_id.size() == 5) //if the firt digit was a 0, please add it at the beginning
		s_id = "0" + s_id;
	insert_item_into_array(agents,"IDs", "AgentsData", s_id);

	insert_item_into_array(agents, "Names", "AgentsData", s_id + "_" + agent.agent_name);
	/*---------------------------------------------------------------------------------------------------------------*/

	//Create the template document for an agent
	create_agent_doc(agents);

	//Save the agent details
	agents.update_one(make_document(kvp("id", 0)), make_document(kvp("$set", make_document(kvp("id", new_agent_id)))));
	agents.update_one(make_document(kvp("id", new_agent_id)), make_document(kvp("$set", make_document(kvp("name", agent.agent_name)))));
	agents.update_one(make_document(kvp("id", new_agent_id)), make_document(kvp("$set", make_document(kvp("phone", agent.phone)))));
	agents.update_one(make_document(kvp("id", new_agent_id)), make_document(kvp("$set", make_document(kvp("location", agent.agent_location)))));

	//Create the agent's sale document
	create_new_agent_sale_doc(agent_sales, new_agent_id);
	
	return s_id;

}

int remove_agent(mongocxx::collection agents, mongocxx::collection agent_sales, std::string agent_name, int64_t id)
{	
	
	delete_item_from_array(agents, "IDs", "AgentsData", std::to_string(id));

	std::string id_name = std::to_string(id) + "_" + agent_name;
	delete_item_from_array(agents, "Names", "AgentsData", id_name);

	agents.delete_one(make_document(kvp("id", id)));

	delete_new_agent_sale_doc(agent_sales, id);

	return 1;
}



int update_agent_data(mongocxx::collection agents, int64_t id, std::string item, int int_value)
{	
	agents.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(item, int_value)))));

	return 1;
}

int update_agent_data(mongocxx::collection agents, int64_t id, std::string item, std::string str_value)
{
	if (item.compare("name") == 0)
	{
		bsoncxx::stdx::optional<bsoncxx::document::value> agent = agents.find_one(document{} << "id" << id << finalize);
		bsoncxx::document::view agent_view = agent.value().view();

		std::string previous_name = std::to_string(id) + "_" + agent_view["name"].get_string().value.to_string();

		std::string new_name = std::to_string(id) + "_" + str_value;

		agents.update_one(make_document(kvp("name", "AgentsData")), make_document(kvp("$pull", make_document(kvp("Names", previous_name)))));
		agents.update_one(make_document(kvp("name", "AgentsData")), make_document(kvp("$push", make_document(kvp("Names", new_name)))));

		agents.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(item, str_value)))));
	}
	else
		agents.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp(item, str_value)))));


	return 1;
}


int  retrieve_agent_points(mongocxx::collection agents, int64_t id)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> agent = agents.find_one(document{} << "id" << id << finalize);
	bsoncxx::document::view agent_view = agent.value().view();

	int points = agent_view["points"].get_int32();

	return points;
}


std::string retieve_agents_data(mongocxx::collection agents)
{
	std::string details = "";
	
	bsoncxx::stdx::optional<bsoncxx::document::value> agent_s = agents.find_one(document{} << "name" << "AgentsData" << finalize);
	bsoncxx::document::view agent_s_view = agent_s.value().view();

	bsoncxx::array::view names = agent_s_view["Names"].get_array();

	for (bsoncxx::array::element name : names)
		details += name.get_string().value.to_string() + ",";
	
	return details;
}


std::vector<std::vector<std::string>> get_agent_details(mongocxx::collection agents)
{
	std::vector<std::vector<std::string>> details;
	std::vector<std::string> detail_vec = {};
	details.push_back(detail_vec);
	details.push_back(detail_vec);
	details.push_back(detail_vec);
	details.push_back(detail_vec);

	bsoncxx::stdx::optional<bsoncxx::document::value> agents_data = agents.find_one(document{} << "name" << "AgentsData" << finalize);
	bsoncxx::document::view agents_data_view = agents_data.value().view();

	bsoncxx::array::view ids = agents_data_view["IDs"].get_array();

	bsoncxx::stdx::optional<bsoncxx::document::value> agent_data;
	bsoncxx::document::view agent_data_view;

	std::string data;

	for (bsoncxx::array::element id : ids)
	{
		agent_data = agents.find_one(document{} << "id" << std::stoi(id.get_string().value.to_string()) << finalize);
		agent_data_view = agent_data.value().view();

		details[0].push_back(id.get_string().value.to_string());

		data = agent_data_view["name"].get_string().value.to_string();
		details[1].push_back(data);

		data = agent_data_view["phone"].get_string().value.to_string();
		details[2].push_back(data);

		data = agent_data_view["location"].get_string().value.to_string();
		details[3].push_back(data);
	}

	return details;
}