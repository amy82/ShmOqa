// SetDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInspDlg.h"
#include "SetDlg.h"
#include "afxdialogex.h"


// CSetDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSetDlg, CDialogEx)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INFO, pParent)
{

}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_LOAD, m_Btn_Info_Load);
    DDX_Control(pDX, IDC_BUTTON_SAVE, m_Btn_Info_Save);
    DDX_Control(pDX, IDC_BUTTON_OPER, m_Btn_Oper);
    DDX_Control(pDX, IDC_BUTTON_ENG, m_Btn_Eng);
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CSetDlg �޽��� ó�����Դϴ�.


void CSetDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
                       // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
    CRect rect;
    GetClientRect(rect);
    dc.FillSolidRect(rect, MAIN_BG_COLOR);
}


BOOL CSetDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

    return TRUE;  // return TRUE unless you set the focus to a control
                  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
