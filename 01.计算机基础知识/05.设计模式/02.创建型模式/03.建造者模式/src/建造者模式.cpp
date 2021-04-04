
#include <iostream>
using namespace std;

#include "string"

class House
{
public:
	void setDoor(string door)
	{
		this->m_door = door;
	}

	void setWall(string wall)
	{
		this->m_wall = wall;
	}

	void setWindow(string window)
	{
		this->m_window = window;
	}

	string getDoor()
	{
		cout << m_door << endl;
		return this->m_door ;
	}

	string getWall()
	{
		cout << m_wall << endl;
		return this->m_wall;
	}

	string getWindow()
	{
		cout << m_window << endl;
		return m_window;
	}

private:
	string	m_door;
	string	m_wall;
	string	m_window;
};

class  Builder
{
public:
	virtual void buildWall() = 0;
	virtual void buildDoor() = 0;
	virtual void buildWindow() = 0;
	virtual House* getHouse() = 0;
};

//��Ԣ���̶�
class  FlatBuilder : public Builder
{
public:
	FlatBuilder()
	{
		m_house = new House();
	}
	virtual void buildWall()
	{
		m_house->setWall(" flat wall");
	}

	virtual void buildDoor()
	{
		m_house->setDoor("flat door");
	}

	virtual void buildWindow()
	{
		m_house->setWindow("flat window");
	}

	virtual House* getHouse()
	{
		return m_house;
	}
private:
	House *m_house;
};

//���� villa ���̶�
class  VillaBuilder : public Builder
{
public:
	VillaBuilder()
	{
		m_house = new House();
	}
	virtual void buildWall()
	{
		m_house->setWall(" villa wall");
	}

	virtual void buildDoor()
	{
		m_house->setDoor("villa door");
	}

	virtual void buildWindow()
	{
		m_house->setWindow("villa window");
	}

	virtual House* getHouse()
	{
		return m_house;
	}
private:
	House *m_house;
};

//���ʦ(ָ����) �������߼�  
//������ �ɾ���Ļ�
class Director 
{
public:
	Director( Builder * build)
	{
		m_build = build;
	}
	void Construct()
	{
		m_build->buildWall();
		m_build->buildWindow();
		m_build->buildDoor();
	}
private:
	 Builder * m_build;
};


int main(void)
{
	House		*house  = NULL;
	Builder		*builder = NULL;
	Director	*director = NULL;

	// ��һ����������Ĺ��̶�
	builder = new VillaBuilder();

	//���ʦ ָ�� ���̶� �ɻ�
	director = new Director(builder);
	director->Construct(); 
	house  =  builder->getHouse();
	house->getWindow();
	house->getDoor();
	delete house;
	delete builder;

	//�� FlatBuilder ��Ԣ
	builder = new FlatBuilder;
	director = new Director(builder);
	director->Construct(); 
	house  =  builder->getHouse();
	house->getWindow();
	house->getDoor();
	delete house;
	delete builder;


	delete director;
	return 0;
}