#include <iostream>
#include <dlfcn.h>

using namespace std;
typedef void (*Fun)(void); //定义void型函数指针

int main(void)
{
	//打开链接动态库，保存句柄
	void *handle = dlopen("./libtest.so", RTLD_LAZY);
	if (handle == NULL) //出错处理
	{
		cout << dlerror() << endl;
		return 0;
	}

	//获取Print函数地址，通过强制转换保存在函数指针中
	Fun Print = (Fun)dlsym(handle, "Print");
	if (Print == NULL) //出错处理
	{
		cout << dlerror() << endl;
		return 0;
	}

	(*Print)(); //执行函数指针地址的函数功能

	dlclose(handle); //关闭指定句柄的动态链接库
	return 0;
}
