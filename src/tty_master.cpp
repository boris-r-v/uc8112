#include "serial.h"
#include "device.h"
#include "log.h"

#include <unistd.h> //for usleep

class Master
{
        unsigned good, bad;
        riku::Serial& serial_;
        std::vector<Device> devices_;
    public:
        Master ( riku::Serial& s );
        void run();
};

Master::Master( riku::Serial & s):
    good( 0 ),
    bad( 0 ),
    serial_( s )
{
    create_devices( devices_, s );

}

void Master::run()
{
    while (1) 
    {
        for ( auto dev : devices_ )
        {
//            std::cout << "Handle: " << dev.name() << std::endl;
            serial_.WriteTTY( dev.request_out(), dev.rsize() );
            serial_.ReadTTY( dev.answer_in(), dev.asize(), 150, dev.name() );
            if ( dev.check_answer( dev.answer_in() ) )
            {
                ++good;
            }
            else
            {
                ++bad;
            }
            usleep(50000);
        }
            std::cout << "  Statistic received_request: " << (good+bad) << ", goods: " << good << ", bads: " << bad << std::endl;
    }
}


int main( int argc, char** argv )
{
    riku::Serial s ( argv[1], 19200, 1, 0.1, 70, 7 );
    Master m( s );
    m.run();
}


