// regexDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "regexDlg.h"
#include "regexDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CregexDlgDlg 对话框




CregexDlgDlg::CregexDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CregexDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CregexDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_EditsrcStr);
	DDX_Control(pDX, IDC_EDIT1, m_EditExpression);
}

BEGIN_MESSAGE_MAP(CregexDlgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CregexDlgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CregexDlgDlg 消息处理程序

BOOL CregexDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CregexDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CregexDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CregexDlgDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString srcStr;
	CString expressionStr;
	m_EditsrcStr.GetWindowText(srcStr);
	m_EditExpression.GetWindowText(expressionStr);
	try
	{
		boost::regex expression(expressionStr);
		std::string strSrc(srcStr);

		if(boost::regex_match(strSrc, expression))
		{
			MessageBox("匹配!");
		}
		else
		{
			MessageBox("不匹配!");
		}
	}
	catch (const boost::regex_error &e)
	{
		MessageBox("正则表达式错误!");
	}
}
