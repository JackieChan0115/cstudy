## lambda表达式

### 1.lambda表达式(匿名表达式)的格式：

**\[变量捕捉列表\]\(参数列表\)mutable-> 返回类型\{函数体\}**

**变量捕捉列表：**

| 捕捉列表  | 说明                                           |
| --------- | ---------------------------------------------- |
| [变量名]  | 值传递的方式捕获某变量                         |
| [=]       | 值传递的方法捕获父作用域的所有变量(包括this)   |
| [&变量名] | 引用传递的方式捕获某变量                       |
| [&]       | 引用传递的方法捕获父作用域的所有变量(包括this) |
| [this]    | 值传递的方法捕获当前的this指针                 |

**参数列表**：与普通函数的参数列表一致，如果不需要参数，可以省略括号"()"

**mutable**:  lambda表达式默认是常函数，使用mutable, 让该函数能够改变外面局部变量的值

```c++
  // 代码文件： test2.cpp
  1 #include <iostream>
  2 using namespace std;
  3 
  4 int main(){
  5 
  6     int d = 10;
  7     auto lbfun = [=]{d = 15;}; // 没有参数，可隐藏括号和箭头，没有指定mutable                                                                                                                                                          
  8     lbfun();
  9     cout << d << endl;
 10 
 11     return 0;
 12 }
```

以上文件test2.cpp的lambda表达式，没有指定mutable，在编译该文件时出现以下错误。

```shell
[root@ec103f220cb0 test]# g++ -o test2 test2.cpp
test2.cpp: In lambda function:
test2.cpp:7:23: error: assignment of read-only variable ‘d’
  auto lbfun = [=]{d = 15;};
                       ^~
```

添加上mutable:

```c++
  // 代码文件：test2.cpp
  1 #include <iostream>
  2 using namespace std;
  3 
  4 int main(){
  5     
  6     int d = 10;
  7     auto lbfun = [=]()mutable->void{d = 15;}; 
  8     lbfun();
  9     cout << d << endl;
 10 
 11     return 0;                                                                                                                                                                           
 12 }
```

该文件可以编译成功，但是输出为 **10**.

因为**变量捕捉列表用的=**，即将d这个局部变量值传递进入函数，所以在函数里修改后是不会影响外面局部变量的。

改变以上代码，将**=**换成 **&** :

```c++
  // 代码文件： test2.cpp
  1 #include <iostream>
  2 using namespace std;
  3 
  4 int main(){
  5 
  6     int d = 10;
  7     auto lbfun = [&]()mutable->void{d = 15;}; // 完整格式的表达式，返回类型是void要指明，否则报错                                                                                                                                          
  8     lbfun();
  9     cout << d << endl;
 10 
 11     return 0;
 12 }
```

该文件输出是 **15**, 说明符号 **&** , 将外面的局部变量通过引用的方式传入函数了，在函数里修改后能够影响外面的局部变量。

**返回类型**: 函数体的返回类型，和普通函数开头指定的返回类型一样，经过测试，完整的lambda格式，**返回类型不能省略(int 除外，默认int)**, 省略掉括号和箭头后，不指定返回类型就可以成功。

如下面代码 18,19, 20行:

```c++
  // 代码文件test2.cpp
  1 #include <iostream>
  2 using namespace std;
  3 
  4 class Person{
  5 private:
  6     int age;
  7 public:
  8     Person(int _age):age(_age){
  9     }
 10     void printAge(){
 11         cout << this->age << endl;
 12     }
 13 };
 14 
 15 int main(){
 16 
 17     int d = 10;
 18     auto lbfun1 = [&d]{ return new Person(d);}; // 可以编译成功 
 19     // auto lbfun1 = [&d]()->{ return new Person(d);}; //会编译失败 
 20     auto lbfun2 = [&d]()-> Person*{ return new Person(d);}; // 可以编译成功
 21     Person *p1 = lbfun1();
 22     Person *p2 = lbfun2();
 23     p1->printAge();
 24     p2->printAge();                                                                                                                                                                     
 25 
 26     return 0;
 27 }
```

**函数体**：正常的函数执行代码， 可以在函数体里面使用**变量捕捉列表**捕获的变量



### 2.lambda表达式的底层实现原理

```c++
// 代码 test1.cpp
#include <iostream>
using namespace std;
int main(){
    int a = 20;
    auto fun = [&](int n)->int{return a + n;};
    fun(10);
    return 0;
}
```

```c++
// 代码test2.cpp
#include <iostream>
using namespace std;
class Add{
private:
    int a;    
public:
    Add(int _a):a(_a){}
    int operator()(int n){ 
        return a + n;
    }
};
int main(){
    Add add(20);
    add(10);
    return 0;
}
```

以上代码test1.cpp编译后，使用gdb的disassemble命令查看汇编代码如下所示：

```shell
Dump of assembler code for function main():
   0x00000000004006c1 <+0>:	push   %rbp
   0x00000000004006c2 <+1>:	mov    %rsp,%rbp
   0x00000000004006c5 <+4>:	sub    $0x10,%rsp
=> 0x00000000004006c9 <+8>:	movl   $0x14,-0x4(%rbp)                                 
   0x00000000004006d0 <+15>:	lea    -0x4(%rbp),%rax
   0x00000000004006d4 <+19>:	mov    %rax,-0x10(%rbp)
   0x00000000004006d8 <+23>:	lea    -0x10(%rbp),%rax
   0x00000000004006dc <+27>:	mov    $0xa,%esi
   0x00000000004006e1 <+32>:	mov    %rax,%rdi
   0x00000000004006e4 <+35>:	callq  0x4006a6 <<lambda(int)>::operator()(int) const>  # (1)
   0x00000000004006e9 <+40>:	mov    $0x0,%eax
   0x00000000004006ee <+45>:	leaveq 
   0x00000000004006ef <+46>:	retq   
End of assembler dump.
```

test2.cpp编译后，使用gdb查看汇编代码如下：

```shell
Dump of assembler code for function main():
   0x00000000004006a6 <+0>:	push   %rbp
   0x00000000004006a7 <+1>:	mov    %rsp,%rbp
   0x00000000004006aa <+4>:	sub    $0x10,%rsp
   0x00000000004006ae <+8>:	lea    -0x4(%rbp),%rax
   0x00000000004006b2 <+12>:	mov    $0x14,%esi
   0x00000000004006b7 <+17>:	mov    %rax,%rdi
=> 0x00000000004006ba <+20>:	callq  0x40072a <Add::Add(int)>           
   0x00000000004006bf <+25>:	lea    -0x4(%rbp),%rax
   0x00000000004006c3 <+29>:	mov    $0xa,%esi
   0x00000000004006c8 <+34>:	mov    %rax,%rdi
   0x00000000004006cb <+37>:	callq  0x400742 <Add::operator()(int)>    # (1)
   0x00000000004006d0 <+42>:	mov    $0x0,%eax
   0x00000000004006d5 <+47>:	leaveq 
   0x00000000004006d6 <+48>:	retq   
End of assembler dump.
```

以上两个汇编代码的 **(1)** 处，都是在执行一个 **仿函数**的过程。

**所以，lambda表达式的底层就是实现了一个仿函数，至于函数名，上面的汇编代码显示的是 <lambda(int)>， 编译器将lambda表达式翻译成了一个未命名或者按照某种格式命名的类，并实现了operator()的符号重载**



