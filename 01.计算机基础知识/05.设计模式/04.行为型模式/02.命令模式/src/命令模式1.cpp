
#include <iostream>
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


class CommandTreatEye
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


class CommandTreatNose
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



int main(void)
{
	//1 ҽ��ֱ�ӿ���
	/*
	Doctor *doctor = new Doctor ;
	doctor->treat_eye();
	delete doctor;
	*/

	//2 ͨ��һ������ ҽ��ͨ�� ���� ���� �β�
	Doctor *doctor = new Doctor() ;
	CommandTreatEye * commandtreateye = new CommandTreatEye(doctor); //shift +u //תСд shift+ctl + uת��д
	commandtreateye->treat();
	delete commandtreateye;
	delete doctor;
	return 0;
}