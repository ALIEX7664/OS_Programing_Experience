#include <iostream>
#include <dlfcn.h>
#include "exp2_v2_Dync_Lib.h"

typedef void (*Fun)(void); //定义void函数指针

bool PrintCall(vector<string> &vec); //调用Print函数
void ReturnPath(string path);		 //输出被调用函数的文件路径

using namespace std;

bool PrintCall(vector<string> &vec) //调用Print函数
{
	Fun p;		  //void型函数指针
	void *handle; //定义动态链接库句柄
	for (int i = 0; i < vec.size(); i++)
	{
		//调用c_str转换string为const char*数据类型
		handle = dlopen(vec[i].c_str(), RTLD_LAZY);
		if (handle == NULL)
		{
			cout << dlerror() << endl;
			return false;
		}

		//通过函数指针保存Print函数地址
		p = (Fun)dlsym(handle, "Print");
		if (p == NULL)
		{
			cout << dlerror() << endl;
			return false;
		}

		ReturnPath(vec[i]); //输出打印内容来自哪个文件
		(*p)();				//函数调用

		dlclose(handle); //关闭动态库
	}
	return true;
}

void ReturnPath(string path) //输出被调用函数的文件路径
{
	cout << "in \"" << path << "\" file:\t";
}

int main(void)
{
	vector<string> v; //定义string类型vector容器
	Dync_Lib lib;	  //定义动态链接库类

	lib.GetName(v); //获取动态链接库路径并存储
	PrintCall(v);	//引用vetor容器中的路径，实现函数功能

	return 0;
}
