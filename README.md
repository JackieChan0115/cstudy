----------------------------------------------------------------------------
#### 开始我的linux及C/C++系统化学习之路，慢慢往这里增加一些相关内容。
#### 日积月累，慢慢来吧，看看能做到什么程度。 --- 2020.11.24 日记
----------------------------------------------------------------------------

### 代码目录
1. [用条件变量和互斥锁实现的高速缓存Demo](https://github.com/JackieChan0115/cstudy/blob/master/producer_consumer.cpp)
2. [多线程资源回收Demo](https://github.com/JackieChan0115/cstudy/blob/master/pthread_clean.cpp)
3. [线程取消Demo](https://github.com/JackieChan0115/cstudy/blob/master/pthread_cancel.cpp)
4. [避免僵尸线程](https://github.com/JackieChan0115/cstudy/blob/master/pthread_defunct.cpp)
5. [文件复制Demo 类似cp](https://github.com/JackieChan0115/cstudy/blob/master/copyfile.cpp)
6. [Linux操作命令](https://github.com/JackieChan0115/cstudy/blob/master/linux-order.md)
7. [用c++ socket写的简单的http服务器](https://github.com/JackieChan0115/cstudy/blob/master/simple-httpserver)
8. [快速排序非递归实现](https://github.com/JackieChan0115/cstudy/blob/master/qsort.md)

## 近期计划内容

1. 冒泡排序， 选择排序，插入排序，[快速排序](https://github.com/JackieChan0115/cstudy/blob/master/qsort.md)，堆排序，归并排序，希尔排序，桶排序，基数排序，计数排序
2. vim使用技巧积累
3. 多线程和进程相关的学习内容


#### 1.线程的两种状态
+ **(1). 线程有joinable和unjoinable两种状态，默认是joinable状态**
+ **(2). joinable状态的线程在自己关闭的时候，系统将不会回收它的资源，导致它成为僵尸线程。**

#### 2.智能指针

**简要说智能指针的实现原理： 利用栈上对象出作用域会自动析构的原理来销毁建立在堆上的对象**



