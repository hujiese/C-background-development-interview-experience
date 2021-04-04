#include <iostream>
#include <list>
#define __HEAD_NURSE__
using namespace std;

class Doctor
{
public:
	void treat_eye()
	{
		cout << "医生 治疗 眼科病" << endl;
	}

	void treat_nose()
	{
		cout << "医生 治疗 鼻科病" << endl;
	}
};

class Command
{
public:
	virtual void treat() = 0;
};

class CommandTreatEye : public Command
{
public:
	CommandTreatEye(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_eye();
	}
private:
	Doctor *m_doctor;
};


class CommandTreatNose : public Command
{
public:
	CommandTreatNose(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_nose();
	}
private:
	Doctor *m_doctor;
};


class BeautyNurse
{
public:
	BeautyNurse(Command *command) :command(command)
	{
	}
public:
	void SubmittedCase() //提交病例 下单命令
	{
		command->treat();
	}
private:
	Command *command;
};

//护士长
class HeadNurse 
{
public:
	HeadNurse()
	{
		m_list.clear();
	}
	
public:
	void setCommand(Command *command)
	{
		m_list.push_back(command);
	}
	void SubmittedCase() //提交病例 下单命令
	{
		for (list<Command *>::iterator it=m_list.begin(); it!=m_list.end(); it++)
		{
			(*it)->treat();
		}
	}
private:
	list<Command *> m_list;
};

#ifndef __HEAD_NURSE__
int main(void)
{
	//护士提交简历 给以上看病
	BeautyNurse		*beautynurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command  = NULL;

	doctor = new Doctor() ;

	//command = new CommandTreatEye(doctor); //shift +u //转小写 shift+ctl + u转大写
	command = new CommandTreatNose(doctor); //shift +u //转小写 shift+ctl + u转大写
	beautynurse = new BeautyNurse(command);
	beautynurse->SubmittedCase();
	
	delete doctor;
	delete command;
	delete beautynurse;
	return 0;
}
#else
// 通过护士长 批量的下单命令
int main(void)
{
	//护士提交简历 给以上看病
	HeadNurse		*headnurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command1  = NULL;
	Command			*command2  = NULL;

	doctor = new Doctor() ;

	command1 = new CommandTreatEye(doctor); 
	command2 = new CommandTreatNose(doctor); 

	headnurse = new HeadNurse(); //new 护士长
	headnurse->setCommand(command1);
	headnurse->setCommand(command2);

	headnurse->SubmittedCase(); // 护士长 批量下单命令

	delete doctor;
	delete command1;
	delete command2;
	delete headnurse;
	return 0;
}
#endif