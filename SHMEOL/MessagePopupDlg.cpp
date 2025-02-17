// MessagePopupDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "MessagePopupDlg.h"
#include "afxdialogex.h"

#define MSG_CLOSE_TIMER			1000
// CMessagePopupDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMessagePopupDlg, CDialogEx)

CMessagePopupDlg::CMessagePopupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessagePopupDlg::IDD, pParent)
{
	m_sTitle = _T("");
	m_sMsg = _T("");
	m_bgColor = RGB_COLOR_WHITE;
	m_nIndex = 0;
	CloseTime = 0;
}

CMessagePopupDlg::~CMessagePopupDlg()
{
}

void CMessagePopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_POPUP_TITLE, m_clColorStaticTitle);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_POPUP_MSG, m_clColorStaticMsg);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_POPUP_CONFIRM, m_clColorButtonConfirm);
}


BEGIN_MESSAGE_MAP(CMessagePopupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_POPUP_CONFIRM, &CMessagePopupDlg::OnBnClickedButtonMessagePopupConfirm)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CMessagePopupDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CMessagePopupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, m_bgColor);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CMessagePopupDlg::OnInitDialog()
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
void CMessagePopupDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTitle.SetTextColor(MESSAGE_TXT_COLOR);
	m_clColorStaticTitle.SetFont(&m_clFontBig);
    m_clColorStaticTitle.SetBorder(TRUE);
    m_clColorStaticTitle.SetFontSize(15);
    m_clColorStaticTitle.SetBkColor(MESSAGE_BG_COLOR);


    ///
	m_clColorStaticMsg.SetTextColor(MESSAGE_TXT_COLOR);
	m_clColorStaticMsg.SetFont(&m_clFontMid);
    m_clColorStaticMsg.SetBorder(TRUE);
    m_clColorStaticMsg.SetFontSize(25);
    m_clColorStaticMsg.SetBkColor(MESSAGE_BG_COLOR);



    m_clColorButtonConfirm.state = 100;
}

//-----------------------------------------------------------------------------
//
//	�˾� ���̾�α� ���� ǥ��
//
//-----------------------------------------------------------------------------
void CMessagePopupDlg::SetContents(CString sTitle, CString sMsg, COLORREF bgColor, int nIndex)
{
	CRect rect;
	int nSizeX;
	int nSizeY;

	GetClientRect(rect);

	nSizeX = GetSystemMetrics(SM_CXSCREEN);
	nSizeY = GetSystemMetrics(SM_CYSCREEN);

	m_sTitle = sTitle;
	m_sMsg = sMsg;
	m_bgColor = bgColor;
	m_nIndex = nIndex;
	
	m_clColorStaticTitle.SetBkColor(m_bgColor);
	m_clColorStaticTitle.SetWindowText(m_sTitle);

	m_clColorStaticMsg.SetBkColor(m_bgColor);
	m_clColorStaticMsg.SetWindowText(m_sMsg);

	//m_clColorButtonConfirm.ChangeColor(m_bgColor);
			
	rect.top = ((nSizeY - rect.bottom) / 2) + (m_nIndex * 25);
	rect.left = (nSizeX - rect.right) / 2 + (m_nIndex * 25);	

	this->SetWindowPos(NULL,rect.left, rect.top, rect.right, rect.bottom, SWP_NOSIZE);
}

//-----------------------------------------------------------------------------
//
//	��ư : CONFIRM
//
//-----------------------------------------------------------------------------
void CMessagePopupDlg::OnBnClickedButtonMessagePopupConfirm()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	g_clDioControl.SetBuzzer(false);
	EndDialog(IDOK);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CMessagePopupDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CMessagePopupDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMessagePopupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (nIDEvent == MSG_CLOSE_TIMER)
	{
		CloseTime = 0;
		OnBnClickedButtonMessagePopupConfirm();
		KillTimer(MSG_CLOSE_TIMER);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMessagePopupDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		if (CloseTime > 0)
		{
			SetTimer(MSG_CLOSE_TIMER, CloseTime * 1000, NULL);
		}
	}
	else
	{
		KillTimer(MSG_CLOSE_TIMER);
	}
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
