
#pragma once

#include "app_common.h"

/*struct containing the details of a sale*/

struct SALE
{
	std::string product_name = "";
	std::string brand = "";
	std::string quantity = "";
	std::string season = "";
	std::string age_group = "";
	std::string gender = "";
	std::string location = "";
	int item_count = 0;
};

/*---------------------------------------------------------------------------------*/

/*Modification modes*/
typedef enum
{
	RESET = 0,
	UPDATE = 1,
	ADD = 2,
	REMOVE = 3
} MODE;

/*----------------------------------------------------------------------------------*/





