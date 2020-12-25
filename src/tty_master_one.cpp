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
            std::cout << "  Handle: " <<  devices_.front().name() << std::endl;
            serial_.WriteTTY( devices_.front().request_out(), devices_.front().rsize() );
            serial_.ReadTTY( devices_.front().answer_in(), devices_.front().asize(), 150000, devices_.front().name() );
            if (  devices_.front().check_answer( devices_.front().answer_in() ) )
            {
                ++good;
            }
            else
            {
                ++bad;
            }
            std::cout<< "STATICTIC: sended_requests: " << (good+bad) << ", goods: " << good << ", bads: " << bad << std::endl << std::endl ;
            usleep(200000);
    }
}


int main( int argc, char** argv )
{
    if ( argc != 2 ) { std::cerr <<"Usage: tty_master name_of_serial_port (/dev/ttyM1)" << std::endl; return 1; }
    riku::Serial s ( argv[1], 19200, 1, 0.001, 70000, 5000 );
    Master m( s );
    m.run();
    return 0;
}


