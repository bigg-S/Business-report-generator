
#include <time.h>

#include "manage_OperationalCosts.h"


int insert_expense(mongocxx::collection expenses, std::string expense_name, double expense_cost)
{
	std::string my_date = get_expense_date();
	
	if (my_date.compare("") == 0)
		return 0;//An error while accessing the date

	expense_name.append(" " + my_date.substr(0, my_date.size()-1));

	/*---------------------------------------------Update the total expenses cost---------------------------------------------------------------*/

	bsoncxx::stdx::optional<bsoncxx::document::value> expenses_data = expenses.find_one(document{} << "name" << "OperationalCosts" << finalize);
	bsoncxx::document::view expenses_data_view = expenses_data.value().view();

	bsoncxx::document::element current_cost = expenses_data_view["Costs"]["Total"];

	float current_total_cost = 0.0;
	if (current_cost.type() == bsoncxx::type::k_double)
		current_total_cost = (float)current_cost.get_double();
	else if (current_cost.type() == bsoncxx::type::k_int64)
		current_total_cost = (float)current_cost.get_int64();
	else if (current_cost.type() == bsoncxx::type::k_int32)
		current_total_cost = (float)current_cost.get_int32();
	 
	current_total_cost += (float)expense_cost;

	std::string nav_path = "Costs.Total";
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$set", make_document(kvp(nav_path, current_total_cost)))));

	/*------------------------------------------------------------------------------------------------------------------------------------------*/

	//Insert into the expenses list
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$push", make_document(kvp("Costs.List", expense_name)))));

	//Insert the expense and its cost
	nav_path = "Costs." + expense_name;
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$set", make_document(kvp(nav_path, expense_cost)))));


	return 1;
}


int delete_expense(mongocxx::collection expenses, std::string expense_name)
{
	/*---------------------------------------------Update the total expenses cost---------------------------------------------------------------*/

	bsoncxx::stdx::optional<bsoncxx::document::value> expenses_data = expenses.find_one(document{} << "name" << "OperationalCosts" << finalize);
	bsoncxx::document::view expenses_data_view = expenses_data.value().view();

	bsoncxx::document::element current_cost = expenses_data_view["Costs"]["Total"];
	bsoncxx::document::element expense_cost = expenses_data_view["Costs"][expense_name];

	double current_total_cost = 0.0;
	if (current_cost.type() == bsoncxx::type::k_double)
		current_total_cost = current_cost.get_double();
	else if (current_cost.type() == bsoncxx::type::k_int64)
		current_total_cost = (double)current_cost.get_int64();
	else if (current_cost.type() == bsoncxx::type::k_int32)
		current_total_cost = (double)current_cost.get_int32();

	if (expense_cost.type() == bsoncxx::type::k_double)
		current_total_cost -= expense_cost.get_double();
	else if (expense_cost.type() == bsoncxx::type::k_int64)
		current_total_cost -= (double)expense_cost.get_int64();
	else if (expense_cost.type() == bsoncxx::type::k_int32)
		current_total_cost -= (double)expense_cost.get_int32();
	

	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$set", make_document(kvp("Costs.Total", current_total_cost)))));

	/*------------------------------------------------------------------------------------------------------------------------------------------*/

	//Delete from the expenses list
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$pull", make_document(kvp("Costs.List", expense_name)))));

	//Delete the expense and its cost
	std::string nav_path = "Costs." + expense_name;
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$unset", make_document(kvp(nav_path, "")))));

	return 1;
}

int reset_expenses(mongocxx::collection expenses)
{
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "name" << "OperationalCosts"
		<< "Expected Expenditure" << 0.0
		<< "Costs"
		<< bsoncxx::builder::stream::open_document
			<< "List"
			   << bsoncxx::builder::stream::open_array					
			   << bsoncxx::builder::stream::close_array
			<< "Total" << 0.0
		<< bsoncxx::builder::stream::close_document
		<< bsoncxx::builder::stream::finalize;

	bsoncxx::document::view replacement_view = doc_value.view();

	expenses.replace_one(make_document(kvp("name", "OperationalCosts")), replacement_view);

	return 1;
}

std::string get_expense_date()
{
	__time32_t aclock;

	char buffer[32];
	errno_t errNum;

	struct tm currtime;

	_time32(&aclock);//Get time in seconds
	_localtime32_s(&currtime, &aclock);//Convert time to struct tm form

	errNum = asctime_s(buffer, 32, &currtime);

	if (errNum)
		return "";//An error has occurred
	
	std::string m_date(buffer);

	return m_date;
}

int set_expected_expenditure(mongocxx::collection expenses, double expenditure)
{
	//Delete from the expenses list
	expenses.update_one(make_document(kvp("name", "OperationalCosts")), make_document(kvp("$set", make_document(kvp("Expected Expenditure", expenditure)))));

	return 1;
}