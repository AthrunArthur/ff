#ifndef FF_LOG_LOGGER_H_
#define FF_LOG_LOGGER_H_
#include <string>
#include <cstring>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "log/logwriter.h"
#include "singlton.h"

namespace ff
{
namespace internal {
template<bool EnableLogFlag>
class logger {
    std::stringstream  buffer_;
public:
    virtual ~logger()
    {
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		time_t now_c = std::chrono::system_clock::to_time_t(now);  
		std::stringstream ss;
		const char * s = asctime(std::localtime(&now_c));
		std::string str(s, std::strlen(s) -1);
		ss<<str<<"\t"<<buffer_.str();
        ff::singleton<logwriter<> >::instance().queue().push_back(ss.str());
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
    self & operator<<(const char * p){
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