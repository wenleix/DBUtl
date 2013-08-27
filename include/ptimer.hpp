#include <cstdlib>


INLINE void DBUtl::PTimer::start() {
    
    gettimeofday(&tim, NULL);
    real_start = tim.tv_sec + 1.0e-6 * tim.tv_usec;
    
#   ifdef __APPLE__
    getrusage (RUSAGE_SELF, &ru);
    user_start = ru.ru_utime.tv_sec + 1.0e-6 * ru.ru_utime.tv_usec;
    system_start = ru.ru_stime.tv_sec + 1.0e-6 * ru.ru_stime.tv_usec;
#   else
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    user_start = ts.tv_sec + 1.0e-9 * ts.tv_nsec;
    system_start = 0.0;
#   endif
    
}


INLINE void DBUtl::PTimer::finish() {
    
    gettimeofday(&tim, NULL);
    real_end = tim.tv_sec + 1.0e-6 * tim.tv_usec;
    
#   ifdef __APPLE__
    getrusage (RUSAGE_SELF, &ru);
    user_end = ru.ru_utime.tv_sec + 1.0e-6 * ru.ru_utime.tv_usec;
    system_end = ru.ru_stime.tv_sec + 1.0e-6 * ru.ru_stime.tv_usec;
#   else
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    user_end = ts.tv_sec + 1.0e-9 * ts.tv_nsec;
    system_end = 0.0;
#   endif
    
}


INLINE double DBUtl::PTimer::realResult() {
    return real_end - real_start;
}


INLINE double DBUtl::PTimer::userResult() {
    return user_end - user_start;
}


INLINE double DBUtl::PTimer::systemResult() {
    return system_end - system_start;
}

