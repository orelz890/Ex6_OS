#pragma once

#include <iostream>
// #include <string>


typedef struct node
{
    void* data;
    node* next;
    node* prev;
    int _fd;

    node(void* new_data, int fd)
    {
        this->data = new_data;
        this->next = NULL;
        this->prev = NULL;
        this->_fd = fd;
    }

    node(void* new_data, node* prev_node, node* next_node, int fd)
    {
        this->data = new_data;
        this->prev = prev_node;
        this->next = next_node;
        this->_fd = fd; 
    }
}node;


class my_Queue
{
private:
    pthread_mutex_t my_mutex;
    pthread_cond_t my_cond;
    node* head;
    node* tail;
    int size;
    
public:
    
    my_Queue();
    ~my_Queue();
    void* enqueue(void* data, int fd);
    std::pair<void*,int>* dequeue();
    bool empty();
};

my_Queue* createQ();

void destroyQ(void* q);

void* enQ(void* data, void* q, int fd);

std::pair<void*,int>* deQ(void* q);
