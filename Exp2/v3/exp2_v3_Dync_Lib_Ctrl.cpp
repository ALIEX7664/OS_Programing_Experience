#include "exp2_v3_Dync_Lib_Ctrl.h"
#include "exp2_v3_Dync_Lib.h"
#include <iostream>
#include <dlfcn.h>

//装载句柄以及函数地址到对应的容器中
bool Dync_Lib_Ctrl::Installer()
{
    vector<string> name;            //定义存储路径的vector容器
    Dync_Lib lib;                   //定义动态链接库类
    if (lib.GetName(name) == false) //获取路径名并装入vecor容器中
    {
        cout << "GetName() false" << endl;
        return false;
    }

    //遍历name容器，装载函数地址到对应的vector容器中
    for (int i = 0; i < name.size(); i++)
    {
        //调用c_str转换string为const char*数据类型
        void *handle = dlopen(name[i].c_str(), RTLD_LAZY);
        if (handle == NULL)
        {
            cout << dlerror() << endl;
            return false;
        }
        else
        {
            handlevec.push_back(handle); //将句柄装入vector容器中
        }

        //获取各动态链接库的各个函数地址
        IDGET id = (IDGET)dlsym(handle, "GetID");
        FUN print = (FUN)dlsym(handle, "Print");
        HELP help = ((HELP)dlsym(handle, "Help"));
        if (id == NULL || print == NULL || help == NULL) //出错判断
        {
            cout << "Functions install error" << endl;
            return false;
        }
        else
        {
            //将各动态链接库的函数地址装入对应的vector容器中
            printvec.push_back(print);
            idvec.push_back(id);
            helpvec.push_back(help);
        }
    }

    return true;
}

bool Dync_Lib_Ctrl::Uninstaller() //卸载句柄
{
    for (int i = 0; i < handlevec.size(); i++)
    {
        dlclose(handlevec[i]); //遍历容器，关闭动态链接库
    }

    return true;
}

bool Dync_Lib_Ctrl::GetHelp() //帮助函数
{
    for (int i = 0; i < helpvec.size(); i++)
    {
        helpvec[i](); //调用各动态链接库Help函数
    }

    return true;
}

bool Dync_Lib_Ctrl::PrintCall(int id) //打印函数
{
    int i;
    for (i = 0; i < idvec.size(); i++)
    {
        if (idvec[i]() == id) //调用GetID函数获取id，与传入的id匹配
        {
            printvec[i](); //调用各动态链接库Print函数
            break;
        }
    }
    if (i == idvec.size()) //传入id不存在时的处理方法
    {
        cout << "Please enter a existing ID!" << endl;
        return false;
    }

    return true;
}
