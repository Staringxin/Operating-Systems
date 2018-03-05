//�������������ź���ʵ��
//���⼢��
#include<pthread.h>
#include<semaphore.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#define TIME_CROSS 4

unsigned tec = 0;     //�������е�������
unsigned twc = 0;     //���������е�������
sem_t sem_tec;
sem_t sem_twc;
sem_t sem_quene;
sem_t sem_print;
sem_t sem_rope;
pthread_t baboon[10];


//��Խ��ɽ��
//����̺߳źͷ���
void to_east(){
	sem_wait(&sem_print);
	std::cout<<pthread_self()<<"    to East"<<'\n';
	sem_post(&sem_print);
}


//����Խ��ɽ��
//����̺߳źͷ���
void to_west(){
	sem_wait(&sem_print);
	std::cout<<pthread_self()<<"    to West"<<'\n';
	sem_post(&sem_print);
}


//���е�����
void *baboon_to_east(void *b){

	//ͬʱֻ�ܵ����߳��������ȴ�����
	//������뵱ǰ����ͨ�з���ͬ�������������ȴ����У�
	//������������������뵱ǰ����ͨ�з�����ͬ��������
	//���⼢������
	sem_wait(&sem_quene);

	//ͬʱֻ�ܵ����̲߳���tec
	sem_wait(&sem_tec);
	++tec;

	//��һ�����е�������ȡ������ͨ��Ȩ��
	//����������ͬ�������������ֱ��ͨ��
	if(tec == 1)
		sem_wait(&sem_rope);
	sem_post(&sem_tec);
	sem_post(&sem_quene);

	to_east();
	sleep(TIME_CROSS);

	sem_wait(&sem_tec);
	--tec;

	//���������һ�����е�����
	//�ͷŶ�������ͨ��Ȩ��
	//��������������е�������ȡ������ͨ��Ȩ
	if(tec == 0)
		sem_post(&sem_rope);
	sem_post(&sem_tec);
}


//�����е�����
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
	//��ʼ���ź���
	sem_init(&sem_tec, 0, 1);
	sem_init(&sem_twc, 0, 1);
	sem_init(&sem_print, 0, 1);
	sem_init(&sem_rope, 0, 1);
	sem_init(&sem_quene, 0, 1);

	//��������
	for(size_t i = 0; i < 10; ++i){
		if(i % 3 == 0)
			pthread_create(&baboon[i], NULL, baboon_to_west, NULL);
		else
			pthread_create(&baboon[i], NULL, baboon_to_east, NULL);
	}

	//���߳����������߳�ִ����ɺ����
	pthread_join(baboon[8], NULL);
	pthread_join(baboon[9], NULL);

	return 0;
}
