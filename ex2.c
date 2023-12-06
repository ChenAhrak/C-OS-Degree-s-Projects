#include <pthread.h>
#include <stdio.h>

int arr[99];
pthread_t th[5];
int sum1;
int sum2;
int sum3;


void* foo(void* arg){
	int idx=(int)arg;
	int j,sum=0;
	int start=idx*33;
	
	for(j=start;j<start+33;j++)
	{
		sum+=arr[j];
		
	}
	
	if(idx==0)
		sum1=sum;
	else if(idx==1)
		sum2=sum;
	else
		sum3=sum;
	
	return ((void*)0);
}

int main(){
	
	int i;
	sum1=0;
	sum2=0;
	sum3=0;
	
	for(i=0;i<99;i++)
	{
		arr[i]=i;
	}
	
	for(i=0;i<3;i++){
		pthread_create(&th[i],NULL,foo,(void*)(i)); //Need to check for error
	}
	
	for(i = 0; i < 3; i++)
		pthread_join(th[i], NULL);
	
	printf("I'm main thread, I'm here ... \n");
	printf("sum1=%d\n",sum1);
	printf("sum2=%d\n",sum2);
	printf("sum3=%d\n",sum3);
	
	printf("total=%d\n",sum1+sum2+sum3);

	return 0;
	
}