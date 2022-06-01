#include <pthread.h>
#include "active_object.hpp"


/*
========================
Functions on a given AO
========================
*/


void destroyAO(void* ao) {
    AO* my_ao = (AO*)ao;
    my_ao->is_active=false;
}


AO* newAO(void* q, void* (*f1)(void*), void* (*f2)(void*)) {
    AO* ao = new AO((my_Queue*)q, f1, f2);
    return ao;
}


void* handleAO(void* ao) {
    printf("Im handling this ao!\n");
    fflush(stdout);
    AO* my_ao = (AO*)ao;
    while (my_ao->is_active) {
        if (!my_ao->Q->empty())
        {
            // Activate the first func of the AO and send the result to the second func
            my_ao->f2(my_ao->f1(deQ(my_ao->Q)));
        }        
    }
    pthread_cancel(my_ao->t_id);
    delete my_ao;
    printf("AO terminated!\n");
    fflush(stdout);
    return NULL;
}


/*
=========================
Class AO implemantation
=========================
*/

AO::AO(my_Queue* q, void* (*func1)(void*), void* (*func2)(void*))
{
    printf("AO thread created\n");
    this->is_active = true;
    this->Q = q;
    this->f1 = func1;
    this->f2 = func2;
    int err = pthread_create(&this->t_id, NULL, handleAO, (void*)this);
    if (err)
    {
        std::cout << "Pthread create error!";
    }
    pthread_join(this->t_id,NULL);
}




// void* f1(void* a)
// {
//     printf("im in f1\n");
//     fflush(stdout);
//     return a;
// }

// void* f2(void* b)
// {
//     printf("im in f2\n");
//     fflush(stdout);
//     return b;
// }

// int main(){
//     my_Queue a{};
//     char aaa[] = {'a','a','a'};
//     a.enqueue(aaa);

//     AO ao{&a,&f1,&f2};
//     handleAO(&ao);

// }

// AO::~AO()
// {
//     delete this->t_id;
//     this->is_active = false;
//     printf("AO terminated!\n");
// }
