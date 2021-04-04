
#include <iostream>
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
	//1 医生直接看病
	/*
	Doctor *doctor = new Doctor ;
	doctor->treat_eye();
	delete doctor;
	*/

	//2 通过一个命令 医生通过 命令 治疗 治病
	Doctor *doctor = new Doctor() ;
	CommandTreatEye * commandtreateye = new CommandTreatEye(doctor); //shift +u //转小写 shift+ctl + u转大写
	commandtreateye->treat();
	delete commandtreateye;
	delete doctor;
	return 0;
}