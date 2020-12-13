/*
 * C++里面三种重定向输入输出的方法
 *
 * 只有cin和cout可以进行重定向,cerr和clog不能重定向。
 * cin\cout\cerr\clog这几个对象是内置对象，在iostream.cpp里面有定义
 *
 * Author: JackieChan
 * 2020.12.11
 */ 
#include <iostream>
#include <fstream>
using namespace std;


// 第一种重定向方法
// 使用c语言自带的freopen方法，比较简单高效
void reMethod1(){
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	string name;
	cout << "请输入姓名" << endl;
	cin >> name;
	cout << "您输入的姓名是:" << name << endl;
}
// 第二种重定向方法
// 用ios类的rdbuf方法，rdbuf->redirect buffer, 重定向缓存
// istream和ostream继承了ios类的rdbuf方法
// cin和cout分别是istream的对象和ostream的对象，所以可以直接调用cin和cout的rdbuf函数
// 该函数使用的缓存指针的类型是streambuf类
void reMethod2(){
	ifstream fin("in.txt");
	ofstream fout("out.txt");
	
	streambuf* old_in = cin.rdbuf(fin.rdbuf());
	streambuf* old_out = cout.rdbuf(fout.rdbuf());

	string name;
	cout << "请输入姓名" << endl;
	cin >> name;
	cout << "您输入的姓名是:" << name << endl;
	
	// 解除重定向
	cin.rdbuf(old_in);
	cout.rdbuf(old_out);

	cout << "请输入姓名" << endl;
	cin >> name;
	cout << "您输入的姓名是:" << name << endl;
}

// 第三种重定向方法，
// 是在命令行执行程序时，使用 '<' 重定向输入文件， '>' 重定向输出文件
void reMethod3(){
	// ./test  <in.txt >out.txt
	string name;
	cout << "请输入姓名：";
	cin >> name;
	cout << "您的姓名是：";
	cout << name<< endl;
}
int main(){
	int test = 1;

	if(test == 1){
		reMethod1();
	}
	else if(test == 2){
		reMethod2();
	}
	else if(test == 3){
		reMethod3();
	}
	return 0;
}
