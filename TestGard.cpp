#include "Guard.hpp"
#include <iostream>


Guard guard;
int I = 0;
void* myFun(void* thread_id)
{
    guard.lock();
    for (int i = 0; i < 350; i++)
    {
        std::cout<< "thread_id: " << *((int*)thread_id) << ", I = " << I++ << std::endl;
    }
    std::cout<< "thread_id: " << *((int*)thread_id) << ", I = " << I << std::endl;
    guard.unlock();
    return 0;
}


int main(int argc, char const *argv[])
{
    pthread_t t1, t2;
    int tid1 = 1, tid2 = 2;
    pthread_create(&t1, NULL, &myFun, &tid1);
    pthread_create(&t2, NULL, &myFun, &tid2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
