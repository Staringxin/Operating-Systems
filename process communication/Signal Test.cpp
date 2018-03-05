//Linux实现父子进程信号通信
#include<iostream>
#include<unistd.h>
#include<signal.h>

int record = 0;

//记录信号
void gotSignal(int signal){
    record = signal;
}

int main(){

    //创建子进程
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
	    return 1;
    }

    //子进程发送信号
    else if(pid == 0){
	    std::cout<<"Process "<<getpid()<<" send signal: "<<SIGUSR1<<'\n';
        
        //进程休眠保证父进程能收到信号
        sleep(5);

        //发送信号SIGUSR1
	kill(getppid(), SIGUSR1);
	return 0;
    }

    //父进程接收信号SIGUSR1
    signal(SIGUSR1, gotSignal);

    //进程阻塞只到收到信号
    pause();

    std::cout<<"Process "<<getpid()<<" get signal: "<<record<<'\n';
    return 0;
}

