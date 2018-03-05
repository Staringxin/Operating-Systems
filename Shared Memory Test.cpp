//Linuxʵ�ָ��ӽ��̹���洢����ʽͨ��
//ʹ��SYSTEM V�ź���ʵ�ֶԹ���洢���Ļ������
#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

//����洢����С
#define BUFFER_SIZE 1024

//semun�ṹ�壬��semctl������ʹ��
union semun{

    //����SETVALʱָ���ź�����ֵ
    int val;                

    //����IPC_STATʱ��ȡ�ź����������ݽṹsemid_ds
    //����IPC_SETʱ����semid_ds�е�Ԫ��ipc_perm
    semid_ds *buf;          

    //����SETALL��GETALLʱ���á���ȡ�ź�����������ֵ
    unsigned short *array;  
};

//�ź�����ʶ��
static int sem_id = 0;

//��ʼ���ź���
static int set_sem(){
    semun sem_union;
    sem_union.val = 1;
    return semctl(sem_id, 0, SETVAL, sem_union);
}

//ɾ���ź���
static int del_sem(){
    semun sem_union;
    return semctl(sem_id, 0, IPC_RMID, sem_union);
}

//���ź���
static int sem_p(){
    sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_op = -1;
    buffer.sem_flg = SEM_UNDO;
    return semop(sem_id, &buffer, 1);
}

//�ر��ź���
static int sem_v(){
    sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_op = 1;
    buffer.sem_flg = SEM_UNDO;
    return semop(sem_id, &buffer, 1);
}

int main(){
    char buffer[BUFFER_SIZE];
    char *shmaddr = NULL;
    int shmid;
    
    //�����ź���
    sem_id = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    if(sem_id == -1){
	std::cerr<<"Semget failed."<<'\n';
	return -1;
    }

    //��ʼ���ź���
    if(set_sem() == -1){
	std::cerr<<"Set_sem failed."<<'\n';
	return -1;
    }

    //���������ڴ���
    //����IPC_PRIVATE������ǰ���̵�˽�й������
    //0666Ȩ�޴���rw-rw-rw-
    shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666);
    if(shmid == -1){
        std::cerr<<"Shmget failed."<<'\n';
        return 1;
    }

    //�����ӽ���
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
        return 1;
    }

    //�ӽ���д����Ϣ
    else if(pid == 0){

        //��д����Ϣ
        char message[] = "This is subprocess.";
        
        //ӳ�乲���ڴ棬���������ڴ�ӳ�䵽�ڴ�ռ���
        //�ڶ�����������NULL��ʾϵͳ�Զ������ַ
        //��������������0�������ڴ�ɶ�д
        shmaddr = (char*)(shmat(shmid, NULL, 0));
        if(shmaddr == (void*)-1){
            std::cerr<<"Shmat failed."<<'\n';
            return 1;
        }

        //���ź���
        if(sem_p() == -1){
	    std::cerr<<"Sem_p failed."<<'\n';
	    return 1;
	}

        //�����ڴ���д����Ϣ
        memcpy(shmaddr, message, strlen(message)+1);
        
        //�ر��ź���
        if(sem_v() == -1){
	    std::cerr<<"Sem_v failed."<<'\n';
	    return 1;
	}

        std::cout<<"Subprocess send: "<<message<<'\n';
        
        //�ӽ����˳�ʱ�Զ����������ڴ�ӳ��
        return 0;
    }

    //�����̶�ȡ��Ϣ
    else{

        //���������߱�֤�ӽ�����д����Ϣ
        sleep(5);

        //ӳ�乲���ڴ�
        shmaddr = (char*)(shmat(shmid, NULL, 0));
        if(shmaddr == (void*)-1){
            std::cerr<<"Shmat failed."<<'\n';
            return 1;
        }

        //���ź���
        if(sem_p() == -1){
	    std::cerr<<"Sem_p failed."<<'\n';
	    return 1;
	}

        //�ӹ����ڴ�����ȡ��Ϣ
        memcpy(buffer, shmaddr, strlen(shmaddr) + 1);

        //�ر��ź���
        if(sem_v() == -1){
	    std::cerr<<"Sem_v failed."<<'\n';
	    return 1;
	}

        std::cout<<"Parent process receive: "<<buffer
    <<'\n';

        //���������ڴ�ӳ��
        if(shmdt(shmaddr) == -1){
            std::cerr<<"Shmdt failed."<<'\n';
            return 1;
        }

        //ɾ�������ڴ�
        //����IPC_RMID�������ڴ������Ϊɾ����
        //ʵ��ɾ�����������һ�������˳���
        if(shmctl(shmid, IPC_RMID, NULL) == -1){
            std::cerr<<"Shmctl failed."<<'\n';
            return 1;
        }

        //ɾ���ź���
        if(del_sem() == -1){
	    std::cerr<<"Del_sem failed."<<'\n';
	    return 1;
	}

        return 0;
    }
}

    
    

