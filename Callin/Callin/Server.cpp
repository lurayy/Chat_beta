
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
#include "server.h"

using namespace std;

chatserver server;

chatserver::chatserver()												//defining constructor function
{
	cout << "Starting up server";
	ready = false;
	WSADATA wsadata;													//contains info about socket implementation
	sockaddr_in sock_data;												//contains info about the socket

	if (WSAStartup(0x101, &wsadata) != 0)
		return;

	sock_data.sin_family = AF_INET;										//initializing sock_data , net family to IPv4
	sock_data.sin_addr.S_un.S_addr = INADDR_ANY; 									//ip address to any
	sock_data.sin_port = htons((USHORT)port);							//port number to 8088

	listen_client = socket(AF_INET, SOCK_STREAM, 0);					//creating socket for listing to incoming client 

	if (listen_client == INVALID_SOCKET)								//error checking 
		return;
	if ((bind(listen_client, (sockaddr*)&sock_data, sizeof(sock_data))) != 0)	//establishing local association of the socket
		return;
	if (listen(listen_client, 20) != 0)									//preparing listen_client socket for taking incoming connections 
		return;

	ready = TRUE;
	return;

}


chatserver::~chatserver()												//defining destrouctor function
{
	closesocket(listen_client);											//closing listening socket
	WSACleanup();														//cleaning up all the other bullshits
}

int chatserver::check_connection()										//checking connection status 
{
	if (ready == TRUE)
		return 1;
	else
		return 0;
}

int chatserver::start_listening()										//listening to the servers
{
	sockaddr_in from;
	int fromlen = sizeof(from);

	client = accept(listen_client, (struct sockaddr*)&from, &fromlen);
	if (client != INVALID_SOCKET)
	{
		client_list.push_back(client);
	}
	AfxBeginThread(server_recv_thread, (void*)client);
	return 0;
}

int chatserver::send_message(string message)							//sned message to the server 
{
	int stat = 0;
	list<SOCKET>::iterator itl;

	if (client_list.size() == 0)
		return 0;
	for (itl = client_list.begin(); itl != client_list.end(); itl++)
	{
		stat = send(*itl, message.c_str(), message.size() + 1, 0);
		if (stat == -1)
			client_list.remove(*itl);
	}
	if (stat == -1)
		return 1;
	return 0;
}

int chatserver::recv_message(SOCKET recv_socket)						//recive the msg for the server 
{
	char temp[4096];
	int stat;
	stat = recv(recv_socket, temp, 4096, 0);
	if (stat == -1)
	{
		client_list.remove(recv_socket);
		return 1;
	}
	else
	{
		cout << ":" << temp << "\n";
		send_message(temp);
		return 0;
	}
	return 0;
}



UINT server_recv_thread(LPVOID pParam)										//thread for the reciving the message 
{
	SOCKET recv_socket = (SOCKET)pParam;
	while (1)
	{
		if (server.recv_message(recv_socket))
			break;
	}
	return 0;
}

UINT server_listen_thread(LPVOID pParam)									//thread for the listening 
{
	while (1)
	{
		server.start_listening();
	}
	return 0;
}


int servermain(int argc, char** argv[])
{
	int nRetcode = 0;
	char buffer[4096];

	cout << "----------------------------Chat server --------------------------\n";

	if (server.check_connection() == 0)
	{
		cout << "Failed to initialise server\n";
		getch();
		return 1;
	}
	AfxBeginThread(server_listen_thread, 0);

	while (gets_s(buffer))
	{
		if (strlen(buffer) == 0)
		{
			break;
		}
		if (server.send_message(buffer))
		{
			cout << "Cannot connect to the server \n";
			break;
		}
	}
	cout << "\n\n\n End";
	getch();
	return nRetcode;
}





/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int main()
{
int nRetCode = 0;

HMODULE hModule = ::GetModuleHandle(nullptr);

if (hModule != nullptr)
{
// initialize MFC and print and error on failure
if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
{
// TODO: change error code to suit your needs
wprintf(L"Fatal Error: MFC initialization failed\n");
nRetCode = 1;
}
else
{
// TODO: code your application's behavior here.
}
}
else
{
// TODO: change error code to suit your needs
wprintf(L"Fatal Error: GetModuleHandle failed\n");
nRetCode = 1;
}

return nRetCode;
}
*/