#include <iostream>
using namespace std;

void* Memcpy(void *dst, const void *src, size_t size);

int main(int argc, char *argv[])
{
	char buf[100] = "abcdefghijk";
	//memcpy(buf + 2, buf, 5);
	Memcpy(buf + 2, buf, 5);
	printf("%s\n", buf + 2);
	return 0;
}

void* Memcpy(void *dst, const void *src, size_t size)
{
	if (dst == nullptr || src == nullptr)
		return nullptr;

	char* pdst = (char*)dst;
	char* psrc = (char*)src;

	if (psrc < pdst && (psrc + size > pdst))
	{
		pdst = pdst + size - 1;
		psrc = psrc + size - 1;

		while (size--)
			*pdst-- = *psrc--;
	}
	else
	{
		while (size--)
		{
			*pdst++ = *psrc++;
		}
	}

	return dst;
}