#include <iostream>
#include <list>
#define __HEAD_NURSE__
using namespace std;

class Doctor
{
public:
	void treat_eye()
	{
		cout << "ҽ�� ���� �ۿƲ�" << endl;
	}

	void treat_nose()
	{
		cout << "ҽ�� ���� �ǿƲ�" << endl;
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
	void SubmittedCase() //�ύ���� �µ�����
	{
		command->treat();
	}
private:
	Command *command;
};

//��ʿ��
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
	void SubmittedCase() //�ύ���� �µ�����
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
	//��ʿ�ύ���� �����Ͽ���
	BeautyNurse		*beautynurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command  = NULL;

	doctor = new Doctor() ;

	//command = new CommandTreatEye(doctor); //shift +u //תСд shift+ctl + uת��д
	command = new CommandTreatNose(doctor); //shift +u //תСд shift+ctl + uת��д
	beautynurse = new BeautyNurse(command);
	beautynurse->SubmittedCase();
	
	delete doctor;
	delete command;
	delete beautynurse;
	return 0;
}
#else
// ͨ����ʿ�� �������µ�����
int main(void)
{
	//��ʿ�ύ���� �����Ͽ���
	HeadNurse		*headnurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command1  = NULL;
	Command			*command2  = NULL;

	doctor = new Doctor() ;

	command1 = new CommandTreatEye(doctor); 
	command2 = new CommandTreatNose(doctor); 

	headnurse = new HeadNurse(); //new ��ʿ��
	headnurse->setCommand(command1);
	headnurse->setCommand(command2);

	headnurse->SubmittedCase(); // ��ʿ�� �����µ�����

	delete doctor;
	delete command1;
	delete command2;
	delete headnurse;
	return 0;
}
#endif