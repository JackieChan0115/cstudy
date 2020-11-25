/**
 * 线程取消的四点及其相关问题
 *  (1) 在主函数中阻塞后等待线程结束, pthread_join(pthread_t, void** val)
 * （2）在调用线程的函数后面，直接使用pthread_cancel(pthread_t pthid)进行取消线程
 *  (3) 在线程内部自己调用pthread_exit(void* arg)或者return进行退出
 *  (4) 调用exit(0)直接退出
 * 
 * 介绍下上面的相关问题：
 *  方法(1),会阻塞主线程
 *  方法(2),需要由主线程调用，那么就需要知道何时调用，涉及到一系列线程和主线程处理问题进度的反馈逻辑问题
 *  方法(3),可以由线程自己调用后直接退出，一般采取这个方法
 *  方法(4),会导致主线程和所有其他线程全部退出，一般不能采取这个极端方法
 * 
 * 另外关于pthread_cancel还涉及到两种关闭属性，一种叫做立即取消，一种叫做延时取消
 * 		其中立即取消，就是主函数调用了pthread_cancel后，线程立即退出
 *		而延时取消，会等到线程运行到取消点的时候再退出，取消点就是可以引起函数中断的处理过程，比如printf,scanf,sleep等
 *	(1) pthread_setcancelstate(int state, int *oldstate) 设置线程可不可以被pthread_cancel函数取消
 *			有两个值，分别是 PTHREAD_CANCEL_ENABLE和PTHREAD_CANCEL_DISABLE
 *			pthread_setcancelstate的第一个参数是新设置的状态，第二个参数是用来返回旧状态的
 *	(2) pthread_setcanceltype(int type, int *oldtype) 设置线程的取消类型，立即取消和延时取消
 *			有两个值，分别是 PTHREAD_CANCEL_DEFERRED(延时取消) 和 PTHREAD_CANCEL_ASYNCHRONOUS(立即取消)
 *			第一个参数是新类型，第二个参数是用来返回旧类型
 *		
 * Author: JackieChan
 * 2020.11.25
 */ 
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;


// 1,2,3,4 => 对应上述退出方法1，2，3，4
static int testMethodNumber = 2;
// 1.测试是否可以取消
// 2.测试取消类型
static int testSet = 1;

void *pthreadMain(void* args){
	if(testSet == 1){
		// 默认是可取消
		// 设置不可取消，这个时候主线线程的pthread_cancel函数将失去作用
		cout << "pthread_setcancelstate 设置了不可取消线程" << endl;
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	}
	if (testSet == 2){
		// 默认是接收到pthread_cancel信号后，立即取消
		// 设置延迟取消,延迟取消的测试最好使用单线程跑，这里看不出具体效果，因为cout和sleep都是取消点，收到pthread_cancel几乎是立即取消
		cout << "pthread_setcanceltype 设置为延迟取消" << endl;
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	}

	int num = int(long(args));
	
	for(int i = 0; i < 10; i++){
		cout << "I am thread: " << num << " the " << i << "times report" << endl;
	
		if(testMethodNumber == 3 && i == 5){
			cout << "调用pthread_exit()方法退出本线程" << endl;
			pthread_exit((void*)(long)0); // 方法(3)
		}
		if(testMethodNumber == 4 && i == 5){
			cout << "调用exit(0)退出本进程" << endl;
			exit(0); // 方法(4)
		}
		
		sleep(1);
	}
	return (void*)0;
}

int main(){
	// 声明两个线程的id
	pthread_t pthid1,pthid2;
	
	// 创建两个线程，分别传入参数1和2加以区分
	pthread_create(&pthid1, NULL, pthreadMain, (void*)(long)1);
	pthread_create(&pthid2, NULL, pthreadMain, (void*)(long)2);
	
	// 使用pthread_join阻塞主线程，等待子线程的结束 -> 方法(1)
	if(testMethodNumber == 1){
		pthread_join(pthid1, NULL);
		pthread_join(pthid2, NULL);
	}

	if(testMethodNumber == 2){
		sleep(2);
		cout << "将线程1关掉，应该可以看到线程1不再输出" << endl;
		pthread_cancel(pthid1); //结束第一个子线程 -> 方法(2)
	}

	if(testMethodNumber != 1){ // 测试其他方法需要堵塞在这里，不然主线程就自己退出了
		sleep(25);
	}

	cout << "All threads come to end" << endl;

	return 0;
}
