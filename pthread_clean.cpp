/**
 * 本文件是线程资源清理函数的学习案例
 * 介绍了pthread_cleanup_push和pthread_cleanup_pop的使用方法
 * 编译命令为： g++ -o pthread_clean pthread_clean.cpp
 * 执行命令: ./pthread_clean
 * Author: JackieChan
 * 2020.11.25
 */
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void cleanFunction(void* args);
void *pthreadMain(void* args);

int main(){
	// 线程id
	pthread_t pthid;
	// create a thread
	pthread_create(&pthid, NULL, pthreadMain, NULL);
	sleep(3);
	// 3秒后，pthreadMain函数还没有执行完毕，这个时候调用线程取消函数，
	// 该线程的循环会被终止，但是清理函数依旧会被调用。
	pthread_cancel(pthid);
	// 设置等待线程结束
	pthread_join(pthid, NULL);

	cout << "Thread comes to an end" << endl;
	return 0;
}
// 清理函数，使用pthread_cleanup_push加入栈里面，退栈的时候执行
void cleanFunction(void* args){
	int num = int(long(args));
	cout << "clean function has been called = >" << num << endl;
}

// 线程主函数
void *pthreadMain(void* args){
	// pthread_cleanup_push和pthread_cleanup_pop必须成对出现，否则报不知名的错误
	pthread_cleanup_push(cleanFunction,(void*)(long)1);
	pthread_cleanup_push(cleanFunction,(void*)(long)2);
	pthread_cleanup_push(cleanFunction,(void*)(long)3);

	for(int i = 0; i < 5; i++){
		cout << "pthread , output << " << i << endl;
		sleep(1);
	}
	
	// pthread_cleanup_pop()里面的参数是int类型，要么大于零，要么等于0
	// 等于0，只是将清理函数退栈，但是不执行，一般没啥用
	// 大于0，将清理函数退栈并且执行
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	// pthread_exit((void*)0); 内部退出使用pthread_exit(void* args)更稳妥
	// 虽然推荐不要使用return 函数，可是C++新标准里，只要函数定义又返回值，就必须return ,否则编译不通过
	return (void*)0;
}
