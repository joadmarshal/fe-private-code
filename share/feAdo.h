#import "C:\\Program Files\\Common Files\\system\\ado\\msado15.dll"  no_namespace rename("EOF","adoEOF")

#include <string>
class CFeAdoConnection
{
public:
	CFeAdoConnection();
	~CFeAdoConnection();
	_com_error &GetLastError(){return m_comerror;}//�������һ�εĴ���
	//��װ�������õ�open �� Execute
	bool Open(const char *connectstr,long options=adConnectUnspecified); //�����ַ���,����ѡ��
	bool Execute(const char *sql,_RecordsetPtr &rs,long commandtype=adCmdText);//����rs��Execute
	bool Execute(const char *sql,_variant_t *ra=NULL,long commandtype=adCmdText);//����Ӱ��������Execute
	_Connection* GetPtr() const{return m_connptr.GetInterfacePtr();}//ֱ�ӻ�ȡ_Connectionָ��,��Ҫ���ⲿrelease
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