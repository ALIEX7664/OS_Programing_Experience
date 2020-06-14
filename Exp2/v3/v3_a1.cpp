#include <dlfcn.h>
#include <iostream>

#define ID 1					 //功能函数id
const char *str = "Hello World"; //输出语句
const char *print = "print";	 //功能函数实现功能定义

using namespace std;
extern "C"
{
	void Print() //打印函数
	{
		cout << str << endl; //输出语句
	}

	void Help() //帮助函数
	{
		//协助功能函数所提供的功能
		cout << ID << " - " << print << " \"" << str << "\"" << endl;
	}

	int GetID() //获取ID函数
	{
		return ID; //返回ID
	}
}
