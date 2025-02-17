#pragma once

#include "afxwin.h"
#include "Utility\GridCtrl\GridCtrl.h"
#include "Utility\ButtonEx\ButtonCT.h"
#include "Utility\Label\Label.h"
// CCcdMesSpecDlg ��ȭ �����Դϴ�.

class CCcdMesSpecDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCcdMesSpecDlg)

public:
	CCcdMesSpecDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCcdMesSpecDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CCD_MES_SPEC};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void SetUnit(int nUnit);
private:
	void InitCtrl();
    void InitMesGridCtrl1();	//EEPROM �׸�
    void ShowMesGridSpec();
	void GetMesSpec();

    void GetLcbSpec();
    void GetYmeanSpec();
private:
    CGridCtrl m_clGridMesSpec1;

	CFont m_clFontSmall;
	CFont m_clFontMid;
	CFont m_clFontBig;

	int m_nUnit;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnNMDblClickedMesGrid1(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedButtonCcdSfrSpecSave();
	afx_msg void OnBnClickedButtonCcdSfrSpecClose();
	
public:
	CLabel m_clColorStaticTopBar;
    CLabel m_clColorStaticCenterSfrLimit;
    CLabel m_clColorStaticCenterSfrLimitVal;
    CLabel m_clColorStaticCornerSfrLimit;
    CLabel m_clColorStaticCornerSfrLimitVal;
    CLabel m_clColorStaticCornerSfrDeviationLimit;
    CLabel m_clColorStaticCornerSfrDeviationLimitVal;
    CLabel m_clColorStaticOCXY;
    CLabel m_clColorStaticOCXYVal;
    CLabel m_clColorStaticOCTh;
    CLabel m_clColorStaticOCThVal;
    CButtonCT m_clColorButtonSave;
	CButtonCT m_clColorButtonLoad;
    CButtonCT m_clColorButtonClose;
	afx_msg void OnBnClickedButtonCcdMesSpecLoad();
};
