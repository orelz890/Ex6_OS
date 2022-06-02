// #pragma once
#include <pthread.h>
#include<stdio.h>
#include <iostream>
using namespace std;
 
pthread_mutex_t safe_lock;
class Guard {
   
    public:
        Guard() {
            pthread_mutex_lock(&safe_lock);
            printf("locked!!!\n");
        }
        
        ~Guard() {
            pthread_mutex_unlock(&safe_lock);
                        printf("unlocked!!!\n");

        }
};

int I = 0;
void* myFun(void* thread_id)
{
    Guard guard;
    
    for (int i = 0; i < 350; i++)
    {
        cout<< "thread_id: " << *((int*)thread_id) << ", I = " << I++ <<endl;
    }
    cout<< "thread_id: " << *((int*)thread_id) << ", I = " << I <<endl;
    return 0;
}
int main(int argc, char const *argv[])
{
    if (pthread_mutex_init(&safe_lock, NULL) != 0)
    {
        printf("\n mutex failed\n");
        return 1;
    }
    pthread_t t1, t2;
    int tid1 = 1, tid2 = 2;
    pthread_create(&t1, NULL, &myFun, &tid1);
    pthread_create(&t2, NULL, &myFun, &tid2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&safe_lock);
    return 0;
}



// Answer to the second quastion:
// https://stackoverflow.com/questions/20820937/strtok-function-and-multithreading

// In a multithreaded environment, this is dangerous because many threads
// may be calling strtok with different strings.
// It will only remember the last one and return the wrong result.