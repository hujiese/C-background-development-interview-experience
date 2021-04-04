#include <iostream>

using namespace std;

void test_malloc_free(void)
{
	void* p1 = malloc(512);
	free(p1);
}

void test_new_delete(void)
{
	int* p1 = new int;
	delete p1;
}

void test_operator_new_delete(void)
{
	void* p1 = ::operator new(512);
	operator delete(p1);
}


void test_placement_new_delete(void)
{
#ifdef _MSC_VER
	int* p1 = allocator<int>().allocate(3, (int*)0);// (int*)0 Œﬁ”√
	allocator<int>().deallocate(p1, 3);
#endif

#ifdef __GNUC__
	void* p2 = alloc::allocate(512);
	alloc::deallocate(p2, 512);
#endif
}

int main(void)
{
	test_malloc_free();
	test_new_delete();
	test_operator_new_delete();
	test_placement_new_delete();
	return 0;
}