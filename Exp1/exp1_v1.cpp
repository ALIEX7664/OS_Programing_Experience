#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

class A
{
private:
	int x; //用于存储序列化和反序列化的数据
public:
	A()
	{
		int x = 0; //初始化
	}
	//显式声明构造函数,杜绝构造函数隐式转换，直接赋值方式，只能显示接受int类型
	explicit A(int in)
	{
		x = in;
	}
	virtual ~A() //虚构函数，继承关系构造，不用子类内存是不会被赋值的
	{
	}

public:
	bool Serialize(const char *path) const //序列化函数
	{
		//打开文件,保存文件描述符并设置权限位
		int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			perror("ser open error");
			return false;
		}

		if (write(fd, &x, sizeof(int)) == -1) //写入x值
		{
			perror("write error");
			close(fd);
			return false;
		}

		close(fd); //关闭文件描述符
		cout << "A write:" << x << endl;
		return true;
	}

	bool Deserialize(const char *path) //反序列化函数
	{
		int fd = open(path, O_RDWR); //打开文件,保存文件描述符
		if (fd == -1)
		{
			perror("deser open error");
			return false;
		}

		int readc = read(fd, &x, sizeof(int)); //读取文件内容并存入x中
		if (readc == -1)
		{
			perror("read error");
			close(fd);
			return false;
		}
		else if (readc == 0)
		{
			close(fd);
			return false;
		}

		close(fd); //关闭文件
		return true;
	}

	void ReturnVal() //返回数据成员内容,验证反序列化是否成功
	{
		cout << "A:" << x << endl;
	}
};

int main(void)
{
	{
		//定义A的成员变量，显式调用构造函数
		A a(12);

		//序列化
		if (a.Serialize("data.txt") == true)
			cout << "Serialize succeed" << endl;
		else
			cout << "Serialize error" << endl;
	}
	{
		//定义A的成员变量
		A a;

		//反序列化
		if (a.Deserialize("data.txt") == true)
			cout << "Deserialize succeed" << endl;
		else
			cout << "Deserialize error" << endl;

		//输出数据成员内容，验证过程是否正确
		a.ReturnVal();
	}
	return 0;
}
