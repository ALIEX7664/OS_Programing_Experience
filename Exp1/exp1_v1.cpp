#include<string.h>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

class test_1{
private:
	int x;
public:
	test_1()
	{
		int x = 0;
	}
	explicit test_1(int y) //显式声明构造函数,杜绝构造函数隐式转换，直接赋值方式，只能显示接受int类型
	{
		x = y;
	}
	virtual ~test_1()//虚构函数，继承关系构造，不用子类内存是不会被赋值的
	{

	}
public:
	void file()
	{
		std::cout<<"in file()"<<x<<std::endl;
	}

public:
bool Serialize(const char*path) const //后面const，告知用户接口内部是不会修改对象内部的私有或公有变量
{
 int fd = open(path,O_RDWR|O_CREAT|O_TRUNC,0777); //加::告知是在全局作用域里边，可不用
 if(fd == -1)
	 return false;

 int  val;
 val = write(fd,&x,sizeof(int));
 /*
if(val == -1)
 {
	 close(fd);
	 return false;
 }
 */
 if(close(fd)== -1)
 {
	 return false;
 }
 cout<<"val:"<<val<<endl;
 cout<<"write:"<<x<<endl;
 return true;
}

bool Deserialize(const char *path )
{
	int fd = open(path,O_RDWR);
	if(fd == -1)
		return false;
	int reading = read(fd,&x,sizeof(int));
	if(reading == -1)
	{
		close(fd);
		return false;
	}
	if(close(fd) == -1)
		return false;
	cout<<"read:"<<x<<endl;
	return true;
}
};

int main(void)
{
	{
		test_1 ex(12);
		ex.Serialize("recored.txt");
	}
	{
		test_1 ex;
		ex.Deserialize("recored.txt");
		ex.file();
	}
	return 0;
}
