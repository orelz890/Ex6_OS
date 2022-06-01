// #pragma once
#include <pthread.h>

class Guard {
    private:
        pthread_mutex_t safe_lock;

    public:
        Guard() {
            pthread_mutex_lock(&this->safe_lock);
        }
        
        ~Guard() {
            pthread_mutex_unlock(&this->safe_lock);
        }
};


// Answer to the second quastion:
// https://stackoverflow.com/questions/20820937/strtok-function-and-multithreading

// In a multithreaded environment, this is dangerous because many threads
// may be calling strtok with different strings.
// It will only remember the last one and return the wrong result.