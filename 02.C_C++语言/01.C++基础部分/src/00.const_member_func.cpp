#include <iostream>
using namespace std;

class complex
{
public:
	complex(double r = 0, double i = 0)
		: re(r), im(i)
	{ }
	complex& operator += (const complex&);
	double real() const { return re; }
	double imag() const { return im; }
private:
	double re, im;
};

int main(void)
{
	complex c1(2, 1);
	cout << c1.real() << endl;
	cout << c1.imag() << endl;
	return 0;
}