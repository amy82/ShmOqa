// MessageInput.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MessageInput.h"
#include "afxdialogex.h"


// CMessageInput ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMessageInput, CDialogEx)

CMessageInput::CMessageInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MESSAGE_TYPE1, pParent)
{
	m_nUnit = 0;
	nPopupId = 0;
	editInputReadOnly = false;
	m_bgColor = MESSAGE_BG_COLOR;
	m_brush.CreateSolidBrush(MESSAGE_BG_COLOR);// RGB(255, 255, 200));  // ���ϴ� �������� �귯�� ����
}

CMessageInput::~CMessageInput()
{
}

void CMessageInput::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT_MSG_TEXT, m_edtMsgText);
	DDX_Control(pDX, IDC_STATIC_MESSAGE_TYPE1_TITLE, m_clColorStaticTitle);

	DDX_Control(pDX, IDC_BUTTON_MSG_SEND, m_clColorButtonConfirm);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageInput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MSG_SEND, &CMessageInput::OnBnClickedButtonMsgSend)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMessageInput �޽��� ó�����Դϴ�.

BOOL CMessageInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CMessageInput::InitCtrl()
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



	CFont m_font;
	// ��Ʈ ���� (Arial, ũ�� 20)
	m_font.CreatePointFont(135, _T("���� ���"));  // ũ�� 20����Ʈ�� 200 ���� ���
	m_edtMsgText.SetFont(&m_font);
	m_edtMsgText.ModifyStyle(0, ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN);

}
void CMessageInput::OnBnClickedButtonMsgSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//Send Button

	//S10F1 - Terminal Requeset
	int i = 0;
	CString tmsg;
	m_edtMsgText.GetWindowTextA(tmsg);


	//void CUbiGem::TerminalMessageSendRequest(int itid, CString tmsg)

	g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(OP_RECOGNIZED_TERMINAL_REPORT_10901, tmsg);
	ShowWindow(SW_HIDE);

	bool bPopupShow = false;

	for (i = 0; i < MAX_TERMINAL_COUNT; i++)
	{
		if (m_clTeminalMessageDlg[i] != NULL)
		{
			if (m_clTeminalMessageDlg[i]->IsWindowVisible() == TRUE)
			{
				bPopupShow = true;
				break;
			}
		}
	}
	if (bPopupShow == false)
	{
		g_pCarAABonderDlg->EnableWindow(TRUE);
	}

	tmsg.Empty();



	//�Ʒ� �κ� Optional �κ�
	//if (editInputReadOnly == true)
	//{
	//	g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(OP_RECOGNIZED_TERMINAL_REPORT_10901, tmsg);
	//}
	//else
	//{
	//	//Optional 
	//	//Enter Terminal Message !!!
	//	//g_pCarAABonderDlg->m_clUbiGemDlg.TerminalMessageSendRequest(g_clMesCommunication[m_nUnit].m_dTerminalId, tmsg);
	//}
	
}

void CMessageInput::setEditInput(CString title, int index, CString txt)
{
	//SetWindowTextA(title);
	CString str = _T("");
	str.Format(_T("Terminal Message[%d]"), index);
	m_clColorStaticTitle.SetWindowTextA(str);

	txt.Replace(_T("\n"), _T("\r\n"));
	m_edtMsgText.SetWindowTextA(txt);
	m_edtMsgText.Invalidate();
}
void CMessageInput::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.


	if (bShow)
	{
		g_pCarAABonderDlg->EnableWindow(FALSE);
		this->CenterWindow();
		if (editInputReadOnly)
		{
			m_edtMsgText.SetReadOnly(TRUE);
		}
		else
		{
			m_edtMsgText.SetReadOnly(FALSE);
		}



		CRect rect;
		this->GetWindowRect(&rect);
		int dOffsetX = 40;
		int dOffsetY = 40;
		int newX = rect.left + (dOffsetX * (nPopupId - (MAX_TERMINAL_COUNT / 2)));
		int newY = rect.top + (dOffsetY * (nPopupId - (MAX_TERMINAL_COUNT / 2)));

		// ���̾�α� ��ġ�� �� ��ǥ�� ����
		this->SetWindowPos(NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);//SWP_NOZORDER
	}
}




BOOL CMessageInput::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CMessageInput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_MSG_TEXT)  // m_edtMsg�� ����� ��Ʈ�� ID
	{
		pDC->SetBkColor(MESSAGE_BG_COLOR);// RGB(255, 255, 200));  // ���� ���� (���� �����)
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CMessageInput::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, m_bgColor);
}


void CMessageInput::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}
