//Main cpp file for the program
#include "Callin.h"
#include "stdafx.h"

using namespace std; 

int main(int argc, char *argv[])
{
	cout << "============================== Callin ==============================\n";
	cout << "Developed by n00bs\n";
	cout << "\n\nMain Menu:\n";
	cout << "1.Create a server\n";
	cout << "2.Open client \n";
	cout << "3.Open readme \n";
	int a;
	cin >> a;
	switch (a)
	{
	case 1:
		cout << "\n\nStarting server !";
		servermain(argc, &argv);
		break;
	case 2:
		cout << "\n\nOpening A client ! ";
		clientmain(argc, &argv);
		break;
	default:
		cout << "\nInvalid Input. Please try again.";
		break;
	}
	cout << "\n\nThanks for using Callin.Please send your feedbacks";
	getch();
	return 0;
}