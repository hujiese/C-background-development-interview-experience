#include <iostream>
#include <vector>
using namespace std;

template<class T> 
inline T Sigma(const T const * start, const T const * end)
{
	T total = T();
	while (start != end)
	{
		total += *start++;
	}
	return total;
}
int main(void)
{
	 //int array[4] = { 1, 2, 3, 4 };
	 //cout << Sigma(array, array + 4) << endl;

	char szNames[] = "abc";
	size_t nLength = strlen(szNames);
	char* p = szNames;
	char* q = szNames + nLength;
	printf("%d\n", Sigma(p, q));// 38 £¬ÓÐÒç³ö
	return 0;
}