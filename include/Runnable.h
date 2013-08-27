#ifndef __DBUTL_RUNNABLE_H__
#define __DBUTL_RUNNABLE_H__

namespace DBUtl {

class Runnable {
public:
    virtual void* run() = 0;
    virtual ~Runnable() = 0;
};

} // namespace DBUtl

#endif /* __DBUTL_RUNNABLE_H__ */

