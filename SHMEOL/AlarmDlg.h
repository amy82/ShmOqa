#pragma once
#include "afxwin.h"
#include "Utility\Label\Label.h"
#include "Utility\HeaderCtrlEx\HeaderCtrlEx.h"
#include "Utility\ButtonEx\ButtonCT.h"
#define MAX_ALARM_COUNT				30
#define MAX_PAGE_COUNT				10

#define		MAX_ALARM_PAGE_CNT			20			// �˶� �����丮 â�� �ִ� �˶� ������ ��..
#define		MAX_ALARM_LIST_IN_PAGE		24			// �˶� ������ �� ��µǴ� �˶� ��..
// CAlarmDlg ��ȭ �����Դϴ�.

class CAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmDlg)

public:
	CAlarmDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAlarmDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	CString m_sAlarmLog;

public:
	void SetUnit(int nUnit);
	void AddAlarm(CString szAlarm);
	void AddAlarm_N(CString sAlarm);

	void ShowCurrentPage();
	void ShowAlarmList_N();
private:
	void InitCtrl();
	
	void ShowAlarmList(int nMode);
	void SaveAlarm(CString szAlarm);

private:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	int m_nUnit;

	CHeaderCtrlEx m_clHeaderCtrlEx;
	CString* m_pAlarmList[2];
	int m_nAlarmPage[2];
	int m_nAlarmIndex[2];
	int m_nCurrentPage[2];
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAlarmPrev();
	afx_msg void OnBnClickedButtonAlarmNext();
	afx_msg void OnBnClickedButtonAlarmClear();

public:
    CLabel m_clColorStaticTitle;
    CLabel m_clColorStaticPage;
	//CListCtrl m_clListCtrlAlarm;
	CButtonCT m_clColorButtonPrev;
    CButtonCT m_clColorButtonNext;
    CButtonCT m_clColorButtonClear;

	//grid
	CGridCtrl m_clGridAlarm;

	int AlarmRow;
	int AlarmCol;
	//

public:
	void	InitGridCtrl_Alarm();
	void	ShowGridCtrl_Alarm();

    
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
