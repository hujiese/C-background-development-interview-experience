#include <iostream>
#include <list>
#include <string>
#include "Subject.h"
#include "F_Observer.h"
#include "S_Observer.h"
#include "T_Observer.h"
using namespace std;

int main(void)
{
	shared_ptr<Observer> fObserver(new F_Observer());
	shared_ptr<Observer> sObserver(new S_Observer());
	shared_ptr<Observer> tObserver(new T_Observer());
	Subject subject;
	subject.addAttach(fObserver);
	subject.addAttach(sObserver);
	subject.addAttach(tObserver);
	subject.setMsg("msg change");

	return 0;
}