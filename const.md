## Const解惑

### 1. const和普通类型

普通类型： int, float, double, char, short, long 等基本类型

```c++
const int v1 = 5;
int const v1 = 5;
```

**[const int]**和**[int const]**是等价的，都被称为常类型，定义常变量，也就是const和普通变量的前后位置没影响

### 2. const 和指针变量

```c++
const int *p; // 定义的是指向int类型的常变量的指针，
  						// 也就是p这个指针变量指向的整形常量，
  						// p指向的地址可以改变，但p指向的整型常量不可以改变
int const *p; // 和 const int *p等价。
const int v1 = 5;
p = &v1; // 将常整型v1的地址给p
const int v2 = 6;
p = &v2; // 可以将p指向的地址换成v2的地址
*p = 10; // (compile error)编译失败, 因为p指向的是常整型，不能改变值

// 另外
int v3 = 10; // 注意这是个普通整型变量
p = &v3; // 能否将整型变量的地址给p呢？ 
				 // (1) 这个地方，应该是不能的，(2) 貌似也是合理的
				 // 解释(1)：既然用常指针指向该变量，那么程序员应该不想改变那个变量的值，所以可以通过修改原变量的值，而不可以通过指针来修改，貌似这样的编程设计没啥意义
				 // 解释(2): 参考常类型（引用或者指针）形参，也就是说，程序员不希望通过该指针变量来修改原值，但是可以通过该变量本身修改值。这样在某些情况下使用该变量，又不想被误改，还是有很大意义的。
				 // 反正这样写，我的编译器不报错，连个警告都没有
```

### 3.const 在类里面的表现

const 在定义类里面，放在成员函数后面可以定义类常成员函数，且只能有该类的常对象访问

在常成员函数中不能对类的成员变量修改。

```c++
  1 #include <iostream>
  2 using namespace std;
  3 class Student{
  4 private:
  5     string name;
  6 public:
  7     Student(string);
  8     void displayName() const;            // (1)
  9     void displayName();                  // (2)   这里(1)和(2)可以构成成员函数重载                                                                                             
 10 		
 11     void setName(string _name) const;    // (3)   
 12     void setName(string _name);          // (4)  
 13 };
 14 Student::Student(string _name)
 15 {
 16     name = _name;
 17 }
 18 void  Student::displayName() const
 19 {
 20     cout << "常成员函数：" << name << endl;
 21 }
 22 
 23 void Student::displayName()
 24 {
 25     cout << "普通成员函数:" << name << endl;
 26 }
 27 
 28 void Student::setName(string _name) const  // 编译不通过
 29 {
 30     name = _name;                        // (5) 在常成员函数里面不能修改成员变量
 31 }
 32 
 33 void Student::setName(string _name)
 34 {
 35     name = _name;
 36 }
 37 
 38 int main(){
 39     Student stu1("xiaolong");       // 普通对象
 40     Student const stu2("xiaolong"); // 常对象
 41 
 42     stu1.displayName(); // 调用普通成员函数
 43     stu2.displayName(); // 调用常成员函数
 44 
 45     stu1.setName("xiaoming");
 46     stu2.setName("xiaoming"); // 报错
 47  
 48     stu1.displayName(); // 调用普通成员函数
 49     stu2.displayName(); // 调用常成员函数
 50     return 0;
 51 }
```

注意在类里面通过在函数后面加const修饰符构成函数重载的特点，通过const构成函数重载也是类里面的一大特点。

去掉上述程序的 **代码行(5)** 后，程序就可以编译通过。输出如下：

```c++
普通成员函数:xiaolong
常成员函数：xiaolong
普通成员函数:xiaoming
常成员函数：xiaolong
```

**注： 在常成员函数里面不能修改成员变量**

### 4.const定义的常变量的值真的不能改吗？

答案：没有C做不到的事情

```c++
volatile const int v = 5;                                                                                                                    
int *p = (int*)&v;
(*p) = 10; 
cout << v << endl;
// 输出是10
```

上面这段代码，就改了

注意：**volatile** 必须要有，它是告诉编译器，别给我优化代码，因为一旦优化了，他可能就直接把整个代码里面所有的**v**都给替换换成字面量5了，就看不到上面的效果了。

当然了 **int \*p = (int\*)&v** 中的强制类型转换必须要有，不然这个代码都编译不通过。