// regexDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CregexDlgDlg �Ի���
class CregexDlgDlg : public CDialog
{
// ����
public:
	CregexDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REGEXDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_EditsrcStr;
	CEdit m_EditExpression;
};
