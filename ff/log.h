/*
 The MIT License (MIT)
Copyright (c) 2013 <AthrunArthur>
Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef FF_COMMON_LOG_H_
#define FF_COMMON_LOG_H_
#include "ff/log/logger.h"
#include "ff/log/logwriter.h"
#include "ff/singlton.h"

#define USING_FF_LOG 1
namespace ff
{
enum LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
};

template<class T = LogLevel>
class log
{
public:
    inline static void		init(const T & l, const char * logfile)
    {
		ll = l;
		singleton<internal::logwriter<blocking_queue<std::string> > >::instance().run(logfile);
    }
    inline static void		init(const T & l, const std::string & logfile)
    {
		ll = l;
		singleton<internal::logwriter<blocking_queue<std::string> > >::instance().run(logfile.c_str());
    }
    static T	ll;
};//end class log

template<class T>
T log<T>::ll = ERROR;

namespace llog
{
	template<class T>
	struct enable_traits{
		static const bool value = false;
	};//end class enable_traits;
}
}//end namespace ff


#define DEF_LOG_MODULE(module) struct log_ ##module{};

#define ENABLE_LOG_MODULE(module) \
	namespace ff{namespace llog{ \
  template<> struct enable_traits<log_ ##module> { \
	static const bool value = true; };  \
	}}

#define LOG_TRACE(module)  if(::ff::log<>::ll <= ::ff::TRACE) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tTRACE"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"

#define LOG_DEBUG(module)  if(::ff::log<>::ll <= ::ff::DEBUG) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tDEBUG"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"

#define LOG_INFO(module)  if(::ff::log<>::ll <= ::ff::INFO) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tINFO"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"
	
#define LOG_WARN(module)  if(::ff::log<>::ll <= ::ff::WARN) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tWARN"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"
	
#define LOG_ERROR(module)  if(::ff::log<>::ll <= ::ff::ERROR) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tERROR"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"
	
#define LOG_FATAL(module)  if(::ff::log<>::ll <= ::ff::FATAL) \
	ff::internal::logger<ff::llog::enable_traits<log_ ## module>::value >()\
	<<"\tFATAL"<<"\t"<<#module<<"\t"<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"\t"

#endif