namespace fesl
{
	class Inet_addr
	{
	public:
		Inet_addr()
		{
			ZeroMemory(__sockaddr,sizeof(__sockaddr));
			__sockaddr.sin_family=AF_INET;
		}
		Inet_addr(const char *str_ip,unsigned short port)
		{
			ZeroMemory(__sockaddr,sizeof(__sockaddr));
			__sockaddr.sin_family=AF_INET;
			set(str_ip,port);
		}
		Inet_addr &set(const char *str_ip,unsigned short port)
		{
			if(str_ip)
				__sockaddr.sin_addr.s_addr = inet_addr(str_ip);
			else
				__sockaddr.sin_addr.s_addr = INADDR_ANY;
			__sockaddr.sin_port=htons(port);
			return __sockaddr;
		}
	private:
		sockaddr_in __sockaddr;
	};
}