## gcc 编译动态链接库并链接到主程序

### gcc编译链接动态库的案例

**要求：共三个文件，tprint.h, tprint.c, ctest.c， 其中tprint.h是tprint.c的函数声明头文件，将tprint.c编译成动态链接库，在ctest.c文件中调用该动态链接库。**

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

   

编译tprint.c为动态链接库

```shell
gcc -fPIC -shared -o libtprint.so tprint.c
# 命令讲解
# 其中，-shared 选项用于生成动态链接库；-fpic（还可写成 -fPIC）选项的功能是，令 GCC 编译器生成动态链接库（多个目标文件的压缩包）时，表示各目标文件中函数、类等功能模块的地址使用相对地址，而非绝对地址。这样，无论将来链接库被加载到内存的什么位置，都可以正常使用。
```

编译ctest.c并链接libtprint.so

```shell
 gcc -o ctest ctest.c tprint.h -L. -ltprint -shared
 # -L. 指定当前目录， -ltprint,指定要链接的库文件是libtprint.so, -shared指定要直接匹配libtprint.so,
 # 如果不指定-shared,还是会优先找libtprint.so(动态链接库)，所以这个-shared可以省略，但是要是加上就只找libtprint.so了。不加，如果找不到libtprint.so就会去找libtprint.a
```

然后执行：

```shell
./ctest # 报错： /ctest: error while loading shared libraries: libtprint.so: cannot open shared object file: No such file or directory
```



哈哈，这就是动态链接库的缺点之一，要想执行，必须先给我准备好动态链接库。

那这个库默认是放在/usr/lib中的，所以要将libtprint.so复制到/usr/lib中去（当然还有个在用户类路径文件里加LD_LIBRARY_DIRECTORY的方法）

```shell
cp libtprint.so /usr/lib/
```

再执行:

```shell
./ctest 
# 输出成功
输出是 10
```

### [官方且详细的介绍](http://c.biancheng.net/view/8023.html)

