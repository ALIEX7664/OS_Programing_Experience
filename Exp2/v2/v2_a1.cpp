#include <dlfcn.h>
#include <iostream>

const char *str = "Hello World"; //定义要打印的内容

using namespace std;

extern "C" void Print() //打印函数
{
	cout << str << endl;
}
