#ifndef _V4_DYNC_LIB_CTRL
#define _V4_DYNC_LIB_CTRL

#include <vector>
#include "exp2_v4_FA_Lib.h"

//定义void类型指针，用于存储InstallLib函数地址
typedef void (*FALib)(FA_Lib **);

using namespace std;

class Dync_Lib_Ctrl //动态链接库控制类
{
private:
    vector<void *> handlevec; //用于存储动态链接库的句柄
    vector<FA_Lib *> libvec;  //用于存储派生类指针
public:
    Dync_Lib_Ctrl()
    {
    }
    ~Dync_Lib_Ctrl()
    {
    }

public:
    bool Installer();   //装载句柄和派生类
    bool Uninstaller(); //卸载句柄

    bool GetHelp();         //主要实现调用派生类的帮助函数
    bool PrintCall(int id); //主要实现调用派生类的打印函数
};

#endif
