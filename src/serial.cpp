#include "log.h"
#include "serial.h"

#include <termios.h>
#include <unistd.h>
#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

riku::Serial::Serial( const std::string& _tty, unsigned _speed, unsigned _debug, float _sleep, unsigned _ansWait, unsigned _reqWait ):
    sfd ( -1 ),
    speed(_speed ),
    debug(_debug ),
    loopWait ( _sleep*1000000 ),
    ansWait (_ansWait ),
    reqWait (_reqWait ),
    tty(_tty )
{
    KSA_LOG << " Serial: create with tty=\"" << tty <<"\" speed=\""<< speed << "\" sleep=\"" << _sleep << "\" ansWait=\"" << _ansWait << "us\" reqWait=\"" << _reqWait <<"us\"" <<std::endl;
    InitSerial();
}



riku::Serial::~Serial ()
{
}

void riku::Serial::CloseSerial( )
{
    if ( sfd > 0 )
        ::close( sfd );
    sfd = -1;
}


bool riku::Serial::InitSerial()
{
    CloseSerial();
    sfd = OpenTTY( );
    if (sfd == -1 )
    {
        KSA_ERROR <<"Serial InitSerial : init-result "<< tty <<" occur error: " << strerror(errno)<<std::endl;
        return false;
    }
    else
    {
        SetStopBits( 2 );
        if ( SpeedTTY( speed ) )
        {
            KSA_LOG << "Serial InitSerial : init-result "<< tty <<" successful. Start handle tty devices."<<std::endl;
        }
        else
        {
            KSA_ERROR << "Serial InitSerial : init-result "<< tty <<" set speed ivoke error, but we start handle tty devices."<<std::endl;
        }
        return true;
    }
    return true;
}

void riku::Serial::SetStopBits ( unsigned stb )
{
    struct termios newtio;
//    tcgetattr( sfd, &oldtio);
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag = B19200 | CS8  | CLOCAL  | CREAD  | ( stb == 2 ? CSTOPB : 0 );
    newtio.c_iflag = 0 ;
    newtio.c_oflag = 0 ;
    newtio.c_lflag = IEXTEN;

    tcsetattr(sfd, TCSANOW, &newtio);
}

int riku::Serial::OpenTTY ( )
{
    int fd( -1 );

    std::cout<<" before Open "<<tty<<", fd="<<fd<<" debug="<<debug<<std::endl;
    fd = open(tty.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) 
    {
        std::cout<<" riku::Serial : Can`t open tty "<<tty<<std::endl;
        return -1;    

    }
    else std::cout<<" Open "<<tty<<", fd="<<fd<<std::endl;

    tcflush(fd, TCOFLUSH);
    tcflush(fd, TCIFLUSH);
    tcflow(fd,TCOON);

    return fd;
}

bool riku::Serial::SpeedTTY ( unsigned baudrate)
{
    struct termios newtio;
    int speed=B19200;
    switch (baudrate) {
        case 19200:
            speed=B19200;
            break;
        case 38400:
            speed=B38400;
            break;
        case 57600:
            speed=B57600;
            break;
        case 115200:
            speed=B115200;
            break;
        default :
            speed=B19200;
            break;
    }
    if( !tcgetattr(sfd, &newtio) )
    {
        cfsetospeed(&newtio, speed);
        cfsetispeed(&newtio, speed);
        if( tcsetattr(sfd, TCSANOW, &newtio) )
           KSA_ERROR << "riku::Serial::SpeedTTY:: set speed "<< baudrate << " for \""<<tty<<"\" " << strerror(errno) << std::endl;
        tcflush(sfd, TCOFLUSH);
        tcflush(sfd, TCIFLUSH);
        tcflow(sfd, TCOON);
        return true;
    }
    KSA_ERROR << "riku::Serial::SpeedTTY:: get tty-attr for \""<<tty<<"\" " << strerror(errno) << std::endl;
    return false;
}

int riku::Serial::WriteTTY ( const void * buffer, int sizeBuffer )
{
    const unsigned char * buff = (const unsigned char*) buffer;
    /*Before send new data, sleep to 3 millyseconds, protocol request*/
//    struct timeval tv={ 0, (int)reqWait };
//    select ( 0, NULL, NULL, NULL, &tv );

    
    std::cout<<"    tty-write: ";
    for (int i=0; i<sizeBuffer; i++)
       printf ("%.2x ",buff[i] );
    printf ("\n");
    
     /*Flush all data into stream*/
    tcflush( sfd, TCIOFLUSH );
    int size = 0, ret;

    while(size != sizeBuffer) 
    {
        ret=write(sfd, buffer, sizeBuffer);
        if( ret == 0 || ret == -1)
        {
            std::cerr << "riku::Serial::WriteTTY, can't write data to tty " << tty << ", write(...) return: " << ret << std::endl;
            return -1;
        }
        size+=ret;
    }
    if ( -1 == tcdrain(sfd) )
        KSA_ERROR << "riku::Serial::WriteTTY, tcdrain strerror: " << strerror(errno) << std::endl;

    if ( size == sizeBuffer ) return size;
    else return -1;
}

int riku::Serial::ReadTTY ( void * buffer, int sizeBuffer, unsigned us_timeout, std::string const& device_name )
{

   int ret = ReadTTY_inner (buffer, sizeBuffer, us_timeout, device_name );

    if ( ret > 0 )
    {
       uint8_t* buff = static_cast<uint8_t*>(buffer);
       std::cout<<"    tty-read: ";
       for ( int i=0; i< ret; ++i )  printf ("%.2x ",buff[i] );
       printf ("\n");
    }

    return ret;
}
//if device_name length is null - invoke REadTTy from slave.
int riku::Serial::ReadTTY_inner ( void * buffer, int sizeBuffer, unsigned us_timeout, std::string const& device_name )
{
    fd_set readfds;
    uint8_t* ptr = static_cast<uint8_t*>(buffer);
    int bytes(0), start(0), nfds( sfd+1 ), cntr( 0 ),  offset(0);

    FD_ZERO(&readfds);
    FD_SET(sfd, &readfds);
    long int ct = us_timeout > ansWait ? us_timeout  : ansWait;
    //std::cout << "riku::Serial::Timeout calc: " << timeout << " xml-set: " <<  ansWait << " handle: " << ct << std::endl;
    struct timeval tv={ 0, ct };
    while(1) 
    {
        int ret = select(nfds, &readfds, NULL, NULL, &tv );
        if( -1 == ret || 0 == ( FD_ISSET (sfd, &readfds ) ) ) 
        {
            //std::cout << "riku::Serial::ReadTTY, responce time "<< ct <<"(ms) out, select(...) ret: " << ret << ", error: "<<  strerror(errno) << std::endl;
            if ( device_name.size() )
            {
                if (0 == start ) KSA_WARNING << "Serial::ReadTTY("<< tty<<"):: время " <<  ct << " (мкс) ожидания ответа от \"" << device_name  <<"\" истекло, неполучено ни одного байта данных." << std::endl;
                //else  KSA_LOG << "Serial::ReadTTY("<< tty<<"): для "<< device_name << " прочитали: " << start << " байт. (select wait:" << ct<< "(ms), return:" << ret <<")" << std::endl;  
            }
            return start==0 ?  -1 : start;
        }
        ++cntr;
        ioctl(sfd, FIONREAD, &bytes);
        if ( bytes > 0 ) 
        {
//std::cout << "offset: " << offset << " start: " << start << " bytes: " << bytes << std::endl;
            offset = start;
            if ( bytes > sizeBuffer-start ) 
            {
                KSA_ERROR << "Serial::ReadTTY("<<tty<<"): готово к чтению " << bytes << " байт, а в ответном буфере " << device_name << " осталось свободно: " << sizeBuffer-start << " байт" << std::endl;
                bytes = sizeBuffer-start;
            }

            start += read( sfd, ( ptr + offset ), bytes );
/*
for ( int i = 0; i < start; ++i )
    std::cout << std::hex << "0x" << (int)ptr[i] << " ";
std::cout << std::dec << std::endl;
*/
            if ( start >= sizeBuffer )
            {
                ///После приема всего пакета, всегда ждем время достаточное для принятия всего пакета и очищаем приемный буфер от того что пришло
//                usleep( us_timeout );
//                tcflush( sfd, TCIFLUSH );
                return sizeBuffer;
            }

        }
        tv.tv_usec = ct;
    }
    return -1;
}
