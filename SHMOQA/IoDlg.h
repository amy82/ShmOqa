#pragma once

#include "uxmodule\uxbutton\ButtonCT.h"
#include "uxmodule\GridCtrl\GridCtrl.h"
// CIoDlg ��ȭ �����Դϴ�.

class CIoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIoDlg)

public:
	CIoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIoDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IO};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();


    int oldInVal[4];

    bool m_bShowFlag;
    bool timerRunFlag;

    void	setInterface();
    void	initIoGrid();
    void	dispInputData();
    void	dispOutputData();

    CRect	m_rcGridDioIn;
    CRect	m_rcGridDioOut;
    //Grid
    CGridCtrl m_clGrid_IO_In;
    CGridCtrl m_clGrid_IO_Out;
    void	InitGridCtrl_In();
    void	InitGridCtrl_Out();

    int inRow;
    int inCol;
    int outRow;
    int outCol;
    int m_nCurrentCh;

    void IoInChChange();
    void IoOutChChange();

    CButtonCT	m_btn_Left;
    CButtonCT	m_btn_Right;
    virtual BOOL OnInitDialog();
};
