#pragma once
#include "afxwin.h"

#include "Utility\Label\Label.h"
#include "Utility\ButtonEx\ButtonCT.h"
#include "CcdMaskViewDlg.h"

// CCcdMarkViewDlg ��ȭ �����Դϴ�.

class CCcdMarkViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCcdMarkViewDlg)

public:
	CCcdMarkViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCcdMarkViewDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CCD_MARK_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void DisplayMarkView(int nUnit, int nMarkNo, double dZoomMarkWidth, double dZoomMarkHeight);
private:
	void InitCtrl();

	void InitMarkViewDlg();
	void ShowMarkNo();
	bool RegisterMark(double dSizeX, double dSizeY, double dCenterX, double dCenterY);

private:
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;

	int m_nUnit;
	int m_nMarkNo;

	CCcdMaskViewDlg m_clMaskViewDlg;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCcdMarkViewPrevious();
	afx_msg void OnBnClickedButtonCcdMarkViewNext();
	afx_msg void OnBnClickedButtonCcdMarkViewInit();
	afx_msg void OnBnClickedButtonCcdMarkViewRegist();
	afx_msg void OnBnClickedButtonCcdMarkViewDelete();
	afx_msg void OnBnClickedButtonCcdMarkViewRoiSave();
	afx_msg void OnBnClickedButtonCcdMarkViewMask();
	afx_msg void OnBnClickedButtonCcdMarkViewFind();

public:
	CLabel m_clColorStaticTitle;
    CLabel m_clColorStaticMarkNo;
	CButtonCT m_clColorButtonPrev;
	CButtonCT m_clColorButtonNext;
	CButtonCT m_clColorButtonInit;
	CButtonCT m_clColorButtonRegist;
	CButtonCT m_clColorButtonDelete;
	CButtonCT m_clColorButtonRoiSave;
	CButtonCT m_clColorButtonMarkView;
	CButtonCT m_clColorButtonMarkFind;
	CButtonCT m_clColorButtonImageSave;

	afx_msg void OnBnClickedButtonCcdBmpSave();
};
