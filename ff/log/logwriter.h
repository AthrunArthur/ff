#ifndef FFNET_COMMON_LOG_LOG_WRITER_H_
#define FFNET_COMMON_LOG_LOG_WRITER_H_
#include <memory>
#include <fstream>
#include "ff/blocking_queue.h"
#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include "ff/singlton.h"

#include <iostream>
namespace ff
{
namespace internal {
//Container
template<class T = ff::blocking_queue<std::string> >
class logwriter
{
protected:
    friend class ff::singleton<logwriter<T> > ;
    logwriter()
        : m_strFilePath()
        , m_bRunning(true) {};
public:
    virtual ~logwriter()
    {
        m_oMutex.lock();
        m_oQueue.push_back("End and quit log!");
		m_bRunning = false;
        m_oMutex.unlock();

        m_pIOThread->join();
    }
    T &	queue() {
        return m_oQueue;
    }
    void			run(const char * filePath)
    {
        m_strFilePath = std::string(filePath);
        if(m_pIOThread)
            return;
        m_pIOThread = std::make_shared<std::thread>([this](){
        this->actualRun();});
    }
    
    void			flush(std::string str)
    {
      m_oFile<<str<<std::endl;
    }
protected:
    void			actualRun()
    {
        std::string str;
        m_oFile.open(m_strFilePath.c_str() );

        m_oMutex.lock();
        while(m_bRunning || !m_oQueue.empty())
        {
            m_oMutex.unlock();
            size_t t = m_oQueue.size();
            while(t!=0)
            {
                m_oQueue.pop(str);
                m_oFile<<str<<std::endl;
                t--;
            }
            m_oFile.flush();
            m_oMutex.lock();
        }
        m_oMutex.unlock();
        m_oFile.close();
    }
protected:
    T		m_oQueue;
    std::shared_ptr<std::thread>		m_pIOThread;
    std::string					m_strFilePath;
    std::mutex					m_oMutex;
    std::ofstream				m_oFile;
    bool						m_bRunning;
};//end class LogWriter
}//end namespace internal
}//end namespace ffnet
#endif