#include <dlfcn.h>
#include <iostream>
#include "exp2_v4_FA_Lib.h"

#define ID 2					 //功能函数id
const char *str = "Hello China"; //输出语句
const char *print = "print";	 //功能函数实现功能定义

using namespace std;

class Lib : public FA_Lib //定义派生类
{
public:
	Lib()
	{
	}
	~Lib()
	{
	}

public:
	void Print() //打印函数
	{
		cout << str << endl; //输出语句
	}

	void Help() //帮助函数
	{
		//协助功能函数所提供的功能
		cout << ID << " - " << print << " \"" << str << "\"" << endl;
	}

	int GetID() //获取id函数
	{
		return ID; //返回ID
	}
};

extern "C" void
InstallLib(FA_Lib **ppLib) //对外声明的注册动态链接库函数
{
	static Lib slib; //定义当前类静态对象
	*ppLib = &slib;	 //取lib的地址存入ppLib二级指针指向的一级指针
}