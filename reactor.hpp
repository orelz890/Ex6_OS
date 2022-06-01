#include <iostream>
#include <pthread.h>


class Reactor
{
public:
    int fd;
    pthread_t t_id;
    void* (*func)(void*);

    Reactor();
    Reactor(int _fd, void*(*f)(void*));
    
    // Reactor& operator=(Reactor& other);
};

void* newReactor();
void InstallHandler(Reactor* p_reactor, void*(func)(void*), int file_des);
void RemoveHandler(Reactor* p_reactor, int fd_free);