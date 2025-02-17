// CcdMaskViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdMaskViewDlg.h"
#include "afxdialogex.h"


#define WM_MARK_LEFT_MOVE_TIMER		WM_USER + 301
#define WM_MARK_RIGHT_MOVE_TIMER	WM_USER + 302
#define WM_MARK_UP_MOVE_TIMER		WM_USER + 303
#define WM_MARK_DOWN_MOVE_TIMER		WM_USER + 304

#define WM_MARK_MOVE_INTERVAL		50
#define MAX_BRUSH_SIZE				100

// CCcdMaskViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdMaskViewDlg, CDialogEx)

CCcdMaskViewDlg::CCcdMaskViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CCD_MASK_VIEW, pParent)
{
	int i = 0;
	m_nUnit = 0;
	m_nMarkNo = 0;
	for (i = 0; i < 2; i++)
	{
		
	}
	m_MilMask = M_NULL;
	m_MilMaskOverlay = M_NULL;
	m_pMaskBuff = NULL;

	m_bEnableOverlay = false;
	m_bInitOverlay = false;

	m_nMovePixel = 10;

	m_bDrawEdge = true;
	m_bMaskDrag = false;
	m_bEraseMask = false;

	m_nBrushSize = 10;
	m_nCircleSize = 10;
	m_nCircleSizeOffset = 10;
	m_nEdgeSmooth = 50;
	m_nZoomSize = 1;

	m_clPtMarkSize.x = m_clPtMarkSize.y = 0;
	m_clPtPicPos.x = m_clPtPicPos.y = 0;
}

CCcdMaskViewDlg::~CCcdMaskViewDlg()
{
}

void CCcdMaskViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CCD_MASK_VIEW_TITLE, m_clColorStaticTopBar);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MOVE_UP, m_clColorButtonMoveUp);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MOVE_DOWN, m_clColorButtonMoveDown);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MOVE_LEFT, m_clColorButtonMoveLeft);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MOVE_RIGHT, m_clColorButtonMoveRight);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MOVE_GAP, m_clColorButtonMoveGap);
	DDX_Control(pDX, IDC_STATIC_CCD_MASK_VIEW_EDGE_SMOOTH, m_clColorStaticSmooth);
	DDX_Control(pDX, IDC_STATIC_CCD_MASK_VIEW_EDGE_SMOOTH_VAL, m_clColorStaticSmoothVal);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_DRAW_EDGE, m_clColorButtonDrawEdge);
	DDX_Control(pDX, IDC_STATIC_CCD_MASK_VIEW_BRUSH_SIZE, m_clColorStaticBrushSize);
	DDX_Control(pDX, IDC_SLIDER_CCD_MASK_VIEW_BRUSH_SIZE, m_clSliderCtrlExBrushSize);
	DDX_Control(pDX, IDC_STATIC_CCD_MASK_VIEW_BRUSH_SIZE_VAL, m_clColorStaticBrushSizeVal);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MASK_CLEAR, m_clColorButtonMaskClear);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_MASK_ERASE, m_clColorButtonMaskErase);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_BG_MASK, m_clColorButtonMaskBG);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_MASK_VIEW_CLOSE, m_clColorButtonClose);
}


BEGIN_MESSAGE_MAP(CCcdMaskViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MOVE_UP, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MOVE_DOWN, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MOVE_LEFT, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MOVE_RIGHT, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveRight)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MOVE_GAP, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveGap)
	ON_STN_CLICKED(IDC_STATIC_CCD_MASK_VIEW_EDGE_SMOOTH_VAL, &CCcdMaskViewDlg::OnStnClickedStaticCcdMaskViewEdgeSmoothVal)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_DRAW_EDGE, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewDrawEdge)
	ON_STN_CLICKED(IDC_STATIC_CCD_MASK_VIEW_BRUSH_SIZE_VAL, &CCcdMaskViewDlg::OnStnClickedStaticCcdMaskViewBrushSizeVal)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MASK_CLEAR, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMaskClear)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_MASK_ERASE, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMaskErase)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_BG_MASK, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewBgMask)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_SAVE, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MASK_VIEW_CLOSE, &CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewClose)
END_MESSAGE_MAP()


// CCcdMaskViewDlg �޽��� ó�����Դϴ�.


//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;

	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(30, 29, 25));//RGB(40, 39, 35) RGB_DLG_BG);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CCcdMaskViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	WINDOWPLACEMENT wndpl;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);
	m_iMarkSetSizeX = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	m_iMarkSetSizeY = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;

	g_clModelFinder.m_clPtZoomMarkDispSize.x = m_iMarkSetSizeX;
	g_clModelFinder.m_clPtZoomMarkDispSize.y = m_iMarkSetSizeY;
#ifdef ON_LINE_MIL
	MbufAllocColor(g_clVision.m_MilSystem[0], 1L, CCD1_CAM_SIZE_X, CCD1_CAM_SIZE_Y, (8 + M_UNSIGNED), M_IMAGE + M_DISP + M_PROC, &g_clModelFinder.m_MilMarkImage[1]);

	if (g_clModelFinder.m_MilMarkImage[1] != M_NULL)
	{
		//MdispAlloc(g_clVision.m_MilSystem[0], M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &g_clModelFinder.m_MilMarkDisplay[1]);
		//g_clModelFinder.m_MilMarkDisplay[1] = MdispAlloc(g_clVision.m_MilSystem[0], M_DEV0, M_DISPLAY_SETUP, M_DEFAULT, M_NULL);

		g_clModelFinder.m_MilMarkDisplay[1] = MdispAlloc(g_clVision.m_MilSystem[0], M_DEV0, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
		MIL_INT DisplayType = MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_DISPLAY_TYPE, M_NULL);
		//if (MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_DISPLAY_TYPE, M_NULL) != M_WINDOWED)
		if (DisplayType != M_WINDOWED)
		{
			MdispFree(g_clModelFinder.m_MilMarkDisplay[1]);
			g_clModelFinder.m_MilMarkDisplay[1] = M_NULL;
		}
	}
#endif
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::InitCtrl()
{
	TCHAR szData[SIZE_OF_100BYTE];

	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);

	m_clColorStaticSmooth.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticSmooth.SetFont(&m_clFontSmall);

	m_clColorStaticSmoothVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticSmoothVal.SetFont(&m_clFontSmall);

	m_clColorStaticBrushSize.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticBrushSize.SetFont(&m_clFontSmall);

	m_clColorStaticBrushSizeVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticBrushSizeVal.SetFont(&m_clFontSmall);
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), m_nBrushSize);
	m_clColorStaticBrushSizeVal.SetWindowText(szData);


	m_clSliderCtrlExBrushSize.SetRange(0, MAX_BRUSH_SIZE);
	m_clSliderCtrlExBrushSize.SetTicFreq(10);
	m_clSliderCtrlExBrushSize.SetPos(m_nBrushSize);
}

//-----------------------------------------------------------------------------
//
//	����ũ ���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::InitMaskViewDlg(int nUnit, int nMarkNo, int m_iSizeX, int m_iSizeY)
{
#ifdef ON_LINE_MIL
	//MIL_ID MilGeoMetricBuff = M_NULL;
	TCHAR szData[SIZE_OF_100BYTE];
	double dSizeX;
	double dSizeY;
	int i = 0;
	WINDOWPLACEMENT wndpl;
	//m_iSizeX = Cam ���󿡼� �巡���� ���� ������
	//m_iSizeY = Cam ���󿡼� �巡���� ���� ������
	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);
	m_iMarkSetSizeX = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	m_iMarkSetSizeY = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top; 

	m_rcDispWin = wndpl.rcNormalPosition;
	/*for (i = 0; i < 2; i++)
	{
		m_MilMask[i] = M_NULL;
		m_MilMaskOverlay[i] = M_NULL;
	}*/

	long BufferAttributes = M_IMAGE + M_DISP + M_PROC;

	MbufAlloc2d(g_clVision.m_MilSystem[0], m_iMarkSetSizeX, m_iMarkSetSizeY, (8 + M_UNSIGNED), BufferAttributes, &g_clModelFinder.m_MilMarkImage[1]);

	if (g_clModelFinder.m_MilMarkImage[1])
	{ 
		if (g_clModelFinder.m_MilMarkDisplay[1])
		{
			MbufClear(g_clModelFinder.m_MilMarkImage[1], 0);

			g_clModelFinder.m_MilMarkDisplay[1] = M_NULL;
			m_MilMaskOverlay = M_NULL;

			g_clModelFinder.m_MilMarkDisplay[1] = MdispAlloc(g_clVision.m_MilSystem[0], M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

			MdispSelectWindow(g_clModelFinder.m_MilMarkDisplay[1], g_clModelFinder.m_MilMarkImage[1], GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->m_hWnd);
		}
	}

	this->EnableOverlay();
	dSizeX = dSizeY = 0.0;

	

	m_nUnit = nUnit;
	m_nMarkNo = nMarkNo;

	m_bMaskDrag = false;
	m_bInitOverlay = false;
	m_bEnableOverlay = false;
	
	m_pMaskBuff = NULL;
	//MilGeoMetricBuff = g_clModelFinder.m_MilModModel[nUnit][nMarkNo];

	m_nEdgeSmooth = g_clMarkData[nUnit].m_nSmooth[nMarkNo];
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), g_clMarkData[nUnit].m_nSmooth[nMarkNo]);
	m_clColorStaticSmoothVal.SetWindowText(szData);

	//�߰�
	m_dZoomX = (double)m_iMarkSetSizeX / (double)m_iSizeX;		//��ũ �̹��� ��� OR Ȯ��
	m_dZoomY = (double)m_iMarkSetSizeY / (double)m_iSizeY;

	MmodControl(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, 3203L, m_dZoomX);//M_DRAW_SCALE_X
	MmodControl(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, 3204L, m_dZoomY);//M_DRAW_SCALE_Y

	//MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[nUnit][nMarkNo], g_clModelFinder.m_MilMarkImage[1], M_DRAW_IMAGE + M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
	MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[nUnit][nMarkNo], g_clModelFinder.m_MilMarkImage[1], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT); 
	//
	// Mask Size
	//MmodInquire(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, M_ALLOC_SIZE_X, &dSizeX);
	//MmodInquire(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, M_ALLOC_SIZE_Y, &dSizeY);

	MmodInquire(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, M_REFERENCE_X + M_TYPE_DOUBLE, &m_clCdCenter.x);	//�巡�׵� �������� �߽� X
	MmodInquire(g_clModelFinder.m_MilModModel[nUnit][nMarkNo], M_DEFAULT, M_REFERENCE_Y + M_TYPE_DOUBLE, &m_clCdCenter.y);	//�巡�׵� �������� �߽� Y


	m_clPtMarkSize.x = (int)(m_iSizeX + 0.5);
	m_clPtMarkSize.y = (int)(m_iSizeY + 0.5);

	// ����ũ �̹��� �ʱ�ȭ
	MbufAllocColor(g_clVision.m_MilSystem[0], 1, m_iSizeX, m_iSizeY, (8 + M_UNSIGNED), M_IMAGE + M_DISP + M_PROC, &m_MilMask);
	if (m_MilMask != M_NULL)
	{
		m_pMaskBuff = (unsigned char*)malloc(m_iSizeX * m_iSizeY * sizeof(unsigned char));
		memset(m_pMaskBuff, 0, (m_iSizeX * m_iSizeY * sizeof(unsigned char))); 

		// �������� Ȱ��
		//this->EnableOverlay();
	}
	//this->InitMaskImage(m_iSizeX , m_iSizeY);

	// �̹��� ��������
	//this->ResizeImage(m_nZoomSize);
	//m_clCdCenter.x = m_iMarkSetSizeX / 2;
	//m_clCdCenter.y = m_iMarkSetSizeY / 2;
	// ���Ͷ��� �׸���
	this->DrawCenterLine(m_clCdCenter);

#endif
}

//-----------------------------------------------------------------------------
//
//	����ũ �̹��� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::InitMaskImage(int dSizeX, int dSizeY)
{
#ifdef ON_LINE_MIL
	if (m_MilMask != M_NULL)
	{
		m_pMaskBuff = (unsigned char*)malloc(dSizeX * dSizeY * sizeof(unsigned char));
		memset(m_pMaskBuff, 0, (dSizeX * dSizeY * sizeof(unsigned char)));

		// �������� Ȱ��
		this->EnableOverlay();
	}
#endif
}

//-----------------------------------------------------------------------------
//
//	�������� Ȱ��
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::EnableOverlay()
{
#ifdef ON_LINE_MIL
	if (m_bInitOverlay == false)
	{
		if (g_clModelFinder.m_MilMarkDisplay[1])
		{
			MIL_INT DisplayType = MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_DISPLAY_TYPE, NULL);
			
			//if (MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_DISPLAY_MODE, NULL) == M_WINDOWED)
			if (DisplayType == (M_WINDOWED | M_USER_WINDOW))
			{
				MdispControl(g_clModelFinder.m_MilMarkDisplay[1], M_OVERLAY, M_ENABLE);
				MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_OVERLAY_ID, &m_MilMaskOverlay);//[0]);

				//MdispControl(g_clModelFinder.m_MilMarkDisplay[1], M_OVERLAY_SHOW, M_ENABLE);

				MdispControl(g_clModelFinder.m_MilMarkDisplay[1], M_OVERLAY_SHOW, M_ENABLE);
				//M_WINDOW_OVR_SHOW
				MdispControl(g_clModelFinder.m_MilMarkDisplay[1], M_OVERLAY_CLEAR, M_DEFAULT);

				m_MilTransparentColor[0] = (MIL_DOUBLE)MdispInquire(g_clModelFinder.m_MilMarkDisplay[1], M_TRANSPARENT_COLOR, M_NULL);

				//MbufClear(m_MilMaskOverlay[0], m_MilTransparentColor[0]);
				MbufClear(m_MilMaskOverlay, m_MilTransparentColor[0]);
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

				m_bInitOverlay = true;
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//
//	����ũ �̹��� ��������
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::ResizeImage(int nZoom)
{
#ifdef ON_LINE_MIL
	/*WINDOWPLACEMENT wndpl;
	double dSizeX;
	double dSizeY;

	dSizeX = dSizeY = 0.0;*/

	//MbufClear(g_clModelFinder.m_MilMarkImage[1], 192);

	//MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_ALLOC_SIZE_X, &dSizeX);
	//MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_ALLOC_SIZE_Y, &dSizeY);

	//m_clPtMarkSize.x = (int)(dSizeX + 0.5);
	//m_clPtMarkSize.y = (int)(dSizeY + 0.5);

	//MdispZoom(g_clModelFinder.m_MilMarkDisplay[1], nZoom, nZoom);

	//MbufCopyColor2d(g_clModelFinder.m_MilMarkImage[0], g_clModelFinder.m_MilMarkImage[1], M_ALL_BAND, 0, 0, M_ALL_BAND, (int)0, (int)0, (LONG)(dSizeX + 0.5), (LONG)(dSizeY + 0.5));

	/*
	//������ �������ִ� �κ� dlg
	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + (int)((dSizeX + 0.5) * nZoom + 1.0);
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + (int)((dSizeY + 0.5) * nZoom + 1.0);
	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->SetWindowPlacement(&wndpl);
	*/
#endif
}

//-----------------------------------------------------------------------------
//
//	���� ���� �׸���
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::DrawCenterLine(CDMPoint clDPt)
{
#ifdef ON_LINE_MIL
	WINDOWPLACEMENT	wndpl;
	CDMPoint dPMarkCenterPoint, cdMaskPic;
	CRect clRect;

	dPMarkCenterPoint.x = clDPt.x;
	dPMarkCenterPoint.y = clDPt.y;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);
	cdMaskPic.x = (wndpl.rcNormalPosition.right);
	cdMaskPic.y = (wndpl.rcNormalPosition.bottom);

	MbufClear(m_MilMaskOverlay, m_MilTransparentColor[0]);

	this->DrawMask();
	
	MgraColor(M_DEFAULT, M_COLOR_RED);
	/*MgraLine(M_DEFAULT, m_MilMaskOverlay[0], (int)(dP.x + 0.5), 0, (int)(dP.x + 0.5), (int)cdMaskPic.y << 1);
	MgraLine(M_DEFAULT, m_MilMaskOverlay[0], 0, (int)(dP.y + 0.5), (int)cdMaskPic.x << 1, (int)(dP.y + 0.5));*/

	MgraLine(M_DEFAULT, m_MilMaskOverlay, (int)(dPMarkCenterPoint.x * m_dZoomX + 0.5), 0, (int)(dPMarkCenterPoint.x * m_dZoomX + 0.5), m_iMarkSetSizeY);
	MgraLine(M_DEFAULT, m_MilMaskOverlay, 0, (int)(dPMarkCenterPoint.y * m_dZoomY + 0.5), m_iMarkSetSizeX, (int)(dPMarkCenterPoint.y * m_dZoomY + 0.5));

	//## �׸� ���� �׸���
	/*
	CDPoint Mark_Size;
	Mark_Size.x = 50;
	Mark_Size.y = 50;

	clRect.left = (int)((int)(dPMarkCenterPoint.x + 0.5) - Mark_Size.x / 2);
	clRect.right = (int)((int)(dPMarkCenterPoint.x + 0.5) + Mark_Size.x / 2);
	clRect.top = (int)((int)(dPMarkCenterPoint.y + 0.5) - Mark_Size.y / 2);
	clRect.bottom = (int)((int)(dPMarkCenterPoint.y + 0.5) + Mark_Size.y / 2);

	MgraRect(M_DEFAULT, m_MilMaskOverlay[1], clRect.left, clRect.top, clRect.right, clRect.bottom);
	*/
	// �� �׸���.	
	m_nCircleSize = 20;
	MgraArc(M_DEFAULT, m_MilMaskOverlay, dPMarkCenterPoint.x* m_dZoomX, dPMarkCenterPoint.y* m_dZoomY, m_nCircleSize, m_nCircleSize, 0, 360);
#endif
}

//-----------------------------------------------------------------------------
//
//	����ũ �׸���
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::DrawMask()
{
#ifdef ON_LINE_MIL
	MbufClear(m_MilMask, 0);

	if (g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo] != M_NULL)
	{
		double m_dZoomX = (double)m_iMarkSetSizeX / (double)g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].x;      //��ũ �̹��� ��� OR Ȯ�� 
		double m_dZoomY = (double)m_iMarkSetSizeY / (double)g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].y;
		double m_dSmallX = (double)g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].x / (double)m_iMarkSetSizeX;      //��ũ �̹��� ��� OR Ȯ�� 
		double m_dSmallY = (double)g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].y / (double)m_iMarkSetSizeY;

		MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3203L, 1.0);//M_DRAW_SCALE_X
		MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3204L, 1.0);//M_DRAW_SCALE_Y 

		MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMask, M_DRAW_DONT_CARE, M_DEFAULT, M_DEFAULT);
		MbufGet(m_MilMask, m_pMaskBuff);

		MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3203L, m_dZoomX);//M_DRAW_SCALE_X
		MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3204L, m_dZoomY);//M_DRAW_SCALE_Y
		MgraColor(M_DEFAULT, M_COLOR_CYAN);// M_COLOR_GREEN);
		MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMaskOverlay, M_DRAW_DONT_CARE, M_DEFAULT, M_DEFAULT);

		if (m_bDrawEdge)
		{
			//MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_X, (double)1.0L);
			//MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_Y, (double)1.0L);

			MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
			MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_CONTEXT, M_SMOOTHNESS, m_nEdgeSmooth);
			MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMaskOverlay, M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
}
   }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//MbufClear(m_MilMask[0], 0); 

	//if (g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo] != M_NULL)
	//{
	//	MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMask[0], M_DRAW_DONT_CARE, M_DEFAULT, M_DEFAULT);
	//	MbufGet(m_MilMask[0], m_pMaskBuff);

	//	MgraColor(M_DEFAULT, M_COLOR_CYAN);// M_COLOR_GREEN);
	//	/*MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_REFERENCE_X, m_clCdCenter.x);
	//	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_REFERENCE_Y, m_clCdCenter.y);
	//	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_X, (double)1.0L);
	//	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_Y, (double)1.0L);*/

	//	MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMaskOverlay[1], M_DRAW_DONT_CARE, M_DEFAULT, M_DEFAULT);

	//	if (m_bDrawEdge)
	//	{
	//		//MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_X, (double)1.0L);
	//		//MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_DRAW_SCALE_Y, (double)1.0L);

	//		MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
	//		MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_CONTEXT, M_SMOOTHNESS, m_nEdgeSmooth);
	//		MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], m_MilMaskOverlay[1], M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
	//	}
	//}
#endif
}

//-----------------------------------------------------------------------------
//
//	��ũ ���
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::CopyBackup(int nMarkNo)
{
	TCHAR szPath[SIZE_OF_1K];
	TCHAR szBackupPath[SIZE_OF_1K];
	SYSTEMTIME stSysTime;
	CFileFind clFinder;

	GetLocalTime(&stSysTime);

	if (nMarkNo < 0 || nMarkNo >= MAX_MARK_COUNT)
		return;

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\Backup"), ROOT_PATH);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\Backup\\%s"), ROOT_PATH, g_clSysData.m_szModelName);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\Backup\\%s\\%04d%02d"), ROOT_PATH, g_clSysData.m_szModelName, stSysTime.wYear, stSysTime.wMonth);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\Backup\\%s\\%04d%02d\\%02d"), ROOT_PATH, g_clSysData.m_szModelName, stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\Backup\\%s\\%04d%02d\\%02d\\Mark"), ROOT_PATH, g_clSysData.m_szModelName, stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szBackupPath, SIZE_OF_1K, _T("%s\\Backup\\%s\\%04d%02d\\%02d\\Mark\\Mark-%d_%02d_%02d_%02d_%03d.mod"),
		BASE_DATA_PATH, g_clSysData.m_szModelName, stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, nMarkNo + 1, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, stSysTime.wMilliseconds);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\MARK-%d.mod"), BASE_DATA_PATH, nMarkNo + 1);

	CopyFile(szPath, szBackupPath, TRUE);

	_stprintf_s(szBackupPath, SIZE_OF_1K, _T("%s\\Backup\\%s\\%04d%02d\\%02d\\Mark\\Mark-%d_%02d_%02d_%02d_%03d.bmp"),
		BASE_DATA_PATH, g_clSysData.m_szModelName, stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, nMarkNo + 1, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, stSysTime.wMilliseconds);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\CAM%d\\MARK-%d.bmp"), BASE_DATA_PATH, m_nUnit + 1, nMarkNo + 1);

	CopyFile(szPath, szBackupPath, TRUE);
}

//-----------------------------------------------------------------------------
//
//	��ư : ��
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveUp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*m_clCdCenter.y -= m_nMovePixel;
	m_clCdCenter.y = (m_clCdCenter.y < 0) ? 1 : m_clCdCenter.y;*/

	if (m_clCdCenter.y  > m_nMovePixel + 1)
	{
		m_clCdCenter.y -= m_nMovePixel; 
		this->DrawCenterLine(m_clCdCenter);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : ��
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveDown()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*m_clCdCenter.y += m_nMovePixel;
	m_clCdCenter.y = (m_clCdCenter.y > m_clPtMarkSize.y) ? m_clPtMarkSize.y - 1 : m_clCdCenter.y;

	this->DrawCenterLine(m_clCdCenter);*/
	if (m_clCdCenter.y < m_rcDispWin.bottom - m_rcDispWin.top - m_nMovePixel - 1)
	{
		m_clCdCenter.y += m_nMovePixel;
		this->DrawCenterLine(m_clCdCenter);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : ��
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveLeft()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*m_clCdCenter.x -= m_nMovePixel;
	m_clCdCenter.x = (m_clCdCenter.x < 0) ? 1 : m_clCdCenter.x;*/
	if (m_clCdCenter.x  > m_nMovePixel + 1)
	{
		m_clCdCenter.x -= m_nMovePixel;
		this->DrawCenterLine(m_clCdCenter);
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : ��
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveRight()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*m_clCdCenter.x += m_nMovePixel;*/
	//m_clCdCenter.x = (m_clCdCenter.x > m_clPtMarkSize.x) ? m_clPtMarkSize.x - 1 : m_clCdCenter.x;
	if (m_clCdCenter.x < m_rcDispWin.right - m_rcDispWin.left  - m_nMovePixel - 1)
	{
		m_clCdCenter.x += m_nMovePixel;
		this->DrawCenterLine(m_clCdCenter);
	}
	
}

//-----------------------------------------------------------------------------
//
//	��ư : GAP
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMoveGap()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szData[SIZE_OF_100BYTE];

	switch (m_nMovePixel)
	{
	case 1:		m_nMovePixel = 5;		break;
	case 5:		m_nMovePixel = 10;		break;
	case 10:	m_nMovePixel = 20;		break;
	case 20:	m_nMovePixel = 50;		break;
	case 50:	m_nMovePixel = 1;		break;
	default:	m_nMovePixel = 1;		break;
	}

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), m_nMovePixel);
	m_clColorButtonMoveGap.SetWindowText(szData);
}

//-----------------------------------------------------------------------------
//
//	STATIC : EDGE SMOOTH
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnStnClickedStaticCcdMaskViewEdgeSmoothVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sVal = _T("");
	int nSmooth;

	m_clColorStaticSmoothVal.GetWindowText(sVal);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sVal, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			nSmooth = (int)pDlg->GetReturnVal();
			if (nSmooth >= 0 && nSmooth <= 100)
			{
				sVal.Format(_T("%d"), nSmooth);
				m_clColorStaticSmoothVal.SetWindowText(sVal);
				m_nEdgeSmooth = nSmooth;

				this->DrawCenterLine(m_clCdCenter);
			}
			else
			{
				AddLog(_T("[INFO] EDGE SMOOTH���� 0 ~ 100 �����Դϴ�."), 1, m_nUnit);
			}
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : DRAW EDGE
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewDrawEdge()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bDrawEdge = !m_bDrawEdge;

	if (m_bDrawEdge) {
		m_clColorButtonDrawEdge.state = 1;
	}
	else {
		m_clColorButtonDrawEdge.state = 0;
	}
	m_clColorButtonDrawEdge.Invalidate();
	this->ResizeImage(m_nZoomSize);

	this->DrawCenterLine(m_clCdCenter);
}

//-----------------------------------------------------------------------------
//
//	STATIC : BRUSH SIZE
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnStnClickedStaticCcdMaskViewBrushSizeVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sSize = _T("");
	int nSize;

	m_clColorStaticBrushSizeVal.GetWindowText(sSize);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sSize, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			nSize = (int)pDlg->GetReturnVal();
			if (nSize >= 0 && nSize <= 255)
			{
				sSize.Format(_T("%d"), nSize);
				m_clColorStaticBrushSizeVal.SetWindowText(sSize);
				m_nBrushSize = nSize;
				m_clSliderCtrlExBrushSize.SetPos(nSize);
			}
			else
			{
				AddLog(_T("[INFO] BRUSH SIZE���� 0 ~ 255 �����Դϴ�."), 1, m_nUnit);
			}
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
//	��ư : MASK CLEAR
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMaskClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_MIL
	MbufClear(m_MilMask, 0);

	if (m_pMaskBuff != NULL)
	{
		memset(m_pMaskBuff, 0x00, (m_clPtMarkSize.x * m_clPtMarkSize.y * sizeof(unsigned char)));

		MbufPut(m_MilMask, m_pMaskBuff);

		MmodMask(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, m_MilMask, M_DONT_CARE, M_DEFAULT);//<---�� ������?

		this->DrawCenterLine(m_clCdCenter);

		memset(m_pMaskBuff, 0x00, (m_clPtMarkSize.x * m_clPtMarkSize.y * sizeof(unsigned char)));
	}
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : MASK ERASE
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewMaskErase()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bEraseMask = !m_bEraseMask;

	if (m_bEraseMask)
		m_clColorButtonMaskErase.state = 1;
	else
		m_clColorButtonMaskErase.state = 0;
}

//-----------------------------------------------------------------------------
//
//	��ư : BG MASK
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewBgMask()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_MIL
	const int		nSpaceX = 2;
	const int		nSpaceY = 2;
	MIL_INT			m_nEdgeCount;
	MIL_DOUBLE		*m_pEdgePosX = NULL;	//! CHAIN POSITION-X
	MIL_DOUBLE		*m_pEdgePosY = NULL;	//! CHAIN POSITION-Y
	int				nPitch = 0;
	int				x = 0, y = 0;
	CSize			m_siMark;				//! MARK-SIZE
	CPoint			pt;
	CRect			rcDraw;

	m_siMark.cx = (INT)m_clPtMarkSize.x;		//mark ������ �Է�.
	m_siMark.cy = (INT)m_clPtMarkSize.y;

	MbufClear(m_MilMask, 0x00);
	memset(m_pMaskBuff, 0x00, (m_siMark.cx * m_siMark.cy));

	//! EDGE ���� GET
	MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_NUMBER_OF_CHAINED_EDGELS + M_TYPE_MIL_INT, &m_nEdgeCount);

	if (m_nEdgeCount < 1)		return;

	m_pEdgePosX = new MIL_DOUBLE[m_nEdgeCount];
	m_pEdgePosY = new MIL_DOUBLE[m_nEdgeCount];
	MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_CHAIN_X + M_TYPE_MIL_DOUBLE, m_pEdgePosX);
	MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_CHAIN_Y + M_TYPE_MIL_DOUBLE, m_pEdgePosY);

	//! FULL-MASK DRAW
	nPitch = m_siMark.cx;
	for (y = 0; y < m_siMark.cy; y++)
	{
		for (x = 0; x < m_siMark.cx; x++)
		{
			m_pMaskBuff[y * nPitch + x] = 255;
		}
	}

	////! Edge ���� CLEAR
	nPitch = m_siMark.cx;
	for (int i = 0; i < m_nEdgeCount; i++)
	{//! Edge ���� ��ġ
		pt.x = (int)m_pEdgePosX[i];
		pt.y = (int)m_pEdgePosY[i];

		rcDraw.left = pt.x - nSpaceX < 0 ? 0 : pt.x - nSpaceX;
		rcDraw.top = pt.y - nSpaceY < 0 ? 0 : pt.y - nSpaceY;
		rcDraw.right = pt.x + nSpaceX >= (int)(m_siMark.cx - 1) ? (int)(m_siMark.cx - 1) : pt.x + nSpaceX;
		rcDraw.bottom = pt.y + nSpaceY >= (int)(m_siMark.cy - 1) ? (int)(m_siMark.cy - 1) : pt.y + nSpaceY;

		for (y = rcDraw.top; y <= rcDraw.bottom; y++)
		{
			for (x = rcDraw.left; x <= rcDraw.right; x++)
			{
				m_pMaskBuff[y * nPitch + x] = 0x00;
			}
		}
	}

	MbufPut(m_MilMask, m_pMaskBuff);
	MmodMask(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, m_MilMask, M_DONT_CARE, M_DEFAULT);

	if (m_pEdgePosX) {
		delete[] m_pEdgePosX;
		m_pEdgePosX = 0;
	}
	if (m_pEdgePosY) {
		delete[] m_pEdgePosY;
		m_pEdgePosY = 0;
	}

	this->DrawCenterLine(m_clCdCenter);
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_MIL
	MbufClear(m_MilMask, 0x00);
	MbufPut(m_MilMask, m_pMaskBuff);

	///////////////////////////////////////////////////////////////////////
	//
	// Mask�� �׷��� MilMask buffer�� pModel�� Don't care ������ setting..
	//
	//////////////////////////////////////////////////////////////////////
	//MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], g_clModelFinder.m_MilMarkImage[1], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_REFERENCE_X, m_clCdCenter.x);
	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_REFERENCE_Y, m_clCdCenter.y);

	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3203L, 1.0);	  //M_DRAW_SCALE_X 
	MmodControl(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, 3204L, 1.0);	  //M_DRAW_SCALE_Y


	MmodMask(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, m_MilMask, M_DONT_CARE, M_DEFAULT);	//210707		<---���⼭ ���� ȭ�鿡 ����ũ �׸���
	MmodPreprocess(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT);
	//
	g_clModelFinder.SaveMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);
	//
	g_clMarkData[m_nUnit].m_nSmooth[m_nMarkNo] = m_nEdgeSmooth;

	g_clMarkData[m_nUnit].SaveData(g_clSysData.m_szModelName);

	g_clModelFinder.SettingFindMark(m_nUnit, m_nMarkNo);

	this->CopyBackup(m_nMarkNo);

	g_pCarAABonderDlg->m_clManualDlg.m_clMarkViewDlg.DisplayMarkView(m_nUnit, m_nMarkNo, m_iMarkSetSizeX, m_iMarkSetSizeY); 

	OnBnClickedButtonCcdMaskViewClose();
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnBnClickedButtonCcdMaskViewClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_MIL
	int i = 0;

	if (m_MilMask != M_NULL)
	{
		MbufFree(m_MilMask);
		m_MilMask = M_NULL;
	}
	if (m_pMaskBuff != NULL)
	{
		free(m_pMaskBuff);
	}

	g_clModelFinder.LoadMark(g_clSysData.m_szModelName);

	MdispDeselect(g_clModelFinder.m_MilMarkDisplay[1], g_clModelFinder.m_MilMarkImage[1]);
#endif

	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	WINDOWPLACEMENT wndpl;
	CRect clRectPos;
	CPoint clPoint;
	CRect m_rcMask;
	int nSx, nEx, nSy, nEy;
	int i, j;

	nSx = nEx = nSy = nEy = 0;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);

	clRectPos.left = wndpl.rcNormalPosition.left;
	clRectPos.right = wndpl.rcNormalPosition.right;
	clRectPos.top = wndpl.rcNormalPosition.top;
	clRectPos.bottom = wndpl.rcNormalPosition.bottom;

	if (point.x > clRectPos.left && point.y > clRectPos.top &&
		point.x < clRectPos.right && point.y < clRectPos.bottom)
	{
		clPoint.x = point.x - clRectPos.left;
		clPoint.y = point.y - clRectPos.top;

		m_rcMask.left = (int)(clPoint.x - m_nBrushSize);
		m_rcMask.top = (int)(clPoint.y - m_nBrushSize);
		m_rcMask.right = (int)(clPoint.x + m_nBrushSize);
		m_rcMask.bottom = (int)(clPoint.y + m_nBrushSize);

		m_bMaskDrag = true;
#ifdef ON_LINE_MIL
		MgraColor(M_DEFAULT, M_COLOR_GREEN);
#endif

		nSx = (int)(((clPoint.x - m_nBrushSize) / m_dZoomX) + 0.5);
		nEx = (int)(((clPoint.x + m_nBrushSize) / m_dZoomX) + 0.5);
		nSy = (int)(((clPoint.y - m_nBrushSize) / m_dZoomY) + 0.5);
		nEy = (int)(((clPoint.y + m_nBrushSize) / m_dZoomY) + 0.5);

		if (nSx < 0)					nSx = 0;
		if (nSy < 0)					nSy = 0;
		if (nEx > m_clPtMarkSize.x)		nEx = m_clPtMarkSize.x;
		if (nEy > m_clPtMarkSize.y)		nEy = m_clPtMarkSize.y;

		if ((nSx > m_clPtMarkSize.x) || (nSy > m_clPtMarkSize.y))
			return;

		if (m_pMaskBuff != NULL)
		{
			for (i = nSy; i < nEy; i++)
			{
				for (j = nSx; j < nEx; j++)
				{
					if (m_bEraseMask)	m_pMaskBuff[i * m_clPtMarkSize.x + j] = 0x00;
					else				m_pMaskBuff[i * m_clPtMarkSize.x + j] = 0xFF;
				}
			}

			//MgraRectFill(M_DEFAULT, m_MilMaskOverlay[1], nSx, nSy, nEx, nEy);
			MgraRectFill(M_DEFAULT, m_MilMaskOverlay, m_rcMask.left, m_rcMask.top, m_rcMask.right, m_rcMask.bottom);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	OnLButtonUp
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	WINDOWPLACEMENT wndpl;
	CRect clRectPos;
	CDMPoint dP, cdMaskPic;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);

	clRectPos.left = wndpl.rcNormalPosition.left;
	clRectPos.right = wndpl.rcNormalPosition.right;
	clRectPos.top = wndpl.rcNormalPosition.top;
	clRectPos.bottom = wndpl.rcNormalPosition.bottom;

	//! Mask Draw L_Button Up�� ���
	if ((m_bMaskDrag == true) && (point.x > clRectPos.left - 1) && (point.y > clRectPos.top - 1) &&
		(point.x < clRectPos.right - 1) && (point.y < clRectPos.bottom - 1))
	{
		dP.x = m_clCdCenter.x;
		dP.y = m_clCdCenter.y;

		GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);
		cdMaskPic.x = (wndpl.rcNormalPosition.right);
		cdMaskPic.y = (wndpl.rcNormalPosition.bottom);

#ifdef ON_LINE_MIL
		MgraColor(M_DEFAULT, M_COLOR_RED);
#endif
		//MgraLine(M_DEFAULT, m_MilMaskOverlay, (int)(dP.x + 0.5), 0, (int)(dP.x + 0.5), (int)cdMaskPic.y << 1);
		//MgraLine(M_DEFAULT, m_MilMaskOverlay, 0, (int)(dP.y + 0.5), (int)cdMaskPic.x << 1, (int)(dP.y + 0.5));

		//MgraArc(M_DEFAULT, m_MilMaskOverlay[1], dP.x, dP.y, iCircleSize, iCircleSize, 0, 360);
	}

	m_bMaskDrag = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	OnMouseMove
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	WINDOWPLACEMENT wndpl;
	CRect clRectPos;
	CPoint clPoint;
	CRect m_rcMask;
	int nSx, nEx, nSy, nEy;
	int x, y;

	nSx = nEx = nSy = nEy = 0;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_ZOOM_IMAGE)->GetWindowPlacement(&wndpl);

	clRectPos.left = wndpl.rcNormalPosition.left;
	clRectPos.right = wndpl.rcNormalPosition.right;
	clRectPos.top = wndpl.rcNormalPosition.top;
	clRectPos.bottom = wndpl.rcNormalPosition.bottom;


	if ((m_bMaskDrag == true) && 
		(point.x > clRectPos.left - 1) && (point.y > clRectPos.top - 1) &&
		(point.x < clRectPos.right - 1) && (point.y < clRectPos.bottom - 1))
	{

		clPoint.x = point.x - clRectPos.left;
		clPoint.y = point.y - clRectPos.top;

		m_rcMask.left = (int)(clPoint.x - m_nBrushSize );
		m_rcMask.top = (int)(clPoint.y - m_nBrushSize);
		m_rcMask.right = (int)(clPoint.x + m_nBrushSize);
		m_rcMask.bottom = (int)(clPoint.y + m_nBrushSize);

#ifdef ON_LINE_MIL
		if (m_bEraseMask == true)
		{
			MgraColor(M_DEFAULT, 0x00);
		}
		else
		{
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
		}
#endif
		nSx = (clPoint.x - m_nBrushSize) / m_dZoomX;
		nEx = (clPoint.x + m_nBrushSize) / m_dZoomX;
		nSy = (clPoint.y - m_nBrushSize) / m_dZoomY;
		nEy = (clPoint.y + m_nBrushSize) / m_dZoomY;

		if (nSx < 0)	nSx = 0;
		if (nSy < 0)	nSy = 0;

		if (nEx > m_clPtMarkSize.x)		nEx = m_clPtMarkSize.x;
		if (nEy > m_clPtMarkSize.y)		nEy = m_clPtMarkSize.y;
		if ((nSx > m_clPtMarkSize.x) || (nSy > m_clPtMarkSize.y))
			return;

		if (m_pMaskBuff != NULL)
		{
			for (x = nSx; x < nEx; x++)
			{
				for (y = nSy; y < nEy; y++)
				{
					if (m_bEraseMask)	m_pMaskBuff[y * m_clPtMarkSize.x + x] = (unsigned char)0x00;
					else				m_pMaskBuff[y * m_clPtMarkSize.x + x] = (unsigned char)0xFF;
				}
			}

			//MgraRectFill(M_DEFAULT, m_MilMaskOverlay[1], nSx, nSy, nEx, nEy);
			MgraRectFill(M_DEFAULT, m_MilMaskOverlay, m_rcMask.left, m_rcMask.top, m_rcMask.right, m_rcMask.bottom);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	OnHScroll
//
//-----------------------------------------------------------------------------
void CCcdMaskViewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	TCHAR szData[SIZE_OF_100BYTE];
	int nPosi;

	if (nSBCode == SB_ENDSCROLL)
		return;

	if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_CCD_MASK_VIEW_BRUSH_SIZE)->m_hWnd)
	{
		nPosi = m_clSliderCtrlExBrushSize.GetPos();

		m_nBrushSize = nPosi;
		_stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), nPosi);
		m_clColorStaticBrushSizeVal.SetWindowText(szData);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
BOOL CCcdMaskViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_STATIC_CCD_MASK_VIEW_TITLE)->m_hWnd)
		{
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


