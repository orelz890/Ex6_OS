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
    pthread_cancel(my_ao->t_id);
    delete my_ao->Q;
    delete my_ao;
    my_ao = NULL;
    printf("\nAO terminated!\n");
}


AO* newAO(void* q, void* (*f1)(void*), void* (*f2)(void*)) {
    AO* ao = new AO((my_Queue*)q, f1, f2);
    return ao;
}


void* handleAO(void* ao) {
    AO* my_ao = (AO*)ao;
    std::pair<void*,int>* de;
    while (my_ao->is_active) {
        if (!my_ao->Q->empty())
        {
            de = deQ(my_ao->Q);
            // std::cout << "de.first = "<< (char*)de->first << '\n';
            // Activate the first func of the AO and send the result to the second func
            my_ao->f2(my_ao->f1(de));
        }
    }
    return ao;
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
    // pthread_join(this->t_id,NULL);
}
