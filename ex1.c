#include <pthread.h>
#include <stdio.h>

int cnt;
pthread_t th[5];

void* foo(void* arg){
	int j;
	
	for(j=0;j<10;j++)
	{
		cnt++;
		printf("I'm thread %ld, cnt=%d \n",pthread_self(),cnt);
	}
	
	return ((void*)0);
}

int main()
{
	int i;
	cnt=0;
	
	for(i=0;i<5;i++)
	{
		pthread_create(&th[i],NULL,foo,NULL); //Need to check for error
	}
	
	for(i=0;i<5;i++)
	{
		pthread_join(th[i],NULL);
	}
	
	return 0;
}
