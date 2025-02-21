#pragma once
#include "afxwin.h"
#include "Utility\GridCtrl\GridCtrl.h"
#include "Utility\Label\Label.h"
// CDioDlg ��ȭ �����Դϴ�.

class CDioDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDioDlg)

public:
	CDioDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDioDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void SetUnit(int nUnit);

private:
	void InitCtrl();
    void InitInGridCtrl();
    void InitOutGridCtrl();

	void ShowDIOName();
	void ShowDIONum();
	void ShowDInState();
	void ShowDOutState();

private:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	int m_nUnit;

	CGridCtrl m_clGridCtrlDIn;
	CGridCtrl m_clGridCtrlDOut;

	DWORD m_dwPrevDIn;
	DWORD m_dwPrevDOut;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblClickedDOut(NMHDR* pNMHDR, LRESULT* pResult);

public:
    CLabel m_clColorStaticTitle;
};
