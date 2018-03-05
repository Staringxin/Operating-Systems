//Linuxʵ�ָ��ӽ���ͨ�Źܵ�����ͨ��
#include<iostream>
#include<unistd.h>
#include<cstring>

//��������С
#define BUFFER_SIZE 1024

int main(){

    //�ļ�������
    int filedes[2];

    char readbuffer[BUFFER_SIZE];

    //����ͨ�Źܵ�����ȡ��д�ļ�������
    if(pipe(filedes) == -1){
        std::cerr<<"Create pipe failed."<<'\n';
	return 1;
    }

    //�����ӽ���
    int pid = fork();
    if(pid == -1){
        std::cerr<<"Fork failed."<<'\n';
	    return 1;
    }

    //�ӽ���д��Ϣ
    else if(pid == 0){

        //��������Ϣ
        char message[] = "This is subprocess.";

        //�رն��ļ�������
        close(filedes[0]);
        
	    std::cout<<"Subprocess send message: "<<message
    <<'\n';

        //д��Ϣ
        write(filedes[1], message, strlen(message));
        
	    return 0;
    }

    //�����̶���Ϣ
    else{

        //�ر�д������
        close(filedes[1]);

        //����Ϣ������ȡ��ȡ��Ϣ���ֽ���
        int num_bytes = read(filedes[0], readbuffer, BUFFER_SIZE);
        
        std::cout<<"Parent process receive "<<num_bytes
    <<" bytes "<<'\n';
        return 0;
    }
}
    
