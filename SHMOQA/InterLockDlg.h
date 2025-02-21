#pragma once

#include "Utility\Label\Label.h"
#include "Utility\ButtonEx\ButtonCT.h"
// CMessageDlg ��ȭ �����Դϴ�.

class CInterLockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInterLockDlg)

public:
    CInterLockDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInterLockDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INTERLOCK};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	void setInterface();
	void setDlg(CString _title, CString  _msg, COLORREF bgColor, int index);

	CString m_sTitle;
	CString m_sMsg;
	COLORREF m_bgColor;
	int m_nIndex;

	CButtonCT m_btn_ok;
	CButtonCT m_btn_Cancel;
	CLabel	m_label_Title;
	CLabel	m_label_Msg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
