//死锁检测
#include<iostream>


unsigned k, m, n;
unsigned resource[100];
unsigned demand[100][100];
unsigned occupy[100][100];
unsigned seq[100];


//判断当前资源能否满足进程id的需求
bool isSatisfied(unsigned id){
    for(unsigned i = 0;i < n;++i)
	if(resource[i] < demand[id][i] - occupy[id][i])
            return false;
    return true;
}


//将进程id所占有的资源释放
void release(unsigned id){
    for(unsigned i = 0;i < n;++i)
	resource[i] += occupy[id][i];
}


//交换seq数组中的两项
void swap(unsigned *seq, unsigned i, unsigned j){
    if(i == j) return;

    unsigned tmp = seq[i];
    seq[i] = seq[j];
    seq[j] = tmp;
}


//死锁检测主函数
//遍历所有未释放资源的进程，
//若现有资源能满足其最大需求，
//则释放该进程所占有的资源	
bool detect(){
    for(unsigned i = 0;i < m;++i)
	for(unsigned j = 0;j < m - i;++j){
	    if(isSatisfied(seq[j])){
		release(seq[j]);
		swap(seq, j, m - i - 1);
		break;
	    }
	    if(j == m - i - 1) return false;
	}
    return true;
}


int main(){
    //输入每种资源的数量k，
    //进程数m和资源种数n
    std::cout<<"Please input k, m and n"
<<"(separated by spaces)"<<'\n';
    std::cin>>k>>m>>n;
    for(unsigned i = 0;i < n;++i)
	resource[i] = k;
    for(unsigned i = 0;i < m;++i)
	seq[i] = i;

    //输入每个进程占用每个资源的数目
    std::cout<<"Please input the occupy resource"
<<"(separated by spaces)"<<'\n';
    for(unsigned i = 0;i < m;++i)
	for(unsigned j = 0;j < n;++j){
            std::cin>>occupy[i][j];
	    if(occupy[i][j] > resource[j]){
		std::cerr<<"Data wrong."<<'\n';
		return 1;
	    }
	    resource[j] -= occupy[i][j];
	}
    
    //输入每个进程对每个资源的最大需求数目
    std::cout<<"Please input the total demand"
<<"(separated by spaces)"<<'\n';
    for(unsigned i = 0;i < m;++i)
	for(unsigned j = 0;j < n;++j){
            std::cin>>demand[i][j];
	    if(demand[i][j] < occupy[i][j]){
		std::cerr<<"Data wrong."<<'\n';
		return 1;
	    }
	}
    
    //输出检测结果
    if(detect())
	std::cout<<"Safe"<<'\n';
    else 
	std::cout<<"Deadlock"<<'\n'; 
	
    return 0;
}
