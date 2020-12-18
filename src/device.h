#ifndef __DEVICE_HH__
#define __DEVICE_HH__

#include "serial.h"
#include <vector>
#include <string>
#include <stdint.h>

class Device
{
        std::string name_;
        riku::Serial& serial_;
        std::vector <uint8_t> request_, answer_;
        static const size_t ansbuff_size_ = 100;
        uint8_t ansbuff_[ansbuff_size_ ];
        uint8_t reqbuff_[ansbuff_size_ ];
    public:
        Device ( riku::Serial& s, std::string, std::vector <uint8_t> r, std::vector <uint8_t> a );
        Device() = delete;
        virtual ~Device() = default;
        uint8_t* request_in();
        uint8_t const* request_out();
        size_t rsize();

        uint8_t* answer_in();
        uint8_t const* answer_out();
        size_t asize();

        std::string name();

        bool check_answer( uint8_t const* );    //return true if answer is good, overwise - false
        bool check_request( uint8_t const* );   //return true if request is good, overwise - false

};



#endif // __DEVICE_HH__