
#include "Guard.hpp"
#include<stdio.h>
#include <iostream>

Guard::Guard(){

    if (pthread_mutex_init(&safe_lock, NULL) != 0)
    {
        printf("\n mutex failed\n");
        exit(1);
    }

}


Guard::~Guard(){
    pthread_mutex_destroy(&safe_lock);
    printf("destroyed!!!\n");
    fflush(stdout);
}


void Guard::lock()
{
    pthread_mutex_lock(&this->safe_lock);
    printf("locked!!!\n");
}


void Guard::unlock()
{
    pthread_mutex_unlock(&this->safe_lock);
    printf("unlocked!!!\n");
}


// Answer to the second quastion:
// https://stackoverflow.com/questions/20820937/strtok-function-and-multithreading

// In a multithreaded environment, this is dangerous because many threads
// may be calling strtok with different strings.
// It will only remember the last one and return the wrong result.