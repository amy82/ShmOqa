#pragma once
#include "afxwin.h"
#include "Utility\ButtonEx\ButtonCT.h"

// CModelSelectDlg ��ȭ �����Դϴ�.

class CModelSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModelSelectDlg)

public:
	CModelSelectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CModelSelectDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODEL_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void SetUnit(int nUnit);
private:
	CGridCtrl m_clGridModelSelect;
	int m_nUnit;

public:
	CButtonCT m_ColorbuttonModelModerate;
	CButtonCT m_ColorbuttonModelNarrow;

public:
	void ShowCurrentModel();

public:
	afx_msg void OnBnClickedButtonModelSelectExit();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonModerateModel();
	afx_msg void OnBnClickedButtonNarrowModel();
	afx_msg void OnBnClickedButtonModelSelectLoad();
};
