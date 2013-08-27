#include <iostream>
#include <boost/date_time.hpp>
#include <boost/timer.hpp>
#include "ff/threadpool.h"

typedef boost::shared_ptr<boost::function<void() > > task_t;

static void task_runner_wrapper(const task_t & ptask)
{
    (*ptask)();
}


template<class F>
static task_t make_task(const F & f)
{
    return task_t(new boost::function<void()>(f));
}

boost::atomic<int> couter;
const int LOOP = 10000;
void fun1(int a) {
    //std::cout<<"func 1"<<a<<std::endl;
}

void func2()
{
    couter ++;
}

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    couter.store(0);
    ff::threadpool<task_t> pool(3, task_runner_wrapper);
    pool.run();

    for(int i = 0; i< LOOP; i++)
    {
        pool.schedule(make_task(boost::bind(fun1, 1)));
        pool.schedule(make_task(func2));
    }
    std::cout<<"schedule done!"<<std::endl;
    
    pool.join();

    std::cout<<"couter is "<<couter.load()<<", should be "<<LOOP<<std::endl;
    
    
    
    return 0;
}
