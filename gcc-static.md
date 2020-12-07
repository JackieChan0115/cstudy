## gcc静态编译库文件并链接到主程序的问题



### 问题：gcc在链接静态库的时候遇到了cannot find -lc的问题

网上的解决方案，千篇一律的为： 

```shell
yum install glibc-static
```

然而我的执行结果：

```shell
yum install glibc-static
Last metadata expiration check: 1:49:42 ago on Sun 06 Dec 2020 11:25:18 PM UTC.
No match for argument: glibc-static
Error: Unable to find a match: glibc-static
```

是的，yum库里面没有这个安装包了，只能自己编译了，于是我找到了glibc的网址：

```shell
http://ftp.gnu.org/gnu/glibc/
```

在这个网站上有很多glibc的二进制压缩包, 但都是源码，那只能下载下来自己编译了。我选择了http://ftp.gnu.org/gnu/glibc/glibc-2.32.tar.xz

使用wget执行下载命令：

```shell
wget http://ftp.gnu.org/gnu/glibc/glibc-2.32.tar.xz
```

然后解压缩:

```shell
tar -xvf glibc-2.32.tar.xz
```

不要进入glibc-2.32的目录哦，最好建一个空目录，将glibc-2.32a转移进去：

```shell
mkdir glib
mv -r glibc-2.32 ./glib/
cd ./glib/ # 进入到glib目录下
```

执行编译命令:

```shell
./glibc-2.32/configure --prefix=/usr 
# 不要在glibc-2.32目录下执行，否则会报下面这个错误
# configure: error: you must configure in a separate build directory
```

可能遇到的错误：These critical programs are missing or too old: bison

这是因为没有安装bison或者bison的版本比较旧的缘故，下面一招好解决：

```shell
yum install bison
```

然后再执行编译命令就好了

```shell
./glibc-2.32/configure --prefix=/usr 
make -j2 # -j2参数的作用是允许make开启两个并发任务，如果不指定，是不限定任务数量的
# 这个过程比较漫长，可以看会剧了。
```

最后，会发现编译出一大堆的文件来，好了，在里面找到我们的目标文件：

**libc.a** 和 **libc.so** 

将他们拷贝到系统默认的库路径 **/usr/lib/**

```shell
cp libc.a libc.so /usr/lib/ # 其实只需要静态库libc.a就够了，万一以后要用，还是把动态库libc.so收了吧
```

**然后再使用gcc执行静态链接过程就应该畅通无阻了**



### gcc编译链接静态库的案例

**要求：共三个文件，tprint.h, tprint.c, ctest.c， 其中tprint.h是tprint.c的函数声明头文件，将tprint.c编译成静态库，在ctest.c文件中调用该静态库。**

1. tprint.h

   ```c
     1 #include <stdio.h>                                                                                                                                                                                 
     2 void print(int val);
   ```

2. tprint.c

   ```c
     1 #include "tprint.h"
     2 
     3 void print(int val){
     4     printf("输出是 %d\n", val);                                                                                                                                                                    
     5 }
   ```

3. ctest.c

   ```c
     1 #include "tprint.h"                                                                                                                                                                                
     2 int main(){
     3     print(10);
     4     return 0;
     5 }
   ```

   

编译tprint.c为静态库

```shell
gcc -c -o libtprint.a tprint.c
```

编译ctest.c并链接libtprint.a

```shell
 gcc -o ctest ctest.c tprint.h -L. -ltprint -static
 # -L. 指定当前目录， -ltprint,指定要链接的库文件是libtprint.a, -static指定要直接匹配libtprint.a,
 # 如果不指定-static,会优先找libtprint.so(动态链接库)，指定了-static,直接找静态链接库
```

然后执行：

```shell
./ctest
# 输出
输出是 10
```



### 最后

以上是使用gcc的过程中遇到的问题，并没有详细介绍gcc命令，这样的讲解有很多，光看是没用的，还得在实际练习中去学习。之前没有怎么使用过gcc，现在还是喜欢这种编译方式，能够更清楚的知道编译过程都做了什么，比直接使用IDE更锻炼基本功。

