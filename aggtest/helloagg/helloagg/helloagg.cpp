#define _WIN32_WINNT 0x0501 
#include <windows.h> 
#include <agg_array.h> 
#include <agg_pixfmt_rgba.h> 
#include <agg_scanline_u.h> 
#include <agg_renderer_scanline.h> 
#include <../font_win32_tt/agg_font_win32_tt.h> 
#include <agg_font_cache_manager.h> 
#include <agg_span_solid.h> 
#include <agg_span_interpolator_linear.h> 
#include <agg_span_gradient.h> 
#include <agg_span_allocator.h> 
#include <agg_conv_transform.h> 
#include <agg_ellipse.h> 
#include <agg_trans_single_path.h> 
using namespace agg; 
typedef agg::font_engine_win32_tt_int16 fe_type; 
typedef agg::font_cache_manager<fe_type> fcman_type; 
typedef agg::renderer_base<agg::pixfmt_bgra32> renb_type; 

// 使用指定的顶点源和线段生成器输出文字 
template<class VS, class SpanGenerator> 
void AggDrawText(renb_type &renb, 
				 fcman_type &font_manager, 
				 VS &vs, SpanGenerator &span_gen, 
				 const wchar_t *txt) 
{ 
	

	span_allocator<rgba8> span_alloc; 
	rasterizer_scanline_aa<> ras; 
	scanline_u8 sl; 

	double x=0, y=0; 
	for(const wchar_t *p = txt; *p; p++) 
	{ 
		const glyph_cache* gc = font_manager.glyph(*p); 
		if(gc) 
		{ 
			font_manager.init_embedded_adaptors(gc, x, y); 

			ras.add_path(vs); 
			agg::render_scanlines_aa(ras, sl, renb, span_alloc, span_gen); 

			x += gc->advance_x; 
			y += gc->advance_y; 
		} 
	} 
} 

// 向renb的指定位置和半径输出http://www.cppprog.com ,有环绕效果 
void DrawUrl(HDC dc, renb_type &renb, 
			 double ox, double oy, double rx, double ry) 
{ 
	using namespace agg; 
	//字体引擎 
	fe_type font(dc); 
	fcman_type font_manager(font); 
	font.height(18.0); 
	font.flip_y(true); 
	font.hinting(true); 
	if(!font.create_font("Comic Sans MS",agg::glyph_ren_outline)) return; 
	//坐标转换管道 
	typedef conv_curve< 
		fcman_type::path_adaptor_type 
	> cc_pa_type; 
	cc_pa_type ccpath(font_manager.path_adaptor()); 

	typedef conv_transform<cc_pa_type, 
		trans_single_path> ct_cc_pa_type; 
	trans_single_path trans_path; 
	ct_cc_pa_type ctpath(ccpath, trans_path); 

	ellipse ell(0,0,rx,ry); 
	trans_affine ellmtx; 
	conv_transform<ellipse> ctell(ell, ellmtx); 
	ellmtx.rotate(agg::pi); 
	ellmtx.translate(ox,oy); 
	trans_path.add_path(ctell); 
	// 线段生成器 
	span_solid<rgba8> ss; 
	ss.color(rgba(1,0,0)); 

	AggDrawText(renb, font_manager, ctpath, ss, L"http://www.cppprog.com"); 
} 
// 向renb的指定位置输出“C++编程”几个字，有镜象效果 
void DrawName(HDC dc, renb_type &renb, double x, double y) 
{ 
	using namespace agg; 
	// 字体引擎 
	fe_type font(dc); 
	fcman_type font_manager(font); 

	font.height(30.0); 
	font.flip_y(true); 
	font.hinting(true); 
	if(!font.create_font("黑体",agg::glyph_ren_outline)) return; 
	// 坐标转换管道 
	typedef conv_curve< 
		fcman_type::path_adaptor_type 
	> cc_pa_type; 
	cc_pa_type ccpath(font_manager.path_adaptor()); 

	typedef conv_transform<cc_pa_type> ct_cc_pa_type; 
	trans_affine mtx; 
	ct_cc_pa_type ctpath( ccpath, mtx ); 

	mtx.translate(50,50); 
	//线段生成器 
	span_solid<rgba8> ss; 
	ss.color(rgba(0,0,0)); 

	AggDrawText(renb, font_manager, ctpath, ss, L"C++编程"); 
	// 改变坐标转换矩阵(镜像) 
	mtx.reset(); 
	mtx.flip_y(); 
	mtx.translate(50,60); 

	// 渐变线段生成器 
	typedef span_interpolator_linear<> interpolator_type; 
	trans_affine img_mtx; 
	interpolator_type ip(img_mtx); 

	typedef gradient_y gradientF_type; 
	gradientF_type grF; 

	typedef gradient_linear_color<rgba8> colorF_type; 
	colorF_type colorF(rgba(0,0,0), rgba(0,0,0,0)); 

	typedef span_gradient<rgba8, 
		interpolator_type, 
		gradientF_type, 
		colorF_type> span_gen_type; 
	span_gen_type span_gen(ip,grF,colorF,30,80); 

	AggDrawText(renb, font_manager, ctpath, span_gen, L"C++编程"); 
} 
// 调用DrawUrl和DrawName向renb输出文字 
void DrawIt(HDC dc, renb_type &renb) 
{ 
	// 以透明色填充 
	renb.clear(rgba(0,0,0,0)); 
	// 输出文字 
	DrawUrl(dc, renb, 100, 50, 80, 40); 
	DrawName(dc, renb, 50, 50); 
} 
// 使用AGG处理图片后与hwnd关联 
void SetLayoutWin(HWND hwnd) 
{ 
	// 起始位置和窗体大小 
	POINT ptWinPos = {500,200}; 
	SIZE sizeWindow = {200, 100}; 

	// 建立DIB 
	BITMAPINFO bmp_info; 
	::ZeroMemory(&bmp_info, sizeof(bmp_info)); 
	bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bmp_info.bmiHeader.biWidth = sizeWindow.cx; 
	bmp_info.bmiHeader.biHeight = sizeWindow.cy; 
	bmp_info.bmiHeader.biPlanes = 1; 
	bmp_info.bmiHeader.biBitCount = 32; 
	bmp_info.bmiHeader.biCompression = BI_RGB; 

	HDC hdcTemp = GetDC(0); 
	HDC mem_dc = ::CreateCompatibleDC(hdcTemp); 
	ReleaseDC(0, hdcTemp); 

	void* buf = NULL; 
	HBITMAP bmp = ::CreateDIBSection( 
		mem_dc, 
		&bmp_info, 
		DIB_RGB_COLORS, 
		&buf, 
		0, 0 
		); 

	// 把bmp与mem_dc关联，这样AGG就可以和原生GDI一起工作了 
	HBITMAP temp = (HBITMAP)::SelectObject(mem_dc, bmp); 
	{ 
		// AGG处理 
		agg::rendering_buffer rbuf( 
			(unsigned char*)buf, 
			sizeWindow.cx, sizeWindow.cy, 
			-sizeWindow.cx*4); 
		agg::pixfmt_bgra32 pixf(rbuf); 
		renb_type renb(pixf); 
		DrawIt(mem_dc,renb); 
	} 

	// 把画好的mem_dc与hwnd关联到一起 
	BLENDFUNCTION m_Blend={AC_SRC_OVER,0,255,AC_SRC_ALPHA}; 
	POINT ptSrc = {0, 0}; 
	BOOL bRet = UpdateLayeredWindow(hwnd, 0, &ptWinPos, 
		&sizeWindow, mem_dc, &ptSrc, 
		0, &m_Blend, ULW_ALPHA); 
	// 回收 
	::DeleteObject(bmp); 
	::DeleteDC(mem_dc); 
} 

// Windows消息处理 
LRESULT CALLBACK WndProc (HWND hwnd, UINT umsg, WPARAM wParam, 
						  LPARAM lParam) 
{ 
	switch (umsg) 
	{ 
	case WM_CLOSE: 
		DestroyWindow (hwnd); 
		return 0; 
	case WM_DESTROY: 
		PostQuitMessage (0); 
		return 0; 
	case WM_NCHITTEST: 
		return HTCAPTION; 
	} 
	return DefWindowProc (hwnd, umsg, wParam, lParam); 
} 

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 LPTSTR     lpCmdLine, 
					 int        nCmdShow) 
{ 
	WNDCLASS wc={ 
		0,WndProc, 
		0,0, 
		hInstance, 
		NULL,LoadCursor(NULL, IDC_ARROW), 
		(HBRUSH)(COLOR_WINDOW+1), 
		0,"AGGWIN" 
	}; 
	::RegisterClass(&wc); 

	HWND hWnd = ::CreateWindowEx(WS_EX_LAYERED,"AGGWIN", NULL, WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL); 

	if (!hWnd) return -1; 
	SetLayoutWin(hWnd); 
	::ShowWindow(hWnd, nCmdShow); 

	// 主消息循环: 
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0)) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 

	return (int) msg.wParam; 
} 
