#include <iostream>
using namespace std;

void print(int start, int end)
{
	for (int i = start; i <= end; ++i)
		cout << i << " ";
	cout << endl;
}

void printConstNum(int num)
{
	if (num < 3)
		return;

	int last = 1;
	int forward = 2;
	int sum = last + forward;
	
	while (forward < num)
	{
		if (sum == num)
		{
			print(last, forward);
			++forward;
			sum += forward;
		}
		else if (sum > num)
		{
			sum -= last;
			++last;
		}
		else if (sum < num)
		{
			++forward;
			sum += forward;
		}
	}
}
int main(void)
{
	printConstNum(15);
	return 0;
}