#include <iostream>
#include <vector>
using namespace std;

// ›Õ»°ºº ıTraits
template <typename T>
class SigmaTraits{};

template <>
class SigmaTraits<char>{
public:
	typedef int ReturnType;
};

template <>
class SigmaTraits<short>{
public:
	typedef int ReturnType;
};

template <>
class SigmaTraits<unsigned int>{
public:
	typedef unsigned long ReturnType;
};

template <>
class SigmaTraits<float>{
public:
	typedef double ReturnType;
};

template<class T>
inline typename SigmaTraits<T>::ReturnType Sigma(const T const * start, const T const * end)
{
	typedef typename SigmaTraits<T>::ReturnType ReturnType;
	ReturnType s = ReturnType();
	while (start != end)
	{
		s += *start++;
	}
	return s;
}
int main(void)
{
	char szNames[] = "abc";
	size_t nLength = strlen(szNames);
	char* p = szNames;
	char* q = szNames + nLength;
	printf("%d\n", Sigma(p, q));// 294
	return 0;
}