#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<vector>

using namespace std;

class CA_LL
{
 private:
	 int val;	//定义变量val
	 int val2;
 public:
	 
	 CA_LL() //初始化		
	 {
		int val = 0;
		int val2 = 0;
	 }
	 
	 explicit CA_LL(int in,int in2)
	 {
		 val = in;
		 val2 = in2;
	 }
	 //析构函数
	 ~CA_LL()
	 {	 
	 }

 public:
	 bool Serialize(int fd) const  //序列化，主要执行写操作
	 {

		  if(write(fd, &val, sizeof(int)) == -1)	//写操作，若出现错误关闭文件并返回
		  {
		 	cout << "val write error" << endl;	
			return false;
		  }

		  if(write(fd, &val2, sizeof(int)) == -1)
		  {
			  cout << "val2 write error" << endl;
			  return false;
		  
		  }

		   cout << "CA_LL write:" << val << " "<< val2  << endl;
		  return true;

		  

	 }

	 bool Deserialize(int fd) //反序列化，执行读操作
	 {
		
	 		
	         int readc,readc2;
		readc = read(fd, &val, sizeof(int));
	
	

		if(readc == -1 || readc == 0 ) 	
		 { 
			return false;
		 
		 }

		readc2 = read(fd,&val2,sizeof(int));

		if(readc2 == -1 || readc == 0)
		{
			return false;	
		}

		
		return true;

	 }

	 void  ReturnVal()
	 {
	 	cout  << val << " " << val2 <<endl; 	 
	 }
};

class CB_LL
{
 private:
	 int x;
	 int y;

 public:
	 CB_LL()
	 {
	  	int x = 0;
		int y = 0;
	 } 
	 
	 explicit CB_LL(int in, int in2)
	 {
		x = in;
	        y = in2;
	 }

	 ~CB_LL()
	 {
	 }

 public:
	bool Serialize(int fd)
	{
		if(write(fd, &x , sizeof(int)) == -1)
		{
			cout << "x wirte error" << endl;
			return false;
		}
		
		if(write(fd, &y, sizeof(int)) == -1)
		{
			cout << "y write error" << endl;
			return false;
		}
		
		cout << "CB_LL wirte:" << x << " " << y << endl;
		return true;
	}

	bool Deserialize(int fd)
	{
		int readc,readc2;

		readc = read(fd, &x, sizeof(int));
		if(readc == -1 || readc == 0)
		{
			return false;
		}
		
		readc2 = read(fd, &y, sizeof(int));
		if(readc == -1 || readc == 0)
		{
			return false;
		}

		return true;
		
	}

	void  ReturnVal()
	{
		cout   << x << " " << y << endl; 
	}
};

struct Serialized
{
	int nType;
	void *pObj;
};

class SerializerForCA_LLs
{


 public:
	SerializerForCA_LLs()
	{
	}
	~SerializerForCA_LLs()
	{
	}
	
 public:
	 bool Serialize(const char *pFilePath, const std::vector<Serialized> &v) const
	 {
		int fd;
		fd = open(pFilePath, O_RDWR | O_CREAT | O_TRUNC,0666);//打开文件，若文件不存在则创建，并清空文件内容

		 if(fd == -1)
		{
			cout << "ser open error" << endl;
			return false;
		}

		for(int i = 0 ; i < v.size(); i++)
		{
			
			if(v[i].nType == 0)
			{
				CA_LL *A;
				A =(CA_LL*)v[i].pObj;
                   

				write(fd, &v[i].nType, sizeof(int));

				if(A -> Serialize(fd) == false)
				{
					close(fd);
					return false;
				}
                                

			}
			else if(v[i].nType == 1)
			{
				CB_LL *B;//不能动态分配内存，会使指针指向发生变化而无法释放;
				B =(CB_LL*)v[i].pObj; //记得强制类型转换
				
				write(fd, &v[i].nType, sizeof(int));

				if(B -> Serialize(fd) == false)
				{
					close(fd);
					return false;
				}

			
			}
		}

		cout << "Serialize vector  succeed" << endl;

		return true;	//执行完毕无错误，返回
	 }

	 bool Deserialize(const char* pFilePath, std::vector<Serialized> &v) 
	 {
		 int fd;
		 fd = open(pFilePath, O_RDWR);
		 if(fd == -1)
		 {
			 cout << "open error" <<endl;
			 return false;
		 }

	
		 while(1)
		 {
			int r,Type;
			Serialized ser;

			r = read(fd, &Type ,sizeof(int));//最后一次循环会在这里读出0，所以return false 这里把-1，0拆开判断了

			if(r == -1)
		        {
			       return false;
		        }
			else if(r == 0)
			{
				cout << "Deserialize vector succeed" << endl;
				return true;
			}
			
			if(Type == 0)
			{
		
				CA_LL *A = new CA_LL();//此时只分配一个指针，并没有调用构造函数，所以要通过new来调用
				if(A -> Deserialize(fd) == true)
				{
					ser.nType = Type;
					ser.pObj = A ; //这里会出现一个问题，若A被delete了，那指向的分配内存也会被释放，此时pobj若直接指向A,那么只会存在随机值
					v.push_back(ser);
				}

			

			
			}
			else if(Type == 1)
			{
				CB_LL *B = new CB_LL();

				if(B -> Deserialize(fd) == true)
				{
					ser.nType = Type;
					ser.pObj = B;
					v.push_back(ser);
				}

			}
			else
			{
				break;
			}
		 }
		
			
		
		 return true;
		 
	 }

};


int main()
{
       {
	vector<Serialized> vec;
	Serialized ser;
        Serialized ser1;
	Serialized ser2;
      	
	CA_LL a(10,12);
	ser.nType = 0;
	ser.pObj = &a;
	vec.push_back(ser);

	CB_LL b(1,2);
	ser1.nType = 1;
	ser1.pObj = &b;
	vec.push_back(ser1);

	CB_LL b2(3,4);
	ser2.nType = 1;
	ser2.pObj = &b2;
	vec.push_back(ser2);

        CA_LL a2(11,20);
	Serialized ser3;
	ser3.pObj = &a2;
	ser3.nType = 0;
	vec.push_back(ser3);
   	
	SerializerForCA_LLs s;
	s.Serialize("data.txt",vec);
       }

       {
	vector<Serialized> vec;
	SerializerForCA_LLs s;

	s.Deserialize("data.txt",vec);
	for(int i = 0 ; i < vec.size() ; i++)
	{
		if(vec[i].nType == 0)
		{
	 		cout << "Type:A " <<endl;


			CA_LL *a;
			a = (CA_LL*)vec[i].pObj;
			a -> ReturnVal();
			

		}
		else if(vec[i].nType == 1)
		{
			cout << "Type:B " << endl;

			CB_LL *b;
			b = (CB_LL*)vec[i].pObj;
			b -> ReturnVal();
			
		}
	}

       }



	return 0;
}

