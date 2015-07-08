#include <iostream>
#include "SimpleSerial.cpp"
#include <time.h>
#include "..\NewVision C++\control\controle.h"
#include <math.h>


using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
	clock_t start, end;
	SimpleSerial serial("COM5",9600);
	

	serial.send(0, -15, 15);
	
	start = clock();
	end = clock();
	while((end-start) < 5000)
	{
		serial.send(0, -15, 15);
		//cout << serial.readLine() << endl;
		end = clock();
	}
	cout << ceil(2.4) << endl;
	while(true)
	{
		serial.send(0, 15, -15);
	}

	return 0;
}
