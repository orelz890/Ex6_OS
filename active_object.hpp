#include "queue.hpp" 


void* handleAO(void* ao);


class AO {

public:
    pthread_t t_id;
    bool is_active;
    my_Queue* Q;
    void* (*f1)(void*);
    void* (*f2)(void*);

    AO(my_Queue* q, void* (*func1)(void*), void* (*func2)(void*));
};

AO* newAO(void* q, void* (*f1)(void*), void* (*f2)(void*));

void destroyAO(void* ao);
