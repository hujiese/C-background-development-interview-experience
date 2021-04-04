## 组合模式 ##

Composite模式也叫组合模式，是构造型的设计模式之一。通过递归手段来构造树形的对象结构，并可以通过一个对象来访问整个对象树。

### 一、角色和职责 ###

![](https://i.imgur.com/jzha6Rr.png)

	#include <iostream>
	#include <string>
	#include <list>
	using namespace std;
	
	class IFile
	{
	public:
		virtual void display() = 0;
		virtual int add(IFile *ifile) = 0;
		virtual int remove(IFile *ifile) = 0;
		virtual list<IFile *>* getChild() = 0;
	};
	
	//文件结点 
	class File : public IFile
	{
	public:
		File(string name)
		{
			m_name = name;
		}
		virtual void display()
		{
			cout << m_name << endl;
		}
	
		virtual int add(IFile *ifile)
		{
			return -1;
		}
	
		virtual int remove(IFile *ifile)
		{
			return -1;
		}
	
		virtual list<IFile *>* getChild()
		{
			return NULL;
		}
	private:
		string m_name;
	};
	
	//目录 结点 
	class Dir : public IFile
	{
	public:
		Dir(string name)
		{
			m_name = name;
			m_list = new list<IFile *>;
			m_list->clear();
		}
		virtual void display()
		{
			cout << m_name << endl;
		}
	
		virtual int add(IFile *ifile)
		{
			m_list->push_back(ifile);
			return 0;
		}
	
		virtual int remove(IFile *ifile)
		{
			m_list->remove(ifile);
			return 0;
		}
	
		virtual list<IFile *>*  getChild()
		{
			return m_list;
		}
	private:
		string m_name;
		list<IFile *>  *m_list;
	};
	
	int main(void)
	{
		Dir *root = new Dir("C");
		//root->display();
	
		Dir *dir1 = new Dir("111dir");
		File *aaafile = new File("aaa.txt");
	
		//获取root结点下的 孩子集合
		list<IFile *>  *mylist =  root->getChild();
	
		root->add(dir1);
		root->add(aaafile);
	
		//  (111dir)   (aaa.txt)
		//				▲
		for ( list<IFile *>::iterator it=mylist->begin(); it!=mylist->end(); it++ )
		{
			(*it)->display();
		}
	
		return 0;
	}

编译运行结果如下：

![](https://i.imgur.com/tnPIoKY.png)

### 三、案例扩展 ###

有一个类 Employee，该类被当作组合模型类。CompositePatternDemo，演示类使用 Employee 类来添加部门层次结构，并打印所有员工。

![](https://i.imgur.com/ONCMjyI.jpg)