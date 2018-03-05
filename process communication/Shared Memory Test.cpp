//Linux实现父子进程共享存储区方式通信
//使用SYSTEM V信号量实现对共享存储区的互斥访问
#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

//共享存储区大小
#define BUFFER_SIZE 1024

//semun结构体，在semctl函数中使用
union semun{

    //传入SETVAL时指定信号量的值
    int val;                

    //传入IPC_STAT时读取信号量集的数据结构semid_ds
    //传入IPC_SET时设置semid_ds中的元素ipc_perm
    semid_ds *buf;          

    //传入SETALL或GETALL时设置、获取信号量集的所有值
    unsigned short *array;  
};

//信号量标识符
static int sem_id = 0;

//初始化信号量
static int set_sem(){
    semun sem_union;
    sem_union.val = 1;
    return semctl(sem_id, 0, SETVAL, sem_union);
}

//删除信号量
static int del_sem(){
    semun sem_union;
    return semctl(sem_id, 0, IPC_RMID, sem_union);
}

//打开信号量
static int sem_p(){
    sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_op = -1;
    buffer.sem_flg = SEM_UNDO;
    return semop(sem_id, &buffer, 1);
}

//关闭信号量
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
    
    //创建信号量
    sem_id = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    if(sem_id == -1){
	std::cerr<<"Semget failed."<<'\n';
	return -1;
    }

    //初始化信号量
    if(set_sem() == -1){
	std::cerr<<"Set_sem failed."<<'\n';
	return -1;
    }

    //创建共享内存区
    //传入IPC_PRIVATE创建当前进程的私有共享进程
    //0666权限代表rw-rw-rw-
    shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666);
    if(shmid == -1){
        std::cerr<<"Shmget failed."<<'\n';
        return 1;
    }

    //创建子进程
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
        return 1;
    }

    //子进程写入信息
    else if(pid == 0){

        //待写入信息
        char message[] = "This is subprocess.";
        
        //映射共享内存，即将共享内存映射到内存空间内
        //第二个参数传入NULL表示系统自动分配地址
        //第三个参数传入0代表共享内存可读写
        shmaddr = (char*)(shmat(shmid, NULL, 0));
        if(shmaddr == (void*)-1){
            std::cerr<<"Shmat failed."<<'\n';
            return 1;
        }

        //打开信号量
        if(sem_p() == -1){
	    std::cerr<<"Sem_p failed."<<'\n';
	    return 1;
	}

        //向共享内存区写入信息
        memcpy(shmaddr, message, strlen(message)+1);
        
        //关闭信号量
        if(sem_v() == -1){
	    std::cerr<<"Sem_v failed."<<'\n';
	    return 1;
	}

        std::cout<<"Subprocess send: "<<message<<'\n';
        
        //子进程退出时自动撤销共享内存映射
        return 0;
    }

    //父进程读取信息
    else{

        //父进程休眠保证子进程先写入信息
        sleep(5);

        //映射共享内存
        shmaddr = (char*)(shmat(shmid, NULL, 0));
        if(shmaddr == (void*)-1){
            std::cerr<<"Shmat failed."<<'\n';
            return 1;
        }

        //打开信号量
        if(sem_p() == -1){
	    std::cerr<<"Sem_p failed."<<'\n';
	    return 1;
	}

        //从共享内存区读取信息
        memcpy(buffer, shmaddr, strlen(shmaddr) + 1);

        //关闭信号量
        if(sem_v() == -1){
	    std::cerr<<"Sem_v failed."<<'\n';
	    return 1;
	}

        std::cout<<"Parent process receive: "<<buffer
    <<'\n';

        //撤销共享内存映射
        if(shmdt(shmaddr) == -1){
            std::cerr<<"Shmdt failed."<<'\n';
            return 1;
        }

        //删除共享内存
        //传入IPC_RMID将共享内存区标记为删除，
        //实际删除发生在最后一个进程退出后
        if(shmctl(shmid, IPC_RMID, NULL) == -1){
            std::cerr<<"Shmctl failed."<<'\n';
            return 1;
        }

        //删除信号量
        if(del_sem() == -1){
	    std::cerr<<"Del_sem failed."<<'\n';
	    return 1;
	}

        return 0;
    }
}

    
    

