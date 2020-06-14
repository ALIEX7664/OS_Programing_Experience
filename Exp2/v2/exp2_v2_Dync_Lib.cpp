#include <dirent.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include "exp2_v2_Dync_Lib.h"

extern int errno; //引入错误码

bool Dync_Lib::GetName(vector<string> &namevec)
{
	DIR *dir;							   //定义目录结构体指针
	if ((dir = opendir("plugin")) == NULL) //打开plugin目录
	{
		perror("Opendir error");
		return false;
	}

	struct dirent *den; //定义目录项结构体指针
	rewinddir(dir);		//初始化读取位置

	//遍历目录项，并调用PrintCall函数
	while (1)
	{
		string str = "plugin/";

		if ((den = readdir(dir)) == NULL) //读取目录项
		{
			if (errno == 9) //出错处理
			{
				perror("Readdir error");
				closedir(dir);
				return false;
			}
			break; //若是到达文件尾则跳出循环并正常退出
		}
		else
		{
			//排除上级目录项和当前目录项
			if (strcmp(den->d_name, ".") != 0 && strcmp(den->d_name, "..") != 0)
			{
				str += den->d_name;
				namevec.push_back(str);
			}
		}
	}

	closedir(dir); //关闭目录
	return true;
}
