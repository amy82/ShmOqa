#pragma once

#include "Utility\Label\Label.h"
#include "Utility\ButtonEx\ButtonCT.h"
#include "afxwin.h"

// CMessageAskDlg ��ȭ �����Դϴ�.

class CMessageAskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageAskDlg)

public:
	CMessageAskDlg(CString sTitle, CString sMsg, COLORREF bgColor, int nAskCount, TCHAR* pszAsk[], CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMessageAskDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MESSAGE_ASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	void InitCtrl();

private:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	CString m_sTitle;
	CString m_sMsg;
	COLORREF m_bgColor;
	CString m_sAnswer[4];
	int m_nAskCount;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonMessageAskAnswer1();
	afx_msg void OnBnClickedButtonMessageAskAnswer2();
	afx_msg void OnBnClickedButtonMessageAskAnswer3();
	afx_msg void OnBnClickedButtonMessageAskAnswer4();

public:
    CLabel m_clColorStaticTitle;
    CLabel m_clColorStaticMsg;
    CButtonCT m_clColorButtonAnswer[4];
};
