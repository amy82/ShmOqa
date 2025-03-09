// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdChartSpecDlg.h"
#include "afxdialogex.h"


// CCcdChartSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdChartSpecDlg, CDialogEx)

CCcdChartSpecDlg::CCcdChartSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdChartSpecDlg::IDD, pParent)
{
	m_nUnit = UNIT_AA1;
}

CCcdChartSpecDlg::~CCcdChartSpecDlg()
{
}

void CCcdChartSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_TOP_BAR, m_clColorStaticTopBar);
	/*DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT, m_clColorStaticCenterSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, m_clColorStaticCenterSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT, m_clColorStaticCornerSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, m_clColorStaticCornerSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT, m_clColorStaticCornerSfrDeviationLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, m_clColorStaticCornerSfrDeviationLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY, m_clColorStaticOCXY);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticOCXYVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticOCThVal);*/

	DDX_Control(pDX, IDC_STATIC_CCD_ROTATION, m_clColorStaticHRotation);
	DDX_Control(pDX, IDC_STATIC_CCD_ROTATION_VAL, m_clColorStaticHRotationVal);
	



    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MIN, m_clColorStaticHfovMin);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MAX, m_clColorStaticHfovMax);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MIN_VAL, m_clColorStaticHfovMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MAX_VAL, m_clColorStaticHfovMaxVal);

    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MIN, m_clColorStaticVfovMin);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MAX, m_clColorStaticVfovMax);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MIN_VAL, m_clColorStaticVfovMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MAX_VAL, m_clColorStaticVfovMaxVal);

    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MIN, m_clColorStaticDistortionMin);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MAX, m_clColorStaticDistortionMax);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MIN_VAL, m_clColorStaticDistortionMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MAX_VAL, m_clColorStaticDistortionMaxVal);

	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
}

BEGIN_MESSAGE_MAP(CCcdChartSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_CHART_GRID_SPEC, &CCcdChartSpecDlg::OnNMDblClickedChartSpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_HFOV_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_HFOV_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMaxVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_VFOV_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_VFOV_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMaxVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_DISTORTION_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_DISTORTION_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMaxVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_ROTATION_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdRotationVal)
END_MESSAGE_MAP()


// CCcdChartSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnPaint()
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
BOOL CCcdChartSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	//this->InitChartGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	//m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	//m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	
	/*m_clColorStaticCenterSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCenterSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCenterSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimitVal.SetFont(&m_clFontMid);*/
/*
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
	m_clColorStaticOCThVal.SetFont(&m_clFontMid);*/
	m_clColorStaticHRotation.SetBkColor(RGB_DLG_BG);
	m_clColorStaticHRotation.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticHRotation.SetFont(&m_clFontSmall);
	

    m_clColorStaticHfovMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticHfovMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticHfovMin.SetFont(&m_clFontSmall);

    m_clColorStaticHfovMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticHfovMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticHfovMax.SetFont(&m_clFontSmall);
    m_clColorStaticHfovMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticHfovMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
	m_clColorStaticHRotationVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);

    m_clColorStaticVfovMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticVfovMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticVfovMin.SetFont(&m_clFontSmall);

    m_clColorStaticVfovMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticVfovMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticVfovMax.SetFont(&m_clFontSmall);
    m_clColorStaticVfovMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticVfovMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);

    m_clColorStaticDistortionMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticDistortionMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticDistortionMin.SetFont(&m_clFontSmall);

    m_clColorStaticDistortionMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticDistortionMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticDistortionMax.SetFont(&m_clFontSmall);
    m_clColorStaticDistortionMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticDistortionMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::InitChartGridCtrl()
{
	TCHAR* pszRow[] = { _T("") ,
		_T("dRealGapX"), _T("dRealGapY"), _T("nFiducialMarkType"),
		_T("dModuleChartDistance"), _T("nDistortionAlrotithmType"), _T("nEFL"),
		_T("dDistanceXFromCenter"), _T("dDistanceYFromCenter")
	};
	
	TCHAR* pszCol[] = { _T("Chart"), _T("spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_CHART_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = ChartMaxCount;//�Ʒ�
	int SpecCol = 2;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 180;
	int gridWidth2 = 110;
	int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth + margin;
	rect.bottom = (gridHeight*SpecRow) + margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
    m_clGridChartSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridChartSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridChartSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridChartSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridChartSpec.SetReference_Setting();
	m_clGridChartSpec.EnableSelection(FALSE);
	m_clGridChartSpec.SetRowCount(SpecRow);
	m_clGridChartSpec.SetColumnCount(SpecCol);
	m_clGridChartSpec.SetFixedRowCount(1);
	m_clGridChartSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
        m_clGridChartSpec.SetRowHeight(i, gridHeight);
        m_clGridChartSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
                    m_clGridChartSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
                    m_clGridChartSpec.SetColumnWidth(j, gridWidth2);
				}

                m_clGridChartSpec.SetItemText(i, j, pszCol[j]);
			}

            m_clGridChartSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowChartSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::ShowChartSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i = 0;
	//_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clModelData[m_nUnit].m_dChartRotation);
	//m_clColorStaticHRotationVal.SetWindowText(szData);
	

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clModelData[m_nUnit].m_dChartDistortionMin);
    m_clColorStaticDistortionMinVal.SetWindowText(szData);
    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clModelData[m_nUnit].m_dChartDistortionMax);
    m_clColorStaticDistortionMaxVal.SetWindowText(szData);
	


}
//---------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecSave()
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

	if (g_ShowMsgModal(_T("Ȯ��"), _T("CHART SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		return;

	this->GetChartSpec();

	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
    g_clModelData[m_nUnit].Save(g_clSysData.m_szModelName);
	g_clModelData[m_nUnit].AcmisDataSave(g_clSysData.m_szModelName);
	AddLog(_T("[INFO] CHART SPEC ����"), 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::GetChartSpec()
{
	CString sData = _T("");
	int i;

	///m_clColorStaticHRotationVal.GetWindowText(sData);
	///g_clModelData[m_nUnit].m_dChartRotation = _ttof((TCHAR*)(LPCTSTR)sData);




    m_clColorStaticDistortionMinVal.GetWindowText(sData);
    g_clModelData[m_nUnit].m_dChartDistortionMin = _ttof((TCHAR*)(LPCTSTR)sData);
    m_clColorStaticDistortionMaxVal.GetWindowText(sData);
    g_clModelData[m_nUnit].m_dChartDistortionMax = _ttof((TCHAR*)(LPCTSTR)sData);

	sData.Empty();
	g_clSysData.sDSave();
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnNMDblClickedChartSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridChartSpec.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
                m_clGridChartSpec.SetItemText(nRow, nCol, sData);

                m_clGridChartSpec.Invalidate();
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
void CCcdChartSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdChartSpecDlg::PreTranslateMessage(MSG* pMsg)
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

void CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticHfovMinVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticHfovMinVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticHfovMaxVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticHfovMaxVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticVfovMinVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticVfovMinVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticVfovMaxVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticVfovMaxVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticDistortionMinVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticDistortionMinVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticDistortionMaxVal.GetWindowText(sPos);
    CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
            m_clColorStaticDistortionMaxVal.SetWindowText(sPos);
        }

        delete pDlg;
    }
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdRotationVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticHRotationVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticHRotationVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}
