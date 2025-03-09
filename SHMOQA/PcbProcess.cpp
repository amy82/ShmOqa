#include "StdAfx.h"
#include "PcbProcess.h"

CWinThread* pThread_TaskCCD = NULL;
bool bThreadCCDRun = false;
typedef struct THREADV
{
	int t_Unit;
}_threadV;

UINT Thread_TaskCCD(LPVOID parm)
{
	_threadV* getData = (_threadV*)parm;
	bThreadCCDRun = true;
	TCHAR szLog[SIZE_OF_1K];
	Sleep(300);//Sleep(1100);
	g_clLaonGrabberWrapper[getData->t_Unit].SetCurrentState(CCD_GRAB_STOP);


	if (g_clLaonGrabberWrapper[getData->t_Unit].OpenDevice() == false)
	{
		AddLog(_T("[FAIL] Device Open Fail"), 0, getData->t_Unit, false);
	}


	Sleep(500);
	if(g_clLaonGrabberWrapper[getData->t_Unit].StartGrabCcdProcess() == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD OPEN 실패. 제품 안착 상태 및 제품 불량을 확인하세요"));
		AddLog(szLog, 1, getData->t_Unit, true);
	}

	bThreadCCDRun = false;


	return 1;
}
CPcbProcess::CPcbProcess(void)
{
	m_bFlagSocketReq = false;
	m_nSocketCnt = 0;
	VaccumRetry = 0;
	TestPatternRetry = 0;
	mLightRetry = 0;
	VarRetry = 0;
	mOcLightChannel = 0;
	rawSumCount = 0;
	m_nDiffTestOk = false;
	nDiffRetry = 0;

	nLotProcessingComplete_ACK = -1;
	nRunOnlineControlState = false;
}


CPcbProcess::~CPcbProcess(void)
{
}

//-----------------------------------------------------------------------------
//
//	OQA 원점(10000 ~ 20000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::OqaHomeProcess(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	//CString sMsg = _T("");
	int nHomeFlag = 0;
	int nRetStep = 0;
	int i = 0;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	nRetStep = nStep;

	g_clTaskWork[m_nUnit].m_bAreaSensorRun = true;	// HomeProcess
	if (g_clDioControl.EziLightCurtainCheck(UNIT_AA1) == true)
	{
		g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1004);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 원점동작 중 CURTAIN 감지. 일시 정지"));
		AddLog(szLog, 1, UNIT_AA1, true);

		nRetStep *= -1;
		return nRetStep;
	}
	switch (nStep)
	{
	case 10000:
		if (g_clMesCommunication[m_nUnit].m_dEqupControlState[1] == eOnlineRemote &&
			g_clMesCommunication[m_nUnit].m_dProcessState[0] != g_clMesCommunication[m_nUnit].m_dProcessState[1])
		{
			g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
			g_clMesCommunication[m_nUnit].m_dProcessState[1] = eIDLE;
			g_clMesCommunication[m_nUnit].m_uAlarmList.clear();

			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] (Idle)Process State Change Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[0].bIdleTimeExceed = false;		//init
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401);	//SEND S6F11


			
			SYSTEMTIME SystemTime;
			::GetLocalTime(&SystemTime);

			CString strData = _T("");
			strData.Format(_T("%02d%02d%02d%02d%02d%02d"),
				SystemTime.wYear,// cTime.GetYear(),
				SystemTime.wMonth,//cTime.GetMonth(),
				SystemTime.wDay,//cTime.GetDay(),
				SystemTime.wHour,//cTime.GetHour(),
				SystemTime.wMinute,//cTime.GetMinute(),
				SystemTime.wSecond);//cTime.GetSecond());

			_stprintf_s(g_clTaskWork[m_nUnit].m_szIdleStartTime, SIZE_OF_100BYTE, _T("%s"), strData);		//Auto_M_PCBLoading
			g_pCarAABonderDlg->KillTimer(WM_IDLE_REASON_TIMER);

			if (g_clMesCommunication[m_nUnit].IdleSetTimeInterval < 1)
			{
				g_clMesCommunication[m_nUnit].IdleSetTimeInterval = 5;	//min  1min = 60000
			}
			g_pCarAABonderDlg->SetTimer(WM_IDLE_REASON_TIMER, g_clMesCommunication[m_nUnit].IdleSetTimeInterval * 60000, NULL);		//30000 Step
			strData.Empty();
		}
		nRetStep = 11200;
		break;
	case 11200:
		g_clDioControl.EziStartBtnLedOn(m_nUnit, false);
		nRetStep = 11400;
		break;
	case 11400:
		//이물 광원 상승
		
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1001);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11400;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 11600;
		break;

	case 11600:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 11650;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1001);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11400;
			break;
		}
		break;
	case 11650:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 제품 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 흡착 동작[STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clDioControl.EziVacuumOn(m_nUnit, true);
			Sleep(10);
			g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, true);

			
			nRetStep = 11700;
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 탈착 동작[STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clDioControl.EziVacuumOn(m_nUnit, false);
			Sleep(10);
			g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 제품 미확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 11800;
		}
		break;
	case 11700:
		if (g_clDioControl.EziVacuumOnCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 흡착 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 11750;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1003);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 흡착 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11650;
			break;
		}
		break;
	case 11750:
		nRetStep = 11800;
		break;

	case 11800:	//jump step
		//pcb 소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1002);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11800;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 소켓 후진 동작 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 12000;
		break;

	case 12000:
		//pcb 소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 12200;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1002);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11800;
			break;
		}
		break;
	case 12200:
		nRetStep = 12400;
		break;
	case 12400:
		
		nRetStep = 13000;
		
		break;
	case 13000:	//jump Step
		nRetStep = 18000;
		break;
	case 18000:
		//! 전체 원점 동작 완료
		g_clMotorSet.m_bOqaOrgFlag = true;
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].state = 1;
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].Invalidate();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 원점 복귀 완료[STEP : %d]"), nStep);

		AddLog(szLog, 0, m_nUnit);
		g_ShowMsgPopup(_T("INFO"), _T("[ORG] 원점 복귀 완료"), RGB_COLOR_RED);
		nRetStep = 20000;
		break;

	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ORG] 원점 복귀 이상 STEP 발생[STEP : %d]"), nStep);
		AddLog(szLog, 1, m_nUnit, true);
		nRetStep = -10000;
		break;
	}

	return nRetStep;
}

//-----------------------------------------------------------------------------
//
//	OQA 운전준비(20000 ~ 30000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::OqaAutoReadyProcess(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	int nRetStep;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	//CString sMsg = _T("");
	int i = 0;
	nRetStep = nStep;

	g_clTaskWork[m_nUnit].m_bAreaSensorRun = true;	// OqaAutoReadyProcess
	if (g_clDioControl.EziLightCurtainCheck(UNIT_AA1) == true)
	{
		g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1008);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 운전준비 중 CURTAIN 감지. 일시 정지"));
		AddLog(szLog, 1, UNIT_AA1, true);

		nRetStep *= -1;
		return nRetStep;
	}
	switch (nStep)
	{
	case 20000:
		if (g_clMesCommunication[m_nUnit].m_dEqupControlState[1] == eOnlineRemote &&
			g_clMesCommunication[m_nUnit].m_dProcessState[0] != g_clMesCommunication[m_nUnit].m_dProcessState[1])
		{
			g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
			g_clMesCommunication[m_nUnit].m_dProcessState[1] = eIDLE;
			g_clMesCommunication[m_nUnit].m_uAlarmList.clear();

			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] (Idle)Process State Change Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[0].bIdleTimeExceed = false;		//init
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401);	//SEND S6F11
			

																									//CTime cTime = CTime::GetCurrentTime();//ERRRRRRRRRRRR
			SYSTEMTIME SystemTime;
			::GetLocalTime(&SystemTime);

			CString strData = _T("");
			strData.Format(_T("%02d%02d%02d%02d%02d%02d"),
				SystemTime.wYear,// cTime.GetYear(),
				SystemTime.wMonth,//cTime.GetMonth(),
				SystemTime.wDay,//cTime.GetDay(),
				SystemTime.wHour,//cTime.GetHour(),
				SystemTime.wMinute,//cTime.GetMinute(),
				SystemTime.wSecond);//cTime.GetSecond());

			_stprintf_s(g_clTaskWork[m_nUnit].m_szIdleStartTime, SIZE_OF_100BYTE, _T("%s"), strData);		//Auto_M_PCBLoading
			g_pCarAABonderDlg->KillTimer(WM_IDLE_REASON_TIMER);

			if (g_clMesCommunication[m_nUnit].IdleSetTimeInterval < 1)
			{
				g_clMesCommunication[m_nUnit].IdleSetTimeInterval = 5;	//min  1min = 60000
			}
			g_pCarAABonderDlg->SetTimer(WM_IDLE_REASON_TIMER, g_clMesCommunication[m_nUnit].IdleSetTimeInterval * 60000, NULL);		//30000 Step
			strData.Empty();
		}
		nRetStep = 21000;
		break;
	case 21000:

		g_clDioControl.EziStartBtnLedOn(m_nUnit, false);
		//이물광원

		nRetStep = 21200;
		break;
	case 21200:
		//LIGHT_BD_OC_CH2
		BackDuLightControl[m_nUnit].ctrlLedVolume(0, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_OC_WHITE1]);
		Sleep(10);
		nRetStep = 21400;
		break;
	case 21400:
		//이물 광원 상승

		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1005);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21400;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 21600;
		break;

	case 21600:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 21650;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1005);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21400;
			break;
		}

		break;
	case 21650:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 제품 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 흡착 동작[STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clDioControl.EziVacuumOn(m_nUnit, true);
			Sleep(10);
			g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, true);


			nRetStep = 21700;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 탈착 동작[STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clDioControl.EziVacuumOn(m_nUnit, false);
			Sleep(10);
			g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 제품 미확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 21800;
		}

		break;
	case 21700:
		if (g_clDioControl.EziVacuumOnCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 흡착 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 21750;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1007);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 흡착 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21650;
			break;
		}
		break;
	case 21750:
		nRetStep = 21800;
		break;
	case 21800:	//jump step
		//pcb 소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1006);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21800;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 소켓 후진 동작 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 22000;
		break;

	case 22000:
		//pcb 소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 22200;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1006);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21800;
			break;
		}
		break;
	case 22200:
		nRetStep = 23000;
		break;

	case 23000:
		nRetStep = 29000;
		break;
	case 29000:
		g_clDioControl.EziSetEziTowerLamp(0, LAMP_YELLOW, true);
		g_clTaskWork[m_nUnit].m_nAutoFlag = MODE_READY;
		//
		//운전준비시 초기화 추가
		g_clTaskWork[m_nUnit].m_nEmission = 0;
		g_pCarAABonderDlg->ShowOkNgState(m_nUnit, g_clTaskWork[m_nUnit].m_nEmission);
		g_clTaskWork[m_nUnit].SaveData();
		//
		g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].state = 1;
		g_pCarAABonderDlg->m_clColorButtonAutoRun[m_nUnit].state = 0;
		g_pCarAABonderDlg->m_clColorButtonAutoPause[m_nUnit].state = 0;
		g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].state = 0;

		g_pCarAABonderDlg->m_clColorButtonAutoRun[m_nUnit].Invalidate();
		g_pCarAABonderDlg->m_clColorButtonAutoPause[m_nUnit].Invalidate();
		g_pCarAABonderDlg->m_clColorButtonAutoStop[m_nUnit].Invalidate();
		g_pCarAABonderDlg->m_clColorButtonAutoReady[m_nUnit].Invalidate();

		_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 운전준비 완료 [STEP : %d]"), nStep);
		g_ShowMsgPopup(_T("INFO"), _T("[READY] 운전준비 완료"), RGB_COLOR_RED, 1);

		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[READY] 운전준비 STEP 이상 [STEP : %d]"), nStep);
		AddLog(szLog, 1, m_nUnit, true);
		nRetStep = -20000;
		break;
	}
	return nRetStep;
}


//-----------------------------------------------------------------------------
//
//	OQA 이물조명 3000,5000,6500,IR 이미지로 검사(100000 ~ 110000)	
//
//-----------------------------------------------------------------------------

int CPcbProcess::OqaAuto_OC_Insp(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szPath[SIZE_OF_1K];
	CFileFind clFinder;
	CString sMsg = _T("");
	double dMoveDist = 0.0;
	double dMoveVel = 0.0;
	double dMoveAcc = 0.0;
	double dPos[3] = { 0.0, 0.0, 0.0 };
	int nRetVal = 0;
	int nRetStep = 0;
	int i = 0;
	double dCenterPosX = 0.0;
	double dCenterPosY = 0.0;
	double dZoomX = 0.0;
	double dZoomY = 0.0;
	bool bInspRtn = false;
	nRetStep = nStep;

	switch (nStep)
	{
	case 60000:
		g_pCarAABonderDlg->CAMChangeHandler(m_nUnit, VIDEO_CCD);
		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);

		g_clTaskWork[m_nUnit].m_dwTotalTactTime = GetTickCount();
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();

		BackDuLightControl[m_nUnit].ctrlLedVolume(LIGHT_BD_OC_CH1, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_OC_WHITE1]);
		nRetStep = 60200;
		break;
	case 60200:

		nRetStep = 60400;
		break;

	case 60400:
		//이물 광원 상승
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -60400;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 61000;
		break;

	case 61000:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 61500;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -60400;
			break;
		}
		break;
	case 61500:
		//소켓 전진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1015);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 전진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -61500;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 전진 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 62000;
		break;

	case 62000:
		//소켓 전진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 전진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 62200;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1015);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 전진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -61500;
			break;
		}
		break;
	case 62200:
		//이물 광원 하강
		if (g_clDioControl.EziOcUp(m_nUnit, false) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1016);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 하강 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -62200;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 하강 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 62400;
		break;
	case 62400:
		//이물 광원 하강 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 하강 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 62500;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1016);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 하강 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -62200;
			break;
		}
		break;
	case 62500:
		nRetStep = 63000;
		break;
	case 63000:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 63100;
		break;
	case 63100:
		if (g_clLaonGrabberWrapper[m_nUnit].GetCurrentState() == CCD_GRAB_LIVE)
		{
			Sleep(500);

			if (g_clModelData[m_nUnit].m_nFpsStopUse == 1)
			{
				if (g_clTaskWork[m_nUnit].m_fFps >= g_clModelData[m_nUnit].m_fFpsStopVal)
				{
					_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD IMAGE GRAB(FPS:%.f/%.f) 성공 [STEP : %d]"), g_clTaskWork[m_nUnit].m_fFps, g_clModelData[m_nUnit].m_fFpsStopVal, nStep);
					AddLog(szLog, 0, m_nUnit);
					g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
					nRetStep = 63200;
				}
				else
				{
					if (g_ShowMsgModal(_T("확인"), _T("[AUTO] FPS값이 낮습니다. 진행하시겠습니까?"), RGB_COLOR_RED) == false)
					{
						g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1017);
						_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] FPS:%.f ERROR. 제품 안착 확인하세요 [STEP : %d]"), g_clTaskWork[m_nUnit].m_fFps, nStep);
						AddLog(szLog, 1, m_nUnit, true);
						nRetStep = -63100;
						break;
					}
					else
					{
						g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
						nRetStep = 63200;
						break;
					}
				}
			}
			else
			{
				g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
				nRetStep = 63200;
				break;
			}
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 10000)	//10초
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1018);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD OPEN 실패. 제품 안착 상태 및 제품 불량을 확인하세요. [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -63100;
			break;
		}

		break;
	case 63200:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 63500;
		break;
	case 63500:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			nRetStep = 64000;
		}
		break;
	case 64000:
		g_clApsInsp[m_nUnit].func_Insp_Version_Read();//OQA raw
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]VERSION READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64050;
		break;
	case 64050:
		nRetStep = 64100;
		break;
	case 64100:
		g_clApsInsp[m_nUnit].func_Insp_Supply_Voltage_Read(true);		//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SUPPLY VOLTAGE READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64150;
		break;
	case 64150:
		nRetStep = 64200;
		break;
	case 64200:
		g_clApsInsp[m_nUnit].func_Insp_Sensor_Voltage_Read(true);		//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SENSOR VOLTAGE READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64250;
		break;
	case 64250:
		nRetStep = 64300;
		break;
	case 64300:
		g_clApsInsp[m_nUnit].func_Insp_Test_SensorRead(true);	//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SERIAL No READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64350;
		break;
	case 64350:
		nRetStep = 64500;
		break;

	case 64500:
		g_clApsInsp[m_nUnit].func_Insp_CurrentMeasure(true);		//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CURRENT READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 64600;
		break;

		
	case 64600:
		//이물 이미지 Get
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			g_clLaonGrabberWrapper[m_nUnit].CopyInspImage(OC_RAW_IMAGE, g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Oc Raw Image Save Ok [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);


			double dFactorX = 0.0;
			double dFactorY = 0.0;

			dFactorX = ((double)g_clModelData[m_nUnit].DefectSizeX / (double)g_clModelData[m_nUnit].m_nWidth);
			dFactorY = ((double)g_clModelData[m_nUnit].DefectSizeY / (double)g_clModelData[m_nUnit].m_nHeight);

			MimResize(g_clVision.m_MilCcdProcImage[m_nUnit], g_clVision.MilDefectImage[m_nUnit], dFactorX, dFactorY, M_DEFAULT);

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 64700;
			break;
		}
		break;
	case 64700:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			nRetStep = 64800;
		}
		
		break;
	case 64800:
		//Raw image
		g_clApsInsp[m_nUnit].LCBInsp(g_clLaonGrabberWrapper[m_nUnit].vRawBuffer_3000k, 0, true);// mRCount);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN LCB 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//
		//
		g_clApsInsp[m_nUnit].FDFInsp(g_clLaonGrabberWrapper[m_nUnit].vRawBuffer_3000k, true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN FDF 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//
		//
		g_clApsInsp[m_nUnit].Blemish_YmeanInsp(g_clLaonGrabberWrapper[m_nUnit].vRawBuffer_3000k, true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN YMEAN 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//--------------- Relative Illumination
		bInspRtn = g_clApsInsp[m_nUnit].func_Insp_Shm_Illumination(g_clLaonGrabberWrapper[m_nUnit].vRawBuffer_3000k, true);

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Relative Illumination 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);



		nRetStep = 64900;
		break;
	case 64900:
		g_clApsInsp[m_nUnit].func_Insp_LightTest(1);		//oqa oc bright
		nRetStep = 65000;
		break;

	case 65000:
//oqa는 yuv필요 없음?
		g_clLaonGrabberWrapper[m_nUnit].CloseDevice();

		g_clLaonGrabberWrapper[m_nUnit].UiconfigLoad(INI_YUV_IMAGE);	//AUTO oc
		g_clLaonGrabberWrapper[m_nUnit].SelectSensor();
		g_clLaonGrabberWrapper[m_nUnit].AllocImageBuff();
		g_clLaonGrabberWrapper[m_nUnit].rawReSet(1);		//yuv

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 65200;
		break;
	case 65200:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 100)
		{
			if (g_pCarAABonderDlg->m_clCustomThread[m_nUnit].GetThreadRunning() == false)
			{
				g_pCarAABonderDlg->m_clCustomThread[m_nUnit].CustomThreadrun(CCD_THREAD);
			}
			else
			{
				g_pCarAABonderDlg->m_clCustomThread[m_nUnit].EndThread();
			}
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 65400;
		}

		break;
	case 65400:
		if (g_clLaonGrabberWrapper[m_nUnit].GetCurrentState() == CCD_GRAB_LIVE)
		{
			Sleep(500);
			nRetStep = 65600;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 10000)	//10초
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1018);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD OPEN 실패. 제품 안착 상태 및 제품 불량을 확인하세요. [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -65200;
			break;
		}

		break;
	case 65600:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 500)
		{
			g_clLaonGrabberWrapper[m_nUnit].CopyInspImage(OC_YUV_IMAGE, g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Oc Yuv Image Save Ok [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);


			double dFactorX = 0.0;
			double dFactorY = 0.0;

			dFactorX = ((double)g_clModelData[m_nUnit].DefectSizeX / (double)g_clModelData[m_nUnit].m_nWidth);
			dFactorY = ((double)g_clModelData[m_nUnit].DefectSizeY / (double)g_clModelData[m_nUnit].m_nHeight);

			MimResize(g_clVision.m_MilCcdProcImage[m_nUnit], g_clVision.MilDefectImage[m_nUnit], dFactorX, dFactorY, M_DEFAULT);

			nRetStep = 65800;
			break;
		}

		break;

	case 65800:
		g_clApsInsp[m_nUnit].func_Insp_ErrorFlag_Read();//AUTO OQA yuv
		nRetStep = 66000;
		break;
	case 66000:
		nRetStep = 66200;
		break;
	case 66200:
		//OC 실린더 상승
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -66200;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 66400;
		break;

	case 66400:
		//OC 실린더 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 66600;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -66200;
			break;
		}
		break;

	case 66600:
		nRetStep = 66800;
		break;

	case 66800:
		//소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1011);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -66800;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 66900;
		break;

	case 66900:
		//소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 67000;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1011);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -66800;
			break;
		}
		break;
	case 67000:
		
		nRetStep = 69900;
		break;


	case 69900:
		BackDuLightControl[m_nUnit].ctrlLedVolume(0, 0);

		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);

		nRetStep = 110000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] OC INSP 이상 STEP 발생 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = -1;
		break;
	}
	//

	return nRetStep;
}


//-----------------------------------------------------------------------------
//
//	OQA PCB 로딩(30000 ~ 40000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::OqaAuto_M_PCBLoading(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	double dPos[3] = { 0.0, 0.0, 0.0 };
	CString sMsg = _T("");
	int nRetStep;
	double dDelay = 0.0;
	bool bRtn = false;
	nRetStep = nStep;

	switch (nStep)
	{
	case 30000:
		if (g_pCarAABonderDlg->m_clUbiGemDlg.bConnected() == false)
		{
			sMsg.Format(_T("MES DISCONNECTED") );
			AddLog(sMsg, 1, m_nUnit, true);
			nRetStep = -30000;
			break;
		}

		if (g_clMesCommunication[m_nUnit].m_dEqupControlState[1] == eOnlineRemote &&
			g_clMesCommunication[m_nUnit].m_dProcessState[0] != g_clMesCommunication[m_nUnit].m_dProcessState[1])
		{

			g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
			g_clMesCommunication[m_nUnit].m_dProcessState[1] = eIDLE;
			g_clMesCommunication[m_nUnit].m_uAlarmList.clear();

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Idle)Process State Change Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[0].bIdleTimeExceed = false;		//init
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401);	//SEND S6F11

			
			
		

			CTime cTime = CTime::GetCurrentTime();//ERRRRRRRRRRRR
			CString strData = _T("");
			strData.Format(_T("%02d%02d%02d%02d%02d%02d"),
				cTime.GetYear(),
				cTime.GetMonth(),
				cTime.GetDay(),
				cTime.GetHour(),
				cTime.GetMinute(),
				cTime.GetSecond());
			_stprintf_s(g_clTaskWork[m_nUnit].m_szIdleStartTime, SIZE_OF_100BYTE, _T("%s"), strData);		//Auto_M_PCBLoading
			g_pCarAABonderDlg->KillTimer(WM_IDLE_REASON_TIMER);

			if (g_clMesCommunication[m_nUnit].IdleSetTimeInterval < 1)
			{
				g_clMesCommunication[m_nUnit].IdleSetTimeInterval = 5;	//min  1min = 60000
			}
			g_pCarAABonderDlg->SetTimer(WM_IDLE_REASON_TIMER, g_clMesCommunication[m_nUnit].IdleSetTimeInterval * 60000, NULL);		//30000 Step
			strData.Empty();
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Process Control State:%d [STEP : %d]"), g_clMesCommunication[m_nUnit].m_dEqupControlState[1], nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		nRetStep = 30010;
		break;
	case 30010:
		mOcLightChannel = 1;	//channel init

		g_clTaskWork[m_nUnit].m_ContactCount = 0;		//step 30000
		g_clTaskWork[m_nUnit].m_nCcdCount = 0;			//step 30000

		g_clTaskWork[m_nUnit].m_nEmissionRun = false;
		g_clTaskWork[m_nUnit].m_bPcbInterLock = true;

		if (g_clModelData[m_nUnit].m_nPinStopUse == 1)
		{
			if (g_clTaskWork[m_nUnit].m_nUsePinCount > g_clSysData.m_nMaxPinCount)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1009);
				sMsg.Format(_T("Socket PIN %d(Spec: %d) \nReset after replacement"), g_clTaskWork[m_nUnit].m_nUsePinCount, g_clSysData.m_nMaxPinCount);//g_clModelData[m_nUnit].m_nSensorCnt);
				AddLog(sMsg, 1, m_nUnit, true);
				nRetStep = -30000;
				break;
			}
		}
		nRetStep = 30020;
		mLightRetry = 0;
		break;

	case 30020:
		//이물 광원 상승
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30020;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30030;
		break;

	case 30030:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 30050;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1010);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30020;
			break;
		}
		
		break;
	case 30050:
		//pcb 소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1011);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30050;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30070;
		break;

	case 30070:
		//pcb 소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 30075;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1011);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30050;
			break;
		}
		
		break;
	case 30075:

		nRetStep = 30110;
		break;

	case 30110:
		nRetStep = 30115;

		break;
	case 30115:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == false)
		{
			g_clDioControl.EziVacuumOn(m_nUnit, true);
			Sleep(10);
			g_clDioControl.EziVacuumBtnLedOn(m_nUnit, true);
		}

		g_clDioControl.EziStartBtnLedOn(m_nUnit, true);
		nRetStep = 30120;
		break;

	case 30120:
		
		if (g_clDioControl.EziStartBtnOnCheck(m_nUnit) == true)		//스타크 버튼 감지되면 Start
		{
			nRetStep = 30125;

			g_clDioControl.EziStartBtnLedOn(m_nUnit, false);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] START PB PRESS [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		break;
	case 30125:
		if (g_pCarAABonderDlg->LgitLicenseCheck() == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] LGIT License 인식 실패 [STEP : %d]"), nStep);

			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30125;
			break;
		}
		nRetStep = 30130;
		break;
	case 30130:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1012);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 제품 감지 실패 [STEP : %d]"), nStep);

			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30130;
			break;
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 제품 감지 확인. [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 30150;
		}

		break;
	case 30150:
		if (g_clDioControl.EziVacuumOnCheck(m_nUnit, true) == false)		//소켓 흡착 상태 확인
		{
			g_clDioControl.EziVacuumOn(m_nUnit, true);
			g_clDioControl.EziVacuumBtnLedOn(m_nUnit, true);
			if (g_ShowMsgModal(_T("확인"), _T("[AUTO] 흡착 상태 확인후 진행해주세요"), RGB_COLOR_RED) == false)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1013);
				nRetStep = -30125;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Vacuum 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				break;
			}
			else
			{
				nRetStep = 30200;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Vacuum 확인. [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
			}
			break;
		}
		else
		{
			g_clTaskWork[m_nUnit].m_bPcbSocketIF[PCB_BCR] = true;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Vacuum 확인 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			//흡착 상태 확인
			nRetStep = 30200;
		}
		break;

	case 30200:
		MbufClear(g_clVision.MilDefectOverlayImage[m_nUnit], (MIL_DOUBLE)g_clVision.m_nMilDefectTransparent[m_nUnit]);
		MbufClear(g_clVision.MilDefectImage[m_nUnit], 0);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 30400;
		break;

	case 30400:
		nRetStep = 30500;
		break;

	case 30500:
		nRunOnlineControlState = false;
		nRunTimeOutSec = g_clMesCommunication[m_nUnit].ConversationTimeoutCount * 1000;

		if (nRunTimeOutSec < 1)
		{
			nRunTimeOutSec = 60 * 1000;
		}
		if (g_clMesCommunication[m_nUnit].m_dEqupControlState[1] == eOnlineRemote)
		{
			nRunOnlineControlState = true;
		}

		if (nRunOnlineControlState == false)
		{
			//Offline 이면 전부 Skip
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Equipment Offline State Object Id Pass [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 32750;	//Jump Setp
			break;
		}

		nRetStep = 30600;
		break;

	case 30600:
		//OBJECT ID REPORT
		g_clTaskWork[m_nUnit].bRecv_Lgit_Pp_select = -1;
		g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
		g_pCarAABonderDlg->KillTimer(WM_IDLE_REASON_TIMER);		//OBJECT ID REPORT start
																//
		g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(OBJECT_ID_REPORT_10701);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Object Id Report Send [STEP : %d]"), nStep);	//-----------------------OBJECT ID REPORT start
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30700;

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		break;
	case 30700:	//ok
		if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 0)
		{
			//g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			//Recv LGIT_LOT_START
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Id Start Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 31400;	//Jump Step
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 1)
		{
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			//Recv LGIT_LOT_ID_FAIL
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Id Fail Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			sMsg.Empty();

			//std::vector<RcmdList_T2> resultsT2;
			//resultsT2 = g_clReportData.T2_FindChildList(g_clReportData.rLot_Id_Fail.parameters);

			//if (resultsT2.size() > 0)
			//{
			//	for (int i = 0; i < resultsT2[0].rVec.size(); i++)
			//	{
			//		if (resultsT2[0].rVec[i].cpName == "MODULEID")
			//		{
			//			//g_pCarAABonderDlg->m_clMessageLot.setLotID(_T("LOT ID"), resultsT2[0].rVec[i].cepVal);		//여긴 lot id? , moduleid?
			//			sMsg.Format(_T("LOT ID:%s"), resultsT2[0].rVec[i].cepVal);// g_clModelData[m_nUnit].m_nSensorCnt);

			//			break;
			//		}
			//	}
			//}
			if (g_clReportData.vLotIdFail.size() > 0)
			{
				for (int i = 0; i < g_clReportData.vLotIdFail[0].children.size(); i++)
				{
					if (g_clReportData.vLotIdFail[0].children[i].name == "MODULEID")
					{
						sMsg.Format(_T("LOT ID:%s"), g_clReportData.vLotIdFail[0].children[i].value);
						break;
					}
				}
			}
			//
			//_stprintf_s(szLog, SIZE_OF_1K, _T("[RCMD L%d]Name:%s, Val:%s"), i + 1, g_clReportData.vLotIdFail[0].children[i].name, g_clReportData.vLotIdFail[0].children[i].value);
			//
			_stprintf_s(szLog, SIZE_OF_1K, _T("[LGIT_LOT_ID_FAIL]\n%s \nCode :%s\nText:%s\n재시도 하시겠습니까?"), sMsg,
				g_clMesCommunication[m_nUnit].m_sErcmdCode, g_clMesCommunication[m_nUnit].m_sErcmdText);

			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot ID Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) LOT id Cancel. by Host [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}

		}
		else if (g_clTaskWork[m_nUnit].bRecv_Lgit_Pp_select == 0)		//LOT START 가 올수고있다  아
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lgit PP Select Recv OK [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].bRecv_Lgit_Pp_select = -1;
			nRetStep = 30750;
		}
		else if (g_clTaskWork[m_nUnit].bRecv_Lgit_Pp_select == 1)
		{
			//Recipe ID Check
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(2000);
			g_clDioControl.SetBuzzer(true);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] 사용중인 RECIPE (%s) 와 다릅니다.\n재시도 하시겠습니까?"), g_clMesCommunication[m_nUnit].m_sRecipeId, g_clMesCommunication[m_nUnit].m_sMesPPID);
			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmClearSendFn();
				g_clDioControl.SetBuzzer(false);
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Object Id Report Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] RECIPE ID 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] LGIT PP SELECT Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10701"));
			nRetStep = -30600;
			break;
		}

		break;
	case 30750:

		g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
		g_clMesCommunication[m_nUnit].m_dProcessState[1] = eSETUP;
		g_clMesCommunication[m_nUnit].m_uAlarmList.clear();

		g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change = -1;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Setup)Process State Change Report [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);


		g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401);	//SEND S6F11
																								//Event = Process State Change Report ("SETUP");
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 30800;

		break;
	case 30800:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change == 0)	//SETUP
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Setup)Process State Change Send Acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change = -1;
			//Recipe Setup Completion
			//SV:Recipe ID Set

			g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
			g_clMesCommunication[m_nUnit].m_dProcessState[1] = eREADY;

			g_clMesCommunication[m_nUnit].m_uAlarmList.clear();

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Ready)Process State Change Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401); //SEND S6F11
																									//S6F11 Send

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 30900;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10401"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Setup)Process State Change Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}


		break;
	case 30900:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change == 0)	//READY
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Ready)Process State Change Send Acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change = -1;
			g_clTaskWork[m_nUnit].bRecv_S6F12_PP_Selected = -1;
			g_clTaskWork[m_nUnit].bRecv_S7F25_Formatted_Process_Program = -1;		//<--미리 초기화
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;		//미리 초기화

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP-Selected Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PP_SELECTED_REPORT_10702);//SEND S6F11
																						 //Event - PP-Selected Report


			//두 종류로 올 수 있다.
			//1..여기서 LGIT_LOT_START 오거나 Jmp
			//2..LGIT_LOT_ID_FAIL Lot ID Cancel by Host 올 수 있따.
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 31000;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10401"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Ready)Process State Change Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}

		break;
	case 31000:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_PP_Selected == 0)		//S6F12 대기
		{
			g_clTaskWork[m_nUnit].bRecv_S6F12_PP_Selected = -1;
			//Recv PP-Select Report
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP-Selected Send Acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 31050;

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}

		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10702"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP-Select Report Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}


		break;
	case 31050:
		//
		////Recipe Parameter Validation EQP 안하면 바로 Lot Start 온다.
		//
		nRetStep = 31100;
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();

		break;
	case 31100:
		if (g_clTaskWork[m_nUnit].bRecv_S7F25_Formatted_Process_Program == 0)		//Ubisam 에서 보내고 0으로 변경
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Formatted Process Program Request [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			//수신 대기 S7F25 - Formatted Process Program Request
			g_clTaskWork[m_nUnit].bRecv_S2F49_PP_UpLoad_Confirm = -1;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 31200;
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 0)
		{
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			//Recv LGIT_LOT_START
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Id Start Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 31400;	//Jump Step
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 1)
		{
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			//Recv LGIT_LOT_ID_FAIL
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Id Fail Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			sMsg.Empty();

			//std::vector<RcmdList_T2> resultsT2;
			//resultsT2 = g_clReportData.T2_FindChildList(g_clReportData.rLot_Id_Fail.parameters);
			//if (resultsT2.size() > 0)
			//{
			//	for (int i = 0; i < resultsT2[0].rVec.size(); i++)
			//	{
			//		if (resultsT2[0].rVec[i].cpName == "MODULEID")
			//		{
			//			sMsg.Format(_T("LOT ID:%s"), resultsT2[0].rVec[i].cepVal);// g_clModelData[m_nUnit].m_nSensorCnt);
			//			break;
			//		}
			//	}
			//}
			if (g_clReportData.vLotIdFail.size() > 0)
			{
				for (int i = 0; i < g_clReportData.vLotIdFail[0].children.size(); i++)
				{
					if (g_clReportData.vLotIdFail[0].children[i].name == "MODULEID")
					{
						sMsg.Format(_T("LOT ID:%s"), g_clReportData.vLotIdFail[0].children[i].value);
						break;
					}
				}
			}


			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(2001);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[LGIT_LOT_ID_FAIL]\n%s \nCode :%s\nText:%s\n재시도 하시겠습니까?"), sMsg,
				g_clMesCommunication[m_nUnit].m_sErcmdCode, g_clMesCommunication[m_nUnit].m_sErcmdText);

			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmClearSendFn();
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot ID Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) LOT id Cancel. by Host [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}

		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S07F25"), _T("10702"));//<<<여긴 맞나
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Formatted Process Program Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		break;
	case 31200:
		if (g_clTaskWork[m_nUnit].bRecv_S2F49_PP_UpLoad_Confirm == 0)	//LGIT_PP_UPLOAD_CONFIRM
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Confirm Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			//LGIT_PP_UPLOAD_CONFIRM 오고 S2F50보낸뒤 , 정상 진행
			g_clTaskWork[m_nUnit].bRecv_S2F49_PP_UpLoad_Confirm = -1;
			g_clTaskWork[m_nUnit].bRecv_S6F12_PP_UpLoad_Completed = -1;
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;		//미리 초기화


			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Completed Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PP_UPLOAD_COMPLETED_REPORT_10703);//SEND S6F11
																								 //Event - PP Upload Completed Report
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 31300;
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_PP_UpLoad_Confirm == 1)	//LGIT_PP_UPLOAD_FAIL 확인필요 250112
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(2002);
			//LGIT_PP_UPLOAD_FAIL 오고 S2F50보낸뒤,  NG (Recipe Body Cancel by Host)
			g_clTaskWork[m_nUnit].bRecv_S2F49_PP_UpLoad_Confirm = -1;

			_stprintf_s(szLog, SIZE_OF_1K, _T("RECIPE ID:%s \nLGIT_PP_UPLOAD_FAIL\nCode :%s\nText:%s\n재시도 하시겠습니까?"),
				g_clMesCommunication[m_nUnit].m_sRecipeId, g_clMesCommunication[m_nUnit].m_sErcmdCode, g_clMesCommunication[m_nUnit].m_sErcmdText);
			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmClearSendFn();
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) PP Upload Fail Pause [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S02F49"), _T("10703"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Confirm Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		break;
	case 31300:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_PP_UpLoad_Completed == 0)
		{
			g_clTaskWork[m_nUnit].bRecv_S6F12_PP_UpLoad_Completed = -1;
			//Recv S6F12
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Completed Send acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 31400;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10703"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PP Upload Completed Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		break;
	case 31400:

		if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 0)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lgit Lot Start Send acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change = -1;
			//Recv LGIT_LOT_START


			//Lot Processing Start ????

			g_clMesCommunication[m_nUnit].m_dProcessState[0] = g_clMesCommunication[m_nUnit].m_dProcessState[1];
			g_clMesCommunication[m_nUnit].m_dProcessState[1] = eEXECUTING;

			g_clMesCommunication[m_nUnit].m_uAlarmList.clear();
			g_clMesCommunication[m_nUnit].m_dLotProcessingState = eWait;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Executing)Process State Change Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(PROCESS_STATE_CHANGED_REPORT_10401); //SEND S6F11

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 31500;
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 1)	//확인필요
		{
			//Recv LGIT_LOT_ID_FAIL
			//NG - Lot ID Cancel by Host

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Id Fail Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			sMsg.Empty();

			/*std::vector<RcmdList_T2> resultsT2;
			resultsT2 = g_clReportData.T2_FindChildList(g_clReportData.rLot_Id_Fail.parameters);
			if (resultsT2.size() > 0)
			{
			for (int i = 0; i < resultsT2[0].rVec.size(); i++)
			{
			if (resultsT2[0].rVec[i].cpName == "MODULEID")
			{
			sMsg.Format(_T("LOT ID:%s"), resultsT2[0].rVec[i].cepVal);
			break;
			}
			}
			}*/

			if (g_clReportData.vLotIdFail.size() > 0)
			{
				for (int i = 0; i < g_clReportData.vLotIdFail[0].children.size(); i++)
				{
					if (g_clReportData.vLotIdFail[0].children[i].name == "MODULEID")
					{
						sMsg.Format(_T("LOT ID:%s"), g_clReportData.vLotIdFail[0].children[i].value);
						break;
					}
				}
			}

			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(2003);


			_stprintf_s(szLog, SIZE_OF_1K, _T("[LGIT_LOT_ID_FAIL]\n%s \nCode :%s\nText:%s\n재시도 하시겠습니까?"), sMsg,
				g_clMesCommunication[m_nUnit].m_sErcmdCode, g_clMesCommunication[m_nUnit].m_sErcmdText);

			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmClearSendFn();
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot ID Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) LOT id Cancel. by Host [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S02F49"), _T("10704"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Start Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		//두 종류로 올 수 있다.
		//1..여기서 LGIT_LOT_START 오거나 Jmp
		//2..LGIT_LOT_ID_FAIL Lot ID Cancel by Host 올 수 있따.
		break;
	case 31500:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change == 0)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Executing) Process State Change Acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[m_nUnit].bRecv_S6F12_Process_State_Change = -1;
			g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Started = -1;
			g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start = -1;
			nLotProcessingComplete_ACK = -1;


			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Started Report [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(LOT_PROCESSING_STARTED_REPORT_10704);//SEND S6F11 Lot Processing Started Report
			nRetStep = 31600;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10401"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] (Executing) Process State Change Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		break;
	case 31600:


		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Started == 0)	//ack  0일대만 양품 배출해야된다.
		{
			nLotProcessingComplete_ACK = g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Started;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Started Acknowledge,Ack:%d [STEP : %d]"), nLotProcessingComplete_ACK, nStep);
			AddLog(szLog, 0, m_nUnit);
			//ack 0 확인하고 진행하면되는지?
			//아니면 LGIT_LOT_ID_FAIL 또 올 수 있는지 확인 필요
			nRetStep = 31700;
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Started == 1)	//nack 1일 아닐수도있다.
		{
			//ack를 체크해야되나?

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Started Fail Recv [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] LOT id Fail.\n재시도 하시겠습니까?"), g_clTaskWork[m_nUnit].m_szChipID);
			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot ID Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) LOT id Cancel. by Host [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}

		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10704"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Start Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30600;	//OBJECT ID REPORT Step
			break;
		}
		break;
	case 31700:
		nRetStep = 31800;
		///
		// Lot Processing Start 받고나서 Lot ID Start , Fail 체크해야되나?
		//nLotProcessingComplete_ACK  값 확인해서 배출 할때 판단?
		//
		//
#if 0
		if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 0)
		{
			//LGIT_LOT_START
			//NG Lot ID Cancel by Host
		}
		else if (g_clTaskWork[m_nUnit].bRecv_S2F49_LG_Lot_Start == 1)
		{
			//LGIT_LOT_ID_FAIL
			//NG Lot ID Cancel by Host
			sMsg.Empty();
			std::vector<RcmdList_T2> resultsT2;
			resultsT2 = g_clReportData.T2_FindChildList(g_clReportData.rLot_Id_Fail.parameters);
			if (resultsT2.size() > 0)
			{
				for (int i = 0; i < resultsT2[0].rVec.size(); i++)
				{
					if (resultsT2[0].rVec[i].cpName == "MODULEID")
					{
						//g_pCarAABonderDlg->m_clMessageLot.setLotID(_T("LOT ID"), resultsT2[0].rVec[i].cepVal);		//여긴 lot id? , moduleid?
						sMsg.Format(_T("LOT ID:%s"), resultsT2[0].rVec[i].cepVal);// g_clModelData[m_nUnit].m_nSensorCnt);

						break;
					}
				}
			}
			_stprintf_s(szLog, SIZE_OF_1K, _T("[LGIT_LOT_ID_FAIL]\n%s \nCode :%s\nText:%s\n재시도 하시겠습니까?"), sMsg,
				g_clMesCommunication[m_nUnit].m_sErcmdCode, g_clMesCommunication[m_nUnit].m_sErcmdText);
			if (g_ShowMsgModal(_T("[INFO]"), szLog, RGB_COLOR_BLUE, _T("RETRY"), _T("PAUSE")) == true)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot ID Fail Retry [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = 30600;
			}
			else
			{
				nRetStep = -30600;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO](%s) LOT id Cancel. by Host [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
				AddLog(szLog, 1, m_nUnit, true);
				break;
			}

		}
#endif
		break;
	case 31800:
		nRetStep = 32000;
		break;
	case 31900:

		break;

		//30500 ~ 31700
	case 32000:
		nRetStep = 32500;
		break;
	case 32500:
		nRetStep = 32600;

		break;
	case 32600:
		// MES 확인
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 32750;
		break;

	case 32750:
		// 바코드값 수신
		if (g_clApsInsp[m_nUnit].func_ModelLotCheck(g_clTaskWork[m_nUnit].m_szChipID) == 0)
		{
#if (____MACHINE_NAME == MODEL_FRONT_100)
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인 완료:%s[002]"), g_clTaskWork[m_nUnit].m_szChipID);
#else
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인 완료:%s[001]"), g_clTaskWork[m_nUnit].m_szChipID);
#endif
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
#if (____MACHINE_NAME == MODEL_FRONT_100)
			_stprintf_s(szLog, SIZE_OF_1K, _T("[BCR] 100H 프로그램입니다. 모델 확인 바랍니다. (002)\n계속 진행하시겠습니까?"));
#else
			_stprintf_s(szLog, SIZE_OF_1K, _T("[BCR] 150H 프로그램입니다. 모델 확인 바랍니다. (001)\n계속 진행하시겠습니까?"));
#endif
			if (g_ShowMsgModal(_T("확인"), szLog, RGB_COLOR_BLUE) == false)
			{
				g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1014);
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] BCR 모델 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);

				nRetStep = -32750;
				break;
			}


			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR CHECK PASS"));
			AddLog(szLog, 0, m_nUnit);
		}

		g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		g_clModelData[m_nUnit].LotDataSave();
		nRetStep = 32755;
		// 바코드값 수신 g_clTaskWork[m_nUnit].m_bPcbSocketIF[PCB_BCR] == true && 
		////if ((_tcslen(g_clTaskWork[m_nUnit].m_szChipID) > 0))//11))/// || g_clModelData[m_nUnit].m_nMesPass == 1)
		////{
		////	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인:%s [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
		////	AddLog(szLog, 0, m_nUnit);
		////	g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		////	g_clModelData[m_nUnit].LotDataSave();
		////	nRetStep = 32755;
		////	break;
		////}
		////else if (GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount > 6000)
		////{
		////	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB BCR 확인 실패 [STEP : %d]"), nStep);
		////	AddLog(szLog, 1, m_nUnit, true);
		////	nRetStep = -32750;
		////}
		
		break;
	case 32755:
		nRetStep = 32770;
		break;
	case 32770:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 32780;
		break;
	case 32780:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			nRetStep = 32900;
		}
		break;
	case 32900://jump step
#ifdef ON_LINE_MIL
		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);
#endif
		for (int i = 0; i < 20; i++)
		{
			g_clTaskWork[m_nUnit].m_bOutputCheck[i] = true; // LOADING 초기화
		}
		//MES 결과 초기화
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 1;	//시작할대 초기화부분 OQA

																//이미지버퍼 초기화
		g_clLaonGrabberWrapper[m_nUnit].CopyInspImage(999, g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer);
		g_clTaskWork[m_nUnit].m_nEmission = 0;
		g_clTaskWork[m_nUnit].SaveData();
		g_pCarAABonderDlg->ShowOkNgState(m_nUnit, g_clTaskWork[m_nUnit].m_nEmission);
		nRetStep = 33000;

		break;
	case 33000:
		g_clTaskWork[m_nUnit].m_dwCycleTactStartTime = GetTickCount();
		nRetStep = 33500;

		break;
	case 33500:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 35500;
		break;
	case 35500:

		dDelay = g_clModelData[m_nUnit].m_dCcdDelay;
		if (dDelay > 100.0)
		{
			dDelay = dDelay / 1000.0;
		}
		if (dDelay < 0.3)
		{
			dDelay = 0.3;
		}
		if (GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount > dDelay * 1000)//1000)
		{
			nRetStep = 36000;
		}

		break;

	case 36000:
		g_clLaonGrabberWrapper[m_nUnit].UiconfigLoad(INI_RAW_IMAGE);	//AUTO start
		g_clLaonGrabberWrapper[m_nUnit].SelectSensor();
		g_clLaonGrabberWrapper[m_nUnit].AllocImageBuff();
		g_clLaonGrabberWrapper[m_nUnit].rawReSet(0);		//raw

		nRetStep = 36500;


		break;
	case 36500:

		nRetStep = 36510;
		break;
	case 36510:
		g_clMesCommunication[m_nUnit].m_dwCycleTactStartTime = GetTickCount();

		g_pCarAABonderDlg->m_clCustomThread[m_nUnit].CustomThreadrun(CCD_THREAD);

		g_pCarAABonderDlg->m_clCcdGrabThread[m_nUnit].UpdateGrabStartTime();

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD OPEN Start [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);


		g_clMandoInspLog[m_nUnit].InitData();
		g_pCarAABonderDlg->_DrawBarGraph(m_nUnit);

		// 포거핀 사용량 증가
		g_clTaskWork[m_nUnit].m_nUsePinCount++;
		g_pCarAABonderDlg->ShowPinCount(m_nUnit);

		g_clTaskWork[m_nUnit].PinSaveData();
		////g_clTaskWork[m_nUnit].SaveData();
		nRetStep = 37500;
		break;

	case 37500:

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PRODUCT PCB LOADING 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_nRetryCount = 0;
		nRetStep = 38000;
		break;

	case 38000:
		nRetStep = 38500;
		break;
	case 38500:

		nRetStep = 39000;
		break;

	case 39000:
		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);

		/*if (g_clModelData[m_nUnit].m_nMasterModeUse == 1)
		{
			if (g_ShowMsgModal(_T("확인"), _T("[AUTO] MASTER MODE 입니다. 진행하시겠습니까?"), RGB_COLOR_RED) == false)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MASTER MODE STOP [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				nRetStep = -39000;
				break;
			}
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MASTER MODE START [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}*/


		nRetStep = 60000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PRODUCT PCB LOADING 이상 STEP 발생 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit, true);
		nRetStep = -1;
		break;
	}

	return nRetStep;
}




//-----------------------------------------------------------------------------
//
//	Oqa Final SFR(110000 ~ 120000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::OqaAutoEOLFinalSFR(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szPath[SIZE_OF_1K];
	CFileFind clFinder;
	CString sMsg = _T("");
	double dOcResult[2] = { 0.0, 0.0 };
	double dPos[3] = { 0.0, 0.0, 0.0 };
	int nRetStep;
	bool bRtn = false;
	nRetStep = nStep;

	switch (nStep)
	{
	case 110000:
		g_clTaskWork[m_nUnit].m_nRetryCount = 0;
		g_clTaskWork[m_nUnit].m_dwInspTactTime = GetTickCount();


		nRetStep = 111000;
		break;
	case 111000:

		nRetStep = 112000;
		break;
	case 112000:
		nRetStep = 113500;

		break;
	case 113500:
		
		nRetStep = 114500;
		break;

	case 114500:

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD CLOSE [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clLaonGrabberWrapper[m_nUnit].CloseDevice();


		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 115000;
		break;
	case 115000:
		g_clDioControl.EziVacuumOn(m_nUnit, false);
		g_clDioControl.EziVacuumBtnLedOn(m_nUnit, false);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB VACUUM 탈착 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 116000;

		break;
	case 116000:

		nRetStep = 116200;
		break;
	case 116200:

		bRtn = g_clMesCommunication[m_nUnit].g_Final_OqaLog(m_nUnit);

		if (bRtn == false)
		{
			g_pCarAABonderDlg->m_clUbiGemDlg.AlarmSendFn(1019);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]FinalLog File is Save Fail!!\n 로그파일이 열려있으면 닫아주세요[STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -116200;
			break;
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] LOG SAVE COMPLETE! [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}

		nRetStep = 116300;// 116500;

		break;
	case 116300:
		if (nRunOnlineControlState == false)
		{
			//Offline 이면 전부 Skip
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Equipment Offline State Apd Pass [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 116500;
			break;
		}
		g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Apd = -1;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot APD Report Send [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(LOT_APD_REPORT_10711);
		nRetStep = 116350;

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		break;
	case 116350:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Apd == 0)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot APD Send Acknowledge [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Completed = -1;
			g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Completed_Ack = -1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Completed Report Send [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(LOT_PROCESSING_COMPLETED_REPORT_10710);//SEND S6F11
			nRetStep = 116400;

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			//g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(CT_TIMEOUT_REPORT_11002); //SEND S6F11
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10711"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot APD Ct TimeOut [STEP : %d]"), nStep); 
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -116300;
			break;
		}

		break;
	case 116400:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 116450;
		break;
	case 116450:
		if (g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Completed == 0)
		{
			if (g_clTaskWork[m_nUnit].bRecv_S6F12_Lot_Processing_Completed_Ack != 0)
			{
				//nack
				//
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Completed Fail [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
				g_ShowMsgPopup(_T("[INFO]"), szLog, RGB_COLOR_RED);
			}
			else
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Completed OK [STEP : %d]"), nStep);
				AddLog(szLog, 0, m_nUnit);
			}


			//0 == OK, ACK
			nRetStep = 116500;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > nRunTimeOutSec)
		{
			//g_pCarAABonderDlg->m_clUbiGemDlg.EventReportSendFn(CT_TIMEOUT_REPORT_11002); //SEND S6F11
			g_pCarAABonderDlg->m_clUbiGemDlg.cTTimeOutSendFn(_T("S06F12"), _T("10710"));
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Lot Processing Completed Ct TimeOut [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -116300;
			break;
		}

		break;

	case 116500:
		if (g_clMesCommunication[m_nUnit].m_nMesFinalResult == 1)	//양불 판정 OQA
		{
			g_clTaskWork[m_nUnit].m_nEmission = 1;
			g_clTaskWork[m_nUnit].SaveData();
			g_pCarAABonderDlg->ShowOkNgState(m_nUnit, g_clTaskWork[m_nUnit].m_nEmission);
			g_clVision.DrawMOverlayText(m_nUnit, 50, 100, _T("PASS"), M_COLOR_GREEN, _T("Arial"), 150, 80, FALSE);
			g_clTaskWork[m_nUnit].m_nTotalOkCount++;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 최종 판단 : OK [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			g_clTaskWork[m_nUnit].m_nEmission = 2;
			g_clTaskWork[m_nUnit].SaveData();
			g_pCarAABonderDlg->ShowOkNgState(m_nUnit, g_clTaskWork[m_nUnit].m_nEmission);
			g_clVision.DrawMOverlayText(m_nUnit, g_clModelData[m_nUnit].m_nWidth * 0.55, 100, _T("FAIL"), M_COLOR_RED, _T("Arial"), 150, 80, FALSE);
			g_clMandoInspLog[m_nUnit].DrawNGOverlay(m_nUnit); //화면 Overlay에 NG List Draw 

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 최종 판단 : NG [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		g_clVision.DrawOverlayAll(m_nUnit);
		nRetStep = 117000;
		break;

	case 117000:
		g_clTaskWork[m_nUnit].m_IoRetry = 0;
		nRetStep = 117100;
		break;
	case 117100:
		nRetStep = 119500;
		break;



	case 119500:
		g_clTaskWork[m_nUnit].m_nTotalWorkCount++;
		g_clTaskWork[m_nUnit].SaveData();
		g_pCarAABonderDlg->ShowOutputCount(m_nUnit);
		nRetStep = 119800;
		break;
	case 119800:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] end Lot : %s [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clMesCommunication[m_nUnit].m_sMesLotID.Format(_T("EMPTY"));
		_stprintf_s(g_clTaskWork[m_nUnit].m_szChipID, SIZE_OF_100BYTE, _T("EMPTY")); //bcr 초기화
		g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		nRetStep = 30000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] EOL FINAL 검사 이상 STEP 발생[STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = -1;
		break;
	}

	return nRetStep;

}
