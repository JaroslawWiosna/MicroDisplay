#include <iostream>
#include "serial.h"


int main(int argc, char* argv[])
{
    SerialClass serial;

    if (serial.connect("/dev/ttyUSB0", 9600))
    {
        std::cout<<"Port is open."<<std::endl;
    }

    else
    {
        std::cout<<"Port open failed."<<std::endl;
    }

 
    while(1)
    {
        serial.send("Hello World\r\n");
        usleep(1000*500);
    }

    return 0;
}
