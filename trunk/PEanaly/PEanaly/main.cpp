#include <stdio.h>
#include <Windows.h>
#include <WinNT.h>

//PE��ʽ
// DOS MZ header
// DOS stub
// PE header
// Section table
// Section 1
// Section 2
// ��
// Section n


// DWORD WINAPI ImageFileType (LPVOID lpFile)
// {
// 	/* ���ȳ��ֵ���DOS�ļ���־ */
// 	if (*(USHORT *)lpFile == IMAGE_DOS_SIGNATURE)
// 	{
// 		/* ��DOSͷ������PE�ļ�ͷ����λ�� */
// 		if (LOWORD (*(DWORD *)NTSIGNATURE (lpFile)) ==
// 			IMAGE_OS2_SIGNATURE ||
// 			LOWORD (*(DWORD *)NTSIGNATURE (lpFile)) ==
// 			IMAGE_OS2_SIGNATURE_LE)
// 			return (DWORD)LOWORD(*(DWORD *)NTSIGNATURE (lpFile));
// 		else if (*(DWORD *)NTSIGNATURE (lpFile) ==
// 			IMAGE_NT_SIGNATURE)
// 			return IMAGE_NT_SIGNATURE;
// 		else
// 			return IMAGE_DOS_SIGNATURE;
// 	}
// 	else
// 		/* �����ļ����� */
// 		return 0;
// }

// #define  GETFILEMAPPINGHANDLE(filename)	HANDLE hFile=INVALID_HANDLE_VALUE;\
// 	HANDLE hFileMap=NULL;\
// 	__try\
// 	{\
// 		HANDLE hFile = CreateFile(filename, GENERIC_READ,\
// 			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);\
// 		DWORD dwFileSizeHigh=0;\
// 		DWORD qwFileSize = GetFileSize(hFile,&dwFileSizeHigh);\
// 		HANDLE hFileMap = CreateFileMapping(hFile, NULL,PAGE_READONLY, 0,0, NULL);\
// 		CloseHandle(hFile);\
// 		hFile=INVALID_HANDLE_VALUE;\
// 	}\
// 	__finally\
// 	{\
// 		if(hFile!=INVALID_HANDLE_VALUE)\
// 			CloseHandle(hFile);\
// 		if(hFileMap)\
// 			CloseHandle(hFileMap);\
// 	}\

int main()
{
// 	HANDLE hFileMap=GetFileMappingHandle("test.exe");
	HANDLE hFile=INVALID_HANDLE_VALUE;
	HANDLE hFileMap=NULL;
	LPBYTE lpbMapAddress=NULL;
	__try
	{
		hFile = CreateFile("test.exe", GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD dwFileSizeHigh=0;
		DWORD qwFileSize = GetFileSize(hFile,&dwFileSizeHigh);
		hFileMap = CreateFileMapping(hFile, NULL,PAGE_READONLY, 0,0, NULL);
		CloseHandle(hFile);
		hFile=INVALID_HANDLE_VALUE;
		lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap,FILE_MAP_READ,0, 
			0,qwFileSize);
	}
	__finally
	{
		if(hFile!=INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
		if(hFileMap)
			CloseHandle(hFileMap);
	}
 	
	
	_IMAGE_DOS_HEADER *DOS_head;
	_IMAGE_NT_HEADERS *NT_head;
	IMAGE_FILE_HEADER *File_head;

	
	IMAGE_OPTIONAL_HEADER32 *Optional_head;
	IMAGE_DATA_DIRECTORY *DataDirectory_head;

	DOS_head=(_IMAGE_DOS_HEADER *)lpbMapAddress;//dosͷ,�ӵ�0�ֽڿ�ʼ
	NT_head = (_IMAGE_NT_HEADERS *)(lpbMapAddress+DOS_head->e_lfanew);//DOS_head->e_lfanewָ����,NT_HEADͷ�Ŀ�ʼ��ַ
	File_head = &NT_head->FileHeader;//
	Optional_head=&NT_head->OptionalHeader;

	DataDirectory_head=&Optional_head->DataDirectory[0];	
// 		0  Export symbols  
// 		1  Import symbols  
// 		2  Resources  
// 		3  Exception  
// 		4  Security  
// 		5  Base relocation  
// 		6  Debug  
// 		7  Copyright string  
// 		8  Unknown  
// 		9  Thread local storage (TLS)  
// 		10  Load configuration  
// 		11  Bound Import  
// 		12  Import Address Table  
// 		13  Delay Import  
// 		14  COM descriptor  


	IMAGE_DATA_DIRECTORY *ImportSysmbols_Directory=(IMAGE_DATA_DIRECTORY *)(DataDirectory_head+1);//�ṹָ��λ��1;


	IMAGE_IMPORT_DESCRIPTOR *Import_table;//�����
	Import_table=(IMAGE_IMPORT_DESCRIPTOR *)(ImportSysmbols_Directory->VirtualAddress+lpbMapAddress);//��λ����������ڵ�ַ;IMAGE_DATA_DIRECTORY�ṹ�����������ÿ��dllһ����

	//IMAGE_THUNK_DATA,�ⶫ��ָ�����ÿ��DLL�к��������������ַ(IMAGE_IMPORT_BY_NAME)
	
	//������е����dll��
	while(Import_table->Characteristics!=0)//�������һ���ṹ��0��䣬�����жϸ�ֵΪ0ʱ����������
	{
		char *dllname=(char *)(Import_table->Name+lpbMapAddress);
		printf(dllname);
		printf("\n");

		IMAGE_THUNK_DATA *Import_Function_RVA=(IMAGE_THUNK_DATA *)(Import_table->FirstThunk+lpbMapAddress);
		while (Import_Function_RVA->u1.AddressOfData!=0)
		{
			if(Import_Function_RVA->u1.AddressOfData & IMAGE_ORDINAL_FLAG32)//��������ŵ���..
			{
				printf("--0x%08xh\n",Import_Function_RVA->u1.AddressOfData & ~IMAGE_ORDINAL_FLAG32);
			}
			else	//�Ժ���������
			{
				IMAGE_IMPORT_BY_NAME *Import_function_name=(IMAGE_IMPORT_BY_NAME *)(Import_Function_RVA->u1.AddressOfData+lpbMapAddress);
				printf("--%s\n",Import_function_name->Name);
			}
			Import_Function_RVA++;
		}
		//�Ƶ���һ��Import_table
		Import_table++;
	}

//	Import_table->OriginalFirstThunk ���Ǹ���ַ��ָ��IMAGE_THUNK_DATA,IMAGE_THUNK_DATA��IMAGE_IMPORT_BY_NAME������.

}