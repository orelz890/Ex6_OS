// #pragma once
#include <pthread.h>


class Guard
{
private:
    pthread_mutex_t safe_lock;
    bool active = false;

public:
    Guard();
    
    ~Guard();

    void lock();

    void unlock();
};
