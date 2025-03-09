// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdDefectSpecDlg.h"
#include "afxdialogex.h"


// CCcdDefectSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdDefectSpecDlg, CDialogEx)

CCcdDefectSpecDlg::CCcdDefectSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdDefectSpecDlg::IDD, pParent)
{
	m_nUnit = UNIT_AA1;
}

CCcdDefectSpecDlg::~CCcdDefectSpecDlg()
{
}

void CCcdDefectSpecDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CCcdDefectSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_DEFECT_SPEC_GRID_SPEC, &CCcdDefectSpecDlg::OnNMDblClickedSfrSpec)
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_UNIFORMITY_SPEC_GRID_SPEC, &CCcdDefectSpecDlg::OnNMDblClickedUniformitySpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdDefectSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnPaint()
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
BOOL CCcdDefectSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	m_nUIDiv = 25; //UI �������� ���
	this->InitGridCtrl();
	this->InitGridCtrl2();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::InitCtrl()
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
void CCcdDefectSpecDlg::InitGridCtrl()
{
	//TCHAR* pszRow[] = { _T("") ,
 //        _T("BlockSizeDark"), _T("BlockSizeBright"),
	//	_T("ThresholdTypeHot"), _T("ThresholdTypeDark"), _T("ThresholdTypeBright"),
	//	_T("DefectThresholdHot"), _T("DefectThresholdDark"), _T("DefectThresholdBright"),
	//	_T("MaxHotDefectNum"), _T("MaxDarkDefectNum"), _T("MaxBrightDefectNum"),
	//	_T("MaxHotClusterNum") ,_T("MaxDarkClusterNum"), _T("MaxBrightClusterNum"),
	//	_T("EnableChannel") , _T("DefectInCouplet"),     _T("DefectInCluster") ,
	//	_T("DefectInClusterChannel"),	_T("specColumnFPN")
	//};
	//
	//TCHAR* pszCol[] = { _T("Defect"), _T("Spec") };

	//CRect rect;
	//int i, j;
	//int DLG = IDC_STATIC_CCD_DEFECT_SPEC_GRID_SPEC;

	//CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	//int SpecRow = m_nUIDiv + 1; //sizeof(DEFECT_SPEC_NAME) / sizeof(DEFECT_SPEC_NAME[0]) + 1;//�Ʒ�
	//int SpecCol = 2;//��
	//int margin = 4;
	//int gridHeight = 25;
	//int gridWidth1 = 200;
	//int gridWidth2 = 90;
	//int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	////
	//pWnd->GetWindowRect(rect);
	//ScreenToClient(rect);

	//rect.right = totalWidth + margin;
	//rect.bottom = (gridHeight*SpecRow) + margin;
	//pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	//GetDlgItem(DLG)->GetWindowRect(rect);
	//ScreenToClient(rect);
	//m_clGridDefectSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	//m_clGridDefectSpec.SetTextBkColor(RGB_COLOR_WHITE);
	//m_clGridDefectSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	//m_clGridDefectSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	//m_clGridDefectSpec.SetReference_Setting();
	//m_clGridDefectSpec.EnableSelection(FALSE);
	//m_clGridDefectSpec.SetRowCount(SpecRow);
	//m_clGridDefectSpec.SetColumnCount(SpecCol);
	//m_clGridDefectSpec.SetFixedRowCount(1);
	//m_clGridDefectSpec.SetFixedColumnCount(1);

	//for (i = 0; i < SpecRow; i++)
	//{
	//	m_clGridDefectSpec.SetRowHeight(i, gridHeight);
	//	m_clGridDefectSpec.SetItemText(i+1, 0, DEFECT_SPEC_NAME[i]);

	//	for (j = 0; j < SpecCol; j++)
	//	{
	//		if (i == 0)
	//		{
	//			if (j == 0)
	//			{
	//				m_clGridDefectSpec.SetColumnWidth(j, gridWidth1);
	//			}
	//			else
	//			{
	//				m_clGridDefectSpec.SetColumnWidth(j, gridWidth2);
	//			}

	//			m_clGridDefectSpec.SetItemText(i, j, pszCol[j]);
	//		}

	//		m_clGridDefectSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	}
	//}
}

void CCcdDefectSpecDlg::InitGridCtrl2()
{
	//TCHAR* pszCol[] = { _T("Defect"), _T("Spec") };

	//CRect rect;
	//int i, j;
	//int DLG = IDC_STATIC_CCD_UNIFORMITY_SPEC_GRID_SPEC;

	//CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	//int SpecRow = (sizeof(DEFECT_SPEC_NAME) / sizeof(DEFECT_SPEC_NAME[0]) + 1) - m_nUIDiv;//�Ʒ�
	//int SpecCol = 2;//��
	//int margin = 4;
	//int gridHeight = 25;
	//int gridWidth1 = 200;
	//int gridWidth2 = 90;
	//int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	////
	//pWnd->GetWindowRect(rect);
	//ScreenToClient(rect);

	//rect.right = totalWidth + margin;
	//rect.bottom = (gridHeight*SpecRow) + margin;
	//pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	//GetDlgItem(DLG)->GetWindowRect(rect);
	//ScreenToClient(rect);
	//m_clGridUniformitySpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	//m_clGridUniformitySpec.SetTextBkColor(RGB_COLOR_WHITE);
	//m_clGridUniformitySpec.SetFixedBkColor(GRID_COLOR_TITLE);
	//m_clGridUniformitySpec.SetFixedTextColor(RGB_COLOR_WHITE);
	//m_clGridUniformitySpec.SetReference_Setting();
	//m_clGridUniformitySpec.EnableSelection(FALSE);
	//m_clGridUniformitySpec.SetRowCount(SpecRow);
	//m_clGridUniformitySpec.SetColumnCount(SpecCol);
	//m_clGridUniformitySpec.SetFixedRowCount(1);
	//m_clGridUniformitySpec.SetFixedColumnCount(1);

	//for (i = 0; i < SpecRow; i++)
	//{
	//	m_clGridUniformitySpec.SetRowHeight(i, gridHeight);
	//	m_clGridUniformitySpec.SetItemText(i + 1, 0, DEFECT_SPEC_NAME[i + m_nUIDiv]);

	//	for (j = 0; j < SpecCol; j++)
	//	{
	//		if (i == 0)
	//		{
	//			if (j == 0)
	//			{
	//				m_clGridUniformitySpec.SetColumnWidth(j, gridWidth1);
	//			}
	//			else
	//			{
	//				m_clGridUniformitySpec.SetColumnWidth(j, gridWidth2);
	//			}

	//			m_clGridUniformitySpec.SetItemText(i, j, pszCol[j]);
	//		}

	//		m_clGridUniformitySpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	}
	//}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowDefectSpec();
		this->ShowDefectSpec2();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::ShowDefectSpec()
{
	/*TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < m_nUIDiv; i++)
	{
		m_clGridDefectSpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_DefectSpec[i], 3);
	}
	m_clGridDefectSpec.Invalidate();*/
}
//-----------------------------------------------------------------------------
//
//	UNIFORMITY SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::ShowDefectSpec2()
{
	/*TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < g_Defectcount - m_nUIDiv; i++)
	{
		m_clGridUniformitySpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_DefectSpec[i + m_nUIDiv], 3);
	}
	m_clGridUniformitySpec.Invalidate();*/
}

//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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
	if (g_ShowMsgModal(_T("Ȯ��"), _T("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		return;

	this->GetDefectSpec();
    this->GetDefectSpec2();
	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	g_clModelData[m_nUnit].AcmisDataSave(g_clSysData.m_szModelName);
	AddLog(_T("[INFO] DEFECT/UNIFORM SPEC ����"), 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::GetDefectSpec()
{
	/*CString sData = _T("");
	int i;

	for (i = 0; i < m_nUIDiv; i++)
	{
		sData = m_clGridDefectSpec.GetItemText(i + 1, 1);
		g_clModelData[m_nUnit].m_DefectSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}

	g_clSysData.sDSave();*/
}
//-----------------------------------------------------------------------------
//
//	UNIFORMITY ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::GetDefectSpec2()
{
	/*CString sData = _T("");
	int i;

	for (i = 0; i < g_Defectcount - m_nUIDiv; i++)
	{
		sData = m_clGridUniformitySpec.GetItemText(i + 1, 1);
		g_clModelData[m_nUnit].m_DefectSpec[i + m_nUIDiv] = _ttof((TCHAR*)(LPCTSTR)sData);
	}
	g_clSysData.sDSave();*/
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnNMDblClickedSfrSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridDefectSpec.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridDefectSpec.SetItemText(nRow, nCol, sData);

				m_clGridDefectSpec.Invalidate();
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
void CCcdDefectSpecDlg::OnNMDblClickedUniformitySpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridUniformitySpec.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridUniformitySpec.SetItemText(nRow, nCol, sData);

				m_clGridUniformitySpec.Invalidate();
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
void CCcdDefectSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdDefectSpecDlg::PreTranslateMessage(MSG* pMsg)
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