/**
 * 本文件是一个使用互斥锁和条件变量实现的高速缓存程序
 *
 * 编译命令： g++ -o producer_consumer producer_consumer.cpp
 * 执行命令： ./producer_consumer
 * 使用 kill -15 producer_consumer 发送信号，可以激活本程序的incache函数
 *
 * Author: JackieChan
 * 2020.11.24
 *  
 */ 

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
using namespace std;

// lock
pthread_mutex_t mtx;
// condition
pthread_cond_t cond;

struct CacheNode{
    int id;
    CacheNode *next;
    CacheNode(){
      id = 0;
      next = NULL;
    }
};

// cache container
struct CacheBlock{
    CacheNode head;
    CacheNode tail;

    CacheBlock(){}
    void addNode(CacheNode *node){
        if(tail.next == NULL)
        {
          tail.next = node;
          head.next = node;
          node->next = NULL;
        }
        else{
          tail.next->next = node;
          tail.next = node;
        }
    }

    CacheNode* getHeadNode(){
        CacheNode *ans=NULL;
        if(head.next != NULL){
          ans = head.next;
          head.next = head.next->next;
          if(head.next == NULL)
            tail.next = NULL;
        }
        return ans;
    }
    bool isEmpty(){
        return head.next == NULL;
    }
};

// 模仿每次新增产品的id
static int cnt = 0;
CacheBlock cacheContainer;

// producer
void incache(int arg){
    // make a product
    struct CacheNode *node;
    node = new CacheNode();
    node->id = cnt++;

    // request lock
    pthread_mutex_lock(&mtx);
    // add the new product into container
    cacheContainer.addNode(node);
    // release the lock
    pthread_mutex_unlock(&mtx);
    // notify all threads who are waitting for the condition
    pthread_cond_broadcast(&cond);
}

void *outcache(void* args){
    while(true){	
        // request a lock
        pthread_mutex_lock(&mtx);
        while(cacheContainer.isEmpty())
        {
          // wait the condtion
          pthread_cond_wait(&cond, &mtx);
        }
        // consume a product
        CacheNode* node = cacheContainer.getHeadNode();

        // release the lock
        pthread_mutex_unlock(&mtx);

        cout << pthread_self() << ": product id is " << node->id << endl;
        delete node; // 一定要注意销毁自己申请的内存
        //线程具有优先唤醒的问题，也就是当这个线程释放锁以后，很大可能再次去抢占得到，其他线程抢不上的问题
        usleep(100); 
    }
    return (void*)0; //C++11 request a return here, otherelse it will report error
}

int main(){
    // set the signal accept function, command line as "kill -15 cachetest"
    signal(15, incache);
    // initialize the mtx
    pthread_mutex_init(&mtx,NULL);
    // initialize the condition
    pthread_cond_init(&cond, NULL);

    int consumerNum = 2;
    pthread_t pthid;
    vector<pthread_t> pthv;
    for(int i = 0; i < consumerNum; i++){
        pthread_create(&pthid, NULL, outcache, NULL);
        pthv.push_back(pthid);
    }
    // wait the pthread end
    for(int i = 0; i < consumerNum; i++){
        pthread_join(pthv[i], NULL);
        // pthread_detach(pthv[i]); // set detach
    }

    // 需要用 kill -9 的方法杀掉本程序，所以这段代码其实不执行，
    // 不过可以使用线程取消的方法让本段代码执行:pthread_cleanup_push/pop
    // destroy the condition
    pthread_cond_destroy(&cond);
    // destroy the mtx
    pthread_mutex_destroy(&mtx);

    return 0;
}
