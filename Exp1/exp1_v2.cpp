#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<vector>

using namespace std;

class CA_LL
{
 private:
	 int val;	//定义变量val

 public:
	 //定义析构函数
	 CA_LL()		
	 {
		int val = 0;
	 }
	 
	 explicit CA_LL(int in)
	 {
		 val = in;
	 }

	 ~CA_LL()
	 {	 
	 }

 public:
	 bool Serialize(int fd) const  //序列化，主要执行写操作
	 {

		  if(write(fd, &val, sizeof(int)) == -1)	//写操作，若出现错误关闭文件并返回
		  {
		  	cout << "write error" << endl;	
			close(fd);
			return false;
		  }

		  cout << "write:" << val << endl;
		  return true;

	 }

	 bool Deserialize(int fd) //反序列化，执行读操作
	 {	

		 int readVal;
		 readVal = read(fd, &val, sizeof(int));
		if(readVal == -1 || readVal == 0 )  
		{
			 close(fd);
			 return false;
		 
		 }

		
		return true;

	 }

	 int ReturnVal()
	 {
	 	return val;	 
	 }

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
	 bool Serialize(const char *pFilePath, const std::vector<CA_LL> &v) const
	 {
		int fd;
		fd = open(pFilePath, O_RDWR | O_CREAT | O_TRUNC,0666);//打开文件，若文件不存在则创建，并清空文件内容
		cout<< fd << endl;
		 if(fd == -1)
		{
			cout << "ser open error" << endl;
			return false;
		}

		for(int i = 0 ; i < v.size() ; i++)
		{
			 if(v[i].Serialize(fd) == false)	//执行vector容器数组对应的对象方法，若执行时出错则返回false
			 {
				 return false;
			 }

		}

		cout << "Serialize vector  succeed" << endl;

		return true;	//执行完毕无错误，返回
	 }

	 bool Deserialize(const char* pFilePath, std::vector<CA_LL> &v) 
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
			CA_LL deser;
			if(deser.Deserialize(fd) == true)
			{
				v.push_back(deser);
			}
			else
			{
				break;
			}
		 
		 }
		

		 cout << "Deserialize vector succeed" << endl;
		 return true;
	 }

};


int main()
{
	{
		CA_LL a(2),b(4),c(6);
		vector<CA_LL> vec;
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(c);
		SerializerForCA_LLs ser;
		ser.Serialize("data.txt",vec);

	}
	{
		vector<CA_LL> vec;
		SerializerForCA_LLs ser;
		ser.Deserialize("data.txt",vec);
		for(int i = 0;i < vec.size() ;i++)
		{
			cout << "read:" << vec[i].ReturnVal() << endl;
		}
	}

	return 0;
}
