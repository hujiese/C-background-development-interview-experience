#include "Image.h"
#include <string>
#include <iostream>
#include <memory>
#include "RealImage.h"
#include "ProxyImage.h"
using namespace std;

int main(void)
{
	shared_ptr<Image> image(new ProxyImage("test_10mb.jpg"));

	//ͼ�񽫴Ӵ��̼���
	image->display();
	cout << endl;
	//ͼ�񽫲���Ӵ��̼���
	image->display();

	return 0;
}