#pragma once

#include "Utility\Label\Label.h"
#include "Utility\ExButton\exbutton.h"

// CEEPROMTest ��ȭ �����Դϴ�.

class CEEPROMTest : public CDialogEx
{
	DECLARE_DYNAMIC(CEEPROMTest)

public:
	CEEPROMTest(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEEPROMTest();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EEPROM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	int m_nUnit;
public:
	void SetUnit(int nUnit);
	void SetInterface();
	int output_Mode; //0 = hex , 1 = float
public:
	CString	m_Str_SlaveAddr;
	CString	m_Str_Addr;
	CString	m_Str_Data_Hex;
	CString	m_Str_Data_Ch;
	//
	BYTE	m_Str_AddrLength;
	BYTE	m_Str_DataLength;

	CExButton	m_Button_Ascii;
	CExButton	m_Button_Float;
	CExButton	m_Button_Dec;
	CExButton   m_Button_Hex;
public:
	void FuncModeChange(int type);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnMiuOpen();
	afx_msg void OnBnClickedBtnMiuStop();
	afx_msg void OnBnClickedBtnMiuClose();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedBtnOutputAscii();
	afx_msg void OnBnClickedBtnOutputHex();
	afx_msg void OnBnClickedBtnOutputFloat();
	afx_msg void OnBnClickedBtnOutputDec();
	afx_msg void OnBnClickedOk();
};
