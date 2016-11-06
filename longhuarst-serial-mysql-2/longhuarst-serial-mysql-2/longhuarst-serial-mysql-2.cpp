#include <iostream>
#include "clh/serial/tty.h"

using namespace std;

clh::serial::tty *tty1;
clh::serial::tty *tty2;


int main(int argc, char *argv[])
{
	tty1 = new clh::serial::tty();
	tty2 = new clh::serial::tty();

	tty1->init("/dev/ttyS1", 9600);
	tty2->init("/dev/ttyS2", 9600);


	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here



	delete tty1;
	delete tty2;
	return 0;
}