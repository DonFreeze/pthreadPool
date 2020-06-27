#include "ThreadPool.h"


#include <iostream>

using namespace std;
using namespace threadpool;

#define Windows
const int MAX_TASKS = 400;


void hello(void* arg)
{
  int* x = (int*) arg;
  cout << "Hello " << *x << endl;
usleep(2000);
}



int main(int argc, char* argv[])
{
#ifdef Windows
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    size_t numCPU = sysinfo.dwNumberOfProcessors;
#endif // Windows

#ifdef LINUX
    size_t numCPU = sysconf(_SC_NPROCESSORS_ONLN);
#endif // LINUX


    ThreadPool threadPool( numCPU );

    for (int i = 0; i < MAX_TASKS; i++)
    {
        int* x = new int();
        *x = i+1;
        Task* task = new Task(&hello, (void*) x);
        threadPool.enqueue(task);
    }

    while( threadPool.isRunning() )
    {
    }


  cout << "Exiting app..." << endl;

  return 0;
}
