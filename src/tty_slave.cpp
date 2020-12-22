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
        int reads = serial_.ReadTTY( reqbuff_, ansbuff_size_, 20  );
        if ( -1 != reads )
        {
            //std::cout << "Read " << reads << " bytes" << std::endl;
            bool get_req_for_any_device = false;
            for ( auto dev : devices_ )
            {
                if ( dev.check_request( reqbuff_ ) )
                {
                    int writes = serial_.WriteTTY( dev.answer_out(), dev.asize() );
                    //std::cout << "Device: " << dev.name() << " send answer: " << writes << " bytes" << std::endl;
                    get_req_for_any_device = true;
                    std::cout << "  Request for device: " << dev.name() << std::endl;
                }
            }

            if ( get_req_for_any_device )   
                ++good;
            else
                ++bad;

            std::cout << "STATICTIC: received_requests: " << (good+bad) << ", goods: " << good << ", bads: " << bad << std::endl;
        }
    }
}


int main( int argc, char** argv )
{
    riku::Serial s ( argv[1], 19200, 1, 0.1, 70, 7 );
    Slave m( s );
    m.run();
}


