// regexDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "regexDlg.h"
#include "regexDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CregexDlgDlg �Ի���




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


// CregexDlgDlg ��Ϣ�������

BOOL CregexDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CregexDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
			MessageBox("ƥ��!");
		}
		else
		{
			MessageBox("��ƥ��!");
		}
	}
	catch (const boost::regex_error &e)
	{
		MessageBox("������ʽ����!");
	}
}
