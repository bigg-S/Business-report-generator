#pragma once

/*
* Handles the organisation's agents details, addition, removal etc.
*/

#include "app_basic.h"

struct AGENT
{
	long agent_id              = 0;
	std::string agent_name     = "";
	std::string agent_location = "";
	std::string phone          = "";
	int points = 0;
};

/*Creates an ID for a new agent*/
long cretate_new_agent_id();

/*Add a new agent*/
std::string add_new_agent(mongocxx::collection agents, mongocxx::collection agent_sales, AGENT agent);

/*Delete an agent*/
int remove_agent(mongocxx::collection agents, mongocxx::collection agent_sales, std::string agent_name, int64_t id);

/*Update agent data*/
//points
int update_agent_data(mongocxx::collection agents, int64_t id, std::string item, int int_value);
//other
int update_agent_data(mongocxx::collection agents, int64_t id, std::string item, std::string str_value);

/*Retrieve the agent's points*/
int  retrieve_agent_points(mongocxx::collection agents, int64_t id);

/*Get agents' details in the form (id_name)*/
std::string retieve_agents_data(mongocxx::collection agents);

/*Get agents' details as vectors with the vector at index 0 having ids, 1 names, 2 phones, 3 locations in that order*/
std::vector<std::vector<std::string>> get_agent_details(mongocxx::collection agents);