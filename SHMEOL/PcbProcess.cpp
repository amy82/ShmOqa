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
	CString sMsg = _T("");
	int nHomeFlag = 0;
	int nRetStep = 0;
	int i = 0;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	nRetStep = nStep;

	if (g_clDioControl.EziLightCurtainCheck(UNIT_AA1) == true)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 원점동작 중 CURTAIN 감지. 일시 정지"));
		AddLog(szLog, 1, UNIT_AA1, true);

		nRetStep *= -1;
		return nRetStep;
	}
	switch (nStep)
	{
	case 10000:
		g_clDioControl.EziStartBtnLedOn(m_nUnit, false);
		nRetStep = 11200;
		break;
	case 11200:
		nRetStep = 11400;
		break;
	case 11400:
		//이물 광원 상승
		
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11400;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 11600;
		break;
	case 11600:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 11800;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11400;
			break;
		}
		
		break;
	case 11800:
		//pcb 소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11800;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 12000;
		break;
	case 12000:
		//pcb 소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 12200;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11800;
			break;
		}
		break;
	case 12200:
		nRetStep = 12400;
		break;
	case 12400:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 제품 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 제품 미확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		
		nRetStep = 12600;
		break;
	case 12600:
		////탈착
		//if (g_clDioControl.EziVacuumOn(m_nUnit, false) == false)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 실패 [STEP : %d]"), nStep);
		//	AddLog(szLog, 1, m_nUnit, true);

		//	nRetStep = -12600;
		//	break;
		//}

		//g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);
		//g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		//_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 [STEP : %d]"), nStep);
		//AddLog(szLog, 0, m_nUnit);
		nRetStep = 12800;
		break;
	case 12800:
		//탈착 확인
		if (g_clDioControl.EziVacuumOnCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 13000;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -12600;
			break;
		}
		
		break;
	case 13000:
		nRetStep = 18000;
		break;
	case 18000:
		//! 전체 원점 동작 완료
		g_clMotorSet.m_bOqaOrgFlag = true;
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].state = 1;
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].Invalidate();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 원점 복귀 완료[STEP : %d]"), nStep);

		AddLog(szLog, 0, m_nUnit);
		g_ShowMsgPopup(_T("INFO"), _T("[AUTO] 원점 복귀 완료"), RGB_COLOR_RED);
		nRetStep = 20000;
		break;

	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 원점 복귀 이상 STEP 발생[STEP : %d]"), nStep);
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
	CString sMsg = _T("");
	int i = 0;
	nRetStep = nStep;

	if (g_clDioControl.EziLightCurtainCheck(UNIT_AA1) == true)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 원점동작 중 CURTAIN 감지. 일시 정지"));
		AddLog(szLog, 1, UNIT_AA1, true);

		nRetStep *= -1;
		return nRetStep;
	}
	switch (nStep)
	{
	case 20000:
		g_clDioControl.EziStartBtnLedOn(m_nUnit, false);
		nRetStep = 21000;
		break;
	case 21000:

		//이물광원
		//LIGHT_BD_OC_CH2
		BackDuLightControl[m_nUnit].ctrlLedVolume(0, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_OC_WHITE1]);
		Sleep(10);

		nRetStep = 21200;
		break;
	case 21200:
		nRetStep = 21400;
		break;
	case 21400:
		//이물 광원 상승

		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21400;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 21600;
		break;
	case 21600:
		//이물 광원 상승 확인
		if (g_clDioControl.EziOcUpCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 21800;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물 광원 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21400;
			break;
		}

		break;
	case 21800:
		//pcb 소켓 후진
		if (g_clDioControl.EziPcbSocketFor(m_nUnit, false) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21800;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 22000;
		break;
	case 22000:
		//pcb 소켓 후진 확인
		if (g_clDioControl.EziPcbSocketForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 22200;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21800;
			break;
		}
		break;
	case 22200:
		nRetStep = 22400;
		break;
	case 22400:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 제품 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 제품 미확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}

		nRetStep = 22600;
		break;
	case 22600:
		////탈착
		//if (g_clDioControl.EziVacuumOn(m_nUnit, false) == false)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 실패 [STEP : %d]"), nStep);
		//	AddLog(szLog, 1, m_nUnit, true);

		//	nRetStep = -22600;
		//	break;
		//}

		//g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);
		//g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		//_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 상승 [STEP : %d]"), nStep);
		//AddLog(szLog, 0, m_nUnit);
		nRetStep = 22800;
		break;
	case 22800:
		//탈착 확인
		if (g_clDioControl.EziVacuumOnCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 23000;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 3000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 탈착 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -22600;
			break;
		}

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

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 운전준비 완료 [STEP : %d]"), nStep);
		g_ShowMsgPopup(_T("INFO"), _T("[AUTO] 운전준비 완료"), RGB_COLOR_RED, 1);

		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 운전준비 STEP 이상 [STEP : %d]"), nStep);
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
		g_clPriInsp[m_nUnit].func_Insp_Version_Read();//OQA raw
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]VERSION READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64100;
		break;
	case 64100:
		g_clPriInsp[m_nUnit].func_Insp_Supply_Voltage_Read(true);		//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SUPPLY VOLTAGE READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64200;
		break;
	case 64200:
		g_clPriInsp[m_nUnit].func_Insp_Sensor_Voltage_Read(true);		//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SENSOR VOLTAGE READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64300;
		break;

	case 64300:
		g_clPriInsp[m_nUnit].func_Insp_Test_SensorRead(true);	//oqa
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]SERIAL No READ 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 64500;
		break;
	case 64500:
		g_clPriInsp[m_nUnit].func_Insp_CurrentMeasure(true);		//oqa
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
		g_clPriInsp[m_nUnit].LCBInsp(g_clLaonGrabberWrapper[m_nUnit].vDefectMidBuffer_3000k, 0, true);// mRCount);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN LCB 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//
		//
		g_clPriInsp[m_nUnit].FDFInsp(g_clLaonGrabberWrapper[m_nUnit].vDefectMidBuffer_3000k, true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN FDF 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//
		//
		g_clPriInsp[m_nUnit].Blemish_YmeanInsp(g_clLaonGrabberWrapper[m_nUnit].vDefectMidBuffer_3000k, true);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] STAIN YMEAN 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		//--------------- Relative Illumination
		bInspRtn = g_clPriInsp[m_nUnit].func_Insp_Shm_Illumination(g_clLaonGrabberWrapper[m_nUnit].vDefectMidBuffer_3000k, true);

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Relative Illumination 검사 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);



		nRetStep = 64900;
		break;
	case 64900:
		g_clPriInsp[m_nUnit].func_Insp_LightTest(1);		//oqa oc bright
		nRetStep = 65000;
		break;
#if 1

	case 65000:
//oqa는 yuv필요 없음?
		g_clLaonGrabberWrapper[m_nUnit].CloseDevice();

		g_clLaonGrabberWrapper[m_nUnit].UiconfigLoad(INI_YUV_IMAGE);	//AUTO oc
		g_clLaonGrabberWrapper[m_nUnit].SelectSensor();
		g_clLaonGrabberWrapper[m_nUnit].AllocImageBuff();
		g_clLaonGrabberWrapper[m_nUnit].rawReSet();

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
#else
	case 65000:
		
		nRetStep = 65800;
		break;
#endif
	case 65800:
		g_clPriInsp[m_nUnit].func_Insp_ErrorFlag_Read();//AUTO OQA yuv
		nRetStep = 66000;
		break;
	case 66000:
		nRetStep = 66200;
		break;
	case 66200:
		//OC 실린더 상승
		if (g_clDioControl.EziOcUp(m_nUnit, true) == false)
		{
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
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -66800;
			break;
		}
		break;
	case 67000:
		//
		nRetStep = 68000;
		break;

	case 68000:
		nRetStep = 69000;
		break;
	case 69000:

		nRetStep = 69800;
		break;

	case 69800:
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
		mOcLightChannel = 1;	//channel init

		g_clTaskWork[m_nUnit].m_ContactCount = 0;		//step 30000
		g_clTaskWork[m_nUnit].m_nCcdCount = 0;			//step 30000

		g_clTaskWork[m_nUnit].m_nEmissionRun = false;
		g_clTaskWork[m_nUnit].m_bPcbInterLock = true;
		if (g_clModelData[m_nUnit].m_nPinStopUse == 1)
		{
			if (g_clTaskWork[m_nUnit].m_nUsePinCount > g_clSysData.m_nMaxPinCount)//g_clModelData[m_nUnit].m_nSensorCnt)
			{
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
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 소켓 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30050;
			break;
		}
		
		break;
	case 30075:

		nRetStep = 30080;
		break;

	case 30080:
		nRetStep = 30090;
		break;
	case 30090:

		nRetStep = 30110;
		break;
	case 30110:
		nRetStep = 30115;

		break;
	case 30115:
		if (g_clDioControl.EziPcbCheck(m_nUnit) == false)
		{
			g_clDioControl.EziVacuumOn(m_nUnit, true);
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
		if (g_clDioControl.EziPcbCheck(m_nUnit) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 제품 감지 실패 [STEP : %d]"), nStep);

			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30120;
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
		nRetStep = 30400;
		break;
	case 30400:
		MbufClear(g_clVision.MilDefectOverlayImage[m_nUnit], (MIL_DOUBLE)g_clVision.m_nMilDefectTransparent[m_nUnit]);
		MbufClear(g_clVision.MilDefectImage[m_nUnit], 0);
		nRetStep = 30500;
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		break;

	case 30500:
		nRetStep = 30700;
		break;
	case 30700:

		nRetStep = 31000;
		break;
	case 31000:
		nRetStep = 32000;
		break;
	case 32000:
		//딜레이
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
		if (g_clPriInsp[m_nUnit].func_ModelLotCheck(g_clTaskWork[m_nUnit].m_szChipID) == 0)
		{
#if (____MACHINE_NAME == MODEL_OHC100)
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인 완료:%s[002]"), g_clTaskWork[m_nUnit].m_szChipID);
#else
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인 완료:%s[001]"), g_clTaskWork[m_nUnit].m_szChipID);
#endif
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
#if (____MACHINE_NAME == MODEL_OHC100)
			_stprintf_s(szLog, SIZE_OF_1K, _T("[BCR] 100H 프로그램입니다. 모델 확인 바랍니다. (002)\n계속 진행하시겠습니까?"));
#else
			_stprintf_s(szLog, SIZE_OF_1K, _T("[BCR] 150H 프로그램입니다. 모델 확인 바랍니다. (001)\n계속 진행하시겠습니까?"));
#endif
			if (g_ShowMsgModal(_T("확인"), szLog, RGB_COLOR_BLUE) == false)
			{
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
		g_clLaonGrabberWrapper[m_nUnit].rawReSet();

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


		if (g_clModelData[m_nUnit].m_nMasterModeUse == 1)
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
		}


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



		//g_clMesCommunication[m_nUnit].m_dwCycleTactEndTime = GetTickCount();
		//g_clMesCommunication[m_nUnit].m_dwMesCycleTime = (g_clMesCommunication[m_nUnit].m_dwCycleTactEndTime - g_clMesCommunication[m_nUnit].m_dwCycleTactStartTime) / 1000;		//SEC

		nRetStep = 116500;
		//if (g_clMesCommunication[m_nUnit].MesEolSave(m_nUnit) == false)//mes 저장
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MES SAVE FAIL! \n통신상태,LOT 확인바랍니다[STEP : %d]"), nStep);
		//	AddLog(szLog, 1, m_nUnit, true);
		//	nRetStep = -116000;
		//	break;
		//}
		//else
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] MES SAVE COMPLETE! [STEP : %d]"), nStep);
		//	AddLog(szLog, 0, m_nUnit, false);

		//	nRetStep = 116500;
		//}
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
		//sprintf_s(szLog, "OC X :%.2lf / OC Y :%.2lf", g_clMandoInspLog[m_nUnit].m_UvAfter_OC_DelatX, g_clMandoInspLog[m_nUnit].m_UvAfter_OC_DelatY);
		//AddLog(szLog, 0, m_nUnit);
		//g_clVision.DrawMOverlayText(0, g_clModelData[m_nUnit].m_nWidth * 0.55, 700, szLog, M_COLOR_CYAN, _T("Arial"), 15, 20);

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
//-----------------------------------------------------------------------------
//
//	EOL 원점(10000 ~ 20000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::HomeProcess(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	CString sMsg = _T("");
	int nHomeFlag = 0;
	int nRetStep = 0;
	int i = 0;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	nRetStep = nStep;

	if (g_clDioControl.CurtainDoorCheck(m_nUnit, true) == true)
	{
		g_clMotorSet.StopAxisAll(m_nUnit);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 원점동작 중 CURTAIN 감지. 일시 정지"));
		nRetStep *= -1;
		AddLog(szLog, 1, m_nUnit, true);
		return nRetStep;
	}
	switch (nStep)
	{
	case 10000:
		nRetStep = 10100;
		break;
	case 10100:

		//WhiteOcLightControl[m_nUnit].DPS_Light_OnOffLevel(LIGHT_OC_WHITE_CH1, 0, 0);

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물조명 OFF [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		/*

		if (g_clDioControl.OCCylinderFor(m_nUnit, false) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 이물조명 후진 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10100;
			break;
		}
		
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();*/
		nRetStep = 10200;
		break;

	case 10200:
		/*if (g_clDioControl.OCCylinderForCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 이물조명 후진 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 10300;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 5000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 이물조명 후진 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10100;
			break;
		}*/
		nRetStep = 10300;
		break;
	case 10300:
		//! PCB Z축 (-)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_Z);
			Sleep(100);
			break;
		}
		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_Z, (2 * -1), 0.1, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10300;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 10400;
		break;

	case 10400:
		//! PCB Z축 (-)Limit 이동 확인
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_Z))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 10500;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 (-)Limit 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -10300;
			break;
		}
		break;
	case 10500:
		/*_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		if (g_clDioControl.DarkPusher(m_nUnit, true) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10500;
			break;
		}*/

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 10600;
		break;

	case 10600:
		nRetStep = 10700;
		break;

		/*if (g_clDioControl.DarkPusherCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 10700;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 5000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10500;
			break;
		}*/
		break;

	case 10700:
		nRetStep = 10720;
		break;
	case 10720:
		nRetStep = 10730;
		break;
	case 10730:
		nRetStep = 10740;
		break;
	case 10740:
		if (g_clDioControl.PcbSensorCheck(m_nUnit) == true)
		{
			if (g_clDioControl.PcbVacuumCheck(m_nUnit, true) == false)		//소켓 흡착 상태 확인
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Vacuum 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);

				nRetStep = -10740;
				break;
			}
		}
		nRetStep = 10750;
		break;
	case 10750:
		//! PCB Y축 (-)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_Y);
			Sleep(100);
			break;
		}

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_Y, (20 * -1), 0.1, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Y축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10750;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Y축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();

		nRetStep = 10800;
		break;

	case 10800:
		//! PCB Y축 (-)Limit 이동 확인
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) && g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_Y))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Y축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 10840;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Y축 (-)Limit 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10750;
			break;
		}
		break;
	case 10840:
		//! PCB X축 (-)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_X) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_X);
			Sleep(100);
			break;
		} 

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_X, (20 * -1), 0.1, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10750;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 10900;
		
		break;

	case 10900:

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 10950;
		break;
	case 10950:
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_X) && g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_X))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 11000;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X축 (-)Limit 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -10840;
			break;
		}
		break;
	case 11000:
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 11050;
		break; 

	case 11050:
		nRetStep = 11100;
		break;
	case 11100:
		
		nRetStep = 11150;
		break;
	case 11150:
		nRetStep = 11200;
		break;
	case 11200:
		
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 11250;
		break;
	case 11250: 
		nRetStep = 11300;
		break;
	case 11300:
		//! PCB TH축 (+)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TH) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TH);
			Sleep(100);
			break;
		}

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TH, (5 * 1), 0.1, HomeSensor, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (+)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11300;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (+)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount(); 
		nRetStep = 11320; 
		break;
	case 11320:
		//! PCB TH축 (+)Limit 이동 확인
		if (g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TH))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TH);
			Sleep(100);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 HOME 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11400;
			break;
		}
		if (g_clMotorSet.GetPosiSensor(m_nUnit, MOTOR_PCB_TH))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (+)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11340;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (+)Limit 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11300;
			break;
		}
		 
		break;
	case 11340:
		//! PCB TH축 (-)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TH) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TH);
			Sleep(100);
			break;
		}

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TH, (5 * -1), 0.1, HomeSensor, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11340;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 11360;
		break;
	case 11360:
		//! PCB TH축 HOME 이동 확인
		if (g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TH))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TH);
			Sleep(100);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 HOME 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11400;
			break;
		}
		if (g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_TH))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11400;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TH축 HOME 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11360;
			break;
		}
		break;
	case 11400:
		//! PCB TX, TY축 (+)Limit 이동
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TX);
			Sleep(100);
			nRetStep = 11400;
			break;
		}
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) == false)
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TY);
			Sleep(100);
			nRetStep = 11400;
			break;
		}
		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TX, (5 * -1), 0.1, HomeSensor, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11100;
			break;
		}
		Sleep(100);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep); 
		AddLog(szLog, 0, m_nUnit);

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TY, (5 * -1), 0.1, HomeSensor, SIGNAL_UP_EDGE, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (-)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11100;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (-)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

	
		nRetStep = 11500;
		break;
	case 11500:
		//! PCB TX, TY축 (+)Limit 이동 확인
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) && (g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_TX) || g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TX)))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TX);
			Sleep(100);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 11600;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (-)Limit 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11400;
			break;
		}
		break;
	case 11600:

		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) && (g_clMotorSet.GetNegaSensor(m_nUnit, MOTOR_PCB_TY) || g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TY)))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TY);
			Sleep(100);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (-)Limit 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11700;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (-)Limit 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11400;
			break;
		}
		
		break;
	case 11700:
		//! PCB TX, TY축 (-)Limit 이동

		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TX, (5 * 1), 0.1, HomeSensor, SIGNAL_HIGH_LEVEL, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (+)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11700;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 (+)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 11800;
		break;
	case 11800:
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) == false && g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TX))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TX);
			Sleep(100);
			break;
		}
		if ( g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TX))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TX);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX축 HOME 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 11850;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 HOME 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11800;
			break;
		}
		
		break;

	case 11850:
		if (g_clMotorSet.MoveAxisLimit(m_nUnit, MOTOR_PCB_TY, (5 * 1), 0.1, HomeSensor, SIGNAL_HIGH_LEVEL, SLOWDOWN_STOP) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (+)Limit 위치 구동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -11850;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 (+)Limit 위치 구동 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 11900;

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		break;
	case 11900:
		//! PCB TX, TY축 HOME 이동 확인
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) == false && g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TY))
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TY);
			Sleep(100);
			break;
		}
		
		
		if (g_clMotorSet.GetHomeSensor(m_nUnit, MOTOR_PCB_TY) )
		{
			g_clMotorSet.StopAxis(m_nUnit, MOTOR_PCB_TY);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TY축 HOME 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 12000; 
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 HOME 위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -11900;
			break;
		}
		break; 
	case 12000:

		nRetStep = 12400;
		break;
	case 12400:
		nRetStep = 12500;
		break;

	case 12500:
		//! 전체 원점 동작 시작
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 전체 원점 검색 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			g_clMotorSet.m_nHomeProcUsing[m_nUnit][i] = 1; 
		}
		nRetStep = 13000;
		break;

	case 13000:
		m_bHomeProc = true;
		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			if (g_clMotorSet.m_nHomeProcUsing[m_nUnit][i] == 1)
			{
				g_clMotorSet.m_bOrgFlag[m_nUnit][i] = false;
				if (g_clMotorSet.StartHomeSearch(m_nUnit, i) == false)
				{
					m_bHomeProc = false;

					_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] 모터 원점 동작 명령 실패 [STEP : %d]"), MOTOR_NAME[i], nStep);
					AddLog(szLog, 1, m_nUnit);

					break;
				}
				else 
				{
					// 동작 중으로 변경
					g_clMotorSet.m_nHomeProcUsing[m_nUnit][i] = 2;
				}
			}
		}

		if (m_bHomeProc == false) {
			nRetStep = -13000; 
		}
		else {
			nRetStep = 14000;
		}
		break;
	case 14000:
		m_bHomeProc = true;
		m_bHomeError = false;
		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			if (g_clMotorSet.m_nHomeProcUsing[m_nUnit][i] == 2) 
			{
				nHomeFlag = g_clMotorSet.IsCompletedHomeSearch(m_nUnit, i);
				// 원점 완료
				if (nHomeFlag == 1)
				{
					g_clMotorSet.m_bOrgFlag[m_nUnit][i] = true;
				}
				// 원점 검색 중
				else if (nHomeFlag == 2)
				{
					m_bHomeProc = false;
				}
				// 원점 동작 ERROR
				else
				{
					m_bHomeError = true;


					_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] 모터 HOME 동작 ERROR 발생[STEP : %d]"), MOTOR_NAME[i], nStep);
					AddLog(szLog, 1, m_nUnit);

					break;
				}
			} 
		}

		if (m_bHomeError == true)
		{
			nRetStep = -13000;
			break;
		}

		if (m_bHomeProc == true)
		{
			nRetStep = 15000;
			break;
		}
		break;

	case 15000:
		/*if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -15000;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 대기위치 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();*/
		nRetStep = 16000;
		break;
	case 16000:
		//! PCB Z축 대기 위치 이동 확인
		nRetStep = 18000;
		break;
		//dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Z];
		//curPos = g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z);// -g_clSysData.m_dOrgDataset[m_nUnit][MOTOR_PCB_Z];
		//if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && (curPos - dPos[0]) < ENCORDER_GAP)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
		//	AddLog(szLog, 0, m_nUnit);
		//	nRetStep = 18000;
		//	break;
		//}

		//if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
		//	AddLog(szLog, 1, m_nUnit, true);

		//	nRetStep = -15000;
		//	break;
		//}
		break;
	case 18000:
		//! 전체 원점 동작 완료
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].state = 1;
		g_pCarAABonderDlg->m_clColorButtonStartingPoint[m_nUnit].Invalidate();
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 전체 원점 복귀 완료[STEP : %d]"), nStep); 

		AddLog(szLog, 0, m_nUnit);
		g_ShowMsgPopup(_T("INFO"), _T("[AUTO] 전체 원점 복귀 완료"), RGB_COLOR_RED);
		nRetStep = 20000;
		break;

	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 원점 복귀 이상 STEP 발생[STEP : %d]"), nStep);
		AddLog(szLog, 1, m_nUnit, true);
		nRetStep = -10000;
		break;
	}

	return nRetStep;
}


//-----------------------------------------------------------------------------
//
//	EOL 운전준비(20000 ~ 30000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::AutoReadyProcess(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	int nRetStep;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	CString sMsg = _T("");
	int i = 0;
	nRetStep = nStep;

	if (g_clDioControl.CurtainDoorCheck(m_nUnit, true) == true)
	{
		g_clMotorSet.StopAxisAll(m_nUnit);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] 운전 운전 중 CURTAIN 감지. 일시 정지"));
		nRetStep *= -1;
		AddLog(szLog, 1, m_nUnit, true);
		return nRetStep;
	}
	switch (nStep)
	{
	case 20000:
		
		nRetStep = 20100;
		break;
	case 20100:
		nRetStep = 20150;
		break;
	case 20150:
		//WhiteOcLightControl[m_nUnit].DPS_Light_OnOffLevel(LIGHT_OC_WHITE_CH1, 0, 0);

		//_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 이물조명 OFF [STEP : %d]"), nStep);
		//AddLog(szLog, 0, m_nUnit);
		nRetStep = 20200;
		break;
	case 20200:
		if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20200;
			break;
		}
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 대기위치 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 20300;
		break;

	case 20300:
		//! PCB Z축 대기 위치 이동 확인
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Z];
		curPos = g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z);		// -g_clSysData.m_dOrgDataset[m_nUnit][MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && (curPos - dPos[0]) < ENCORDER_GAP)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 20400;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20200;
			break;
		}
		break;

	case 20400:
		if (g_clMotorSet.MovePcbTxTyMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 WAIT_POS 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20400;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 WAIT_POS 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 20500;
		break;

	case 20500:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_TX];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_TY];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TX) - dPos[0]) < ENCORDER_GAP)) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TY) - dPos[1]) < ENCORDER_GAP)))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 WAIT_POS 위치 이동 완료 [STEP : %d"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 20600;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20650;
			break;
		}
		break;

	case 20600:
		if (g_clDioControl.PcbSensorCheck(m_nUnit) == true)
		{
			if (g_clDioControl.PcbVacuumCheck(m_nUnit, true) == false)		//소켓 흡착 상태 확인
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB Vacuum 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);

				nRetStep = -20600;
				break;
			}
		}
		if (g_clMotorSet.MovePcbYMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB Y축 대기위치 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20600;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB Y축 대기위치 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 20640;
		break;

	case 20640:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Y];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Y) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB Y축 대기위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 20700;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB Y축 대기위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -20600;
			break;
		}
		break;
	case 20700:
		if (g_clMotorSet.MovePcbXYTMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB X,Y,T축 대기위치 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21300;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB X,Y,T축 대기위치 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 21300;
		break;

	case 21300:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] CCD CLOSE [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clLaonGrabberWrapper[m_nUnit].CloseDevice(); 
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 21500;
		break;
	case 21500:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_X];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Y];
		dPos[2] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_TH];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_X) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TH) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TH) - dPos[2]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB X,Y,T축 대기위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 21600;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO READY] PCB X,Y,T축 대기위치 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -21300;
			break;
		}
		break;
	
	case 21600: 
		nRetStep = 21770;
		break;
	case 21770:
		nRetStep = 21800;
		break;
	case 21800:

		nRetStep = 21850;
		break;
	case 21850:
		nRetStep = 22000;
		break;
	case 22000:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			g_clTaskWork[m_nUnit].m_IoRetry = 0;
			nRetStep = 22500;
		}
		
		break;
	case 22500:
		if (g_clDioControl.PcbSensorCheck(m_nUnit) == false)
		{
			//제품 없음 
			//탈락 추가?
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB SOCKET 제품 미확인 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			g_clDioControl.PcbVacuum(m_nUnit, false);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB SOCKET Vacuum 탈착 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			//제품 o
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB SOCKET 제품 확인 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		
		nRetStep = 23000;
		break;
	case 23000:
		g_clVision.ClearOverlay(m_nUnit, VIDEO_CAM);
		g_clVision.DrawOverlayAll(m_nUnit, VIDEO_CAM);

		
		nRetStep = 23500;
		break;
	case 23500:
		for (i = 0; i < 20; i++)
		{
			g_clTaskWork[m_nUnit].m_bOutputCheck[i] = true;	//READY 초기화
		}
		nRetStep = 24000;
		break;
	case 24000:

		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount(); 
		nRetStep = 28000;
		break;
	case 28000:
		//차트
		TopChartControl[m_nUnit + 1].dpctrlLedVolume(LIGHT_CHART_CH_1, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_TOP_CHART_1]);
		Sleep(10);
		LightLeftChartControl[m_nUnit + 1].dpctrlLedVolume(LIGHT_CHART_CH_1, g_clModelData[m_nUnit].m_nLight[LEFT_CHART_CH_1]);
		Sleep(10);
		LightRightChartControl[m_nUnit + 1].dpctrlLedVolume(LIGHT_CHART_CH_1, g_clModelData[m_nUnit].m_nLight[RIGHT_CHART_CH_1]);
		Sleep(10);
		
		//align
		BackDuLightControl[m_nUnit].ctrlLedVolume(LIGHT_BD_ALIGN_CH1, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_ALIGN_SENSOR]);
		Sleep(10);
		//이물광원
		BackDuLightControl[m_nUnit].ctrlLedVolume(LIGHT_BD_OC_CH1, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_OC_WHITE1]);
		Sleep(10);

		nRetStep = 29000;
		break;
	case 29000:
		g_clDioControl.SetTowerLamp(LAMP_YELLOW, true);
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

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 운전준비 완료 [STEP : %d]"), nStep);
		g_ShowMsgPopup(_T("INFO"), _T("[AUTO] 운전준비 완료"), RGB_COLOR_RED, 1);

		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] 운전준비 STEP 이상 [STEP : %d]"), nStep);
		AddLog(szLog, 1, m_nUnit, true);
		nRetStep = -20000;
		break;
	}
	return nRetStep;
}


//-----------------------------------------------------------------------------
//
//	EODL PCB 로딩(30000 ~ 40000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::Auto_M_PCBLoading(int nStep)
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
		mOcLightChannel = 1;	//channel init

		g_clTaskWork[m_nUnit].m_ContactCount = 0;		//step 30000
		g_clTaskWork[m_nUnit].m_nCcdCount = 0;			//step 30000

		g_clTaskWork[m_nUnit].m_nEmissionRun = false;
		g_clTaskWork[m_nUnit].m_bPcbInterLock = true;
		if (g_clModelData[m_nUnit].m_nPinStopUse == 1)
		{
			if (g_clTaskWork[m_nUnit].m_nUsePinCount > g_clSysData.m_nMaxPinCount)//g_clModelData[m_nUnit].m_nSensorCnt)
			{
				sMsg.Format(_T("Socket PIN %d(Spec: %d) \nReset after replacement"), g_clTaskWork[m_nUnit].m_nUsePinCount, g_clSysData.m_nMaxPinCount);// g_clModelData[m_nUnit].m_nSensorCnt);
				AddLog(sMsg, 1, m_nUnit, true);
				nRetStep = -30000;
				break;
			}
		}
		if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS, 0.0, true) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 대기 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30000;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 대기 위치 이동 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 30020;
		mLightRetry = 0;
		break;
	case 30020:
		nRetStep = 30030;
		break;
	case 30030:
		nRetStep = 30050;
		break;
	case 30050:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO][PCB] PCB X,Y,T축 투입 위치 이동 명령 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		if (g_clMotorSet.MovePcbXYTMotor(m_nUnit, SUPPLY_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO][PCB] PCB X,Y,T축 투입 위치 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30050;
			break;
		}

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 30070;
		break;
	case 30070:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_X];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_Y];
		dPos[2] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_TH];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_X) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TH) && (g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TH) - dPos[2]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X,Y,T축 SUPPLY_POS 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 30075;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB X,Y,T축 SUPPLY_POS 이동 시간초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30050;
			break; 
		}
		break;
	case 30075:
		if (g_clMotorSet.MovePcbTxTyMotor(m_nUnit, SUPPLY_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 SUPPLY_POS 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30075;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 SUPPLY_POS 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 30080;
		break;

	case 30080:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_TX];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_TY];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TX) - dPos[0]) < ENCORDER_GAP)) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TY) - dPos[1]) < ENCORDER_GAP)))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 SUPPLY_POS 위치 이동 완료 [STEP : %d"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 30090;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 SUPPLY_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30075;
			break;
		}
		break;
	case 30090:

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 공급 위치 이동 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		if (g_clMotorSet.MovePcbZMotor(m_nUnit, SUPPLY_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 공급 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30090;
			break;
		}
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 30110;
		break;
	case 30110: 
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[SUPPLY_POS].dPos[MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 SUPPLY_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 30115;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 SUPPLY_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30090;
			break;
		}

		break;
	case 30115:
		if (g_clDioControl.PcbSensorCheck(m_nUnit) == false)
		{
			g_clDioControl.PcbVacuum(m_nUnit, true);
		}

		g_clDioControl.Start_Button_LedOn(m_nUnit, true, false);
		nRetStep = 30120;
		break;

	case 30120:		//<--step 번호 사용중
		if (g_clDioControl.StartBbCheck(m_nUnit) == true)		//스타크 버튼 감지되면 Start
		{
			nRetStep = 30125;

			g_clDioControl.Start_Button_LedOn(m_nUnit, false , false);

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] START PB PRESS. [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
		}
		break;
	case 30125:
		if (g_clDioControl.PcbSensorCheck(m_nUnit) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB 제품 감지 실패 [STEP : %d]"), nStep);

			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -30120;
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
		if (g_clDioControl.PcbVacuumCheck(m_nUnit , true) == false)		//소켓 흡착 상태 확인
		{
			g_clDioControl.PcbVacuum(m_nUnit, true);

			if (g_ShowMsgModal(_T("확인"), _T("[AUTO] 흡착 상태 확인후 진행해주세요"), RGB_COLOR_RED) == false)
			{
				nRetStep = -30125;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Vacuum 확인 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);
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
		BackDuLightControl[m_nUnit].ctrlLedVolume(LIGHT_BD_ALIGN_CH1, g_clModelData[m_nUnit].m_nLight[LIGHT_DATA_ALIGN_SENSOR]);
		nRetStep = 30400;
		break;
		//if (g_clDioControl.PcbSensorCheck(m_nUnit, true) == false)
		//{
		//	//제품 없음
		//	g_clTaskWork[m_nUnit].m_bPcbSocketIF[PCB_BCR] = false;	//제품 xxx
		//	_stprintf_s(g_clTaskWork[m_nUnit].m_szChipID, SIZE_OF_100BYTE, _T("EMPTY")); //bcr 초기화
		//	g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		//	g_clTaskWork[m_nUnit].m_nEmission = 0;
		//	g_clTaskWork[m_nUnit].SaveData();
		//	g_pCarAABonderDlg->ShowOkNgState(m_nUnit, g_clTaskWork[m_nUnit].m_nEmission);

		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB SOCKET 제품 미확인 [STEP : %d]"), nStep);
		//	AddLog(szLog, 0, m_nUnit);

		//	nRetStep = 30400;
		//	break;
		//}
		//else
		//{
		//	g_clTaskWork[m_nUnit].m_bPcbSocketIF[PCB_BCR] = true;	//제품 ooo
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB SOCKET 제품 확인 [STEP : %d]"), nStep);
		//	AddLog(szLog, 0, m_nUnit);

		//	if (g_ShowMsgModal(_T("확인"), _T("[AUTO] PCB 제품이 있습니다. 진행하시겠습니까?"), RGB_COLOR_RED) == false)
		//	{
		//		nRetStep = -30200;
		//	}
		//	else
		//	{
		//		g_clTaskWork[m_nUnit].m_IoRetry = 0;
		//		nRetStep = 30700;	//jump
		//	}
		//}
		break;
	case 30400:
		MbufClear(g_clVision.MilDefectOverlayImage[m_nUnit], (MIL_DOUBLE)g_clVision.m_nMilDefectTransparent[m_nUnit]);
		MbufClear(g_clVision.MilDefectImage[m_nUnit], 0);
		nRetStep = 30500;
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		break;

	case 30500:
		nRetStep = 30700;
		break;
	case 30700:
		
		nRetStep = 31000;
		break;
	case 31000:
		nRetStep = 32000;
		break;
	case 32000:
		//딜레이
		nRetStep = 32500;
		break;
	case 32500:
		nRetStep = 32600;
		
		break;
	case 32600:
		// MES 확인
		nRetStep = 32750;
		break;
	case 32750:
		// 바코드값 수신
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인:%s [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
		AddLog(szLog, 0, m_nUnit);
		g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		g_clModelData[m_nUnit].LotDataSave();
		nRetStep = 32755;
		break;

		//if ((g_clTaskWork[m_nUnit].m_bPcbSocketIF[PCB_BCR] == true && _tcslen(g_clTaskWork[m_nUnit].m_szChipID) > 0))//11))/// || g_clModelData[m_nUnit].m_nMesPass == 1)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO]BCR 확인:%s [STEP : %d]"), g_clTaskWork[m_nUnit].m_szChipID, nStep);
		//	AddLog(szLog, 0, m_nUnit);
		//	g_pCarAABonderDlg->ShowBarcode(m_nUnit);
		//	g_clModelData[m_nUnit].LotDataSave();
		//	nRetStep = 32755;
		//	break;
		//}
		//else if (GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount > 6000)
		//{
		//	_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB BCR 확인 실패 [STEP : %d]"), nStep);
		//	AddLog(szLog, 1, m_nUnit, true);
		//	nRetStep = -36000;
		//}
		
		break;
	case 32755:
		nRetStep = 32770;
		break;
	case 32770:
		nRetStep = 32780;
		break;
	case 32780:
		nRetStep = 32900;
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
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 1;	//시작할대 초기화부분 EOL
		
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
		g_clLaonGrabberWrapper[m_nUnit].UiconfigLoad(INI_YUV_IMAGE);	//YUV AUTO start
		g_clLaonGrabberWrapper[m_nUnit].SelectSensor();
		g_clLaonGrabberWrapper[m_nUnit].AllocImageBuff();
		g_clLaonGrabberWrapper[m_nUnit].rawReSet();

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
		
		if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -37500;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PRODUCT PCB LOADING 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		 
		g_clTaskWork[m_nUnit].m_nRetryCount = 0;
		nRetStep = 38000;
		break;

	case 38000:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 38500;

			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -37500;
			break;
		}
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

		
		nRetStep = 40000;
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
//	Lens 로딩(50000 ~ 60000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::AutoLensLoading(int nStep)
{

	TCHAR szLog[SIZE_OF_1K];
	double dPos[3] = { 0.0, 0.0, 0.0 };
	CString sMsg = _T("");
	int nRetStep = nStep;

	return nRetStep;

}


//-----------------------------------------------------------------------------
//
//	LENS , PCB CHART 이동(60000 ~ 70000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::AutoChartMoving(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	double dPos[3] = { 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	int i = 0;
	CString sMsg = _T("");
	int nRetVal = 0;
	int nRetStep;
	bool bResult = false;

	return nRetStep;
}

//-----------------------------------------------------------------------------
//
//	DARK 이미지 CAPTURE(70000 ~ 80000)	
//-----------------------------------------------------------------------------
int CPcbProcess::Auto_DarkImage_Get(int nStep)
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
	nRetStep = nStep;

	switch (nStep)
	{
	case 90000:
		g_pCarAABonderDlg->CAMChangeHandler(m_nUnit, VIDEO_CCD);
		g_clVision.ClearOverlay(m_nUnit);
		g_clVision.DrawOverlayAll(m_nUnit);

		g_clTaskWork[m_nUnit].m_dwTotalTactTime = GetTickCount();
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 90200;
		break;
	case 90200:
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) == true)
		{
			if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS) == false)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 실패 [STEP : %d]"), nStep);
				AddLog(szLog, 1, m_nUnit, true);

				nRetStep = -90200;
				break;
			}

			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 90400;
		}

		break;
	case 90400:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 91000;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -90200;
			break;
		}

		break;
	case 91000:
		/*if (g_clMotorSet.MovePcbXYTMotor(m_nUnit, DARK_POS, g_clTaskWork[m_nUnit].m_dSensorAlign[0], g_clTaskWork[m_nUnit].m_dSensorAlign[1], g_clTaskWork[m_nUnit].m_dSensorAlign[2]) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO][PCB] PCB DARK POS 검사 위치 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);
			nRetStep = -91000;
			break;
		}*/

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO][PCB] PCB DKARK POS 검사 위치 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 91200;
		break;
	case 91200:

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 91500;
		break;
	case 91500:
		//! PCB X,Y,T축 Chart 검사 위치 이동 확인
		/*dPos[0] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_X] + g_clTaskWork[m_nUnit].m_dSensorAlign[0];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_Y] + g_clTaskWork[m_nUnit].m_dSensorAlign[1];
		dPos[2] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_TH] + g_clTaskWork[m_nUnit].m_dSensorAlign[2];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_X) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP)) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Y) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP)) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TH) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TH) - dPos[2]) < ENCORDER_GAP))
			)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB DARK POS 검사위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 92000;
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB DARK POS 검사 위치  이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -91000;
			break;
		}*/
		break;
	case 92000:
		/*if (g_clMotorSet.MovePcbTxTyMotor(m_nUnit, DARK_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 DARK_POS 이동 명령 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -92000;
			break;
		}*/

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 DARK_POS 이동 명령 성공 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 92500;
		break;
	case 92500:
		/*dPos[0] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_TX];
		dPos[1] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_TY];
		if ((g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TX) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TX) - dPos[0]) < ENCORDER_GAP)) &&
			(g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_TY) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_TY) - dPos[1]) < ENCORDER_GAP)))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 DARK_POS 위치 이동 완료 [STEP : %d"), nStep);
			AddLog(szLog, 0, m_nUnit);

			nRetStep = 93000;
			break;
		}
		else if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB TX,TY축 DARK_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -92000;
			break;
		}*/
		break;
	case 93000:
		//다크 실린더 하강
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 하강 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		if (g_clDioControl.DarkPusher(m_nUnit, false) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 하강 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -93000;
			break;
		}

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 93500;
		break;

	case 93500:
		/*if (g_clMotorSet.MovePcbZMotor(m_nUnit, DARK_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 DARK POS 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -93500;
			break;
		}*/

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 DARK POS 위치 이동 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();

		nRetStep = 94000;
		break;
	case 94000:
		/*dPos[0] = g_clModelData[m_nUnit].m_stTeachData[DARK_POS].dPos[MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 DARK_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 94500;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 DARK_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -93500;
			break;
		}*/
		break;
	case 94500:
		nRetStep = 95000;
		break;
		/*if (g_clDioControl.DarkPusherCheck(m_nUnit, false) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 하강 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 95000;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 5000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 하강 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -93000;
			break;
		}*/
		break;
	case 95000:
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 300)
		{
			nRetStep = 95500;
		}
		break;
	case 95500:
		g_clLaonGrabberWrapper[m_nUnit].CopyInspImage(LOW_LEVEL_RAW, g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer);
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK Image Save Ok! [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = 96000;

		break;
	case 96000:

		nRetStep = 96500;
		break;
	case 96500:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);

		if (g_clDioControl.DarkPusher(m_nUnit, true) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -96500;
			break;
		}

		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 97000;
		break;
	case 97000:
		if (g_clMotorSet.MovePcbZMotor(m_nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 실패 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -97000;
			break;
		}

		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
		nRetStep = 97500;
		break;
	case 97500:
		dPos[0] = g_clModelData[m_nUnit].m_stTeachData[WAIT_POS].dPos[MOTOR_PCB_Z];
		if (g_clMotorSet.GetStopAxis(m_nUnit, MOTOR_PCB_Z) && ((g_clMotorSet.GetEncoderPos(m_nUnit, MOTOR_PCB_Z) - dPos[0]) < ENCORDER_GAP))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			nRetStep = 98000;
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			break;
		}

		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] PCB Z축 WAIT_POS 위치 이동 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -97000;
			break;
		}
		break;
	case 98000:
		nRetStep = 98500;
		break;
		/*if (g_clDioControl.DarkPusherCheck(m_nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 확인 완료 [STEP : %d]"), nStep);
			AddLog(szLog, 0, m_nUnit);
			g_clTaskWork[m_nUnit].m_dwPcbTickCount = GetTickCount();
			nRetStep = 98500;
			break;
		}
		if ((GetTickCount() - g_clTaskWork[m_nUnit].m_dwPcbTickCount) > 5000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK CYLINDER 상승 확인 시간 초과 [STEP : %d]"), nStep);
			AddLog(szLog, 1, m_nUnit, true);

			nRetStep = -96500;
			break;
		}*/
		break;
	case 98500:
		nRetStep = 100000;
		break;
	default:
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] DARK  STEP 발생 [STEP : %d]"), nStep);
		AddLog(szLog, 0, m_nUnit);
		nRetStep = -1;
		break;
	}
	return nRetStep;
}



//-----------------------------------------------------------------------------
//
//	FCM30_AAB AA(70000 ~ 80000)	
//
//-----------------------------------------------------------------------------
int CPcbProcess::AutoFocus(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	double dMoveDist = 0.0;
	double dMoveVel = 0.0;
	double dMoveAcc = 0.0; 
	int nRetStep;
	
	nRetStep = nStep;

	return nRetStep;
}


//-----------------------------------------------------------------------------
//
//	SFR값이 연속으로 하락하는지 체크
//
//-----------------------------------------------------------------------------
bool CPcbProcess::CheckDecreaseSFR()
{
	int nStartIndex;
	int nCheckIndex;
	int i, j;

	nCheckIndex = g_clModelData[m_nUnit].m_nCountOfCheckSFR;

	if (g_clTaskWork[m_nUnit].m_bFlagDecreaseSFR == true)
	{
		nStartIndex = g_clModelData[m_nUnit].m_nCountOfCheckSFR;
		nCheckIndex = g_clModelData[m_nUnit].m_nCountOfCheckSFR * 2;
	}
	else
	{
		nStartIndex = 0;
	}
	int FieldJumpStep = 0;

	for (i = nStartIndex; i < MAX_MTF_COUNT; i++)
	{
		if (g_clModelData[m_nUnit].m_nSfrMode == FIELD_085_AA && i > 0)
		{
			FieldJumpStep = 4;
		}
		for (j = nStartIndex; j < nCheckIndex; j++)
		{
			if (g_clTaskWork[m_nUnit].m_stSfrInsp.fSFR_AVR_N4[j][i + FieldJumpStep] >= 0.2)
				break;
		}

		if (j == nCheckIndex)
			continue;

		for (j = 1; j < nCheckIndex; j++)
		{
			if (g_clTaskWork[m_nUnit].m_stSfrInsp.fSFR_AVR_N4[j - 1][i + FieldJumpStep] - g_clTaskWork[m_nUnit].m_stSfrInsp.fSFR_AVR_N4[j][i + FieldJumpStep] < 0)
				break;
		}

		if (j == nCheckIndex)
			return false;
		else
			continue;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	SFR값으로 MAX Z축 값 찾기
//
//-----------------------------------------------------------------------------
bool CPcbProcess::GetSfrMaxPos()
{

	return true;
}



//-----------------------------------------------------------------------------
//
//	FCM30_AAB UV(80000 ~ 90000)
//
//-----------------------------------------------------------------------------
int CPcbProcess::AutoUV(int nStep)
{
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szPath[SIZE_OF_1K];
	double dMoveDist = 0.0;
	double dMoveVel = 0.0;
	double dPos[4] = { 0.0, 0.0, 0.0, 0.0 };
	double curPos = 0.0;
	double dCenterPosX = 0.0;
	double dCenterPosY = 0.0;
	int nRetStep;
	bool mRtn = false;

	return nRetStep;
}

