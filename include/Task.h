#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

#include "Global.h"

using namespace std;

namespace threadpool
{
    //template<class TClass>
    class Task
    {
    public:
        Task(void (*fn_ptr)(void*), void* arg) : m_fn_ptr(fn_ptr), m_arg(arg) {}

        void operator()();
        void run();
    private:
        void (*m_fn_ptr)(void*);
        void* m_arg;
    };
}
