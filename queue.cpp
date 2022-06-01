#include "queue.hpp"


my_Queue::my_Queue()
{
    this->head = NULL;
    this->tail = NULL;
    this->size = 0;
    pthread_mutex_init(&this->my_mutex, NULL);
    pthread_cond_init(&this->my_cond, NULL);
}


my_Queue::~my_Queue()
{
    node* temp = NULL;
    while (this->head != NULL)
    {
        this->dequeue();
    }
    this->tail = NULL;
    pthread_cond_broadcast(&this->my_cond);
    pthread_cond_destroy(&this->my_cond);
    pthread_mutex_destroy(&this->my_mutex);
}


void my_Queue::enqueue(void* data)
{
    pthread_mutex_lock(&this->my_mutex);
    if (this->head == NULL)
    {
        node* new_node = new node(data);
        this->head = new_node;
        this->tail = new_node;
    }
    else
    {
        node* new_node = new node(data,this->tail,NULL);
        this->tail->next = new_node;
        this->tail = this->tail->next;
    }
    this->size++;
    pthread_mutex_unlock(&this->my_mutex);
    pthread_cond_signal(&this->my_cond);
}


void* my_Queue::dequeue()
{
    pthread_mutex_lock(&this->my_mutex);
    if (this->tail != NULL)
    {
        node* temp = this->tail->prev;
        void* data = this->tail->data;
        delete this->tail;
        this->size--;
        this->tail = temp;
        if (this->tail != NULL)
        {
            this->tail->next = NULL;
        }
        else
        {
            this->head = NULL;
        }
        pthread_mutex_unlock(&this->my_mutex);
        pthread_cond_signal(&this->my_cond);
        return data;
    }
    pthread_mutex_unlock(&this->my_mutex);
    pthread_cond_signal(&this->my_cond);
    return NULL;
}

bool my_Queue::empty()
{
    return this->size == 0;
}


my_Queue* createQ()
{
    return new my_Queue();
}


void destroyQ(void* q)
{
    my_Queue* queue = (my_Queue*)q; 
    queue->~my_Queue();
    delete queue;
}


void enQ(void* data, void* q)
{
    ((my_Queue*)q)->enqueue((node*)data);
}


void* deQ(void* q)
{
    return ((my_Queue*)q)->dequeue();
}
