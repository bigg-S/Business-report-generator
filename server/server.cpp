
#include "server.h"
#include "agents.h"
#include "agent_sales.h"

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



SALE get_sale_from_string(std::string sales_string, int64_t& agent_id)
{
	/*
	*	The sales string from the client is in the form: "id product_name brand_name quantity sason age_group gender location number_of_items"
	*	If a name has 2 parts eg. "Cooking Oil", it is received in the form "Cooking_Oil" for easy parsing. The underscore is then replaced by a space.
	*	The number of items and the agent id are converted from string to int and long respectively.
	*/
	std::string my_sale_string = sales_string;
	SALE sale;
	std::string item_name = "";
	size_t next_item_index = 0;
	int item_name_size = 0;


	for (int item_count = 0; item_count < 9; item_count++)
	{
		next_item_index = my_sale_string.find(" ");
		item_name_size = (int)next_item_index;
		item_name = my_sale_string.substr(0, item_name_size);

		switch (item_count)
		{
		case 0:
			agent_id = stol(item_name);
			break;
		case 1:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.product_name = item_name;
			break;
		case 2:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.brand = item_name;
			break;
		case 3:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.quantity = item_name;
			break;
		case 4:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.season = item_name;
			break;
		case 5:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.age_group = item_name;
			break;
		case 6:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.gender = item_name;
			break;
		case 7:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.location = item_name;
			break;
		case 8:
			std::replace(item_name.begin(), item_name.end(), '_', ' ');
			sale.item_count = stoi(item_name);
			break;
		default:
			break;
		}

		my_sale_string = my_sale_string.substr(next_item_index + 1, my_sale_string.size() - 1); //eliminate the extracted item from the sale string
	}


	return sale;
}


int  server()
{
		
	/*Specify the host to connect to and the port*/
	mongocxx::uri uri("mongodb://localhost:27017");

	/*Connect to a running MongoDB instance*/
	mongocxx::client client(uri);

	/* Access the database: sales & marketing (sm)*/
	mongocxx::database server_db = client["sm"];


	mongocxx::collection agent_sales = server_db["AgentSales"];
	mongocxx::collection products = server_db["products"];
	mongocxx::collection agents = server_db["Agents"];
	
	bsoncxx::stdx::optional<bsoncxx::document::value> details;
	bsoncxx::document::view details_view;

	//product details i.e, product names, brands, quantities, seasons etc
	details = products.find_one(document{} << "name" << "Components" << finalize);
	details_view = details.value().view();
	std::string components_json_doc = "items:" + bsoncxx::to_json(details_view);

	//agents ids
	details = agents.find_one(document{} << "name" << "AgentsData" << finalize);
	details_view = details.value().view();
	bsoncxx::array::view ids = details_view["IDs"].get_array();

	std::string agents_ids = "ids:_";

	for (bsoncxx::array::element id : ids)
	{
		agents_ids += id.get_string().value.to_string() + "_";
	}
	
	
	/*****************************SERVER CODE********************************/

	
	bool client_connected = false;

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) 
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	freeaddrinfo(result);
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	
	
	// Receive until the peer shuts down the connection
	do
	{
		if(client_connected == false)
		{
			// Accept a client socket
			ClientSocket = accept(ListenSocket, NULL, NULL);
			if (ClientSocket == INVALID_SOCKET) {
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
			else
			{
				client_connected = true;

				// send the agent ids to the  sender immediately after connection
				iSendResult = send(ClientSocket, agents_ids.c_str(), (int)agents_ids.size(), 0);
		
				if (iSendResult == SOCKET_ERROR)
				{
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
			}
		}
		
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);


		if (iResult > 0)
		{
			
			std::string agent_data = recvbuf;
			agent_data = agent_data.substr(0, iResult);
			
			if (agent_data.compare("Valid") == 0)
			{
				//send the products details to the client

				iSendResult = send(ClientSocket, components_json_doc.c_str(), (int)components_json_doc.size(), 0);

			}
			else 
			{
				//each sale from the client is separated by a new line character
				int no_of_sales = (int)std::count(agent_data.begin(), agent_data.end(), '\n');
				while (no_of_sales != 0)
				{
					size_t end_of_firs_line = agent_data.find_first_of('\n', 0);

					std::string sale_string = agent_data.substr(0, end_of_firs_line);
					int64_t agent_id = 0;
					SALE sale = get_sale_from_string(sale_string, agent_id);

					add_agent_sale(agent_sales, agent_id, sale);

					agent_data = agent_data.substr(end_of_firs_line + 1, agent_data.size() - 1);

					no_of_sales--;
				}
			}

			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (iResult == 0)
		{
			//client has disconnected
			client_connected = false;
		}
		else if(iResult < 0)//error occurred
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
				
	} while (1);


	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	// cleanup
	closesocket(ClientSocket);
	closesocket(ListenSocket);
	WSACleanup();

	return 1;
}
