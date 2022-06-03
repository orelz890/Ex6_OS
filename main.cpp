// How to run:
// g++ -pthread -o a.out queue.hpp queue.cpp main.cpp active_object.hpp active_object.cpp
// #include "queue.hpp"
#include "active_object.hpp"
#include <ctype.h>
#include <unistd.h>


AO* ao1;
AO* ao2;
AO* ao3;

void* fill_Q2(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    return ao2->Q->enqueue(p->first,p->second);
}


void* fill_Q3(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    return ao3->Q->enqueue(p->first,p->second);
}


void* print_data(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    std::cout << p->first;
    delete p;
    return data;
}


void* send_data(void* data){

    return data;
}


void* f1(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    std::cout << "f1-data = " << (char*)p->first << '\n';
    char* str = (char*)p->first;
    while (*str) {
        if (!isspace(*str) || !isblank(*str))
        {
            if (*str >= 'a' && *str <= 'z')
            {
                *str = (((*str - 'a') + 1) % 26) + 'a';
            }
            else if ((*str >= 'A' && *str <= 'Z'))
            {
                *str = (((*str - 'A') + 1) % 26) + 'A';
            }
            else if(*str != '\n')
            {
                throw std::runtime_error("Input error!");
            }
        }
        str += 1;
    }
    // std::cout << "f1-ans = " << (char*)p->first << '\n';
    return data;
}

void* f2(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    // std::cout << "f2-data = " << (char*)p->first << '\n';
    char* str = (char*)p->first;
    for (int j = 0; j < sizeof(str)-1; j++)
    {
        str[j] = toupper(str[j]);
    }
    std::cout << "f2-ans = " << (char*)p->first << '\n';
    fflush(stdout);
    return data;
}


int main()
{
    my_Queue q{};
    char a[1024] = "aaa";
    char b[1024] = "BBB";
    char c[1024];
    char d[1024];

    enQ(a,&q, -1);
    char* de = (char*)deQ(&q);
    std::cout << '\n' << de << '\n';
    de = (char*)deQ(&q);
    if (de == NULL)
    {
        std::cout << "\nElement dequeued!\n";
    }
    enQ(a,&q, -1);
    de = (char*)deQ(&q);
    std::cout << '\n' << de << '\n';

    ao1 = newAO(createQ(),&f1,&fill_Q2);
    ao2 = newAO(createQ(),&f2,&fill_Q3);
    ao3 = newAO(createQ(),&print_data,&send_data);

    // std::cin.getline(c,sizeof(c));
    // std::cin.getline(d,sizeof(d));

    enQ(a,ao1->Q, -1);
    enQ(b,ao1->Q, -1);

    sleep(1);

    destroyAO(ao1);

    ao1 = newAO(createQ(),&f1,&fill_Q2);
    enQ(a,ao1->Q, -1);

    sleep(2);
}
