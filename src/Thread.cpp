
// TODO: why do we need printerror????
#include <iostream>

// TODO: use the DBUtl version of assert?
// #include <assert.h>

#include "DBUtl/Thread.h"
#include "DBUtl/Log.h"
#include "DBUtl/LogCPP.h"

using namespace DBUtl;

Thread::Thread(std::auto_ptr<Runnable> _runnable, bool isDetached) : runnable(_runnable), detached(isDetached) {
	DBUTL_ASSERT_ALWAYS( runnable.get() != NULL );
}

Thread::Thread(bool isDetached) : runnable(NULL), detached(isDetached) { }

void* Thread::startThreadRunnable(void* pVoid) {
    // thread start function when a Runnable is involved
    Thread* runnableThread = static_cast<Thread*> (pVoid);
    DBUTL_ASSERT( runnableThread );
    runnableThread->result = runnableThread->runnable->run();
    runnableThread->setCompleted();
    return runnableThread->result;
}

void* Thread::startThread(void* pVoid) {
    // thread start function when no Runnable is involved
    Thread* aThread = static_cast<Thread*> (pVoid);
    DBUTL_ASSERT( aThread );
    aThread->result = aThread->run();
    aThread->setCompleted();
    return aThread->result;
}

Thread::~Thread() { }

void Thread::start() {
    int status = pthread_attr_init(&threadAttribute);  // initilize attribute object
	DBUTL_ASSERT_ALWAYS( status == 0 );
    status = pthread_attr_setscope(&threadAttribute, PTHREAD_SCOPE_SYSTEM);
	DBUTL_ASSERT_ALWAYS( status == 0 );
    if (!detached) {
        if (runnable.get() == NULL) {
            status = pthread_create(&pthreadThreadID, &threadAttribute, Thread::startThread, (void*) this);
        }
        else {
            status = pthread_create(&pthreadThreadID, &threadAttribute, Thread::startThreadRunnable, (void*) this);
        }
		DBUTL_ASSERT_ALWAYS( status == 0 );
    }
    else {
        // set the detachstate to detached
        status = pthread_attr_setdetachstate(&threadAttribute, PTHREAD_CREATE_DETACHED);
		DBUTL_ASSERT_ALWAYS( status == 0 );
        if (runnable.get() == NULL) {
            status = pthread_create(&pthreadThreadID, &threadAttribute, Thread::startThread, (void*) this);
        }
        else {
            status = pthread_create(&pthreadThreadID, &threadAttribute, Thread::startThreadRunnable, (void*) this);
        }
		DBUTL_ASSERT_ALWAYS( status == 0 );
        status = pthread_attr_destroy(&threadAttribute);
		DBUTL_ASSERT_ALWAYS( status == 0 );
    }
}

void* Thread::join() {
    int status = pthread_join(pthreadThreadID, NULL);
	DBUTL_ASSERT_ALWAYS( status == 0 );
    return result;
}

void Thread::setCompleted() { }

void Thread::printError(const std::string& msg, int status, const std::string& fileName, int lineNumber) {
     LOG(INFO) << msg << " " << fileName << ":" << lineNumber << "-" << strerror(status) << std::endl;
}
