#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "serial.h"


using namespace std;

SimpleSerial::SimpleSerial(std::string port, unsigned int baud_rate)
: io(), serial(io,port)
{
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}


void SimpleSerial::send(int id, int v_e, int v_d)
{
	int msg_e, msg_d;
		
	//std::string md;

	if (v_e > 0)
	{
		msg_e = (id*64) + (1 * 32) + v_e;
	}

	else
	{
		msg_e = (id*64) + (1 * 32) + (1 * 16) + abs(v_e);
	}
	
	if (v_d > 0)
	{
		msg_d = (id*64) + abs(v_d);
	}
	else
	{
		msg_d = (id*64) + (1 * 16) + abs(v_d);
	}

	string me = boost::lexical_cast<string>( msg_e );
	string md = boost::lexical_cast<string>( msg_d );
	//me = std::to_string(()msg_e)
	//me = msg_e;
	//md = msg_d;
	
	writeString(me);
	writeString(md);

	
}
	
void SimpleSerial::writeString(std::string s)
{
	boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
	printf("PEDRO VIADO \n \n \n \n");
}

std::string SimpleSerial::readLine()
{
    //Reading data char by char, code is optimized for simplicity, not speed
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(serial,asio::buffer(&c,1));
        switch(c)
        {
            case '\r':
                break;
            case '\n':
                return result;
            default:
                result+=c;
        }
    }
}




