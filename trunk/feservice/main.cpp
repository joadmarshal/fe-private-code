#include "myservice.h"
int main(int argc ,char **argv)
{
	MyService ms("����������");
	if ( (argc > 1) &&
		((*argv[1] == '-') || (*argv[1] == '/')) )
	{
		if ( _stricmp( "debug", argv[1]+1 ) == 0 )
		{
			//��������
			ms.debugservice();
			return TRUE;
		}
	}
	else
	{
		ms.startservice();
	}
	return 0;
}