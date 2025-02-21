// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdFpnSpecDlg.h"
#include "afxdialogex.h"


// CCcdFpnSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdFpnSpecDlg, CDialogEx)

CCcdFpnSpecDlg::CCcdFpnSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdFpnSpecDlg::IDD, pParent)
{
	m_nUnit = UNIT_AA1;
}

CCcdFpnSpecDlg::~CCcdFpnSpecDlg()
{
}

void CCcdFpnSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_TOP_BAR, m_clColorStaticTopBar);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT, m_clColorStaticCenterSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, m_clColorStaticCenterSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT, m_clColorStaticCornerSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, m_clColorStaticCornerSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT, m_clColorStaticCornerSfrDeviationLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, m_clColorStaticCornerSfrDeviationLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY, m_clColorStaticOCXY);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticOCXYVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticOCThVal);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
}


BEGIN_MESSAGE_MAP(CCcdFpnSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_FPN_SPEC_GRID_SPEC, &CCcdFpnSpecDlg::OnNMDblClickedFpnSpec)
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_FPN_TEST_SPEC_GRID_SPEC, &CCcdFpnSpecDlg::OnNMDblClickedFpnTestSpec)

	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdFpnSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdFpnSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdFpnSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnPaint()
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
BOOL CCcdFpnSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ

	this->InitFpnGridCtrl();
	this->InitFpnSpecGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	//m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	//m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	
	m_clColorStaticCenterSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCenterSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCenterSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticCornerSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCornerSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCornerSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticCornerSfrDeviationLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCornerSfrDeviationLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrDeviationLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCornerSfrDeviationLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrDeviationLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticOCXY.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCXY.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCXY.SetFont(&m_clFontSmall);

	m_clColorStaticOCXYVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCXYVal.SetFont(&m_clFontMid);

	m_clColorStaticOCTh.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCTh.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCTh.SetFont(&m_clFontSmall);

	m_clColorStaticOCThVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCThVal.SetFont(&m_clFontMid);

}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------

void CCcdFpnSpecDlg::InitFpnGridCtrl()
{
#if 0
	TCHAR* pszCol[] = { _T("FPN"), _T("Spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_FPN_SPEC_GRID_SPEC;//IDC_STATIC_CCD_RI4_SPEC_GRID_SPEC

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = 16;//g_Fpncount sizeof(FPN_SPEC_NAME) / sizeof(FPN_SPEC_NAME[0]) + 1;//�Ʒ�
	//15���� ��� �������� min,max ������̴�.
	int SpecCol = 2;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 200;
	int gridWidth2 = 90;

	int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth + margin;
	rect.bottom = (gridHeight*SpecRow) + margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridFpnSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridFpnSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridFpnSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridFpnSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridFpnSpec.SetReference_Setting();
	m_clGridFpnSpec.EnableSelection(FALSE);
	m_clGridFpnSpec.SetRowCount(SpecRow);
	m_clGridFpnSpec.SetColumnCount(SpecCol);
	m_clGridFpnSpec.SetFixedRowCount(1);
	m_clGridFpnSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
		m_clGridFpnSpec.SetRowHeight(i, gridHeight);
		m_clGridFpnSpec.SetItemText(i + 1, 0, FPN_SPEC_NAME[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridFpnSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridFpnSpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridFpnSpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridFpnSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
#endif
}


//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------

void CCcdFpnSpecDlg::InitFpnSpecGridCtrl()
{
#if 0
	TCHAR* pszCol[] = { _T("SPEC"), _T("5000k") , _T("Dark") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_FPN_TEST_SPEC_GRID_SPEC;//IDC_STATIC_CCD_RI4_SPEC_GRID_SPEC

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = 25;// sizeof(FPN_SPEC_NAME) / sizeof(FPN_SPEC_NAME[0]) + 1;//�Ʒ�
	int SpecCol = 3;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 100;
	int gridWidth2 = 90;

	int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth + margin;
	rect.bottom = (gridHeight*SpecRow) + margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridFpnTestSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridFpnTestSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridFpnTestSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridFpnTestSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridFpnTestSpec.SetReference_Setting();
	m_clGridFpnTestSpec.EnableSelection(FALSE);
	m_clGridFpnTestSpec.SetRowCount(SpecRow);
	m_clGridFpnTestSpec.SetColumnCount(SpecCol);
	m_clGridFpnTestSpec.SetFixedRowCount(1);
	m_clGridFpnTestSpec.SetFixedColumnCount(1);

	//m_clGridFpnTestSpec.SetItemText(1, 0, "SNR R MIN");
	//m_clGridFpnTestSpec.SetItemText(2, 0, "SNR R MAX");
	//m_clGridFpnTestSpec.SetItemText(3, 0, "SNR Gr MIN");
	//m_clGridFpnTestSpec.SetItemText(4, 0, "SNR Gr MAX");
	//m_clGridFpnTestSpec.SetItemText(5, 0, "SNR Gb MIN");
	//m_clGridFpnTestSpec.SetItemText(6, 0, "SNR Gb MAX");
	//m_clGridFpnTestSpec.SetItemText(7, 0, "SNR B MIN");
	//m_clGridFpnTestSpec.SetItemText(8, 0, "SNR B MAX");
	////
	//m_clGridFpnTestSpec.SetItemText(9, 0, "C FPN R MIN");
	//m_clGridFpnTestSpec.SetItemText(10, 0, "C FPN R MAX");
	//m_clGridFpnTestSpec.SetItemText(11, 0, "C FPN Gr MIN");
	//m_clGridFpnTestSpec.SetItemText(12, 0, "C FPN Gr MAX");
	//m_clGridFpnTestSpec.SetItemText(13, 0, "C FPN Gb MIN");
	//m_clGridFpnTestSpec.SetItemText(14, 0, "C FPN Gb MAX");
	//m_clGridFpnTestSpec.SetItemText(15, 0, "C FPN B MIN");
	//m_clGridFpnTestSpec.SetItemText(16, 0, "C FPN B MAX");
	////
	//m_clGridFpnTestSpec.SetItemText(17, 0, "SNR R MIN");
	//m_clGridFpnTestSpec.SetItemText(18, 0, "R FPN R MAX");
	//m_clGridFpnTestSpec.SetItemText(19, 0, "R FPN Gr MIN");
	//m_clGridFpnTestSpec.SetItemText(20, 0, "R FPN Gr MAX");
	//m_clGridFpnTestSpec.SetItemText(21, 0, "R FPN Gb MIN");
	//m_clGridFpnTestSpec.SetItemText(22, 0, "R FPN Gb MAX");
	//m_clGridFpnTestSpec.SetItemText(23, 0, "R FPN B MIN");
	//m_clGridFpnTestSpec.SetItemText(24, 0, "R FPN B MAX");
	for (i = 0; i < SpecRow; i++)
	{
		m_clGridFpnTestSpec.SetRowHeight(i, gridHeight);
		m_clGridFpnTestSpec.SetItemText(i + 1, 0, FPN_SPEC_NAME[15 + i]);
		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridFpnTestSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridFpnTestSpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridFpnTestSpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridFpnTestSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowFpnSpec();
	}
}


//-----------------------------------------------------------------------------
//
//	FPN SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::ShowFpnSpec()
{
	//TCHAR szData[SIZE_OF_100BYTE];
	//int i = 0;
	//int riTotal = 15;//
	//for (i = 0; i < riTotal; i++)	//g_Fpncount
	//{
	//	m_clGridFpnSpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_FpnSpec[i], 3);
	//}
	//int mFpnStart = riTotal;
	//for (i = mFpnStart; i < g_Fpncount + 1; i++)
	//{
	//	m_clGridFpnTestSpec.SetItemText(i - mFpnStart + 1, 1, g_clModelData[m_nUnit].m_FpnSpec[i], 3);
	//	m_clGridFpnTestSpec.SetItemText(i - mFpnStart + 1, 2, g_clModelData[m_nUnit].m_FpnSpec[i + 24], 3);
	//}

	//m_clGridFpnSpec.Invalidate();
	//m_clGridFpnTestSpec.Invalidate();
}
//-----------------------------------------------------------------------------
//
//	FPN ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------

void CCcdFpnSpecDlg::GetFpnSpec()
{
	//CString sData = _T("");
	//int i;
	//int _x = 0;
	//int _y = 0;
	//int riTotal = 15;//g_Fpncount sizeof(FPN_SPEC_NAME) / sizeof(FPN_SPEC_NAME[0]);
	//for (i = 0; i < riTotal; i++)
	//{
	//	sData = m_clGridFpnSpec.GetItemText(i + 1, 1);
	//	g_clModelData[m_nUnit].m_FpnSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	//}
	////15���� min,max �����̴�
	//int mFpnEnd = 24;	//24���� 2�� �ݺ��̶�
	//for (i = 0; i < mFpnEnd; i++)
	//{
	//	sData = m_clGridFpnTestSpec.GetItemText(i + 1, 1);
	//	g_clModelData[m_nUnit].m_FpnSpec[i + riTotal] = _ttof((TCHAR*)(LPCTSTR)sData);

	//	sData = m_clGridFpnTestSpec.GetItemText(i + 1, 2);
	//	g_clModelData[m_nUnit].m_FpnSpec[i + riTotal + 24] = _ttof((TCHAR*)(LPCTSTR)sData);
	//}
}
//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCenterSfrLimitVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{			
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticCenterSfrLimitVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	CORNER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrLimitVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{			
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticCornerSfrLimitVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	�ڳʺ� SFR ���� LIMIT
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrDeviationLimitVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{			
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticCornerSfrDeviationLimitVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	����˻� XY
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCXYVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{			
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticOCXYVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	����˻� TH
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCThVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{			
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticOCThVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}

	if (g_ShowMsgModal(_T("Ȯ��"), _T("ColorShading SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		return;

	this->GetFpnSpec();
	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	g_clModelData[m_nUnit].AcmisDataSave(g_clSysData.m_szModelName);//g_clSysData.m_szModelName = TESLA
	AddLog(_T("[INFO] RI SPEC ����"), 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnNMDblClickedFpnSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridFpnSpec.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridFpnSpec.SetItemText(nRow, nCol, sData);

				m_clGridFpnSpec.Invalidate();
			}

			delete pDlg;
		}
	}
}


//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnNMDblClickedFpnTestSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridFpnTestSpec.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridFpnTestSpec.SetItemText(nRow, nCol, sData);

				m_clGridFpnTestSpec.Invalidate();
			}

			delete pDlg;
		}
	}
}


//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdFpnSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdFpnSpecDlg::PreTranslateMessage(MSG* pMsg)
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
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}