/**
 * 避免僵尸线程
 * 线程有两种状态，joinable和unjoinable,线程默认是joinable的
 * 而joinable的线程在退出的时候，如果主线程还没有关闭，那么该子线程的资源不会被回收,成为僵尸线程
 * 将joinable变为unjoinable可以让子线程退出后能够被系统回收资源
 * 方法有：
 * （1）在生成线程时，使用pthread_attr_t设置新线程的属性为unjoinable的
 * （2）在生成线程后，直接使用pthread_detach将该线程设置为unjoinable状态，也可以在线程內部自己设置
 * 注意：unjoinable状态的线程不能再使用pthread_join函数阻塞该线程，即使使用该语句也不会起作用
 * 
 * 原理如上，但是在测试过程中，即使将线程有意设置joinable，发现资源还是被回收了，所以这个地方有点莫名其妙
 * 不过在编程中还是要考虑好逻辑关系，要认为他不会自己回收，要主动对线程进行资源回收方面的处理
 * 
 * Author: JackieChan
 * 2020.11.25
 */ 

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

// 0,1,2,3和分别对应要测试的不设置unjoinable,和方法1,及方法2的两个方式
static int testMethodNumber = 1;

void *pthreadMain(void* args);

int main(){
	pthread_t pthid;
	if(testMethodNumber == 1){
		pthread_attr_t attr; // 声明一个线程属性对象
		pthread_attr_init(&attr); // 初始化一个线程的默认属性
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); // PTHREAD_CREATE_JOINABLE 设置该线程为detached模式，即成为unjoinable状态
		pthread_create(&pthid, &attr, pthreadMain, NULL);
		pthread_attr_destroy(&attr); // 不再使用的线程属性对象应该被销毁
	}
	else{
		pthread_create(&pthid, NULL, pthreadMain, NULL);
		if(testMethodNumber == 2){
			// 立即将该线程设置为detached
			pthread_detach(pthid);
		}
	}
	sleep(50); // 等待子线程结束，然后使用ps -xH查看该线程是否还存在
	return 0;
}

void *pthreadMain(void* args){
	
	if(testMethodNumber == 3){
		pthread_detach(pthread_self());
	}
	
	cout << "进入子线程，子线程id是 " << pthread_self() << endl;
	for(int i = 0; i < 4; i++){
		cout << "child process " << pthread_self() << endl;
		sleep(1);
	}
	cout << "子线程退出了" << endl;
	pthread_exit((void*)0);
	return (void*)0;
}
