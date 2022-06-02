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


void* my_Queue::enqueue(void* data, int fd)
{
    pthread_mutex_lock(&this->my_mutex);
    printf("trying to enq: %s\n",(char*)data);
    if (this->head == NULL)
    {
        node* new_node = new node(data,fd);
        this->head = new_node;
        this->tail = new_node;
    }
    else
    {
        node* new_node = new node(data,this->tail,NULL,fd);
        this->tail->next = new_node;
        this->tail = this->tail->next;
    }
    this->size++;
    pthread_mutex_unlock(&this->my_mutex);
    pthread_cond_signal(&this->my_cond);
    return data;
}


std::pair<void*,int>* my_Queue::dequeue()
{
    pthread_mutex_lock(&this->my_mutex);
    if (this->tail != NULL)
    {
        node* temp = this->tail->prev;
        void* data = this->tail->data;
        int fd = this->tail->_fd;
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
        return new std::pair<void*,int>{data, fd};
    }
    pthread_mutex_unlock(&this->my_mutex);
    pthread_cond_signal(&this->my_cond);
    return new std::pair<void*,int>{NULL, -1};
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


void* enQ(void* data, void* q, int fd)
{
    return ((my_Queue*)q)->enqueue((node*)data,fd);
}


std::pair<void*,int>* deQ(void* q)
{
    return ((my_Queue*)q)->dequeue();
}
