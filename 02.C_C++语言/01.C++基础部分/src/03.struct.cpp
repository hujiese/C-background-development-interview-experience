#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<string.h>
#include<cstring>

using namespace std;
struct student{ //����һ���ṹ������student
	int num;
	char name[20];
	int age;
};//�����һ���ֺ�
int main(){
	struct student stu1;//����һ��student���͵ı���stu1
	student stu2;//����ʱҲ���Բ���struct
	stu1.num = 1;//������st1��numԪ�ظ�ֵ
	char temp[20] = "Xiao ming";
	strncpy(stu1.name, temp, strlen(temp));
	stu1.age = 10;
	cout << stu1.num << " " << stu1.name << " " << stu1.age << endl;
	student *stu3 = &stu1;//stu3�ǽṹ���ָ�룬ָ��stu1
	(*stu3).num = 2;//stu1��numֵ���޸ĳ���2��
	cout << stu1.num << " " << stu1.name << " " << stu1.age << endl;
	return 0;
}
