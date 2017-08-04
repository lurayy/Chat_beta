#pragma once

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <WinSock2.h>
#include<afxwin.h>
#include <string.h>
#include <list>
#include <stdio.h>

#define port 8088

using namespace std;


class chatserver
{
private:
	bool ready;
	list<SOCKET> client_list;											//All sockets connected to client 
	SOCKET client;														//socket for client
	SOCKET listen_client;												//socket listening for incoming clients 
public:
	chatserver();
	~chatserver();
	int check_connection();											//to check connection status
	int start_listening();												//Listen to incoming clients
	int send_message(string message);													//send message to all the connected clients
	int recv_message(SOCKET recv_socket);													//receive messgae from all the clients
};

UINT server_recv_thread(LPVOID pParam);									//process thread for reciveing incoming clients
UINT server_listen_thread(LPVOID pParam);								//process thread for listen for clients

int servermain(int argc, char **argv[]);