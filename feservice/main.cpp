#include "myservice.h"
int main(int argc ,char **argv)
{
	MyService ms("����һ����");
	ms.startservice(argc,argv);
	return 0;
}