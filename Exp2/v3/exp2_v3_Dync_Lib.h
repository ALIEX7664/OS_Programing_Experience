#ifndef _V3_DYNC_LIB_H //防止头文件重复包含
#define _V3_DYNC_LIB_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Dync_Lib //动态链接库类
{
public:
    Dync_Lib()
    {
    }
    ~Dync_Lib()
    {
    }

public:
    //保存动态链接库路径到vector容器中
    bool GetName(vector<string> &namevec);
};

#endif
