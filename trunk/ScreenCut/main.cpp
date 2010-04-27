#include <windows.h>
#include <string>
#pragma   comment(lib,"msimg32.lib")
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HDC hdcMem=0;	  //原图
HDC hdcMemugly=0;//灰图
HDC hbluePen=0;	//画矩形用画笔
HBRUSH hgrayMapBrush=0;
int   nFullWidth=0;
int   nFullHeight=0;
POINT p1,p2;
RECT rect;		//矩形
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	nFullWidth=GetSystemMetrics(SM_CXSCREEN);       
    nFullHeight=GetSystemMetrics(SM_CYSCREEN); 

	HBRUSH hBrush = CreateSolidBrush (RGB(20,20,20));
	HDC hdcScreenDC=GetWindowDC(0);
	hdcMem = CreateCompatibleDC (hdcScreenDC) ;
	HBITMAP sCreenBMP=CreateCompatibleBitmap(hdcScreenDC,nFullWidth,nFullHeight);	
	SelectObject(hdcMem,sCreenBMP);	
	BitBlt(hdcMem,0,0,nFullWidth,nFullHeight,hdcScreenDC,0,0,SRCCOPY);
	
	//灰图生成
	hdcMemugly = CreateCompatibleDC (hdcScreenDC) ;
	HBITMAP sCreenBMPUgly=CreateCompatibleBitmap(hdcScreenDC,nFullWidth,nFullHeight);
	SelectObject(hdcMemugly,sCreenBMPUgly);
	SelectObject(hdcMemugly,hBrush);
	PatBlt (hdcMemugly, 0, 0, nFullWidth, nFullHeight, PATINVERT) ;
	
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER; 
	bf.BlendFlags = 0; 
	bf.AlphaFormat = 0; 
	bf.SourceConstantAlpha = 100; 
	
	AlphaBlend(hdcMemugly,0,0,nFullWidth,nFullHeight,
		hdcScreenDC,0,0,nFullWidth,nFullHeight,bf);
	
	LOGBRUSH logb;
	logb.lbStyle=BS_PATTERN;
	logb.lbHatch=(LONG)sCreenBMPUgly;
	
	hgrayMapBrush=CreateBrushIndirect(&logb);


    static TCHAR szAppName[] = TEXT ("11");
    HWND         hwnd;
    MSG          msg;
    WNDCLASSEX   wndclassex = {0};
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpfnWndProc   = WndProc;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 0;
    wndclassex.hInstance     = hInstance;
    wndclassex.hIcon         = LoadIcon (NULL, IDI_INFORMATION);
    wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclassex.hbrBackground = hgrayMapBrush/*(HBRUSH) GetStockObject (WHITE_BRUSH)*/;
    wndclassex.lpszMenuName  = NULL;
    wndclassex.lpszClassName = szAppName;
    wndclassex.hIconSm       = wndclassex.hIcon;
	
	
	   
    if (!RegisterClassEx (&wndclassex))
    {
        MessageBox (NULL, TEXT ("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
        return 0;
    }
	
	
	
    hwnd = CreateWindowEx (NULL, 
		szAppName, 
		TEXT ("WindowTitle"),
		WS_POPUP,
		0, 
		0, 
		nFullWidth, 
		nFullHeight, 
		NULL, 
		NULL, 
		hInstance,
        		       NULL); 
	
    ShowWindow (hwnd, iCmdShow);
    UpdateWindow (hwnd);
	
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
	DeleteObject(sCreenBMP);
	DeleteDC(hdcMem);
	ReleaseDC(0,hdcScreenDC);
	DeleteObject(hBrush);
	DeleteObject(hgrayMapBrush);
    return msg.wParam;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL isdrawrect=FALSE;
    HDC hdc;
	int mx=0;
	int my=0;
/*	HDC hdctemp;*/
    PAINTSTRUCT ps;
    switch (message)
    {
	case   WM_LBUTTONDOWN://鼠标消息
		if(!isdrawrect)
		{
			mx=LOWORD(lParam);
			my=HIWORD(lParam);
// 			char gg[6];
// 			sprintf(gg,"%d",mx);
// 			MessageBox(hwnd,gg,"",0);
			p1.x=mx;
			p1.y=my;
			p2.x=mx;
			p2.y=my;
		}
		return 0 ;
	case   WM_MOUSEMOVE:
		if(!isdrawrect&&(wParam & MK_LBUTTON ))
		{
			p2.x=LOWORD(lParam);
			p2.y=HIWORD(lParam);
			InvalidateRect (hwnd, NULL, TRUE) ;
		}
		return 0 ;
	case   WM_LBUTTONUP:
		isdrawrect=TRUE;
		return 0 ;

    case WM_CREATE:
        return 0;

    case WM_PAINT:
        hdc = BeginPaint (hwnd, &ps);
		//先上灰度图
// 		BitBlt (hdc, 0, 0, nFullWidth, nFullHeight,
// 			hdcMemugly, 0, 0, SRCCOPY);
		//再画矩形
		if(p1.x<=p2.x)
		{
			rect.left=p1.x;
			rect.right=p2.x;
		}
		else
		{
			rect.left=p2.x;
			rect.right=p1.x;
		}
		
		if(p1.y<=p2.y)
		{
			rect.top=p1.y;
			rect.bottom=p2.y;
		}
		else
		{
			rect.top=p2.y;
			rect.bottom=p1.y;
		}
		Rectangle(hdc,rect.left-2,rect.top-2,rect.right+2,rect.bottom+2);
		//矩形内再用原图同位置填充
		BitBlt(hdc,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,hdcMem
			,rect.left,rect.top,SRCCOPY);

		EndPaint (hwnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage (0);
        return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

