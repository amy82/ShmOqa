// TeachingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "TeachingDlg.h"
#include "afxdialogex.h"

#define	WM_UI_TIMER			WM_USER + 990
#define WM_JOG_TIMER		WM_USER + 995

// CTeachingDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTeachingDlg, CDialogEx)

CTeachingDlg::CTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeachingDlg::IDD, pParent)
{
	int i;

	m_nUnit = UNIT_AA1;

	m_nSelectAxis = MOTOR_PCB_X;

	
	m_dJogSpeed = MOTOR_JOG_LOW;
	m_bMode = false;
	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		m_bPrevHomeState[i] = false;
		m_bPrevAlarm[i] = false;
		m_bPreNegLimit[i] = false;
		m_bPrePosLimit[i] = false;
		m_bPrevHome[i] = false;
		m_bPrevServoState[i] = false;

		m_bJogPosDir[i] = false;
		m_bPrevJogPosDir[i] = false;
		m_bJogNegDir[i] = false;
		m_bPrevJogNegDir[i] = false;
	}
}

CTeachingDlg::~CTeachingDlg()
{
}

void CTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEACHING_TITLE, m_clColorStaticTitle);
	DDX_Control(pDX, IDC_STATIC_TEACHING_MOVE_VALUE, m_clColorStaticMoveVal);
	DDX_Control(pDX, IDC_STATIC_TEACHING_SELECT_AXIS, m_clColorStaticSelectedAxis);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_SERVO_ON, m_clColorButtonServoOn);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_SERVO_OFF, m_clColorButtonServoOff);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_SERVO_RESET, m_clColorButtonServoReset);

	DDX_Control(pDX, IDC_BUTTON_TEACHING_ALL_SERVO_ON,		m_clColorButtonAllServoOn);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_ALL_SERVO_OFF,		m_clColorButtonAllServoOff);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_ALL_SERVO_RESET,	m_clColorButtonAllServoReset);


	

	DDX_Control(pDX, IDC_BUTTON_TEACHING_SPEED_LOW, m_clColorButtonSpeedLow);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_SPEED_MID, m_clColorButtonSpeedMid);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_SPEED_HIGH, m_clColorButtonSpeedHigh);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_CHANGE_MODE, m_clColorButtonChangeMode);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_CHANGE_MODE2, m_clColorButtonChangeMode2);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_JOG_MINUS, m_clColorButtonJogMinus);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_JOG_STOP, m_clColorButtonJogStop);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_JOG_PLUS, m_clColorButtonJogPlus);	
	DDX_Control(pDX, IDC_BUTTON_TEACHING_MOVE_MINUS, m_clColorButtonMoveMinus);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_MOVE_PLUS, m_clColorButtonMovePlus);
    DDX_Control(pDX, IDC_BUTTON_TEACHING_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_CROSS, m_clColorButtonCrossImage);

	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_X, m_clColorButtonMotorX);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_Y, m_clColorButtonMotorY);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_Z, m_clColorButtonMotorZ);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_TH, m_clColorButtonMotorTH);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_TX, m_clColorButtonMotorTX);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_PCB_TY, m_clColorButtonMotorTY);

	DDX_Control(pDX, IDC_BUTTON_TEACHING_LENS_X, m_clColorButtonLensMotorX);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_LENS_Y, m_clColorButtonLensMotorY);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_LENS_Z, m_clColorButtonLensMotorZ);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_LENS_TX, m_clColorButtonLensMotorTX);
	DDX_Control(pDX, IDC_BUTTON_TEACHING_LENS_TY, m_clColorButtonLensMotorTY);

    
}


BEGIN_MESSAGE_MAP(CTeachingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_STATIC_TEACHING_GRID_TEACH, &CTeachingDlg::OnNMClickedTeach)
    //
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_TEACHING_GRID_TEACH, &CTeachingDlg::OnNMDbclickedTeach)
	//ON_NOTIFY(NM_RCLICK, IDC_STATIC_TEACHING_GRID_TEACH, &CTeachingDlg::OnNMRclickedTeach)

	ON_BN_CLICKED(IDC_BUTTON_TEACHING_JOG_STOP, &CTeachingDlg::OnBnClickedButtonTeachingJogStop)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SPEED_LOW, &CTeachingDlg::OnBnClickedButtonTeachingSpeedLow)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SPEED_MID, &CTeachingDlg::OnBnClickedButtonTeachingSpeedMid)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SPEED_HIGH, &CTeachingDlg::OnBnClickedButtonTeachingSpeedHigh)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_CHANGE_MODE, &CTeachingDlg::OnBnClickedButtonTeachingChangeMode)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_MOVE_MINUS, &CTeachingDlg::OnBnClickedButtonTeachingMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_MOVE_PLUS, &CTeachingDlg::OnBnClickedButtonTeachingMovePlus)
	ON_STN_CLICKED(IDC_STATIC_TEACHING_MOVE_VALUE, &CTeachingDlg::OnStnClickedStaticTeachingMoveValue)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SERVO_ON, &CTeachingDlg::OnBnClickedButtonTeachingServoOn)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SERVO_OFF, &CTeachingDlg::OnBnClickedButtonTeachingServoOff)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SERVO_RESET, &CTeachingDlg::OnBnClickedButtonTeachingServoReset)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_SAVE, &CTeachingDlg::OnBnClickedButtonTeachingSave)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_X, &CTeachingDlg::OnBnClickedButtonTeachingPcbX)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_Y, &CTeachingDlg::OnBnClickedButtonTeachingPcbY)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_Z, &CTeachingDlg::OnBnClickedButtonTeachingPcbZ)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_TH, &CTeachingDlg::OnBnClickedButtonTeachingPcbTh)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_TX, &CTeachingDlg::OnBnClickedButtonTeachingPcbTx)
    ON_BN_CLICKED(IDC_BUTTON_TEACHING_PCB_TY, &CTeachingDlg::OnBnClickedButtonTeachingPcbTy)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_CROSS, &CTeachingDlg::OnBnClickedButtonTeachingCross)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_LENS_X, &CTeachingDlg::OnBnClickedButtonTeachingLensX)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_LENS_Y, &CTeachingDlg::OnBnClickedButtonTeachingLensY)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_LENS_Z, &CTeachingDlg::OnBnClickedButtonTeachingLensZ)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_LENS_TX, &CTeachingDlg::OnBnClickedButtonTeachingLensTx)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_LENS_TY, &CTeachingDlg::OnBnClickedButtonTeachingLensTy)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_CHANGE_MODE2, &CTeachingDlg::OnBnClickedButtonTeachingChangeMode2)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_ALL_SERVO_ON, &CTeachingDlg::OnBnClickedButtonTeachingAllServoOn)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_ALL_SERVO_OFF, &CTeachingDlg::OnBnClickedButtonTeachingAllServoOff)
	ON_BN_CLICKED(IDC_BUTTON_TEACHING_ALL_SERVO_RESET, &CTeachingDlg::OnBnClickedButtonTeachingAllServoReset)
END_MESSAGE_MAP()


// CTeachingDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnPaint()
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
BOOL CTeachingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();
	
	// �׸��� �ʱ�ȭ
	this->InitGridCtrl();

	// ���õ� �� ǥ��
	this->ShowSelectedAxis();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CTeachingDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	m_clColorStaticTitle.SetBkColor(RGB_CTRL_BG);
	m_clColorStaticTitle.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticTitle.SetFont(&m_clFontBig);
    m_clColorStaticTitle.SetFontBold(TRUE);
    m_clColorStaticTitle.SetBorder(FALSE);
    m_clColorStaticTitle.SetFontSize(15);

	m_clColorStaticMoveVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticMoveVal.SetFont(&m_clFontMid);

	m_clColorStaticSelectedAxis.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticSelectedAxis.SetTextColor(RGB_DLG_BG);
	m_clColorStaticSelectedAxis.SetFont(&m_clFontBig);	
    m_clColorStaticSelectedAxis.SetFontSize(30);
    m_clColorStaticSelectedAxis.SetFontBold(TRUE);

    m_clColorButtonSpeedLow.state = 1;
	m_clColorButtonChangeMode.state = 1;

	//ȭ�� �� PCB X,Y,TH 3��
	//m_clColorButtonMotorY.SetWindowTextA(_T("PCB X"));
	//m_clColorButtonMotorTH.SetWindowTextA(_T("PCB Y"));
	//m_clColorButtonMotorZ.SetWindowTextA(_T("PCB TH"));
	//m_clColorButtonLensMotorX.ShowWindow(SW_HIDE);
	//m_clColorButtonLensMotorY.ShowWindow(SW_HIDE);
	//m_clColorButtonLensMotorZ.ShowWindow(SW_HIDE);
	m_clColorButtonLensMotorTX.ShowWindow(SW_HIDE);
	m_clColorButtonLensMotorTY.ShowWindow(SW_HIDE);

	/*m_clColorButtonPcbOCFor.SetWindowTextA(_T("LENS GRIP"));
	m_clColorButtonPcbOCBack.SetWindowTextA(_T("LENS UNGRIP"));

	m_clColorButtonLensGripFor.ShowWindow(SW_HIDE);
	m_clColorButtonLensGripBack.ShowWindow(SW_HIDE);*/



}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CTeachingDlg::SetUnit(int nUnit)
{
	TCHAR szTitle[SIZE_OF_100BYTE];
	m_nUnit = nUnit;
	_stprintf_s(szTitle, SIZE_OF_100BYTE, _T("[TEACHING]")); //, m_nUnit + 1);
	GetDlgItem(IDC_STATIC_TEACHING_TITLE)->SetWindowText(szTitle);

	this->ShowTeachingData();
}

//-----------------------------------------------------------------------------
//
// �׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CTeachingDlg::InitGridCtrl()
{
	CRect rect;	
	int i, j;
    int GRID_DLG = IDC_STATIC_TEACHING_GRID_TEACH;
    CWnd *pWnd = (CWnd*)GetDlgItem(GRID_DLG);
	GridRow = 18;		//17 + 2;//�Ʒ���
	
	GridCol = MAX_MOTOR_COUNT + 1;	//������
    int margin = 4;
    int gridHeight = 38;
    int gridWidth1 = 110;
    int gridWidth2 = 80;
    int totalWidth = gridWidth1 + (gridWidth2*(GridCol - 1));
	int nSizeGridRow = GridRow - 2; // 	- ������
	GetDlgItem(GRID_DLG)->GetWindowRect(rect);
	ScreenToClient(rect);

    rect.right = totalWidth + margin;
    //rect.bottom = (gridHeight*nSizeGridRow) + margin;
	rect.bottom = (gridHeight * GridRow) + margin;
    pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.

    GetDlgItem(GRID_DLG)->GetWindowRect(rect);
    ScreenToClient(rect);
	m_clGridTeach.Create(rect, this, GRID_DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridTeach.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridTeach.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridTeach.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridTeach.SetReference_Setting();
	m_clGridTeach.EnableSelection(FALSE);
				 
	m_clGridTeach.SetRowCount(GridRow);
	m_clGridTeach.SetColumnCount(GridCol);
	m_clGridTeach.SetFixedRowCount(1);
	m_clGridTeach.SetFixedColumnCount(1);

	// Teaching Position
	m_clGridTeach.SetItemText(0, 0, _T(""));
	m_clGridTeach.SetItemText(1, 0, _T("����"));
	m_clGridTeach.SetItemText(2, 0, _T("ServoOn"));
	m_clGridTeach.SetItemText(3, 0, _T("Alarm"));
	m_clGridTeach.SetItemText(4, 0, _T("Limit(+)"));
	m_clGridTeach.SetItemText(5, 0, _T("HOME"));
	m_clGridTeach.SetItemText(6, 0, _T("Limit(-)"));
	m_clGridTeach.SetItemText(7, 0, _T("�ӵ�(mm/s)"));
	m_clGridTeach.SetItemText(8, 0, _T("���ӵ�(sec)"));
    //
	m_clGridTeach.SetItemText(9, 0, _T("WaitPos"));
	m_clGridTeach.SetItemText(10, 0, _T("Align Pos"));
	m_clGridTeach.SetItemText(11, 0, _T("LoadPos"));		//���Ͽ� ��ǰ ����
	m_clGridTeach.SetItemText(12, 0, _T("UnLoadPos"));		//���Ͽ��� ��ǰ ����
	m_clGridTeach.SetItemText(13, 0, _T("LaserPos")); //_T("DarkPos"));
	m_clGridTeach.SetItemText(14, 0, _T("OC_Pos"));
	m_clGridTeach.SetItemText(15, 0, _T("ChartPos"));

	m_clGridTeach.SetItemText(16, 0, _T("���� ����"));
	m_clGridTeach.SetItemText(17, 0, _T("���� ��ġ"));//18, 0, _T("���� ��ġ"));



	for (i = 0; i < GridRow; i++)
	{
		m_clGridTeach.SetRowHeight(i, gridHeight);
		for (j = 0; j < GridCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridTeach.SetColumnWidth(j, gridWidth1);
				}					
				else
				{
					m_clGridTeach.SetColumnWidth(j, gridWidth2);
					m_clGridTeach.SetItemText(i, j, MOTOR_NAME[j - 1]);
				}					
			}

			m_clGridTeach.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

//-----------------------------------------------------------------------------
//
//	�ӵ�/���ӵ� <-> ������ LIMIT ����
//
//-----------------------------------------------------------------------------
void CTeachingDlg::ChangeMode()
{
	int i;
	m_clColorButtonChangeMode.state = 0;
	m_clColorButtonChangeMode2.state = 0;
	if (m_bMode == false)
	{
		m_clColorButtonChangeMode.state = 1;
	}
	else
	{
		m_clColorButtonChangeMode2.state = 1;
	}
	m_clColorButtonChangeMode.Invalidate();
	m_clColorButtonChangeMode2.Invalidate();
	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		// �ӵ�/���ӵ� ǥ��
		if (m_bMode == false)
		{
			m_clGridTeach.SetItemText(7, 0, _T("�ӵ�"));
			m_clGridTeach.SetItemText(8, 0, _T("���ӵ�"));

			// �ӵ�
			m_clGridTeach.SetItemText(7, (i) + 1, g_clSysData.m_dMotorSpeed[m_nUnit][i], 3);
			// ���ӵ�
			m_clGridTeach.SetItemText(8, (i) + 1, g_clSysData.m_dMotorAccTime[m_nUnit][i], 3);
		}
		// ������ Limit ǥ��
		else
		{
			m_clGridTeach.SetItemText(7, 0, _T("OK LIMIT"));
			m_clGridTeach.SetItemText(8, 0, _T("ERR LIMIT"));

			// OK Limit
			m_clGridTeach.SetItemText(7, (i) + 1, g_clModelData[m_nUnit].m_dOkLimit[i], 3);
			// ERR Limit
			m_clGridTeach.SetItemText(8, (i) + 1, g_clModelData[m_nUnit].m_dErrLimit[i], 3);
		}
	}

	m_clGridTeach.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	ƼĪ ������ ǥ��
//
//-----------------------------------------------------------------------------
void CTeachingDlg::ShowTeachingData()
{
	int i, j;
	int gridHeight = 36;

	CRect rect;
	int GRID_DLG = IDC_STATIC_TEACHING_GRID_TEACH;
	CWnd *pWnd = (CWnd*)GetDlgItem(GRID_DLG);
	int margin = 4;

	int gridWidth1 = 110;
	int gridWidth2 = 55;// 74;
	int totalWidth = gridWidth1 + (gridWidth2*(GridCol - 1));
	int nSizeGridRow = GridRow - 2; // 	- ������

	/*if (m_nUnit == 1)
		nSizeGridRow = nSizeGridRow - 5;*/

	GetDlgItem(GRID_DLG)->GetWindowRect(rect);
	ScreenToClient(rect);

	TCHAR szLog[SIZE_OF_1K];
	for (j = 1; j < MAX_MOTOR_COUNT + 1; j++)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%d]%s"), MOTOR_AXIS_NUMBER[(j - 1) + (m_nUnit * MAX_MOTOR_COUNT)], MOTOR_NAME[j - 1]);
		m_clGridTeach.SetItemText(0, j, szLog);// MOTOR_NAME[j - 1]);
	}

	for (i = 6; i < GridRow; i++)
	{
		for (j = 0; j < MAX_MOTOR_COUNT; j++)
		{			
			switch (i)
			{
			// �ӵ�
			case 7:
				if(m_bMode == false)
					m_clGridTeach.SetItemText(i, (j) + 1, g_clSysData.m_dMotorSpeed[m_nUnit][j], 3);
				else
					m_clGridTeach.SetItemText(i, (j) + 1, g_clModelData[m_nUnit].m_dOkLimit[j], 3);
				break;
			// ���ӵ�
			case 8:
				if(m_bMode == false)
					m_clGridTeach.SetItemText(i, (j) + 1, g_clSysData.m_dMotorAccTime[m_nUnit][j], 3);
				else
					m_clGridTeach.SetItemText(i, (j) + 1, g_clModelData[m_nUnit].m_dErrLimit[j], 3);
				break;
			// �����ġ
			case 9:		
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// SENSOR ALIGN ��ġ
			case 10:
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[SENSOR_ALIGN_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// LENS,PCB �ε� ��ġ
			case 11:
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// PCB ���� ��ġ
			case 12:
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[UNLOAD_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// LASER ��ġ
			case 13:	
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[LASER_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// ���ձ��� ��ġ
			case 14:	
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[OC_LIGHT_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
            // CHART_POS ��ġ
            case 15:	
				m_clGridTeach.SetItemText(i, (j) + 1, (g_clModelData[m_nUnit].m_stTeachData[CHART_POS].dPos[j] - g_clSysData.m_dOrgDataset[m_nUnit][j]), 3);
				break;
			// ��������
			case 16:
				m_clGridTeach.SetItemText(i, (j) + 1, g_clSysData.m_dOrgDataset[m_nUnit][j], 3);
				break;
			}	
		}
	} 
	m_clGridTeach.Invalidate();
} 

//-----------------------------------------------------------------------------
//
//	���� ���� ǥ��
//
//-----------------------------------------------------------------------------
void CTeachingDlg::ShowMotorState()
{
	COLORREF color;
	CString szMotorPos = _T("");
	CString szTeachPos = _T("");
	CString szMotorNum;
	bool bState = false;
	bool bChange = false;
	int i;

#ifndef ON_LINE_MOTOR
	return;
#endif
	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		// ����
		bState = g_clMotorSet.GetHomeState(m_nUnit, i);
		if(bState != m_bPrevHomeState[i])
		{
			m_bPrevHomeState[i] = bState;

			color = (bState == true) ? RGB_COLOR_GREEN : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(1, (i ) + 1, color);

			bChange = true;
		}

		// SERVO ON
		bState = g_clMotorSet.GetServoOn(m_nUnit, i);
		if (bState != m_bPrevServoState[i])
		{
			m_bPrevServoState[i] = bState;

			color = (bState == true) ? RGB_COLOR_GREEN : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(2, (i)+1, color);

			bChange = true;
		}
		// Alarm
		bState = g_clMotorSet.GetAmpFault(m_nUnit, i);
		if(bState != m_bPrevAlarm[i])
		{
			m_bPrevAlarm[i] = bState;

			color = (bState == true) ? RGB_COLOR_RED : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(3, (i ) + 1, color);

			bChange = true;
		}

		// LIMIT(+)
		bState = g_clMotorSet.GetPosiSensor(m_nUnit, i);
		if (bState != m_bPrePosLimit[i])
		{
			m_bPrePosLimit[i] = bState;

			color = (bState == true) ? RGB_COLOR_RED : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(4, (i ) + 1, color);

			bChange = true;
		}

		// HOME
        
        bState = g_clMotorSet.GetHomeSensor(m_nUnit, i);
        if (bState != m_bPrevHome[i])
        {
            m_bPrevHome[i] = bState;
			
			color = (bState == true) ? RGB_COLOR_GREEN : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(5, (i ) + 1, color);

            bChange = true;
        }
		
		// LIMIT(-)
		bState = g_clMotorSet.GetNegaSensor(m_nUnit, i);
		if (bState != m_bPreNegLimit[i])
		{
			m_bPreNegLimit[i] = bState;

			color = (bState == true) ? RGB_COLOR_RED : RGB_COLOR_WHITE;
			m_clGridTeach.SetItemBkColor(6, (i ) + 1, color);
			/*if (i == MOTOR_PCB_Z)
			{
				color = (bState == true) ? RGB_COLOR_GREEN : RGB_COLOR_WHITE;
				m_clGridTeach.SetItemBkColor(4, (i - MOTOR_PCB_X) + 1, color);

			}
			else
			{
				color = (bState == true) ? RGB_COLOR_RED : RGB_COLOR_WHITE;
				m_clGridTeach.SetItemBkColor(5, (i - MOTOR_PCB_X) + 1, color);
			}*/
			

			bChange = true;
		}
		
	
		// ���� ��ġ
		szMotorPos.Format(_T("%.03lf"), g_clMotorSet.GetEncoderPos(m_nUnit, i) - g_clSysData.m_dOrgDataset[m_nUnit][i]);
		szTeachPos = m_clGridTeach.GetItemText(GridRow-1, (i ) + 1);
		if (szMotorPos != szTeachPos)
		{
			m_clGridTeach.SetItemText(GridRow-1, (i ) + 1, szMotorPos);

			bChange = true;
		}

		// SERVO ON ����
		//bState = g_clMotorSet.GetAmpEnable(m_nUnit, i);
		//if (bState != m_bPrevServoState[i])
		//{
		//	m_bPrevServoState[i] = bState;
		//	color = (bState == true) ? RGB_COLOR_GREEN : RGB_COLOR_RED;
		//	//m_clGridTeach.SetItemBkColor(18, (i - MOTOR_PCB_X) + 1, color);
		//	//m_clGridTeach.SetItemFgColor(18, (i - MOTOR_PCB_X) + 1, RGB_COLOR_BLACK);
		//	bChange = true;
		//}

		//szMotorNum.Format(_T("%d"), ((i - 1) + (m_nUnit * MAX_MOTOR_COUNT)) + 1);
		//m_clGridTeach.SetItemText(18, (i - MOTOR_PCB_X) + 1, szMotorNum);
	}

	if (bChange == true)
		m_clGridTeach.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	���õ� �� �̸� ǥ��
//
//-----------------------------------------------------------------------------
void CTeachingDlg::ShowSelectedAxis()
{
	if (m_nSelectAxis >= MOTOR_PCB_X && m_nSelectAxis < MAX_MOTOR_COUNT)
	{
		m_clColorStaticSelectedAxis.SetWindowText(MOTOR_NAME[m_nSelectAxis]);
	}

	
}

//-----------------------------------------------------------------------------
//
// JOG �̵�
//
//-----------------------------------------------------------------------------
void CTeachingDlg::MoveMotorJog()
{
	// + ����
	if (m_bJogPosDir[m_nSelectAxis] == true && m_bPrevJogPosDir[m_nSelectAxis] == false)
	{
		g_clMotorSet.JogMove(m_nUnit, m_nSelectAxis, g_clSysData.m_dMotorSpeed[m_nUnit][m_nSelectAxis] * g_clSysData.m_dMotorResol[m_nUnit][m_nSelectAxis] * m_dJogSpeed);
	}
	else if(m_bJogPosDir[m_nSelectAxis] == false && m_bPrevJogPosDir[m_nSelectAxis] == true)
	{
		g_clMotorSet.JogStop(m_nUnit, m_nSelectAxis);
	}
	m_bPrevJogPosDir[m_nSelectAxis] = m_bJogPosDir[m_nSelectAxis];

	// - ����
	if (m_bJogNegDir[m_nSelectAxis] == true && m_bPrevJogNegDir[m_nSelectAxis] == false)
	{
		g_clMotorSet.JogMove(m_nUnit, m_nSelectAxis, (g_clSysData.m_dMotorSpeed[m_nUnit][m_nSelectAxis] * g_clSysData.m_dMotorResol[m_nUnit][m_nSelectAxis] * m_dJogSpeed) * -1);
	}
	else if (m_bJogNegDir[m_nSelectAxis] == false && m_bPrevJogNegDir[m_nSelectAxis] == true)
	{
		g_clMotorSet.JogStop(m_nUnit, m_nSelectAxis);
	}
	m_bPrevJogNegDir[m_nSelectAxis] = m_bJogNegDir[m_nSelectAxis];
}

//-----------------------------------------------------------------------------
//
// ��ư : JOG STOP
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingJogStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	g_clMotorSet.JogStop(m_nUnit, m_nSelectAxis);
}

//-----------------------------------------------------------------------------
//
// ��ư : ����
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingSpeedLow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_dJogSpeed = MOTOR_JOG_LOW;
	m_clColorButtonSpeedLow.state = 1;
	m_clColorButtonSpeedMid.state = 0;
	m_clColorButtonSpeedHigh.state = 0;

    m_clColorButtonSpeedLow.Invalidate();
    m_clColorButtonSpeedMid.Invalidate();
    m_clColorButtonSpeedHigh.Invalidate();
}

//-----------------------------------------------------------------------------
//
// ��ư : �߼�
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingSpeedMid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_dJogSpeed = MOTOR_JOG_MID;
	m_clColorButtonSpeedLow.state = 0;
	m_clColorButtonSpeedMid.state = 1;
	m_clColorButtonSpeedHigh.state = 0;
    m_clColorButtonSpeedLow.Invalidate();
    m_clColorButtonSpeedMid.Invalidate();
    m_clColorButtonSpeedHigh.Invalidate();
}

//-----------------------------------------------------------------------------
//
// ��ư : ���
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingSpeedHigh()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_dJogSpeed = MOTOR_JOG_HIGH;
	m_clColorButtonSpeedLow.state = 0;
	m_clColorButtonSpeedMid.state = 0;
	m_clColorButtonSpeedHigh.state = 1;
    m_clColorButtonSpeedLow.Invalidate();
    m_clColorButtonSpeedMid.Invalidate();
    m_clColorButtonSpeedHigh.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	��ư : �ӵ�/���ӵ�
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingChangeMode()
{
	m_bMode = false;
	this->ChangeMode();
}

//-----------------------------------------------------------------------------
//
// ��ư : JOG -
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingMoveMinus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	/*if (m_nSelectAxis != MOTOR_PCB_Z) 
	{
		if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
		{
			g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
			return;
		}
	}*/
	TCHAR szLog[SIZE_OF_1K];


	CString sPos = _T("");
	double dMovePos = 0.0;

	GetDlgItem(IDC_STATIC_TEACHING_MOVE_VALUE)->GetWindowText(sPos);
	dMovePos = _ttof((TCHAR*)(LPCTSTR)sPos);

	g_clMotorSet.MoveFromAbsRel(m_nUnit, m_nSelectAxis, dMovePos, false);

	if(fabs(dMovePos) > 0.0)
	{
		if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_READY)
		{
			//
			g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_STOP;
            g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].state = 0;
            g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].state = 1;
            g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].Invalidate();
            g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].Invalidate();
            
		}
	}
}

//-----------------------------------------------------------------------------
//
// ��ư : JOG +
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingMovePlus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	/*if (m_nSelectAxis != MOTOR_PCB_Z)
	{
		if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
		{
			g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
			return;
		}
	}*/
	TCHAR szLog[SIZE_OF_1K];

	CString sPos = _T("");
	double dMovePos = 0.0;

	GetDlgItem(IDC_STATIC_TEACHING_MOVE_VALUE)->GetWindowText(sPos);
	dMovePos = _ttof((TCHAR*)(LPCTSTR)sPos);

	g_clMotorSet.MoveFromAbsRel(m_nUnit, m_nSelectAxis, dMovePos, true);

	if(fabs(dMovePos) > 0.0)
	{
		if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_READY)
		{
			g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_STOP;
            g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].state = 0;
            g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].state = 1;
            g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].Invalidate();
            g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].Invalidate();
		}
	}
}

//-----------------------------------------------------------------------------
//
// STATIC : �̵���
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnStnClickedStaticTeachingMoveValue()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticMoveVal.GetWindowText(sPos);
	CKeyPadDlg* pDlg = new CKeyPadDlg(sPos, 10);
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			if (fabs(pDlg->GetReturnVal()) > 3.0)
			{
				delete pDlg;
				return;
			}

			sPos.Format(_T("%.03lf"), pDlg->GetReturnVal());
			m_clColorStaticMoveVal.SetWindowText(sPos);
		}

		delete pDlg;
	}
}

//-----------------------------------------------------------------------------
//
// ��ư : ��ü SERVO ON
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingServoOn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;
	
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clMotorSet.AmpEnable(m_nUnit, m_nSelectAxis) == false)
	{
		sMsg.Format(_T("[FAIL] [%s] ���� SERVO ON ���� ����"), MOTOR_NAME[m_nSelectAxis]);
		AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
	}
	sMsg.Format(_T("[SUCCESS] [%s] ���� SERVO ON ���� �Ϸ�."), MOTOR_NAME[m_nSelectAxis]);
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
// ��ư : ��ü SERVO OFF
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingServoOff()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;
	
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clMotorSet.AmpDisable(m_nUnit, m_nSelectAxis) == false)
	{
		sMsg.Format(_T("[FAIL] [%s] ���� SERVO OFF ���� ����"), MOTOR_NAME[m_nSelectAxis]);
		AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
	}
	sMsg.Format(_T("[SUCCESS] [%s] ���� SERVO OFF ���� �Ϸ�."), MOTOR_NAME[m_nSelectAxis]);
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
// ��ư : ��ü SERVO RESET
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingServoReset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clMotorSet.AmpFaultReset(m_nUnit, m_nSelectAxis) == false)
	{
		sMsg.Format(_T("[FAIL] [%s] ���� ALARM RESET ���� ����"), MOTOR_NAME[m_nSelectAxis]);
		AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
	}

	sMsg.Format(_T("[SUCCESS] [%s] ���� ALARM RESET ���� �Ϸ�."), MOTOR_NAME[m_nSelectAxis]);
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
// ��ư : SAVE
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnBnClickedButtonTeachingSave()
{
	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ��� �Ұ�."), RGB_COLOR_RED);
		return;
	}
	TCHAR szLog[SIZE_OF_1K];
	_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ƼĪ �����͸� �����Ͻðڽ��ϱ�?"), g_clSysData.m_szModelName);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (g_ShowMsgModal(_T("Ȯ��"), szLog, RGB_COLOR_RED) == false)
		return;

	this->GetTeachData();

	if (m_bMode == false) 
	{
		g_clSysData.sDSave();
	}
	else 
	{
		g_clModelData[m_nUnit].Save(g_clSysData.m_szModelName);
	}
	g_clModelData[m_nUnit].SaveTeachData(g_clSysData.m_szModelName);

	AddLog(_T("[SAVE] PCB ƼĪ ������ ����"), 0, m_nUnit);
	this->ShowTeachingData();
}

//-----------------------------------------------------------------------------
//
//	�׸��� ƼĪ �����͸� �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CTeachingDlg::GetTeachData()
{
	CString sData = _T("");
	int i, j;

	for(i = 6; i < GridRow; i++)
	{
		for (j = 0; j < MAX_MOTOR_COUNT; j++)
		{
			switch (i)
			{
			// �ӵ�
			case 7:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				if (m_bMode == false)
					g_clSysData.m_dMotorSpeed[m_nUnit][j] = _ttof((TCHAR*)(LPCTSTR)sData);
				else
					g_clModelData[m_nUnit].m_dOkLimit[j] = _ttof((TCHAR*)(LPCTSTR)sData);
				break;
			// ���ӵ�
			case 8:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				if (m_bMode == false)
				{
					g_clSysData.m_dMotorAccTime[m_nUnit][j] = _ttof((TCHAR*)(LPCTSTR)sData);
					g_clSysData.m_dMotorDecTime[m_nUnit][j] = _ttof((TCHAR*)(LPCTSTR)sData);
				}
				else
				{
					g_clModelData[m_nUnit].m_dErrLimit[j] = _ttof((TCHAR*)(LPCTSTR)sData);
				}
				break;
			// �����ġ
			case 9:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
				break;
			//SENSOR ALIGN ��ġ
			case 10:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clModelData[m_nUnit].m_stTeachData[SENSOR_ALIGN_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
				break;
			//���� ��ġ
			case 11:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
				break;
			//���� ALIGN ��ġ
			case 12:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clModelData[m_nUnit].m_stTeachData[UNLOAD_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
				break;
           // LASER ��ġ
            case 13:
                sData = m_clGridTeach.GetItemText(i, j + 1);
                g_clModelData[m_nUnit].m_stTeachData[LASER_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
                break;
           // ���ձ��� ��ġ
            case 14:
                sData = m_clGridTeach.GetItemText(i, j + 1);
                g_clModelData[m_nUnit].m_stTeachData[OC_LIGHT_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
                break;
				// ��Ʈ ��ġ
			case 15:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clModelData[m_nUnit].m_stTeachData[CHART_POS].dPos[j] = _ttof((TCHAR*)(LPCTSTR)sData) + g_clSysData.m_dOrgDataset[m_nUnit][j];
				break;
			// ��������
			case 16:
				sData = m_clGridTeach.GetItemText(i, j + 1);
				g_clSysData.m_dOrgDataset[m_nUnit][j] = _ttof((TCHAR*)(LPCTSTR)sData);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//	ƼĪ �׸��� Ŭ��
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnNMClickedTeach(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString szData = _T("");
	CString sMsg = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 0 && nCol > 0)
	{
		//m_nSelectAxis = MOTOR_PCB_X + nCol - 1;
		this->ShowSelectedAxis();
	}
    int curCol = nCol;
    if (nCol == 0)
        curCol = m_nSelectAxis + 1 ;

    changeMotorNo(curCol - 1);
    if (nRow == GridRow - 1 && nCol > 0)//if (nRow == 18 && nCol > 0)
	{
		if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO || g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)		return;

		m_nSelectAxis =  nCol - 1;
		if (g_ShowMsgModal(_T("Ȯ��"), _T("���� SERVO ON �Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == true)
		{
			if (g_clMotorSet.AmpEnable(m_nUnit, m_nSelectAxis) == false)
			{
				sMsg.Format(_T("[FAIL] [%s] ���� SERVO ON ���� ����"), MOTOR_NAME[m_nSelectAxis]);
				AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
			}
		}
		else
		{
			return;
		}
	}
    if ((nRow >= 7 && nRow < GridRow-1) && nCol >= 1)
	{
		if ((nRow > 10 && nRow < 17) && nCol > 11)//6)
		{
			return;
		}

		if (nRow == 17)
		{
			CKeyBoardDlg* pDlg = new CKeyBoardDlg(20, true);
			if (pDlg != NULL)
			{
				if (pDlg->DoModal() == IDOK)
				{
					if (_tcscmp(g_clSysData.m_szPassword, (TCHAR*)(LPCTSTR)pDlg->GetReturnValue()))
					{
						AddLog(_T("��й�ȣ�� ��ġ���� �ʽ��ϴ�."), 1, m_nUnit);
						delete pDlg;

						return;
					}					
				}
				else
				{
					delete pDlg;
					return;
				}

				delete pDlg;
			}
		}

		szData = m_clGridTeach.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(szData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				m_clGridTeach.SetItemText(nRow, nCol, pDlg->GetReturnVal(), 3);
			}

			delete pDlg;
			m_clGridTeach.Invalidate();
		}		
	}
}

//-----------------------------------------------------------------------------
//
//	ƼĪ �׸��� ��Ŭ��
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnNMDbclickedTeach(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	int nRow, nCol;
	CString sData = _T("");
	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;
    if (nRow <= 0 || nCol < 0)	return;
    int curCol = nCol;
    if (nCol == 0)
    {
        curCol = m_nSelectAxis + 1;
    }

    

    //if (g_ShowMsgModal(_T("Ȯ��"), _T("���� ��ġ�� ƼĪ ��ġ�� �����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
      //  return;
	// if ((nRow >= 7
    if (nRow >= 9 && nCol == 0)   
    {
        sData = m_clGridTeach.GetItemText(GridRow - 1, curCol);
        m_clGridTeach.SetItemText(nRow, curCol, sData);
		//
		m_clGridTeach.SetItemBkColor(nRow, curCol, GRID_COLOR_GREEN);
		//
        m_clGridTeach.Invalidate();
    }
    

	/*if ((nRow >= 8 && nRow <= 15) && (nCol > 0))
	{
		if ( nRow == 9 && (nCol >= 1 && nCol < 7))
		{
			return;
		}

		if ((nRow > 10 && nRow < 16) && nCol > 6)
		{
			return;
		}

		if (g_ShowMsgModal(_T("Ȯ��"), _T("���� ��ġ�� ƼĪ ��ġ�� �����Ͻðڽ��ϱ�?"), RGB_COLOR_RED) == false)
			return;

		sData = m_clGridTeach.GetItemText(GridRow-1, nCol);
		m_clGridTeach.SetItemText(nRow, nCol, sData);
		m_clGridTeach.Invalidate();
	}*/
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		// ƼĪ ������ ǥ��
		g_pCarAABonderDlg->m_clVisionStaticCam[m_nUnit].SetDragFlag(MOUSE_DRAG);
		this->ShowTeachingData();
        changeMotorNo(m_nSelectAxis);
		SetTimer(WM_UI_TIMER, 300, NULL);
		SetTimer(WM_JOG_TIMER, 50, NULL);
	}
	else
	{
		KillTimer(WM_UI_TIMER);
		KillTimer(WM_JOG_TIMER);
	}
}

//-----------------------------------------------------------------------------
//
//	Ÿ�̸�
//
//-----------------------------------------------------------------------------
void CTeachingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case WM_UI_TIMER:
		this->ShowMotorState();
		break;
	case WM_JOG_TIMER:
		this->MoveMotorJog();	
	}

	CDialogEx::OnTimer(nIDEvent);
}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CTeachingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_ON)->m_hWnd)
		{
			//m_clColorButtonServoOn.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_OFF)->m_hWnd)
		{
			//m_clColorButtonServoOff.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_RESET)->m_hWnd)
		{
			//m_clColorButtonServoReset.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_MINUS)->m_hWnd)
		{
			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
				return TRUE;
			}

			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
				return TRUE;
			}

			/*if (g_clDioControl.DarkPusherCheck(m_nUnit, true) == false)
			{
				g_clDioControl.DarkPusher(m_nUnit, true);
				g_ShowMsgPopup(_T("ERROR"), _T("DARK CYLINDER ��� Ȯ�� ����"), RGB_COLOR_RED);
				return TRUE;
			}*/
			if (g_clMotorSet.GetPcbZMotorPosCheck(m_nUnit, WAIT_POS) == false)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("Z�� �����ġ Ȯ�� ����"), RGB_COLOR_RED);
				return TRUE;
			}
			m_bJogNegDir[m_nSelectAxis] = true;
			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_READY)
			{
				g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_STOP;
				g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].state = 0;
				g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].state = 1;
				g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].Invalidate();
				g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].Invalidate();
			}

			//m_clColorButtonJogMinus.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_STOP)->m_hWnd)
		{
			//m_clColorButtonJogStop.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_PLUS)->m_hWnd) 
		{
			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
				return TRUE;
			}

			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
				return TRUE;
			}
			/*if (g_clDioControl.DarkPusherCheck(m_nUnit, true) == false)
			{
				g_clDioControl.DarkPusher(m_nUnit, true);
				g_ShowMsgPopup(_T("ERROR"), _T("DARK CYLINDER ��� Ȯ�� ����"), RGB_COLOR_RED);
				return TRUE;
			}*/
			if (g_clMotorSet.GetPcbZMotorPosCheck(m_nUnit, WAIT_POS) == false)
			{
				g_ShowMsgPopup(_T("ERROR"), _T("Z�� �����ġ Ȯ�� ����"), RGB_COLOR_RED);
				return TRUE;
			}
			m_bJogPosDir[m_nSelectAxis] = true;
			if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_READY)
			{
				g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_STOP;
				g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].state = 0;
				g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].state = 1;
				g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].Invalidate();
				g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].Invalidate();
			}
			//m_clColorButtonJogPlus.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_MOVE_MINUS)->m_hWnd)
		{
			//m_clColorButtonMoveMinus.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_MOVE_PLUS)->m_hWnd)
		{
			//m_clColorButtonMovePlus.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SAVE)->m_hWnd)
		{
			//m_clColorButtonSave.SetSelected(true);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_CHANGE_MODE)->m_hWnd)
		{
			//m_clColorButtonChangeMode.SetSelected(true);
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		m_bJogPosDir[m_nSelectAxis] = false;
		m_bJogNegDir[m_nSelectAxis] = false;

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_ON)->m_hWnd)
		{
			//m_clColorButtonServoOn.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_OFF)->m_hWnd)
		{
			//m_clColorButtonServoOff.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SERVO_RESET)->m_hWnd)
		{
			//m_clColorButtonServoReset.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_MINUS)->m_hWnd)
		{
			//m_clColorButtonJogMinus.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_STOP)->m_hWnd)
		{
			//m_clColorButtonJogStop.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_JOG_PLUS)->m_hWnd)
		{
			//m_clColorButtonJogPlus.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_MOVE_MINUS)->m_hWnd)
		{
			//m_clColorButtonMoveMinus.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_MOVE_PLUS)->m_hWnd)
		{
			//m_clColorButtonMovePlus.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_SAVE)->m_hWnd)
		{
			//m_clColorButtonSave.SetSelected(false);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_TEACHING_CHANGE_MODE)->m_hWnd)
		{
			//m_clColorButtonChangeMode.SetSelected(false);
		}
	}


	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CTeachingDlg::changeMotorNo(int MotorNo)
{
	if (MotorNo < 0)
	{
		return;
	}
    int j = m_nSelectAxis + 1;

    int selectedStart = 7;
    for (int i = selectedStart; i <GridRow; i++)
    { 
        m_clGridTeach.SetItemBkColor(i, (m_nSelectAxis + 1), GRID_COLOR_WHITE);
    }


    m_nSelectAxis = MotorNo;
    for (int i = selectedStart; i <GridRow; i++)
    {
        m_clGridTeach.SetItemBkColor(i, (m_nSelectAxis + 1), GRID_COLOR_SELECT);
    }
    m_clGridTeach.Invalidate();
    //
	m_clColorButtonMotorX.state = 0;
	m_clColorButtonMotorY.state = 0;
	m_clColorButtonMotorZ.state = 0;
	m_clColorButtonMotorTH.state = 0;
	m_clColorButtonMotorTX.state = 0;
	m_clColorButtonMotorTY.state = 0;

	m_clColorButtonLensMotorX.state = 0;
	m_clColorButtonLensMotorY.state = 0;
	m_clColorButtonLensMotorZ.state = 0;
	m_clColorButtonLensMotorTX.state = 0;
	m_clColorButtonLensMotorTY.state = 0;


    switch (MotorNo)
    {
	case MOTOR_PCB_X:
		m_clColorButtonMotorX.state = 1;
		break;
	case MOTOR_PCB_Y:
		m_clColorButtonMotorY.state = 1;
		break;
	case MOTOR_PCB_Z:
		m_clColorButtonMotorZ.state = 1;
		break;
		
	case MOTOR_PCB_TH:
		m_clColorButtonMotorTH.state = 1;
		break;
    case MOTOR_PCB_TX:
        m_clColorButtonMotorTX.state = 1;
        break;
    case MOTOR_PCB_TY:
        m_clColorButtonMotorTY.state = 1;
        break;

	

    }
    this->ShowSelectedAxis();
    m_clColorButtonMotorX.Invalidate();
    m_clColorButtonMotorY.Invalidate();
    m_clColorButtonMotorZ.Invalidate();
    m_clColorButtonMotorTH.Invalidate();
    m_clColorButtonMotorTX.Invalidate();
    m_clColorButtonMotorTY.Invalidate();

	m_clColorButtonLensMotorX.Invalidate();
	m_clColorButtonLensMotorY.Invalidate();
	m_clColorButtonLensMotorZ.Invalidate();
	m_clColorButtonLensMotorTX.Invalidate();
	m_clColorButtonLensMotorTY.Invalidate();

}

void CTeachingDlg::OnBnClickedButtonTeachingPcbX()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int i;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }
    changeMotorNo(MOTOR_PCB_X);
}


void CTeachingDlg::OnBnClickedButtonTeachingPcbY()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int i = 0;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }
	changeMotorNo(MOTOR_PCB_Y);
    
}


void CTeachingDlg::OnBnClickedButtonTeachingPcbZ()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sMsg = _T("");
    int i;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    } 
	changeMotorNo(MOTOR_PCB_Z);
	
}


void CTeachingDlg::OnBnClickedButtonTeachingPcbTh()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int i;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }
	changeMotorNo(MOTOR_PCB_TH);
    
}


void CTeachingDlg::OnBnClickedButtonTeachingPcbTx()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int i;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }
    changeMotorNo(MOTOR_PCB_TX);
}


void CTeachingDlg::OnBnClickedButtonTeachingPcbTy()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int i;

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }

    if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
    {
        g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
        return;
    }
    changeMotorNo(MOTOR_PCB_TY);
}


void CTeachingDlg::OnBnClickedButtonTeachingCross()
{
	g_clVision.DrawMOverlayLine(m_nUnit, 0, (int)(g_clModelData[m_nUnit].m_nHeight / 2), g_clModelData[m_nUnit].m_nWidth, (int)(g_clModelData[m_nUnit].m_nHeight / 2), M_COLOR_RED, 1, FALSE, PS_SOLID);
	g_clVision.DrawMOverlayLine(m_nUnit, (int)(g_clModelData[m_nUnit].m_nWidth / 2), 0, (int)(g_clModelData[m_nUnit].m_nWidth / 2), g_clModelData[m_nUnit].m_nHeight, M_COLOR_RED, 1, FALSE, PS_SOLID);
	g_clVision.DrawOverlayAll(m_nUnit);
}


void CTeachingDlg::OnBnClickedButtonTeachingLensX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}
	changeMotorNo(MOTOR_PCB_TH);
}


void CTeachingDlg::OnBnClickedButtonTeachingLensY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}
	changeMotorNo(MOTOR_PCB_TX);
} 


void CTeachingDlg::OnBnClickedButtonTeachingLensZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}
	changeMotorNo(MOTOR_PCB_TY);
}


void CTeachingDlg::OnBnClickedButtonTeachingLensTx()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

}


void CTeachingDlg::OnBnClickedButtonTeachingLensTy()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

}


void CTeachingDlg::OnBnClickedButtonTeachingChangeMode2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bMode = true;
	this->ChangeMode();
}


void CTeachingDlg::OnBnClickedButtonTeachingAllServoOn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		if (g_clMotorSet.AmpEnable(m_nUnit, i) == false)
		{
			sMsg.Format(_T("[FAIL] [%s] ���� SERVO ON ���� ����"), MOTOR_NAME[i]);
			AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
		}
	}
	sMsg.Format(_T("[SUCCESS] ALL ���� SERVO ON ���� �Ϸ�."));
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}


void CTeachingDlg::OnBnClickedButtonTeachingAllServoOff()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		if (g_clMotorSet.AmpDisable(m_nUnit, i) == false)
		{
			sMsg.Format(_T("[FAIL] [%s] ���� SERVO OFF ���� ����"), MOTOR_NAME[i]);
			AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
		}
	}
	sMsg.Format(_T("[SUCCESS] ALL ���� SERVO OFF ���� �Ϸ�."));
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}


void CTeachingDlg::OnBnClickedButtonTeachingAllServoReset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("");
	int i;

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		return;
	}

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		if (g_clMotorSet.AmpFaultReset(m_nUnit, i) == false)
		{
			sMsg.Format(_T("[FAIL] [%s] ���� ALARM RESET ���� ����"), MOTOR_NAME[i]);
			AddLog((TCHAR*)(LPCTSTR)sMsg, 1, m_nUnit);
		}
	}
	sMsg.Format(_T("[SUCCESS] ALL ���� ALARM RESET ���� �Ϸ�."));
	AddLog((TCHAR*)(LPCTSTR)sMsg, 0, m_nUnit);
}
