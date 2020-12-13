/*
 * 本文件是利用系统调用函数execvp实现了一个模拟bash窗口
 * 编译命令：gcc -o minibash minibash.cpp
 * 执行命令：./minibash
 * 执行后，可以在里面输入单条常用的命令，比如ls -l 或 netstat -ano等等
 * 只支持每次使用一个命令工具，不支持使用管道连接多个命令
 *
 * Author: JackieChan
 * 2020.12.12
 */ 

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>
using namespace std;

int main(){
	char buf[512];
	int len = 512;
	char func_name[64];
	while(1){

		vector<char*> params;
		bzero(buf, sizeof(buf));
		bzero(func_name, sizeof(func_name));
		cout << "minibash$: ";
		cin.getline(buf, sizeof(buf));
		char* token = strtok(buf, " ");
		while(token != NULL){
			params.push_back(token);
			token = strtok(NULL, " ");
		}
		if(params.size() == 0){
			cout << "Input command error!" << endl;
			continue;
		}
		char **args = (char**)malloc((params.size() + 1) * sizeof(char*));
		for(int i = 0; i < params.size(); i++){
			args[i] = params[i];
		}
		args[params.size()] = NULL;
		
		int pid = fork();
		if( pid == 0 ){ // 进入子进程
			int ans = execvp(args[0], args);
			if( ans < 0){
				cout << "Error happened!" << endl;
			}
			exit(0);
		}
		else if( pid > 0 ){ // 进入父进程
			int status;
			waitpid( pid , &status, 0);
		}
		else{ // fork失败
			cout << "fork error!" << endl;
		}
	}

	return 0;
}
