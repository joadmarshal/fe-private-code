namespace fesl
{
	class Inet_addr
	{
	public:
		Inet_addr()
		{
			ZeroMemory(&__sockaddr,sizeof(__sockaddr));
			__sockaddr.sin_family=AF_INET;
		}
		Inet_addr(unsigned short port,const char *str_ip=0)
		{
			ZeroMemory(&__sockaddr,sizeof(__sockaddr));
			__sockaddr.sin_family=AF_INET;
			set(port,str_ip);
		}
		sockaddr_in & set(unsigned short port,const char *str_ip=0)
		{
			if(str_ip)
				__sockaddr.sin_addr.s_addr = inet_addr(str_ip);
			else
				__sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			__sockaddr.sin_port=htons(port);
			return __sockaddr;
		}
	private:
		sockaddr_in __sockaddr;
// 	public:
// 	static const int addrsize = sizeof(sockaddr_in);
	};
}