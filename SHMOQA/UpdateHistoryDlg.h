#pragma once

#include "Utility\GridCtrl\GridCtrl.h"
#include "Utility\Label\Label.h"
#include "afxwin.h"

// 2018.08.31. Jpromote
// CUpdateHistoryDlg ��ȭ �����Դϴ�.

class CUpdateHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateHistoryDlg)

public:
	CUpdateHistoryDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUpdateHistoryDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_UPDATE_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	void InitCtrl();
	void InitGridCtrl();
	
private:
	CFont m_clFontSmall;
	CFont m_clFontMid;
	CFont m_clFontBig;

	CGridCtrl m_clGridCtrlUpdateContents;
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

public:
    CLabel m_clColorStaticUpdateHistory;
    CButtonCT m_clColorButtonClose;
	afx_msg void OnBnClickedButtonUpdateHistoryClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
