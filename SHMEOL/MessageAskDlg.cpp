// MessageAskDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "MessageAskDlg.h"
#include "afxdialogex.h"


// CMessageAskDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMessageAskDlg, CDialogEx)

CMessageAskDlg::CMessageAskDlg(CString sTitle, CString sMsg, COLORREF bgColor, int nAskCount, TCHAR* pszAsk[], CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageAskDlg::IDD, pParent)
{
	int nIndex;
	int i;

	m_sTitle = sTitle;
	m_sMsg = sMsg;
	m_bgColor = bgColor;
	m_nAskCount = nAskCount;
	if(m_nAskCount > 4)
		m_nAskCount = 4;

	nIndex = 4 - m_nAskCount;

	for(i = 0; i < 4; i++)
	{
		if(i >= nIndex)
			m_sAnswer[i].Format(_T("%s"), pszAsk[i - nIndex]);
	}
}

CMessageAskDlg::~CMessageAskDlg()
{
}

void CMessageAskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_ASK_TITLE, m_clColorStaticTitle);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_ASK_MSG, m_clColorStaticMsg);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_ASK_ANSWER1, m_clColorButtonAnswer[0]);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_ASK_ANSWER2, m_clColorButtonAnswer[1]);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_ASK_ANSWER3, m_clColorButtonAnswer[2]);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_ASK_ANSWER4, m_clColorButtonAnswer[3]);	
}


BEGIN_MESSAGE_MAP(CMessageAskDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_ASK_ANSWER1, &CMessageAskDlg::OnBnClickedButtonMessageAskAnswer1)
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_ASK_ANSWER2, &CMessageAskDlg::OnBnClickedButtonMessageAskAnswer2)
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_ASK_ANSWER3, &CMessageAskDlg::OnBnClickedButtonMessageAskAnswer3)
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_ASK_ANSWER4, &CMessageAskDlg::OnBnClickedButtonMessageAskAnswer4)
END_MESSAGE_MAP()


// CMessageAskDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	int nIndex;
	int i;

	GetClientRect(rect);

    dc.FillSolidRect(rect, MESSAGE_BG_COLOR);// m_bgColor);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CMessageAskDlg::OnInitDialog()
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
void CMessageAskDlg::InitCtrl()
{
	int nIndex;
	int i;

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
	m_clColorStaticTitle.Invalidate();
    //
	m_clColorStaticMsg.SetBkColor(m_bgColor);
	m_clColorStaticMsg.SetTextColor(MESSAGE_TXT_COLOR);
	m_clColorStaticMsg.SetFont(&m_clFontMid);
    m_clColorStaticMsg.SetBorder(TRUE);
    m_clColorStaticMsg.SetFontSize(20);
    m_clColorStaticMsg.SetWindowText(m_sMsg);
    m_clColorStaticMsg.SetBkColor(MESSAGE_BG_COLOR);
	m_clColorStaticMsg.Invalidate();
	nIndex = 4 - m_nAskCount;

	for (i = 0; i < 4; i++)
	{
        m_clColorButtonAnswer[i].state = 100;

		if (i >= nIndex)
		{
			m_clColorButtonAnswer[i].SetWindowText(m_sAnswer[i]);
			m_clColorButtonAnswer[i].ShowWindow(SW_SHOW);
		}
		else
		{
			m_clColorButtonAnswer[i].SetWindowText(m_sAnswer[i]);
			m_clColorButtonAnswer[i].ShowWindow(SW_HIDE);
		}		
	}
}

//-----------------------------------------------------------------------------
//
//	STATIC : ANSWER 1
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnBnClickedButtonMessageAskAnswer1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(1);
}

//-----------------------------------------------------------------------------
//
//	STATIC : ANSWER 2
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnBnClickedButtonMessageAskAnswer2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(2);
}

//-----------------------------------------------------------------------------
//
//	STATIC : ANSWER 3
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnBnClickedButtonMessageAskAnswer3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(3);
}

//-----------------------------------------------------------------------------
//
//	STATIC : ANSWER 4
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnBnClickedButtonMessageAskAnswer4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(4);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CMessageAskDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CMessageAskDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


