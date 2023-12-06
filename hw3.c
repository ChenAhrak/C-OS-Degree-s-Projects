#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define CORE 4
#define SIZE 4

// int matrix[SIZE][SIZE] = {
//  {1, 2, 3, 4 , 17 ,18},
//    {5, 6, 7, 8 , 19 ,20},
//     {9, 10, 11, 12 , 21 , 22},
//    {13, 14, 15, 16 , 23, 24} ,
//    {31, 32, 33, 34 , 35, 36} ,
//    {37, 38, 39, 40 , 41, 42} };

int matrix[SIZE][SIZE] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}};

int array[SIZE * SIZE];
int arr_threads[SIZE * SIZE];

int cnt;
pthread_t th[CORE];

void *foo(void *arg)
{

    int index = (int)arg;
    int x = 0;
    int y = 1;
    int j, i;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] % 2 == 0)
            {
                array[x] = matrix[i][j];
                x += 2;
            }
            else // check to change else to if
            {
                array[y] = matrix[i][j];
                y += 2;
            }
        }
    }

    for (i = 0; i < (SIZE * SIZE) / CORE; i++)
    {
        arr_threads[cnt] = index;
        cnt++;
    }

    return ((void *)0);
}

int main()
{
    int i;
    cnt = 0;

    for (i = 0; i < CORE; i++)
    {
        if (pthread_create(&th[i], NULL, foo, (void *)i) != 0)
        {
            printf("Error!!");
            exit(1);
        }
    }

    for (i = 0; i < CORE; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("Error!!");
            exit(1);
        }
    }

    for (i = 0; i < SIZE * SIZE; i++)
    {
        printf("Num %d was found by thread %d \n", array[i], arr_threads[i]);
    }

    return 0;
}