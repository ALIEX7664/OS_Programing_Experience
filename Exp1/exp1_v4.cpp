#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

using namespace std;

class FA_Ser
{
public:
	FA_Ser()
	{
	}
	virtual ~FA_Ser() //virtual使之调用派生类析构函数
	{
	}

public:
	//声明为纯虚函数
	virtual bool Serialize(int fd) const = 0;  //类序列化函数
	virtual FA_Ser *Deserialize(int fd) = 0;   //反序列化函数
	virtual void GetType(int &type) const = 0; //获取类的类型值函数
	virtual void ReturnVal() = 0;			   //输出变量函数
};

class A : public FA_Ser //A类继承基类FA_Ser
{
private:
	int val;
	int val2;

public:
	//定义构造函数和析构函数
	A()
	{
		val = 0;
		val2 = 0;
	}
	explicit A(int in, int in2)
	{
		val = in;
		val2 = in2;
	}
	virtual ~A()
	{
	}

public:
	bool Serialize(int fd) const //序列化函数，主要执行写操作
	{
		//写操作，若出现错误返回false
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

	//反序列化函数，类型为基类指针，根据函数中返回A类指针
	FA_Ser *Deserialize(int fd)
	{
		A *p = new A; //动态分配A类指针

		//读操作，通过类指针的成员变量传入
		int readc = read(fd, &(p->val), sizeof(int));
		if (readc == -1 || readc == 0)
		{
			perror("val read error");
			close(fd);
			exit(0);
		}

		int readc2 = read(fd, &(p->val2), sizeof(int)); //读操作
		if (readc2 == -1 || readc2 == 0)
		{
			perror("val2 read error");
			close(fd);
			exit(0);
		}

		return p; //返回A类指针
	}

	void GetType(int &type) const //获取A的类型值
	{
		type = 0; //A的类型值为0
	}

	void ReturnVal() //验证执行过程是否成功
	{
		cout << "A:" << val << " " << val2 << endl;
	}
};

class B : public FA_Ser //B类继承基类FA_Ser
{
private:
	int x;
	int y;

public:
	//定义构造函数和析构函数
	B()
	{
		x = 0;
		y = 0;
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
	bool Serialize(int fd) const //序列化函数，主要执行写操作
	{
		//写操作，若出现错误返回false
		if (write(fd, &x, sizeof(int)) == -1)
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

	//反序列化函数，类型为基类指针，根据函数中返回B类指针
	FA_Ser *Deserialize(int fd)
	{
		B *p = new B; //动态分配B类指针

		//读操作，通过类指针的成员变量传入
		int readc = read(fd, &(p->x), sizeof(int));
		if (readc == -1 || readc == 0)
		{
			close(fd);
			exit(0);
		}

		int readc2 = read(fd, &(p->y), sizeof(int)); //读操作
		if (readc == -1 || readc == 0)
		{
			close(fd);
			exit(0);
		}

		return p; //返回A类指针
	}

	void GetType(int &type) const //获取B的类型值
	{
		type = 1; //B的类型值为1
	}

	void ReturnVal() //验证执行过程是否成功
	{
		cout << "B:" << x << " " << y << endl;
	}
};

class C : public FA_Ser //C类继承基类FA_Ser
{
private:
	int a;

public:
	//定义构造函数和析构函数
	C()
	{
		a = 0;
	}
	explicit C(int in)
	{
		a = in;
	}
	virtual ~C()
	{
	}

public:
	bool Serialize(int fd) const //序列化函数，主要执行写操作
	{
		//写操作，若出现错误返回false
		if (write(fd, &a, sizeof(int)) == -1)
		{
			perror("a write error");
			return false;
		}

		//写成功，输出写入的结果
		cout << "C write:" << a << endl;
		return true;
	}

	//反序列化函数，类型为基类指针，根据函数中返回C类指针
	FA_Ser *Deserialize(int fd)
	{
		C *p = new C; //动态分配C类指针

		//读操作，通过类指针的成员变量传入
		int readc = read(fd, &(p->a), sizeof(int));
		if (readc == -1 || readc == 0)
		{
			perror("a read error");
			close(fd);
			exit(0);
		}

		return p; //返回A类指针
	}

	void GetType(int &type) const //获取A的类型值
	{
		type = 2; //C的类型值为2
	}

	void ReturnVal() //验证执行过程是否成功
	{
		cout << "C:" << a << endl;
	}
};

class Serializer //序列化器，完成主要的序列化操作
{

private:
	//定义基类指针类型的vector容器，用于注册不同的派生类
	vector<FA_Ser *> reg;

public:
	Serializer()
	{
	}
	~Serializer()
	{
	}

public:
	//序列化操作，完成对不同派生类的写入文件操作
	bool Serialize(const char *path, const vector<FA_Ser *> &v) const
	{
		//打开文件
		int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			perror("ser open error");
			return false;
		}

		for (int i = 0; i < v.size(); i++)
		{
			int type;

			//调用相派生类的GetType函数，获取其类型值
			v[i]->GetType(type);
			//将类型值type写入文件中
			if (write(fd, &type, sizeof(int)) == -1)
			{
				perror("write type error");
				close(fd);
				return false;
			}
			//引用派生类的序列化函数
			if (v[i]->Serialize(fd) == false)
			{
				close(fd);
				return false;
			}
		}

		//序列化成功，输出执行成功语句
		cout << "Serialize vector succeed " << endl;
		close(fd);
		return true;
	}

	//反序列化操作，完成将文件内容读入不同派生类的操作
	bool Deserialize(const char *path, vector<FA_Ser *> &v)
	{
		//定义装入vector容器的派生类对象下标,以便执行反序列化
		int cnt = 0;
		int fd = open(path, O_RDWR); //以读写方式打开文件
		if (fd == -1)
		{
			perror("deser open error");
			return false;
		}

		while (1)
		{
			int nType, type;
			//读取类型标识，用于做比较
			int readc = read(fd, &nType, sizeof(int));
			if (readc == -1)
			{
				perror("nType read error");
				close(fd);
				return false;
			}
			else if (readc == 0) //读取到文件尾，返回成功
			{
				cout << "Deserialize succeed" << endl;
				close(fd);
				return true;
			}

			for (int i = 0; i < reg.size(); i++)
			{
				reg[i]->GetType(type); //获取注册的reg类的类型标识
				if (type == nType)
				{
					//将注册的reg类型装入vector容器中
					v.push_back(reg[i]);
					//调用vector容器里的派生类对象进行反序列化
					v[cnt] = v[cnt]->Deserialize(fd);
					break;
				}
			}
			cnt++; //vector下标+1
		}
	}

	void Register(FA_Ser *obj) //用于注册已经声明的派生类
	{
		reg.push_back(obj); //注册派生类对象
	}
};

int main(void)
{
	{
		//动态分配内存
		A *a = new A(3, 4);
		B *b = new B(12, 11);
		A *c = new A(22, 23);
		B *d = new B(7, 10);
		C *e = new C(9);

		//定义基类容器指针
		vector<FA_Ser *> vec;

		//将类相关变量放入容器
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(c);
		vec.push_back(d);
		vec.push_back(e);

		//定义序列化器
		Serializer s;

		//序列化操作
		s.Serialize("data.txt", vec);

		// 释放内存
		delete a;
		delete b;
		delete c;
		delete d;
		delete e;
	}
	{
		//定义派生类指针，用于保存相关类型
		A *a = new A;
		B *b = new B;
		C *c = new C;

		//定义基类容器指针
		vector<FA_Ser *> vec;

		//定义序列化器
		Serializer s;

		//将各种类型进行注册存储
		s.Register(a);
		s.Register(b);
		s.Register(c);

		//反序列化
		if (s.Deserialize("data.txt", vec) == true)
		{
			//利用C++多态性，输出相应类的函数，返回类私有变量
			for (int i = 0; i < vec.size(); i++)
			{
				vec[i]->ReturnVal();
			}
		}

		//释放内存
		delete a;
		delete b;
		delete c;
	}

	return 0;
}
