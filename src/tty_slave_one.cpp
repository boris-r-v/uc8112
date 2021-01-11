#include "serial.h"
#include "device.h"
#include "log.h"

#include <unistd.h> //for usleep
#include <string.h> //for bzero

class Slave
{
        unsigned good, bad;
        riku::Serial& serial_;
        std::vector<Device> devices_;
        static const size_t ansbuff_size_ = 100;
        uint8_t reqbuff_[ansbuff_size_ ];

    public:
        Slave ( riku::Serial& s );
        void run();
};

Slave::Slave( riku::Serial & s):
    good( 0 ),
    bad( 0 ),
    serial_( s )
{

    create_devices( devices_, s );

}

void Slave::run()
{
    while (1) 
    {
        bzero( reqbuff_ , ansbuff_size_ );
        int reads = serial_.ReadTTY( reqbuff_, ansbuff_size_, 1  );
        if ( -1 != reads )
        {
            serial_.WriteTTY( devices_.front().answer_out(), devices_.front().asize() );
        }
    }
}


int main( int argc, char** argv )
{
    if ( argc < 2 ) { std::cerr <<"Usage: tty_slave name_of_serial_port (/dev/ttyM1) [wait_us_before_answer]" << std::endl; return 1; }
    unsigned wait = 100;
    if ( argc == 3 )
	wait = atoi(argv[2]);

    riku::Serial s ( argv[1], 19200, 1, 0.1, 1, wait );

    s.SetStopBits( 1 );

    Slave m( s );
    m.run();
    return 0;
}


