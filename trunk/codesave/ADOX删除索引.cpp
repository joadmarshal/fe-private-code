#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#import   "c:\Program Files\Common Files\system\ado\msadox.dll" no_namespace rename("EOF", "EndOfFile")
BOOL deleteAccessIndex(const char *mdbpath,const char *tablename,const char *columnName);
int main()
{  
	::CoInitialize(NULL);
	
	deleteAccessIndex("abc.mdb","tbl_radiusclient","ipaddress");
	deleteAccessIndex("abc.mdb","tbl_departmentex","depname");

	::CoUninitialize();
}

BOOL deleteAccessIndex(const char *mdbpath,const char *tablename,const char *columnName)
{
	_CatalogPtr m_pCatalog = NULL;
	
	_bstr_t strcnn("Provider=Microsoft.JET.OLEDB.4.0;"
		"Data source = abc.mdb;");
    m_pCatalog.CreateInstance(__uuidof (Catalog));
	
	m_pCatalog->PutActiveConnection(strcnn);
	_TablePtr m_pTable = m_pCatalog->Tables->Item[_bstr_t(tablename)];
	for (long i=m_pTable->Indexes->Count-1;i>=0;i--)
	{
		_IndexPtr m_pIndex = m_pTable->Indexes->Item[i];
		for (long j=m_pIndex->Columns->Count-1;j>=0;j--)
		{
			printf("%s\n",(char *)m_pIndex->Columns->Item[j]->Name);
			if(strcmp((char *)m_pIndex->Columns->Item[j]->Name,columnName)==0)
			{
				m_pIndex = NULL;
				m_pTable->Indexes->Delete(i);
			}
		}
	}
	return TRUE;
}   

