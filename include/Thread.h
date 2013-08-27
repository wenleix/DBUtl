#ifndef __DBUTL_THREAD_H__
#define __DBUTL_THREAD_H__

#include <memory>
#include <string>

#include <pthread.h>
#include <cstdlib>

#include "Runnable.h"

namespace DBUtl {

class Thread {
public:
    Thread(std::auto_ptr<Runnable> _runnable, bool isDetached = true);
    Thread(bool isDetached = true);
    virtual ~Thread();
    void start();
    virtual void* join();
private:
    pthread_t pthreadThreadID;
    bool detached;  // true if thread created in detached state; false otherwise
    pthread_attr_t threadAttribute;
    std::auto_ptr<Runnable> runnable;

    Thread(const Thread&);
    const Thread& operator=(const Thread&);
    void setCompleted();
    void* result;   // stores return value of run()
    virtual void* run() { return 0; }

    static void* startThreadRunnable(void* pVoid);
    static void* startThread(void* pVoid);

    void printError(const std::string& msg, int status, const std::string& fileName, int lineNumber);
};

} // namespace DBUtl

#endif /* __DBUTL_THREAD_H__ */
