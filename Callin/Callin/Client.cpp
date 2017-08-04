// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#include <conio.h>
#include <afxwin.h>
#define port 8088 

using namespace std;

class dead
{
private:
	bool con_status;
	string ip;
	SOCKET sock;
public:
	dead();
	~dead();
	void initialise(string ip);
	int send_message(string message);
	int recv_message();
	bool connection_status;
};
dead clinet;
UINT listenthread(LPVOID pParam);
int clientmain(int argc, char **argv[]);

using namespace std;


dead::dead()
{
	con_status = FALSE;
}

dead::~dead()
{
	if (con_status)
		closesocket(sock);
}

void dead::initialise(string ip)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;

	WSADATA wsadata;
	if (WSAStartup(0x101, &wsadata) != 0)
		return;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return;

	addr = inet_addr(ip.c_str());

	hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (hp == NULL)
	{
		closesocket(sock);
		return;
	}

	server.sin_addr.S_un.S_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (connect(sock, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(sock);
		return;
	}
	connection_status = TRUE;
	return;
}


int dead::send_message(string message)
{
	int stat;
	stat = send(sock, message.c_str(), message.size() + 1, 0);
	if (stat == -1)
		return 1;
	return 0;
}

int dead::recv_message()
{
	char message[4096];
	int stat = 0;
	stat = recv(sock, message, 4096, 0);
	if (stat == -1)
		return 1;
	cout << "----->>" << message << endl;
	return	0;
}


UINT listenthread(LPVOID pParam)
{
	while (1)
	{
		if (client.recv_message())
			break;
	}
	return 0;
}

int clientmain(int argc, char **argv[])
{
	char buffer[4096];
	cout << "------------------------------------- Chat Client -------------------------------------\n";
	cout << "-----------------------------------Version 1.1(beta) -----------------------------------\n\n";
	cout << "Enter server's IP address = ";

	string serverip;
	gets_s(buffer);
a:
	serverip = buffer;

	if (serverip.size() == 0)
	{
		cout << "Server IP is invalid.\n";
		cout << "Enter sever's IP address = ";
		getch();
		goto a;
	}

	client.initialise(serverip.c_str());
	if (!client.connection_status)
	{
		cout << "\nCannot connect to the server";
		getch();
		return 0;
	}

	AfxBeginThread(listenthread, 0);

	while (gets_s(buffer))
	{
		if (strlen(buffer) == 0)
			break;
		if (client.send_message(buffer))
		{
			cout << "\nProblem Connecting to the server";
			break;
		}
	}
	cout << "\n\nEnd";
	getch();
	return 0;
}