#pragma once

#include "uxmodule\uxbutton\ButtonCT.h"
// CSetDlg ��ȭ �����Դϴ�.

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INFO};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();

public:
    CButtonCT m_Btn_Info_Load;
    CButtonCT m_Btn_Info_Save;
    CButtonCT m_Btn_Oper;
    CButtonCT m_Btn_Eng;
};
