//Linux实现父子进程通信管道进行通信
#include<iostream>
#include<unistd.h>
#include<cstring>

//缓冲区大小
#define BUFFER_SIZE 1024

int main(){

    //文件描述符
    int filedes[2];

    char readbuffer[BUFFER_SIZE];

    //建立通信管道并获取读写文件描述符
    if(pipe(filedes) == -1){
        std::cerr<<"Create pipe failed."<<'\n';
	return 1;
    }

    //创建子进程
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
	    return 1;
    }

    //子进程写信息
    else if(pid == 0){

        //待发送信息
        char message[] = "This is subprocess.";

        //关闭读文件描述符
        close(filedes[0]);
        
	    std::cout<<"Subprocess send message: "<<message
    <<'\n';

        //写信息
        write(filedes[1], message, strlen(message));
        
	    return 0;
    }

    //父进程读信息
    else{

        //关闭写描述符
        close(filedes[1]);

        //读信息，并获取读取信息的字节数
        int num_bytes = read(filedes[0], readbuffer, BUFFER_SIZE);
        
        std::cout<<"Parent process receive "<<num_bytes
    <<" bytes "<<'\n';
        return 0;
    }
}
    
