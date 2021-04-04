#include <iostream>
using namespace std;

class complex
{
public:
	complex(double r = 0, double i = 0)
		: re(r), im(i)
	{ }
	int func(const complex& param)
	{
		return param.re + param.im;
	}
private:
	double re, im;
};

int main(void)
{
	complex c1(2, 1);
	complex c2;
	cout << c2.func(c1) << endl;
	return 0;
}