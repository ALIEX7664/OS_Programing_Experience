#include <iostream>
#include <dlfcn.h>
#include <string.h>
#include "exp2_v4_Dync_Lib.h"
#include "exp2_v4_Dync_Lib_Ctrl.h"

typedef void (*Fun)(void); //定义void型函数指针

using namespace std;

int main(int argc, char *argv[])
{

	if (argc == 2)//输入参数个数正确
	{
		Dync_Lib_Ctrl controller; //定义动态链接库控制类对象

		controller.Installer();//装载句柄和函数地址

		if (strcmp(argv[1], "help") == 0)//判断输入参数内容
		{
			controller.GetHelp();//调用帮助函数
		}
		else
		{
			controller.PrintCall(atoi(argv[1]));//调用打印函数
		}

		controller.Uninstaller(); //卸载句柄
	}
	else
	{
		//输入格式错误输出
		cout << "Please enter:<filename> help/<id>" << endl;
	}

	return 0;
}
