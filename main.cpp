// How to run:
// g++ -pthread -o a.out queue.hpp queue.cpp main.cpp active_object.hpp active_object.cpp Guard.hpp
// #include "queue.hpp"
#include "active_object.hpp"
#include <ctype.h>

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

void* f1(void* data){
    char* str = (char*)data;
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
            else
            {
                throw std::runtime_error("Input error!");
            }
        }
        str += 1;
    }
    return data;
}

void* f2(void* data){

    char* str = (char*)data;
    for (int j = 0; j < sizeof(str)-1; j++)
    {
        str[j] = toupper(str[j]);
    }
    printf("f2 = %s\n", (char*)data);
    fflush(stdout);
    return data;
}


int main()
{
    my_Queue q{};
    char a[1024] = "aaa";
    char b[1024] = "BBB";

    // enQ(a,&q);
    // char* de = (char*)deQ(&q);
    // std::cout << '\n' << de << '\n';
    // de = (char*)deQ(&q);
    // if (de == NULL)
    // {
    //     std::cout << "\nElement dequeued!\n";
    // }
    // enQ(a,&q);
    // de = (char*)deQ(&q);
    // std::cout << '\n' << de << '\n';

    enQ(a,&q);
    enQ(b,&q);
    AO* ao = newAO(&q,&f1,&f2);
    
    // destroyAO(ao);
}
