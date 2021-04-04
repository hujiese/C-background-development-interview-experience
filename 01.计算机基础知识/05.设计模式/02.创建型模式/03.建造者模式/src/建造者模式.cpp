
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

//公寓工程队
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

//别墅 villa 工程队
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

//设计师(指挥者) 负责建造逻辑  
//建筑队 干具体的活
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

	// 请一个建造别墅的工程队
	builder = new VillaBuilder();

	//设计师 指挥 工程队 干活
	director = new Director(builder);
	director->Construct(); 
	house  =  builder->getHouse();
	house->getWindow();
	house->getDoor();
	delete house;
	delete builder;

	//请 FlatBuilder 公寓
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