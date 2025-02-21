// UpdateHistoryDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "UpdateHistoryDlg.h"
#include "afxdialogex.h"

#define MAX_ROW_COUNT			30

// CUpdateHistoryDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUpdateHistoryDlg, CDialogEx)

CUpdateHistoryDlg::CUpdateHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateHistoryDlg::IDD, pParent)
{

}

CUpdateHistoryDlg::~CUpdateHistoryDlg()
{
}

void CUpdateHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_UPDATE_HISTORY, m_clColorStaticUpdateHistory);
	DDX_Control(pDX, IDC_BUTTON_UPDATE_HISTORY_CLOSE, m_clColorButtonClose);
}


BEGIN_MESSAGE_MAP(CUpdateHistoryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_HISTORY_CLOSE, &CUpdateHistoryDlg::OnBnClickedButtonUpdateHistoryClose)
END_MESSAGE_MAP()


// CUpdateHistoryDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CUpdateHistoryDlg::OnPaint()
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
BOOL CUpdateHistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� ��Ʈ�� �ʱ�ȭ
	this->InitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CUpdateHistoryDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticUpdateHistory.SetBkColor(RGB_CTRL_BG);
	m_clColorStaticUpdateHistory.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticUpdateHistory.SetFont(&m_clFontBig);


}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CUpdateHistoryDlg::InitGridCtrl()
{
	CRect rect;
	int i, j;

	GetDlgItem(IDC_STATIC_UPDATE_HISTORY_GRID_CONTENTS)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridCtrlUpdateContents.Create(rect, this, IDC_STATIC_UPDATE_HISTORY_GRID_CONTENTS, WS_TABSTOP | WS_VISIBLE);

	m_clGridCtrlUpdateContents.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridCtrlUpdateContents.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridCtrlUpdateContents.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridCtrlUpdateContents.SetReference_Setting();
	m_clGridCtrlUpdateContents.EnableSelection(FALSE);
	m_clGridCtrlUpdateContents.SetRowCount(MAX_ROW_COUNT + 1);
	m_clGridCtrlUpdateContents.SetColumnCount(4);
	m_clGridCtrlUpdateContents.SetFixedRowCount(1);
	m_clGridCtrlUpdateContents.SetFixedColumnCount(1);

	m_clGridCtrlUpdateContents.SetItemText(0, 0, _T("NO"));
	m_clGridCtrlUpdateContents.SetItemText(0, 1, _T("VERSION"));
	m_clGridCtrlUpdateContents.SetItemText(0, 2, _T("DATE"));
	m_clGridCtrlUpdateContents.SetItemText(0, 3, _T("UPDATE CONTENTS"));

	for (i = 0; i < MAX_ROW_COUNT + 1; i++)
	{
		m_clGridCtrlUpdateContents.SetRowHeight(i, 27);
		m_clGridCtrlUpdateContents.SetItemText(i + 1, 0, i + 1);

		for (j = 0; j < 4; j++)
		{
			if (i == 0)
			{
				switch(j)
				{
				case 0:	m_clGridCtrlUpdateContents.SetColumnWidth(j, 60);	break;
				case 1: m_clGridCtrlUpdateContents.SetColumnWidth(j, 120);	break;
				case 2:	m_clGridCtrlUpdateContents.SetColumnWidth(j, 120);	break;
				case 3: m_clGridCtrlUpdateContents.SetColumnWidth(j, 630);	break;
				}

				m_clGridCtrlUpdateContents.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				//m_clGridSfrSpec.SetItemText(i, j, pszCol[j]);
			}
			else
			{
				if(j == 3)
					m_clGridCtrlUpdateContents.SetItemFormat(i, j, DT_VCENTER);
				else
					m_clGridCtrlUpdateContents.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
	
	m_clGridCtrlUpdateContents.SetItemText(1, 1, _T("VER.1.01"));
	m_clGridCtrlUpdateContents.SetItemText(1, 2, _T("2018-10-01"));
	m_clGridCtrlUpdateContents.SetItemText(1, 3, _T("SENSOR ALIGN ���� �� PCB GRIP ����/���� �߰�"));
	
	m_clGridCtrlUpdateContents.SetItemText(2, 1, _T("VER.1.0"));
	m_clGridCtrlUpdateContents.SetItemText(2, 2, _T("2018-09-01"));
	m_clGridCtrlUpdateContents.SetItemText(2, 3, _T("���� ���� �� ����"));
	
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CUpdateHistoryDlg::OnBnClickedButtonUpdateHistoryClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CUpdateHistoryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.


	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
