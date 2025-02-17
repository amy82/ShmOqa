// MessageModalDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "MessageModalDlg.h"
#include "afxdialogex.h"


// CMessageModalDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMessageModalDlg, CDialogEx)

CMessageModalDlg::CMessageModalDlg(CString sTitle, CString sMsg, COLORREF bgColor, CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageModalDlg::IDD, pParent)
{
	m_sTitle = sTitle;
	m_sMsg = sMsg;
	m_bgColor = bgColor;
}

CMessageModalDlg::~CMessageModalDlg()
{
}

void CMessageModalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_MODAL_TITLE, m_clColorStaticTitle);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_MODAL_MSG, m_clColorStaticMsg);	
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_MODAL_YES, m_clColorButtonYes);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_MODAL_NO, m_clColorButtonNo);
}


BEGIN_MESSAGE_MAP(CMessageModalDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_MODAL_YES, &CMessageModalDlg::OnBnClickedButtonMessageModalYes)
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_MODAL_NO, &CMessageModalDlg::OnBnClickedButtonMessageModalNo)
END_MESSAGE_MAP()


// CMessageModalDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CMessageModalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);

    dc.FillSolidRect(rect, MESSAGE_BG_COLOR);// m_bgColor);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CMessageModalDlg::OnInitDialog()
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
void CMessageModalDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTitle.SetBkColor(m_bgColor);
	m_clColorStaticTitle.SetTextColor(MESSAGE_TXT_COLOR);
	m_clColorStaticTitle.SetFont(&m_clFontBig);
	m_clColorStaticTitle.SetWindowText(m_sTitle);
    m_clColorStaticTitle.SetBorder(TRUE);
    m_clColorStaticTitle.SetFontSize(15);
    m_clColorStaticTitle.SetBkColor(MESSAGE_BG_COLOR);

	m_clColorStaticMsg.SetBkColor(m_bgColor);
	m_clColorStaticMsg.SetTextColor(MESSAGE_TXT_COLOR);
	m_clColorStaticMsg.SetFont(&m_clFontMid);
    m_clColorStaticMsg.SetBorder(TRUE);
    m_clColorStaticMsg.SetFontSize(23);
    m_clColorStaticMsg.SetWindowText(m_sMsg);
    m_clColorStaticMsg.SetBkColor(MESSAGE_BG_COLOR);
    

    m_clColorButtonYes.state = 100;
    m_clColorButtonNo.state = 100;
	//m_clColorButtonYes.ChangeColor(m_bgColor);
	//m_clColorButtonNo.ChangeColor(m_bgColor);
}

//-----------------------------------------------------------------------------
//
//	��ư : YES
//
//-----------------------------------------------------------------------------
void CMessageModalDlg::OnBnClickedButtonMessageModalYes()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDYES);
}

//-----------------------------------------------------------------------------
//
//	��ư : NO
//
//-----------------------------------------------------------------------------
void CMessageModalDlg::OnBnClickedButtonMessageModalNo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDNO);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CMessageModalDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CMessageModalDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


