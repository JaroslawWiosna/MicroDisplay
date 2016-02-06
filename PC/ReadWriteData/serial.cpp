#include "serial.h"


SerialClass::SerialClass(): port(io), quitFlag(false){};

SerialClass::~SerialClass()
{
    /*Stop the I/O services */
    io.stop();
    disconnect();

    /*Wait for the thread to finish*/
    receive_thr.join();    

}

bool SerialClass::connect(const std::string& portname, unsigned int baud)
{

    port.open(portname);
   
    port.set_option(boost::asio::serial_port::baud_rate(baud));
    port.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));

    if (port.is_open())
    {
        /* start io-service in a background thread */ 
        receive_thr = boost::thread( boost::bind(&boost::asio::io_service::run, &io) );

        receive_start();
    }

    return port.is_open();
}

void SerialClass::disconnect()
{
	if(port.is_open())
	{
		port.close();
	}

}

 /* read async receive and call the callback when \r\n */


void SerialClass::send(const std::string &msg)
{
	boost::asio::write(port, boost::asio::buffer(msg));
}

void SerialClass::receive_start()
{
	receive_flag = 0;
	boost::asio::async_read_until(port, buffer, "\r\n", 
		boost::bind(&SerialClass::receive_read, this, boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

void SerialClass::receive_read(const boost::system::error_code& e, std::size_t size)
{
    if (!e)
    {
        std::istream is(&buffer);
        std::string data(size,'\0');
        is.read(&data[0],size);

        std::cout<<"Received data: " << data;

        /* if receive quit, end of operation */
        quitFlag = (data.compare("quit()\r\n") == 0);    
        receive_start();
    }
 };

        // void read_start(void)
        // { // Start an asynchronous read and call read_complete when it completes or fails
        //         serialPort.async_read_some(boost::asio::buffer(read_msg_, max_read_length),
        //                 boost::bind(&minicom_client::read_complete,
        //                         this,
        //                         boost::asio::placeholders::error,
        //                         boost::asio::placeholders::bytes_transferred));
        // }
       
        // void read_complete(const boost::system::error_code& error, size_t bytes_transferred)
        // { // the asynchronous read operation has now completed or failed and returned an error
        //         if (!error)
        //         { // read completed, so process the data
        //                 cout.write(read_msg_, bytes_transferred); // echo to standard output
        //                 read_start(); // start waiting for another asynchronous read again
        //         }
        //         else
        //                 do_close(error);
        // } 

std::string SerialClass::get_data()
{
	std::string buffer_str;  

	// Convert streambuf to std::string  
	if(!buffer.size())
	{
		std::istream(&buffer) >> buffer_str;  
	}

	return buffer_str;

}

bool SerialClass::quit()
{
	return quitFlag;
}


