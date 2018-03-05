//����д�����⣬д������ʵ��
#include<pthread.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>

#define TIME_READ 1
#define TIME_WRITE 5
#define TIME_REST 5

pthread_mutex_t mutex_rc = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_r = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t db = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_w = PTHREAD_MUTEX_INITIALIZER;
pthread_t readers[6];
pthread_t writers[2];
unsigned rc = 0;
int data = 0;

//ģ�������
void read_data_base(){
    pthread_mutex_lock(&mutex_r);
    std::cout<<"Read "<<data<<std::endl;
    pthread_mutex_unlock(&mutex_r);
}

//ģ��д����
void write_data_base(){
    data = rand()%100;
    std::cout<<"Write "<<data<<std::endl;
}

//���߽���
void* reader(void* r){
    while(true){

		//��ֹ��data��дȨ��
		pthread_mutex_lock(&mutex_w);

		//��ȡ��rc�Ļ������Ȩ
		pthread_mutex_lock(&mutex_rc);

		//��������һ
		++rc;

		//��Ϊ�ȴ������еĵ�һ�����ߣ�
                //��ȡ��data�Ļ������Ȩ
		if(rc == 1) pthread_mutex_lock(&db);

		//�ͷŶ�rc�Ļ������Ȩ
		pthread_mutex_unlock(&mutex_rc);

		//���Ŷ�data��дȨ��
		pthread_mutex_unlock(&mutex_w);

		read_data_base();
		sleep(TIME_READ);

		//��ȡ��rc�Ļ������Ȩ
		pthread_mutex_lock(&mutex_rc);

		//��������һ
		--rc;

		//��Ϊ�ȴ����������һ�����ߣ�
                //�ͷŶ�data�Ļ������Ȩ
		if(rc == 0) pthread_mutex_unlock(&db);

		//�ͷŶ�rc�Ļ������Ȩ
		pthread_mutex_unlock(&mutex_rc);

		sleep(TIME_REST);
    }
}

//д�߽���
void* writer(void* w){
    while(true){

		//��ȡ��data��дȨ��
		pthread_mutex_lock(&mutex_w);

		//��ȡ��data�Ļ������Ȩ
		pthread_mutex_lock(&db);

		write_data_base();
		sleep(TIME_READ);

		//�ͷŶ�data�Ļ������Ȩ
		pthread_mutex_unlock(&db);

		pthread_mutex_unlock(&mutex_w);

		sleep(TIME_REST);
    }
}

int main(){

    //���ɶ�д����
    pthread_create(&writers[0], NULL, writer, NULL);
    for(unsigned i = 0;i < 3;++i)
	pthread_create(&readers[i], NULL, reader, NULL);
    pthread_create(&writers[1], NULL, writer, NULL);
    for(unsigned i = 3;i < 6;++i)
	pthread_create(&readers[i], NULL, reader, NULL);

    //���̱߳������߲��˳�
    while(true)
	sleep(10);
    return 0;
}
     	
