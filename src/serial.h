#ifndef __SERIAL_HH__
#define __SERIAL_HH__

#include <string>

namespace riku
{
    class Serial
    {
            int sfd;
            unsigned speed, debug, loopWait, ansWait, reqWait;
            std::string tty;

            int ReadTTY_inner ( void * buffer, int sizeBuffer, unsigned ms_timeout, std::string const&);

        public:
            Serial( const std::string& _tty, unsigned _speed, unsigned _debug, float _sleep, unsigned _ansWait, unsigned _reqWait );
            virtual ~Serial ();

            bool InitSerial ( );
            void CloseSerial( );
            int OpenTTY ( );
            bool SpeedTTY ( unsigned baudrate);

            int WriteTTY ( const void * buffer, int sizeBuffer);
            int ReadTTY ( void * buffer, int sizeBuffer, unsigned ms_timeout = 10, std::string const& dev = "");

    };

}

#endif