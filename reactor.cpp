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


void InstallHandler(void* p_r, void*(f)(void*), int _fd)
{
    Reactor* pr = (Reactor*)p_r;
    pr->fd = _fd;
    pr->func = f;
    pthread_create(&pr->t_id, NULL, f, p_r);
}


void RemoveHandler(void* p_r)
{
    Reactor* pr = (Reactor*)p_r;
    // Make calling thread wait for this thread termination
    pthread_join(pr->t_id, NULL);
    pr->fd = -1;
    pr->func = NULL;
}
