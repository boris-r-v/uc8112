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

    devices_.push_back( Device(s, "AK", 
                    {0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1}, 
                    {0x11,0x11,0x11} 
                ) );

    devices_.push_back( Device(s, "ION",
                    {0x2,0x2,0x2,0x2,0x2}, 
                    {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22} 
                ) );

}

void Master::run()
{
    while (1) 
    {
        for ( auto dev : devices_ )
        {
            std::cout << "Handle: " << dev.name() << std::endl;
            serial_.WriteTTY( dev.request_out(), dev.rsize() );
//           usleep(500000);
            serial_.ReadTTY( dev.answer_in(), dev.asize(), 100, dev.name() );
            
            if ( dev.check_answer( dev.answer_in() ) )
            {
            //    KSA_MASTER << "answer valid" << std::endl;
                ++good;
            }
            else
            {
            //    KSA_MASTER << "answer not valid" << std::endl;
                ++bad;
            }
            usleep(500000);
        }
std::cout << "  Statistic good:bad -- " << good << ":" << bad << std::endl;
    }
}


int main( int argc, char** argv )
{
    riku::Serial s ( argv[1], 19200, 1, 0.1, 70, 70 );
    Master m( s );
    m.run();
}


