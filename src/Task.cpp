#include "Task.h"
using namespace threadpool;

void Task::operator()()
{
    (*m_fn_ptr)(m_arg);
    if (m_arg != NULL)
    {
        delete m_arg;
    }
}

void Task::run()
{
    (*m_fn_ptr)(m_arg);
}
