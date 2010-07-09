// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0572E823_124B_4008_B66E_B4DC3228BAF6__INCLUDED_)
#define AFX_STDAFX_H__0572E823_124B_4008_B66E_B4DC3228BAF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable: 4786)
#include <Windows.h>
#include "TLogger.h"
#include "lkUtility.h"
#include <string>
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0572E823_124B_4008_B66E_B4DC3228BAF6__INCLUDED_)
