#pragma once


// CMessageInput ��ȭ �����Դϴ�.

class CMessageInput : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageInput)

public:
	CMessageInput(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMessageInput();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESSAGE_TYPE1 };
#endif

private:
	int m_nUnit;
	
public:
	int nPopupId;
	CFont m_clFontBig;
	CFont m_clFontMid;
	CFont m_clFontSmall;


	CEdit m_edtMsgText;
	CLabel m_clColorStaticTitle;
	CButtonCT m_clColorButtonConfirm;
	bool editInputReadOnly;

	CBrush m_brush;  // Ŭ���� ��� ����
	COLORREF m_bgColor;

	void InitCtrl();
	void setEditInput(CString title,int index,  CString txt);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMsgSend();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
