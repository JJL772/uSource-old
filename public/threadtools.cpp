#include "threadtools.h"

#ifdef _POSIX
#include <pthread.h>
#else
#include <windows.h>
#endif

CThread::CThread()
{

}

CThread::CThread(void *(*threadfn)(void *))
{

}

void CThread::Run()
{

}

void CThread::Terminate()
{

}

void CThread::Kill()
{

}

void CThread::Join()
{

}
