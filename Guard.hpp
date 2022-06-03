// #pragma once
#include <pthread.h>


class Guard
{
private:
    pthread_mutex_t safe_lock;

public:
    Guard();
    
    ~Guard();

    void lock();

    void unlock();
};
