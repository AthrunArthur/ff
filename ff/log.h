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