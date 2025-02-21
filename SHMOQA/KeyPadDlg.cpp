// KeyPadDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "KeyPadDlg.h"
#include "afxdialogex.h"


// CKeyPadDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CKeyPadDlg, CDialogEx)

CKeyPadDlg::CKeyPadDlg(CString sVal, int nMaxCount, CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyPadDlg::IDD, pParent)
{
	m_sVal = sVal;
	m_nMaxCount = nMaxCount;
	m_bInit = false;// true; //true�� �Է½� �ʱ�ȭ
}

CKeyPadDlg::~CKeyPadDlg()
{
}

void CKeyPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_KEYPAD_CALC_VAL, m_clColorStaticCalcVal);
	DDX_Control(pDX, IDC_STATIC_KEYPAD_VAL, m_clColorStaticVal);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM0, m_clColorButtonNum[0]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM1, m_clColorButtonNum[1]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM2, m_clColorButtonNum[2]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM3, m_clColorButtonNum[3]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM4, m_clColorButtonNum[4]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM5, m_clColorButtonNum[5]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM6, m_clColorButtonNum[6]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM7, m_clColorButtonNum[7]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM8, m_clColorButtonNum[8]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_NUM9, m_clColorButtonNum[9]);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_DOT, m_clColorButtonDot);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_SIGN, m_clColorButtonSign);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_PLUS, m_clColorButtonPlus);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_MINUS, m_clColorButtonMinus);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_EQUAL, m_clColorButtonEqual);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_DEL, m_clColorButtonDel);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_CLEAR, m_clColorButtonClear);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_CANCEL, m_clColorButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_KEYPAD_ENTER, m_clColorButtonEnter);
}


BEGIN_MESSAGE_MAP(CKeyPadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_PLUS, &CKeyPadDlg::OnBnClickedButtonKeypadPlus)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_MINUS, &CKeyPadDlg::OnBnClickedButtonKeypadMinus)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_EQUAL, &CKeyPadDlg::OnBnClickedButtonKeypadEqual)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_DEL, &CKeyPadDlg::OnBnClickedButtonKeypadDel)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_DOT, &CKeyPadDlg::OnBnClickedButtonKeypadDot)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_SIGN, &CKeyPadDlg::OnBnClickedButtonKeypadSign)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_CLEAR, &CKeyPadDlg::OnBnClickedButtonKeypadClear)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_CANCEL, &CKeyPadDlg::OnBnClickedButtonKeypadCancel)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_ENTER, &CKeyPadDlg::OnBnClickedButtonKeypadEnter)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_KEYPAD_NUM0, IDC_BUTTON_KEYPAD_NUM9, &CKeyPadDlg::OnBnClickedButtonKeypadNum)
END_MESSAGE_MAP()


// CKeyPadDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB_DLG_BG);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CKeyPadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::InitCtrl()
{
	int i;

	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	//m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	//m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);	
	
	m_clColorStaticCalcVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCalcVal.SetFont(&m_clFontSmall);

	m_clColorStaticVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticVal.SetFont(&m_clFontBig);
	m_clColorStaticVal.SetWindowText(m_sVal);

}

//-----------------------------------------------------------------------------
//
//	��ư : +
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadPlus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sVal.GetLength() > 0)
	{
		m_sCalcVal = _T("+ ");
		m_sCalcVal += m_sVal;

		m_sVal = _T("");

		m_clColorStaticVal.SetWindowText(m_sVal);
		m_clColorStaticCalcVal.SetWindowText(m_sCalcVal);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : -
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadMinus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sVal.GetLength() > 0)
	{
		m_sCalcVal = _T("- ");
		m_sCalcVal += m_sVal;

		m_sVal = _T("");

		m_clColorStaticVal.SetWindowText(m_sVal);
		m_clColorStaticCalcVal.SetWindowText(m_sCalcVal);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : =
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadEqual()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double dCalcValue = 0.0;

	if (m_sCalcVal.GetLength() > 0 && m_sVal.GetLength() > 0)
	{
		if (m_sCalcVal.GetAt(0) == _T('-'))
		{
			m_sCalcVal.SetAt(0, _T(' '));

			dCalcValue = _ttof((TCHAR*)(LPCTSTR)m_sCalcVal.Trim()) - _ttof((TCHAR*)(LPCTSTR)m_sVal);
		}
		else
		{
			m_sCalcVal.SetAt(0, _T(' '));
			
			dCalcValue = _ttof((TCHAR*)(LPCTSTR)m_sCalcVal.Trim()) + _ttof((TCHAR*)(LPCTSTR)m_sVal);
		}

		m_sCalcVal = _T("");
		m_sVal.Format(_T("%.3lf"), dCalcValue);
		m_clColorStaticCalcVal.SetWindowText(m_sCalcVal);
		m_clColorStaticVal.SetWindowText(m_sVal);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : DEL
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sVal.GetLength() > 0)
	{
		m_sVal = m_sVal.Left(m_sVal.GetLength() - 1);

		m_clColorStaticVal.SetWindowText(m_sVal);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : .
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadDot()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sVal.Find(_T('.'), 0) < 0)
		m_sVal += _T('.');

	m_clColorStaticVal.SetWindowText(m_sVal);
}

//-----------------------------------------------------------------------------
//
//	��ư : +/-
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadSign()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double dValue = 0.0;

	if (m_sVal.GetLength() > 0)
	{
		dValue = _ttof((TCHAR*)(LPCTSTR)m_sVal) * -1;

		m_sVal.Format(_T("%.6lf"), dValue);
		m_clColorStaticVal.SetWindowText(m_sVal);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : CLEAR
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_sVal = _T("");
	m_sCalcVal = _T("");

	m_clColorStaticCalcVal.SetWindowText(m_sCalcVal);
	m_clColorStaticVal.SetWindowText(m_sVal);
}

//-----------------------------------------------------------------------------
//
//	��ư : CANCEL
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDNO);
}

//-----------------------------------------------------------------------------
//
//	��ư : ENTER
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadEnter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDOK);
}

//-----------------------------------------------------------------------------
//
//	��ư : 0 ~ 9
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnBnClickedButtonKeypadNum(UINT nID)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nIndex;

	if (m_sVal.GetLength() > m_nMaxCount)
		return;

	nIndex = nID - IDC_BUTTON_KEYPAD_NUM0;

	if (m_bInit == true)
	{
		m_sVal = _T("");
		m_bInit = false;
	}


	if (nIndex == 0)
	{
		if (m_sVal.GetLength() > 0)
		{			
			if (m_sVal.GetAt(0) == _T('0'))
			{
				if (m_sVal.Find(_T('.'), 0) < 0)
					return;
			}				
		}
	}

	switch (nIndex)
	{
	case 0:		m_sVal += _T('0');		break;
	case 1:		m_sVal += _T('1');		break;
	case 2:		m_sVal += _T('2');		break;
	case 3:		m_sVal += _T('3');		break;
	case 4:		m_sVal += _T('4');		break;
	case 5:		m_sVal += _T('5');		break;
	case 6:		m_sVal += _T('6');		break;
	case 7:		m_sVal += _T('7');		break;
	case 8:		m_sVal += _T('8');		break;
	case 9:		m_sVal += _T('9');		break;
	}

	m_clColorStaticVal.SetWindowText(m_sVal);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CKeyPadDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CKeyPadDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		//if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			//return TRUE;

		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			this->OnBnClickedButtonKeypadCancel();
			break;
		case VK_RETURN:
			this->OnBnClickedButtonKeypadEnter();
			break;
		case 0x08:
			this->OnBnClickedButtonKeypadDel();
			break;
		case 0x30:
		case 0x60:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM0);
			break;
		case 0x31:
		case 0x61:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM1);
			break;
		case 0x32:
		case 0x62:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM2);
			break;
		case 0x33:
		case 0x63:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM3);
			break;
		case 0x34:
		case 0x64:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM4);
			break;
		case 0x35:
		case 0x65:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM5);
			break;
		case 0x36:
		case 0x66:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM6);
			break;
		case 0x37:
		case 0x67:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM7);
			break;
		case 0x38:
		case 0x68:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM8);
			break;
		case 0x39:
		case 0x69:
			this->OnBnClickedButtonKeypadNum(IDC_BUTTON_KEYPAD_NUM9);
			break;
		case 0x6B:
			this->OnBnClickedButtonKeypadPlus();
			break;
		case 0x6D:
			this->OnBnClickedButtonKeypadMinus();
			break;
		case 0x6E:
		case 0xBE:
			this->OnBnClickedButtonKeypadDot();
			break;
		case 0xBB:
			this->OnBnClickedButtonKeypadEqual();
			break;
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

