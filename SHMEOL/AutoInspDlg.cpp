
// TESLAInspInspDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "AutoInspDlg.h"
#include "afxdialogex.h"
#include "GlobalDeclare.h"
#define WM_UI_CM_TIMER				WM_USER + 992
#define WM_DELETE_TIMER				WM_USER + 995
//#include "uxmodule\uxgraph\UXGraph.h"

#ifdef _DEBUG
#include <crtdbg.h>
//#define new DEBUG_NEW
//#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
//#define malloc(s)_malloc_dbg(s,_NORMAL_BLOCK,__FILE__,__LINE__)


#endif

enum DLG_TYPE { DLG_MAIN = 0, DLG_MANUAL, DLG_TEACH, DLG_CCD, DLG_DIO, DLG_LIGHT, DLG_ALARM, DLG_CONFIG };
enum DLG_MOVE_TYPE { SHOW_ALL = 0, SHOW_UNIT1, SHOW_UNIT2 };
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

//UXGraph CUXGraph[2];
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoInspDlg ��ȭ ����

//-----------------------------------------------------------------------------
//
//	������� USB �̺�Ʈ �ڵ鷯
//
//-----------------------------------------------------------------------------
void CALLBACK USBEventHandler(char iIndex, int Event)
{
 //   TCHAR szLog[SIZE_OF_1K];
 //   int i;
	//return;
 //   switch (Event)
 //   {
 //   case MIU_EVENT_TIMEOUT:
 //       _stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] USB TIME OUT : BOARD NO: %d, EVENT: %d"), iIndex, Event);
 //       AddLog(szLog, 0, iIndex);
 //       break;
 //       // USB ���� ������ �߻�
 //   case MIU_EVENT_ATTACHED:
 //       _stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] USB ATTACHED"));
 //       //AddLog(szLog, 999, iIndex);
 //       AddLog(szLog, 999, 0);
 //       AddLog(szLog, 999, 1);
 //       for (i = 0; i < MAX_UNIT_COUNT; i++)
 //       {
 //           g_clLaonGrabberWrapper[i].CloseDevice();

 //           Sleep(2000);

 //           if (g_clLaonGrabberWrapper[i].OpenDevice() == true)
 //           {
 //               g_pCarAABonderDlg->ShowIconState(i);
 //           }

 //       }

 //       break;
 //       // USB ���� ������ �߻�
 //   case MIU_EVNET_DETACHED:
 //       _stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] USB DETACHED"));
 //       //AddLog(szLog, 999, iIndex);
 //       AddLog(szLog, 999, 0);
 //       AddLog(szLog, 999, 1);
 //       for (i = 0; i < MAX_UNIT_COUNT; i++)
 //       {
 //           if (g_clLaonGrabberWrapper[i].OpenDevice() == true)
 //               g_pCarAABonderDlg->ShowIconState(i);
 //       }
 //       break;

 //   default:
 //       break;
 //   }
}


CAutoInspDlg::CAutoInspDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AUTO_INSP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int i, j;

	for (i = 0; i < MAX_POPUP_COUNT; i++)
	{
		g_pMessagePopupDlg[i] = NULL;
	}
	g_nPopupIndex = 0;

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < MAX_GRAPH_COUNT; j++)
		{
			m_pGraphWnd[i][j] = NULL;
		}
		m_bPrevCcdState[i] = false;
		m_bPrevLinkState[i] = false;
		m_nSignal[i] = -1;

		m_bMasterModeState[i] = false;

		m_bCamState[i] = VIDEO_CCD;
	}

	m_nModelType = -1;

	m_nCurrentDlg = DLG_MANUAL;// DLG_MAIN;
	m_nCurrentUnit = UNIT_AA1;
	m_bConnectMain = false;


	
}

void CAutoInspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//STATIC 1
	//DDX_Control(pDX, IDC_STATIC_MAIN_TITLE1, m_clColorStaticTitle[0]);
	//DDX_Control(pDX, IDC_STATIC_MAIN_TITLE2, m_clColorStaticTitle[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_VERSION1, m_clColorStaticVersion[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_VERSION2, m_clColorStaticVersion[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_MODEL_NO1, m_clColorStaticModelNo[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_MODEL_NO2, m_clColorStaticModelNo[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_LOT_ID1, m_clColorStaticLotID[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_LOT_ID2, m_clColorStaticLotID[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CCD_ID1, m_clColorStaticCcdID[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CCD_ID2, m_clColorStaticCcdID[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CURR_MODE1, m_clColorStaticCurrMode[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CURR_MODE2, m_clColorStaticCurrMode[1]);

	DDX_Control(pDX, IDC_STATIC_MAIN_CURR_MODE3, m_clColorStaticCurrAlignMode[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CURR_MODE4, m_clColorStaticCurrAlignMode[1]);
	
	DDX_Control(pDX, IDC_STATIC_MAIN_BCR1, m_clColorStaticBcr[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_BCR2, m_clColorStaticBcr[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_BCR_VAL1, m_clColorStaticBcrVal[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_BCR_VAL2, m_clColorStaticBcrVal[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_OUTPUT1, m_clColorStaticOutput[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_OUTPUT2, m_clColorStaticOutput[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_OUTPUT_VAL1, m_clColorStaticOutputVal[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_OUTPUT_VAL2, m_clColorStaticOutputVal[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_PIN1, m_clColorStaticPin[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_PIN2, m_clColorStaticPin[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_PIN_VAL1, m_clColorStaticPinVal[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_PIN_VAL2, m_clColorStaticPinVal[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_FPS1, m_clColorStaticFps[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_FPS2, m_clColorStaticFps[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_FPS_VAL1, m_clColorStaticFpsVal[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_FPS_VAL2, m_clColorStaticFpsVal[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_MODEL_TYPE1, m_clColorStaticPcbType[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_MODEL_TYPE2, m_clColorStaticPcbType[1]);

	DDX_Control(pDX, IDC_STATIC_CURRENT_DATE, m_clColorStaticCurrentDate);
	DDX_Control(pDX, IDC_STATIC_CURRENT_TIME, m_clColorStaticCurrentTime);
	DDX_Control(pDX, IDC_LIST_MAIN_LOG1, m_clListLog[0]);
	DDX_Control(pDX, IDC_LIST_MAIN_LOG2, m_clListLog[1]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CCD1, m_clVisionStaticCcd[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CCD2, m_clVisionStaticCcd[1]);

	DDX_Control(pDX, IDC_STATIC_MAIN_CAM1, m_clVisionStaticCam[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_CAM2, m_clVisionStaticCam[1]);
	
	//
	//Unit1 �޴�
	DDX_Control(pDX, IDC_BUTTON_MAIN_STARTING_POINT1, m_clColorButtonStartingPoint[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_READY1, m_clColorButtonAutoReady[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_RUN1, m_clColorButtonAutoRun[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_STOP1, m_clColorButtonAutoStop[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_PAUSE1, m_clColorButtonAutoPause[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_EMISSION1, m_clColorButtonComplEmission[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_OK1, m_clColorButtonComplOk[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_NG1, m_clColorButtonComplNg[0]);

	//MASTER MODE ��ư
	DDX_Control(pDX, IDC_BUTTON_MASTER_CHAGE_MODE1, m_clColorButtonMasterMode[0]);
	DDX_Control(pDX, IDC_BUTTON_MASTER_CHAGE_MODE2, m_clColorButtonMasterMode[1]);

	//���� �Ϸ� ��ư
	DDX_Control(pDX, IDC_BUTTON_EPOXY_COMPLETE1, m_clColorButtonEpoxyCompl[0]);
	DDX_Control(pDX, IDC_BUTTON_EPOXY_COMPLETE2, m_clColorButtonEpoxyCompl[1]);

	//LENS �ѱ� �Ϸ� ��ư
	DDX_Control(pDX, IDC_BUTTON_LENS_LOADING1, m_clColorButtonLensLoading[0]);
	DDX_Control(pDX, IDC_BUTTON_LENS_LOADING2, m_clColorButtonLensLoading[1]);

	DDX_Control(pDX, IDC_BUTTON_MODEL_SELCT, m_clColorButtonModelSelect);
	
	//Unit1 �޴�
	DDX_Control(pDX, IDC_BUTTON_MAIN_STARTING_POINT2, m_clColorButtonStartingPoint[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_READY2, m_clColorButtonAutoReady[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_RUN2, m_clColorButtonAutoRun[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_STOP2, m_clColorButtonAutoStop[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_AUTO_PAUSE2, m_clColorButtonAutoPause[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_EMISSION2, m_clColorButtonComplEmission[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_OK2, m_clColorButtonComplOk[1]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_COMPL_NG2, m_clColorButtonComplNg[1]);
	//�ϴܸ޴�
	DDX_Control(pDX, IDC_BUTTON_MAIN_MAIN, m_clColorButtonMain);
	DDX_Control(pDX, IDC_BUTTON_MAIN_UNIT_CHANGE, m_clColorButtonUnitChange);
	DDX_Control(pDX, IDC_BUTTON_MAIN_MANUAL, m_clColorButtonManual);
	DDX_Control(pDX, IDC_BUTTON_MAIN_TEACHING, m_clColorButtonTeaching);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CCD, m_clColorButtonCcd);
	DDX_Control(pDX, IDC_BUTTON_MAIN_DIO, m_clColorButtonDio);
	DDX_Control(pDX, IDC_BUTTON_MAIN_LIGHT, m_clColorButtonLight);
	DDX_Control(pDX, IDC_BUTTON_MAIN_ALARM, m_clColorButtonAlarm);
    DDX_Control(pDX, IDC_BUTTON_MAIN_CONFIG, m_clColorButtonConfig);
    DDX_Control(pDX, IDC_BUTTON_MAIN_MINIMIZE, m_clColorButtonMinimize);
	DDX_Control(pDX, IDC_BUTTON_MAIN_EXIT, m_clColorButtonExit);
	//���
	DDX_Control(pDX, IDC_BUTTON_MAIN_LAN, m_clButtonExLan);
	DDX_Control(pDX, IDC_BUTTON_INSPECTION_LAN, m_clButtonExInspLan);
	//CCD
	DDX_Control(pDX, IDC_BUTTON_MAIN_LINK1, m_clButtonExLink[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_LINK2, m_clButtonExLink[1]);

	DDX_Control(pDX, IDC_BUTTON_MAIN_CCD1, m_clButtonExCcd[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CCD2, m_clButtonExCcd[1]);

	DDX_Control(pDX, IDC_BUTTON_MAIN_MES1, m_clButtonMes[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_MES2, m_clButtonMes[1]);

	DDX_Control(pDX, IDC_BUTTON_MAIN_DOOR1, m_clButtonExDoor[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_DOOR2, m_clButtonExDoor[1]);
	


	DDX_Control(pDX, IDC_BUTTON_MAIN_CAM_CHANGE1, m_clButtonCAMChange[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CAM_CHANGE2, m_clButtonCAMChange[1]);

	DDX_Control(pDX, IDC_BUTTON_MAIN_CCD_CHANGE1, m_clButtonCCDChange[0]);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CCD_CHANGE2, m_clButtonCCDChange[1]);
	
	

	DDX_Control(pDX, IDC_STATIC_MAIN_A1_INPUT, m_clColorStaticAA_Input[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_A2_INPUT, m_clColorStaticAA_Input[1]);

	DDX_Control(pDX, IDC_STATIC_MAIN_A1_OUTPUT, m_clColorStaticAA_Output[0]);
	DDX_Control(pDX, IDC_STATIC_MAIN_A2_OUTPUT, m_clColorStaticAA_Output[1]);



	//IDC_BUTTON_MAIN_LAN
	//IDC_BUTTON_INSPECTION_LAN

	//DDX_Control(pDX, IDC_STATIC_MSG, m_clColorStaticMsg);
}

BEGIN_MESSAGE_MAP(CAutoInspDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_MAIN_LINK1, &CAutoInspDlg::OnBnClickedButtonMainLink1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_CCD1, &CAutoInspDlg::OnBnClickedButtonMainCcd1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_STARTING_POINT1, &CAutoInspDlg::OnBnClickedButtonMainStartingPoint1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_READY1, &CAutoInspDlg::OnBnClickedButtonMainAutoReady1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_RUN1, &CAutoInspDlg::OnBnClickedButtonMainAutoRun1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_OK1, &CAutoInspDlg::OnBnClickedButtonMainComplOk1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_EMISSION1, &CAutoInspDlg::OnBnClickedButtonMainComplEmission1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_STOP1, &CAutoInspDlg::OnBnClickedButtonMainAutoStop1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_PAUSE1, &CAutoInspDlg::OnBnClickedButtonMainAutoPause1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_NG1, &CAutoInspDlg::OnBnClickedButtonMainComplNg1)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_LINK2, &CAutoInspDlg::OnBnClickedButtonMainLink2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_CCD2, &CAutoInspDlg::OnBnClickedButtonMainCcd2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_STARTING_POINT2, &CAutoInspDlg::OnBnClickedButtonMainStartingPoint2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_READY2, &CAutoInspDlg::OnBnClickedButtonMainAutoReady2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_RUN2, &CAutoInspDlg::OnBnClickedButtonMainAutoRun2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_OK2, &CAutoInspDlg::OnBnClickedButtonMainComplOk2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_EMISSION2, &CAutoInspDlg::OnBnClickedButtonMainComplEmission2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_STOP2, &CAutoInspDlg::OnBnClickedButtonMainAutoStop2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_AUTO_PAUSE2, &CAutoInspDlg::OnBnClickedButtonMainAutoPause2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_COMPL_NG2, &CAutoInspDlg::OnBnClickedButtonMainComplNg2)
    //
	ON_BN_CLICKED(IDC_BUTTON_MAIN_UNIT_CHANGE, &CAutoInspDlg::OnBnClickedButtonMainUnitChange)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_MAIN, &CAutoInspDlg::OnBnClickedButtonMainMain)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_MANUAL, &CAutoInspDlg::OnBnClickedButtonMainManual)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_TEACHING, &CAutoInspDlg::OnBnClickedButtonMainTeaching)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CCD, &CAutoInspDlg::OnBnClickedButtonMainCcd)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_DIO, &CAutoInspDlg::OnBnClickedButtonMainDio)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_LIGHT, &CAutoInspDlg::OnBnClickedButtonMainLight)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_ALARM, &CAutoInspDlg::OnBnClickedButtonMainAlarm)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CONFIG, &CAutoInspDlg::OnBnClickedButtonMainConfig)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_EXIT, &CAutoInspDlg::OnBnClickedButtonMainExit)
    //
    ON_BN_CLICKED(IDC_BUTTON_MAIN_LAN, &CAutoInspDlg::OnBnClickedButtonMainLan)
    ON_STN_CLICKED(IDC_STATIC_MAIN_OUTPUT1, &CAutoInspDlg::OnStnClickedStaticMainOutput1)
    ON_STN_CLICKED(IDC_STATIC_MAIN_OUTPUT2, &CAutoInspDlg::OnStnClickedStaticMainOutput2)
    ON_STN_CLICKED(IDC_STATIC_MAIN_VERSION1, &CAutoInspDlg::OnStnClickedStaticMainVersion1)
    ON_STN_CLICKED(IDC_STATIC_MAIN_VERSION2, &CAutoInspDlg::OnStnClickedStaticMainVersion2)
    ON_STN_CLICKED(IDC_STATIC_MAIN_PIN1, &CAutoInspDlg::OnStnClickedStaticMainPin1)
    ON_STN_CLICKED(IDC_STATIC_MAIN_PIN2, &CAutoInspDlg::OnStnClickedStaticMainPin2)
    ON_STN_CLICKED(IDC_STATIC_MAIN_BCR_VAL1, &CAutoInspDlg::OnStnClickedStaticMainBcrVal1)
    ON_STN_CLICKED(IDC_STATIC_MAIN_BCR_VAL2, &CAutoInspDlg::OnStnClickedStaticMainBcrVal2)
    ON_BN_CLICKED(IDC_BUTTON_MAIN_MINIMIZE, &CAutoInspDlg::OnBnClickedButtonMainMinimize)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_CHAGE_MODE1, &CAutoInspDlg::OnBnClickedButtonMasterChageMode1)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_CHAGE_MODE2, &CAutoInspDlg::OnBnClickedButtonMasterChageMode2)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CAM_CHANGE1, &CAutoInspDlg::OnBnClickedButtonMainCamChange1)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CAM_CHANGE2, &CAutoInspDlg::OnBnClickedButtonMainCamChange2)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CCD_CHANGE1, &CAutoInspDlg::OnBnClickedButtonMainCcdChange1)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CCD_CHANGE2, &CAutoInspDlg::OnBnClickedButtonMainCcdChange2)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_MES1, &CAutoInspDlg::OnBnClickedButtonMainMes1)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_MES2, &CAutoInspDlg::OnBnClickedButtonMainMes2)
	ON_BN_CLICKED(IDC_BUTTON_EPOXY_COMPLETE1, &CAutoInspDlg::OnBnClickedButtonEpoxyComplete1)
	ON_BN_CLICKED(IDC_BUTTON_EPOXY_COMPLETE2, &CAutoInspDlg::OnBnClickedButtonEpoxyComplete2)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LOADING1, &CAutoInspDlg::OnBnClickedButtonLensLoading1)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LOADING2, &CAutoInspDlg::OnBnClickedButtonLensLoading2)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SELCT, &CAutoInspDlg::OnBnClickedButtonModelSelct)
	ON_STN_CLICKED(IDC_STATIC_MAIN_CURR_MODE1, &CAutoInspDlg::OnStnClickedStaticMainCurrMode1)
	ON_STN_CLICKED(IDC_STATIC_MAIN_OUTPUT_VAL1, &CAutoInspDlg::OnStnClickedStaticMainOutputVal1)
	ON_STN_CLICKED(IDC_STATIC_MAIN_PIN_VAL1, &CAutoInspDlg::OnStnClickedStaticMainPinVal1)
	ON_STN_CLICKED(IDC_STATIC_MAIN_CURR_MODE3, &CAutoInspDlg::OnStnClickedStaticMainCurrMode3)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_DOOR1, &CAutoInspDlg::OnBnClickedButtonMainDoor1)
END_MESSAGE_MAP()


// CAutoInspDlg �޽��� ó����

BOOL CAutoInspDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
#ifdef _DEBUG
	// �޸� ���� ���� ����
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// ���α׷��� ������ �κ� ����
	//_CrtSetBreakAlloc(197);
	//_CrtMemDumpAllObjectsSince(0);
#endif
	this->InitializeService();


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CAutoInspDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

//-----------------------------------------------------------------------------
//
// Timer
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case WM_UI_CM_TIMER:
		//this->InformationState();
		break;
	
		
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CAutoInspDlg::InformationState()
{
	int nState[MAX_UNIT_COUNT];
	//
	for (int i = 0; i < MAX_UNIT_COUNT; i++)//for (int i = 0; i < 2; i++)
	{
		if (g_clTaskWork[i].m_bInputSignal == true)
		{
			m_clColorStaticAA_Input[i].SetBkColor(RGB_COLOR_GREEN);
			m_clColorStaticAA_Output[i].SetBkColor(RGB_COLOR_RED);
			nState[i] = 1;
		}
		else if(g_clTaskWork[i].m_bOutputSignal == true)
		{
			m_clColorStaticAA_Input[i].SetBkColor(RGB_COLOR_RED);
			m_clColorStaticAA_Output[i].SetBkColor(RGB_COLOR_GREEN);
			nState[i] = 2;
		}
		else
		{
			m_clColorStaticAA_Input[i].SetBkColor(RGB_COLOR_RED);
			m_clColorStaticAA_Output[i].SetBkColor(RGB_COLOR_RED);
			nState[i] = 3;
		}
		if (m_nSignal[i] != nState[i])
		{
			m_clColorStaticAA_Input[i].Invalidate();
			m_clColorStaticAA_Output[i].Invalidate();
			m_nSignal[i] = nState[i];
		}

		//if (m_nModelType != g_clSysData.m_nModelIndex)
		//{
		//	
		//	m_nModelType = g_clSysData.m_nModelIndex;
		//}

	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAutoInspDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(rect);
        dc.FillSolidRect(rect, MAIN_BG_COLOR);
        
        this->_DrawBarGraph(0);
        this->_DrawBarGraph(1);
        
        CDialogEx::OnPaint();

	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CAutoInspDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//-----------------------------------------------------------------------------
//
//	���� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitializeService()
{
	
    TCHAR szPath[SIZE_OF_1K];
    TCHAR szLog[SIZE_OF_1K];
    CString sMsg = _T("");
    int i;

    // ���� ���̾�α� ������
    g_pCarAABonderDlg = this;
	m_clLogThread.StartThread();
	// MODEL TYPE LOAD
	ModelList.ModelListLoad();
	g_clModelType.mTLoad();


    // �ý��� ������ �ε�
	g_clSysData.sDLoad();
	g_clSysData.commonDataLoad();
	//g_clSysData.OcOffsetLoad();
	//Spec Load
	MesDataLoad();

	
    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
		// �� ������ �ε�
		g_clModelData[i].SetUnit(i);
		g_clModelData[i].LoadTeachData(g_clSysData.m_szModelName);
		g_clModelData[i].Load(g_clSysData.m_szModelName); 
		//g_clModelData[i].EpoxyDataLoad(g_clSysData.m_szModelName);

        g_clLaonGrabberWrapper[i].SetUnit(i);
        g_clLaonGrabberWrapper[i].BoardtInitialize();
		//
        g_clLaonGrabberWrapper[i].UiconfigLoad(INI_RAW_IMAGE);		//pg start
        g_clLaonGrabberWrapper[i].SelectSensor();

       

        g_clMarkData[i].SetUnit(i);
        g_clMarkData[i].LoadData(g_clSysData.m_szModelName);


		g_clModelData[i].LotDataLoad();
		
        // TASK ������ �ε�
        g_clTaskWork[i].SetUnit(i);
		g_clTaskWork[i].LoadData();
		g_clTaskWork[i].PinLoadData();
		
		
        m_clActiveAlignThread[i].SetUnit(i);

		//eepromData[i].SetUnit(i);
        m_clCcdGrabThread[i].SetUnit(i);
		m_clVisionGrabThread[i].SetUnit(i);
        m_clCcdCvtColorThread[i].SetUnit(i);

        g_clPriInsp[i].SetUnit(i);

        g_clMandoSfrSpec[i].SetUnit(i);
        g_clMandoSfrSpec[i].Load();


		//My ������ ����
		m_clCustomThread[i].SetUnit(i);

    }

    // MIL ���̺귯�� �ʱ�ȭ
    this->InitMilLib();
    // ���� ���̺귯�� �ʱ�ȭ
    this->InitMotorLib();
    // DIO ���̺귯�� �ʱ�ȭ
    this->InitDioLib();
    // �׷��� ���̺귯�� �ʱ�ȭ
    this->InitGrabberLib();

    // Socket �ʱ�ȭ
    //this->InitSocket();

    // ���� ���̾�α� ����
    this->CreateSubDlg();

    // ��Ʈ�� ��ġ ����
    this->SetCtrlPos();

    // ��Ʈ�� �ʱ�ȭ
    this->InitCtrl();

	

	//m_clColorButtonModelSelect.SetWindowTextA(_T("MODEL SELECT"));
	//m_clColorButtonModelSelect.state = 0;


#ifndef GRAPH_NO_USE
    // ������ �׷��� ��Ʈ�� ����
    this->MakeGraphWnd();
#endif
	bool bSerialRtn = false;
	CString sCommPort = _T("");
	if (m_clSerialThread.OpenBcrSerial(0, g_clSysData.m_nBcrPort[0], 9600) == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL] BCR COM(%d) ���� ����"), g_clSysData.m_nBcrPort[0]);
		AddLog(szLog, 0, 0);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]BCR COM(%d) ���� ����"), g_clSysData.m_nBcrPort[0]);
		AddLog(szLog, 0, 0);
	}
#ifdef ON_LINE_LASER
	if (m_clSerialThread.OpenKeyenceSerial(0, g_clSysData.m_nLaserPort[0], 115200) == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL] LASER COM(%d) ���� ����"), g_clSysData.m_nLaserPort[0]);
		AddLog(szLog, 0, 0);
	}
	else
	{
		if (m_clSerialThread.SendKeyLockSet(0, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL] �������������� COM(%d) ���� ����"), g_clSysData.m_nLaserPort[i]);
			AddLog(szLog, 0, i);

		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[LASER] �������������� COM(%d) ���� ����"), g_clSysData.m_nLaserPort[i]);
			AddLog(szLog, 0, i);
		}


	}

#endif
#ifdef ON_LINE_LIGHT
	//------------------------------------------------------------------------------------------
	//
	//
	//
	//ALIGN, OC Light ��� ����
	//
	//------------------------------------------------------------------------------------------
	BackDuLightControl[UNIT_AA1].SetReceiveProcPtr(this);
	sCommPort.Format("COM%d", g_clSysData.m_nAlign_Oc_Port[UNIT_AA1]);
	//bSerialRtn = AlignLightControl[UNIT_AA1].Connect_BackDu_Device(sCommPort, 0);


	bSerialRtn = BackDuLightControl[UNIT_AA1].Connect_Device(sCommPort, UNIT_AA1, 19200);
	if (bSerialRtn == true)
	{
		Sleep(200);
		int lightVal = g_clModelData[UNIT_AA1].m_nLight[LIGHT_DATA_ALIGN_SENSOR];
		BackDuLightControl[UNIT_AA1].ctrlLedVolume(LIGHT_BD_ALIGN_CH1, lightVal);
		Sleep(200);
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
		lightVal = g_clModelData[UNIT_AA1].m_nLight[LIGHT_DATA_OC_WHITE1];
		BackDuLightControl[UNIT_AA1].ctrlLedVolume(LIGHT_BD_OC_CH1, lightVal);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]ALIGN , OC ���� �Ϸ� COM%d"), g_clSysData.m_nAlign_Oc_Port[UNIT_AA1]);
#else
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL] OC ���� �Ϸ� COM%d"), g_clSysData.m_nAlign_Oc_Port[UNIT_AA1]);
#endif
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]ALIGN , OC ���� ���� COM%d"), g_clSysData.m_nAlign_Oc_Port[UNIT_AA1]);
	}
	AddLog(szLog, 0, 0);

#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	//------------------------------------------------------------------------------------------
	//
	//
	//
	//Top Chart Light
	//
	//------------------------------------------------------------------------------------------
	TopChartControl[UNIT_AA1].SetReceiveProcPtr(this);
	sCommPort.Format("COM%d", g_clSysData.m_nTopLightPort[UNIT_AA1]);
	bSerialRtn = TopChartControl[UNIT_AA1].Connect_Device(sCommPort, UNIT_AA1, 19200);
	if (bSerialRtn == true)
	{
		Sleep(200);
		TopChartControl[UNIT_AA1].SendDomeChartLightONOFF(true);
		Sleep(300);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL] Chart ���� �Ϸ� COM%d"), g_clSysData.m_nTopLightPort[UNIT_AA1]);
		TopChartControl[UNIT_AA1].dpctrlLedVolume(LIGHT_CHART_CH_1 , g_clModelData[UNIT_AA1].m_nLight[LIGHT_DATA_TOP_CHART_1]);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]Chart ���� ���� COM%d"), g_clSysData.m_nTopLightPort[UNIT_AA1]);
	}
	AddLog(szLog, 0, 0);
	//------------------------------------------------------------------------------------------
	//
	//
	//
	//LEFT Chart Light
	//
	//------------------------------------------------------------------------------------------
	LightLeftChartControl[UNIT_AA1].SetReceiveProcPtr(this);
	sCommPort.Format("COM%d", g_clSysData.m_nLeftLightPort[UNIT_AA1]);
	bSerialRtn = LightLeftChartControl[UNIT_AA1].Connect_Device(sCommPort, UNIT_AA1, 19200);
	if (bSerialRtn == true)
	{
		Sleep(200);
		LightLeftChartControl[UNIT_AA1].SendDomeChartLightONOFF(true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]Left Chart ���� �Ϸ� COM%d"), g_clSysData.m_nLeftLightPort[UNIT_AA1]);
		Sleep(500);
		LightLeftChartControl[UNIT_AA1].dpctrlLedVolume(LIGHT_CHART_CH_1, g_clModelData[UNIT_AA1].m_nLight[LIGHT_DATA_SIDE_CHART_LEFT]);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]Left Chart ���� ���� COM%d"), g_clSysData.m_nLeftLightPort[UNIT_AA1]);
	}
	AddLog(szLog, 0, 0);

	//------------------------------------------------------------------------------------------
	//
	//
	//
	//RIGHT Chart Light
	//
	//------------------------------------------------------------------------------------------
	LightRightChartControl[UNIT_AA1].SetReceiveProcPtr(this);
	sCommPort.Format("COM%d", g_clSysData.m_nRightLightPort[UNIT_AA1]);
	bSerialRtn = LightRightChartControl[UNIT_AA1].Connect_Device(sCommPort, UNIT_AA1, 19200);
	if (bSerialRtn == true)
	{
		Sleep(200);
		LightRightChartControl[UNIT_AA1].SendDomeChartLightONOFF(true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]Right Chart ���� �Ϸ� COM%d"), g_clSysData.m_nRightLightPort[UNIT_AA1]);
		Sleep(500);
		LightRightChartControl[UNIT_AA1].dpctrlLedVolume(LIGHT_CHART_CH_1, g_clModelData[UNIT_AA1].m_nLight[LIGHT_DATA_SIDE_CHART_RIGHT]);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SERIAL]Right Chart ���� ���� COM%d"), g_clSysData.m_nRightLightPort[UNIT_AA1]);
	}
	AddLog(szLog, 0, 0);
#else
	
#endif
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	BYTE iSlaveNo = 0; // Slave No (0 ~ 15)
	int nRtn;
	BYTE nType;
	char lpBuff[256];
	int nBuffSize = 256;
	unsigned long uInput;
	unsigned long uLatch;
	int m_nPortNo = g_clSysData.m_nEzi_Port[0];
	if (!FAS_Connect(m_nPortNo, 115200))
	{
		AddLog(_T("Ezi connection failed."), 0, 0);
	}
	else
	{
		if (FAS_IsSlaveExist(m_nPortNo, iSlaveNo) == FALSE)
		{
			AddLog(_T("Ezi get Slave failed."), 0, 0);
		}
		nRtn = FAS_GetSlaveInfo(m_nPortNo, iSlaveNo, &nType, lpBuff, nBuffSize);
		if (nRtn != FMM_OK)
		{
			// ����� ���������� ������� �ʾҽ��ϴ�.
			// ReturnCodes_Define.h �� �����Ͻʽÿ�.
		}
		//logStr.Format("EZIO Port: COM%d", m_nPortNo);// , iSlaveNo);
		_stprintf_s(szLog, SIZE_OF_1K, _T("EZIO Port: COM%d"), m_nPortNo);
		//putListLog(logStr);
		AddLog(szLog, 0, 0);
		//logStr.Format("Type : %d", nType);
		_stprintf_s(szLog, SIZE_OF_1K, _T("Type : %d"), nType);
		//putListLog(logStr);
		AddLog(szLog, 0, 0);
		//logStr.Format("Version : %s", lpBuff);
		_stprintf_s(szLog, SIZE_OF_1K, _T("Version : %s"), lpBuff);
		AddLog(szLog, 0, 0);
		Sleep(500);
		//nRtn = FAS_GetInput(m_nPortNo, iSlaveNo, &uInput, &uLatch);
		//FAS_Close(ConfigData.iCommPort[COMM_SENSOR_LIGHT]);
	}
#endif
#ifdef ON_LINE_SOCKET
    //m_bConnectMain = this->ConnectAAMain();
    //if (m_bConnectMain == true)
    //{
    //    AddLog(_T("[SOCKET] MAIN PC CONNECT OK"), 0, 0);

    //}
    //else
    //{
    //    //AddLog(_T("[FAIL] MAIN PC CONNECT FAIL"), 0, 999);
    //    AddLog(_T("[SOCKET] MAIN PC CONNECT FAIL"), 0, 0);
    //}
#endif
	m_clSerialThread.StartThread();	//bcr, laser

    // �α� ������ ����
    //m_clLogThread.StartThread();
    //m_clLogThread.CreateLogFolder();
    Sleep(50);
    // �ð� ������ ����
    m_clClockThread.StartThread();
    Sleep(50);
    // ����� ������ ����
    m_clMonitorThread.StartThread();
    Sleep(50);
	// ���ϰ��� ������ ����
	m_clFileThread.StartThread();
	Sleep(50);
	
	//
    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {

        this->ShowOkNgState(i , g_clTaskWork[i].m_nEmission);

        // ���� ���� ǥ��
        this->ShowOutputCount(i);

        // ������ ��뷮 ǥ��
        this->ShowPinCount(i);

		CAMChangeHandler(i, VIDEO_CCD);

		////this->ShowCurrentMode(i);

		this->ShowBarcode(i);
		g_clModelData[i].PatternLoad(g_clSysData.m_szModelName);        //���� �̹��� �ε�
		g_clModelData[i].AcmisDataLoad(g_clSysData.m_szModelName);
		AddLog(_T("[AUTO] PG START"), 0, i);



    }

	
	//g_clLaonGrabberWrapper[0].PatternImageLoad();

	m_nCurrentDlg = DLG_MANUAL;
	this->ShowDialog(m_nCurrentDlg);
	

	//EEPROM_Temp_DumpLoad(0, g_clMesCommunication[0].mMesGetEEpromData);
	g_clDioControl.ReadDOut(1);

	g_clDioControl.ReadByteOut(1, 0);
	g_clDioControl.ReadByteOut(1, 1);
	g_clDioControl.ReadByteOut(1, 2);
	g_clDioControl.ReadByteOut(1, 3);
	
	
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	int offset = 8;		// DIO_OUT_PCB_VACUUM_ON
	int nrtn = g_clDioControl.ReadBitIn(1, offset);	//���� , Ż�� OUT ��ȣ Ȯ��

	if (nrtn == 1)	//DIO_OUT_PCB_VACUUM_ON : ON ����
	{
		g_clDioControl.Stop_Button_LedOn(UNIT_AA1, true);
	}
	else if (nrtn == 0)	//DIO_OUT_PCB_VACUUM : OFF ����
	{
		g_clDioControl.Stop_Button_LedOn(UNIT_AA1, false);
	}

#else
	int offset = 6;		// EZDIO_OUT_VACUUM_ON
	int nrtn = g_clDioControl.eZiReadOut(0, offset);	//���� , Ż�� OUT ��ȣ Ȯ��
	if (nrtn == 1)
	{
		g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, true);
	}
	else if (nrtn == 0)
	{
		g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);
	}
#endif
	//SetTimer(WM_UI_CM_TIMER, 200, NULL);
    //AddLog(_T("[INFO] ���� �ʱ�ȭ �Ϸ�"), 0, 999);

	sMsg.Empty();
}


//-----------------------------------------------------------------------------
//
//	���� ���̾�α� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::CreateSubDlg()
{
    // �˾� ���̾�α�
    int i;

    for (i = 0; i < MAX_POPUP_COUNT; i++)
    {
        g_pMessagePopupDlg[i] = new CMessagePopupDlg;
        if (g_pMessagePopupDlg[i] != NULL)
        {
            g_pMessagePopupDlg[i]->Create(IDD_DIALOG_MESSAGE_POPUP, this);
            g_pMessagePopupDlg[i]->ShowWindow(SW_HIDE);
        }
    }
	if (InterLockDlg == NULL) {
		InterLockDlg = new CInterLockDlg;
		InterLockDlg->Create(IDD_DIALOG_INTERLOCK);
		InterLockDlg->ShowWindow(SW_HIDE);
		InterLockDlg->Invalidate();
	}


    // ���� ���̾�α�
    m_clManualDlg.Create(IDD_DIALOG_MANUAL, this);
    m_clManualDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clManualDlg.ShowWindow(SW_HIDE);

    // ƼĪ ���̾�α�
    m_clTeachingDlg.Create(IDD_DIALOG_TEACHING, this);
    m_clTeachingDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clTeachingDlg.ShowWindow(SW_HIDE);

    // CCD ���̾�α�
    m_clCcdDlg.Create(IDD_DIALOG_CCD, this);
    m_clCcdDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clCcdDlg.ShowWindow(SW_HIDE);

    // DIO ���̾�α�
    m_clDioDlg.Create(IDD_DIALOG_DIO, this);
    m_clDioDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clDioDlg.ShowWindow(SW_HIDE);

    // LIGHT ���̾�α�
    m_clLightDlg.Create(IDD_DIALOG_LIGHT, this);
    m_clLightDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clLightDlg.ShowWindow(SW_HIDE);

    // ALARM ���̾�α�
    m_clAlarmDlg.Create(IDD_DIALOG_ALARM, this);
    m_clAlarmDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clAlarmDlg.ShowWindow(SW_HIDE);

    // ���� ���̾�α�
    m_clConfigDlg.Create(IDD_DIALOG_CONFIG, this);
    m_clConfigDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clConfigDlg.ShowWindow(SW_HIDE);

	// MODEL ���� ���̾�α�
	//m_clModelSelectDlg.Create(IDD_DIALOG_MODEL_SELECT, this);
	//m_clModelSelectDlg.ShowWindow(SW_HIDE);
	//m_clModelSelectDlg.SetUnit(0);

    /*for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
        m_clProductNGDlg[i].Create(IDD_DIALOG_NG, this);
        m_clProductNGDlg[i].ShowWindow(SW_HIDE);
        m_clProductNGDlg[i].SetUnit(i);
    }*/

    // UPDATE HISTORY ���̾�α�
   /* m_clUpdateHistoryDlg.Create(IDD_DIALOG_UPDATE_HISTORY, this);
    m_clUpdateHistoryDlg.SetWindowPos(NULL, 960, 2, 0, 0, SWP_NOSIZE);
    m_clUpdateHistoryDlg.ShowWindow(SW_HIDE);*/

	
}
//-----------------------------------------------------------------------------
//
//	��Ʈ�� ��ġ ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::SetCtrlPos()
{
    WINDOWPLACEMENT wndpl;
    CPoint clPos;
    CString szKey = _T("");
    int i;

    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
        m_clCtrlPos[i].InitHashTable(150);

        /*GetDlgItem(IDC_STATIC_MAIN_LOGO1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;*/

        szKey.Format(_T("IDC_STATIC_MAIN_LOGO%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        /*GetDlgItem(IDC_STATIC_MAIN_TITLE1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;*/

        szKey.Format(_T("IDC_STATIC_MAIN_TITLE%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_VERSION1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_VERSION%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        /*GetDlgItem(IDC_STATIC_MAIN_INFO1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;*/

        szKey.Format(_T("IDC_STATIC_MAIN_INFO%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_MODEL_NO1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_MODEL_NO%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_LOT_ID1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_LOT_ID%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_CCD_ID1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_CCD_ID%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_LINK1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_LINK%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_BUTTON_MAIN_CCD1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MAIN_CCD%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_BUTTON_MAIN_DOOR1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MAIN_DOOR%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		
		GetDlgItem(IDC_BUTTON_MAIN_MES1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MAIN_MES%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_STATIC_MAIN_CCD1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_STATIC_MAIN_CCD%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_STATIC_MAIN_CAM1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_STATIC_MAIN_CAM%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		//
        GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_CURR_MODE%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MAIN_CCD_CHANGE%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		
		GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MAIN_CAM_CHANGE%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);
		
#ifndef GRAPH_NO_USE
        GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_1ST_AA%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_2ND_AA%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);
#endif
        GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_BAR_GRAPH%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_BCR1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_BCR%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_BCR_VAL%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

		GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_STATIC_MAIN_MODEL_TYPE%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);
		
		//master
		GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_MASTER_CHAGE_MODE%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		//Epoxy Complete
		GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_EPOXY_COMPLETE%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);
		
		//LENS LOADING �Ϸ�
		GetDlgItem(IDC_BUTTON_LENS_LOADING1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_BUTTON_LENS_LOADING%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

		//STAIN
		GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1 + i)->GetWindowPlacement(&wndpl);
		clPos.x = wndpl.rcNormalPosition.left;
		clPos.y = wndpl.rcNormalPosition.top;

		szKey.Format(_T("IDC_STATIC_MAIN_STAIN_VIEW_AA%d"), i + 1);
		m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_OUTPUT1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT_VAL%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_PIN1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_PIN%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_PIN_VAL%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_FPS1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_FPS%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_STATIC_MAIN_FPS_VAL%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_STARTING_POINT%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_READY%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_RUN%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_OK%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_EMISSION%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_STOP%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_PAUSE%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_NG%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        /*GetDlgItem(IDC_STATIC_MAIN_LOG1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;*/

        szKey.Format(_T("IDC_STATIC_MAIN_LOG%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);

        GetDlgItem(IDC_LIST_MAIN_LOG1 + i)->GetWindowPlacement(&wndpl);
        clPos.x = wndpl.rcNormalPosition.left;
        clPos.y = wndpl.rcNormalPosition.top;

        szKey.Format(_T("IDC_LIST_MAIN_LOG%d"), i + 1);
        m_clCtrlPos[i].SetAt(szKey, clPos);
    }
}


//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitCtrl()
{
    WINDOWPLACEMENT wndpl;
    int i = 0;
	CString strtemp;
	int CCDPosX1[2];
	int BcrPosX1[2];
	RECT vvv;
    // ��Ʈ ����
	m_clFontBcrSmall.CreateFont(11, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
    m_clFontSmall.CreateFont(13, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
    m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
    m_clFontBig.CreateFont(100, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

    m_clListFontSmall.CreateFont(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
    for (i = 0; i < 2; i++)
    {
        /*m_clColorStaticTitle[i].SetBkColor(RGB_CTRL_BG);
        m_clColorStaticTitle[i].SetTextColor(RGB_COLOR_WHITE);
        m_clColorStaticTitle[i].SetFont(&m_clFontBig);
        m_clColorStaticTitle[i].SetWindowText(DEF_TITLE);*/
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//title color
		m_clColorStaticVersion[i].SetBkColor(RGB(20, 20, 145));
#else
		m_clColorStaticVersion[i].SetBkColor(RGB_CTRL_BG);
#endif
        
        m_clColorStaticVersion[i].SetTextColor(RGB_COLOR_WHITE);
        m_clColorStaticVersion[i].SetFontBold(TRUE);
        m_clColorStaticVersion[i].SetFont(&m_clFontMid);
        m_clColorStaticVersion[i].SetBorder(FALSE);
        m_clColorStaticVersion[i].SetFontSize(20);

		//m_clColorStaticVersion[i].SetWindowText(VER_STR);

        m_clColorStaticModelNo[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticModelNo[i].SetFont(&m_clFontSmall);
        m_clColorStaticModelNo[i].SetWindowText(_T("TEST"));

        m_clColorStaticLotID[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticLotID[i].SetFont(&m_clFontSmall);
        m_clColorStaticLotID[i].SetWindowText(_T(""));

        m_clColorStaticCcdID[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticCcdID[i].SetFont(&m_clFontSmall);
        m_clColorStaticCcdID[i].SetWindowText(_T(""));

        //m_clColorStaticCurrMode[i].SetBkColor(RGB_COLOR_WHITE);
        //m_clColorStaticCurrMode[i].SetFont(&m_clFontMid);
        m_clColorStaticCurrMode[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_GREEN);
		m_clColorStaticCurrAlignMode[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_GREEN);
        //m_clColorStaticBcr[i].SetBkColor(RGB_CTRL_BG);
        //m_clColorStaticBcr[i].SetTextColor(RGB_COLOR_WHITE);
        //m_clColorStaticBcr[i].SetFont(&m_clFontSmall);
        m_clColorStaticBcr[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_WHITE);
		m_clColorStaticBcrVal[i].SetBkColor(RGB_COLOR_WHITE);
		m_clColorStaticBcrVal[i].SetFont(&m_clFontSmall);
		GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1 + i)->GetWindowRect(&vvv);
		BcrPosX1[i] = vvv.left;

        //m_clColorStaticOutput[i].SetBkColor(RGB_CTRL_BG);
        //m_clColorStaticOutput[i].SetTextColor(RGB_COLOR_WHITE);
        //m_clColorStaticOutput[i].SetFont(&m_clFontSmall);
        m_clColorStaticOutput[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_WHITE);

        m_clColorStaticOutputVal[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticOutputVal[i].SetFont(&m_clFontSmall);

        //m_clColorStaticPin[i].SetBkColor(RGB_CTRL_BG);
        //m_clColorStaticPin[i].SetTextColor(RGB_COLOR_WHITE);
        //m_clColorStaticPin[i].SetFont(&m_clFontSmall);
        m_clColorStaticPin[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_WHITE);

        m_clColorStaticPinVal[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticPinVal[i].SetFont(&m_clFontSmall);

        //m_clColorStaticFps[i].SetBkColor(RGB_CTRL_BG);
        //m_clColorStaticFps[i].SetTextColor(RGB_COLOR_WHITE);
        //m_clColorStaticFps[i].SetFont(&m_clFontSmall);
        m_clColorStaticFps[i].SetFontBold(TRUE).SetBkColor(RGB_COLOR_WHITE);

        m_clColorStaticFpsVal[i].SetBkColor(RGB_COLOR_WHITE);
        m_clColorStaticFpsVal[i].SetFont(&m_clFontSmall);

		m_clColorStaticAA_Input[i].SetBkColor(RGB_COLOR_WHITE);
		m_clColorStaticAA_Input[i].SetFont(&m_clFontSmall);

		m_clColorStaticAA_Output[i].SetBkColor(RGB_COLOR_WHITE);
		m_clColorStaticAA_Output[i].SetFont(&m_clFontSmall);
		
		GetDlgItem(IDC_BUTTON_MAIN_CCD1 + i)->GetWindowRect(&vvv);
		CCDPosX1[i] = vvv.left;
        GetDlgItem(IDC_STATIC_MAIN_CCD1 + i)->GetWindowPlacement(&wndpl);
		m_clVisionStaticCcd[i].MoveWindow(wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, CCD1_DISP_SIZE_X, CCD1_DISP_SIZE_Y);
		//
		//IDC_STATIC_MAIN_CAM
		GetDlgItem(IDC_STATIC_MAIN_CAM1)->GetWindowRect(&vvv);
		GetDlgItem(IDC_STATIC_MAIN_CAM2)->GetWindowRect(&vvv);
		GetDlgItem(IDC_STATIC_MAIN_CAM1 + i)->GetWindowPlacement(&wndpl);

		m_clVisionStaticCam[i].MoveWindow(wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
		//
        m_clColorButtonAutoStop[i].state = 1;

        //m_clButtonExLink[i].SetImage(IDB_BITMAP_MAIN_LINK, 80);
#ifdef ON_LINE_GRABBER
        m_clButtonExLink[i].state = 1;
#else
        m_clButtonExLink[i].state = 0;
#endif

        // m_clButtonExCcd[i].SetImage(IDB_BITMAP_MAIN_CCD, 80);
		m_clButtonExCcd[i].state = 0;
		m_clButtonExCcd[i].Invalidate();

		m_clButtonExDoor[i].state = 0;
		m_clButtonExDoor[i].Invalidate();
		
    }
	TCHAR szData[SIZE_OF_100BYTE];

#if  (RIVIAN___MODE__CH == ON_OQA_MODE)			//naming
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("OQA"));
#else
#ifdef KUMI_TEST_MODE
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("TEST"));
#else
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("EOL"));
#endif
#endif

//
//
//
//#if (____MACHINE_NAME ==  MODEL_FOV_80)
//	strtemp.Format("Rivian %s 3M %s", szData, VER_STR);
//#elif (____MACHINE_NAME ==  MODEL_FOV_120)
//	strtemp.Format("Rivian %s 8M %s", szData, VER_STR);
//#else 
//	strtemp.Format("Rivian %s MINI %s", szData, VER_STR);
//#endif


#if (____MACHINE_NAME == MODEL_OHC100)
	strtemp.Format("SHM OHC 100 %s %s", szData, VER_STR);
	m_clColorStaticVersion[0].SetWindowText(strtemp);
#elif (____MACHINE_NAME == MODEL_IP150)		//MODEL_FOV_IP)
	strtemp.Format("SHM IP 150 %s %s", szData, VER_STR);
	m_clColorStaticVersion[0].SetWindowText(strtemp);
#endif



	strtemp.Format("%s", VER_STR);
	m_clColorStaticVersion[1].SetWindowText(strtemp);


	
	

	
	m_clColorStaticVersion[0].GetWindowPlacement(&wndpl);
	m_clColorStaticVersion[0].MoveWindow(wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, wndpl.rcNormalPosition.left + BcrPosX1[0], wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top);


	GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1)->ShowWindow(SW_HIDE);
	m_clColorButtonLensLoading[0].ShowWindow(SW_HIDE);
	g_clModelData[0].m_nCOMPLETE_MODE = 1;


	m_clColorStaticVersion[0].Invalidate();
	m_clColorStaticVersion[1].Invalidate();
    //m_clButtonExLan.SetImage(IDB_BITMAP_MAIN_LAN, 50);
    m_clButtonExLan.state = 0;

    //m_clButtonExInspLan.SetImage(IDB_BITMAP_MAIN_INSP_LAN, 50);
    m_clButtonExInspLan.state = 0;

    m_clColorStaticCurrentDate.SetBkColor(RGB_CTRL_BG);
    m_clColorStaticCurrentDate.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticCurrentDate.SetFont(&m_clFontMid);

    m_clColorStaticCurrentTime.SetBkColor(RGB_CTRL_BG);
    m_clColorStaticCurrentTime.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticCurrentTime.SetFont(&m_clFontBig);
    m_clColorStaticCurrentTime.SetFontSize(16);
    m_clColorStaticCurrentTime.SetFontBold(TRUE);


    m_clListLog[0].SetFont(&m_clListFontSmall);
    m_clListLog[1].SetFont(&m_clListFontSmall);


    m_clColorButtonUnitChange.state = 200;
    m_clColorButtonMain.state = 1;
    m_clColorButtonManual.state = 200;
    m_clColorButtonTeaching.state = 200;
    m_clColorButtonCcd.state = 200;
    m_clColorButtonDio.state = 200;
    m_clColorButtonLight.state = 200;
    m_clColorButtonAlarm.state = 200;
    m_clColorButtonConfig.state = 200;
    m_clColorButtonMinimize.state = 200;
    m_clColorButtonExit.state = 200;
}

//-----------------------------------------------------------------------------
//
//	�׷��� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::MakeGraphWnd()
{
    strGraph2D_Y_Axis_Info stAxisYInfo;	//! Y��
    strGraph2DResourceInfo stResourceInfo;	//! X��	
    CRect rect;
    int i, j, k;

	for (i = 0; i < 2; i++)//
    {
        for (j = 0; j < MAX_GRAPH_COUNT - 1; j++)
        {
            m_pGraphWnd[i][j] = new CGraph2DWnd;
            if (m_pGraphWnd[i][j] != NULL)
            {
                m_pGraphWnd[i][j]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, 0x0000, NULL);

                // ���� ���ҽ��� ������ ������ ��.
                m_pGraphWnd[i][j]->MakeResourcePtr();

                // 5���� �׷����� �׸� �� �ְ� �Ѵ�.
                m_pGraphWnd[i][j]->MakePlotBuffer(MAX_GRAPH_OVERLAP_COUNT);

                // �׷��� ��Ʈ���� INDEX ����
                m_pGraphWnd[i][j]->SetPlotIndex(j);

                // �������� �׷����� Y���� 1����.
                m_pGraphWnd[i][j]->m_bMultiPlotOneAxis = true;

                m_pGraphWnd[i][j]->m_bCursorEventMsg = true;
                m_pGraphWnd[i][j]->m_bRegionCursorEventMsg = false;
                m_pGraphWnd[i][j]->m_bRegionCursorRMouseDownChk = false;
                m_pGraphWnd[i][j]->m_bXAutoRangeRegionCursor = false;
                m_pGraphWnd[i][j]->m_bYAutoRangeRegionCursor = false;

                //! �߰� �� Ŀ�� �� ǥ�� ��� ����
                m_pGraphWnd[i][j]->m_eCursorLineMode = E_GRAPH2D_CUROSR_LINE_X;

                m_pGraphWnd[i][j]->m_eCursorAllSearchMode = E_GRAPH2D_CURSOR_ALL_SEARCH_NONE;
                m_pGraphWnd[i][j]->m_bXValueLineRMouseDownChk = false;
                m_pGraphWnd[i][j]->m_bChildCursorRMouseDownChk = false;
                m_pGraphWnd[i][j]->m_bAutoDrawCompleteMsg = false;
                m_pGraphWnd[i][j]->m_bAxisYEventMsg = false;
                m_pGraphWnd[i][j]->m_bAuxCursorRMouseDownChk = false;

                for (k = 0; k < MAX_GRAPH_OVERLAP_COUNT; k++)
                {
                    m_pGraphWnd[i][j]->GetResourceYInfo(stAxisYInfo, k);
                    stAxisYInfo.bShowGraph = false;

                    switch (k)
                    {
                    case 0:		stAxisYInfo.clrData = RGB(0, 0, 255);		break;
                    case 1:		stAxisYInfo.clrData = RGB(255, 0, 0);		break;
                    case 2:		stAxisYInfo.clrData = RGB(0, 255, 0);		break;
                    case 3:		stAxisYInfo.clrData = RGB(63, 0, 153);		break;
                    case 4:		stAxisYInfo.clrData = RGB(0, 216, 255);		break;
                    }

                    //! �׷������� Y�� Range�� �׷��� �����Ͱ� Ƣ�� �ʴ� �ѿ��� '0 ~ 1'�� �����Ѵ�. 
                    stAxisYInfo.strAxisInfo.eAutoRangeReservedMax = E_GRAPH2D_AUTO_RANGE_RESERVED_CONSIDER;
                    stAxisYInfo.strAxisInfo.eAutoRangeReservedMin = E_GRAPH2D_AUTO_RANGE_RESERVED_CONSIDER;
                    stAxisYInfo.strAxisInfo.eMarginType = E_GRAPH2D_RANGE_MARGIN_NONE;
                    stAxisYInfo.strAxisInfo.dbAutoRangeReservedMax = 1.0;
                    stAxisYInfo.strAxisInfo.dbAutoRangeReservedMin = 0.;
                    stAxisYInfo.iLineWidth = 3;

                    stAxisYInfo.strAxisInfo.iGridTextPrecision = 3;
                    stAxisYInfo.strAxisInfo.iCursorTextPrecision = 3;
                    m_pGraphWnd[i][j]->SetResourceYInfo(stAxisYInfo, k);
                }

                m_pGraphWnd[i][j]->GetResourceInfo(stResourceInfo);
                stResourceInfo.iAxisThickness = 1;
                stResourceInfo.clrEntireBackGround = RGB(231, 231, 231);//RGB(50, 50, 50);	//! �׷��� ���� ����
                stResourceInfo.strAxisInfoX.iCursorTextPrecision = 0;

                /* LKS */
                //////////////////////////////////////////////////////////////////////////
                if (j < 2)
                {
                    stResourceInfo.strAxisInfoX.eGridTextType = E_GRAPH2D_GRID_DOUBLE;	//E_GRAPH2D_GRID_INTEGER;
                    stResourceInfo.strAxisInfoX.bIncrementGrid = false;	//! Modify by LHW (2013/3/26)
                    stResourceInfo.strAxisInfoX.dbIncrementGrid = -5.0;
                    stResourceInfo.strAxisInfoX.iGridTextPrecision = 3;//0;
                    stResourceInfo.strAxisInfoX.bDirection_Plus = false;	//! Added by LHW (2013/3/26)
                }
                else
                {
                    stResourceInfo.strAxisInfoX.eGridTextType = E_GRAPH2D_GRID_DOUBLE;//E_GRAPH2D_GRID_INTEGER;
                    stResourceInfo.strAxisInfoX.bIncrementGrid = false;
                    stResourceInfo.strAxisInfoX.dbIncrementGrid = 5.0;
                    stResourceInfo.strAxisInfoX.iGridTextPrecision = 3;//0;
                    stResourceInfo.strAxisInfoX.bDirection_Plus = false;// true;		//! Added by LHW (2013/3/26)
                }
                //////////////////////////////////////////////////////////////////////////

                stResourceInfo.strMainTitleInfo.bShow = true;
                stResourceInfo.strMainTitleInfo.eAlign = E_GRAPH2D_TITLE_ALIGN_CENTER;
                m_pGraphWnd[i][j]->SetResourceInfo(stResourceInfo);

                //! <------------------------------------------------------------
                //! X�� Ư���� ǥ�ü� �߰�, X�࿡ ������
                //! �߰��Ǵ� ������ �����Ѵ�. 
                strValueLineXInfo stXLineInfo;

                stXLineInfo.InitInfo();
                stXLineInfo.bShow = false;
                stXLineInfo.dbValue = 0.;
                stXLineInfo.clrValue = RGB(255, 0, 0);
                stXLineInfo.iLineWidth = 1;
                stXLineInfo.szText.Format(_T("[%d] MTF(N/4)"), k + 1);
                m_pGraphWnd[i][j]->AddXValueLine(stXLineInfo);

                stXLineInfo.InitInfo();
                stXLineInfo.bShow = false;
                stXLineInfo.dbValue = 0.;
                stXLineInfo.clrValue = RGB(0, 0, 255);
                stXLineInfo.iLineWidth = 1;
                stXLineInfo.szText.Format(_T("[%d] MTF(N/8)"), k + 1);
                m_pGraphWnd[i][j]->AddXValueLine(stXLineInfo);

                stXLineInfo.InitInfo();
                stXLineInfo.bShow = false;
                stXLineInfo.dbValue = 0.;
                stXLineInfo.clrValue = RGB(255, 128, 0);
                stXLineInfo.iLineWidth = 1;
                stXLineInfo.szText.Format(_T("[%d] MTF50"), k + 1);
                m_pGraphWnd[i][j]->AddXValueLine(stXLineInfo);
                //! <------------------------------------------------------------

                //! �׷����� X, Y�� �̸��� ����
                m_pGraphWnd[i][j]->SetCaptionX(_T(""));
                m_pGraphWnd[i][j]->SetCaptionY(_T("MTF"), _T(""));

                m_pGraphWnd[i][j]->GetResourceInfo(stResourceInfo);
                switch (j)
                {
                case 0:
                    stResourceInfo.strMainTitleInfo.szTitle = _T("ROUGH AA");
                    break;
                case 1:
                    stResourceInfo.strMainTitleInfo.szTitle = _T("FINE AA");
                    break;
                case 2:
                    stResourceInfo.strMainTitleInfo.szTitle = _T("FINAL SFR");
                    break;
                }


                m_pGraphWnd[i][j]->SetResourceInfo(stResourceInfo);

                //! �޽����� ���� �ڵ� ����
                m_pGraphWnd[i][j]->m_hMsgHandle = this->m_hWnd;

                switch (j)
                {
                case 0:
                    GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1 + i)->GetWindowRect(rect);
                    break;
                case 1:
                    GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1 + i)->GetWindowRect(rect);
                    break;
                case 2:
                    GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + i)->GetWindowRect(rect);
                    break;
                }

                ScreenToClient(rect);
                m_pGraphWnd[i][j]->MoveWindow(rect);

                m_pGraphWnd[i][j]->m_bChanged = true;
            }
        }
    }
}

//-----------------------------------------------------------------------------
//
//	MIL ���̺귯�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitMilLib()
{
    double dZoomFac = 0.0;
    CPoint clPt;
    int i;

#ifdef ON_LINE_MIL
    // Application Alloc
    if (g_clVision.AllocMilApplication() == false)
    {
        return;
    }

    // System Alloc
    /*if (g_clVision.AllocMilSystem(0) == false)
    {
        return;
    }*/
     
	// Digitizer �Ҵ�
	if (g_clVision.AllocMilDigitizer(0, 0) == false)
	{
		//AddLog(_T("[FAIL] MIL Digitizer Alloc Failed"), 1, false, 999);
		AddLog(_T("[FAIL] Align ī�޶� ���� ���� Ȯ�ιٶ��ϴ�."), 0, 0);
		return;
	}
	WINDOWPLACEMENT wndpl;
	CWnd* pWnd;
    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
		GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1 + i)->GetWindowPlacement(&wndpl);
		g_clModelData[i].DefectSizeX = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
		g_clModelData[i].DefectSizeY = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
		// Buffer Alloc
		if (g_clVision.AllocMilCamBuffer(0, 0, i, MAX_CAM_COUNT) == false)
		{
			AddLog(_T("[FAIL] MIL Color Buffer Alloc Failed"), 1, i);
			return;
		}
        // CCD Buff Alloc
        if (g_clVision.AllocMilCcdBuffer(0, i, g_clLaonGrabberWrapper[i].m_stMIUDevice.nWidth, (g_clLaonGrabberWrapper[i].m_stMIUDevice.nHeight)) == false)
        {
            return;
        }
		if (g_clVision.AllocMilDefectBuffer(0, i, g_clModelData[i].DefectSizeX, g_clModelData[i].DefectSizeY) == false)
		{
			return;
		}

		
		// Display Alloc
		if (g_clVision.AllocMilDisplay(0, i, m_clVisionStaticCcd[i].m_hWnd) == false)
		{
			return;
		}
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
		// Display Alloc
		if (g_clVision.AllocMilCamDisplay(0, i, m_clVisionStaticCam[i].m_hWnd) == false)
		{
			return;
		}
#endif
        clPt.x = g_clLaonGrabberWrapper[i].m_stMIUDevice.nWidth;
        clPt.y = g_clLaonGrabberWrapper[i].m_stMIUDevice.nHeight;

		dZoomFac = (double)((double)(CCD1_DISP_SIZE_X) / (double)(g_clLaonGrabberWrapper[i].m_stMIUDevice.nWidth));

        m_clVisionStaticCcd[i].SetInit(i, dZoomFac, clPt);
        m_clVisionStaticCcd[i].m_nDragType = MOUSE_DRAG;
        m_clVisionStaticCcd[i].m_bDrag = false;

		clPt.x = CCD1_CAM_SIZE_X;
		clPt.y = CCD1_CAM_SIZE_Y;

		dZoomFac = (double)((double)(SMALL_CAM_SIZE_X) / (double)CCD1_CAM_SIZE_X);
		m_clVisionStaticCam[i].SetInit(i, dZoomFac, clPt);
		m_clVisionStaticCam[i].m_nDragType = MOUSE_DRAG;
		m_clVisionStaticCam[i].m_bDrag = false;
		
        //m_clVisionStaticCcd[i].DisplayZoom();

#ifdef ON_LINE_MIL
		pWnd = GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1 + i);
		MdispSelectWindow(g_clVision.MilDefectDisplayImage[i], g_clVision.MilDefectImage[i], pWnd->m_hWnd);
#endif

        g_clVision.EnableOverlay(0, i);
		g_clVision.EnableCamOverlay(0, i);

		
		
    }
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
    g_clModelFinder.InitMark();
    g_clModelFinder.LoadMark(g_clSysData.m_szModelName);



	m_clVisionGrabThread[0].StartThread();
#endif
#else
    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
        clPt.x = g_clLaonGrabberWrapper[i].m_stMIUDevice.nWidth;
        clPt.y = g_clLaonGrabberWrapper[i].m_stMIUDevice.nHeight/* - 4*/;

		dZoomFac = (double)((double)(CCD1_DISP_SIZE_X) / (double)(g_clLaonGrabberWrapper[i].m_stMIUDevice.nWidth));

        m_clVisionStaticCcd[i].SetInit(i, dZoomFac, clPt);
        m_clVisionStaticCcd[i].SetLock(true);
        m_clVisionStaticCcd[i].m_bDrag = false;

		m_clVisionStaticCam[i].SetInit(i, dZoomFac, clPt);
		m_clVisionStaticCam[i].m_nDragType = MOUSE_DRAG;
		m_clVisionStaticCam[i].m_bDrag = false;
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	���� ���̺귯�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitMotorLib()
{
#ifdef ON_LINE_MOTOR
    int i;
    g_clMotorSet.Axl_Init();

	for (i = 0; i < MAX_UNIT_COUNT; i++)//for (i = 0; i < 2; i++)
	{
		g_clMotorSet.AmpEnableAll(i);
	}

	
#endif
}

//-----------------------------------------------------------------------------
//
//	DIO ���̺귯�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitDioLib()
{
#ifdef ON_LINE_DIO
    g_clDioControl.InitDio();
#endif
}

//-----------------------------------------------------------------------------
//
//	������ �׷��� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitGrabberLib()
{
    int i;

	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		g_clLaonGrabberWrapper[i].AllocImageBuff();
		g_clLaonGrabberWrapper[i].rawReSet();
	}
#ifdef ON_LINE_GRABBER
    int nCount = g_clLaonGrabberWrapper[i].SearchDevice();

    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
        g_clLaonGrabberWrapper[i].SetDevice(i);

   //     if (g_clLaonGrabberWrapper[i].OpenDevice() == true)
   //     {
			//AddLog(_T("[GRAB] OPEN OK"), 1, i);
   //     }
   //     else
   //     {
   //         AddLog(_T("[GRAB] OPEN FAIL"), 1, i);
			////g_clLaonGrabberWrapper[i].CloseDevice();
   //     }
		m_clCcdCvtColorThread[i].StartThread();
		Sleep(50);

		m_clCcdGrabThread[i].StartThread();


    }

    //USBRestartCallback(USBEventHandler, 3000);
#endif
}

//-----------------------------------------------------------------------------
//
//	�׷��� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ReleaseGraphWnd()
{
    int i, j;

    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
        for (j = 0; j < MAX_GRAPH_COUNT; j++)
        {
            if (m_pGraphWnd[i][j] != NULL)
            {
                m_pGraphWnd[i][j]->DestroyWindow();

                delete m_pGraphWnd[i][j];
                m_pGraphWnd[i][j] = NULL;
            }
        }
    }
}

//-----------------------------------------------------------------------------
//
//	�׷��� �׸���
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::DrawGraph(int nUnit, int nPlot)
{

#ifndef GRAPH_NO_USE
    strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��	
    int nIndex;
    int i, j;

    if (nPlot < 0 || nPlot >= MAX_GRAPH_COUNT)
    {
        return;
    }
	int FieldJumpStep = 0;
    //! 1) �׷����� �����͸� �Է��Ѵ�. 
    //! 2) ����, Ŀ�� (Mouse�� ���� ������ �׷��� ������ ǥ�� ���)�� ������ �߰��Ѵ�. 
    //! 3) �׷����� �ٽ� �׸���. 
	g_clModelData[nUnit].m_nSfrMode = 0;	//wide
	//g_clModelData[nUnit].m_nSfrMode = FIELD_085_AA; 

	
    if (m_pGraphWnd[nUnit][nPlot] != NULL)
    { 
        switch (nPlot)
        {
        case 0:
            for (i = 0; i < MAX_GRAPH_OVERLAP_COUNT; i++)
            {
                //! <-----------------------------------------------------------
                //! �׷����� ���� ���̴� ���θ� �����Ѵ�.
                m_pGraphWnd[nUnit][nPlot]->GetResourceYInfo(strAxisYInfo, i);

                m_nDataSize[nUnit][nPlot][i] = g_clTaskWork[nUnit].m_nCountOfTotalStepAA;

                if (m_nDataSize[nUnit][nPlot][i] <= 0)
                {
                    strAxisYInfo.bShowGraph = false;
                }
                else
                {
                    strAxisYInfo.bShowGraph = true;//m_bFlagVisible[nUnit][nPlot][i];
                }

                m_pGraphWnd[nUnit][nPlot]->SetResourceYInfo(strAxisYInfo, i);


                nIndex = 0;

                if (g_clTaskWork[nUnit].m_bFlagDecreaseSFR == true)
                {
                    nIndex = g_clModelData[nUnit].m_nCountOfCheckSFR;
                }
#if (____AA_WAY == PCB_TILT_AA)
				for (j = nIndex; j < m_nDataSize[nUnit][nPlot][i]; j++)
				{
					m_dDataX[nUnit][nPlot][i][j - nIndex] = g_clTaskWork[nUnit].m_stSfrInsp.fMotorPos[j][MOTOR_PCB_Z - MOTOR_PCB_X] *-1;
					m_dDataY[nUnit][nPlot][i][j - nIndex] = g_clTaskWork[nUnit].m_stSfrInsp.fSFR_AVR_N4[j][i];
				}
#else
                for (j = nIndex; j < m_nDataSize[nUnit][nPlot][i]; j++)
                {
					m_dDataX[nUnit][nPlot][i][j - nIndex] = g_clTaskWork[nUnit].m_stSfrInsp.fMotorPos[j][MOTOR_LENS_Z - MOTOR_LENS_X];
                    m_dDataY[nUnit][nPlot][i][j - nIndex] = g_clTaskWork[nUnit].m_stSfrInsp.fSFR_AVR_N4[j][i];
                }
#endif
                //! �׷����� ������ �Է�
                m_pGraphWnd[nUnit][nPlot]->SetPlotData(m_dDataX[nUnit][nPlot][i], m_dDataY[nUnit][nPlot][i], m_nDataSize[nUnit][nPlot][i] - nIndex, i);
            }

            break;
        case 1:
            for (i = 0; i < MAX_GRAPH_OVERLAP_COUNT; i++)
            {
                //! <-----------------------------------------------------------
                //! �׷����� ���� ���̴� ���θ� �����Ѵ�.
                m_pGraphWnd[nUnit][nPlot]->GetResourceYInfo(strAxisYInfo, i);

                m_nDataSize[nUnit][nPlot][i] = g_clTaskWork[nUnit].m_nCountOfTotalStepAA - g_clTaskWork[nUnit].m_nCountOfSecondStepAA;

                if (m_nDataSize[nUnit][nPlot][i] <= 0)
                {
                    strAxisYInfo.bShowGraph = false;
                }
                else
                {
                    strAxisYInfo.bShowGraph = true;//m_bFlagVisible[nUnit][nPlot][i];
                }

                m_pGraphWnd[nUnit][nPlot]->SetResourceYInfo(strAxisYInfo, i);

#if (____AA_WAY == PCB_TILT_AA)
				for (j = 0; j < m_nDataSize[nUnit][nPlot][i]; j++)
				{
					m_dDataX[nUnit][nPlot][i][j] = g_clTaskWork[nUnit].m_stSfrInsp.fMotorPos[j + g_clTaskWork[nUnit].m_nCountOfSecondStepAA][MOTOR_PCB_Z - MOTOR_PCB_X] * -1;
					m_dDataY[nUnit][nPlot][i][j] = g_clTaskWork[nUnit].m_stSfrInsp.fSFR_AVR_N4[j + g_clTaskWork[nUnit].m_nCountOfSecondStepAA][i];
				}
#else
                for (j = 0; j < m_nDataSize[nUnit][nPlot][i]; j++)
                {
					m_dDataX[nUnit][nPlot][i][j] = g_clTaskWork[nUnit].m_stSfrInsp.fMotorPos[j + g_clTaskWork[nUnit].m_nCountOfSecondStepAA][MOTOR_LENS_Z - MOTOR_LENS_X];
                    m_dDataY[nUnit][nPlot][i][j] = g_clTaskWork[nUnit].m_stSfrInsp.fSFR_AVR_N4[j + g_clTaskWork[nUnit].m_nCountOfSecondStepAA][i];
                }
#endif
                m_pGraphWnd[nUnit][nPlot]->SetPlotData(m_dDataX[nUnit][nPlot][i], m_dDataY[nUnit][nPlot][i], m_nDataSize[nUnit][nPlot][i], i);
            }
            break;
        case 2:
            break;
        }

        m_pGraphWnd[nUnit][nPlot]->m_bChanged = true; 
        m_pGraphWnd[nUnit][nPlot]->Invalidate();
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	�� �׷��� �׸���
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::_DrawBarGraph(int nUnit)
{
    CString sTemp;
    HWND hWnd = GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + nUnit)->m_hWnd;
    CDC pDC;
    HDC hDC = ::GetDC(hWnd);
    pDC.Attach(hDC);

    CRect rcClient;
    GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + nUnit)->GetClientRect(rcClient);
    CBrush Brush;
    CBrush* pOldBrush;
    Brush.CreateSolidBrush(RGB(255, 255, 255));//�׷��� ����
    pOldBrush = pDC.SelectObject(&Brush);
    pDC.FillRect(rcClient, &Brush);

    int TopGap = 25;//50;
    int BottomGap = 20; 
    int barGraphX = 0;
    int barWidth = 20;

    CPen* pOldPen;
    CPen pen_LineBase; CPen pen_LineVertical;

    CFont font_LimitVal; CFont font_BarName; CFont* pOldFont;
    CFont font_Value;
    Graph_Width = rcClient.right - rcClient.left;
    Graph_Height = rcClient.bottom - rcClient.top;
    pen_LineBase.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    //
    pen_LineVertical.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));		//���� ���� ��
    pOldPen = pDC.SelectObject(&pen_LineBase);
    //�׷��� �ϴ� £����
    pDC.MoveTo(5, Graph_Height - BottomGap + 1);
    pDC.LineTo(Graph_Width - 5, Graph_Height - BottomGap + 1);
    //
    pOldPen = pDC.SelectObject(&pen_LineVertical);
    //���� ���� ����
    int startX = 35;//���ζ��� ���� ��ġ x
    int poxY = 0;
    int endGap = 20;
	int lineYGap = (Graph_Height - BottomGap - TopGap) / 5;
    CSize fontSize;
    sTemp = "0.1";
    pDC.SetBkMode(TRANSPARENT);//���ڻ� ��� ����
                               //
    double gapNum = 0.0;
    font_Value.CreateFont(15, 6, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "arial");
    pOldFont = pDC.SelectObject(&font_Value);
    pDC.SetTextColor(RGB(100, 100, 100));
    
    for (int i = 0; i < 5; i++)
    {
        gapNum = ((5 - i) / 5.0);//(i % 5);
        sTemp.Format("%.1lf", gapNum);
        poxY = (lineYGap * i) + TopGap;
        pDC.MoveTo(startX, poxY);
        pDC.LineTo(Graph_Width - endGap, poxY);
        fontSize = pDC.GetTextExtent(sTemp);
        pDC.TextOut(10, poxY - (fontSize.cy / 2), sTemp);
    }

    //
    font_LimitVal.CreateFont(16, 7, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "arial");
    pOldFont = pDC.SelectObject(&font_LimitVal);


    //���� �׷��� ��
    font_BarName.CreateFont(15, 6, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "arial");
    //
    pOldFont = pDC.SelectObject(&font_BarName);
    pDC.SetTextColor(RGB(0, 0, 0));
    COLORREF _BarColor;
	const char *labels[] = { "Center", "LT04", "RT04", "LB04", "RB04" };
	
	//const char *labels[] = { "Center", "LT", "RT", "LB", "RB" };
    
    int textX = 0;
    CSize display_size;
    double sfrValue = 0.0;
    for (int i = 0; i < 5; i++)//for (int i = 0; i<MAX_MTF_NO; i++)
    {
        sfrValue = g_clTaskWork[nUnit].m_stSfrInsp._fAverSfr[i];
		if (sfrValue < 0.0) sfrValue = 0.0;
		if (sfrValue > 1.0) sfrValue = 1.0;
        if (i == 0)			_BarColor = RGB(0, 0, 255);
        else if (i == 1)	_BarColor = RGB(255, 0, 0);
        else if (i == 2)	_BarColor = RGB(0, 180, 0); 
        else if (i == 3)	_BarColor = RGB(63, 0, 153);
        else if (i == 4)	_BarColor = RGB(0, 216, 255);
        CPen BarPen;
        CBrush Barbrush;
        BarPen.CreatePen(PS_DOT, 3, _BarColor);    // ������ ���� ����
        CPen* BaroldPen = pDC.SelectObject(&BarPen);
        Barbrush.CreateSolidBrush(_BarColor);     // �������� ä����� ����
        CBrush* BaroldBrush = pDC.SelectObject(&Barbrush);
        barGraphX = (Graph_Width - 5) / 6 * (i + 1);
        pDC.Rectangle(
            barGraphX,
            (Graph_Height - BottomGap) - (int)(sfrValue * (Graph_Height - BottomGap - TopGap) + 0) + 0,
            barGraphX + barWidth,
            Graph_Height - BottomGap);
        pDC.SelectObject(BaroldBrush);
        pDC.SelectObject(BaroldPen);

        //////////////////////////////////////////////////////////////////////////////////////////
        
        sTemp.Format("%.3lf", sfrValue);
        display_size = pDC.GetTextExtent(sTemp);
        textX = barGraphX + (barWidth / 2) - (display_size.cx / 2);

        pDC.SetBkMode(TRANSPARENT);//���ڻ� ��� ����
        pDC.SetTextColor(RGB(0, 0, 0));
        pDC.TextOut(textX, (Graph_Height - BottomGap) - (int)(sfrValue * (Graph_Height - BottomGap - TopGap) + 0.5) - 15, sTemp);

        pDC.SetTextColor(_BarColor);
        textX = barGraphX + (barWidth / 2) - (display_size.cx / 2);
        display_size = pDC.GetTextExtent(labels[i]);
        pDC.TextOut(barGraphX + (barWidth / 2) - (display_size.cx / 2), (Graph_Height - BottomGap) + 2, labels[i]);
        //pDC.DrawText(labels[i], CRect(100, 100, 320, 240), NULL);
        //////////////////////////////////////////////////////////////////////////////////////////
    }

    pDC.SelectObject(pOldPen);
    pen_LineBase.DeleteObject();
    pen_LineVertical.DeleteObject();

    pDC.SelectObject(pOldBrush);
    Brush.DeleteObject();

    pDC.SelectObject(pOldFont);
    font_Value.DeleteObject();
    font_LimitVal.DeleteObject();
    font_BarName.DeleteObject();

    pDC.Detach();
    ::ReleaseDC(hWnd, hDC);
}
void CAutoInspDlg::DrawBarGraph(int nUnit)
{
    CString sTemp;
    HWND hWnd = GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + nUnit)->m_hWnd;
    CDC pDC;
    HDC hDC = ::GetDC(hWnd);

    pDC.Attach(hDC);

    CRect rcClient;
    GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + nUnit)->GetClientRect(rcClient);

    CBrush Brush;
    CBrush* pOldBrush;
    Brush.CreateSolidBrush(RGB(255, 255, 255));
    pOldBrush = pDC.SelectObject(&Brush);

    pDC.FillRect(rcClient, &Brush);

    int iSizeX_Client = rcClient.right - rcClient.left;
    int iSizeY_Client = rcClient.bottom - rcClient.top;
    int iOffsetY = 20;

    CPen* pOldPen;
    CPen pen_LineBase;
    CPen pen_LineVertical;
    CPen pen_LineLimitCen;
    CPen pen_LineLimitSide;

    CFont font_LimitVal;
    CFont font_BarName;
    CFont* pOldFont;

    pen_LineBase.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    pen_LineVertical.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    pen_LineLimitCen.CreatePen(PS_SOLID, 1, RGB(128, 0, 0));
    pen_LineLimitSide.CreatePen(PS_SOLID, 1, RGB(128, 0, 128));

    pOldPen = pDC.SelectObject(&pen_LineBase);

    pDC.MoveTo(5, iSizeY_Client - iOffsetY);
    pDC.LineTo(iSizeX_Client - 5, iSizeY_Client - iOffsetY);

    pOldPen = pDC.SelectObject(&pen_LineVertical);

    pDC.MoveTo(5, 0);
    pDC.LineTo(iSizeX_Client - 5, 0);

    pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 1);
    pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 1);

    pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 2);
    pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 2);

    pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 3);
    pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 3);

    font_LimitVal.CreateFont(10, 6, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Airal"));
    pOldFont = pDC.SelectObject(&font_LimitVal);

    // AA����϶��� ���� SFR �׷��� �׸���
    pOldPen = pDC.SelectObject(&pen_LineLimitCen);
    pDC.MoveTo(5, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRCent * (iSizeY_Client - iOffsetY) + 0.5));
    pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRCent * (iSizeY_Client - iOffsetY) + 0.5));
    sTemp.Format(_T("%.02lf"), g_clModelData[nUnit].m_dLimitSFRCent);
    pDC.SetTextColor(RGB(128, 0, 0));
    pDC.TextOut(iSizeX_Client - 30, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRCent * (iSizeY_Client - iOffsetY) + 0.5) - 12, sTemp);

    pOldPen = pDC.SelectObject(&pen_LineLimitSide);
    pDC.MoveTo(5, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRSide * (iSizeY_Client - iOffsetY) + 0.5));
    pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRSide * (iSizeY_Client - iOffsetY) + 0.5));
    sTemp.Format(_T("%.02lf"), g_clModelData[nUnit].m_dLimitSFRSide);
    pDC.SetTextColor(RGB(128, 0, 128));
    pDC.TextOut(iSizeX_Client - 30, (iSizeY_Client - iOffsetY) - (int)(g_clModelData[nUnit].m_dLimitSFRSide * (iSizeY_Client - iOffsetY) + 0.5) - 12, sTemp);

    CPen pen_Cen, pen_LeftUpper, pen_RightUpper, pen_LeftLower, pen_RightLower;
    pen_Cen.CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
    pen_LeftUpper.CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
    pen_RightUpper.CreatePen(PS_SOLID, 4, RGB(0, 255, 0));
    pen_LeftLower.CreatePen(PS_SOLID, 4, RGB(63, 0, 153));
    pen_RightLower.CreatePen(PS_SOLID, 4, RGB(0, 216, 255));

    font_BarName.CreateFont(12, 7, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Arial"));
    pOldFont = pDC.SelectObject(&font_BarName);
    pDC.SetTextColor(RGB(0, 0, 0));

    // // 20150526 LHC - cen, 0.4f, 0.7f  Min�� �׷����� ��Ÿ����
    float sfrValue = 0.0;
    for (int i = 0; i < MAX_MTF_COUNT; i++)
    {
        switch (i)
        {
        case MTF_CENTER:		pOldPen = pDC.SelectObject(&pen_Cen);			break;
        case MTF_LEFT_UPPER:	pOldPen = pDC.SelectObject(&pen_LeftUpper);		break;
        case MTF_RIGHT_UPPER:	pOldPen = pDC.SelectObject(&pen_RightUpper);	break;
        case MTF_LEFT_BOTTOM:	pOldPen = pDC.SelectObject(&pen_LeftLower);		break;
        case MTF_RIGHT_BOTTOM:	pOldPen = pDC.SelectObject(&pen_RightLower);	break;
        }
        sfrValue = g_clTaskWork[nUnit].m_stSfrInsp._fAverSfr[i];
        //g_clTaskWork[nUnit].m_stSfrInsp._fAverSfr[i]
        // AA����϶��� ���� SFR �׷��� �׸���
        pDC.MoveTo((iSizeX_Client - 5) / 6 * (i + 1), iSizeY_Client - iOffsetY);
        pDC.LineTo((iSizeX_Client - 5) / 6 * (i + 1), (iSizeY_Client - iOffsetY) - (int)(sfrValue * (iSizeY_Client - iOffsetY) + 0.5));
        sTemp.Format(_T("%.3lf"), sfrValue);
        pDC.TextOut((iSizeX_Client - 5) / 6 * (i + 1) - 15, (iSizeY_Client - iOffsetY) - (int)(sfrValue * (iSizeY_Client - iOffsetY) + 0.5) - 10, sTemp);
    }

    pDC.TextOut((iSizeX_Client - 5) / 6 - 13, (iSizeY_Client - iOffsetY) + 5, _T("CEN"));
    pDC.TextOut((iSizeX_Client - 5) / 6 * 2 - 10, (iSizeY_Client - iOffsetY) + 5, _T("L/U"));
    pDC.TextOut((iSizeX_Client - 5) / 6 * 3 - 10, (iSizeY_Client - iOffsetY) + 5, _T("R/U"));
    pDC.TextOut((iSizeX_Client - 5) / 6 * 4 - 10, (iSizeY_Client - iOffsetY) + 5, _T("L/L"));
    pDC.TextOut((iSizeX_Client - 5) / 6 * 5 - 10, (iSizeY_Client - iOffsetY) + 5, _T("R/L"));
    // 20150526 LHC - cen, 0.4f, 0.7f  Min�� �׷����� ��Ÿ����

    pDC.SelectObject(pOldPen);
    pen_LineBase.DeleteObject();
    pen_LineVertical.DeleteObject();
    pen_LineLimitCen.DeleteObject();
    pen_LineLimitSide.DeleteObject();

    pen_Cen.DeleteObject();
    pen_LeftUpper.DeleteObject();
    pen_RightUpper.DeleteObject();
    pen_LeftLower.DeleteObject();
    pen_RightLower.DeleteObject();

    pDC.SelectObject(pOldBrush);
    Brush.DeleteObject();

    pDC.SelectObject(pOldFont);
    font_LimitVal.DeleteObject();
    font_BarName.DeleteObject();

    pDC.Detach();
    ::ReleaseDC(hWnd, hDC);
}

//-----------------------------------------------------------------------------
//
//	CCD FPS ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::DrawCcdFps(int nUnit)
{
    TCHAR szData[SIZE_OF_100BYTE];
	 
    if (nUnit < 0 || nUnit >= MAX_UNIT_COUNT)
        return;
    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.3f"), g_clTaskWork[nUnit].m_fFps);
    m_clColorStaticFpsVal[nUnit].SetWindowText(szData);
	m_clColorStaticFpsVal[nUnit].Invalidate();
}

//-----------------------------------------------------------------------------
//
//	AA MAIN ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::ConnectAAMain()
{
    CString sMsg = _T("");

	if (m_bConnectMain == true)
	{
		return true;
	}

    m_clAgentConnSocket.Create();
    if (m_clAgentConnSocket.Connect(g_clSysData.m_szAAIp, g_clSysData.m_nAAPort) == FALSE) 
    {
        m_clAgentConnSocket.Close();

        return false;
    }

    m_clAgentConnSocket.SetMainDlgPtr(this);
    m_clButtonExLan.state = 1;
    m_clButtonExLan.Invalidate();
    m_bConnectMain = true;

    return true;
}

//-----------------------------------------------------------------------------
//
//	AA ���� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::CloseAAMain()
{
    m_clAgentConnSocket.Close();
    m_bConnectMain = false;

    m_clButtonExLan.state = 0;
    m_clButtonExLan.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	AA ������ ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::RecvAAMainData(CString sData)
{
    TCHAR szLog[SIZE_OF_1K];
    CString sToken = _T("");
    CString sCmd = _T("");
    CString sVal = _T("");
    int nAANo = 0;
    int nUnit = 0;

    // ��Ŷ ��� #AA%d@CMD&VAL$

    // AA NO
    AfxExtractSubString(sToken, sData, 0, _T('@'));
    sToken = sToken.Right(sToken.GetLength() - 1);


	if (sToken == _T("AA1"))
	{
		nAANo = 0;
		nUnit = 0;
	} 
	else if (sToken == _T("AA2"))
	{
		nAANo = 1;
		nUnit = 0;
	}
	else if (sToken == _T("AA3"))
	{
		nAANo = 2;
		nUnit = 0;
	}
	else if (sToken == _T("AA4"))
	{
		nAANo = 3;
		nUnit = 0;
	}
	else
	{
		return;
	}
    

    // ���� �α�
    _stprintf_s(szLog, SIZE_OF_1K, _T("[RECV] %s"), (TCHAR*)(LPCTSTR)sData);
    AddLog(szLog, 0, nUnit);

    // CMD
    AfxExtractSubString(sToken, sData, 1, _T('@'));
    AfxExtractSubString(sCmd, sToken, 0, _T('&'));

    // VAL
    AfxExtractSubString(sVal, sToken, 1, _T('&'));
    sVal = sVal.Left(sVal.GetLength() - 1);


	_stprintf_s(szLog, SIZE_OF_1K, sData);

	AddLog(szLog, 0, nUnit);
    this->ProcessAAMainPacket(nUnit, nAANo, sCmd, sVal);
}

//-----------------------------------------------------------------------------
//
//	AA MAIN ��Ŷ ó��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ProcessAAMainPacket(int nUnit, int nAANo, CString sCmd, CString sVal)
{
    
    double dPos = 0.0;

    if (sCmd == _T("PCB"))	//PCB ���޿�û ����
    {
        if (sVal == _T("LOCK"))
        {
            g_clTaskWork[nUnit].m_bPcbInterLock = true;
        }
        else if (sVal == _T("UNLOCK"))
        {
            g_clTaskWork[nUnit].m_bPcbInterLock = false;
        }
    }
	else if (sCmd == _T("LENS"))	//LENS ���޿�û ����
	{
		if (sVal == _T("LOCK"))
		{
			g_clTaskWork[nUnit].m_bLensInterLock = true;
		}
		else if (sVal == _T("UNLOCK"))
		{
			g_clTaskWork[nUnit].m_bLensInterLock = false;
		}
	}
	else if (sCmd == _T("LENS_CHK"))	//LENS ���޿�û ����
	{
		if (sVal == _T("ACK"))
		{
			g_clTaskWork[nUnit].m_bLensTransferChk = true;
		}
		else if (sVal == _T("NAK"))
		{
			g_clTaskWork[nUnit].m_bLensTransferChk = false;
		}
	}


	else if (sCmd == _T("MES_ACK"))
	{
		g_clTaskWork[nUnit].m_bPcbSocketIF[MES_ACK] = true;
	}
	else if (sCmd == _T("PCB_BCR"))	//�������� LOCK ��ȣ -> PCB_BCR ��ȣ -> UNLOCK ��ȣ
	{
		TCHAR szLog[SIZE_OF_1K];
		CString sToken = _T("");
		SYSTEMTIME stSysTime;
		::GetLocalTime(&stSysTime);

		_stprintf_s(szLog, SIZE_OF_1K, _T("FROM LEE:%s"), sVal);
		//sMsg.Format("FROM LEE:%s", sVal);
		AddLog(szLog, 0, nUnit);
		g_clTaskWork[nUnit].m_bPcbSocketIF[PCB_BCR] = true;


		//[20:47:36.081] [SEND] #AA2@PCB_BCR&21A14T132164,0$
		// BCR
		AfxExtractSubString(sToken, (TCHAR*)(LPCTSTR)sVal, 0, _T(','));
		_stprintf_s(g_clTaskWork[nUnit].m_szChipID, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)sToken);
		g_clMesCommunication[nUnit].m_sMesLotID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sToken);//PCB_BCR
		g_clMandoInspLog[nUnit].m_sBarcodeID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sToken);
		// MES ����
		AfxExtractSubString(sToken, (TCHAR*)(LPCTSTR)sVal, 1, _T(','));
		g_clMesCommunication[nUnit].m_nMesCnt = _ttoi((TCHAR*)(LPCTSTR)sToken);
		g_clModelData[nUnit].LotDataSave();
		m_clColorStaticBcrVal[nUnit].SetWindowText(g_clTaskWork[nUnit].m_szChipID);
		m_clColorStaticBcrVal[nUnit].Invalidate();
		//
		//eeprom data get
		CString EEpromTemp;
		CString parseTemp;
		EEpromTemp.Empty();
		parseTemp.Empty();
		AfxExtractSubString(EEpromTemp, sVal, 2, _T(','));
		int nCount = count((LPCTSTR)EEpromTemp, (LPCTSTR)EEpromTemp + EEpromTemp.GetLength(), _T(';'));
		int leng = 0;
		int index = 0;
		int i = 0;
		int j = 0;
		
		//memset(g_clMesCommunication[nUnit].mMesGetEEpromData, 0x00, sizeof(g_clMesCommunication[nUnit].mMesGetEEpromData));
		//for (i = 0; i < nCount + 1; i++)
		//{ 
		//	parseTemp.Empty();
		//	AfxExtractSubString(parseTemp, EEpromTemp, i, ';');
		//	leng = parseTemp.GetLength();

		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[RECV]EEPROM LENGTH:[%d = %d]"), i, leng);
		//	AddLog(szLog, 0, nUnit);

		//	for (j = 0; j < leng / 2; j++)
		//	{
		//		g_clMesCommunication[nUnit].mMesGetEEpromData[index] = (byte)_tcstol(parseTemp.Mid(j * 2, 2), NULL, 16);
		//		index++;
		//	}
		//}

		////230411 EOL���� Ű�� MAIN PC���� ���� mMesGetEEpromData ���� ������� �����ϰ� �ε��ض�
		//EEPROM_Temp_DumpSave(nUnit, g_clMesCommunication[nUnit].mMesGetEEpromData);
		//
		EEpromTemp.Empty();
		parseTemp.Empty();
		sToken.Empty();
		//
	}
	else if (sCmd == _T("PCB_ALR"))
	{
		g_clTaskWork[nUnit].m_bPcbSocketIF[PCB_ALR] = true;
		CString szStr;
		AfxExtractSubString(szStr, sVal, 0, _T(','));
		if (szStr == _T("ACK"))
		{
			g_clTaskWork[nUnit].m_bPcbSocketIF[PCB_ALR_ACK] = true;

			AfxExtractSubString(szStr, sVal, 1, _T(','));
			g_clTaskWork[nUnit].m_dAADataX = _ttof(szStr);
			AfxExtractSubString(szStr, sVal, 2, _T(','));
			g_clTaskWork[nUnit].m_dAADataY = _ttof(szStr);
			AfxExtractSubString(szStr, sVal, 3, _T(','));
			g_clTaskWork[nUnit].m_dAADataTh = _ttof(szStr);
		}
		else if (szStr == _T("NAK"))
		{
			g_clTaskWork[nUnit].m_bPcbSocketIF[PCB_ALR_ACK] = false;
		}
		else if (szStr == _T("OK"))
		{
			g_clTaskWork[nUnit].m_bPcbSocketIF[PCB_ALR_OK] = true;

			AfxExtractSubString(szStr, sVal, 1, _T(','));
			g_clTaskWork[nUnit].m_dAADataX = _ttof(szStr);
			AfxExtractSubString(szStr, sVal, 2, _T(','));
			g_clTaskWork[nUnit].m_dAADataY = _ttof(szStr);
			AfxExtractSubString(szStr, sVal, 3, _T(','));
			g_clTaskWork[nUnit].m_dAADataTh = _ttof(szStr);
		}

		m_clManualDlg.ShowAlignValue();
	}

	//else if (sCmd == _T("PCB_TYP"))
	//{
	//	if (sVal == _T("MODERATE"))	//LEFT
	//	{
	//		g_clModelType.m_nModelIndex = MODERATE_MODEL;
	//		m_clColorStaticPcbType[0].SetWindowText(_T("MODEL TYPE #1 : MODERATE"));
	//		m_clColorStaticPcbType[1].SetWindowText(_T("MODEL TYPE #2 : MODERATE"));
	//	}
	//	else
	//	{
	//		g_clModelType.m_nModelIndex = NARROW_MODEL;
	//		m_clColorStaticPcbType[0].SetWindowText(_T("MODEL TYPE #1 : NARROW"));
	//		m_clColorStaticPcbType[1].SetWindowText(_T("MODEL TYPE #2 : NARROW"));
	//	}

	//	// �� ������ �ε�
	//	///g_clModelData[nUnit].SetUnit(nUnit);
	//	g_clModelData[nUnit].Load(g_clSysData.m_szModelName);
	//	g_clModelData[nUnit].ModulTypeLoad();
	//	//
	//	m_clTeachingDlg.SetUnit(nUnit);

	//	g_clSysData.sDSave();
	//	if (nUnit == 1)
	//	{
	//		this->ShowDialog(m_nCurrentDlg);
	//		//this->ShowDialog(DLG_TEACH);
	//	}
	//}
    return;
}

//-----------------------------------------------------------------------------
//
//	AA MAIN���� ��Ŷ �۽�
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::SendDataToAAMain(int nUnit, CString sData)
{
#if 0
    //TCHAR szLog[SIZE_OF_1K];
	CString szLog;
    int nSendSize;
    int nRetVal;

#ifndef ON_LINE_SOCKET
    return true;
#endif

    if (m_bConnectMain == false)
        return false;

    nSendSize = sData.GetLength();

    nRetVal = m_clAgentConnSocket.Send(sData, nSendSize);
    if (nRetVal != nSendSize)
    {
       // _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] MAIN %s SEND FAIL(ORG:%d, SENT:%d)"), (TCHAR*)(LPCTSTR)sData, nSendSize, nRetVal);
		szLog.Format(_T("[SOCKET] MAIN %s SEND FAIL(ORG:%d, SENT:%d)"), (TCHAR*)(LPCTSTR)sData, nSendSize, nRetVal);
        AddLog(szLog, 0, nUnit);

        return false;
    }

    //_stprintf_s(szLog, SIZE_OF_1K, _T("[SEND] %s"), (TCHAR*)(LPCTSTR)sData);
	szLog.Format(_T("[SEND] %s"), (TCHAR*)(LPCTSTR)sData);

	if(SkipLog(szLog) == true)
		AddLog(szLog, 0, nUnit);

#endif
    return true;
}
bool CAutoInspDlg::SkipLog(CString strLog)
{
	if (strLog.Find(_T("@ULD&REQ")) < 0)
		return false;
	if (strLog.Find(_T("@PCB&REQ")) < 0)
		return false;

	return true;
}
//-----------------------------------------------------------------------------
//
//	Socket �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::InitSocket()
{
#ifdef ON_LINE_SOCKET
    TCHAR szLog[SIZE_OF_1K];

    if (this->CreateListenSocket() == false)
    {
        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] AA ���� ����(%d) ���� ����"), g_clSysData.m_nInspPort);
        //AddLog(szLog, 1, 999);
        AddLog(szLog, 1, 0);
        AddLog(szLog, 1, 1);
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	ȭ�� ����� Lisen Socket ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::CreateListenSocket()
{
    TCHAR szLog[SIZE_OF_1K];
    //int i;

    if (m_clAgentListenSocket.Create(g_clSysData.m_nInspPort, SOCK_STREAM, NULL) == TRUE)
    {
        m_clInspSocketList.RemoveAll();

        m_pInspConnSocket = NULL;

        m_clAgentListenSocket.SetMainDlgPtr(this);
        m_clAgentListenSocket.Listen();

        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] Agent Listen Socket(%d) Create ����"), g_clSysData.m_nInspPort);
        //AddLog(szLog, 0, 999);
        AddLog(szLog, 0, 0);
        AddLog(szLog, 0, 1);
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//
//	����� ȭ�� ���� Close
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::CloseAllSocket()
{
    POSITION posi;
    //int i;

    m_clAgentListenSocket.Close();

    posi = m_clInspSocketList.GetHeadPosition();
    while (posi != NULL)
    {
        CInspConnSocket* p = (CInspConnSocket*)m_clInspSocketList.GetNext(posi);
        if (p != NULL)
        {
            p->Close();

            delete p;
        }
    }

    m_clInspSocketList.RemoveAll();

    m_pInspConnSocket = NULL;
}

//-----------------------------------------------------------------------------
//
//	ȭ�� PC ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::AcceptInspConn()
{
    TCHAR szLog[SIZE_OF_1K];
    CString sPeerAddress;
    unsigned int nPeerPort;

    CInspConnSocket* pSocket = new CInspConnSocket;
    if (pSocket != NULL)
    {
        m_clAgentListenSocket.Accept(*pSocket);

        pSocket->SetMainDlgPtr(this);

        m_clInspSocketList.AddTail(pSocket);

        pSocket->GetPeerName(sPeerAddress, nPeerPort);
        if (this->IsAcceptableInspAgent(pSocket, sPeerAddress) == false)
        {
            this->CloseInspConn(pSocket, true);
        }
        else
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] ȭ��PC(%s)���� ����"), (TCHAR*)(LPCTSTR)sPeerAddress);
            //AddLog(szLog, 0, 999);
            AddLog(szLog, 0, 0);
            AddLog(szLog, 0, 1);
        }
    }
}


//-----------------------------------------------------------------------------
//
//	���� ������ IP���� üũ
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::IsAcceptableInspAgent(CInspConnSocket* pSocket, CString sPeerAddress)
{
    //int i;

    if (!_tcscmp((TCHAR*)(LPCTSTR)sPeerAddress, g_clSysData.m_szInspIp))
    {
        m_clButtonExInspLan.state = 1;

        m_pInspConnSocket = pSocket;

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//
//	ȭ�� ���� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::CloseInspConn(CInspConnSocket* pSocket, bool bForce)
{
    TCHAR szLog[SIZE_OF_1K];
    CString sPeerAddress;
    unsigned int nPeerPort;
    POSITION prePosi, curPosi;
    //int i;

    pSocket->GetPeerName(sPeerAddress, nPeerPort);

    if (bForce == false)
        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] AA PC(%s)���� ���� ����"), (TCHAR*)(LPCTSTR)sPeerAddress);
    else
        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] �� �� ���� ȭ��PC(%s) => ������ ���� ����"), (TCHAR*)(LPCTSTR)sPeerAddress);

    //AddLog(szLog, 0, 999);
    AddLog(szLog, 0, 0);
    AddLog(szLog, 0, 1);

    pSocket->Close();

    if (m_pInspConnSocket == pSocket)
    {
        m_clButtonExInspLan.state = 1;
        m_pInspConnSocket = NULL;
    }

    // ���� ����Ʈ���� ����
    curPosi = m_clInspSocketList.GetHeadPosition();
    while (curPosi != NULL)
    {
        prePosi = curPosi;

        CInspConnSocket* p = (CInspConnSocket*)m_clInspSocketList.GetNext(curPosi);
        if (p == pSocket)
        {
            m_clInspSocketList.RemoveAt(prePosi);
            delete p;

            break;
        }
    }
}


//-----------------------------------------------------------------------------
//
//	ȭ�� ��Ŷ ó��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ProcessInspPacket(CString sPacket)
{
    CString sToken = _T("");
    TCHAR szLog[SIZE_OF_1K];
    CString sMsg = _T("");

    AfxExtractSubString(sToken, sPacket, 1, _T('|'));
    if (sToken == _T("GET_DATA"))
    {
        AfxExtractSubString(sToken, sPacket, 2, _T('|'));
        if (sToken.GetLength() > 0)
        {
            if (this->SelectAAData(999, sToken) == false)
            {
                _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] %s ���ڵ忡 ���� �ڼ� ������ �������� �ʽ��ϴ�."), (TCHAR*)(LPCTSTR)sToken);
                //AddLog(szLog, 0, 999);
                AddLog(szLog, 0, 0);
                AddLog(szLog, 0, 1);

                sMsg.Format(_T("#|PUT_DATA|%s|NG|0.000,0.000,0.000,0.000,0.000,0.000,0.000|@"), (TCHAR*)(LPCTSTR)sToken);

                this->SendDataToInsp(sMsg);
            }
            else
            {
                sMsg.Format(_T("#|PUT_DATA|%s|OK|%.03lf,%.03lf,%.03lf,%.03lf,%.03lf,%.03lf,%.03lf|@"), (TCHAR*)(LPCTSTR)sToken,
                    m_dInspAlignData[0], m_dInspAlignData[1], m_dInspAlignData[2], m_dInspInTiltData[0], m_dInspInTiltData[1],
                    m_dInspOutTiltData[0], m_dInspOutTiltData[1]);

                this->SendDataToInsp(sMsg);
            }
        }
    }
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::SendDataToInsp(CString sData)
{
    TCHAR szLog[SIZE_OF_1K];
    int nSendSize;
    int nRetVal;

    nSendSize = sData.GetLength();

    if (m_pInspConnSocket != NULL)
    {
        nRetVal = m_pInspConnSocket->Send(CT2A(sData), nSendSize);
        if (nRetVal != nSendSize)
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET][->ȭ��] %s SEND FAIL(ORG:%d, SENT:%d)"), (TCHAR*)(LPCTSTR)sData, nSendSize, nRetVal);
            //AddLog(szLog, 0, 999);
            AddLog(szLog, 0, 0);
            AddLog(szLog, 0, 1);
            return false;
        }

        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET][->ȭ��] %s"), (TCHAR*)(LPCTSTR)sData);
        //AddLog(szLog, 0, 999);
        AddLog(szLog, 0, 0);
        AddLog(szLog, 0, 1);
        return true;
    }

    _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET][ȭ��] ȭ�� ���� �ȵ�"));
    //AddLog(szLog, 0, 999);
    AddLog(szLog, 0, 0);
    AddLog(szLog, 0, 1);

    return false;
}






BOOL CAutoInspDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//CRect rect;
	//GetClientRect(rect);
	////rect.bottom = rect.bottom - 200;
	//pDC->FillSolidRect(rect, RGB(220, 220, 220));
	////pDC->Rectangle(MAIN_DLG_WIDTH / 2, 0, MAIN_DLG_WIDTH / 2+5, CUserCommon.m_ListBox1_Pos.bottom);
	//pDC->MoveTo(MAIN_DLG_WIDTH / 2, 0);
	//pDC->LineTo(MAIN_DLG_WIDTH / 2, CUserCommon.m_ListBox1_Pos.bottom);

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CAutoInspDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	/*if (pWnd->GetDlgCtrlID() == IDD_TESLAINSPINSP_DIALOG) {

		pDC->SetTextColor(RGB(0, 255, 255));

		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)::GetStockObject(NULL_BRUSH);

	}*/
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}



//-----------------------------------------------------------------------------
//
//	���̾�α� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowDialog(int nDlg)
{

    int nMoveType = 0;
    int i;

	if (m_nCurrentUnit == UNIT_AA1) 
	{
		nMoveType = SHOW_UNIT1;
	}
	else {
		nMoveType = SHOW_UNIT2;
	}
    for (i = 0; i < MAX_UNIT_COUNT; i++)
    {
		m_clVisionStaticCcd[i].m_bDrag = false;
		m_clVisionStaticCam[i].m_bDrag = false;
		
#ifdef ON_LINE_MIL
		g_clVision.ClearOverlay(i, VIDEO_CCD);
		
#endif
#ifdef ON_LINE_CAM
		g_clVision.ClearOverlay(i, VIDEO_CAM);
#endif
    }

   // if (m_clUpdateHistoryDlg.IsWindowVisible() == TRUE)
        //m_clUpdateHistoryDlg.ShowWindow(SW_HIDE);

	m_clColorButtonMain.state = 200;
	m_clColorButtonManual.state = 200;
	m_clColorButtonTeaching.state = 200;
	m_clColorButtonCcd.state = 200;
	m_clColorButtonDio.state = 200;
	m_clColorButtonLight.state = 200;
	m_clColorButtonAlarm.state = 200;
	m_clColorButtonConfig.state = 200;
    switch (nDlg)
    {
    case DLG_MAIN:
        this->MoveMainUI(SHOW_ALL);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

		m_clColorButtonMain.state = 1;

        break;
    case DLG_MANUAL:
        this->MoveMainUI(nMoveType);

        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonManual.state = 1;
		m_clVisionStaticCcd[m_nCurrentUnit].m_bDrag = false;
		m_clVisionStaticCam[m_nCurrentUnit].m_bDrag = false;

#ifdef ON_LINE_MIL
		if (g_pCarAABonderDlg->m_bCamState[m_nCurrentUnit] == VIDEO_CAM)
		{
			m_clVisionStaticCam[m_nCurrentUnit].DrawCamAlign(999);
		}
		else
		{
			m_clVisionStaticCcd[m_nCurrentUnit].DrawRectSfr(999);
			/*m_clVisionStaticCcd[m_nCurrentUnit].SetSfrRoi();
			m_clVisionStaticCcd[m_nCurrentUnit].SetFovRoi();

			if (g_pCarAABonderDlg->m_clVisionStaticCcd[m_nCurrentUnit].m_FovSetMode == true)
			{
				m_clVisionStaticCcd[m_nCurrentUnit].DrawRectFov(999);
			}
			else
			{
				m_clVisionStaticCcd[m_nCurrentUnit].DrawRectSfr(999);
			}*/
		}

#endif

        m_clManualDlg.SetUnit(m_nCurrentUnit);
        m_clManualDlg.ShowWindow(SW_SHOW);
        break;
    case DLG_TEACH:
		//g_clVision.ClearOverlay(m_nCurrentUnit);
		//g_clVision.DrawOverlayAll(m_nCurrentUnit);
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonTeaching.state = 1;

        m_clTeachingDlg.SetUnit(m_nCurrentUnit);
        m_clTeachingDlg.ShowWindow(SW_SHOW);


#ifdef ON_LINE_MIL
		if (g_pCarAABonderDlg->m_bCamState[m_nCurrentUnit] == VIDEO_CAM)
		{
			m_clVisionStaticCam[m_nCurrentUnit].DrawCamAlign(999);
		}
		else
		{
			m_clVisionStaticCcd[m_nCurrentUnit].DrawRectSfr(999);

		}

#endif

        break;
    case DLG_CCD:
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonCcd.state = 1;

		CAMChangeHandler(0, VIDEO_CCD);

#ifdef ON_LINE_MIL
		if (g_pCarAABonderDlg->m_bCamState[m_nCurrentUnit] == VIDEO_CAM)
		{
			m_clVisionStaticCam[m_nCurrentUnit].DrawCamAlign(999);
		}
		else
		{
			m_clVisionStaticCcd[m_nCurrentUnit].DrawRectSfr(999);

			/*m_clVisionStaticCcd[m_nCurrentUnit].SetSfrRoi();
			m_clVisionStaticCcd[m_nCurrentUnit].SetFovRoi();

			if (g_pCarAABonderDlg->m_clVisionStaticCcd[m_nCurrentUnit].m_FovSetMode == true)
			{
				m_clVisionStaticCcd[m_nCurrentUnit].DrawRectFov(999);
			}
			else
			{
				m_clVisionStaticCcd[m_nCurrentUnit].DrawRectSfr(999);
			}*/
		}

#endif
        m_clCcdDlg.SetUnit(m_nCurrentUnit);
        m_clCcdDlg.ShowWindow(SW_SHOW);
        break;
    case DLG_DIO:
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonDio.state = 1;
        m_clDioDlg.SetUnit(0);//(m_nCurrentUnit);
        m_clDioDlg.ShowWindow(SW_SHOW);
        break;
    case DLG_LIGHT:
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonLight.state = 1;

        m_clLightDlg.SetUnit(m_nCurrentUnit);
        m_clLightDlg.ShowWindow(SW_SHOW);
        break;
    case DLG_ALARM:
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clConfigDlg.ShowWindow(SW_HIDE);

        m_clColorButtonAlarm.state = 1;

        m_clAlarmDlg.SetUnit(m_nCurrentUnit);
		m_clAlarmDlg.ShowCurrentPage();
		m_clAlarmDlg.ShowAlarmList_N();
		
        m_clAlarmDlg.ShowWindow(SW_SHOW);
        break;
    case DLG_CONFIG:
        this->MoveMainUI(nMoveType);

        m_clManualDlg.ShowWindow(SW_HIDE);
        m_clTeachingDlg.ShowWindow(SW_HIDE);
        m_clCcdDlg.ShowWindow(SW_HIDE);
        m_clDioDlg.ShowWindow(SW_HIDE);
        m_clLightDlg.ShowWindow(SW_HIDE);
        m_clAlarmDlg.ShowWindow(SW_HIDE);

        m_clColorButtonConfig.state = 1;

        m_clConfigDlg.SetUnit(m_nCurrentUnit);
        m_clConfigDlg.ShowWindow(SW_SHOW);
        break;
    }
	m_clColorButtonMain.Invalidate();
	m_clColorButtonManual.Invalidate();
	m_clColorButtonTeaching.Invalidate();
	m_clColorButtonCcd.Invalidate();
	m_clColorButtonDio.Invalidate();
	m_clColorButtonLight.Invalidate();
	m_clColorButtonAlarm.Invalidate();
	m_clColorButtonConfig.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	�������� ��ư ���� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowHomeState(int nUnit)
{
    if (m_clColorButtonStartingPoint[nUnit].state == 0)
        m_clColorButtonStartingPoint[nUnit].state = 1;
    else
        m_clColorButtonStartingPoint[nUnit].state = 0;
}

//-----------------------------------------------------------------------------
//
//	�����غ� ��ư ���� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowAutoReadyState(int nUnit)
{
    if (m_clColorButtonAutoReady[nUnit].state == 0)
        m_clColorButtonAutoReady[nUnit].state = 1;
    else
        m_clColorButtonAutoReady[nUnit].state = 0;

	m_clColorButtonAutoReady[nUnit].Invalidate();
}

//-----------------------------------------------------------------------------
//
//	���ŵ� ���ڵ� ���� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowBarcode(int nUnit)
{
    g_clMesCommunication[nUnit].m_sMesLotID.Format(_T("%s"), g_clTaskWork[nUnit].m_szChipID);//ShowBarcode

    m_clColorStaticBcrVal[nUnit].SetWindowText(g_clTaskWork[nUnit].m_szChipID);
	m_clColorStaticBcrVal[nUnit].Invalidate();
}

//-----------------------------------------------------------------------------
//
//	���귮 ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowOutputCount(int nUnit)
{
    TCHAR szData[SIZE_OF_100BYTE];

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), g_clTaskWork[nUnit].m_nTotalWorkCount);

    m_clColorStaticOutputVal[nUnit].SetWindowText(szData);
}

//-----------------------------------------------------------------------------
//
//	������ ��뷮 ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowPinCount(int nUnit)
{
    TCHAR szData[SIZE_OF_100BYTE];

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%d"), g_clTaskWork[nUnit].m_nUsePinCount);

    m_clColorStaticPinVal[nUnit].SetWindowText(szData);
}

//-----------------------------------------------------------------------------
//
//	LINK, CCD ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowIconState(int nUnit)
{
    bool bState;

    bState = g_clLaonGrabberWrapper[nUnit].GetDeviceOpen();
    if (bState != m_bPrevLinkState[nUnit])
    {
        if (bState == true)
            m_clButtonExLink[nUnit].state = 3;
        else
            m_clButtonExLink[nUnit].state = 1;

        m_bPrevLinkState[nUnit] = bState;
    }

    bState = g_clLaonGrabberWrapper[nUnit].GetCcdState();
    if (bState != m_bPrevCcdState[nUnit])
    {
        if (bState == true)
            m_clButtonExCcd[nUnit].state = 1;
        else
            m_clButtonExCcd[nUnit].state = 0;

		m_clButtonExCcd[nUnit].Invalidate();
        m_bPrevCcdState[nUnit] = bState;
    }
}

//-----------------------------------------------------------------------------
//
//	OK, NG���� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowOkNgState(int nUnit, int _FINAL)
{
    m_clColorButtonComplOk[nUnit].state = 0;
    m_clColorButtonComplNg[nUnit].state = 0;
    if (_FINAL == 1)			// OK
    {
        m_clColorButtonComplOk[nUnit].state = 1;
    }
    else if (_FINAL == 2)		// NG
    {
        m_clColorButtonComplNg[nUnit].state = 1;
    }
    m_clColorButtonComplOk[nUnit].Invalidate();
    m_clColorButtonComplNg[nUnit].Invalidate();
}


//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::StartHomeProcess(int nUnit)
{
    TCHAR szLog[SIZE_OF_1K];
    CString sMsg;
    int i = 0;
#ifdef ON_LINE_MIL
	g_clVision.ClearOverlay(nUnit);
#endif
    //! Main PC ��� ���� Ȯ��(Main ������ ����üũ)
//#ifdef ON_LINE_SOCKET	
//    if (m_bConnectMain == false)
//    {
//        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] MAIN PC�� ��� ������ ���� �ʾҽ��ϴ�."));
//        AddLog(szLog, 1, nUnit);
//		g_ShowMsgPopup(_T("ERROR"), szLog, RGB_COLOR_RED);
//        return false;
//    }
//
//#ifdef ON_LINE_SOCKET
//	sMsg.Format(_T("#AA%d@PCB&INIT$"), (g_clSysData.m_nUnitNo * 1) + nUnit + 1);		//Auto Pause
//	this->SendDataToAAMain(nUnit, sMsg);
//
//
//#endif
   /* sMsg.Format(_T("#AA%d@PCB_ITF&CHK$"), nUnit + g_clSysData.m_nUnitNo + 1);
    g_pCarAABonderDlg->SendDataToAAMain(nUnit, sMsg);
    Sleep(50);*/
//#endif
	g_clMotorSet.m_bOqaOrgFlag = false;
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	for (i = 0; i < MAX_MOTOR_COUNT; i++) 
	{
		if (g_clMotorSet.AmpEnable(nUnit, i) == false)
		{
			sMsg.Format(_T("[FAIL] [%s] ���� SERVO ON ���� ����"), MOTOR_NAME[i]);
			AddLog((TCHAR*)(LPCTSTR)sMsg, 1, nUnit);
		}
	}
#endif

    // �����尡 �������̰�
    if (m_clActiveAlignThread[nUnit].GetThreadRunning() == true)
    {
        // �Ͻ� ���� ���¸�
        if (m_clActiveAlignThread[nUnit].GetThreadPause() == true)
        {
            /*g_clTaskWork[nUnit].m_nAutoFlag = MODE_AUTO;
            m_clActiveAlignThread[nUnit].SetThreadPause(false);
            g_clTaskWork[nUnit].m_dwTickCount = GetTickCount();

            return true;*/

            _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] �Ͻ����� ���¿��� ����� �� �����ϴ�."));
            AddLog(szLog, 1, nUnit);

            return false;
        }
        else
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("�����尡 ���� ���Դϴ�."));
            AddLog(szLog, 1, nUnit);
            return false;
        }
    }
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
    for (i = 0; i < MAX_MOTOR_COUNT; i++)
    {
        if (g_clMotorSet.GetStopAxis(nUnit, i) == false)
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("���� �������Դϴ�."));
            AddLog(szLog, 1, nUnit);

            return false;
        }
    }
#endif
    g_clTaskWork[nUnit].m_nStartStep = 10000;
    g_clTaskWork[nUnit].m_nEndStep = 20000;
    g_clTaskWork[nUnit].m_nCurrentPcbStep = 10000;

    if (m_clActiveAlignThread[nUnit].StartThread() == false)
    {
        return false;
    }

	m_clColorButtonAutoReady[nUnit].state = 0;
	m_clColorButtonAutoRun[nUnit].state = 0;
	m_clColorButtonAutoPause[nUnit].state = 0;
	m_clColorButtonAutoStop[nUnit].state = 0;

	m_clColorButtonAutoReady[nUnit].Invalidate();
	m_clColorButtonAutoRun[nUnit].Invalidate();
	m_clColorButtonAutoPause[nUnit].Invalidate();
	m_clColorButtonAutoStop[nUnit].Invalidate();

    return true;
}

//-----------------------------------------------------------------------------
//
//	���� �غ�
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::StartAutoReadyProcess(int nUnit)
{
    TCHAR szLog[SIZE_OF_1K];
    CString sMsg;
    int i;
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
    // ���� ���� ���� üũ
    if (g_clMotorSet.GetHomeState(nUnit) == false)
    {
        _stprintf_s(szLog, SIZE_OF_1K, _T("[READY] ���� ���Ͱ� �Ϸ���� �ʾҽ��ϴ�."));
        AddLog(szLog, 1, nUnit);

        return false;
    }
#else
	if (g_clMotorSet.m_bOqaOrgFlag == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] ���� ���Ͱ� �Ϸ���� �ʾҽ��ϴ�."));
		AddLog(szLog, 1, nUnit);

		return false;
	}
#endif
    //! Main PC ��� ���� Ȯ��(Main ������ ����üũ)
#ifdef ON_LINE_SOCKET
    /*if (m_bConnectMain == false)
    {
        _stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] MAIN ��� ���� �����ʾҽ��ϴ�."));
        AddLog(szLog, 1, nUnit);
		g_ShowMsgPopup(_T("ERROR"), szLog, RGB_COLOR_RED);
        return false;
    }*/

	//sMsg.Format(_T("#AA%d@ALARM&CLR$"), (g_clSysData.m_nUnitNo * 1) + nUnit + 1);
	//this->SendDataToAAMain(nUnit, sMsg);
	//Sleep(100);
	//sMsg.Format(_T("#AA%d@ULD&ESC$"), (g_clSysData.m_nUnitNo * 1) + nUnit + 1);
	//this->SendDataToAAMain(nUnit, sMsg);
	//Sleep(100);
	//sMsg.Format(_T("#AA%d@PCB&INIT$"), (g_clSysData.m_nUnitNo * 1) + nUnit + 1);		//Auto Pause
	//this->SendDataToAAMain(nUnit, sMsg);
#endif

    // ������ ���������� üũ	
    if (m_clActiveAlignThread[nUnit].GetThreadRunning() == true)
    {
        // �Ͻ� ���� ���¸�
        if (m_clActiveAlignThread[nUnit].GetThreadPause() == true)
        {
            /*m_clActiveAlignThread[nUnit].SetThreadPause(false);
            g_clTaskWork[nUnit].m_dwTickCount = GetTickCount();

            return true;*/

            _stprintf_s(szLog, SIZE_OF_1K, _T("[READY] �Ͻ����� ���¿��� ����� �� �����ϴ�."));
            AddLog(szLog, 1, nUnit);

            return false;
        }
        else
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[READY] �����尡 ���� ���Դϴ�."));
            AddLog(szLog, 1, nUnit);
        }

        return false;
    }
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
    // ���� ���� ���� üũ
    for (i = 0; i < MAX_MOTOR_COUNT; i++)
    {
        if (g_clMotorSet.GetStopAxis(nUnit, i) == false)
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[READY] ����(%s) �������Դϴ�."), MOTOR_NAME[i]);
            AddLog(szLog, 1, nUnit);

            return false;
        }
    }
#endif
    for (int i = PCB_ITF_CHK; i < MAX_PCB_IF_COUNT; i++)
        g_clTaskWork[nUnit].m_bPcbSocketIF[i] = false;

    for (int i = ULD_ITF_CHK; i < MAX_ULD_IF_COUNT; i++)
        g_clTaskWork[nUnit].m_bUldSocketIF[i] = false;

    for (i = 0; i < MAX_MTF_COUNT; i++)
    {
        g_clTaskWork[nUnit].m_stSfrInsp.fSFR_AVR_N4[g_clTaskWork[nUnit].m_nDrawBarStep - 1][i] = 0.0;
        g_clTaskWork[nUnit].m_stSfrInsp._fAverSfr[i] = 0.0;
    }
	m_clColorButtonAutoRun[nUnit].state = 0;
	m_clColorButtonAutoPause[nUnit].state = 0;
	m_clColorButtonAutoStop[nUnit].state = 0;
	m_clColorButtonAutoReady[nUnit].state = 0;
	m_clColorButtonAutoReady[nUnit].Invalidate();
	m_clColorButtonAutoRun[nUnit].Invalidate();
	m_clColorButtonAutoPause[nUnit].Invalidate();
	m_clColorButtonAutoStop[nUnit].Invalidate();

    g_clTaskWork[nUnit].m_nStartStep = 20000;
    g_clTaskWork[nUnit].m_nEndStep = 30000;
    g_clTaskWork[nUnit].m_nCurrentPcbStep = 20000;
   

    if (m_clActiveAlignThread[nUnit].StartThread() == false)
    { 
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
		g_clDioControl.SetTowerLamp(LAMP_RED, true);
#else
		g_clDioControl.EziSetEziTowerLamp(0, LAMP_RED, true);
#endif
        g_clTaskWork[nUnit].m_nAutoFlag = MODE_STOP;
        return false;
    }
    //m_clColorButtonStartingPoint[nUnit].ChangeColor(0);
	

    return true;
}

//-----------------------------------------------------------------------------
//
//	�ڵ� ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::StartAutoProcess(int nUnit)
{
    TCHAR szLog[SIZE_OF_1K];
    CString sMsg = _T("");
    CString sData = _T("");
    int i;

    // ���� ���� ���� üũ
    if (g_clMotorSet.GetHomeState(nUnit) == false)
    {
        _stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] ���� ���Ͱ� �Ϸ���� �ʾҽ��ϴ�."));
        AddLog(szLog, 1, false);

        return false;
    }
	g_clTaskWork[nUnit].m_nEmissionRun = false;
	//if (g_clAdo.GetAccessDbConnect() == false)
	//{
	//	//_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\EEPROM_DATABASE_1ST.mdb"), BASE_DATA_PATH);	//1ȣ��� 1st 2ȣ��� 2st�� ���ض�
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] EEPROM DB �����ӻ���.%s\\EEPROM_DATABASE_1ST.mdb Ȯ�����ּ���") , BASE_DATA_PATH);
	//	AddLog(szLog, 1, nUnit);
	//	g_ShowMsgPopup(_T("ERROR"), _T("[AUTO] EEPROM DB �����ӻ����Դϴ�."), RGB_COLOR_RED);
	//	return false;
	//}
	/*if (g_clTaskWork[nUnit].m_nAutoFlag != MODE_READY)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] ���� �غ� �Ϸ���� �ʾҽ��ϴ�."));
		AddLog(szLog, 1, false);

		return false;
	}*/
	//g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_READY;
    //! Main PC ��� ���� Ȯ��(Main ������ ����üũ)
#ifdef ON_LINE_SOCKET
    /*if (m_bConnectMain == false)
    {
        _stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MAIN PC�� ��� ������ ���� �ʾҽ��ϴ�."));
        AddLog(szLog, 1, nUnit);
		g_ShowMsgPopup(_T("ERROR"), szLog, RGB_COLOR_RED);
        return false;
    }

    for (int i = PCB_ITF_CHK; i < MAX_PCB_IF_COUNT; i++)
    { 
        g_clTaskWork[nUnit].m_bPcbSocketIF[i] = false;
    }

    for (int i = LEN_ITF_CHK; i < MAX_LENS_IF_COUNT; i++)
    {
        g_clTaskWork[nUnit].m_bLensSocketIF[i] = false;
    }*/

   // sMsg.Format(_T("#AA%d@PCB_ITF&CHK$"), nUnit + g_clSysData.m_nUnitNo + 1);
    //g_pCarAABonderDlg->SendDataToAAMain(nUnit, sMsg);
   // Sleep(50);
	 

#endif
   // �����غ� �������� üũ
	if (g_clTaskWork[nUnit].m_nAutoFlag == MODE_STOP)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] �����غ� �Ϸ���� �ʾҽ��ϴ�."));
		AddLog(szLog, 1, nUnit, false);

		return false;
	}
    // ������ ���������� üũ
    if (m_clActiveAlignThread[nUnit].GetThreadRunning() == true)
    {
        // �Ͻ� ���� ���¸�
        if (m_clActiveAlignThread[nUnit].GetThreadPause() == true)
        {

			///*if (m_clLensAxisThread[nUnit].GetThreadPause() == true)
			//{
			//	g_clTaskWork[nUnit].m_nCurrentLensStep = abs(g_clTaskWork[nUnit].m_nCurrentLensStep);
			//	m_clLensAxisThread[nUnit].bLensWorkRun = true;
			//	m_clLensAxisThread[nUnit].SetThreadPause(false);
			//}*/
			if (g_clDioControl.PcbSensorCheck(nUnit, true) == true)
			{
				if (g_clLaonGrabberWrapper[nUnit].m_pBoard->IsGrabStarted() == false)
				{
					_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Open Start"));
					AddLog(szLog, 0, nUnit);
					if (g_clLaonGrabberWrapper[nUnit].OpenDevice() == false)
					{
						_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Open Fail"));
						AddLog(szLog, 0, nUnit);
						return false;
					}
					else
					{
						_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Open End"));
						AddLog(szLog, 0, nUnit);
						Sleep(600);
						_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Grab Start"));
						AddLog(szLog, 0, nUnit);
						if (g_clLaonGrabberWrapper[nUnit].StartGrabCcdProcess() == false)
						{
							_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Grab Start Fail"));
							AddLog(szLog, 0, nUnit);
							return false;
						}
						_stprintf_s(szLog, SIZE_OF_1K, _T("[CCD] Device Grab Start END"));
						AddLog(szLog, 0, nUnit);
					}

				}
			}
            //_stprintf_s(g_clTaskWork[nUnit].m_szLotID, SIZE_OF_100BYTE, _T("%s"), g_clTaskWork[nUnit].m_szLotID);//200718 �ּ�ó�� ����???????
            //this->ShowBarcode();

			g_clTaskWork[nUnit].m_nCurrentPcbStep = abs(g_clTaskWork[nUnit].m_nCurrentPcbStep);
			//g_clTaskWork[nUnit].m_nCurrentLensStep = abs(g_clTaskWork[nUnit].m_nCurrentLensStep);

			g_clDioControl.EziSetEziTowerLamp(0, LAMP_GREEN, true);

            g_clTaskWork[nUnit].m_nAutoFlag = MODE_AUTO;
            // Step ������Ʈ
            g_clTaskWork[nUnit].m_dwLensTickCount = GetTickCount();
            g_clTaskWork[nUnit].m_dwPcbTickCount = GetTickCount();
            m_clActiveAlignThread[nUnit].SetThreadPause(false);

            //m_clColorButtonStartingPoint[nUnit].ChangeColor(0);
            m_clColorButtonAutoReady[nUnit].state = 0;
            m_clColorButtonAutoRun[nUnit].state = 1;
            m_clColorButtonAutoPause[nUnit].state = 0;
            m_clColorButtonAutoStop[nUnit].state = 0;

			m_clColorButtonAutoReady[nUnit].Invalidate();
			m_clColorButtonAutoRun[nUnit].Invalidate();
			m_clColorButtonAutoPause[nUnit].Invalidate();
			m_clColorButtonAutoStop[nUnit].Invalidate();


            return true;
        }
        else
        {

            _stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] �ڵ������� �Դϴ�."));
            AddLog(szLog, 1, nUnit, false);

            return false;
        }
	}
	else
	{
		//if (g_clDioControl.PcbSensorCheck(nUnit, true) == true)
		//{
		//	if (g_clTaskWork[nUnit].m_nEmission == 1)
		//	{
		//		g_clTaskWork[nUnit].m_nEmissionRun = true;
		//		g_clMesCommunication[nUnit].m_nMesFinalResult = 1;	//���� ��ǰ ����
		//		g_clTaskWork[nUnit].m_nCurrentPcbStep = 119200;
		//		//��ǰ ����
		//	}
		//	else if (g_clTaskWork[nUnit].m_nEmission == 2)
		//	{
		//		g_clTaskWork[nUnit].m_nEmissionRun = true;
		//		g_clMesCommunication[nUnit].m_nMesFinalResult = 0;	//���� NG ����
		//		g_clTaskWork[nUnit].m_nCurrentPcbStep = 119200;
		//		//NG ����
		//	}
		//	
		//}
	}


	///*if (m_clLensAxisThread[nUnit].GetThreadRunning() == true)
	//{
	//	if (m_clLensAxisThread[nUnit].GetThreadPause() == true)
	//	{
	//		g_clTaskWork[nUnit].m_nCurrentLensStep = abs(g_clTaskWork[nUnit].m_nCurrentLensStep);
	//		m_clLensAxisThread[nUnit].bLensWorkRun = true;
	//		m_clLensAxisThread[nUnit].SetThreadPause(false);
	//	}
	//}*/
	
    // ���� ���������� üũ
    for (i = 0; i < MAX_MOTOR_COUNT; i++)
    {
        if (g_clMotorSet.GetStopAxis(nUnit, i) == false)
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] ����(%s) �������Դϴ�."), MOTOR_NAME[i]);
            AddLog(szLog, 1, false);

            return false;
        }
    }

    // �����غ� �������� üũ
	if (g_clTaskWork[nUnit].m_nAutoFlag == MODE_STOP)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] �����غ� �Ϸ���� �ʾҽ��ϴ�."));
		AddLog(szLog, 1, nUnit, false);

		return false;
	}
	if (g_clTaskWork[nUnit].m_nAutoFlag == MODE_AUTO)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] �ڵ����� ���Դϴ�."));
		AddLog(szLog, 1, nUnit, false);

		return false;
	}

  //  if (g_pCarAABonderDlg->ConnectAAMain() == false)
  //  {
  //      _stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MAIN PC�� ������� �ʾҽ��ϴ�."));
  //      //AddLog(szLog, 1, 999, false);
  //      AddLog(szLog, 999, 0);
  //      AddLog(szLog, 999, 1);
		//g_ShowMsgPopup(_T("ERROR"), szLog, RGB_COLOR_RED);
  //      return false;
  //  }
	 
	//if (g_clDioControl.PcbSocketCheck(nUnit, true) == false)
	//{
	//	if (g_clDioControl.PcbGrip(nUnit, false, true) == true)
	//	{// Grip ����
	//		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB GRIP ���� ����"));
	//		AddLog(szLog, 0, nUnit);
	//	}
	//	else
	//	{
	//		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB GRIP ���� ����"));
	//		AddLog(szLog, 1, nUnit, true);
	//		return false;
	//	}
	//}
    m_nCurrentDlg = 0;

	///*m_clColorStaticBcrVal[nUnit].GetWindowText(sData);
	//if (sData.GetLength() < 1)
	//{
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] ���ڵ� ������ Ȯ���ϼ���:%s"), sData);
	//	AddLog(szLog, 1, nUnit, false);
	//	sData.Empty();
	//	return false;
	//}*/
    // ����ǰ ����
   // if (g_clModelData[nUnit].m_nCOMPLETE_MODE == 1) 
    //{
        
        //else
        //{
        //    _stprintf_s(g_clTaskWork[nUnit].m_szChipID, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)sData);
        //    g_clMesCommunication[nUnit].m_sMesLotID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sData);
        //    g_clMandoInspLog[nUnit].m_sBarcodeID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sData);

        //    // DB���� ���ڵ� ������ ��ȸ
        //    if (this->SelectAAData(nUnit, sData) == false)
        //    {
        //        sMsg.Format(_T("[AUTO] %s ���ڵ� AA ������ �������� �ʽ��ϴ�. �����Ͻðڽ��ϱ�?"), (TCHAR*)(LPCTSTR)sData);
        //        if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
        //            return false;
        //    }
        //}
  //  }
    //else
    //{
    //    m_clColorStaticBcrVal[nUnit].GetWindowText(sData);

    //    g_clMesCommunication[nUnit].m_sMesLotID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sData);
    //    g_clMandoInspLog[nUnit].m_sBarcodeID.Format(_T("%s"), (TCHAR*)(LPCTSTR)sData);
    //}


	g_clTaskWork[nUnit].m_nStartStep = 30000;
	g_clTaskWork[nUnit].m_nEndStep = 120000;
	
	//g_clTaskWork[nUnit].m_nCurrentLensStep = 100000;
	//
	

	g_clTaskWork[nUnit].m_nAutoFlag = MODE_AUTO;

#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	g_clDioControl.SetTowerLamp(LAMP_GREEN, true);
#else
	g_clDioControl.EziSetEziTowerLamp(0, LAMP_GREEN, true);
#endif
	g_clMandoInspLog[nUnit].m_bInspRes = true;
	

    

	if (m_clActiveAlignThread[nUnit].StartThread() == false)
	{
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
		g_clDioControl.SetTowerLamp(LAMP_RED, true);
#else
		g_clDioControl.EziSetEziTowerLamp(0, LAMP_RED, true);
#endif
		g_clTaskWork[nUnit].m_nAutoFlag = MODE_STOP;
		return false;
	}
	

    //m_clColorButtonStartingPoint[nUnit].ChangeColor(0);
    m_clColorButtonAutoReady[nUnit].state = 0;
    m_clColorButtonAutoRun[nUnit].state = 1;
    m_clColorButtonAutoPause[nUnit].state = 0;
    m_clColorButtonAutoStop[nUnit].state = 0;

    //m_clDIOThread.SetTowerLamp(2, true);

    AddLog(_T("[AUTO] �ڵ� ������ ����"), 0, nUnit);

    m_clColorButtonAutoReady[nUnit].Invalidate();
    m_clColorButtonAutoRun[nUnit].Invalidate();
    m_clColorButtonAutoPause[nUnit].Invalidate();
    m_clColorButtonAutoStop[nUnit].Invalidate();

	sData.Empty();
    return true;
}

//-----------------------------------------------------------------------------
//
//	�Ͻ� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::PauseAutoProcess(int nUnit)
{
    if (m_clActiveAlignThread[nUnit].GetThreadRunning() == true)
    {
        m_clActiveAlignThread[nUnit].SetThreadPause(true);


        g_clTaskWork[nUnit].m_nAutoFlag = MODE_PAUSE;

		g_clTaskWork[nUnit].m_bInputSignal = false;
		g_clTaskWork[nUnit].m_bOutputSignal = false;
		
        //m_clColorButtonStartingPoint[nUnit].state = 0;
        m_clColorButtonAutoReady[nUnit].state = 0;
        m_clColorButtonAutoRun[nUnit].state = 0;
        m_clColorButtonAutoPause[nUnit].state = 1;
        m_clColorButtonAutoStop[nUnit].state = 0;

        AddLog(_T("[PAUSE] �ڵ� ������ �Ͻ� ����"), 0, nUnit);
        m_clColorButtonAutoReady[nUnit].Invalidate();
        m_clColorButtonAutoRun[nUnit].Invalidate();
        m_clColorButtonAutoPause[nUnit].Invalidate();
        m_clColorButtonAutoStop[nUnit].Invalidate();
    }

	
}

//-----------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::StopAutoProcess(int nUnit)
{
    

    g_clMotorSet.StopAxisAll(nUnit);

    if (m_clActiveAlignThread[nUnit].GetThreadRunning() == true)
    {
        m_clActiveAlignThread[nUnit].EndThread();
    }

	if (m_clCustomThread[nUnit].GetThreadRunning() == true)
	{
		m_clCustomThread[nUnit].EndThread(); 
	}
	
	g_clLaonGrabberWrapper[nUnit].CloseDevice();

    m_clColorButtonAutoReady[nUnit].state = 0;
    m_clColorButtonAutoRun[nUnit].state = 0;
    m_clColorButtonAutoPause[nUnit].state = 0;
    m_clColorButtonAutoStop[nUnit].state = 1;

    //m_clDIOThread.SetTowerLamp(1, true);

    

    m_clColorButtonAutoReady[nUnit].Invalidate();
    m_clColorButtonAutoRun[nUnit].Invalidate();
    m_clColorButtonAutoPause[nUnit].Invalidate();
    m_clColorButtonAutoStop[nUnit].Invalidate();

	g_clTaskWork[nUnit].m_bInputSignal = false;
	g_clTaskWork[nUnit].m_bOutputSignal = false;
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	g_clDioControl.SetTowerLamp(LAMP_RED, true);
	g_clMotorSet.StopAxisAll(nUnit);
#else
	g_clDioControl.EziSetEziTowerLamp(0, LAMP_RED, true);
#endif
	

	g_clTaskWork[nUnit].m_nAutoFlag = MODE_STOP;
	AddLog(_T("[STOP] �ڵ� ���� ����"), 0, nUnit);
    return true;
}



//-----------------------------------------------------------------------------
//
//	UNIT���� ��Ʈ�� �̵�
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::MoveMainUI(int nType)
{
    CString szKey = _T("");
    CPoint clPos;
    int i;

    switch (nType)
    {
    case SHOW_ALL:
		for (i = 0; i < MAX_UNIT_COUNT; i++)//// for (i = 0; i < 2; i++)//MAX_UNIT_COUNT
        {
            szKey.Format(_T("IDC_STATIC_MAIN_VERSION%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_VERSION1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_VERSION1 + i)->ShowWindow(SW_SHOW);
            }
			szKey.Format(_T("IDC_BUTTON_MAIN_CCD%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_MAIN_CCD1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_BUTTON_MAIN_CCD1 + i)->ShowWindow(SW_SHOW);
			}
			szKey.Format(_T("IDC_BUTTON_MAIN_DOOR%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				//GetDlgItem(IDC_BUTTON_MAIN_DOOR1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				//GetDlgItem(IDC_BUTTON_MAIN_DOOR1 + i)->ShowWindow(SW_SHOW);
			}


			
			szKey.Format(_T("IDC_BUTTON_MAIN_MES%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				//GetDlgItem(IDC_BUTTON_MAIN_MES1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				//GetDlgItem(IDC_BUTTON_MAIN_MES1 + i)->ShowWindow(SW_SHOW);
			}

            szKey.Format(_T("IDC_STATIC_MAIN_CCD%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_CCD1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                //GetDlgItem(IDC_STATIC_MAIN_CCD1 + i)->ShowWindow(SW_SHOW);
				
            }

			szKey.Format(_T("IDC_STATIC_MAIN_CAM%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_STATIC_MAIN_CAM1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				//GetDlgItem(IDC_STATIC_MAIN_CAM1 + i)->ShowWindow(SW_SHOW);
			}
			
			szKey.Format(_T("IDC_STATIC_MAIN_CURR_MODE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1 + i)->ShowWindow(SW_SHOW);
			}

			/*szKey.Format(_T("IDC_BUTTON_MAIN_CCD_CHANGE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1 + i)->ShowWindow(SW_SHOW);
			}*/


			
			/*szKey.Format(_T("IDC_BUTTON_MAIN_CAM_CHANGE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1 + i)->ShowWindow(SW_SHOW);
			}*/

			
#ifndef GRAPH_NO_USE
            szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_1ST_AA%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1 + i)->ShowWindow(SW_SHOW);
                if (m_pGraphWnd[i][0] != NULL)
                {
                    m_pGraphWnd[i][0]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                    m_pGraphWnd[i][0]->ShowWindow(SW_SHOW);
                }
            }

            szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_2ND_AA%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1 + i)->ShowWindow(SW_SHOW);
                if (m_pGraphWnd[i][1] != NULL)
                {
                    m_pGraphWnd[i][1]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                    m_pGraphWnd[i][1]->ShowWindow(SW_SHOW);
                }
            }
#endif
            szKey.Format(_T("IDC_STATIC_MAIN_BAR_GRAPH%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1 + i)->ShowWindow(SW_SHOW);
                if (m_pGraphWnd[i][2] != NULL)
                {
                    m_pGraphWnd[i][2]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                    m_pGraphWnd[i][2]->ShowWindow(SW_SHOW);
                }
            }

            szKey.Format(_T("IDC_STATIC_MAIN_BCR%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_BCR1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                //GetDlgItem(IDC_STATIC_MAIN_BCR1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_BCR_VAL%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1 + i)->ShowWindow(SW_SHOW);
            }

			/*szKey.Format(_T("IDC_STATIC_MAIN_MODEL_TYPE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1 + i)->ShowWindow(SW_SHOW);
			}*/
			//master
			szKey.Format(_T("IDC_BUTTON_MASTER_CHAGE_MODE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				//GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1 + i)->ShowWindow(SW_SHOW);
			}

			//Epoxy Complete
			/*szKey.Format(_T("IDC_BUTTON_EPOXY_COMPLETE%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1 + i)->ShowWindow(SW_SHOW);
			}*/

			//LENS LOADING �Ϸ�
			szKey.Format(_T("IDC_BUTTON_LENS_LOADING%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_BUTTON_LENS_LOADING1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_BUTTON_LENS_LOADING1 + i)->ShowWindow(SW_HIDE);

				
			}
			
			//STAIN
			szKey.Format(_T("IDC_STATIC_MAIN_STAIN_VIEW_AA%d"), i + 1);
			if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
			{
				GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
				GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1 + i)->ShowWindow(SW_SHOW);
			}

			

			
			//
            szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_OUTPUT1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_OUTPUT1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT_VAL%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_PIN%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_PIN1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_PIN1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_PIN_VAL%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_FPS%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_FPS1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_FPS1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_STATIC_MAIN_FPS_VAL%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_STARTING_POINT%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_READY%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_RUN%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_OK%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1 + i)->ShowWindow(SW_SHOW);
            }

            /*szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_EMISSION%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1 + i)->ShowWindow(SW_SHOW);
            }*/

            szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_STOP%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_PAUSE%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1 + i)->ShowWindow(SW_SHOW);
            }

            szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_NG%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1 + i)->ShowWindow(SW_SHOW);
            }

           /* szKey.Format(_T("IDC_STATIC_MAIN_LOG%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_STATIC_MAIN_LOG1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_STATIC_MAIN_LOG1 + i)->ShowWindow(SW_SHOW);
            }*/

            szKey.Format(_T("IDC_LIST_MAIN_LOG%d"), i + 1);
            if (m_clCtrlPos[i].Lookup(szKey, clPos) == TRUE)
            {
                GetDlgItem(IDC_LIST_MAIN_LOG1 + i)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                GetDlgItem(IDC_LIST_MAIN_LOG1 + i)->ShowWindow(SW_SHOW);
            }
        }
        //m_clColorStaticMsg.ShowWindow(SW_SHOW);
        break;
    case SHOW_UNIT1:
        //GetDlgItem(IDC_STATIC_MAIN_LOGO2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_TITLE2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_VERSION2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_INFO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_MODEL_NO2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_LOT_ID2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_CCD_ID2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_LINK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_CCD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_DOOR2)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_BUTTON_MAIN_MES2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_CCD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_CAM2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_MAIN_CURR_MODE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE2)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE2)->ShowWindow(SW_HIDE);

		//
		//
		GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA2)->ShowWindow(SW_HIDE);//���⼭ �״´�
		GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH2)->ShowWindow(SW_HIDE);
		if (MAX_UNIT_COUNT > 1)
		{
			
			if (m_pGraphWnd[1][0] != NULL)
				m_pGraphWnd[1][0]->ShowWindow(SW_HIDE);
			
			if (m_pGraphWnd[1][1] != NULL)
				m_pGraphWnd[1][1]->ShowWindow(SW_HIDE);
			
			if (m_pGraphWnd[1][2] != NULL)
				m_pGraphWnd[1][2]->ShowWindow(SW_HIDE);
		}
        GetDlgItem(IDC_STATIC_MAIN_BCR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_BCR_VAL2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_OUTPUT2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_PIN2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_PIN_VAL2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_FPS2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_FPS_VAL2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LENS_LOADING2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA2)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_LOG2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LIST_MAIN_LOG2)->ShowWindow(SW_HIDE);

       /* szKey.Format(_T("IDC_STATIC_MAIN_LOGO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOGO1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOGO1)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_STATIC_MAIN_TITLE1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_TITLE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_TITLE1)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_STATIC_MAIN_INFO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_INFO1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_INFO1)->ShowWindow(SW_SHOW);
        }*/

        szKey.Format(_T("IDC_STATIC_MAIN_VERSION1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_VERSION1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_VERSION1)->ShowWindow(SW_SHOW);
        }

       /* szKey.Format(_T("IDC_STATIC_MAIN_MODEL_NO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_MODEL_NO1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_MODEL_NO1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_LOT_ID1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOT_ID1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOT_ID1)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_STATIC_MAIN_CCD_ID1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_CCD_ID1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_CCD_ID1)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_BUTTON_MAIN_LINK1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_LINK1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_LINK1)->ShowWindow(SW_SHOW);
        }*/

		szKey.Format(_T("IDC_BUTTON_MAIN_CCD1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CCD1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CCD1)->ShowWindow(SW_SHOW);
		}
		szKey.Format(_T("IDC_BUTTON_MAIN_DOOR1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			//GetDlgItem(IDC_BUTTON_MAIN_DOOR1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MAIN_DOOR1)->ShowWindow(SW_SHOW);
		}
		
		szKey.Format(_T("IDC_BUTTON_MAIN_MES1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			//GetDlgItem(IDC_BUTTON_MAIN_MES1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MAIN_MES1)->ShowWindow(SW_SHOW);
		}

        szKey.Format(_T("IDC_STATIC_MAIN_CCD1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_CCD1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            //GetDlgItem(IDC_STATIC_MAIN_CCD1)->ShowWindow(SW_SHOW);
        }

		szKey.Format(_T("IDC_STATIC_MAIN_CAM1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_CAM1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_STATIC_MAIN_CAM1)->ShowWindow(SW_SHOW);
		}


		szKey.Format(_T("IDC_STATIC_MAIN_CURR_MODE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1)->ShowWindow(SW_SHOW);
		}

		/*szKey.Format(_T("IDC_BUTTON_MAIN_CCD_CHANGE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1)->ShowWindow(SW_SHOW);
		}
*/

		
		/*szKey.Format(_T("IDC_BUTTON_MAIN_CAM_CHANGE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1)->ShowWindow(SW_SHOW);
		}*/

		
#ifndef GRAPH_NO_USE
        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_1ST_AA1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[0][0] != NULL)
            {
                m_pGraphWnd[0][0]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[0][0]->ShowWindow(SW_SHOW);
            }
        }

        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_2ND_AA1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[0][1] != NULL)
            {
                m_pGraphWnd[0][1]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[0][1]->ShowWindow(SW_SHOW);
            }
        }
#endif
        szKey.Format(_T("IDC_STATIC_MAIN_BAR_GRAPH1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[0][2] != NULL)
            {
                m_pGraphWnd[0][2]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[0][2]->ShowWindow(SW_SHOW);
            }
        }

        szKey.Format(_T("IDC_STATIC_MAIN_BCR1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_BCR1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            //GetDlgItem(IDC_STATIC_MAIN_BCR1)->ShowWindow(SW_SHOW);
        }

		szKey.Format(_T("IDC_STATIC_MAIN_BCR_VAL1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1)->ShowWindow(SW_SHOW);
		}

		/*szKey.Format(_T("IDC_STATIC_MAIN_MODEL_TYPE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1)->ShowWindow(SW_SHOW);
		}*/



		//master
		szKey.Format(_T("IDC_BUTTON_MASTER_CHAGE_MODE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1)->ShowWindow(SW_SHOW);
		}

		//Epoxy Complete
		/*szKey.Format(_T("IDC_BUTTON_EPOXY_COMPLETE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1)->ShowWindow(SW_SHOW);
		}
*/
		//LENS LOADING �Ϸ�
		szKey.Format(_T("IDC_BUTTON_LENS_LOADING1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_LENS_LOADING1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_LENS_LOADING1)->ShowWindow(SW_HIDE);

			
		}
		
		//STAIN
		szKey.Format(_T("IDC_STATIC_MAIN_STAIN_VIEW_AA1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1)->ShowWindow(SW_SHOW);
		}


		
		
		//
        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_PIN1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_PIN1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_PIN1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_PIN_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_FPS1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_FPS1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_FPS1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_FPS_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_STARTING_POINT1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_READY1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_RUN1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_OK1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1)->ShowWindow(SW_SHOW);
        }

        /*szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_EMISSION1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1)->ShowWindow(SW_SHOW);
        }*/

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_STOP1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_PAUSE1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_NG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1)->ShowWindow(SW_SHOW);
        }

        /*szKey.Format(_T("IDC_STATIC_MAIN_LOG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOG1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOG1)->ShowWindow(SW_SHOW);
        }*/

        szKey.Format(_T("IDC_LIST_MAIN_LOG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_LIST_MAIN_LOG1)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_LIST_MAIN_LOG1)->ShowWindow(SW_SHOW);
        }
        //m_clColorStaticMsg.ShowWindow(SW_SHOW);
        break;
    case SHOW_UNIT2:
        //GetDlgItem(IDC_STATIC_MAIN_LOGO1)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_TITLE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_VERSION1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_MODEL_NO2)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_INFO1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_MODEL_NO1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_LOT_ID1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_CCD_ID1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_LINK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_CCD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_DOOR1)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_BUTTON_MAIN_MES1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_CCD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_CAM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_CURR_MODE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE1)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE1)->ShowWindow(SW_HIDE);
		
        GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA1)->ShowWindow(SW_HIDE);
        if (m_pGraphWnd[0][0] != NULL)
            m_pGraphWnd[0][0]->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA1)->ShowWindow(SW_HIDE);
        if (m_pGraphWnd[0][1] != NULL)
            m_pGraphWnd[0][1]->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH1)->ShowWindow(SW_HIDE);
        if (m_pGraphWnd[0][2] != NULL)
            m_pGraphWnd[0][2]->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_BCR1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_BCR_VAL1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LENS_LOADING1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA1)->ShowWindow(SW_HIDE);
		
		
		
        GetDlgItem(IDC_STATIC_MAIN_OUTPUT1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_PIN1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_PIN_VAL1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_FPS1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_MAIN_FPS_VAL1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG1)->ShowWindow(SW_HIDE);
        //GetDlgItem(IDC_STATIC_MAIN_LOG1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LIST_MAIN_LOG1)->ShowWindow(SW_HIDE);

        /*szKey.Format(_T("IDC_STATIC_MAIN_LOGO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOGO2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOGO2)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_STATIC_MAIN_TITLE1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_TITLE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_TITLE2)->ShowWindow(SW_SHOW);
        }*/

        szKey.Format(_T("IDC_STATIC_MAIN_VERSION1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_VERSION2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_VERSION2)->ShowWindow(SW_SHOW);
        }

        /*szKey.Format(_T("IDC_STATIC_MAIN_INFO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_INFO2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_INFO2)->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_STATIC_MAIN_MODEL_NO1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_MODEL_NO2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_MODEL_NO2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][0] != NULL)
                m_pGraphWnd[1][0]->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_LOT_ID1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOT_ID2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOT_ID2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][1] != NULL)
                m_pGraphWnd[1][1]->ShowWindow(SW_SHOW);
        }*/

       /* szKey.Format(_T("IDC_STATIC_MAIN_CCD_ID1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_CCD_ID2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_CCD_ID2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][2] != NULL)
                m_pGraphWnd[1][2]->ShowWindow(SW_SHOW);
        }*/

        /*szKey.Format(_T("IDC_BUTTON_MAIN_LINK1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_LINK2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_LINK2)->ShowWindow(SW_SHOW);
        }*/

		szKey.Format(_T("IDC_BUTTON_MAIN_CCD1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CCD2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CCD2)->ShowWindow(SW_SHOW);
		}
		szKey.Format(_T("IDC_BUTTON_MAIN_DOOR1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			//GetDlgItem(IDC_BUTTON_MAIN_DOOR2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MAIN_DOOR2)->ShowWindow(SW_SHOW);
		}
		
		szKey.Format(_T("IDC_BUTTON_MAIN_MES1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			//GetDlgItem(IDC_BUTTON_MAIN_MES2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MAIN_MES2)->ShowWindow(SW_SHOW);

		}

        szKey.Format(_T("IDC_STATIC_MAIN_CCD1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_CCD2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_CCD2)->ShowWindow(SW_SHOW);
			
        }

		szKey.Format(_T("IDC_STATIC_MAIN_CAM1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_CAM2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_CAM2)->ShowWindow(SW_SHOW);
		}

		//
		szKey.Format(_T("IDC_STATIC_MAIN_CURR_MODE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_CURR_MODE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_CURR_MODE2)->ShowWindow(SW_SHOW);
		}

		/*szKey.Format(_T("IDC_BUTTON_MAIN_CCD_CHANGE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CCD_CHANGE2)->ShowWindow(SW_SHOW);
		}*/

		//IDC_BUTTON_MAIN_CCD_CHANGE1
		/*szKey.Format(_T("IDC_BUTTON_MAIN_CAM_CHANGE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_MAIN_CAM_CHANGE2)->ShowWindow(SW_SHOW);
		}*/
		//IDC_BUTTON_MAIN_CAM_CHANGE1
#ifndef GRAPH_NO_USE
        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_1ST_AA1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_1ST_AA2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][0] != NULL)
            {
                m_pGraphWnd[1][0]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[1][0]->ShowWindow(SW_SHOW);
            }
        }

        szKey.Format(_T("IDC_STATIC_MAIN_GRAPH_2ND_AA1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_GRAPH_2ND_AA2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][1] != NULL)
            {
                m_pGraphWnd[1][1]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[1][1]->ShowWindow(SW_SHOW);
            }
        }
#endif
        szKey.Format(_T("IDC_STATIC_MAIN_BAR_GRAPH1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_BAR_GRAPH2)->ShowWindow(SW_SHOW);
            if (m_pGraphWnd[1][2] != NULL)
            {
                m_pGraphWnd[1][2]->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
                m_pGraphWnd[1][2]->ShowWindow(SW_SHOW);
            }
        }

        szKey.Format(_T("IDC_STATIC_MAIN_BCR1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_BCR2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_BCR2)->ShowWindow(SW_SHOW);
        }

		szKey.Format(_T("IDC_STATIC_MAIN_BCR_VAL1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_BCR_VAL2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_BCR_VAL2)->ShowWindow(SW_SHOW);
		}

		/*szKey.Format(_T("IDC_STATIC_MAIN_MODEL_TYPE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_STATIC_MAIN_MODEL_TYPE2)->ShowWindow(SW_SHOW);
		}*/
		//master
		szKey.Format(_T("IDC_BUTTON_MASTER_CHAGE_MODE2"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			//GetDlgItem(IDC_BUTTON_MASTER_CHAGE_MODE2)->ShowWindow(SW_SHOW);
		}
		

		//Epoxy Complete
		/*szKey.Format(_T("IDC_BUTTON_EPOXY_COMPLETE1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
			GetDlgItem(IDC_BUTTON_EPOXY_COMPLETE2)->ShowWindow(SW_SHOW);
		}*/

		//LENS LOADING �Ϸ�
		szKey.Format(_T("IDC_BUTTON_LENS_LOADING1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_BUTTON_LENS_LOADING2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
		}

		//STAIN
		szKey.Format(_T("IDC_STATIC_MAIN_STAIN_VIEW_AA1"));
		if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
		{
			GetDlgItem(IDC_STATIC_MAIN_STAIN_VIEW_AA2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
		}
		
		
		//
        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_OUTPUT_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_OUTPUT_VAL2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_PIN1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_PIN2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_PIN2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_PIN_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_PIN_VAL2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_PIN_VAL2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_FPS1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_FPS2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_FPS2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_STATIC_MAIN_FPS_VAL1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_FPS_VAL2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_FPS_VAL2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_STARTING_POINT1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_STARTING_POINT2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_READY1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_READY2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_RUN1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_RUN2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_OK1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_OK2)->ShowWindow(SW_SHOW);
        }

        /*szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_EMISSION1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_EMISSION2)->ShowWindow(SW_SHOW);
        }
*/
        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_STOP1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_STOP2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_AUTO_PAUSE1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_AUTO_PAUSE2)->ShowWindow(SW_SHOW);
        }

        szKey.Format(_T("IDC_BUTTON_MAIN_COMPL_NG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_BUTTON_MAIN_COMPL_NG2)->ShowWindow(SW_SHOW);
        }

        /*szKey.Format(_T("IDC_STATIC_MAIN_LOG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_STATIC_MAIN_LOG2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_STATIC_MAIN_LOG2)->ShowWindow(SW_SHOW);
        }*/

        szKey.Format(_T("IDC_LIST_MAIN_LOG1"));
        if (m_clCtrlPos[0].Lookup(szKey, clPos) == TRUE)
        {
            GetDlgItem(IDC_LIST_MAIN_LOG2)->SetWindowPos(NULL, clPos.x, clPos.y, 0, 0, SWP_NOSIZE);
            GetDlgItem(IDC_LIST_MAIN_LOG2)->ShowWindow(SW_SHOW);
        }

        //m_clColorStaticMsg.ShowWindow(SW_HIDE);
        break;
    }
}

//-----------------------------------------------------------------------------
//
//	���� ��¥ ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowCurrentDate(TCHAR* szDate)
{
    CString sDate = _T("");

    m_clColorStaticCurrentDate.GetWindowText(sDate);
    if (_tcscmp(szDate, (TCHAR*)(LPCTSTR)sDate))
    {
        m_clColorStaticCurrentDate.SetWindowText(szDate);
    }
}
//-----------------------------------------------------------------------------
//
//	���� �ð� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowCurrentTime(TCHAR* szTime)
{
    CString sTime = _T("");

    m_clColorStaticCurrentTime.GetWindowText(sTime);
    if (_tcscmp(szTime, (TCHAR*)(LPCTSTR)sTime))
    {
        m_clColorStaticCurrentTime.SetWindowText(szTime);
    }
}
//-----------------------------------------------------------------------------
//
//	���� ��� ǥ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::ShowCurrentMode(int nUnit)
{
    // DRY RUN
	//if (g_clModelData[nUnit].m_nCOMPLETE_MODE == 1)
 //   {
 //       m_clColorStaticCurrMode[nUnit].SetWindowText(_T("����ǰ MODE"));
 //   }
 //   // NORMAL
 //   else
 //   {
 //       m_clColorStaticCurrMode[nUnit].SetWindowText(_T("AA MODE"));
 //   }
	/*m_clColorStaticCurrMode[nUnit].SetWindowText(_T("����ǰ MODE"));


	if (m_bMesConnect == true)
	{
		m_clButtonMes[nUnit].state = 1;
	}
	else
	{
		m_clButtonMes[nUnit].state = 0;
	}

	m_clButtonMes[nUnit].Invalidate();
	m_clColorStaticCurrMode[nUnit].Invalidate();*/
}


//-----------------------------------------------------------------------------
//
// DB ������ load
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::SelectAAData(int nUnit, CString sBcr)
{
#ifdef ON_ACCESS_DB
    TCHAR szQuery[SIZE_OF_1K];
    TCHAR szLog[SIZE_OF_1K];
    CString sData = _T("");

    if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
    {
        g_clTaskWork[nUnit].m_dDBDataX = 0.0;
        g_clTaskWork[nUnit].m_dDBDataY = 0.0;
        g_clTaskWork[nUnit].m_dDBDataTh = 0.0;
        g_clTaskWork[nUnit].m_dDBDataInTx = 0.0;
        g_clTaskWork[nUnit].m_dDBDataInTy = 0.0;
        g_clTaskWork[nUnit].m_dDBDataOutTx = 0.0;
        g_clTaskWork[nUnit].m_dDBDataOutTy = 0.0;
    }
    else
    {
        m_dInspAlignData[0] = m_dInspAlignData[1] = m_dInspAlignData[2] = 0.0;
        m_dInspInTiltData[0] = m_dInspInTiltData[1] = 0.0;
        m_dInspOutTiltData[0] = m_dInspOutTiltData[1] = 0.0;
    }

    /*if (!_tcscmp(g_clSysData.m_szModelName, _T("TEST")) || (_tcslen(g_clSysData.m_szModelName) <= 0))
    {
    return false;
    }*/
    if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
    {
        if (!_tcscmp(g_clTaskWork[nUnit].m_szChipID, _T("EMPTY")))
        {
            return false;
        }
    }

    /*_stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT SensorAlignX, SensorAlignY, SensorAlignT, LaserInTx, LaserInTy, LaserOutTx, LaserOutTy "
    "FROM AABonder "
    "WHERE BarcodeID = '%s' AND ModelName = '%s';"),
    (TCHAR*)(LPCTSTR)sBcr, g_clSysData.m_szModelName);*/


    _stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT SensorAlignX, SensorAlignY, SensorAlignT, LaserInTx, LaserInTy, LaserOutTx, LaserOutTy "
        "FROM AABonder "
        "WHERE BarcodeID = '%s' "
        "AND ModelName = '%s';"),
        (TCHAR*)(LPCTSTR)sBcr, g_clSysData.m_szModelName);

    if (g_clAdo.ExcuteQry(szQuery) > 0)
    {
        if (g_clAdo.GetRowSize() <= 0)
            return false;

        g_clAdo.GetData(0, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataX = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspAlignData[0] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(1, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataY = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspAlignData[1] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(2, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataTh = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspAlignData[2] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(3, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataInTx = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspInTiltData[0] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(4, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataInTy = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspInTiltData[1] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(5, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataOutTx = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspOutTiltData[0] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        g_clAdo.GetData(6, 0, sData);
        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            g_clTaskWork[nUnit].m_dDBDataOutTy = _ttof((TCHAR*)(LPCTSTR)sData);
        }
        else
        {
            m_dInspOutTiltData[1] = _ttof((TCHAR*)(LPCTSTR)sData);
        }

        if (nUnit >= 0 && nUnit <  MAX_UNIT_COUNT)
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[DB] ALIGN(X:%.03lf, Y:%.03lf, TH:%.03lf), TILT IN(TX:%.03lf, TY:%.03lf) TILT OUT(TX:%.03lf, TY:%.03lf)"),
                g_clTaskWork[nUnit].m_dDBDataX, g_clTaskWork[nUnit].m_dDBDataY, g_clTaskWork[nUnit].m_dDBDataTh,
                g_clTaskWork[nUnit].m_dDBDataInTx, g_clTaskWork[nUnit].m_dDBDataInTy,
                g_clTaskWork[nUnit].m_dDBDataOutTx, g_clTaskWork[nUnit].m_dDBDataOutTx);
            AddLog(szLog, 0, nUnit);
        }
        else
        {
            _stprintf_s(szLog, SIZE_OF_1K, _T("[DB] ALIGN(X:%.03lf, Y:%.03lf, TH:%.03lf), TILT IN(TX:%.03lf, TY:%.03lf) TILT OUT(TX:%.03lf, TY:%.03lf)"),
                m_dInspAlignData[0], m_dInspAlignData[1], m_dInspAlignData[2], m_dInspInTiltData[0], m_dInspInTiltData[1],
                m_dInspOutTiltData[0], m_dInspOutTiltData[1]);
            AddLog(szLog, 0, nUnit);
        }

        return true;
    }
#endif
    return false;
}


//-----------------------------------------------------------------------------
//
//	������ �������� ��Ŷ �۽�
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::SendLaserMeasurePacket(int nUnit)
{
#ifndef ON_LINE_LASER
    return true;
#endif

    return m_clSerialThread.SendKeyenceMeasureSet(nUnit);
}

//-----------------------------------------------------------------------------
//
//	
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::GetPcbTeachingData(int nUnit)
{
#ifdef ON_ACCESS_DB
	CString sDBQry = _T("");
	TCHAR szPath[SIZE_OF_1K];
	TCHAR szConnStr[SIZE_OF_1K];
	CString strx;
	CString stry;
	CString strz;
	CString strTx;
	CString strTy;

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\AXISPCB_DATABASE.mdb"), BASE_DATA_PATH);
	_stprintf_s(szConnStr, SIZE_OF_1K, _T("Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=%s;DefaultDir=;Uid=;Pwd=;"), szPath);

	if (g_clAdo.bAdoConnect == false) 
	{
		if (!g_clAdo.AdoConnect(szConnStr)) return false;
	}

	//���� ����

	sDBQry.Format(_T("SELECT * FROM PCB_TABLE WHERE (LOTID = '%s')"), g_clTaskWork[nUnit].m_szChipID);

	//_stprintf_s(sDBQry, SIZE_OF_1K, _T("SELECT COUNT(LOTID) FROM PCB_TABLE WHERE LOTID='%s'"), "1");// g_clTaskWork[nUnit].m_szChipID);

	g_clAdo.ExecuteQryRs(sDBQry);

	g_clAdo.m_csAdoProcPtr.Lock();


	if (g_clAdo.IsEOF()) {
		//func_AA_DBDisConnect();
		AddLog(_T("[FAIL] LOT ID�� ã�� ���� �����ϴ�."), 0, UNIT_AA1, false);
		g_clAdo.m_csAdoProcPtr.Unlock();
		return false;
	}

	g_clAdo.GetRsDatachar("PCBX", strx);// g_clSFRTeacingData.dSFRPcbX);
	g_clAdo.GetRsDatachar("PCBY", stry);//g_clSFRTeacingData.dSFRPcbY);
	g_clAdo.GetRsDatachar("PCBZ", strz);//g_clSFRTeacingData.dSFRPcbZ);
	g_clAdo.GetRsDatachar("PCBTX", strTx);// g_clSFRTeacingData.dSFRPcbX);
	g_clAdo.GetRsDatachar("PCBTY", strTy);//g_clSFRTeacingData.dSFRPcbY);

	g_clSFRTeacingData.dSFRPcbX = atof(strx);
	g_clSFRTeacingData.dSFRPcbY = atof(stry);
	g_clSFRTeacingData.dSFRPcbZ = atof(strz);
	g_clSFRTeacingData.dSFRPcbTx = atof(strTx);
	g_clSFRTeacingData.dSFRPcbTy = atof(strTy);

	g_clAdo.m_csAdoProcPtr.Unlock();
#endif
	return true;
}


bool CAutoInspDlg::InsertPcbTeachingData(int nUnit)
{
#ifdef ON_ACCESS_DB
	TCHAR szQuery[SIZE_OF_1K];
	int nCount = 0;

	_stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT COUNT(LOTID) FROM PCB_TABLE WHERE LOTID='%s'"), g_clTaskWork[nUnit].m_szChipID);

	g_clAdo.m_csAdoProcPtr.Lock();
	if (g_clAdo.ExcuteQry(szQuery) < 1)
	{
		g_clAdo.m_csAdoProcPtr.Unlock();
		return false;
	}


	TCHAR szPcbXTeachingVal[SIZE_OF_1K];
	TCHAR szPcbYTeachingVal[SIZE_OF_1K];
	TCHAR szPcbZTeachingVal[SIZE_OF_1K];
	TCHAR szPcbTxTeachingVal[SIZE_OF_1K];
	TCHAR szPcbTyTeachingVal[SIZE_OF_1K];

	//SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbX;
	//SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbY;
	//SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbZ;

	_stprintf_s(szPcbXTeachingVal, SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbX);
	_stprintf_s(szPcbYTeachingVal, SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbY);
	_stprintf_s(szPcbZTeachingVal, SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbZ);
	_stprintf_s(szPcbTxTeachingVal, SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbTx);
	_stprintf_s(szPcbTyTeachingVal, SIZE_OF_1K, _T("%.3lf"), g_clSFRTeacingData.dSFRPcbTy);

	g_clAdo.GetData(0, 0, nCount);
	SYSTEMTIME m_TIME;
	GetLocalTime(&m_TIME);
	CString sCTime = _T("");
	sCTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_TIME.wYear, m_TIME.wMonth, m_TIME.wDay, m_TIME.wHour, m_TIME.wMinute, m_TIME.wSecond);

	if (nCount > 0)
	{
		//�����Ͱ� �ִ� ��� UPDATTE
		_stprintf_s(szQuery, SIZE_OF_1K, _T("UPDATE PCB_TABLE SET _TIME = '%s', PCBX = '%s', PCBY = '%s', PCBZ = '%s', PCBTX = '%s', PCBTY = '%s' WHERE LOTID = '%s'"),
			sCTime, szPcbXTeachingVal, szPcbYTeachingVal, szPcbZTeachingVal, szPcbTxTeachingVal, szPcbTyTeachingVal, g_clTaskWork[nUnit].m_szChipID);// g_clTaskWork[nUnit].m_szChipID);
	}
	else
	{
		//���°�� INSERT
		_stprintf_s(szQuery, SIZE_OF_1K, _T("INSERT INTO PCB_TABLE(_TIME , LOTID, PCBX, PCBY, PCBZ, PCBTX, PCBTY) "
			"VALUES(NOW(), '%s','%s','%s','%s','%s','%s')"),//
			g_clTaskWork[nUnit].m_szChipID, szPcbXTeachingVal, szPcbYTeachingVal, szPcbZTeachingVal, szPcbTxTeachingVal, szPcbTyTeachingVal);
	}

	if (g_clAdo.InsUpDel(szQuery) > 0)
	{
		g_clAdo.m_csAdoProcPtr.Unlock();
		return true;
	}
	g_clAdo.m_csAdoProcPtr.Unlock();
#endif
	return false;
}

bool CAutoInspDlg::InsertEEpromData(int nUnit)
{
#ifdef ON_ACCESS_DB
	TCHAR szQuery[SIZE_OF_1K];
	int nCount = 0;
	_stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT COUNT(LOTID) FROM EEPROM_TABLE WHERE LOTID='%s'"), g_clTaskWork[nUnit].m_szChipID);

	g_clAdo.m_csAdoProcPtr.Lock();
	if (g_clAdo.ExcuteQry(szQuery) < 1)
	{
		g_clAdo.m_csAdoProcPtr.Unlock();
		return false;
	}
	TCHAR szOcXData[SIZE_OF_1K];
	TCHAR szOcYData[SIZE_OF_1K];
	TCHAR szRgData[SIZE_OF_1K];
	TCHAR szBgData[SIZE_OF_1K];
	TCHAR szGrGbData[SIZE_OF_1K];

	
	_stprintf_s(szOcXData, SIZE_OF_1K, _T("%.3lf"), g_clMandoInspLog[nUnit].m_UvAfter_OC_DelatX);
	_stprintf_s(szOcYData, SIZE_OF_1K, _T("%.3lf"), g_clMandoInspLog[nUnit].m_UvAfter_OC_DelatY);
	_stprintf_s(szRgData, SIZE_OF_1K, _T("%.6lf"), g_clMandoInspLog[nUnit].m_Log_CS_RG_6500K);
	_stprintf_s(szBgData, SIZE_OF_1K, _T("%.6lf"), g_clMandoInspLog[nUnit].m_Log_CS_BG_6500K);
	_stprintf_s(szGrGbData, SIZE_OF_1K, _T("%.6lf"), g_clMandoInspLog[nUnit].m_Log_CS_GrGb_6500K);

	//TRACE("------------------------->%d\n", g_clAdo.GetRowSize());

	g_clAdo.GetData(0, 0, nCount);
	SYSTEMTIME m_TIME;
	GetLocalTime(&m_TIME);
	CString sCTime = _T("");
	sCTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_TIME.wYear, m_TIME.wMonth, m_TIME.wDay, m_TIME.wHour, m_TIME.wMinute, m_TIME.wSecond);

	if (nCount > 0)
	{
		//�����Ͱ� �ִ� ��� UPDATTE
		_stprintf_s(szQuery, SIZE_OF_1K, _T("UPDATE EEPROM_TABLE SET _TIME = '%s', CASEID = '%s', OCX = '%s', OCY = '%s', RG_6500K = '%s' , BG_6500K = '%s', GRGB_6500K = '%s' WHERE LOTID = '%s'"),
			sCTime, _T("0"),
			szOcXData, szOcYData, szRgData, szBgData, szGrGbData,
			g_clTaskWork[nUnit].m_szChipID);
	}
	else
	{
		//���°�� INSERT
		_stprintf_s(szQuery, SIZE_OF_1K, _T("INSERT INTO EEPROM_TABLE(_TIME , LOTID, CASEID, OCX, OCY, RG_6500K, BG_6500K, GRGB_6500K) "
			"VALUES(NOW(), '%s','%s','%s','%s','%s','%s','%s')"),//
			g_clTaskWork[nUnit].m_szChipID, _T("0"),
			szOcXData, szOcYData, szRgData, szBgData, szGrGbData);
	}

	if (g_clAdo.InsUpDel(szQuery) > 0)
	{
		g_clAdo.m_csAdoProcPtr.Unlock();
		return true;
	}
	g_clAdo.m_csAdoProcPtr.Unlock();
#endif
	return false;
}
//-----------------------------------------------------------------------------
//
//	LASER �������� ������ ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::InsertLaserTilt(int nUnit, bool bLaserIn)
{
#ifdef ON_ACCESS_DB
    TCHAR szQuery[SIZE_OF_1K];
    int nCount = 0;

    if (/*!_tcscmp(g_clSysData.m_szModelName, _T("TEST")) || */(_tcslen(g_clSysData.m_szModelName) <= 0))
    {
        return false;
    }

    if (!_tcscmp(g_clTaskWork[nUnit].m_szChipID, _T("EMPTY")))
    {
        return false;
    }

    _stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT COUNT(BarcodeID) FROM AABonder WHERE BarcodeID='%s' AND ModelName = '%s'"),
        g_clTaskWork[nUnit].m_szChipID, g_clSysData.m_szModelName);
    if (g_clAdo.ExcuteQry(szQuery) < 1)
        return false;

    TRACE("------------------------->%d\n", g_clAdo.GetRowSize());

    g_clAdo.GetData(0, 0, nCount);

    if (nCount > 0)
        // �����Ͱ� ������ UPDATE
        //if (g_clAdo.GetRowSize() > 0)
    {
        // ���� �����̸�
        if (bLaserIn == true)
        {
            _stprintf_s(szQuery, SIZE_OF_1K, _T("UPDATE AABonder SET "
                "												 LaserInTx = %.03lf, "
                "												 LaserInTy = %.03lf "
                "												 WHERE BarcodeID = '%s' AND ModelName = '%s'"),
                g_clTaskWork[nUnit].m_dLaserTiltX[0], g_clTaskWork[nUnit].m_dLaserTiltY[0],
                g_clTaskWork[nUnit].m_szChipID, g_clSysData.m_szModelName);
        }
        // �ܺ� �����̸�
        else
        {
            _stprintf_s(szQuery, SIZE_OF_1K, _T("UPDATE AABonder SET "
                "												 LaserOutTx = %.03lf, "
                "												 LaserOutTy = %.03lf "
                "												 WHERE BarcodeID = '%s' AND ModelName = '%s'"),
                g_clTaskWork[nUnit].m_dLaserTiltX[1], g_clTaskWork[nUnit].m_dLaserTiltY[1],
                g_clTaskWork[nUnit].m_szChipID, g_clSysData.m_szModelName);
        }

        if (g_clAdo.InsUpDel(szQuery) > 0)
            return true;
    }
    // �����Ͱ� ������ INSERT
    else
    {
        // ���� �����̸�
        if (bLaserIn == true)
        {
            _stprintf_s(szQuery, SIZE_OF_1K, _T("INSERT INTO AABonder(AADate, ModelName, BarcodeID, LaserInTx, LaserInTy) "
                "VALUES(NOW(), '%s', '%s', %.03lf, %.03lf)"),
				g_clSysData.m_szModelName, g_clTaskWork[nUnit].m_szChipID, g_clTaskWork[nUnit].m_dLaserTiltX[0], g_clTaskWork[nUnit].m_dLaserTiltY[0]);
        }
        // �ܺ� �����̸�
        else
        {
            _stprintf_s(szQuery, SIZE_OF_1K, _T("INSERT INTO AABonder(AADate, ModelName, BarcodeID, LaserOutTx, LaserOutTy) "
                "VALUES(NOW(), '%s', '%s', %.03lf, %.03lf)"),
				g_clSysData.m_szModelName, g_clTaskWork[nUnit].m_szChipID, g_clTaskWork[nUnit].m_dLaserTiltX[1], g_clTaskWork[nUnit].m_dLaserTiltY[1]);
        }

        if (g_clAdo.InsUpDel(szQuery) > 0)
            return true;
    }
#endif
    return false;
}
//-----------------------------------------------------------------------------
//
//	ALIGN ������ ����
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::InsertAlignData(int nUnit)
{
#ifdef ON_ACCESS_DB
    TCHAR szQuery[SIZE_OF_1K];
    int nCount;

    if (/*!_tcscmp(g_clSysData.m_szModelName, _T("TEST")) || */(_tcslen(g_clSysData.m_szModelName) <= 0))
    {
        return false;
    }

    if (!_tcscmp(g_clTaskWork[nUnit].m_szChipID, _T("EMPTY")) || (_tcslen(g_clTaskWork[nUnit].m_szChipID) <= 0))
    {
        return false;
    }

    _stprintf_s(szQuery, SIZE_OF_1K, _T("SELECT COUNT(BarcodeID) FROM AABonder WHERE BarcodeID='%s' AND ModelName = '%s'"),
        g_clTaskWork[nUnit].m_szChipID, g_clSysData.m_szModelName);
    if (g_clAdo.ExcuteQry(szQuery) < 1)
        return false;

    g_clAdo.GetData(0, 0, nCount);

    // �����Ͱ� ������ UPDATE
    //if (g_clAdo.GetRowSize() > 0)
    if (nCount > 0)
    {
        _stprintf_s(szQuery, SIZE_OF_1K, _T("UPDATE AABonder SET SensorAlignX = %.03lf, "
            "					 SensorAlignY = %.03lf, "
            "					 SensorAlignT = %.03lf "
            "WHERE BarcodeID = '%s' AND ModelName = '%s'"),
            //g_clTaskWork[nUnit].m_dAADataX, g_clTaskWork[nUnit].m_dAADataY, g_clTaskWork[nUnit].m_dAADataTh,
            g_clTaskWork[nUnit].m_dSensorAlign[0], g_clTaskWork[nUnit].m_dSensorAlign[1], g_clTaskWork[nUnit].m_dSensorAlign[2],
            g_clTaskWork[nUnit].m_szChipID, g_clSysData.m_szModelName);

        if (g_clAdo.InsUpDel(szQuery) > 0)
            return true;
    }
    // �����Ͱ� ������ INSERT
    else
    {
        _stprintf_s(szQuery, SIZE_OF_1K, _T("INSERT INTO AABonder(AADate, ModelName, BarcodeID, SensorAlignX, SensorAlignY, SensorAlignT) "
            "VALUES(NOW(), '%s', '%s', %.03lf, %.03lf, %.03lf)"),
			g_clSysData.m_szModelName, g_clTaskWork[nUnit].m_szChipID,
            g_clTaskWork[nUnit].m_dSensorAlign[0], g_clTaskWork[nUnit].m_dSensorAlign[1], g_clTaskWork[nUnit].m_dSensorAlign[2]);
        /*g_clTaskWork[nUnit].m_dAADataX,
        g_clTaskWork[nUnit].m_dAADataY, g_clTaskWork[nUnit].m_dAADataTh);*/

        if (g_clAdo.InsUpDel(szQuery) > 0)
            return true;
    }
#endif
    return false;
}



//-----------------------------------------------------------------------------
//
//	��ư : LINK(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainLink1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_GRABBER
    if (g_clLaonGrabberWrapper[UNIT_AA1].OpenDevice() == false)
    {
        AddLog(_T("[FAIL] Device Open Fail"), 1, UNIT_AA1);
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : CCD(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainCcd1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}
#ifdef ON_LINE_GRABBER
	m_clCustomThread[UNIT_AA1].CustomThreadrun(CCD_THREAD);

	//return;
    //pThread_MIUCheck = ::AfxBeginThread(Thread_MIUCheck, this);
   // m_clCcdGrabThread[UNIT_AA1].MIUCheck_process();
   // return;


#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : ���� ����(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainStartingPoint1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}
	if (g_clModelData[UNIT_AA1].m_nDoorLockUse == 1)
	{
		if (g_clDioControl.GetDoorState() > 0)	// �� �ݰ������� true, ������ false.
		{
			AddLog(_T("[INFO] DOOR OPEN.. �������� �Ұ�."), 1, UNIT_AA1, TRUE);
			return;
		}
	}


    if (g_ShowMsgModal(_T("Ȯ��"), _T("��ü �������� �Ͻðڽ��ϱ� ?"), RGB_COLOR_RED) == false)
        return;

    this->StartHomeProcess(UNIT_AA1);
}

//-----------------------------------------------------------------------------
//
//	��ư : ���� �غ�(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoReady1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*TCHAR szLog[SIZE_OF_1K];
	
	if (g_clModelData[UNIT_AA1].m_nInspOKPass == 1 && g_clModelData[UNIT_AA1].m_nMasterModeUse == 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("���� MODE Ȯ�� �ٶ��ϴ�."));
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, true);
	}
	else if (g_clModelData[UNIT_AA1].m_nInspOKPass == 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("���� MODE Ȯ�� �ٶ��ϴ�."));
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, true);
	}
	else if (g_clModelData[UNIT_AA1].m_nMasterModeUse == 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("���� MODE Ȯ�� �ٶ��ϴ�."));
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, true);
	}
	else if (g_clModelData[UNIT_AA1].m_nInspOKPass == 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EOL PASS MODE �Դϴ�."));
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, true);
	}
	else if (g_clModelData[UNIT_AA1].m_nMasterModeUse == 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("MASTER MODE �Դϴ�."));
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, true);
	}
	else
	{
		InterLockMsg(_T("����"), szLog, MESSAGE_BG_COLOR, false);
	}*/
	
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_PAUSE)
	{
		AddLog(_T("[INFO] �Ͻ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}
	if (g_clModelData[UNIT_AA1].m_nDoorLockUse == 1)
	{
		if (g_clDioControl.GetDoorState() > 0)	// �� �ݰ������� true, ������ false.
		{
			AddLog(_T("[INFO] DOOR OPEN.. �������� �Ұ�."), 1, UNIT_AA1, TRUE);
			return;
		}
	}
	this->StartAutoReadyProcess(UNIT_AA1);
    
}

//-----------------------------------------------------------------------------
//
//	��ư : �ڵ� ����(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoRun1()
{
	TCHAR szLog[SIZE_OF_1K];
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	//if (g_clModelData[UNIT_AA1].m_nMesPass == 1)
	//{
	//	
	//	/*if (g_ShowMsgModal(_T("Ȯ��"), _T("[AUTO] MES PASS �Դϴ� �����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
	//	{
	//		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]MES PASS MODE.STOP"));
	//		AddLog(szLog, 1, UNIT_AA1);
	//		return;
	//	}*/
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]MES PASS �Դϴ�"));
	//	AddLog(szLog, 0, UNIT_AA1);
	//}

	/*if (MesSpecLoadCheck == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MES SPEC LOAD FAIL"));
		AddLog(szLog, 1, UNIT_AA1, true);
		return;
	}*/
	if (g_clModelData[UNIT_AA1].m_nDoorLockUse == 1)
	{
		if (g_clDioControl.GetDoorState() > 0)	// �� �ݰ������� true, ������ false.
		{
			AddLog(_T("[INFO] DOOR OPEN.. �������� �Ұ�."), 1, UNIT_AA1, TRUE);
			return;
		}
	}


    this->StartAutoProcess(UNIT_AA1);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ OK(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplOk1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
    {
        AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
        return;
    }

    // �����غ� Ǯ��
   /* g_clTaskWork[UNIT_AA1].m_nAutoFlag = MODE_STOP;
	g_pCarAABonderDlg->m_clColorButtonAutoReady[UNIT_AA1].state = 0;
	g_pCarAABonderDlg->m_clColorButtonAutoStop[UNIT_AA1].state = 1;
	g_pCarAABonderDlg->m_clColorButtonAutoReady[UNIT_AA1].Invalidate();
	g_pCarAABonderDlg->m_clColorButtonAutoStop[UNIT_AA1].Invalidate();*/
	if (g_clTaskWork[UNIT_AA1].m_nEmission == 1)
	{
		g_clTaskWork[UNIT_AA1].m_nEmission = 0;
		AddLog(_T("[SOCKET] ��ǰ ���� ���"), 0, 0, false);
		//_stprintf_s(g_clTaskWork[UNIT_AA1].m_szChipID, SIZE_OF_100BYTE, _T("EMPTY")); //bcr �ʱ�ȭ
		//g_pCarAABonderDlg->ShowBarcode(UNIT_AA1);
	}
	else
	{
		g_clTaskWork[UNIT_AA1].m_nEmission = 1;
		AddLog(_T("[SOCKET] ��ǰ ���� "), 0, 0, false);
		//AddLog(_T("[INFO] ��ǰ �������� ��� �Ұ�"), 1, UNIT_AA1);
		/*if (g_ShowMsgModal(_T("Ȯ��"), _T("[AUTO] ����˻� ������ ��ǰ�� ��ǰ ���Ⱑ���մϴ�.\n�����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == true)
		{
			g_clTaskWork[UNIT_AA1].m_nEmission = 1;
			AddLog(_T("[SOCKET] ��ǰ ����"), 0, 0, false);
		}
		else
		{
			AddLog(_T("[SOCKET] ��ǰ ���� ���"), 0, 0, false);
		}*/
	}
    g_clTaskWork[UNIT_AA1].SaveData();
    this->ShowOkNgState(UNIT_AA1 , g_clTaskWork[UNIT_AA1].m_nEmission);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ ����(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplEmission1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //m_clProductNGDlg.SetUnit(UNIT_AA1);
    //m_clProductNGDlg[UNIT_AA1].ShowWindow(SW_SHOW);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoStop1()
{
	//g_clPriInsp[UNIT_AA1].func_EEprom_CheckSum_Check(true);
	//return;

     //TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.  (m_nUnit + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
    CString sMsg = _T("");

#ifdef ON_LINE_SOCKET
	//sMsg.Format(_T("#AA%d@PCB&ESC$"), (UNIT_AA1 + 1));		//Auto Stop
	//this->SendDataToAAMain(UNIT_AA1, sMsg);
	//Sleep(100);
 //   sMsg.Format(_T("#AA%d@ALARM&CLR$"), (UNIT_AA1 + 1));
 //   this->SendDataToAAMain(UNIT_AA1, sMsg);
	//Sleep(100);
 //   sMsg.Format(_T("#AA%d@ULD&ESC$"), (UNIT_AA1 + 1));
 //   this->SendDataToAAMain(UNIT_AA1, sMsg); 
	//sMsg.Empty();
#endif


    this->StopAutoProcess(UNIT_AA1);
}

//-----------------------------------------------------------------------------
//
//	��ư : �Ͻ� ����(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoPause1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

#ifdef ON_LINE_SOCKET
	sMsg.Format(_T("#AA%d@PCB&ESC$"), (UNIT_AA1 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));		//Auto Pause
	this->SendDataToAAMain(UNIT_AA1, sMsg);
	Sleep(100);
	sMsg.Format(_T("#AA%d@LENS&ESC$"), (UNIT_AA1 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));		//Auto Stop
	this->SendDataToAAMain(UNIT_AA1, sMsg);
	Sleep(100);
    sMsg.Format(_T("#AA%d@ALARM&CLR$"), (UNIT_AA1 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
    this->SendDataToAAMain(UNIT_AA1, sMsg);
	Sleep(100);
   
    sMsg.Format(_T("#AA%d@ULD&ESC$"), (UNIT_AA1 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
    this->SendDataToAAMain(UNIT_AA1, sMsg);

#endif


	this->PauseAutoProcess(UNIT_AA1);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ NG(UNIT1)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplNg1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
    {
        AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ�"), 1, UNIT_AA1);
        return;
    }

    // �����غ� Ǯ��
   /* g_clTaskWork[UNIT_AA1].m_nAutoFlag = MODE_STOP;
	g_pCarAABonderDlg->m_clColorButtonAutoReady[UNIT_AA1].state = 0;
	g_pCarAABonderDlg->m_clColorButtonAutoStop[UNIT_AA1].state = 1;
	g_pCarAABonderDlg->m_clColorButtonAutoReady[UNIT_AA1].Invalidate();
	g_pCarAABonderDlg->m_clColorButtonAutoStop[UNIT_AA1].Invalidate();*/
	if (g_clTaskWork[UNIT_AA1].m_nEmission == 2)
	{
		g_clTaskWork[UNIT_AA1].m_nEmission = 0;
		AddLog(_T("[SOCKET] NG ���� ���"), 0, 0, false);
	}
	else
	{
		g_clTaskWork[UNIT_AA1].m_nEmission = 2;
		AddLog(_T("[SOCKET] NG ����"), 0, 0, false);
		//AddLog(_T("[SOCKET] NG �������� ��� �Ұ�"), 1, UNIT_AA1);
	}
    g_clTaskWork[UNIT_AA1].SaveData();
    this->ShowOkNgState(UNIT_AA1 , g_clTaskWork[UNIT_AA1].m_nEmission);
}

//-----------------------------------------------------------------------------
//
//	��ư : LINK(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainLink2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_GRABBER
    if (g_clLaonGrabberWrapper[UNIT_AA1].OpenDevice() == false)
    {
        AddLog(_T("[FAIL] Device Open Fail"), 1, UNIT_AA1);
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	��ư : CCD(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainCcd2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}
#ifdef ON_LINE_GRABBER
	m_clCustomThread[UNIT_AA2].CustomThreadrun(CCD_THREAD);
    m_clCcdGrabThread[UNIT_AA2].MIUCheck_process();

#endif

}

//-----------------------------------------------------------------------------
//
//	��ư : ���� ����(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainStartingPoint2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}
    CString sMsg;

    if (g_ShowMsgModal(_T("Ȯ��"), _T("��ü �������� �Ͻðڽ��ϱ� ?"), RGB_COLOR_RED) == false)
        return;

   // sMsg.Format(_T("#AA%d@PCB_ITF&ACK$"), UNIT_AA2 + g_clSysData.m_nUnitNo+ 1);
    //g_pCarAABonderDlg->SendDataToAAMain(UNIT_AA2, sMsg);

    this->StartHomeProcess(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	��ư : ���� �غ�(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoReady2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}
    this->StartAutoReadyProcess(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	��ư : �ڵ� ����(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoRun2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clModelData[UNIT_AA2].m_nMesPass == 1)
	{
		TCHAR szLog[SIZE_OF_1K];
		/*if (g_ShowMsgModal(_T("Ȯ��"), _T("[AUTO] MES PASS �Դϴ� �����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]MES PASS MODE.STOP"));
			AddLog(szLog, 1, UNIT_AA2);
			return;
		}*/
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]MES PASS �Դϴ�"));
		AddLog(szLog, 0, UNIT_AA2);
	}
    this->StartAutoProcess(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ OK(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplOk2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	if (g_clTaskWork[UNIT_AA2].m_nEmission == 1)
	{
		g_clTaskWork[UNIT_AA2].m_nEmission = 0;
		AddLog(_T("[SOCKET] ���� ���"), 0, 0, false);
		//_stprintf_s(g_clTaskWork[UNIT_AA2].m_szChipID, SIZE_OF_100BYTE, _T("EMPTY")); //bcr �ʱ�ȭ
		//g_pCarAABonderDlg->ShowBarcode(UNIT_AA2);
	}
	else
	{
		g_clTaskWork[UNIT_AA2].m_nEmission = 1;
		AddLog(_T("[SOCKET] ��ǰ ����"), 0, 0, false);
		//AddLog(_T("[INFO] ��ǰ �������� ��� �Ұ�"), 1, UNIT_AA2);
		/*if (g_ShowMsgModal(_T("Ȯ��"), _T("[AUTO] ����˻� ������ ��ǰ�� ��ǰ ���Ⱑ���մϴ�.\n�����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == true)
		{
			g_clTaskWork[UNIT_AA2].m_nEmission = 1;
			AddLog(_T("[SOCKET] ��ǰ ����"), 0, 0, false);
		}
		else
		{
			AddLog(_T("[SOCKET] ��ǰ ���� ���"), 0, 0, false);
		}*/
	}
	g_clTaskWork[UNIT_AA2].SaveData();
	this->ShowOkNgState(UNIT_AA2, g_clTaskWork[UNIT_AA2].m_nEmission);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ ����(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplEmission2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //m_clProductNGDlg.SetUnit(UNIT_AA2);
    //m_clProductNGDlg[UNIT_AA2].ShowWindow(SW_SHOW);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoStop2()
{
	CString sMsg = _T("");
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#ifdef ON_LINE_SOCKET
	sMsg.Format(_T("#AA%d@ALARM&CLR$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
	this->SendDataToAAMain(UNIT_AA2, sMsg);

	sMsg.Format(_T("#AA%d@PCB&ESC$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));		//Auto Stop2
	this->SendDataToAAMain(UNIT_AA2, sMsg);

	sMsg.Format(_T("#AA%d@ULD&ESC$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
	this->SendDataToAAMain(UNIT_AA2, sMsg);


#endif
    this->StopAutoProcess(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	��ư : �Ͻ� ����(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAutoPause2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

#ifdef ON_LINE_SOCKET
    sMsg.Format(_T("#AA%d@ALARM&CLR$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
    this->SendDataToAAMain(UNIT_AA2, sMsg);
    Sleep(50);

    sMsg.Format(_T("#AA%d@PCB&ESC$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));		//Auto Pause2
    this->SendDataToAAMain(UNIT_AA2, sMsg);
    Sleep(50);

    sMsg.Format(_T("#AA%d@ULD&ESC$"), (UNIT_AA2 + g_clSysData.m_nUnitNo + g_clSysData.m_nSysNo + 1));
    this->SendDataToAAMain(UNIT_AA2, sMsg);
    Sleep(50);

#endif
    this->PauseAutoProcess(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	��ư : ����ǰ NG(UNIT2)
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainComplNg2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	if (g_clTaskWork[UNIT_AA2].m_nEmission == 2)
	{
		g_clTaskWork[UNIT_AA2].m_nEmission = 0;
		AddLog(_T("[SOCKET] ���� ���"), 0, 0, false);
	}
	else
	{
		g_clTaskWork[UNIT_AA2].m_nEmission = 2;
		AddLog(_T("[SOCKET] NG ����"), 0, 0, false);
	}
	g_clTaskWork[UNIT_AA2].SaveData();
	this->ShowOkNgState(UNIT_AA2, g_clTaskWork[UNIT_AA2].m_nEmission);
}

//-----------------------------------------------------------------------------
//
//	��ư : ���� ��ȯ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainUnitChange()
{
	//return;//cal  ���� ����
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	return;


	if (MAX_UNIT_COUNT < 2)
	{
		return;
	}
	if (m_nCurrentDlg == DLG_MAIN)
		return;

	if (m_nCurrentUnit == UNIT_AA1)
		m_nCurrentUnit = UNIT_AA2;
	else
		m_nCurrentUnit = UNIT_AA1;

	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : MAIN
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainMain()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//m_nCurrentUnit = SHOW_ALL;

	return;
	if (MAX_UNIT_COUNT < 2)
	{
		return;
	}
	m_nCurrentDlg = DLG_MAIN;

	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : MANUAL
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainManual()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_MANUAL;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : TEACH
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainTeaching()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	return;
#endif
	m_nCurrentDlg = DLG_TEACH;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : CCD
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainCcd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_CCD;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : DIO
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainDio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_DIO;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : LIGHT
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainLight()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_LIGHT;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//	��ư : CONFIG
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainAlarm()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_ALARM;
	this->ShowDialog(m_nCurrentDlg);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainConfig()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nCurrentDlg = DLG_CONFIG;
	this->ShowDialog(m_nCurrentDlg);
}
//-----------------------------------------------------------------------------
//
// ����ȭ��
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainMinimize()
{
    g_pCarAABonderDlg->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

//-----------------------------------------------------------------------------
//
//	��ư : EXIT
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 0, UNIT_AA1);
		return;
	}


	CMessageModalDlg* pDlg = new CMessageModalDlg(_T("����"), _T("�����Ͻðڽ��ϱ� ?"), RGB_COLOR_RED);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDNO)
		{
			delete pDlg;
			return;
		}

		delete pDlg;
	}
#ifdef _DEBUG
	//_CrtSetBreakAlloc(2098);
	//_CrtMemDumpAllObjectsSince(0);
	//AfxSetAllocStop(190); // �޸� �Ҵ� Ƚ���� 10���� ����
#endif
	this->FinishService();
	// �޸� ���� ���� ���
	_CrtDumpMemoryLeaks();
	PostMessage(WM_CLOSE, NULL, NULL);
}


//-----------------------------------------------------------------------------
//
//	��ư : LAN
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnBnClickedButtonMainLan()
{
	CString sMsg = _T("");
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.		
#ifdef ON_LINE_SOCKET
    if (this->ConnectAAMain() == false)
    {
		AddLog(_T("[SOCKET] MAIN ���� ����"), 0, 0, false);
		//AddLog(_T("[SOCKET] MAIN ���� ����"), 0, 1, false);

        m_clButtonExLan.state = 0;
        m_clButtonExLan.Invalidate();
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	Static : 1para ���귮 �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainOutput1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

    sMsg.Format(_T("[PRODUCT COUNT] ��ǰ ���� ���� �ʱ�ȭ�����Ͻðڽ��ϱ�?"));

    if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
    {
        return;
    }

    g_clTaskWork[UNIT_AA1].m_nTotalWorkCount = 0;
    g_clTaskWork[UNIT_AA1].m_nTotalOkCount = 0;
    g_clTaskWork[UNIT_AA1].m_nTotalNgCount = 0;
    g_clTaskWork[UNIT_AA1].SaveData();
    this->ShowOutputCount(UNIT_AA1);
}

//-----------------------------------------------------------------------------
//
//	Static : 2para ���귮 �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainOutput2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

    sMsg.Format(_T("[PRODUCT COUNT] 2PARA ��ǰ ���� ���� �ʱ�ȭ�� �����Ͻðڽ��ϱ�?"));

    if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
    {
        return;
    }

    g_clTaskWork[UNIT_AA2].m_nTotalWorkCount = 0;
    g_clTaskWork[UNIT_AA2].m_nTotalOkCount = 0;
    g_clTaskWork[UNIT_AA2].m_nTotalNgCount = 0;
    g_clTaskWork[UNIT_AA2].SaveData();
    this->ShowOutputCount(UNIT_AA2);
}

//-----------------------------------------------------------------------------
//
//	Static : 1para ������ ��뷮 �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainPin1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

    sMsg.Format(_T("[PRODUCT COUNT] ������ ��뷮 �ʱ�ȭZ�����Ͻðڽ��ϱ�?"));

    if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
    {
        return;
    }

    g_clTaskWork[UNIT_AA1].m_nUsePinCount = 0;
    g_clTaskWork[UNIT_AA1].PinSaveData();
    this->ShowPinCount(UNIT_AA1);

}

//-----------------------------------------------------------------------------
//
//	Static : 2para ������ ��뷮 �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainPin2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");

    sMsg.Format(_T("[PRODUCT COUNT] 2PARA ������ ��뷮 �ʱ�ȭ�� �����Ͻðڽ��ϱ�?"));

    if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
    {
        return;
    }

    g_clTaskWork[UNIT_AA2].m_nUsePinCount = 0;
    g_clTaskWork[UNIT_AA2].PinSaveData();
    this->ShowPinCount(UNIT_AA2);
}


void CAutoInspDlg::OnStnClickedStaticMainVersion1()
{
#ifdef _DEBUG
	TCHAR szLog[SIZE_OF_1K];
	/*
	g_clVision.ClearOverlay(0);
	g_FindCirclePos(0, g_clVision.m_pImgBuff[0][1], g_clModelData[0].m_clSfrInfo.m_clRectCircle);
	g_clVision.DrawOverlayAll(0);
	double m_dMesUvAfterRotate = g_CalcImageAlign(0);

	_stprintf_s(szLog, SIZE_OF_1K, _T("rotation :%lf"), m_dMesUvAfterRotate);
	AddLog(szLog, 0, 0);
	double count =  1 - (4 * 0.1);

	g_clMotorSet.MovePcbTMotor(0, m_dMesUvAfterRotate * count, true);
*/
	byte WData[1];
	WData[0] = 0x07;

	int index = 2;
	byte ndata = (WData[0] & (1 << index)) >> index;
	_stprintf_s(szLog, SIZE_OF_1K, _T("Start Data :%d"), WData[0]);
	AddLog(szLog, 0, 0);
	for (size_t i = 0; i < 4; i++)
	{
		ndata = (WData[0] & (1 << i)) >> i;
		_stprintf_s(szLog, SIZE_OF_1K, _T("Data :%d"), ndata);
		AddLog(szLog, 0, 0);
	}
	byte data = WData[0];
	ndata = 0;
	data <<= index;
	data >>= 7;
	return;
//bit���
//return (data & (1 << n��°)) >>n��°;
//data <<= n��°;
//data >>=7;
//return data
//g_clVision.ClearOverlay(0);
//g_OpencvFindCirclePos(0, g_clLaonGrabberWrapper[0].m_pFrameRawBuffer, g_clModelData[0].m_clSfrInfo.m_clRectCircle, false);
//g_clVision.DrawOverlayAll(0);
	//int i = 0;
	//for (i = 0; i < 10; i++)
	//{
	//	g_AvrGetSFR(g_clLaonGrabberWrapper[0].m_pFrameRawBuffer, i);//g_clTaskWork[m_nUnit].m_nRawSumCount);
	//}
	//g_DiffReset();
	//g_clMesCommunication[0].g_FinalEolLog(0);
	
	
#else

#endif


#ifdef NORINDA_MODE


	//TCHAR szLog[SIZE_OF_1K];
	//g_clMesCommunication[0].compareData[0] = 1;
	//int nrtn = memcmp(g_clMesCommunication[0].mMesGetEEpromData, g_clMesCommunication[0].compareData, 300);
	//if (nrtn == 0)
	//{
	//	//sLog.Format("EEPROM TOTAL VERIFY OK!");
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM RETURN DATA VERIFY OK!"));
	//	AddLog(szLog, 0, 0);
	//}
	//else
	//{
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM RETURN DATA VERIFY NG!"));
	//	AddLog(szLog, 0, 0);
	//}

	//g_clMesCommunication[0].g_FovVertexLog(0);
	//g_clPriInsp[0].func_EEprom_Write(true);
	/*g_clVision.DrawMOverlayText(0, 50, 100, _T("EOL PASS"), M_COLOR_GREEN, _T("Arial"), 150, 80, FALSE);
	g_clVision.DrawOverlayAll(0);*/
	//TCHAR szLog[SIZE_OF_1K];
	//byte bLightCh = 0x01;
	//_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] %d channel"), bLightCh);
	//AddLog(szLog, 0, 0);
	//bool kkk = g_ShowMsgModal(_T("Ȯ��"), _T("[AUTO] MASTER MODE �Դϴ�. �����Ͻðڽ��ϱ�?"), RGB_COLOR_RED);

	//g_clModelData[0].VarationLoad();

	//RawDataDelete(0);
	//g_clPriInsp[0].func_EEprom_Write(true);
	
	/*
	TCHAR szPath[SIZE_OF_1K];
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		_stprintf_s(szPath, SIZE_OF_1K, _T("i = %d=%d"), i / 3, i % 3);
		AddLog(szPath, 0, 0);
	}
*/
	//g_clMesCommunication[0].g_FinalEolLog(0);
	///EEPROM_Temp_DumpSave(0, g_clMesCommunication[0].mMesGetEEpromData);
	//g_clMesCommunication[0].g_FinalEolLog(0);
	//return;
	//int i = 0;
	//int j = 0;
	//int mindex = 0;

	//Binary_Dump_Load(0);

	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 346, 2, g_clMesCommunication[0].mStrData[0], DEC_MODE);	//Color temperature 1
	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 348, 2, g_clMesCommunication[0].mStrData[1], DEC_MODE);//Color temperature 2
	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 403, 1, g_clMesCommunication[0].mStrData[2], DEC_MODE);//Horizontal grid number 
	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 404, 1, g_clMesCommunication[0].mStrData[3], DEC_MODE);//Vertical grid number
	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 405, 2, g_clMesCommunication[0].mStrData[4], DEC_MODE);//High color temperature
	//eepromData[0].HexToString(g_clMesCommunication[0].mReadEEpromData + 1431, 2, g_clMesCommunication[0].mStrData[5], DEC_MODE);//Low color temperature

	//g_clMesCommunication[0].MesEolSave(0);
	/*eepromData.HexToString(MesProcess[0].mReadEEpromData + 256, 4, MesProcess[0].mStrData[0], FLOAT_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 260, 2, MesProcess[0].mStrData[1], DEC_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 262, 2, MesProcess[0].mStrData[2], DEC_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 32, 8, MesProcess[0].mStrData[3], ASCII_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 40, 16, MesProcess[0].mStrData[4], ASCII_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 80, 16, MesProcess[0].mStrData[5], ASCII_MODE);
	eepromData.HexToString(MesProcess[0].mReadEEpromData + 96, 8, MesProcess[0].mStrData[6], ASCII_MODE);*/

	//g_pCarAABonderDlg->m_clVisionStaticCcd[0].SaveSfrRoi();
	//g_clPriInsp[0].func_EEprom_Write(true);
	//int mEolStartAddress = 346;// 256;
	//int mEolEndAddress = 2457;// 345;
	//int writingByteNum = 32;
	//TCHAR szPath[SIZE_OF_1K];
	////i % writingByteNum  �̰� 0�̸� 32������
	//for (i = 0; i < 400; i++)
	//{
	//	
	//	_stprintf_s(szPath, SIZE_OF_1K, _T("check =[%d / %d / %d]"), i, i / writingByteNum, (i+1) % writingByteNum);
	//	AddLog(szPath, 0, 0);
	//}
	return;
	
#endif
}


void CAutoInspDlg::OnStnClickedStaticMainVersion2()
{
	//g_clMesCommunication[UNIT_AA2].newSave(UNIT_AA2);//mes ���� 
    //g_FinalInspLog(UNIT_AA2);
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //m_clUpdateHistoryDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainBcrVal1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // ����ǰ ����϶���
    CKeyBoardDlg* pDlg = new CKeyBoardDlg(20, false);
	CString sData;
	m_clColorStaticBcrVal[UNIT_AA1].GetWindowText(sData);
	pDlg->SetReturnValue(sData);
    if (pDlg != NULL)
    {
		
		
        if (pDlg->DoModal() == IDOK)
        {
            m_clColorStaticBcrVal[UNIT_AA1].SetWindowText(pDlg->GetReturnValue());

			//_tcscpy_s(g_clTaskWork[UNIT_AA1].m_szLotID, SIZE_OF_100BYTE, _T(pDlg->GetReturnValue()));
			_tcscpy_s(g_clTaskWork[UNIT_AA1].m_szChipID, SIZE_OF_100BYTE, _T(pDlg->GetReturnValue()));
			//g_clTaskWork[UNIT_AA1].SaveData();
			g_clModelData[UNIT_AA1].LotDataSave();
        }

        delete pDlg;
    }
	sData.Empty();
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::OnStnClickedStaticMainBcrVal2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // ����ǰ ����϶���
    CKeyBoardDlg* pDlg = new CKeyBoardDlg(20, false);
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            m_clColorStaticBcrVal[UNIT_AA2].SetWindowText(pDlg->GetReturnValue());
            _tcscpy_s(g_clTaskWork[UNIT_AA2].m_szLotID, SIZE_OF_100BYTE, _T(pDlg->GetReturnValue()));
			_tcscpy_s(g_clTaskWork[UNIT_AA2].m_szChipID, SIZE_OF_100BYTE, _T(pDlg->GetReturnValue()));
			g_clTaskWork[UNIT_AA2].mRepetCount = 0;
			
        }

        delete pDlg;
    }

}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::FinishService()
{
	int i = 0;
	g_clLaonGrabberWrapper[UNIT_AA1].CloseDevice();
	
	TopChartControl[UNIT_AA1].dpctrlLedVolume(LIGHT_CHART_CH_1, 0);
	LightLeftChartControl[UNIT_AA1].dpctrlLedVolume(LEFT_CHART_CH_1, 0);
	LightRightChartControl[UNIT_AA1].dpctrlLedVolume(RIGHT_CHART_CH_1, 0);
	BackDuLightControl[UNIT_AA1].ctrlLedVolume(LIGHT_BD_OC_CH1, 0);
	BackDuLightControl[UNIT_AA1].ctrlLedVolume(LIGHT_BD_ALIGN_CH1, 0);
#ifdef ON_LINE_LIGHT

	TopChartControl[UNIT_AA1].Close_Device();
	BackDuLightControl[UNIT_AA1].Close_Device();
	LightLeftChartControl[UNIT_AA1].Close_Device();
	LightRightChartControl[UNIT_AA1].Close_Device();

	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_clSerialThread.CloseLightSerial(i);
		m_clSerialThread.CloseBcrSerial(i);
}
#endif

#ifdef ON_LNIE_SOCKET
	//this->CloseAAMain();
#endif
#ifndef GRAPH_NO_USE
	// �׷��� ������ ����
	this->ReleaseGraphWnd();
#endif
	// CLOCK ������ ����

	m_clClockThread.EndThread();
	m_clFileThread.EndThread();
	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_clActiveAlignThread[i].EndThread();
		m_clCustomThread[i].EndThread();
	}
#ifdef ON_LINE_LASER

	// Serial ������ ����	
	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_clSerialThread.CloseKeyenceSerial(i);
		m_clSerialThread.CloseLightSerial(i);
		m_clSerialThread.CloseBcrSerial(i);
	}

	
#endif
	m_clSerialThread.EndThread();
#ifdef ON_LINE_GRABBER
	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_clCcdCvtColorThread[i].EndThread();
		m_clCcdGrabThread[i].EndThread();
	}
#endif

#ifdef ON_LINE_MIL
	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		g_clVision.ClearOverlay(i);
		m_clVisionGrabThread[i].EndThread();
	}

	g_clModelFinder.CloseModelFind();
	g_clVision.CloseViion();
#endif

#ifdef ON_LINE_SOCKET
	//g_clAdo.Disconnect();
#endif

	// �α� ������ ����
	m_clLogThread.StopLogging();
	m_clLogThread.EndThread();//EndThread();
	
	// ����� ������ ����
	m_clMonitorThread.EndThread();
	

	// �˾� ���̾�α� ����
	for (i = 0; i < MAX_POPUP_COUNT; i++)
	{
		if (g_pMessagePopupDlg[i] != NULL)
		{
			delete g_pMessagePopupDlg[i];
			g_pMessagePopupDlg[i] = NULL;
		}
	}
	if (InterLockDlg != NULL)
	{
		delete InterLockDlg;
	}
	
	if (g_clCMSMESSocket != NULL)
	{
		delete g_clCMSMESSocket;
	}
	
	m_clCtrlPos[0].RemoveAll();
	
}


void CAutoInspDlg::MasterModeChangeHandler(int Unit)
{

	m_bMasterModeState[Unit] = !m_bMasterModeState[Unit];
	if (m_bMasterModeState[Unit] == true)
	{
		m_clColorButtonMasterMode[Unit].SetWindowTextA(_T("MASTER MODE: ON"));
		m_clColorButtonMasterMode[Unit].state = 1;
	}
	else
	{
		m_clColorButtonMasterMode[Unit].SetWindowTextA(_T("MASTER MODE: OFF"));
		m_clColorButtonMasterMode[Unit].state = 0;
	}
	m_clColorButtonMasterMode[Unit].Invalidate();
}

void CAutoInspDlg::EpoxyCompleteChange(int Unit)
{
	
	if (g_clTaskWork[Unit].m_nEpoxyCompl == 1)
	{
		m_clColorButtonEpoxyCompl[Unit].SetWindowTextA(_T("���� �Ϸ�"));
		m_clColorButtonEpoxyCompl[Unit].state = 1;
	}
	else
	{
		m_clColorButtonEpoxyCompl[Unit].SetWindowTextA(_T("���� �̿Ϸ�"));
		m_clColorButtonEpoxyCompl[Unit].state = 0;
	}
	m_clColorButtonEpoxyCompl[Unit].Invalidate();
}

void CAutoInspDlg::LensLoadingChange(int Unit)
{
	if (g_clTaskWork[Unit].m_nLensLoading == 1)
	{
		m_clColorButtonLensLoading[Unit].SetWindowTextA(_T("LENS �ѱ� �Ϸ�"));
		m_clColorButtonLensLoading[Unit].state = 1;
	}
	else
	{
		m_clColorButtonLensLoading[Unit].SetWindowTextA(_T("LENS �ѱ� �̿Ϸ�"));
		m_clColorButtonLensLoading[Unit].state = 0;
	}
	m_clColorButtonLensLoading[Unit].Invalidate();
}

bool CAutoInspDlg::EpoxyFinddispense(int nIndex)
{
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);
	MbufClear(g_clVision.m_MilGrabImageChild[nIndex][0], 0);
	Sleep(500);
	//g_clVision.m_csGrab.Lock();
	Sleep(100);
	//MimFlip(vision.MilGrabImageChild[CAM1], vision.MilGrabImageChild[CAM1], M_HORIZONTAL, M_DEFAULT);
	MbufGet(g_clVision.m_MilGrabImageChild[nIndex][0], m_Imagebuf);	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

	g_clVision.ClearOverlay(nIndex);
	//vision.m_csGrab.Unlock();

	bool bRtn = true;

	bool bRtn1 = true;
	bool bRtn2 = true;
	bool bRtn3 = true;
	bool bRtn4 = true;
	//
	bool bRtn5 = true;
	bool bRtn6 = true;
	bool bRtn7 = true;
	bool bRtn8 = true;

	bRtn1 = InspResignRect(false, nIndex, 0, 0, m_Imagebuf);		// left (����)
	bRtn2 = InspResignRect(false, nIndex, 0, 1, m_Imagebuf);	// right (����)
	bRtn3 = InspResignRect(false, nIndex, 1, 2, m_Imagebuf);	// top (����)
	bRtn4 = InspResignRect(false, nIndex, 1, 3, m_Imagebuf);	// bottom (����)
															//
	bRtn5 = InspQuaternHole(true, 0, 0, 0);
	bRtn6 = InspQuaternHole(true, 0, 0, 1);
	bRtn7 = InspQuaternHole(true, 0, 0, 2);
	bRtn8 = InspQuaternHole(true, 0, 0, 3);

	bRtn = (bRtn1 && bRtn2 && bRtn3 && bRtn4 && bRtn5 && bRtn6 && bRtn7 && bRtn8);
	free(m_Imagebuf);


	if (bRtn)
	{
		g_clVision.m_clMilDrawText[nIndex].AddList(860, 690, 35, 15, "O K", "Arial", M_COLOR_GREEN);
	}
	else
	{
		g_clVision.m_clMilDrawText[nIndex].AddList(860, 690, 35, 15, "N G", "Arial", M_COLOR_RED);
	}
	g_clVision.DrawOverlayAll(nIndex);

	return bRtn;
}

bool CAutoInspDlg::InspResignRect(bool autoMode, int index, int iDirection, int iRectCnt, unsigned char *m_Imagebuf)
{
	int iCh = index;
	double centX = g_clTaskWork[index].m_dAlignData.x;
	double centY = g_clTaskWork[index].m_dAlignData.y;
	
	int margine = 3;
	CString sTemp;
	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int iDispenseCnt = 0;
	int iIsDispense = 0;
	int iDispenseSpec = 10;


	int Ewidth = 0;
	int Eheight = 0;
	bool pass = true;
	if (centX == 0)
	{
		centX = CAM_SIZE_X / 2;// 1024.0 / 2;
	}
	else
	{
		centX = (CAM_SIZE_X / 2) + centX;
	}


	if (centY == 0)
	{
		centY = CAM_SIZE_Y / 2;// 768.0 / 2;
	}
	else
	{
		centY = (CAM_SIZE_Y / 2) + centY;
	}
	CRect inRect;

	inRect.left = (int)(centX + g_clModelData[index].m_ResingRectStart[iRectCnt].x);
	inRect.top = (int)(centY + g_clModelData[index].m_ResingRectStart[iRectCnt].y);
	inRect.right = (int)(centX + g_clModelData[index].m_ResingRectStart[iRectCnt].x + g_clModelData[index].m_ResingRectSize[iRectCnt].x);
	inRect.bottom = (int)(centY + g_clModelData[index].m_ResingRectStart[iRectCnt].y + g_clModelData[index].m_ResingRectSize[iRectCnt].y);
	Ewidth = inRect.right - inRect.left;
	Eheight = inRect.bottom - inRect.top;

	iDispenseSpec = (g_clModelData[index].m_dResinInspRectSpec *  Ewidth) / 100;//���� ��� �����

																 //! �˻� ������ �ȼ��� ��� �� ���ϱ�
	int iAllSumCnt = 0, iSumCnt = 0;				//! �˻� �������� ��Ⱚ ���� �ȼ� ��ü�� ī��Ʈ
	int sum = 0;									//! �ȼ� ��Ⱚ�� ��
	int whiteVal = 0;								//! ��� �ȼ� ��� �� ��ȯ

	CPoint point;
	if (iDirection == 0)	// ���ι���
	{
		for (y = inRect.top + 1; y<inRect.bottom - 1; y++)
		{
			iIsDispense = 0;
			iDispenseCnt = 0;
			for (x = inRect.left + 1; x<inRect.right - 1; x++)
			{
				pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
				if (m_Imagebuf[pos] > 200)// model.m_ResingInspLevel)
				{
					iDispenseCnt++;
				}
				else
				{
					point.x = x;
					point.y = y;
					g_clVision.m_clMilDrawPixel[iCh].AddList(point,5, M_COLOR_MAGENTA);
				}					//! ��� �� ���ۿ� ����
			}
			if (iDispenseCnt < iDispenseSpec)//���η� �̵��������� ����ۼ�Ʈ���� �� ��ĥ��
			{
				pass = false;

			}

		}
	}
	else// ���ι���
	{
		inRect.left = (int)(centX + g_clModelData[index].m_ResingRectStart[iRectCnt].x);
		inRect.top = (int)(centY + g_clModelData[index].m_ResingRectStart[iRectCnt].y);
		inRect.right = (int)(centX + (g_clModelData[index].m_ResingRectStart[iRectCnt].x + g_clModelData[index].m_ResingRectSize[iRectCnt].x));
		inRect.bottom = (int)(centY + (g_clModelData[index].m_ResingRectStart[iRectCnt].y + g_clModelData[index].m_ResingRectSize[iRectCnt].y));
		Ewidth = inRect.right - inRect.left;
		Eheight = inRect.bottom - inRect.top;

		iDispenseSpec = (g_clModelData[index].m_dResinInspRectSpec *  Eheight) / 100;//���� ��� �����
		for (x = inRect.left + 1; x<inRect.right - 1; x++)
		{
			iIsDispense = 0;
			iDispenseCnt = 0;
			for (y = inRect.top + 1; y<inRect.bottom - 1; y++)
			{
				pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
				if (m_Imagebuf[pos] > 200)// model.m_ResingInspLevel)
				{
					iDispenseCnt++;
				}
				else
				{
					point.x = x;
					point.y = y;
					g_clVision.m_clMilDrawPixel[iCh].AddList(point, 5, M_COLOR_MAGENTA);
				}					//! ��� �� ���ۿ� ����
			}
			if (iDispenseCnt < iDispenseSpec)//���η� �̵��������� ����ۼ�Ʈ���� �� ��ĥ��
			{
				pass = false;
			}

		}
	}
	if (pass)
	{
		g_clVision.m_clMilDrawBox[iCh].AddList(inRect,1, PS_SOLID, M_COLOR_GREEN);				// ���� �˻� ����..
		return true;
	}
	else
	{
		g_clVision.m_clMilDrawBox[iCh].AddList(inRect,1, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
		return false;
	}

	return true;
}

bool CAutoInspDlg::InspQuaternHole(bool autoMode, int index, int dispMode, int iCirCnt)
{
	int iCh = index;


	//Task.m_bResign_Result[index] = false;
	//====================================================�����˻��̹��� �߰�
	double dReduceFactorX = 0.;
	double dReduceFactorY = 0.;

	dReduceFactorX = (double)g_pCarAABonderDlg->Graph_Width / CAM_SIZE_X;
	dReduceFactorY = (double)g_pCarAABonderDlg->Graph_Height / CAM_SIZE_Y;
	//MimResize(g_clVision.m_MilGrabImageChild[index][0], g_clVision.MilOptImage, dReduceFactorX, dReduceFactorY, M_DEFAULT);////���� �̹��� ȭ�鿡 ���÷��� 

																											 //====================================================�����˻��̹��� �߰�
																											 //vision.drawOverlay(CCD);
																											 //end
	CPoint inRadius, outRadius;
	float inOffsetX = (float)((g_clModelData[index].m_CircleDrawSize[iCirCnt].x * 1) - g_clModelData[index].m_dResinInspOffset[0].x);	//���� Offset
	float inOffsetY = (float)((g_clModelData[index].m_CircleDrawSize[iCirCnt].y * 1) - g_clModelData[index].m_dResinInspOffset[0].y);
	float outOffsetX = (float)((g_clModelData[index].m_CircleDrawSize[iCirCnt].x * 1) + g_clModelData[index].m_dResinInspOffset[1].x);	//�ٱ��� Offset
	float outOffsetY = (float)((g_clModelData[index].m_CircleDrawSize[iCirCnt].y * 1) + g_clModelData[index].m_dResinInspOffset[1].y);
	inRadius.x = (int)inOffsetX / g_clSysData.m_clCamResol[iCh].x;
	outRadius.x = (int)outOffsetX / g_clSysData.m_clCamResol[iCh].x;
	int i_limit_rate = g_clModelData[index].m_nResinInspLimit;

	int margine = 5;

	char	szDispData[256];
	CString sTemp = "";
	

	//g_clVision.ClearOverlay(index);

	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int sx = 0;
	int	sy = 0;
	int	ex = 0;
	int	ey = 0;

	double centX = 0.0;
	double centY = 0.0;
	if (g_clTaskWork[index].m_dAlignData.x == 0)//PCB_Chip_MARK = 0
	{
		centX = CAM_SIZE_X / 2;//512
	}
	else
	{
		centX = (CAM_SIZE_X / 2) + g_clTaskWork[index].m_dAlignData.x;
	}
	if (g_clTaskWork[index].m_dAlignData.y == 0)
	{
		centY  = CAM_SIZE_Y / 2;//384
	}
	else
	{
		centY = (CAM_SIZE_Y / 2) + g_clTaskWork[index].m_dAlignData.y;
	}

	centX = centX + g_clModelData[index].m_CircleDrawCenter[iCirCnt].x;
	centY = centY + g_clModelData[index].m_CircleDrawCenter[iCirCnt].y;


	//centX = model.m_CircleDrawCenter[iCirCnt].x; ;// model.dEpoxyOffset_X;
	//centY = model.m_CircleDrawCenter[iCirCnt].y; ;// model.dEpoxyOffset_Y;

	CRect inRect, outRect;
	inRect.left = (int)(centX - inOffsetX / g_clSysData.m_clCamResol[iCh].x);
	inRect.right = (int)(centX + inOffsetX / g_clSysData.m_clCamResol[iCh].x);
	inRect.top = (int)(centY - inOffsetY / g_clSysData.m_clCamResol[iCh].y);
	inRect.bottom = (int)(centY + inOffsetY / g_clSysData.m_clCamResol[iCh].y);

	outRect.left = (int)(centX - outOffsetX / g_clSysData.m_clCamResol[iCh].x);
	outRect.right = (int)(centX + outOffsetX / g_clSysData.m_clCamResol[iCh].x);
	outRect.top = (int)(centY - outOffsetY / g_clSysData.m_clCamResol[iCh].y);
	outRect.bottom = (int)(centY + outOffsetY / g_clSysData.m_clCamResol[iCh].y);

	//g_clVision.m_clMilDrawCross[iCh].AddList((int)centX, (int)centY, 2, M_COLOR_BLUE);	// ���� �˻� �߽� ��ġ..
	g_clVision.m_clMilDrawBox[iCh].AddList(inRect,2, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
	g_clVision.m_clMilDrawBox[iCh].AddList(outRect,2, PS_SOLID, M_COLOR_GREEN);


	int Hist[CAM_SIZE_X] = { 0, };

	int inspStartPosX[CAM_SIZE_X] = { 0, };
	int inspEndPosX[CAM_SIZE_X] = { 0, };
	int inspStartPosY[CAM_SIZE_Y] = { 0, };
	int inspEndPosY[CAM_SIZE_Y] = { 0, };

	int startPoint[CAM_SIZE_X] = { 0, };
	int endPoint[CAM_SIZE_X] = { 0, };

	int leftVal = 0;
	int leftPos = 0;
	int rightVal = 0;
	int rightPos = 0;
	int topVal = 0;
	int topPos = 0;
	int bottomVal = 0;
	int bottomPos = 0;

	static CString sLangChange;

	//////////////////////////////////////////////////////////////////////////////////
	// �� ���� �˻� Start..
	sx = (int)outRect.left;
	ex = (int)outRect.right;
	sy = (int)outRect.top;
	ey = (int)outRect.bottom;

	if (sx<0)					sx = 0;
	if (ex >= CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey >= CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if ((outOffsetX - inOffsetX) / g_clSysData.m_clCamResol[iCh].x < 10)
	{
		sLangChange.Format("[ ERROR ] Insp ROI size X Error.(%d~%d)", inOffsetX, outOffsetX);
		_stprintf_s(szDispData, sLangChange);
		g_clVision.m_clMilDrawText[iCh].AddList(50, 690, 17, 8, szDispData, _T("arialuni"), M_COLOR_RED);
		AddLog(_T("[���� �˻�] �˻� ���� ���� �� �̻�"), 1, UNIT_AA1);
		return false;
	}
	if ((outOffsetY - inOffsetY) / g_clSysData.m_clCamResol[iCh].y < 10)
	{
		sLangChange.Format("[ ERROR ] Insp ROI size Y Error.(%d~%d)", outOffsetY, inOffsetY);
		_stprintf_s(szDispData, sLangChange);
		g_clVision.m_clMilDrawText[iCh].AddList(50, 690, 17, 8, szDispData, _T("arialuni"), M_COLOR_RED);

		//g_clVision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		AddLog(_T("[���� �˻�] �˻� ���� ���� �� �̻�"), 1, UNIT_AA1);
		return false;
	}


	//!! Hole�� ������ �������� ��� 0x00���� Image ��ȯ
	double HoleW = 0.0, HoleH = 0.0;
	double Length = 0;
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

	MbufGet(g_clVision.m_MilGrabImageChild[index][0], m_Imagebuf);
	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

	//!�ܺο��� �������� Buf�� ����.
	CPoint cpCutSize;
	cpCutSize.x = abs(ex - sx);
	cpCutSize.y = abs(ey - sy);

	CString FileName = "";
	unsigned char	*cutimgBuf;
	cutimgBuf = (unsigned char *)malloc(cpCutSize.x * cpCutSize.y);
	memset(cutimgBuf, 0, sizeof(cutimgBuf));

	//! �ܺο������۸� cv�� ����.
	IplImage *tempImg;
	tempImg = cvCreateImage(cvSize(CAM_SIZE_X, CAM_SIZE_Y), 8, 1);
	memcpy(tempImg->imageData, m_Imagebuf, CAM_SIZE_X * CAM_SIZE_Y);

	//-------------------------------------------------------------------------------------------
	int value;//CvScalar value; (�÷�)
	int thresholdNum = 125;
	int insp_specIn_percent = 50;		//���� �������� �ۼ�Ʈ % (�̰����ٴ� Ŀ�� �����ƴٰ� �Ǵ�)
	int insp_Circle_Spec = 45;		//������ �� ���� (90�� �� 45)
	int insp_color_Spec = 150;// 150;			//�������� ��� ����(0 ~ 255)
	int xSum = 0;

	double pixel_mm = 0.26458333;//1px = mm

	double centerX = 0.0;
	double centerY = 0.0;
	double _x = 0.0;
	double _y = 0.0;
	double x2 = 0.0;
	double y2 = 0.0;
	double inspX = 0.0;
	double inspY = 0.0;
	double iColorLevel = 0.0;
	int pointRadius = 1;
	int pointThickness = 2;
	int specIn_Line = 0;
	int specIn_circleLine = 0;
	// 
#if ____MODEL_NAME ==  XPENG_REAR 
	int startAngArr[4] = { 360,105,185,280 };//LT>RT>RB>RB   
	int circleAng[4] = { 85,70,70,75 };
#else
	int startAngArr[4] = { 10,125,200,284 };//LT>RT>RB>LB   
	int circleAng[4] = { 65,50,50,65 };
#endif
	int startAng = startAngArr[iCirCnt]; //360;					//���׸��� ������ġ 
	double circleSize = circleAng[iCirCnt]; //120;// 360.0;				//�� ũ��		

	double radiusx = (inRect.right - inRect.left) / 2;// 50.0;	//�� ������ x
	double radiusy = (inRect.bottom - inRect.top) / 2;// 75.0;	//�� ������ y
	double inLine_outLine_Gap = inRect.left - outRect.left;// 18.0;		//�ζ��� , �ƿ����� ���� �����˻� ����
														   //double ang = 0.0;
														   //ang = getAngle(x1,y1,x2,y2);//����45.000


	int _width = CAM_SIZE_X;
	int _height = CAM_SIZE_Y;
	//CRect inRect, outRect;
	centerX = inRect.left + ((inRect.right - inRect.left) / 2);
	//centerX = inRect.left + ((outRect.right - inRect.left)/2);
	centerY = inRect.top + ((inRect.bottom - inRect.top) / 2);
	//centerY = inRect.top + ((outRect.bottom - inRect.top)/2);

	xSum = ((inLine_outLine_Gap / 100)*insp_specIn_percent);
	TCHAR* pszRow[] = { _T("TL"), _T("TR"), _T("BR"), _T("BL") };
	g_clVision.m_clMilDrawLine[iCh].AddList(CPoint(inRect.left, inRect.top), CPoint(inRect.right, inRect.bottom), 3, PS_SOLID, M_COLOR_GREEN);
	g_clVision.m_clMilDrawLine[iCh].AddList(CPoint(outRect.left, outRect.top), CPoint(outRect.right, outRect.bottom), 3, PS_SOLID, M_COLOR_GREEN);
	g_clVision.m_clMilDrawText[iCh].AddList(centerX, centerY, 15, 12, pszRow[iCirCnt], "Arial",M_COLOR_GREEN);
	for (int ang = startAng; ang<(startAng + circleSize); ang++)
	{
		_x = (radiusx + 0)*cos(getRadianFromAng(ang)) + centerX;
		_y = (radiusy + 0)*sin(getRadianFromAng(ang)) + centerY;
		x2 = (radiusx + inLine_outLine_Gap)*cos(getRadianFromAng(ang)) + centerX;
		y2 = (radiusy + inLine_outLine_Gap)*sin(getRadianFromAng(ang)) + centerY;

		specIn_Line = 0;
		for (int i = 0; i<inLine_outLine_Gap; i++)
		{
			inspX = i * cos(getRadianFromAng((double)ang)) + _x;
			inspY = i * sin(getRadianFromAng((double)ang)) + _y;
			if (inspY < 0 || inspX < 0)
			{
				sTemp.Format("Pos[%d] ��������", iCirCnt);
				return false;
				break;
			}
			value = cvGetReal2D(tempImg, inspY, inspX);
			iColorLevel = (value);//(value.val[0] + value.val[1] + value.val[2]) / 3;

			if (iColorLevel>insp_color_Spec)
			{
				specIn_Line++;
			}
			else
			{
				g_clVision.m_clMilDrawPixel[iCh].AddList(CPoint(inspX, inspY), 20, M_COLOR_RED);// M_COLOR_BLUE);
			}
		}
		if (specIn_Line < xSum)
		{
			specIn_circleLine += 1;
		}
	}

	sTemp.Format("����������[%s] =%.1lfmm [Spec: %.1lfmm]", pszRow[iCirCnt], specIn_circleLine*pixel_mm, insp_Circle_Spec*pixel_mm);// , circleSize*pixel_mm);
																															   //sTemp.Format("����������[%d] =%.1lfmm||������� = %.1lfmm||�ѱ��� =%.1lfmm", iCirCnt, specIn_circleLine*pixel_mm, insp_Circle_Spec*pixel_mm);// , circleSize*pixel_mm);
	AddLog(sTemp, 1, UNIT_AA1);


	free(m_Imagebuf);
	cvReleaseImage(&tempImg);
	if (specIn_circleLine < insp_Circle_Spec)//(������ ����)
	{
		sTemp.Format("Pos[%d] ��������", iCirCnt);
	}
	else
	{
		sTemp.Format("Pos[%d] ��������", iCirCnt);
		return false;
	}

	return true;
}

void CAutoInspDlg::OnBnClickedButtonMasterChageMode1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}
	MasterModeChangeHandler(UNIT_AA1);
}


void CAutoInspDlg::OnBnClickedButtonMasterChageMode2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	MasterModeChangeHandler(UNIT_AA2);
}


void CAutoInspDlg::OnBnClickedButtonMainCamChange1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	CAMChangeHandler(UNIT_AA1 , VIDEO_CAM);
}


void CAutoInspDlg::OnBnClickedButtonMainCamChange2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	CAMChangeHandler(UNIT_AA2 , VIDEO_CAM);
}

void CAutoInspDlg::CAMChangeHandler(int Unit, int Mode)
{
#ifdef ON_LINE_CAM
	
#else
	m_clColorStaticCurrMode[0].SetBkColor(RGB_COLOR_GREEN);
	m_clColorStaticCurrAlignMode[0].SetBkColor(RGB_COLOR_GRAY);
	m_clColorStaticCurrMode[0].SetFontBold(TRUE);
	m_clColorStaticCurrAlignMode[0].SetFontBold(FALSE);
	m_clButtonCAMChange[Unit].state = 0;
	m_clButtonCCDChange[Unit].state = 1;
	m_clVisionStaticCam[Unit].ShowWindow(SW_HIDE);
	m_clVisionStaticCcd[Unit].ShowWindow(SW_SHOW);
	m_clButtonCAMChange[Unit].Invalidate();
	return;
#endif
	m_bCamState[Unit] = Mode;
	if(Mode == VIDEO_CAM)
	{
		//m_clButtonCAMChange[Unit].SetWindowTextA(_T("CCD"));
		///m_clColorStaticCurrMode[Unit].SetWindowText(_T("ALIGN CAM"));

		m_clButtonCAMChange[Unit].state = 1;
		m_clButtonCCDChange[Unit].state = 0;

		m_clColorStaticCurrMode[0].SetBkColor(RGB_COLOR_GRAY);
		m_clColorStaticCurrAlignMode[0].SetBkColor(RGB_COLOR_GREEN);

		m_clColorStaticCurrMode[0].SetFontBold(FALSE);
		m_clColorStaticCurrAlignMode[0].SetFontBold(TRUE);
		m_clVisionStaticCam[Unit].ShowWindow(SW_SHOW);
		m_clVisionStaticCcd[Unit].ShowWindow(SW_HIDE);
			//ShowWindow(SW_SHOW);
	}
	else
	{
		//m_clButtonCAMChange[Unit].SetWindowTextA(_T("CAM"));
		///m_clColorStaticCurrMode[Unit].SetWindowText(_T("CCD"));

		m_clColorStaticCurrMode[0].SetBkColor(RGB_COLOR_GREEN);
		m_clColorStaticCurrAlignMode[0].SetBkColor(RGB_COLOR_GRAY);
		m_clColorStaticCurrMode[0].SetFontBold(TRUE);
		m_clColorStaticCurrAlignMode[0].SetFontBold(FALSE);
		m_clButtonCAMChange[Unit].state = 0;
		m_clButtonCCDChange[Unit].state = 1;
		m_clVisionStaticCam[Unit].ShowWindow(SW_HIDE);
		m_clVisionStaticCcd[Unit].ShowWindow(SW_SHOW);
	}
	m_clColorStaticCurrMode[0].Invalidate();
	m_clColorStaticCurrAlignMode[0].Invalidate();
#ifdef ON_LINE_MIL
	if (g_pCarAABonderDlg->m_bCamState[Unit] == VIDEO_CAM)
	{
		m_clVisionStaticCcd[Unit].DrawCamAlign(999);
	}
	else
	{
		m_clVisionStaticCcd[Unit].DrawRectSfr(999);
	}

#endif
	m_clButtonCAMChange[Unit].Invalidate();
	m_clButtonCCDChange[Unit].Invalidate();
}


//-----------------------------------------------------------------------------
//
//	MES ���� 
//
//-----------------------------------------------------------------------------
bool CAutoInspDlg::ConnectMes()
{
	//
	TCHAR szLog[SIZE_OF_1K];
	if (m_bMesConnect == true)
		return true;

	g_clSysData.sDLoad();
	// ���� ����
	m_clMesConnSocket.Create();
	// MES ����
	if (m_clMesConnSocket.Connect(g_clSysData.m_szMesIp, g_clSysData.m_nMesPort) == FALSE)
	{
		m_clButtonMes[0].state = 0;
		m_clButtonMes[0].Invalidate();
		m_clMesConnSocket.Close();
		m_bMesConnect = false;
		//m_clButtonExMes.SetPress(3);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] MES(%s:%d) ���� ����"), g_clSysData.m_szMesIp, g_clSysData.m_nMesPort);
		AddLog(szLog, 1, UNIT_AA1, true);
		return false;
	}

	m_clButtonMes[0].state = 1;
	m_clButtonMes[0].Invalidate();
	m_clMesConnSocket.IDENTITY = MES_SOCKET;
	m_clMesConnSocket.SetMainDlgPtr(this);
	m_bMesConnect = true;
	_stprintf_s(szLog, SIZE_OF_1K, _T("[SOCKET] MES ���� �Ϸ�\n(MES Connection)"));
	AddLog(szLog, 0, UNIT_AA1, false);

	//g_clSysData.m_nMesUse = 1;	//20181004
	//m_clButtonExMes.SetPress(1);
	return true;
}
//-----------------------------------------------------------------------------
//
//	MES ���� ����
//
//-----------------------------------------------------------------------------
void CAutoInspDlg::DisConnectMes()
{
	TCHAR szLog[SIZE_OF_1K];
	//m_clButtonExMes.SetPress(3);
	if (m_bMesConnect == false)
		return;
	m_clMesConnSocket.Close();
	m_clButtonMes[0].state = 0;
	m_clButtonMes[0].Invalidate();
	m_bMesConnect = false;
	//g_clSysData.m_nMesUse = 0;	//20181004
	_stprintf_s(szLog, SIZE_OF_1K, _T("[INFO] MES�� ���� ������\n(MES Disconnected)"));
	AddLog(szLog, 0, UNIT_AA1, false);
}
void CAutoInspDlg::SendCMSMESMessage(int message, int mAlarmIndex)
{
	CString strPacket = _T("");
	CString m_strSTX = _T("$");
	CString m_strETX = _T("\r");
	switch (message)
	{
	case eMES_CHECK_ONLINE:
	{
		strPacket.Format(_T("%s,0000,Check Online,,%s"), m_strSTX, m_strETX);
		g_clCMSMESSocket->doSendCmd(strPacket);
	}
	break;

	case eMES_ALARM:
	{
		m_nAlarmOccured = 1;
		m_nAlarmcode = mAlarmIndex;
		strPacket.Format(_T("%s,0001,Alarm Report,%d@%d,%s"), m_strSTX, m_nAlarmcode, m_nAlarmOccured, m_strETX);
		g_clCMSMESSocket->doSendCmd(strPacket);
	}
	break;
	case eMES_ALARM_RESET:	//�˶� �ʱ�ȭ
	{
		m_nAlarmOccured = 0;
		m_nAlarmcode = mAlarmIndex;
		strPacket.Format(_T("%s,0001,Alarm Report,%d@%d,%s"), m_strSTX, m_nAlarmcode, m_nAlarmOccured, m_strETX);
		g_clCMSMESSocket->doSendCmd(strPacket);
	}
	break;

	case eMES_TRACK_IN:
	{
		strPacket.Format(_T("%s,0002,Track In Request,%s,%s"), m_strSTX, g_clCMSMESSocket->m_strBarcode, m_strETX);
		g_clCMSMESSocket->doSendCmd(strPacket);
	}
	break;

	case eMES_TRACK_OUT:
	{
		CString Result = _T("");
		CString str46HA_Sensor = _T("");	// = _T("46HA_Sensor_Value");
		CString str83HB_Sensor = _T("");	// = _T("83HB_Sensor_Value");
		CString str46HA_Main = _T("");		// = _T("46HA_Main_Value");
		CString str83HB_Main = _T("");		// = _T("83HB_Main_Value");
		int InspData = 0;

		//const float absolute_upper_limit = m_pclsConfig->IniFileReadFloat(_T("LIMITS"), _T("ID_UPPER_LIMIT"), ABSOLUTE_UPPER_LIMIT);	//1
		//const float absolute_lower_limit = m_pclsConfig->IniFileReadFloat(_T("LIMITS"), _T("ID_LOWER_LIMIT"), ABSOLUTE_LOWER_LIMIT);	//-1

		//if (g_clMesCommunication.dMES_MainPitch > absolute_upper_limit)
		//{
		//	g_clMesCommunication.iMES_FinalResult = 0;
		//}
		//if (g_clMesCommunication.dMES_MainPitch < absolute_lower_limit)
		//{
		//	g_clMesCommunication.iMES_FinalResult = 0;
		//}

		//if (g_clMesCommunication.iMES_FinalResult == 1)
		//{
		//	Result.Format(_T("OK"));
		//	InspData = 1;
		//}
		//else
		//{
		//	Result.Format(_T("NG"));
		//	InspData = 0;
		//}
		//str46HA_Sensor.Format(_T("%s"), g_clTaskWork.m_46HA_Sensor);
		//str83HB_Sensor.Format(_T("%s"), g_clTaskWork.m_83HB_Sensor);
		//str46HA_Main.Format(_T("%.3lf"), g_clMesCommunication.dMES_NarrowPitch);
		//str83HB_Main.Format(_T("%.3lf"), g_clMesCommunication.dMES_MainPitch);

		strPacket.Format(_T("%s,0003,Track OUT Request,%d@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s,%s"),
			m_strSTX,
			InspData,
			g_clCMSMESSocket->m_strBarcode,
			g_clCMSMESSocket->m_strLotID,
			g_clCMSMESSocket->m_strProdID,
			g_clCMSMESSocket->m_strProCID,
			g_clCMSMESSocket->m_strBarcode,
			str46HA_Sensor,
			str83HB_Sensor,
			Result,
			str46HA_Main,
			str83HB_Main,
			m_strETX);
		g_clCMSMESSocket->doSendCmd(strPacket);
		//
		//
		str46HA_Sensor.Empty();
		str83HB_Sensor.Empty();
		Result.Empty();
		str46HA_Main.Empty();
		str83HB_Main.Empty();

	/*	if (g_clMesCommunication.iMES_FinalResult == 1)
		{
			g_clCMSMESSocket->m_strBarcode = _T("EMPTY");
		}*/
	}
	break;

	default:
		break;
	}
	strPacket.Empty();


}

void CAutoInspDlg::versionList()
{
	TCHAR szLog[SIZE_OF_1K];

	AddLog(_T("	"), 0, UNIT_AA1);
	AddLog(_T("	"), 0, UNIT_AA1);
	AddLog(_T("==RIVIAN EOL �����̷�=="), 0, UNIT_AA1, false);
	
	_stprintf_s(szLog, SIZE_OF_1K, _T("[VER.240417_1] ���� UI ��ġ ����"));
	AddLog(szLog, 0, UNIT_AA1);
}
void CAutoInspDlg::OnBnClickedButtonMainCcdChange1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	CAMChangeHandler(UNIT_AA1, VIDEO_CCD);
}


void CAutoInspDlg::OnBnClickedButtonMainCcdChange2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	CAMChangeHandler(UNIT_AA2, VIDEO_CCD);
}


void CAutoInspDlg::OnBnClickedButtonMainMes1()
{
	// TODO: Add your control notification handler code here
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	if (m_bMesConnect == true)
	{
		this->DisConnectMes();
	}
	else
	{
		this->ConnectMes();
	}
}


void CAutoInspDlg::OnBnClickedButtonMainMes2()
{
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}
	// TODO: Add your control notification handler code here
}


void CAutoInspDlg::OnBnClickedButtonEpoxyComplete1()
{
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}

	if (g_clTaskWork[UNIT_AA1].m_nEpoxyCompl == 1)
	{
		g_clTaskWork[UNIT_AA1].m_nEpoxyCompl = 0;
		m_clColorButtonEpoxyCompl[UNIT_AA1].SetWindowTextA(_T("���� �̿Ϸ�"));
		m_clColorButtonEpoxyCompl[UNIT_AA1].state = 0;
		AddLog(_T("[INFO] ���� �̿Ϸ� ����"), 1, UNIT_AA1);
	}
	else
	{
		g_clTaskWork[UNIT_AA1].m_nEpoxyCompl = 1;
		m_clColorButtonEpoxyCompl[UNIT_AA1].SetWindowTextA(_T("���� �Ϸ�"));
		m_clColorButtonEpoxyCompl[UNIT_AA1].state = 1;
		AddLog(_T("[INFO] ���� �Ϸ� ����"), 1, UNIT_AA1);
	}
	m_clColorButtonEpoxyCompl[UNIT_AA1].Invalidate();

}


void CAutoInspDlg::OnBnClickedButtonEpoxyComplete2()
{
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	if (g_clTaskWork[UNIT_AA2].m_nEpoxyCompl == 1)
	{
		g_clTaskWork[UNIT_AA2].m_nEpoxyCompl = 0;
		m_clColorButtonEpoxyCompl[UNIT_AA2].SetWindowTextA(_T("���� �̿Ϸ�"));
		m_clColorButtonEpoxyCompl[UNIT_AA2].state = 0;
		AddLog(_T("[INFO] ���� �̿Ϸ� ����"), 1, UNIT_AA2);
	}
	else
	{
		g_clTaskWork[UNIT_AA2].m_nEpoxyCompl = 1;
		m_clColorButtonEpoxyCompl[UNIT_AA2].SetWindowTextA(_T("���� �Ϸ�"));
		m_clColorButtonEpoxyCompl[UNIT_AA2].state = 1;
		AddLog(_T("[INFO] ���� �Ϸ� ����"), 1, UNIT_AA2);
	}
	m_clColorButtonEpoxyCompl[UNIT_AA2].Invalidate();

}


void CAutoInspDlg::OnBnClickedButtonLensLoading1()
{
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA1);
		return;
	}

	if (g_clTaskWork[UNIT_AA1].m_nLensLoading == 1)
	{
		g_clTaskWork[UNIT_AA1].m_nLensLoading = 0;
		m_clColorButtonLensLoading[UNIT_AA1].SetWindowTextA(_T("LENS �ѱ� �̿Ϸ�"));
		m_clColorButtonLensLoading[UNIT_AA1].state = 0;
		AddLog(_T("[INFO] LENS �ѱ� �̿Ϸ� ����"), 1, UNIT_AA1);
	}
	else
	{
		g_clTaskWork[UNIT_AA1].m_nLensLoading = 1;
		m_clColorButtonLensLoading[UNIT_AA1].SetWindowTextA(_T("LENS �ѱ� �Ϸ�"));
		m_clColorButtonLensLoading[UNIT_AA1].state = 1;
		AddLog(_T("[INFO] LENS �ѱ� �Ϸ� ����"), 1, UNIT_AA1);
	}
	m_clColorButtonLensLoading[UNIT_AA1].Invalidate();
}


void CAutoInspDlg::OnBnClickedButtonLensLoading2()
{
	if (g_clTaskWork[UNIT_AA2].m_nAutoFlag == MODE_AUTO)
	{
		AddLog(_T("[INFO] �ڵ������� ��� �Ұ�"), 1, UNIT_AA2);
		return;
	}

	if (g_clTaskWork[UNIT_AA2].m_nLensLoading == 1)
	{
		g_clTaskWork[UNIT_AA2].m_nLensLoading = 0;
		m_clColorButtonLensLoading[UNIT_AA2].SetWindowTextA(_T("LENS �ѱ� �̿Ϸ�"));
		m_clColorButtonLensLoading[UNIT_AA2].state = 0;
		AddLog(_T("[INFO] LENS �ѱ� �̿Ϸ� ����"), 1, UNIT_AA2);
	}
	else
	{
		g_clTaskWork[UNIT_AA2].m_nLensLoading = 1;
		m_clColorButtonLensLoading[UNIT_AA2].SetWindowTextA(_T("LENS �ѱ� �Ϸ�"));
		m_clColorButtonLensLoading[UNIT_AA2].state = 1;
		AddLog(_T("[INFO] LENS �ѱ� �Ϸ� ����"), 1, UNIT_AA2);
	}
	m_clColorButtonLensLoading[UNIT_AA2].Invalidate();
}


void CAutoInspDlg::OnBnClickedButtonModelSelct()
{
	//m_clModelSelectDlg.ShowWindow(SW_SHOW);
}


BOOL CAutoInspDlg::PreTranslateMessage(MSG* pMsg)
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
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoInspDlg::OnStnClickedStaticMainCurrMode1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CAMChangeHandler(0, VIDEO_CCD);
	/*if (m_bCamState[0] == VIDEO_CAM)
	{
		CAMChangeHandler(0, VIDEO_CCD);
	}
	else
	{
		CAMChangeHandler(0, VIDEO_CAM);
	}*/
	
	return;
}


void CAutoInspDlg::OnStnClickedStaticMainOutputVal1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");

	sMsg.Format(_T("[PRODUCT] ��ǰ ���� ���� �ʱ�ȭ�Ͻðڽ��ϱ�?"));

	if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
	{
		return;
	}

	g_clTaskWork[UNIT_AA1].m_nTotalWorkCount = 0;
	g_clTaskWork[UNIT_AA1].m_nTotalOkCount = 0;
	g_clTaskWork[UNIT_AA1].m_nTotalNgCount = 0;
	g_clTaskWork[UNIT_AA1].SaveData();
	this->ShowOutputCount(UNIT_AA1);
}


void CAutoInspDlg::OnStnClickedStaticMainPinVal1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");

	sMsg.Format(_T("[PRODUCT COUNT] ������ ��뷮 �ʱ�ȭ�����Ͻðڽ��ϱ�?"));

	if (g_ShowMsgModal(_T("Ȯ��"), sMsg, RGB_COLOR_RED) == false)
	{
		return;
	}

	g_clTaskWork[UNIT_AA1].m_nUsePinCount = 0;
	g_clTaskWork[UNIT_AA1].PinSaveData();
	this->ShowPinCount(UNIT_AA1);
}



bool CAutoInspDlg::CDP800_Connect()
{
#if 0
	// TODO: Add your control notification handler code here
	ViStatus status;
	ViSession defaultRM;
	ViString expr = "?*";
	ViPFindList findList = new unsigned long;
	ViPUInt32 retcnt = new unsigned long;
	ViChar instrDesc[1000];
	CString strSrc = "";
	CString strInstr = "";
	unsigned long i = 0;
	bool bFindDP = false;
	m_strInstrAddr.Empty();
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		// Error Initializing VISA...exiting
		//MessageBox("No VISA instrument was opened ! ");
		AddLog(_T("No VISA instrument was opened !"), 0, 0);
		return false;
	}
	memset(instrDesc, 0, 1000);
	// Find resource
	status = viFindRsrc(defaultRM, expr, findList, retcnt, instrDesc);
	for (i = 0; i < (*retcnt); i++)
	{
		// Get instrument name
		strSrc.Format("%s", instrDesc);
		InstrWrite(strSrc, "*IDN?");
		::Sleep(200);
		InstrRead(strSrc, &strInstr);
		// If the instrument(resource) belongs to the DP series then jump out from the loop
		strInstr.MakeUpper();
		if (strInstr.Find("DP") >= 0)
		{
			//putListLog(_T("Current measurement ���� ����"));
			//AddLog(_T("Current measurement ���� ����"), 0, 0);
			bFindDP = true;
			m_strInstrAddr = strSrc;
			//putListLog(strSrc);
			AddLog(strSrc, 0, 0);
			break;
		}
		//Find next instrument
		status = viFindNext(*findList, instrDesc);
	}
	if (bFindDP == true)
	{
		CurrentPowerOnOff(false);
	}
	UpdateData(false);
	return bFindDP;
#endif
	return false;
}


bool CAutoInspDlg::CurrentPowerOnOff(bool _Work)
{
#if 0
	CString strLog = _T("");
	CString strInstr = _T("");
	bool rtn = true;
	//��������

	//rtn = theApp.MainDlg->InstrWrite(theApp.MainDlg->m_strInstrAddr, _T(":PRES:USER1:SET:VOLT 5"));  //volt ����
	//rtn = theApp.MainDlg->InstrWrite(theApp.MainDlg->m_strInstrAddr, _T(":VOLT 0"));  //voltage ����
	//rtn = theApp.MainDlg->InstrWrite(theApp.MainDlg->m_strInstrAddr, _T(":VOLT:STEP 0.1"));  //voltage ����
	//return true;
	//: PRES : USER1 : SET : VOLT 5
	if (_Work == true)
	{
		rtn = InstrWrite(m_strInstrAddr, _T(":OUTP CH1,ON"));// _T(":MEAS:ALL? CH1"));//11.9993,0.1925,2310
	}
	else {
		rtn = InstrWrite(m_strInstrAddr, _T(":OUTP CH1,OFF"));// _T(":MEAS:ALL? CH1"));//11.9993,0.1925,2310
	}

	if (rtn)
	{
		InstrWrite(m_strInstrAddr, _T(":VOLT 13.5"));  //voltage ����
		::Sleep(200);
		rtn = InstrRead(m_strInstrAddr, &strInstr);

		strInstr.Replace("\n", "");
		//int index = 1;		//1 = A��� , 2�� W���
		//AfxExtractSubString(current, strInstr, index, ',');
		//MesProcess.dMES_Current = atof(current);
		////theApp.MainDlg->putListLog(_T("��������: ") + current);
		//double cSpecMin = ConfigData.m_Current_Min;
		//double cSpecMax = ConfigData.m_Current_Max;
		////���� ���� üũ
		//if (MesProcess.dMES_Current < cSpecMin || MesProcess.dMES_Current > cSpecMax)
		//{
		//	rtn = false;
		//	strLog.Format(_T("[Current]Spec NG :%.3lf[%.3lf~%.3lf]"), MesProcess.dMES_Current, cSpecMin, cSpecMax);
		//}
		//else
		//{
		//	strLog.Format(_T("[Current]Spec OK :%.3lf[%.3lf~%.3lf]"), MesProcess.dMES_Current, cSpecMin, cSpecMax);
		//}
		//theApp.MainDlg->putListLog(strLog);
	}

	strLog.Empty();
	strInstr.Empty();
	return rtn;
#endif
	return false;
}


bool CAutoInspDlg::InstrRead(CString strAddr, CString *pstrResult)
//Read from the instrument
{
#if 0
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char * SendAddr = NULL;
	unsigned char RecBuf[MAX_REC_SIZE];
	bool bReadOK = true;
	CString str;
	//Change the address's data style from CString to char*
	SendAddr = strAddr.GetBuffer(strAddr.GetLength());
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();
	memset(RecBuf, 0, MAX_REC_SIZE);
	//open the VISA instrument 
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		bReadOK = false;
		// Error Initializing VISA...exiting
		//AfxMessageBox("No VISA instrument was opened !");
		//return false;
	}
	//open the instrument
	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);
	//read from the instrument
	status = viRead(instr, RecBuf, MAX_REC_SIZE, &retCount);
	//close the instrument
	status = viClose(instr);
	status = viClose(defaultRM);
	(*pstrResult).Format("%s", RecBuf);

	str.Empty();
	return bReadOK;
#endif
	return false;
}
bool CAutoInspDlg::InstrWrite(CString strAddr, CString strContent) //write function
{
#if 0
	ViSession defaultRM, instr;
	ViStatus status;
	ViUInt32 retCount;
	char * SendBuf = NULL;
	char * SendAddr = NULL;
	bool bWriteOK = true;
	CString str;
	//Change the address's data style from CString to char*
	SendAddr = strAddr.GetBuffer(strAddr.GetLength());
	strcpy(SendAddr, strAddr);
	strAddr.ReleaseBuffer();
	//Change the command's data style from CString to char*
	SendBuf = strContent.GetBuffer(strContent.GetLength());
	strcpy(SendBuf, strContent);
	strContent.ReleaseBuffer();
	//open the VISA instrument
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		bWriteOK = false;
		//AfxMessageBox("No VISA instrument was opened !")
	}

	status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);
	//write command to the instrument
	status = viWrite(instr, (unsigned char *)SendBuf, strlen(SendBuf), &retCount);
	//close the instrument

	status = viClose(instr);
	status = viClose(defaultRM);
	return bWriteOK;
#endif
	return false;
}

bool CAutoInspDlg::CurrentInsp()
{
#if 0
	TCHAR szLog[SIZE_OF_1K];
	CString strInstr = _T("");
	CString current = _T("");
	bool rtn = true;
	//��������
	rtn = InstrWrite(m_strInstrAddr, _T(":MEAS:ALL? CH1"));//11.9993,0.1925,2310
														   //InstrWrite(m_strInstrAddr, _T(":VOLT 12"));  //voltage ����
	if (rtn)
	{
		::Sleep(200);
		rtn = InstrRead(m_strInstrAddr, &strInstr);
		strInstr.Replace("\n", "");
		int index = 1;		//1 = A��� , 2�� W���
		AfxExtractSubString(current, strInstr, index, ',');

		double valCurrent = (atof(current)) * 1000;		//1000���ϱ� LGIT��û

		g_clMandoInspLog[0].m_dCurrent = valCurrent;
		g_clMesCommunication[0].m_dMesCurrent = valCurrent;
		g_clMesCommunication[0].m_nMesCurrentResult = 1;
		
		//double cSpecMin = g_clModelData[0].m_CurrentSpec[0];
		//double cSpecMax = g_clModelData[0].m_CurrentSpec[1];

		double cSpecMin = (_ttof(EEpromVerifyData.vMinData[0]));
		double cSpecMax = (_ttof(EEpromVerifyData.vMaxData[0]));

		if (valCurrent < cSpecMin || valCurrent > cSpecMax)
		{
			g_clMesCommunication[0].m_nMesFinalResult = 0;	//CURRENT//XX
			g_clTaskWork[0].m_bOutputCheck[13] = false;		//��������
			g_clMesCommunication[0].m_nMesCurrentResult = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("CURRENT: %.3lf Spec Out[%.3lf ~ %.3lf] "), valCurrent, cSpecMin, cSpecMax);
			AddLog(szLog, 0, 0);


			g_clMandoInspLog[0].m_sNGList += _T(" [CURRENT NG]");
			g_clMandoInspLog[0].m_bInspRes = false;
			if (g_clMandoInspLog[0].m_nNGCnt < 30)
			{
				g_clMandoInspLog[0].m_sDispNG[g_clMandoInspLog[0].m_nNGCnt].Format(_T("NG [CURRENT NG]"));
				g_clMandoInspLog[0].m_nNGCnt++;
			}
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("CURRENT: %.3lf Spec In[%.3lf ~ %.3lf] "), valCurrent, cSpecMin, cSpecMax);
			AddLog(szLog, 0, 0);
		}
	}
	else
	{
		g_clMesCommunication[0].m_nMesFinalResult = 0;	//CURRENT//XX
		g_clTaskWork[0].m_bOutputCheck[13] = false;		//��������
		g_clMesCommunication[0].m_nMesCurrentResult = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("CURRENT TEST FAIL"));
		AddLog(szLog, 0, 0);
	}

	strInstr.Empty();
	current.Empty();
	return rtn;
#endif
	return false;
}

void CAutoInspDlg::OnStnClickedStaticMainCurrMode3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)	
	CAMChangeHandler(0, VIDEO_CAM);
#endif
	/*if (m_bCamState[0] == VIDEO_CAM)
	{
		CAMChangeHandler(0, VIDEO_CCD);
	}
	else
	{
		CAMChangeHandler(0, VIDEO_CAM);
	}*/

	return;
}


void CAutoInspDlg::OnBnClickedButtonMainDoor1()
{
	// TODO: Add your control notification handler code here
}
