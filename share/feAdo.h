#import "C:\\Program Files\\Common Files\\system\\ado\\msado15.dll"  no_namespace rename("EOF","adoEOF")

#include <string>
class CFeAdoConnection
{
public:
	CFeAdoConnection();
	~CFeAdoConnection();
	_com_error &GetLastError(){return m_comerror;}//返回最后一次的错误
	//封装两个常用的open 和 Execute
	bool Open(const char *connectstr,long options=adConnectUnspecified); //连接字符串,连接选项
	bool Execute(const char *sql,_RecordsetPtr &rs,long commandtype=adCmdText);//返回rs的Execute
	bool Execute(const char *sql,_variant_t *ra=NULL,long commandtype=adCmdText);//返回影响行数的Execute
	_Connection* GetPtr() const{return m_connptr.GetInterfacePtr();}//直接获取_Connection指针,不要在外部release
protected:
private:
	
private:
	std::string m_strConnectstring;
	_com_error m_comerror;
	_ConnectionPtr m_connptr;
};

CFeAdoConnection::CFeAdoConnection():m_comerror(0)
{
	m_connptr.CreateInstance(__uuidof(Connection));
}

CFeAdoConnection::~CFeAdoConnection()
{
	m_comerror=NULL;
}

bool CFeAdoConnection::Open(const char *connectstr,long options) 
{
	bool ret=false;
	try
	{m_connptr->Open(connectstr,"","",options);ret=true;}
	catch (_com_error &e)
	{m_comerror=e;}
	return ret;
}

bool CFeAdoConnection::Execute(const char *sql,_RecordsetPtr &prs,long commandtype)
{
	bool ret = false;
	try
	{
		prs=NULL;
		prs.CreateInstance(__uuidof(Recordset));
		prs->PutCursorLocation(adUseClient);
		prs->Open(sql,(IUnknown*)m_connptr,adOpenKeyset,adLockOptimistic,adCmdText);
		//rs = m_connptr->Execute(sql,0,commandtype);
		ret=true;
	}
	catch (_com_error &e)
	{m_comerror=e;}
	return ret;
}

bool CFeAdoConnection::Execute(const char *sql,_variant_t *ra,long commandtype)
{
	bool ret=false;
	try
	{
		m_connptr->Execute(sql,ra,commandtype);
		ret=true;
	}
	catch (_com_error &e)
	{m_comerror=e;}
	return ret;
}