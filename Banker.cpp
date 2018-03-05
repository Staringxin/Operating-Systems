//银行家算法
#include<iostream>
#include<fstream>


unsigned n, m;
unsigned resource[10];
unsigned occupy[10][10];
unsigned demand[10][10];


////判断当前资源能否满足进程id的需求
bool isSatisfied(unsigned id, unsigned *res){
    for(unsigned i = 0;i < n;++i)
	if(res[i] < (demand[id][i] - occupy[id][i]))
	    return false;
    return true;
}


//将进程id所占有的资源释放
void release(unsigned id, unsigned *res){
    for(unsigned i = 0;i < n;++i)
	res[i] += occupy[id][i];
}


//交换seq数组中的两项
void swap(unsigned *seq, unsigned i, unsigned j){
    if(i == j) return;

    unsigned tmp = seq[i];
    seq[i] =  seq[j];
    seq[j] = tmp;
}


//检测当前状态是否安全
bool detect(){
    bool result = true;
    unsigned *res = new unsigned[n]();
    for(unsigned i = 0;i < n;++i)
	res[i] = resource[i];
    unsigned *seq = new unsigned[m]();
    for(unsigned i = 0;i < m;++i)
	seq[i] = i;

    for(unsigned i = 0;i < m;++i)
	for(unsigned j = 0;j < m - i;++j){
            if(isSatisfied(seq[j], res)){
		release(seq[j], res);
		swap(seq, j, m - i - 1);
		break;
	    }
	    if(j == m - i - 1) result = false;
	}
	
    delete []res;
    delete []seq;
    return result;
}


//银行家算法主函数
int banker(){
    //打开输出文件
    std::ofstream os;
	os.open("output.txt", std::ofstream::out);
    
    if(!os.is_open()){
	std::cerr<<"File open failed."<<'\n';
	return 1;
    }
	
    //输入客户请求
    std::cout<<"Please input the request: "<<
"'client_id res_id res_num'"<<"(exit with '-1')"<<'\n';
    int client_id, res_id, res_num;
    while(std::cin>>client_id){

        if(client_id == -1){
	    std::cout<<"Process exit."<<'\n';
	    break;
	}

	//判断输入是否合法
	std::cin>>res_id>>res_num;
        if(client_id >= m){
	    std::cout<<"Client_id is over range."<<'\n';
	    continue;
	}
	if(res_id >= n){
	    std::cout<<"Res_id is over range."<<'\n';
	    continue;
	}
	if(res_num > demand[client_id][res_id]){
	    std::cout<<"Res_num is unfit."<<'\n';
	    continue;
	}
	if(res_num > resource[res_id]){
	    std::cout<<"Sufficient resource."<<'\n';
	    continue;
	}
	
	//更新现有资源和客户占有资源的记录
	occupy[client_id][res_id] += res_num;
	resource[res_id] -= res_num;

	//向输出文件写入客户请求
	os<<"客户"<<client_id<<' '<<"资源"<<
    res_id<<' '<<"数目"<<res_num<<' ';

	//判断请求是否安全
        if(detect()){ 
	    os<<"Safe"<<'\n';
	    std::cout<<"You request has been accepted."<<'\n';
	}

	//若请求不安全，将资源记录还原到请求前的状态
	else{
            os<<"Unsafe"<<'\n';
	    occupy[client_id][res_id] -= res_num;
	    resource[res_id] += res_num;
	    std::cout<<"You request has been denied."<<'\n';
	}
    }

    os.close();
    return 0;
}


int main(){
    //输入客户数目和资源种数
    std::cout<<"Please input m and n"<<
"(separated by spaces)"<<'\n';
    std::cin>>m>>n;

    //输入每种资源的数量
    std::cout<<"Please input the num of resource"<<
"(separated by spaces)"<<'\n';
    for(unsigned i = 0;i < n;++i)
	std::cin>>resource[i];

    //输入每个客户对每种资源的最大需求
    std::cout<<"Please input the total demand"<<
"(separated by spaces)"<<'\n';
    for(unsigned i = 0;i < m;++i)
	for(unsigned j = 0;j < n;++j)
	    std::cin>>demand[i][j];
    
    return banker();
}
		





