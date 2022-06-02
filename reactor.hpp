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
void InstallHandler(void* p_reactor, void*(func)(void*), int _fd);
void RemoveHandler(void* p_reactor);