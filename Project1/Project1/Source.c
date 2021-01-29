#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<conio.h>

void* my_Func(void* received_arr)
{

    int* arr = (int*)received_arr;

    for (int i = 0; i < 3; i++)
    {
        printf("Value lf: lf\n", i + 1, arr[i]);
    }
    //Now use arr[] as you wish
}
void main()
{
    pthread_t tid;
    double d[3] = { 3.14,2.5,1.1 };

    pthread_create(&tid, NULL, my_Func, (void*)d);
    pthread_join(tid, NULL);
}

//C program to pass as an integer and double value to a thred.

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<conio.h>

struct param
{
    pthread_t tid;
    int i = 3733;
    double d = 3733.001;
};

void* myfunc(void* p)
{
    // Print value received as argument:
    printf("Value recevied as argument in starting routine: ");
    printf("%i\n", *(int*)p);

    // Return reference to global variable:
    pthread_exit(&i);
}

void main()
{
    // Declare variable for thread's ID:
    pthread_t id;
    struct p;

    pthread_create(&id, NULL, myfunc, &d);

    int* ptr;

    // Wait for myfunc() and retrieve value in ptr;
    pthread_join(id, (void**)&ptr);
    printf("Value recevied by parent from child: ");
    printf("%i\n", *ptr);
    printf("The value of i is:%d", p.i);
    printf("The value of d is %d", p.d);
}