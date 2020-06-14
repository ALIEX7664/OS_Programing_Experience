#ifndef _V4_DYNC_LIB_H
#define _V4_DYNC_LIB_H

#include <vector>
#include <string>

using namespace std;

class Dync_Lib
{
public:
    Dync_Lib()
    {
    }
    ~Dync_Lib()
    {
    }

public:
    bool GetName(vector<string> &namevec); //获取动态链接库的路径
};

#endif
