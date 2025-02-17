// ProductNG.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "ProductNGDlg.h"
#include "afxdialogex.h"

// CProductNG

IMPLEMENT_DYNAMIC(CProductNGDlg, CDialogEx)

CProductNGDlg::CProductNGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductNGDlg::IDD, pParent)
{
	m_nSelectList = 0;
	m_nUnit = UNIT_AA1;
}

CProductNGDlg::~CProductNGDlg()
{
}

void CProductNGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NG_TOP_BAR, m_clColorStaticTopBar);
	DDX_Control(pDX, IDC_STATIC_NG_NAME, m_clColorStaticNGName);
	DDX_Control(pDX, IDC_BUTTON_NG_RETRY, m_clColorButtonNGRetry);
	DDX_Control(pDX, IDC_BUTTON_NG_OK, m_clColorButtonNGOk);
	DDX_Control(pDX, IDC_BUTTON_NG_PCB, m_clColorButtonNGPcb);
	DDX_Control(pDX, IDC_BUTTON_NG_PRODUCT, m_clColorButtonNGProduct);
	DDX_Control(pDX, IDC_BUTTON_NG_CHECK, m_clColorButtonNGCheck);
	DDX_Control(pDX, IDC_BUTTON_NG_CANCEL, m_clColorButtonNGCancel);
	DDX_Control(pDX, IDC_LIST_NG_LIST, m_clListCtrlNG);
}

BEGIN_MESSAGE_MAP(CProductNGDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NG_CANCEL, &CProductNGDlg::OnBnClickedButtonNgCancel)
	ON_BN_CLICKED(IDC_BUTTON_NG_CHECK, &CProductNGDlg::OnBnClickedButtonNgCheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NG_LIST, &CProductNGDlg::OnLvnItemchangedListNgList)
END_MESSAGE_MAP()



// CProductNG �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CProductNGDlg::OnPaint()
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
BOOL CProductNGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CProductNGDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticTopBar.SetFont(&m_clFontMid);

	m_clColorStaticNGName.SetBkColor(RGB_DLG_BG);
	m_clColorStaticNGName.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticNGName.SetFont(&m_clFontMid);


	m_clListCtrlNG.ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
	m_clListCtrlNG.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_clListCtrlNG.InsertColumn(0, _T("��ȣ"), LVCFMT_CENTER, 50, -1);
	m_clListCtrlNG.InsertColumn(1, _T("NG ����"), LVCFMT_CENTER, 272, -1);
	m_clListCtrlNG.DeleteAllItems();
	m_clListCtrlNG.SetFont(&m_clFontSmall);

	TCHAR* szNGList[] = {_T("�ػ�� �ҷ�"), _T("�̹� �ҷ�")/*, _T("MES �ҷ�"), _T("EPOXY �ҷ�")*/};
	int i;
	CString cStr;
	//for (i = 0; i < 4; i++)
	for (i = 0; i < 2; i++)
	{
		cStr.Format(_T("%d"), i+1);
		m_clListCtrlNG.InsertItem(i, cStr);
		m_clListCtrlNG.SetItem(i, 1, LVIF_TEXT, szNGList[i], 0, 0, 0, NULL);
	}
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CProductNGDlg::SetUnit(int nUnit)
{
	TCHAR szTitle[SIZE_OF_100BYTE];

	if(m_nUnit != nUnit)
	{
		m_nUnit = nUnit;

		_stprintf_s(szTitle, SIZE_OF_100BYTE, _T("[INSP #%d] NG LIST"), m_nUnit + 1);
		GetDlgItem(IDC_STATIC_NG_TOP_BAR)->SetWindowText(szTitle);
	}

	m_nUnit = nUnit;
}

void CProductNGDlg::OnLvnItemchangedListNgList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString sMsg = _T("");

	m_nSelectList = pNMLV->iItem + 1;

	if (m_nSelectList == 1)
	{
		sMsg.Format(_T(" ���� : �ػ�� �ҷ�"));
		m_clColorStaticNGName.SetWindowText(sMsg);
	}
	else if (m_nSelectList == 2)
	{
		sMsg.Format(_T(" ���� : �̹� �ҷ�"));
		m_clColorStaticNGName.SetWindowText(sMsg);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : Ȯ��
//
//-----------------------------------------------------------------------------
void CProductNGDlg::OnBnClickedButtonNgCheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szLog[SIZE_OF_1K];

	if (m_nSelectList == 1)
	{
		g_clMesCommunication[m_nUnit].m_nMesStainResult = 1;
		g_clMesCommunication[m_nUnit].m_nMesDefectResult[1] = 1;
	}
	else if (m_nSelectList == 2)
	{
		g_clMesCommunication[m_nUnit].m_nMesStainResult = 0;
		g_clMesCommunication[m_nUnit].m_nMesDefectResult[1] = 0;
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[MANUAL] NG ������ ������ �ּ���."));
		AddLog(szLog, 1, m_nUnit, false);
		return;
	}

	g_clTaskWork[m_nUnit].m_nEmission = 2;
	g_clTaskWork[m_nUnit].SaveData();
	g_pCarAABonderDlg->ShowOkNgState(m_nUnit , g_clTaskWork[m_nUnit].m_nEmission);

	m_nSelectList = 0;
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	��ư : ���
//
//-----------------------------------------------------------------------------
void CProductNGDlg::OnBnClickedButtonNgCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CProductNGDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CProductNGDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
