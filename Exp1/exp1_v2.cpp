#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

using namespace std;

class A
{
private:
	int x; //定义变量val

public:
	//定义构造函数和析构函数
	A()
	{
		int x = 0;
	}
	explicit A(int in)
	{
		x = in;
	}
	~A()
	{
	}

public:
	bool Serialize(int fd) const //序列化函数，主要执行写操作
	{
		//写操作，若出现错误关闭文件并返回
		if (write(fd, &x, sizeof(int)) == -1)
		{
			perror("write error");
			close(fd);
			return false;
		}

		cout << "A write:" << x << endl;
		return true;
	}

	bool Deserialize(int fd) //反序列化，主要执行读操作
	{
		//读操作，若出现错误关闭文件并返回
		int readc = read(fd, &x, sizeof(int));
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

		return true;
	}

	void ReturnVal() //用于验证过程是否成功
	{
		cout << "A:" << x << endl;
	}
};

class Serializer //序列化器
{

public:
	Serializer()
	{
	}
	~Serializer()
	{
	}

public:
	//序列化函数，用于vector容器序列化
	bool Serialize(const char *path, const std::vector<A> &v) const
	{
		//打开文件，若文件不存在则创建，并清空文件内容
		int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			perror("ser open error");
			return false;
		}

		for (int i = 0; i < v.size(); i++)
		{
			//执行vector容器数组对应的对象方法，若执行时出错则返回false
			if (v[i].Serialize(fd) == false)
				return false;
		}

		cout << "Serialize vector succeed" << endl;
		close(fd);
		return true; //执行完毕无错误，返回
	}

	//反序列化函数，用于vector容器反序列化
	bool Deserialize(const char *path, std::vector<A> &v)
	{
		int fd = open(path, O_RDWR); //打开文件，获得文件描述符
		if (fd == -1)
		{
			perror("deser open error");
			return false;
		}

		while (1)
		{
			A deser;
			//调用deser对象的反序列化函数，读取文件内容
			if (deser.Deserialize(fd) == true)
				//将读取到内容的对象装入vector容器中
				v.push_back(deser);
			else
				break; //读取完毕或出错，跳出循环
		}

		cout << "Deserialize vector succeed" << endl;
		return true;
	}
};

int main(void)
{
	{
		//定义用于存储的实例化对象和vector容器
		A a(2), b(4), c(6);
		vector<A> vec;

		//将对象装入容器中
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(c);

		//定义序列化器并执行序列化操作
		Serializer ser;
		ser.Serialize("data.txt", vec);
	}
	{
		//定义vector容器,用于存储反序列化的内容
		vector<A> vec;

		//定义序列化器并执行反序列化操作
		Serializer ser;
		ser.Deserialize("data.txt", vec);

		//执行容器中对象的对应方法，验证过程是否成功
		for (int i = 0; i < vec.size(); i++)
		{
			vec[i].ReturnVal();
		}
	}

	return 0;
}
