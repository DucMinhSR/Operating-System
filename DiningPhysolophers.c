#include<stdio.h>
#include<sys/wait.h>
#include<pthread.h>
#include<semaphore.h>


#define quantity 5
#define thinking 2
#define hungry 1
#define eating 0
#define Left (phnum + 4) % quantity// person left beside
#define Right (phnum + 1 ) % quantity// person right beside


int state[quantity];
int phil[quantity]= {0,1,2,3,4};

sem_t mutex;
sem_t S[quantity];


void test(int phnum){
	if(state[phnum]==hungry && state[Left]!= eating && state[Right]!= eating){
		state[phnum] = eating;

		sleep(2);
		printf("Physolopher %d take fork %d and %d!\n",phnum+1,Left+1,phnum+1);
		printf("Physolopher %d is eating!\n",phnum+1 );

		sem_post(&S[phnum]);
	}
}

void take_fork(int phnum){ //phil take chopsticks
	sem_wait(&mutex);
	state[phnum] = hungry;// speak hungry
	printf("Philosopher %d is hungry!\n",phnum+1 );
	//if beside no eating 
	test(phnum);
	sem_post(&mutex);
	//if can't eat , please wait to signable
	sem_wait(&S[phnum]);
	sleep(1);
}
void put_fork(int phnum){
	sem_wait(&mutex);
	state[phnum] = thinking;

	printf("Physolopher %d putting fork %d and %d!\n",phnum+1, Left+1 , phnum+1 );
	printf("Phisolopher %d is thinking!\n", phnum+1); 

	test(Left);
	test(Right);
	sem_post(&mutex);
}

void* physolopher(void *num){
	while(1){
		int* i = num ; 
		sleep(1);
		take_fork(*i);
		sleep(0);
		put_fork(*i);//
	}
}

int main(int argc,char* argv[]){
	int i;
	pthread_t thread_array[quantity];
	sem_init(&mutex,0,1); // create semaphores

	for(i =0;i< quantity;i++){
		sem_init(&S[i],0,0);
	}
	for ( i = 0; i < quantity; ++i)
	{
		pthread_create(&thread_array[i],NULL,physolopher,(void*)&phil[i]);
		printf("Physolopher %d is thinking!\n",i+1);
	}
	for(i=0;i<quantity;i++){
		pthread_join(thread_array[i],NULL);
	}
}