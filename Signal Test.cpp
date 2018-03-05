//Linuxʵ�ָ��ӽ����ź�ͨ��
#include<iostream>
#include<unistd.h>
#include<signal.h>

int record = 0;

//��¼�ź�
void gotSignal(int signal){
    record = signal;
}

int main(){

    //�����ӽ���
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
	    return 1;
    }

    //�ӽ��̷����ź�
    else if(pid == 0){
	    std::cout<<"Process "<<getpid()<<" send signal: "<<SIGUSR1<<'\n';
        
        //�������߱�֤���������յ��ź�
        sleep(5);

        //�����ź�SIGUSR1
	kill(getppid(), SIGUSR1);
	return 0;
    }

    //�����̽����ź�SIGUSR1
    signal(SIGUSR1, gotSignal);

    //��������ֻ���յ��ź�
    pause();

    std::cout<<"Process "<<getpid()<<" get signal: "<<record<<'\n';
    return 0;
}

