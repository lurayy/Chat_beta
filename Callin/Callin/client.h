//Header File for Client.cpp
#pragma once

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
