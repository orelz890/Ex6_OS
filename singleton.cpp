// Aidded by this site:
// https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template

#include "singleton.hpp"
#include <cassert>
#include <iostream>
#include <pthread.h>

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

template <typename T> Singleton<T>* Singleton<T>::element = NULL;


template <typename T> Singleton<T>::Singleton(T other) : data(other)
{
}


template <typename T> Singleton<T> *Singleton<T>::Instance(T other)
{
    if (element == NULL)
    {
        pthread_mutex_lock(&mutex_lock);
        element = new Singleton(other);
        pthread_mutex_unlock(&mutex_lock);
    }
    return element;
}


template <typename T> void Singleton<T>::Destroy()
{
    delete element;
    element = NULL;
}


// Singalton Test:

int main()
{
    FILE *file;
    Singleton<FILE *> *a = Singleton<FILE *>::Instance(file);
    std::cout << "First file address is: " << a << '\n';
    Singleton<FILE *> *b = Singleton<FILE *>::Instance(file);
    std::cout << "Second file address is: " << b << '\n';
    assert(a == b);
    std::cout << "The test was a success!!!\n";

    return 0;
}