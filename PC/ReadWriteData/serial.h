#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


class SerialClass	
{
	boost::asio::io_service io;
	boost::asio::serial_port port;

	boost::thread receive_thr;

	boost::asio::streambuf buffer;

	

	bool quitFlag;

public:

	unsigned int receive_flag = 0;
	SerialClass();

    ~SerialClass();

    bool connect(const std::string& port_name, unsigned int baud);
    void disconnect();

    void receive_start();
    void receive_read(const boost::system::error_code &e, std::size_t size);
    std::string get_data();

    void send(const std::string &msg);

    bool quit();

};



