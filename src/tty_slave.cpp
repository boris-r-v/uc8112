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

    devices_.push_back( Device(s, "AK", 
                    {0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1}, 
                    {0x11,0x11,0x11} 
                ) );

    devices_.push_back( Device(s, "ION",
                    {0x2,0x2,0x2,0x2,0x2}, 
                    {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22} 
                ) );

}

void Slave::run()
{
    while (1) 
    {
        bzero( reqbuff_ , ansbuff_size_ );
        int reads = serial_.ReadTTY( reqbuff_, ansbuff_size_  );
        if ( -1 != reads )
        {
            std::cout << "Read " << reads << " bytes" << std::endl;
            ++good;
            for ( auto dev : devices_ )
            {
                if ( dev.check_request( reqbuff_ ) )
                {
                    int writes = serial_.WriteTTY( dev.answer_out(), dev.asize() );
                    std::cout << "Device: " << dev.name() << std::endl << "Send: " << writes << " bytes" << std::endl;
                }
                else
                {
                    std::cout << "Device: Not found" << std::endl;
                }
            }
            std::cout << "  Statistic good:bad -- " << good << ":" << bad << std::endl;
        }
        else
            ++bad;

        usleep(500000);
    }
}


int main( int argc, char** argv )
{
    riku::Serial s ( argv[1], 19200, 1, 0.1, 70, 70 );
    Slave m( s );
    m.run();
}


