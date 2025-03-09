// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdBlemishSpecDlg.h"
#include "afxdialogex.h"


// CCcdBlemishSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdBlemishSpecDlg, CDialogEx)

CCcdBlemishSpecDlg::CCcdBlemishSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdBlemishSpecDlg::IDD, pParent)
{
	m_nUnit = UNIT_AA1;
	m_brCheck.CreateSolidBrush(RGB(45, 45, 45));
}

CCcdBlemishSpecDlg::~CCcdBlemishSpecDlg()
{
}

void CCcdBlemishSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_TOP_BAR, m_clColorStaticTopBar);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT, m_clColorStaticCenterSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, m_clColorStaticCenterSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT, m_clColorStaticCornerSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, m_clColorStaticCornerSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT, m_clColorStaticCornerSfrDeviationLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, m_clColorStaticCornerSfrDeviationLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_LCB_SPEC_OFFSET, m_clColorStaticLcbOffset);
	DDX_Control(pDX, IDC_STATIC_CCD_YMEAN_SPEC_OFFSET, m_clColorStaticYmeanOffset);
	DDX_Control(pDX, IDC_STATIC_CCD_FDF_SPEC_OFFSET, m_clColorStaticFdfOffset);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticOCXYVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticOCThVal);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
	//
	DDX_Control(pDX, IDC_LCB_OFFSET, m_clLcbOffset);
	DDX_Control(pDX, IDC_YMEAN_OFFSET, m_clYmeanOffset);
	DDX_Control(pDX, IDC_FDF_OFFSET, m_clFdfOffset);
	
	DDX_Control(pDX, IDC_STATIC_CCD_BLEMISH_BLACKLEVEL_VAL, m_clColorStaticBlemishBlackLevelVal);

	DDX_Control(pDX, IDC_STATIC_CCD_BLEMISH_DATAFORMAT, m_clColorStaticCcdBlemishDataformat);
	DDX_Control(pDX, IDC_STATIC_CCD_BLEMISH_OUTMODE, m_clColorStaticCcdBlemishOutmode);
	DDX_Control(pDX, IDC_STATIC_CCD_BLEMISH_SENSORTYPE, m_clColorStaticCcdBlemishSensortype);
	DDX_Control(pDX, IDC_STATIC_CCD_BLEMISH_DEMOSAIC,				m_clColorStaticCcdBlemishDemosaic);
	DDX_Control(pDX, IDC_STATIC_CONFIG_CCD_BLEMISH_BLACKLEVEL,	m_clColorStaticCcdBlemishBlacklevel);

	DDX_Control(pDX, IDC_CCD_BLEMISH_COMBO_DATAFORMAT, m_CcdBlemishComboDataFormat);
	DDX_Control(pDX, IDC_CCD_BLEMISH_COMBO_OUTMODE, m_CcdBlemishComboOutMode);
	DDX_Control(pDX, IDC_CCD_BLEMISH_COMBO_SENSORTYPE, m_CcdBlemishComboSensorType);
	DDX_Control(pDX, IDC_CCD_BLEMISH_COMBO_DEMOSAIC, m_CcdBlemishComboDemosaic);
}


BEGIN_MESSAGE_MAP(CCcdBlemishSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_BLEMISH_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrBlockspot)
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_LCB_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrLcb)
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_YMEAN_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrYmean)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_BLEMISH_BLACKLEVEL_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdBlemishBlacklevelVal)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCcdBlemishSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnPaint()
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
BOOL CCcdBlemishSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
    this->InitBlemishGridCtrl();
    this->InitLcbGridCtrl();
    this->InitYmeanGridCtrl();

    
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::InitCtrl()
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

	m_clColorStaticLcbOffset.SetBkColor(RGB_DLG_BG);
	m_clColorStaticLcbOffset.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticLcbOffset.SetFont(&m_clFontSmall);

	m_clColorStaticYmeanOffset.SetBkColor(RGB_DLG_BG);
	m_clColorStaticYmeanOffset.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticYmeanOffset.SetFont(&m_clFontSmall);

	m_clColorStaticFdfOffset.SetBkColor(RGB_DLG_BG);
	m_clColorStaticFdfOffset.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticFdfOffset.SetFont(&m_clFontSmall);

	m_clColorStaticOCXYVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCXYVal.SetFont(&m_clFontMid);

	m_clColorStaticOCTh.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCTh.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCTh.SetFont(&m_clFontSmall);

	m_clColorStaticOCThVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCThVal.SetFont(&m_clFontMid);

	m_clColorStaticBlemishBlackLevelVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticBlemishBlackLevelVal.SetFont(&m_clFontMid);

	m_clColorStaticCcdBlemishDataformat.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCcdBlemishDataformat.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCcdBlemishDataformat.SetFont(&m_clFontSmall);

	m_clColorStaticCcdBlemishOutmode.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCcdBlemishOutmode.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCcdBlemishOutmode.SetFont(&m_clFontSmall);

	m_clColorStaticCcdBlemishSensortype.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCcdBlemishSensortype.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCcdBlemishSensortype.SetFont(&m_clFontSmall);

	m_clColorStaticCcdBlemishDemosaic.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCcdBlemishDemosaic.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCcdBlemishDemosaic.SetFont(&m_clFontSmall);

	m_clColorStaticCcdBlemishBlacklevel.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCcdBlemishBlacklevel.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCcdBlemishBlacklevel.SetFont(&m_clFontSmall);

	SetWindowTheme(GetDlgItem(IDC_CHECK_CCD_BLEMISH_8BITUSE)->m_hWnd, L"", L"");

	/*
	1. DataFormat
	2. OutMode
	3. SensorType
	4. DemosaicMethod
	*/
	m_CcdBlemishComboDataFormat.ResetContent();
	m_CcdBlemishComboOutMode.ResetContent();
	m_CcdBlemishComboSensorType.ResetContent();
	m_CcdBlemishComboDemosaic.ResetContent();
	//
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_YUV"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_8BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_10BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_PARALLEL_10BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_12BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_PARALLEL_12BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_16BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_16BIT_BE"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_16BIT_PWL_HDR"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_16BIT_PWL_DECOMP"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_24BIT"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_PARALLEL_10BIT_BE"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_BAYER_PARALLEL_12BIT_BE"));
	m_CcdBlemishComboDataFormat.AddString(_T("DATAFORMAT_RGB"));		//rgb = 100
																	//
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_BGGR"));	//1
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_RGGB"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GBRG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GRBG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_BLACKWHITE"));	//1
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_YUV422_YCbYCr"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_YUV422_YCrYCb"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_YUV422_CbYCrY"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_YUV422_CrYCbY"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_RGB_BGR"));		//1
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_RGB_RGB"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_BNGR"));	//1
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_RGNB"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_NBRG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GRBN"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_BGGN"));	//1
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_RGGN"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GBNG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GRNG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_NGGR"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_NGGB"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GNRG"));
	m_CcdBlemishComboOutMode.AddString(_T("OUTMODE_BAYER_GNBG"));
	//
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_RGGB"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_RCCC"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_RCCB"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_CCCC"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_RBCC"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_OTHER"));
	m_CcdBlemishComboSensorType.AddString(_T("SENSORTYPE_RGBIR2"));
	//

	m_CcdBlemishComboDemosaic.AddString(_T("DEMOSAIC_NONE"));
	m_CcdBlemishComboDemosaic.AddString(_T("GRADIENT"));
	m_CcdBlemishComboDemosaic.AddString(_T("BL33"));
	m_CcdBlemishComboDemosaic.AddString(_T("OPENCV_VNG"));
	m_CcdBlemishComboDemosaic.AddString(_T("VNG"));
	m_CcdBlemishComboDemosaic.AddString(_T("GR_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GB_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GBTF"));
	m_CcdBlemishComboDemosaic.AddString(_T("BLLC"));
	m_CcdBlemishComboDemosaic.AddString(_T("RESIDUAL"));
	m_CcdBlemishComboDemosaic.AddString(_T("BAYER_Y"));
	m_CcdBlemishComboDemosaic.AddString(_T("GRADIENT_RESIDUAL"));
	m_CcdBlemishComboDemosaic.AddString(_T("GBTF_RESIDUAL"));
	m_CcdBlemishComboDemosaic.AddString(_T("RESIDUAL_RESIDUAL"));
	m_CcdBlemishComboDemosaic.AddString(_T("BL33_RESIDUAL"));
	m_CcdBlemishComboDemosaic.AddString(_T("IR_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GRADIENT_G_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GBTF_G_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("RESIDUAL_G_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("BL33_G_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GRADIENT_IR_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("GBTF_IR_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("RESIDUAL_IR_ONLY"));
	m_CcdBlemishComboDemosaic.AddString(_T("BL33_IR_ONLY"));


	m_CcdBlemishComboDataFormat.SetCurSel(0);
	m_CcdBlemishComboOutMode.SetCurSel(0);
	m_CcdBlemishComboSensorType.SetCurSel(0);
	m_CcdBlemishComboDemosaic.SetCurSel(0);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::InitBlemishGridCtrl()
{
 //   TCHAR* pszCol[] = { _T("FDF"), _T("Spec") };

 //   CRect rect;
 //   int i, j;
 //   int DLG = IDC_STATIC_CCD_BLEMISH_SPEC_GRID_SPEC;

 //   CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	//int SpecRow = g_StainFdfcount + 1;// sizeof(STAIN_FDF_SPEC_NAME) / sizeof(STAIN_FDF_SPEC_NAME[0]) + 1;//�Ʒ�
 //   int SpecCol = 2;//��
 //   int margin = 4;
 //   int gridHeight = 25;
 //   int gridWidth1 = 140;
 //   int gridWidth2 = 90;
 //   int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
 //   //
 //   pWnd->GetWindowRect(rect);
 //   ScreenToClient(rect);

 //   rect.right = totalWidth + margin;
 //   rect.bottom = (gridHeight*SpecRow) + margin;
 //   pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


 //   GetDlgItem(DLG)->GetWindowRect(rect);
 //   ScreenToClient(rect);
 //   m_clGridBlemishSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

 //   m_clGridBlemishSpec.SetTextBkColor(RGB_COLOR_WHITE);
 //   m_clGridBlemishSpec.SetFixedBkColor(GRID_COLOR_TITLE);
 //   m_clGridBlemishSpec.SetFixedTextColor(RGB_COLOR_WHITE);
 //   m_clGridBlemishSpec.SetReference_Setting();
 //   m_clGridBlemishSpec.EnableSelection(FALSE);
 //   m_clGridBlemishSpec.SetRowCount(SpecRow);
 //   m_clGridBlemishSpec.SetColumnCount(SpecCol);
 //   m_clGridBlemishSpec.SetFixedRowCount(1);
 //   m_clGridBlemishSpec.SetFixedColumnCount(1);

 //   for (i = 0; i < SpecRow; i++)
 //   {
 //       m_clGridBlemishSpec.SetRowHeight(i, gridHeight);
 //       m_clGridBlemishSpec.SetItemText(i + 1, 0, STAIN_FDF_SPEC_NAME[i]);

 //       for (j = 0; j < SpecCol; j++)
 //       {
 //           if (i == 0)
 //           {
 //               if (j == 0)
 //               {
 //                   m_clGridBlemishSpec.SetColumnWidth(j, gridWidth1);
 //               }
 //               else
 //               {
 //                   m_clGridBlemishSpec.SetColumnWidth(j, gridWidth2);
 //               }

 //               m_clGridBlemishSpec.SetItemText(i, j, pszCol[j]);
 //           }

 //           m_clGridBlemishSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
 //       }
 //   }
}
void CCcdBlemishSpecDlg::InitLcbGridCtrl()
{
 //   TCHAR* pszCol[] = { _T("Lcb"), _T("Spec") };

 //   CRect rect;
 //   int i, j;
 //   int DLG = IDC_STATIC_CCD_LCB_SPEC_GRID_SPEC;

 //   CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	//int SpecRow = g_StainLcbcount + 1;// sizeof(STAIN_LCB_SPEC_NAME) / sizeof(STAIN_LCB_SPEC_NAME[0]) + 1;//�Ʒ�
 //   int SpecCol = 2;//��
 //   int margin = 4;
 //   int gridHeight = 25;
 //   int gridWidth1 = 140;
 //   int gridWidth2 = 90;
 //   int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
 //   //
 //   pWnd->GetWindowRect(rect);
 //   ScreenToClient(rect);

 //   rect.right = totalWidth + margin;
 //   rect.bottom = (gridHeight*SpecRow) + margin;
 //   pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


 //   GetDlgItem(DLG)->GetWindowRect(rect);
 //   ScreenToClient(rect);
 //   m_clGridLcbSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

 //   m_clGridLcbSpec.SetTextBkColor(RGB_COLOR_WHITE);
 //   m_clGridLcbSpec.SetFixedBkColor(GRID_COLOR_TITLE);
 //   m_clGridLcbSpec.SetFixedTextColor(RGB_COLOR_WHITE);
 //   m_clGridLcbSpec.SetReference_Setting();
 //   m_clGridLcbSpec.EnableSelection(FALSE);
 //   m_clGridLcbSpec.SetRowCount(SpecRow);
 //   m_clGridLcbSpec.SetColumnCount(SpecCol);
 //   m_clGridLcbSpec.SetFixedRowCount(1);
 //   m_clGridLcbSpec.SetFixedColumnCount(1);

 //   for (i = 0; i < SpecRow; i++)
 //   {
 //       m_clGridLcbSpec.SetRowHeight(i, gridHeight);
 //       m_clGridLcbSpec.SetItemText(i + 1, 0, STAIN_LCB_SPEC_NAME[i]);

 //       for (j = 0; j < SpecCol; j++)
 //       {
 //           if (i == 0)
 //           {
 //               if (j == 0)
 //               {
 //                   m_clGridLcbSpec.SetColumnWidth(j, gridWidth1);
 //               }
 //               else
 //               {
 //                   m_clGridLcbSpec.SetColumnWidth(j, gridWidth2);
 //               }

 //               m_clGridLcbSpec.SetItemText(i, j, pszCol[j]);
 //           }

 //           m_clGridLcbSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
 //       }
 //   }
}
void CCcdBlemishSpecDlg::InitYmeanGridCtrl()
{
    //TCHAR* pszRow[] = { _T("") ,
    //    _T("AnEdgeSize"), _T("AdCenterThreshold"), _T("AdEdgeThreshold"), _T("AdCornerThreshold"),
    //    _T("AnDefecBlockSize"), _T("AnLscBlockSize"), _T("AnCalcType"),
    //    _T("AnnMaxRecursiveCount"), _T("AdMaxDefectSize"), _T("AdPixelSize"),
    //    _T("AbEnableCircle"), _T("AnPosOffsetX"), _T("AnPosOffsetY"),
    //    _T("AdRadiusRatioX"), _T("AdRadiusRatioY"), _T("AdThresholdRatio"),
    //    _T("AdROIRange"), _T("AnUsedFixedCircle"), _T("AbEnableMultiCircle"),
    //    _T("dZoneSizeRatio1") ,_T("dZoneSizeRatio2"), _T("dZoneSizeRatio3"),
    //    _T("dThreshold1") ,_T("dThreshold2"), _T("dThreshold3")
    //};

    //TCHAR* pszCol[] = { _T("Ymean"), _T("Spec") };

    //CRect rect;
    //int i, j;
    //int DLG = IDC_STATIC_CCD_YMEAN_SPEC_GRID_SPEC;

    //CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
    //int SpecRow = sizeof(STAIN_YMEAN_SPEC_NAME) / sizeof(STAIN_YMEAN_SPEC_NAME[0]) + 1;//�Ʒ�
    //int SpecCol = 2;//��
    //int margin = 4;
    //int gridHeight = 25;
    //int gridWidth1 = 140;
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
    //m_clGridYmenaSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

    //m_clGridYmenaSpec.SetTextBkColor(RGB_COLOR_WHITE);
    //m_clGridYmenaSpec.SetFixedBkColor(GRID_COLOR_TITLE);
    //m_clGridYmenaSpec.SetFixedTextColor(RGB_COLOR_WHITE);
    //m_clGridYmenaSpec.SetReference_Setting();
    //m_clGridYmenaSpec.EnableSelection(FALSE);
    //m_clGridYmenaSpec.SetRowCount(SpecRow);
    //m_clGridYmenaSpec.SetColumnCount(SpecCol);
    //m_clGridYmenaSpec.SetFixedRowCount(1);
    //m_clGridYmenaSpec.SetFixedColumnCount(1);

    //for (i = 0; i < SpecRow; i++)
    //{
    //    m_clGridYmenaSpec.SetRowHeight(i, gridHeight);
    //    m_clGridYmenaSpec.SetItemText(i + 1, 0, STAIN_YMEAN_SPEC_NAME[i]);

    //    for (j = 0; j < SpecCol; j++)
    //    {
    //        if (i == 0)
    //        {
    //            if (j == 0)
    //            {
    //                m_clGridYmenaSpec.SetColumnWidth(j, gridWidth1);
    //            }
    //            else
    //            {
    //                m_clGridYmenaSpec.SetColumnWidth(j, gridWidth2);
    //            }

    //            m_clGridYmenaSpec.SetItemText(i, j, pszCol[j]);
    //        }

    //        m_clGridYmenaSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //    }
    //}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
        this->ShowFdfSpec();
        this->ShowLcbSpec();
        this->ShowYmeanSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::ShowFdfSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;


	/*for (i = 0; i < g_StainFdfcount; i++)
	{
		m_clGridBlemishSpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_FDFSpec[i], 3);
	}*/
	m_clFdfOffset.SetAddress(g_clModelData[m_nUnit].m_FDFOffsetSpec[0], g_clModelData[m_nUnit].m_FDFOffsetSpec[1], g_clModelData[m_nUnit].m_FDFOffsetSpec[2], g_clModelData[m_nUnit].m_FDFOffsetSpec[3]);

	//m_clGridBlemishSpec.Invalidate();
}
void CCcdBlemishSpecDlg::ShowLcbSpec()
{
    TCHAR szData[SIZE_OF_100BYTE];
    int i;

    /*for (i = 0; i < g_StainLcbcount; i++)
    {
        m_clGridLcbSpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_LcbSpec[i], 3);
    }*/

	m_clLcbOffset.SetAddress(g_clModelData[m_nUnit].m_LcbOffsetSpec[0], g_clModelData[m_nUnit].m_LcbOffsetSpec[1], g_clModelData[m_nUnit].m_LcbOffsetSpec[2], g_clModelData[m_nUnit].m_LcbOffsetSpec[3]);
    //m_clGridLcbSpec.Invalidate();
}
void CCcdBlemishSpecDlg::ShowYmeanSpec()
{
    TCHAR szData[SIZE_OF_100BYTE];
	CButton* pButton;
    int i;

    /*for (i = 0; i < g_StainYmeancount; i++)
    {
        m_clGridYmenaSpec.SetItemText(i + 1, 1, g_clModelData[m_nUnit].m_YmeanSpec[i], 3);
    }*/
	m_clYmeanOffset.SetAddress(g_clModelData[m_nUnit].m_YmeanOffsetSpec[0], g_clModelData[m_nUnit].m_YmeanOffsetSpec[1], g_clModelData[m_nUnit].m_YmeanOffsetSpec[2], g_clModelData[m_nUnit].m_YmeanOffsetSpec[3]);
    //m_clGridYmenaSpec.Invalidate();

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CCD_BLEMISH_8BITUSE);
	if (g_clModelData[m_nUnit].m_nBlemish8BitUse == 1)
		pButton->SetCheck(1);
	else
		pButton->SetCheck(0);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), g_clModelData[m_nUnit].m_dBlemishBlackLevel);
	m_clColorStaticBlemishBlackLevelVal.SetWindowText(szData);

	m_CcdBlemishComboDataFormat.SetCurSel(g_clModelData[m_nUnit].blemishParameter[0]);
	m_CcdBlemishComboOutMode.SetCurSel(g_clModelData[m_nUnit].blemishParameter[1]);
	m_CcdBlemishComboSensorType.SetCurSel(g_clModelData[m_nUnit].blemishParameter[2]);
	m_CcdBlemishComboDemosaic.SetCurSel(g_clModelData[m_nUnit].blemishParameter[3]);
}


//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::GetFdfSpec()
{
    CString sData = _T("");
    int i;

	/*for (i = 0; i < g_StainFdfcount; i++)
	{
		sData = m_clGridBlemishSpec.GetItemText(i + 1, 1);
		g_clModelData[m_nUnit].m_FDFSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}*/

	BYTE ipData[4];
	m_clFdfOffset.GetAddress(ipData[0], ipData[1], ipData[2], ipData[3]);
	for (i = 0; i < 4; i++)
	{
		g_clModelData[m_nUnit].m_FDFOffsetSpec[i] = ipData[i];
	}

    g_clSysData.sDSave();
}
void CCcdBlemishSpecDlg::GetLcbSpec()
{
    CString sData = _T("");
    int i;

    /*for (i = 0; i < g_StainLcbcount; i++)
    {
        sData = m_clGridLcbSpec.GetItemText(i + 1, 1);
        g_clModelData[m_nUnit].m_LcbSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
    }*/
	BYTE ipData[4];
	m_clLcbOffset.GetAddress(ipData[0], ipData[1], ipData[2], ipData[3]);
	for (i = 0; i < 4; i++)
	{
		g_clModelData[m_nUnit].m_LcbOffsetSpec[i] = ipData[i];
	}
    g_clSysData.sDSave();
}
void CCcdBlemishSpecDlg::GetYmeanSpec()
{
    CString sData = _T("");
	CButton* pButton;
    int i;
    /*for (i = 0; i < g_StainYmeancount; i++)
    {
        sData = m_clGridYmenaSpec.GetItemText(i + 1, 1);
        g_clModelData[m_nUnit].m_YmeanSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
    }*/
	BYTE ipData[4];
	m_clYmeanOffset.GetAddress(ipData[0], ipData[1], ipData[2], ipData[3]);
	for (i = 0; i < 4; i++)
	{
		g_clModelData[m_nUnit].m_YmeanOffsetSpec[i] = ipData[i];
	}
    g_clSysData.sDSave();

	GetDlgItem(IDC_STATIC_CCD_BLEMISH_BLACKLEVEL_VAL)->GetWindowText(sData);
	g_clModelData[m_nUnit].m_dBlemishBlackLevel = _ttoi((TCHAR*)(LPCTSTR)sData);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CCD_BLEMISH_8BITUSE);
	if (pButton->GetCheck() == 1)
		g_clModelData[m_nUnit].m_nBlemish8BitUse = 1;
	else
		g_clModelData[m_nUnit].m_nBlemish8BitUse = 0;


	g_clModelData[m_nUnit].blemishParameter[0] = m_CcdBlemishComboDataFormat.GetCurSel();
	g_clModelData[m_nUnit].blemishParameter[1] = m_CcdBlemishComboOutMode.GetCurSel();
	g_clModelData[m_nUnit].blemishParameter[2] = m_CcdBlemishComboSensorType.GetCurSel();
	g_clModelData[m_nUnit].blemishParameter[3] = m_CcdBlemishComboDemosaic.GetCurSel();

	g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec.eDataFormat = g_clLaonGrabberWrapper[m_nUnit].rDataFormatType(g_clModelData[m_nUnit].blemishParameter[0]);
	g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec.eOutMode = g_clLaonGrabberWrapper[m_nUnit].rOutmodeType(g_clModelData[m_nUnit].blemishParameter[1]);
	g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec.eSensorType = g_clLaonGrabberWrapper[m_nUnit].rSensortypeType(g_clModelData[m_nUnit].blemishParameter[2]);
	g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec.eDemosaicMethod = g_clLaonGrabberWrapper[m_nUnit].rdemosaicmethodType(g_clModelData[m_nUnit].blemishParameter[3]);
	g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec.nBlackLevel = g_clModelData[m_nUnit].m_dBlemishBlackLevel;

}
//---------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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

	if (g_ShowMsgModal(_T("Ȯ��"), _T("BLEMISH SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		return;

    this->GetFdfSpec();
    this->GetLcbSpec();
    this->GetYmeanSpec();

	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	g_clModelData[m_nUnit].Save(g_clSysData.m_szModelName);
	g_clModelData[m_nUnit].AcmisDataSave(g_clSysData.m_szModelName);
	AddLog(_T("[INFO] BLEMISH SPEC ����"), 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnNMDblClickedSfrBlockspot(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridBlemishSpec.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
                sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
                m_clGridBlemishSpec.SetItemText(nRow, nCol, sData);

                m_clGridBlemishSpec.Invalidate();
            }

            delete pDlg;
        }
    }
}

void CCcdBlemishSpecDlg::OnNMDblClickedSfrLcb(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridLcbSpec.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
                sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
                m_clGridLcbSpec.SetItemText(nRow, nCol, sData);

                m_clGridLcbSpec.Invalidate();
            }

            delete pDlg;
        }
    }
}

void CCcdBlemishSpecDlg::OnNMDblClickedSfrYmean(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridYmenaSpec.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
                sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
                m_clGridYmenaSpec.SetItemText(nRow, nCol, sData);

                m_clGridYmenaSpec.Invalidate();
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


//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdBlemishSpecDlg::PreTranslateMessage(MSG* pMsg)
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

void CCcdBlemishSpecDlg::OnStnClickedStaticCcdBlemishBlacklevelVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticBlemishBlackLevelVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			sPos.Format(_T("%d"), (int)pDlg->GetReturnVal());
			m_clColorStaticBlemishBlackLevelVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
	sPos.Empty();
}


HBRUSH CCcdBlemishSpecDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (pWnd->GetDlgCtrlID() == IDC_CHECK_CCD_BLEMISH_8BITUSE
		)
	{
		pDC->SetTextColor(RGB_COLOR_WHITE);
		pDC->SetBkColor(RGB(100, 100, 100));
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)m_brCheck;
		//return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
