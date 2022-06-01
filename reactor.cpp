#include "reactor.hpp"
#include <iostream>
#include <pthread.h>



Reactor::Reactor() : fd(-1), func(NULL)
{
}


Reactor::Reactor(int _fd, void*(*f)(void*)) : fd(_fd), func(f)
{
    pthread_create(&(this->t_id), NULL, this->func,this);
}

void* newReactor()
{
    return new Reactor();
}


void InstallHandler(Reactor* p_r, void*(f)(void*), int file_descriptor);
void RemoveHandler(Reactor* p_r, int _fd);