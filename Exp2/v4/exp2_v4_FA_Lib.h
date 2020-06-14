#ifndef _V4_FA_LIB_H
#define _V4_FA_LIB_H

#include <vector>

//定义基类，统一接口类型
class FA_Lib
{
public:
    FA_Lib()
    {
    }
    virtual ~FA_Lib()
    {
    }

public:
    //要求派生类实现以下函数
    virtual void Help() = 0;  //Help虚函数
    virtual void Print() = 0; //Print虚函数
    virtual int GetID() = 0;  //GetID虚函数
};

#endif