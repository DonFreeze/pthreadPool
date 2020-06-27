#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

#include "Mutex.h"
#include "Task.h"
#include "CondVar.h"
#include "Global.h"

using namespace std;

namespace threadpool
{
    class ThreadPool
    {
    public:
        ThreadPool( std::size_t numThreads);
        ~ThreadPool();
        void enqueue(Task* task);
        void* executeThread();
        bool isRunning();

    private:
        int start();
        void stop();


        std::size_t numThreads;

        Mutex mTaskMutex;

        CondVar mTaskCondVar;

        std::vector<pthread_t> mThreads;
        std::deque<Task*> mTasks;

        volatile int mPoolState;
    };

}

