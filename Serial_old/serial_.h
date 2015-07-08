#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>


class SimpleSerial
{
public:
	SimpleSerial(std::string, unsigned int);
	//~SimpleSerial();
	void send(int, int, int);
	void writeString(std::string);
	std::string readLine();

private:
	boost::asio::io_service io;
    boost::asio::serial_port serial;


};