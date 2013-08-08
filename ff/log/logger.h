#ifndef FF_LOG_LOGGER_H_
#define FF_LOG_LOGGER_H_
#include <string>
#include <cstring>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <thread>
#include <exception>
#include "ff/log/logwriter.h"
#include "ff/singlton.h"
#ifdef SYNC_WRITING_LOG
#include <iostream>
#endif
namespace ff
{
namespace internal {
template<bool EnableLogFlag>
class logger {
    std::stringstream  buffer_;
public:
    virtual ~logger()
    {
        try {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            const char * s = asctime(std::localtime(&now_c));


            std::string str(s, std::strlen(s) -1);
            ss<<str<<"\t"<<std::this_thread::get_id()<<buffer_.str();
#ifdef SYNC_WRITING_LOG
	    ff::singleton<logwriter<> >::instance().flush(ss.str());
	    std::cout<<ss.str()<<std::endl;
#else
            ff::singleton<logwriter<> >::instance().queue().push_back(ss.str());
#endif
        } catch(const std::exception & e)
        {
        }
    }
public:
    typedef logger<EnableLogFlag> self;

    template<typename T>
    self& operator<<(T v)
    {
        buffer_ << v;
        return * this;
    }
    template<typename T>
    self& operator<<(T * p)
    {
        uintptr_t v = reinterpret_cast<uintptr_t>(p);
        buffer_<<"0x"<<std::hex<<v<<"  ";
        return *this;
    }
    self & operator<<(const char * p) {
        buffer_<<p;
        return *this;
    }


    self & operator<<(bool v)
    {
        buffer_<<(v ? "1": "0");
        return *this;
    }

};

template<>
class logger<false> {
public:
    typedef logger<false> self;
    template<typename T>
    self& operator<<(T v) {
        return *this;
    }
    template<typename T>
    self& operator<< (T *v) {
        return *this;
    }
};
}//end namespace internal

}//end namespace ff
#endif