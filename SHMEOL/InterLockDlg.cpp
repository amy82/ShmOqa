// MessageDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "InterLockDlg.h"
#include "afxdialogex.h"


// CInterLockDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInterLockDlg, CDialogEx)

CInterLockDlg::CInterLockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INTERLOCK, pParent)
{
}

CInterLockDlg::~CInterLockDlg()
{
}

void CInterLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_label_Title);
	DDX_Control(pDX, IDC_STATIC_MSG, m_label_Msg);
	DDX_Control(pDX, IDC_BTN_OK, m_btn_ok);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btn_Cancel);
}


BEGIN_MESSAGE_MAP(CInterLockDlg, CDialogEx)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDOK, &CInterLockDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDCANCEL, &CInterLockDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_OK, &CInterLockDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CInterLockDlg::OnBnClickedBtnCancel)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CInterLockDlg �޽��� ó�����Դϴ�.

void CInterLockDlg::setInterface()
{

	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(25, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(64, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_label_Title.SetTextColor(MESSAGE_TXT_COLOR);
	m_label_Title.SetFont(&m_clFontMid);
	m_label_Title.SetBorder(TRUE);
	m_label_Title.SetFontSize(35);
	m_label_Title.SetBkColor(MESSAGE_BG_COLOR);
	m_label_Title.SetText("����");

	m_label_Msg.SetTextColor(RGB_COLOR_RED);
	m_label_Msg.SetFont(&m_clFontBig);
	m_label_Msg.SetBorder(TRUE);
	m_label_Msg.SetFontSize(45);//(60);
	m_label_Msg.SetBkColor(MESSAGE_BG_COLOR);
	
	m_btn_ok.state = 100;
	m_btn_Cancel.state = 100;
	m_btn_ok._grident = false;
	m_btn_Cancel._grident = false;
	m_btn_ok.Invalidate();
	m_btn_Cancel.Invalidate();
}

void CInterLockDlg::setDlg(CString _title, CString  _msg, COLORREF bgColor, int index)
{

	/*m_label_Title.SetWindowText(_title);
	m_label_Msg.SetWindowText("\n"+_msg);*/

	CRect rect;
	int nSizeX;
	int nSizeY;

	GetClientRect(rect);

	nSizeX = GetSystemMetrics(SM_CXSCREEN);
	nSizeY = GetSystemMetrics(SM_CYSCREEN);

	m_sTitle = _title;
	m_sMsg = _msg;
	m_bgColor = bgColor;
	m_nIndex = index;

	m_label_Title.SetBkColor(m_bgColor);
	m_label_Title.SetWindowText(m_sTitle);

	m_label_Msg.SetBkColor(m_bgColor);
	m_label_Msg.SetWindowText(m_sMsg);

	//m_clColorButtonConfirm.ChangeColor(m_bgColor);

	rect.top = ((nSizeY - rect.bottom) / 2) + (m_nIndex * 25);
	rect.left = (nSizeX - rect.right) / 2 + (m_nIndex * 25);

	this->SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_NOSIZE);

}
void CInterLockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, MESSAGE_BG_COLOR);

    CRect iRect;
    this->GetWindowRect(iRect);
    //SystemParametersInfo(SPI_GETWORKAREA, NULL, &wndRect, NULL);

    int nWidth = iRect.right - iRect.left;
    int nHeight = iRect.bottom - iRect.top;

	int nShow = 0;
    nShow = this->SetWindowPos(&wndTopMost,(MAIN_DIALOG_SIZE_W /2) - (nWidth/2), (MAIN_DIALOG_SIZE_H / 2) - (nHeight/2), 0, 0, SWP_NOSIZE);
	//
	/*GetDlgItem(IDC_BUTTON_MESSAGE_MODAL_YES)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.top -= 3;
	rect.left -= 3;
	rect.right += 3;
	rect.bottom += 3;
	dc.FillSolidRect(rect, RGB_COLOR_WHITE);*/

	/*GetDlgItem(IDC_BUTTON_MESSAGE_MODAL_NO)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.top -= 3;
	rect.left -= 3;
	rect.right += 3;
	rect.bottom += 3;
	dc.FillSolidRect(rect, RGB_COLOR_WHITE);*/
}




BOOL CInterLockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

    LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
    SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
    BYTE byAlphaValue = 150;//0~255
    ::SetLayeredWindowAttributes(GetSafeHwnd(), 0, byAlphaValue, LWA_ALPHA);

	setInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CInterLockDlg::OnBnClickedBtnOk()
{
	EndDialog(IDYES);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CInterLockDlg::OnBnClickedBtnCancel()
{
	EndDialog(IDNO);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CInterLockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}
