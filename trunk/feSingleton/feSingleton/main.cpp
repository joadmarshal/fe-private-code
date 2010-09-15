#include "feSingleton.h"
#include <stdio.h>
class A
{
public:
	A(){}
	~A()
	{
		printf("ggfgfg");
	}
	static A& Instance2(){return *(new A);}
	void afaf()
	{
		printf("afaf\n");
	}
	static A *aaa;
};
#define sA FeSingleton<A>::Instance()
#define sA2 A::Instance2()
int main()
{
	sA2.
	return 0;
}