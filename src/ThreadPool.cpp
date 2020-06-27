#include "ThreadPool.h"
#include <iostream>

using namespace std;
using namespace threadpool;

ThreadPool::ThreadPool(std::size_t numThreads) : numThreads(numThreads)
{
    cout << "Constructed ThreadPool of size " << numThreads << endl;
    this->start();
}

ThreadPool::~ThreadPool()
{
    stop();
}


extern "C"
void* start_thread(void* arg)
{
    ThreadPool* tp = (ThreadPool*) arg;
    tp->executeThread();
    return NULL;
}

int ThreadPool::start()
{

  mPoolState = STARTED;
  int ret = -1;
  for (auto i = 0u; i < numThreads; ++i) {
    pthread_t tid;
    ret = pthread_create(&tid, NULL, start_thread, (void*) this);
    if (ret != 0) {
      cerr << "pthread_create() failed: " << ret << endl;
      return -1;
    }
    mThreads.push_back(tid);
  }
  cout << numThreads << " threads created by the thread pool" << endl;

  return 0;
}

void ThreadPool::stop()
{
    mTaskMutex.lock();
    mPoolState = STOPPED;
    mTaskMutex.unlock();

    mTaskCondVar.broadcast();

    for(pthread_t thread : mThreads)
    {
        void* result;
        pthread_join(thread, &result);
        mTaskCondVar.broadcast();
        cout << "join Thread"  << endl;
    }
}

bool ThreadPool::isRunning()
{
  return   mTasks.size()  > 0 ? true : false;
}


void* ThreadPool::executeThread()
{
    Task* task = NULL;
    auto id = pthread_self();
    cout << "Starting thread "<< &id << endl;
    while(true)
    {
        mTaskMutex.lock();
        while ((mPoolState != STOPPED) && (mTasks.empty()))
        {
            mTaskCondVar.wait(mTaskMutex.get_mutex_ptr());
        }

        if (mPoolState == STOPPED)
        {
              mTaskMutex.unlock();
              pthread_exit(NULL);
        }

        task = mTasks.front();
        mTasks.pop_front();
        mTaskMutex.unlock();

        (*task)();
        delete task;
    }
    return NULL;
}

void ThreadPool::enqueue(Task* task)
{
  mTaskMutex.lock();

  mTasks.push_back(task);
  mTaskCondVar.signal();

  mTaskMutex.unlock();
}
