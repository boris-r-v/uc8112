#include "serial.h"
#include "device.h"
#include "log.h"

void create_devices ( std::vector<Device>& devices, riku::Serial& serial )
{

    devices.push_back( Device(serial, "AK", 
                    {0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1}, 
                    {0x11,0x11,0x11} 
                ) );

    devices.push_back( Device(serial, "ION",
                    {0x2,0x2,0x2,0x2,0x2}, 
                    {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22,0x22,0x22,0x22,22,0x22,0x22} 
                ) );

}



/*-------------------------*/

Device::Device ( riku::Serial& s, std::string t, std::vector <uint8_t> r, std::vector <uint8_t> a ):
    name_( t ),
    serial_( s ),
    request_( r ),
    answer_( a )
{
}

std::string Device::name()
{
    return name_;
}

bool Device::check_answer(  uint8_t const* buff )
{
    for ( size_t i = 0; i < answer_.size(); ++i )
    {
        if ( buff[i] != answer_[i] )
            return false;
    }
    return true;
}

bool Device::check_request( uint8_t const* buff )
{
    for ( size_t i = 0; i < request_.size(); ++i )
    {
        if ( buff[i] != request_[i] )
            return false;
    }
    return true;
}

uint8_t const* Device::request_out()
{
    return request_.data();
}
uint8_t* Device::request_in()
{
    return reqbuff_;
}
size_t Device::rsize()
{
    return request_.size();
}

uint8_t const* Device::answer_out()
{
    return answer_.data();
}
uint8_t* Device::answer_in()
{
    return ansbuff_;
}
size_t Device::asize()
{
    return answer_.size();
}