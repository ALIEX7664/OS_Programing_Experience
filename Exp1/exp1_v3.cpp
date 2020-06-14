#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

using namespace std;

class A
{
private:
	//定义成员变量
	int val;
	int val2;

public:
	//定义析构函数和构造函数
	A()
	{
		int val = 0;
		int val2 = 0;
	}
	explicit A(int in, int in2)
	{
		val = in;
		val2 = in2;
	}
	~A()
	{
	}

public:
	bool Serialize(int fd) const //序列化，主要执行写操作
	{
		//写操作，若出现错误返回
		if (write(fd, &val, sizeof(int)) == -1)
		{
			perror("val write error");
			return false;
		}

		if (write(fd, &val2, sizeof(int)) == -1) //写操作
		{
			perror("val2 write error");
			return false;
		}
		//写成功，输出写入的结果
		cout << "A write:" << val << " " << val2 << endl;
		return true;
	}

	bool Deserialize(int fd) //反序列化，主要执行读操作
	{
		//读操作，若出现错误或读结束关闭文件并返回
		int readc = read(fd, &val, sizeof(int));
		if (readc == -1)
		{
			perror("read error"); //错误输出
			close(fd);			  //关闭文件
			return false;
		}
		else if (readc == 0)
		{
			close(fd);
			return false;
		}

		int readc2 = read(fd, &val2, sizeof(int)); //读操作
		if (readc2 == -1)
		{
			perror("read error"); //错误输出
			close(fd);
			return false;
		}
		else if (readc2 == 0)
		{
			close(fd);
			return false;
		}

		return true; //执行成功，返回true
	}

	void ReturnVal() //用于验证过程是否成功
	{
		cout << "A:" << val << " " << val2 << endl;
	}
};

class B
{
private:
	int x;
	int y;

public:
	B()
	{
		int x = 0;
		int y = 0;
	}
	explicit B(int in, int in2)
	{
		x = in;
		y = in2;
	}
	~B()
	{
	}

public:
	bool Serialize(int fd) //序列化，主要执行写操作
	{
		if (write(fd, &x, sizeof(int)) == -1) //写操作
		{
			perror("x write error");
			return false;
		}

		if (write(fd, &y, sizeof(int)) == -1) //写操作
		{
			perror("y write error");
			return false;
		}
		//写成功，输出写入的结果
		cout << "B write:" << x << " " << y << endl;
		return true;
	}

	bool Deserialize(int fd) //反序列化，主要执行读操作
	{
		int readc = read(fd, &x, sizeof(int)); //读操作
		if (readc == -1)
		{
			perror("read error"); //错误输出
			close(fd);			  //关闭文件
			return false;
		}
		else if (readc == 0)
		{
			close(fd);
			return false;
		}

		int readc2 = read(fd, &y, sizeof(int)); //读操作
		if (readc2 == -1)
		{
			perror("read error"); //错误输出
			close(fd);			  //关闭文件
			return false;
		}
		else if (readc2 == 0)
		{
			close(fd);
			return false;
		}

		return true; //执行成功，返回true
	}

	void ReturnVal() //用于验证过程是否成功
	{
		cout << "B:" << x << " " << y << endl;
	}
};

struct Serialized //定义存储A,B类类型的结构体
{
	int nType;	//定义类型区分A,B,A:0 B:1
	void *pObj; //定义void型指针,通过强制类型转换保存A类和B类成员的地址
};

class Serializer //序列化器，完成主要的序列化和反序列化操作
{
public:
	Serializer()
	{
	}
	~Serializer()
	{
	}

public:
	//序列化函数，完成对A类与B类的序列化操作
	bool Serialize(const char *path, const std::vector<Serialized> &v) const
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

			if (v[i].nType == 0) //判断为A类型
			{
				//定义A类指针
				A *a;
				//强转v[i]中结构体变量pObj为A类指针类型，并赋给指针a
				a = (A *)v[i].pObj;

				write(fd, &v[i].nType, sizeof(int)); //将A类型类型写入

				if (a->Serialize(fd) == false) //执行A类的序列化操作
				{
					close(fd);
					return false;
				}
			}
			else if (v[i].nType == 1)
			{
				//定义B类指针
				B *b;
				//强转v[i]中结构体变量pObj为B类指针类型，并赋给指针b
				b = (B *)v[i].pObj;

				write(fd, &v[i].nType, sizeof(int)); //将B类型类型写入

				if (b->Serialize(fd) == false) //执行B类的序列化操作
				{
					close(fd);
					return false;
				}
			}
		}

		cout << "Serialize vector  succeed" << endl; //输出执行成功语句
		close(fd);
		return true; //执行完毕无错误，返回
	}

	//反序列化函数，完成对A类与B类的反序列化操作
	bool Deserialize(const char *path, std::vector<Serialized> &v)
	{
		int fd = open(path, O_RDWR); //打开文件，保存文件描述符
		if (fd == -1)
		{
			perror("deser open error");
			return false;
		}

		while (1)
		{
			int Type;		//用于存储文件中读取的类的类型
			Serialized ser; //用于保存文件内容中的类的类型和类的对象

			//读取首字节，判断类型
			int readc = read(fd, &Type, sizeof(int));
			if (readc == -1)
			{
				perror("read error");
				close(fd);
				return false;
			}
			else if (readc == 0) //最后一次循环读出0字节，表示读取完毕
			{
				break; //跳出循环
			}

			if (Type == 0)
			{
				A *a = new A(); //动态分配A类指针

				if (a->Deserialize(fd) == true) //对a进行反序列化
				{
					ser.nType = Type; //保存读取的Type类型
					ser.pObj = a;	  //保存经过反序列化的a指针

					//将结构体ser装入vector容器中保存
					v.push_back(ser);
				}
			}
			else if (Type == 1)
			{
				B *b = new B(); //动态分配B类指针

				if (b->Deserialize(fd) == true)
				{
					ser.nType = Type; //保存读取的Type类型
					ser.pObj = b;	  //保存经过反序列化的b指针

					//将结构体ser装入vector容器中保存
					v.push_back(ser);
				}
			}
			else
			{
				return false; //读取类型不属于任何已存在类的类型
			}
		}

		cout << "Deserialize vector succeed" << endl; //输出执行成功语句
		close(fd);
		return true;
	}
};

int main(void)
{
	{
		//定义vector容器和Serialized结构体变量
		vector<Serialized> vec;
		Serialized ser;
		Serialized ser1;
		Serialized ser2;
		Serialized ser3;
		Serialized ser4;

		A a(10, 12);		//定义A实例对象
		ser.nType = 0;		//存储A的类型值0
		ser.pObj = &a;		//使ser的pObj指针指向A实例对象
		vec.push_back(ser); //将ser装入vector容器中

		B b(1, 2);			 //定义B实例对象
		ser1.nType = 1;		 //存储B的类型值1
		ser1.pObj = &b;		 //使ser1的pObj指针指向A实例对象
		vec.push_back(ser1); //将ser1装入vector容器中

		B b2(3, 4);
		ser2.nType = 1;
		ser2.pObj = &b2;
		vec.push_back(ser2);

		A a2(11, 20);
		ser3.pObj = &a2;
		ser3.nType = 0;
		vec.push_back(ser3);

		B b3(7, 9);
		ser4.nType = 1;
		ser4.pObj = &b3;
		vec.push_back(ser4);

		Serializer s;				  //定义序列化器s
		s.Serialize("data.txt", vec); //执行序列化函数
	}

	{
		//定义vector容器vec和序列化器s
		vector<Serialized> vec;
		Serializer s;

		//执行反序列化，通过vector容器获得相应的内容
		s.Deserialize("data.txt", vec);
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i].nType == 0) //若类型为A
			{
				//定义A类指针a
				A *a;
				//强制转换vector容器中的pObj为A类指针类型,并存入a中
				a = (A *)vec[i].pObj;
				//输出结果，验证序列化过程正确性
				a->ReturnVal();
			}
			else if (vec[i].nType == 1) //若类型为A
			{
				//定义B类指针b
				B *b;
				//强制转换vector容器中的pObj为B类指针类型，并存入b中
				b = (B *)vec[i].pObj;
				//输出结果，验证序列化过程正确性
				b->ReturnVal();
			}
		}
	}
	return 0;
}
