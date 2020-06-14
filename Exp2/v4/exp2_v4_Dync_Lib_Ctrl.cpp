#include "exp2_v4_Dync_Lib_Ctrl.h"
#include "exp2_v4_Dync_Lib.h"
#include "exp2_v4_FA_Lib.h"
#include <iostream>
#include <dlfcn.h>

//注册函数以及函数功能到对应的容器中
bool Dync_Lib_Ctrl::Installer()
{
    vector<string> name;            //定义存储路径名的vetor容器
    Dync_Lib lib;                   //定义用于获取路径名的对象
    if (lib.GetName(name) == false) //获取链接库路径并存入name中
    {
        cout << "GetName() false" << endl;
        return false;
    }

    //遍历vector容器，获取所有的派生类对象
    for (int i = 0; i < name.size(); i++)
    {
        void *handle;  //用于保存动态链接库句柄
        FA_Lib *pplib; //定义基类指针，用来存储派生类的地址

        //调用c_str转换string为const char*数据类型
        handle = dlopen(name[i].c_str(), RTLD_LAZY);
        if (handle == NULL)
        {
            cout << dlerror() << endl;
            return false;
        }
        else
        {
            handlevec.push_back(handle); //将句柄装入对应的vector容器中
        }

        //获取每个注册动态库派生类函数的地址
        FALib inslib = (FALib)dlsym(handle, "InstallLib");
        if (inslib == NULL)
        {
            cout << dlerror() << endl;
            return false;
        }

        pplib = NULL; //初始化指针

        //以二级指针形式传入，pplib获得派生类的地址传出
        (*inslib)(&pplib);
        if (pplib == NULL)
        {
            cout << "InstallLib() execute error" << endl;
            return false;
        }

        //将派生类地址存入对应的vector容器中
        libvec.push_back(pplib);
    }

    return true;
}

bool Dync_Lib_Ctrl::Uninstaller() //卸载函数
{
    //遍历vector容器，将注册的派生类
    for (int i = 0; i < handlevec.size(); i++)
    {
        dlclose(handlevec[i]);
    }

    return true;
}

bool Dync_Lib_Ctrl::GetHelp() //帮助函数调用函数
{
    //遍历vector容器，调用所有Help函数
    for (int i = 0; i < libvec.size(); i++)
    {
        libvec[i]->Help();
    }

    return true;
}

bool Dync_Lib_Ctrl::PrintCall(int id) //打印函数调用函数
{
    int i;
    //遍历vector容器，调用相应的打印函数
    for (i = 0; i < libvec.size(); i++)
    {
        if (libvec[i]->GetID() == id) //id匹配
        {
            libvec[i]->Print(); //调用相应的打印函数
            break;
        }
    }
    if (i == libvec.size()) //匹配不到对应id，出错处理
    {
        cout << "Please enter a existing ID!" << endl;
        return false;
    }

    return true;
}
