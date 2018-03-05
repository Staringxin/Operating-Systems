//狒狒过谷问题信号量实现
//避免饥饿
#include<pthread.h>
#include<semaphore.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#define TIME_CROSS 4

unsigned tec = 0;     //期望向东行的狒狒数
unsigned twc = 0;     //期望向西行的狒狒数
sem_t sem_tec;
sem_t sem_twc;
sem_t sem_quene;
sem_t sem_print;
sem_t sem_rope;
pthread_t baboon[10];


//向东越过山谷
//输出线程号和方向
void to_east(){
	sem_wait(&sem_print);
	std::cout<<pthread_self()<<"    to East"<<'\n';
	sem_post(&sem_print);
}


//向西越过山谷
//输出线程号和方向
void to_west(){
	sem_wait(&sem_print);
	std::cout<<pthread_self()<<"    to West"<<'\n';
	sem_post(&sem_print);
}


//向东行的狒狒
void *baboon_to_east(void *b){

	//同时只能单个线程申请进入等待队列
	//如果有与当前绳索通行方向不同的狒狒申请进入等待队列，
	//则会阻塞接下来所有与当前绳索通行方向相同的狒狒，
	//避免饥饿问题
	sem_wait(&sem_quene);

	//同时只能单个线程操作tec
	sem_wait(&sem_tec);
	++tec;

	//第一个向东行的狒狒获取绳索的通行权，
	//队列内所有同方向的狒狒可以直接通过
	if(tec == 1)
		sem_wait(&sem_rope);
	sem_post(&sem_tec);
	sem_post(&sem_quene);

	to_east();
	sleep(TIME_CROSS);

	sem_wait(&sem_tec);
	--tec;

	//队列中最后一个向东行的狒狒
	//释放对绳索的通行权，
	//允许队列中向西行的狒狒获取绳索的通行权
	if(tec == 0)
		sem_post(&sem_rope);
	sem_post(&sem_tec);
}


//向西行的狒狒
void *baboon_to_west(void *b){
	sem_wait(&sem_quene);
	sem_wait(&sem_twc);
	++twc;
	if(twc == 1)
		 sem_wait(&sem_rope);
	sem_post(&sem_twc);
	sem_post(&sem_quene);

	to_west();
	sleep(TIME_CROSS);

	sem_wait(&sem_twc);
	--twc;
	if(twc == 0)
		sem_post(&sem_rope);
	sem_post(&sem_twc);
}


int main(){
	//初始化信号量
	sem_init(&sem_tec, 0, 1);
	sem_init(&sem_twc, 0, 1);
	sem_init(&sem_print, 0, 1);
	sem_init(&sem_rope, 0, 1);
	sem_init(&sem_quene, 0, 1);

	//测试序列
	for(size_t i = 0; i < 10; ++i){
		if(i % 3 == 0)
			pthread_create(&baboon[i], NULL, baboon_to_west, NULL);
		else
			pthread_create(&baboon[i], NULL, baboon_to_east, NULL);
	}

	//主线程在所有子线程执行完成后结束
	pthread_join(baboon[8], NULL);
	pthread_join(baboon[9], NULL);

	return 0;
}
