// CcdMarkViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdMarkViewDlg.h"
#include "afxdialogex.h"


// CCcdMarkViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdMarkViewDlg, CDialogEx)

CCcdMarkViewDlg::CCcdMarkViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CCD_MARK_VIEW, pParent)
{
	m_nUnit = 0;
	m_nMarkNo = 0;
}

CCcdMarkViewDlg::~CCcdMarkViewDlg()
{
}

void CCcdMarkViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CCD_MARK_VIEW_TITLE, m_clColorStaticTitle);
	DDX_Control(pDX, IDC_STATIC_CCD_MARK_VIEW_MARK_NO, m_clColorStaticMarkNo);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_PREVIOUS, m_clColorButtonPrev);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_NEXT, m_clColorButtonNext);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_INIT, m_clColorButtonInit);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_REGIST, m_clColorButtonRegist);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_DELETE, m_clColorButtonDelete);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_ROI_SAVE, m_clColorButtonRoiSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_MASK, m_clColorButtonMarkView);
	DDX_Control(pDX, IDC_BUTTON_CCD_MARK_VIEW_FIND, m_clColorButtonMarkFind);

	DDX_Control(pDX, IDC_BUTTON_CCD_BMP_SAVE, m_clColorButtonImageSave);
	
}


BEGIN_MESSAGE_MAP(CCcdMarkViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_PREVIOUS, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewPrevious)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_NEXT, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewNext)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_INIT, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewInit)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_REGIST, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewRegist)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_DELETE, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewDelete)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_ROI_SAVE, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewRoiSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_MASK, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewMask)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MARK_VIEW_FIND, &CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewFind)
	ON_BN_CLICKED(IDC_BUTTON_CCD_BMP_SAVE, &CCcdMarkViewDlg::OnBnClickedButtonCcdBmpSave)
END_MESSAGE_MAP()


// CCcdMarkViewDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnPaint()
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
//
//
//-----------------------------------------------------------------------------
BOOL CCcdMarkViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	this->InitMarkViewDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTitle.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticTitle.SetFont(&m_clFontMid);

	m_clColorStaticMarkNo.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticMarkNo.SetFont(&m_clFontMid);


	m_clMaskViewDlg.Create(IDD_DIALOG_CCD_MASK_VIEW, this);
	m_clMaskViewDlg.ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	��ũ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::InitMarkViewDlg()
{
#ifdef ON_LINE_MIL
	WINDOWPLACEMENT wndpl;

	GetDlgItem(IDC_STATIC_ALIGN_MARK_VIEW_IMAGE)->GetWindowPlacement(&wndpl);


	double dSizeX = 0.0;
	double dSizeY = 0.0;

	dSizeX = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	dSizeY = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;

	g_clModelFinder.m_clPtSmallMarkDispSize.x = dSizeX;
	g_clModelFinder.m_clPtSmallMarkDispSize.y = dSizeY;

	//MbufAllocColor(g_clVision.m_MilSystem[0], 3, dSizeX, dSizeY, 8 + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &g_clModelFinder.m_MilMarkImage[0]);
	MbufAllocColor(g_clVision.m_MilSystem[0], 1L, dSizeX, dSizeY, (8 + M_UNSIGNED), M_IMAGE + M_DISP + M_PROC, &g_clModelFinder.m_MilMarkImage[0]);

	if (g_clModelFinder.m_MilMarkImage[0] != M_NULL)
	{
		//MdispAlloc(g_clVision.m_MilSystem[0], M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &g_clModelFinder.m_MilMarkDisplay[0]);

		//g_clModelFinder.m_MilMarkDisplay[0] = MdispAlloc(g_clVision.m_MilSystem[0], M_DEV0, M_DISPLAY_SETUP, M_DEFAULT, M_NULL);
		g_clModelFinder.m_MilMarkDisplay[0] = MdispAlloc(g_clVision.m_MilSystem[0], M_DEV0, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

		MIL_INT DisplayType = MdispInquire(g_clModelFinder.m_MilMarkDisplay[0], M_DISPLAY_TYPE, M_NULL);
		//if (MdispInquire(m_MilCamDisplay[nDispIndex][i], M_DISPLAY_TYPE, M_NULL) == M_WINDOWED)
		//if (DisplayType == (M_WINDOWED | M_USER_WINDOW))
		//if (MdispInquire(g_clModelFinder.m_MilMarkDisplay[0], M_DISPLAY_TYPE, M_NULL) != M_WINDOWED)
		if (DisplayType != M_WINDOWED)
		{
			MdispFree(g_clModelFinder.m_MilMarkDisplay[0]);
			g_clModelFinder.m_MilMarkDisplay[0] = M_NULL;
		}
	}

	g_clModelFinder.m_clPtMarkDispSize.x = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	g_clModelFinder.m_clPtMarkDispSize.y = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	g_clModelFinder.m_clPtMarkMaxSize.x = g_clModelFinder.m_clPtMarkDispSize.x;// MAX_MARK_SIZE_X;
	g_clModelFinder.m_clPtMarkMaxSize.y = g_clModelFinder.m_clPtMarkDispSize.y;// MAX_MARK_SIZE_Y;

	if (g_clModelFinder.m_MilMarkDisplay[0] != M_NULL)
	{
		MdispSelectWindow(g_clModelFinder.m_MilMarkDisplay[0], g_clModelFinder.m_MilMarkImage[0], GetDlgItem(IDC_STATIC_ALIGN_MARK_VIEW_IMAGE)->m_hWnd);
		g_clModelFinder.EnableOverlay();
	}

	this->DisplayMarkView(m_nUnit, m_nMarkNo , m_clMaskViewDlg.m_iMarkSetSizeX, m_clMaskViewDlg.m_iMarkSetSizeY);

	this->ShowMarkNo();

#endif
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::DisplayMarkView(int nUnit, int nMarkNo, double dZoomMarkWidth, double dZoomMarkHeight)
{
#ifdef ON_LINE_MIL
	CFileFind clFinder;
	TCHAR szPath[SIZE_OF_1K];
	double dZoomX, dZoomY;
	double dSizeX, dSizeY;
	double dCenterX, dCenterY;

	m_nUnit = nUnit;

	if (nMarkNo < 0 || nMarkNo > MAX_MARK_COUNT)
		return;

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\MARK-%d.bmp"), BASE_DATA_PATH, nMarkNo + 1);

	MbufClear(g_clModelFinder.m_MilMarkOverlay[0], g_clModelFinder.m_lTransparentColor[0]);
	MbufClear(g_clModelFinder.m_MilMarkImage[0], 192);

	if (clFinder.FindFile(szPath) == TRUE)
	{
		MbufImport(szPath, M_BMP, M_LOAD, M_NULL, &g_clModelFinder.m_MilMarkImage[1]);

		//MmodDraw(M_DEFAULT, g_clModelFinder.m_MilModModel[m_nUnit][nMarkNo], g_clModelFinder.m_MilMarkImage[0], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

		MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][nMarkNo], M_DEFAULT, M_ALLOC_SIZE_X, &dSizeX);		//<-----���⼭�� ��ũ ����� �ٲ���?
		MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][nMarkNo], M_DEFAULT, M_ALLOC_SIZE_Y, &dSizeY);
		//dSizeX = 300;
		//dSizeY = 300;
		g_clModelFinder.m_clPtMarkSize[m_nUnit][nMarkNo].x = (LONG)dSizeX;
		g_clModelFinder.m_clPtMarkSize[m_nUnit][nMarkNo].y = (LONG)dSizeY;

		MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][nMarkNo], M_DEFAULT, M_REFERENCE_X, &dCenterX);
		MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][nMarkNo], M_DEFAULT, M_REFERENCE_Y, &dCenterY);

		g_clModelFinder.m_clPtMarkCenterPos.x = (LONG)dCenterX;
		g_clModelFinder.m_clPtMarkCenterPos.y = (LONG)dCenterY;
		 
		//g_clModelFinder.m_clPtMarkDispSize = ��� ��ũ �̹��� dlg ������
		dZoomX = g_clModelFinder.m_clPtMarkDispSize.x / dZoomMarkWidth;// dZoomMarkWidth;// 656.0;// dSizeX - 0.36;		//0.3987 , dSizeX = �뷫660 �̾�ߵȴ�
		dZoomY = g_clModelFinder.m_clPtMarkDispSize.y / dZoomMarkHeight;// dZoomMarkHeight;// 501.0;// dSizeY;
		

		//MdispZoom(g_clModelFinder.m_MilMarkDisplay[0], 1.0 * dZoomX, 1.0 * dZoomY);
		MimResize(g_clModelFinder.m_MilMarkImage[1], g_clModelFinder.m_MilMarkImage[0], dZoomX, dZoomY, M_DEFAULT);


		g_clModelFinder.DrawMarkOverlay(m_nUnit, nMarkNo);	//<----���⼭ ���� ��ũ�� Line Draw
	}

	m_nMarkNo = nMarkNo;

	g_clTaskWork[m_nUnit].m_ManualMarkIndex = m_nMarkNo;
	this->ShowMarkNo();
#endif
}

//-----------------------------------------------------------------------------
//
//	��ũ��ȣ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::ShowMarkNo()
{
	TCHAR szTemp[SIZE_OF_1K]; 

	switch (m_nMarkNo)
	{
	case SENSOR_ALIGN_MARK:
		_stprintf_s(szTemp, SIZE_OF_1K, _T("%s"), MARK_ALIGN_NAME[SENSOR_ALIGN_MARK]);
		break;
	default:
		_stprintf_s(szTemp, SIZE_OF_1K, _T("Mark Empty"));
		break;

	}
	//_stprintf_s(szTemp, SIZE_OF_100BYTE, _T("%d / %d"), m_nMarkNo + 1, MAX_MARK_COUNT);
	m_clColorStaticMarkNo.SetWindowText(szTemp); 
}

//-----------------------------------------------------------------------------
//
//	��ũ ���
//
//-----------------------------------------------------------------------------
bool CCcdMarkViewDlg::RegisterMark(double dSizeX, double dSizeY, double dCenterX, double dCenterY)
{
#ifdef ON_LINE_MIL
	TCHAR szLog[SIZE_OF_1K];
	CRect clRectMarkPos;
	int nTempNo = m_nMarkNo;// 999;
	int i = 0;

	if (dSizeX <= 5 || dSizeY <= 5)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("MARK SIZE�� �ּ� MARK Size���� �۽��ϴ�."));
		AddLog(szLog, 1, m_nUnit);

		return false;
	}
	if (dSizeX >= MAX_MARK_SIZE_X || dSizeY >= MAX_MARK_SIZE_Y)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("MARK SIZE�� �ִ� MARK Size���� Ů�ϴ�."));
		AddLog(szLog, 1, m_nUnit);

		return false;
	}

	/*for (i = 0; i < MAX_MARK_COUNT; i++)
	{
		if (g_clModelFinder.m_bMarkState[m_nUnit][i] == false)
		{
			nTempNo = i;
			break;
		}
	}

	if (nTempNo == 999)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("����� �� ������ �����ϴ�. MARK ���� �� ����ϼ���"));
		AddLog(szLog, 1, m_nUnit);

		return false;
	}*/

	g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].x = (LONG)dSizeX;	//��ũ������ Drag Width
	g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].y = (LONG)dSizeY;	//��ũ������ Drag Height

	g_clModelFinder.m_clPtMarkCenterPos.x = (LONG)dCenterX;
	g_clModelFinder.m_clPtMarkCenterPos.y = (LONG)dCenterY;

	clRectMarkPos.left = (LONG)(dCenterX - dSizeX / 2);//���� ��ǥ
	clRectMarkPos.top = (LONG)(dCenterY - dSizeY / 2);

	clRectMarkPos.right = (LONG)(clRectMarkPos.left + dSizeX);
	clRectMarkPos.bottom = (LONG)(clRectMarkPos.top + dSizeY);

	g_clModelFinder.m_clPtMarkStartPos.x = clRectMarkPos.left;
	g_clModelFinder.m_clPtMarkStartPos.y = clRectMarkPos.top;

	if ((clRectMarkPos.left < 0) || (clRectMarkPos.top < 0) ||
		(clRectMarkPos.right > g_clModelData[m_nUnit].m_nWidth) || (clRectMarkPos.bottom > g_clModelData[m_nUnit].m_nHeight))
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("MARK ���� ������ ���� �ʽ��ϴ�."));
		AddLog(szLog, 1, m_nUnit);

		return false;
	}

	m_nMarkNo = nTempNo;
	///this->DisplayMarkView(m_nUnit, m_nMarkNo);
	////this->ShowMarkNo();

	if (g_clModelFinder.RegistMark(m_nUnit, m_nMarkNo) == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[FAIL] ��ũ ��� ����"));
		AddLog(szLog, 1, m_nUnit);
		return false;
	}
	m_clMaskViewDlg.InitMaskViewDlg(m_nUnit, m_nMarkNo , g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].x , g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo].y);
	m_clMaskViewDlg.ShowWindow(SW_SHOW);
	//this->DisplayMarkView(m_nUnit, m_nMarkNo);
	//this->ShowMarkNo();
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	��ư : ������
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewPrevious()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nMarkNo <= 0)
	{
		//g_ShowMsgPopup(_T("Ȯ��"), _T("ù ��° ��ũ�Դϴ�."), RGB_COLOR_RED);
		//return;
		m_nMarkNo = (MAX_MARK_COUNT - 1);
	}
	else
	{
		m_nMarkNo--;
	}

	
	this->DisplayMarkView(m_nUnit, m_nMarkNo, m_clMaskViewDlg.m_iMarkSetSizeX, m_clMaskViewDlg.m_iMarkSetSizeY);
}

//-----------------------------------------------------------------------------
//
//	��ư : ������
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nMarkNo >= (MAX_MARK_COUNT - 1))
	{
		/*g_ShowMsgPopup(_T("Ȯ��"), _T("������ ��ũ�Դϴ�."), RGB_COLOR_RED);
		return;*/
		m_nMarkNo = 0;
	}
	else
	{
		m_nMarkNo++;
	}

	
	this->DisplayMarkView(m_nUnit, m_nMarkNo, m_clMaskViewDlg.m_iMarkSetSizeX, m_clMaskViewDlg.m_iMarkSetSizeY);
}

//-----------------------------------------------------------------------------
//
//	��ư : INIT
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewInit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_MIL
	g_clVision.ClearOverlay(m_nUnit);

	g_pCarAABonderDlg->m_clVisionStaticCcd[m_nUnit].m_nDragType = MOUSE_DRAG;
	g_pCarAABonderDlg->m_clVisionStaticCcd[m_nUnit].m_bDrag = true;
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : REGIST
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewRegist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szLog[SIZE_OF_1K];
#ifdef ON_LINE_MIL
	CPoint	clPtSize;					//��� �� Mark Size ���庯��
	CPoint	clPtCenter;			//��� �� Mark �߽� ���庯��
	CRect clRect;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	/*if (g_ShowMsgModal(_T("Ȯ��"), _T("��ũ�� ��� �Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
		return;*/

	_stprintf_s(szLog, SIZE_OF_1K, _T("[Mark] Regist Start"));
	AddLog(szLog, 0, m_nUnit);
	//-------------------
	//�߰���
	//g_clModelFinder.DeleteMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);
	//g_clModelFinder.LoadMark(g_clSysData.m_szModelName);

	//// Mark ����
	//g_clModelFinder.ReSortMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);
	//this->DisplayMarkView(m_nUnit, m_nMarkNo);
	////

	clRect = g_pCarAABonderDlg->m_clVisionStaticCam[m_nUnit].GetRect();

	clPtSize.x = (LONG)(clRect.right - clRect.left);
	clPtSize.y = (LONG)(clRect.bottom - clRect.top);
	g_clModelFinder.m_clPtMarkSize[m_nUnit][m_nMarkNo] = clPtSize;

	clPtCenter.x =  (LONG)(clRect.left + (clPtSize.x / 2));
	clPtCenter.y = (LONG)(clRect.top + (clPtSize.y / 2));
	g_clModelFinder.m_clPtMarkCenterPos = clPtCenter;

	_stprintf_s(szLog, SIZE_OF_1K, _T("Mark Start X:%ld , Y:%ld"), clRect.left, clRect.top);
	AddLog(szLog, 0, m_nUnit);

	_stprintf_s(szLog, SIZE_OF_1K, _T("Mark Size Width:%ld , Height:%ld") , clPtSize.x , clPtSize.y);
	AddLog(szLog, 0, m_nUnit);

	if (this->RegisterMark(clPtSize.x, clPtSize.y, clPtCenter.x, clPtCenter.y) == false)
	{
		return;
	}
	//
	//g_clModelFinder.SaveMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);

	//g_clModelFinder.SettingFindMark(m_nUnit, m_nMarkNo);

	g_pCarAABonderDlg->m_clVisionStaticCam[m_nUnit].m_bDrag = false;
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : DELETE
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szLog[SIZE_OF_1K];

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clModelFinder.m_bMarkState[m_nUnit][m_nMarkNo] == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("��ϵ� ��ũ�� �������� �ʽ��ϴ�."));
		AddLog(szLog, 1, m_nUnit);

		return;
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("MARK-%d���� �����Ͻðڽ��ϱ�?"), m_nMarkNo + 1);
	if (g_ShowMsgModal(_T("Ȯ��"), szLog, RGB_COLOR_RED) == false)
		return;

	g_clModelFinder.DeleteMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);
	g_clModelFinder.LoadMark(g_clSysData.m_szModelName);

	// Mark ����
	g_clModelFinder.ReSortMark(g_clSysData.m_szModelName, m_nUnit, m_nMarkNo);
	this->DisplayMarkView(m_nUnit, m_nMarkNo, m_clMaskViewDlg.m_iMarkSetSizeX, m_clMaskViewDlg.m_iMarkSetSizeY);

	_stprintf_s(szLog, SIZE_OF_1K, _T("���õ� ��ũ�� �����Ͽ����ϴ�.[%s][MARK-%d]"), g_clSysData.m_szModelName, m_nMarkNo + 1);
	AddLog(szLog, 0, m_nUnit);

	g_pCarAABonderDlg->m_clVisionStaticCcd[m_nUnit].m_bDrag = false;
}

//-----------------------------------------------------------------------------
//
//	��ư : ROI SAVE
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewRoiSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CPoint clPtSize;
	CPoint clPtCenter;
	CRect clRect;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_ShowMsgModal(_T("Ȯ��"), _T("FIND ��ũ ROI������ ����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
		return;

	clRect = g_pCarAABonderDlg->m_clVisionStaticCam[m_nUnit].GetRect();
	clPtSize.x = (LONG)(clRect.right - clRect.left);
	clPtSize.y = (LONG)(clRect.bottom - clRect.top);

	//g_clModelFinder.m_clPtMarkSize = clPtSize;

	clPtCenter.x = (LONG)(clRect.left + (clPtSize.x / 2));
	clPtCenter.y = (LONG)(clRect.top + (clPtSize.y / 2));

	g_clModelFinder.m_clPtMarkCenterPos = clPtCenter;

	g_clMarkData[m_nUnit].m_clRectRoi[m_nMarkNo].left = (LONG)(clPtCenter.x - clPtSize.x / 2);
	g_clMarkData[m_nUnit].m_clRectRoi[m_nMarkNo].right = (LONG)(clPtCenter.x + clPtSize.x / 2);
	g_clMarkData[m_nUnit].m_clRectRoi[m_nMarkNo].top = (LONG)(clPtCenter.y - clPtSize.y / 2);
	g_clMarkData[m_nUnit].m_clRectRoi[m_nMarkNo].bottom = (LONG)(clPtCenter.y + clPtSize.y / 2);
	g_clMarkData[m_nUnit].SaveData(g_clSysData.m_szModelName);

	g_pCarAABonderDlg->m_clVisionStaticCam[m_nUnit].m_bDrag = false;
}

//-----------------------------------------------------------------------------
//
//	��ư : MASK VIEW
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewMask()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}
	double dSizeX = 0.0; 
	double dSizeY = 0.0;

	MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_ALLOC_SIZE_X, &dSizeX);		//<-----���⼭�� ��ũ ����� �ٲ���?
	MmodInquire(g_clModelFinder.m_MilModModel[m_nUnit][m_nMarkNo], M_DEFAULT, M_ALLOC_SIZE_Y, &dSizeY);

	m_clMaskViewDlg.InitMaskViewDlg(m_nUnit, m_nMarkNo , (int)dSizeX, (int)dSizeY);
	m_clMaskViewDlg.ShowWindow(SW_SHOW);
}

//-----------------------------------------------------------------------------
//
//	��ư : MARK FIND
//
//-----------------------------------------------------------------------------
void CCcdMarkViewDlg::OnBnClickedButtonCcdMarkViewFind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDMotor dmAlign;
	CDMPoint dpFind;
	bool bChk = false;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

	dmAlign.x = dmAlign.y = dmAlign.t = 0.0;
	dpFind.x = dpFind.y = 0.0;

	g_clVision.CalcSingleMarkAlign(m_nUnit, m_nMarkNo, 0, dmAlign, dpFind);

	g_pCarAABonderDlg->m_clManualDlg.ShowAlignValue(); 

	TCHAR szData[SIZE_OF_100BYTE];
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), dmAlign.x);
	g_pCarAABonderDlg->m_clManualDlg.m_clColorStaticAlignXVal.SetWindowText(szData); 
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), dmAlign.y);
	g_pCarAABonderDlg->m_clManualDlg.m_clColorStaticAlignYVal.SetWindowText(szData);
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), dmAlign.t);
	g_pCarAABonderDlg->m_clManualDlg.m_clColorStaticAlignTHVal.SetWindowText(szData);



	if (g_clModelData[m_nUnit].m_nDryRunMode == 1)
	{
		g_clMesCommunication[m_nUnit].m_dGrrAlignPos[0] = dmAlign.x;
		g_clMesCommunication[m_nUnit].m_dGrrAlignPos[1] = dmAlign.y;
		g_clMesCommunication[m_nUnit].m_dGrrAlignPos[2] = dmAlign.t;

		g_clMesCommunication[m_nUnit].g_Grr____Align(0);
	}

}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CCcdMarkViewDlg::PreTranslateMessage(MSG* pMsg)
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



void CCcdMarkViewDlg::OnBnClickedButtonCcdBmpSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ��մϴ�."), 1, m_nUnit);
		return;
	}

#ifdef ON_LINE_MIL
	CString sPath = _T("");
	CString sTemp = _T("");

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("*.bmp|*.BMP|"), _T("IMAGE"), OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("Image Files (*.bmp)|*.bmp|"));
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			sPath = pDlg->GetPathName();
			sTemp = sPath.Right(4);
			if (sTemp.CompareNoCase(_T(".bmp")) != 0)
				sPath += _T(".bmp");

			MbufExport(sPath, M_BMP, g_clVision.m_MilGrabImage[0][0]);
		}
		delete pDlg;
	}
#endif
}
