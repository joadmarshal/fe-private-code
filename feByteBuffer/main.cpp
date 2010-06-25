#include "feByteBuffer.h"
#include <iostream>
int main()
{
	ByteBuffer buff(1000);
	int i=1;
	float f=1.0f;
	int c='c';
	buff<<1;
	buff<<1.0f;
	buff<<'c';
	
	i=0;
	f=0.0f;
	c=0;
	buff>>i>>f>>c;
	std::cout<<i<<" "<<f<<" "<<(char)c;

}