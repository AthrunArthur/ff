#ifndef FF_COMMON_BLOCKING_QUEUE_H_
#define FF_COMMON_BLOCKING_QUEUE_H_
#if __cplusplus < 201103L
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/noncopyable.hpp>
#else
#include <mutex>
#include <condition_variable>
#endif

#include <queue>

namespace ff {
template<class Ty>
class blocking_queue
#if __cplusplus < 201103L
: public boost::noncopyable
#endif
{
public:
#if __cplusplus >= 201103L
    blocking_queue(const blocking_queue<Ty> & ) = delete;
    blocking_queue<Ty> operator =(const blocking_queue<Ty> &) = delete;
#endif
    blocking_queue()
        : m_oMutex()
        , m_oCond()
        , m_bForceQuit(false)
        , m_oContainer() { }

    void push_back(const Ty & val)
    {
        m_oMutex.lock();
        m_oContainer.push(val);
        size_t s = m_oContainer.size();
        m_oMutex.unlock();

        if(s == 1)
        {
            m_oCond.notify_all();
        }
    }

    bool pop(Ty & val)
    {
#if __cplusplus < 201103L
        boost::unique_lock<boost::mutex> ul(m_oMutex);
#else
        std::unique_lock<std::mutex> ul(m_oMutex);
#endif

        while(m_oContainer.empty() && !m_bForceQuit)
        {
            m_oCond.wait(ul);
        }

        if(m_bForceQuit)
            return false;
        val = m_oContainer.front();
        m_oContainer.pop();
        return true;
    }

    size_t size() const
    {
#if __cplusplus < 201103L
        boost::unique_lock<boost::mutex> ul(m_oMutex);
#else
        std::unique_lock<std::mutex> ul(m_oMutex);
#endif
        return m_oContainer.size();
    }
    bool empty() const
    {
#if __cplusplus < 201103L
        boost::unique_lock<boost::mutex> ul(m_oMutex);
#else
        std::unique_lock<std::mutex> ul(m_oMutex);
#endif
        return m_oContainer.empty();
    }

    void force_quit()
    {
        m_bForceQuit = true;
        m_oCond.notify_all();
    }
protected:
#if __cplusplus < 201103L
    mutable boost::mutex m_oMutex;
    mutable boost::condition_variable m_oCond;
#else
    mutable std::mutex  m_oMutex;
    mutable std::condition_variable m_oCond;
#endif
    bool	m_bForceQuit;
    std::queue<Ty> m_oContainer;

};

}//end namespace ff


#endif
