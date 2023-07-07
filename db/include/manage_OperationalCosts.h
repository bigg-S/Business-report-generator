#pragma once

/*
* Handles the deletion and insertion of an operational cost in the database
*/

#include "app_basic.h"

/*Insert a new expense*/
int insert_expense(mongocxx::collection expenses, std::string expense_name, double exppense_cost);

/*Delete an expense*/
int delete_expense(mongocxx::collection expenses, std::string expense_name);

/*Get the date of recording the expense*/
std::string get_expense_date();

/*Reset the expenses to none*/
int reset_expenses(mongocxx::collection expenses);

int set_expected_expenditure(mongocxx::collection expenses, double expenditure);