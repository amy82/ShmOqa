#pragma once

#include "Utility\GridCtrl\GridCtrl.h"
#include "Utility\ButtonEx\ButtonCT.h"
#include "Utility\Label\Label.h"

// CEpoxySet ��ȭ �����Դϴ�.

class CEpoxySet : public CDialogEx
{
	DECLARE_DYNAMIC(CEpoxySet)

public:
	CEpoxySet(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEpoxySet();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EPOXY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	void InitEpoxyLineGridCtrl();
	void InitEpoxyLineLengthGridCtrl();
	void InitEpoxyCurveLengthGridCtrl();

	void InitEpoxyRectInspGridCtrl();			// �����˻� ���� ���� ����.
	void InitEpoxyCircleRectInspGridCtrl();		// �����˻� � ���� ����.
	void InitEpoxyInspSpecGridCtrl();			// �����˻� ����.

	void ShowEpoxySet();
	void GetEpoxyData();
private:
	CGridCtrl m_clGridEpoxyLine;
	CGridCtrl m_clGridEpoxyLineLength;
	CGridCtrl m_clGridEpoxyCurveLength;
	CGridCtrl m_clGridEpoxyInspRect;
	CGridCtrl m_clGridEpoxyCircleRect;
	CGridCtrl m_clGridEpoxyInspSpec;
public:
	void SetUnit(int nUnit) { m_nUnit = nUnit; }

private:
	int m_nUnit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonEpoxySave();
	afx_msg void OnStnDblclickStaticEpoxyLineGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnDblclickStaticEpoxyLineLengthGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnDblclickStaticEpoxyCurveLengthGrid(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnStnDblclickStaticEpoxyInspGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnDblclickStaticEpoxyInspCircleGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnDblclickStaticEpoxyInspSpecGrid(NMHDR* pNMHDR, LRESULT* pResult);
	
};
