#include <iostream>
#include <stdlib.h>
using namespace std;

class Object
{
public:
	Object() {
		std::cout << "Object()" << std::endl;
	}
	~Object(){
	}

	Object(const Object& object) {
		std::cout << "Object(const Object&)" << std::endl;
	}

	Object(const Object&& object) {
		std::cout << "Object(const Object&&)" << std::endl;

	}
	Object getObject(void) {
		Object obj;
		return obj;
	}
};

int main(void)
{
	Object o1;
	//Object o2 = o1.getObject();
	Object o3 = move(o1);

	return 0;
}