#pragma once

#include "Utility\Label\Label.h"
#include "afxwin.h"

// CMessagePopupDlg ��ȭ �����Դϴ�.

class CMessagePopupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessagePopupDlg)

public:
	CMessagePopupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMessagePopupDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MESSAGE_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void SetContents(CString sTitle, CString sMsg, COLORREF bgColor, int nIndex);

private:
	void InitCtrl();

private:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	CString m_sTitle;
	CString m_sMsg;
	COLORREF m_bgColor;
	int m_nIndex;
		
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonMessagePopupConfirm();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	int CloseTime;
    CLabel m_clColorStaticTitle;
    CLabel m_clColorStaticMsg;
    CButtonCT m_clColorButtonConfirm;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
