#ifndef _V3_DYNC_LIB_CTRL
#define _V3_DYNC_LIB_CTRL

#include <vector>

typedef int (*IDGET)(void); //定义int函数指针，用于调用GetID函数
typedef void (*HELP)(void); //定义void函数指针，用于调用Help函数
typedef void (*FUN)(void);  //定义void函数指针，用于调用功能函数

using namespace std;

class Dync_Lib_Ctrl //动态链接库控制类
{
private:
    vector<void *> handlevec; //用于存储链接库的句柄
    vector<IDGET> idvec;      //用于存储GetID函数地址
    vector<HELP> helpvec;     //用于存储Help函数地址
    vector<FUN> printvec;     //用于存储功能函数地址
public:
    Dync_Lib_Ctrl()
    {
    }
    ~Dync_Lib_Ctrl()
    {
    }

public:
    bool Installer();   //装载句柄和函数地址函数
    bool Uninstaller(); //卸载句柄函数

    bool GetHelp();         //帮助函数
    bool PrintCall(int id); //打印函数
};

#endif
