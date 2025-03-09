#include "stdAfx.h"
#include "AxlMotorSet.h"
#include "../XpengInspInsp.h"
#include "../XpengInspInspDlg.h"
#include <math.h>

AxlMotorSet::AxlMotorSet(void)
{
	int iUnit = 0, iAxis;
	dTime = dSTime = 0;

	for (iUnit = 0; iUnit < UNIT_CH; iUnit++)
	{
		for ( iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++ )
		{
			m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;	//���� ���� �ʱ�ȭ
			iHomeProcUsing[iUnit][iAxis] = 0;	//���� ���� �䱸 ���� �ʱ�ȭ
			m_bAxisState[iUnit][iAxis] = 0;			//���� On, OFF �ʱ�ȭ.
		}

		m_hThreadHomeAll[iUnit] = NULL;
		m_hThreadHome[iUnit] = NULL;
		m_hThreadReady[iUnit] = NULL;

		bReadyPusher[iUnit] = false;
	}	

	//! Motor ���� ���� ���� ������ ���� ��� (Motion Controller Library���� �����ϴ� ���)
	m_sMotionSettingFilePath = _T("C:/Program Files/EzSoftware UC/EzSoftware/MotionSetting.mot");
}

AxlMotorSet::~AxlMotorSet(void)
{
	Axl_Close();
}


bool AxlMotorSet::Axl_Init()
{
	int iUnit = 0, iAxis;
	for (iUnit = 0; iUnit < UNIT_CH; iUnit++)
		for ( iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++ )
			m_bOrgFlag[iUnit][iAxis] = CUR_MOTOR_NO;	//�������� �ʱ�ȭ

	DWORD dwRet = 0;
#ifdef	ON_LINE_MOTOR

	dwRet = AxlOpen(0);			//���̺귯�� �ʱ�ȭ  0 : �����Ⱚ..
	if ( dwRet != AXT_RT_SUCCESS )
	{
		sMsg.Format("��� ���� �ʱ�ȭ�� ���� �Ͽ����ϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG + DEF_LOG_POPUP);
		//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
		return false;
	}

	//! ��ϵ� AXL ��밡�� ������ ������ Ȯ���Ѵ�.
	long lBoardCounts = 0;
	AxlGetBoardCount(&lBoardCounts);
	if ( lBoardCounts <= 0 )
	{
		sMsg.Format("��� ���� �ν��� ���� �Ͽ����ϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG + DEF_LOG_POPUP);
		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
		return false;
	}
	sMsg.Format("AXL ��� ���� �ν� �Ϸ�...[ Board Count : %d ]", lBoardCounts);
	g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG);
	//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG, (LPARAM)&sMsg);




	TCHAR* pszPath = NULL;

	pszPath = m_sMotionSettingFilePath.GetBuffer( m_sMotionSettingFilePath.GetLength() + 1 );

	if ( IsExist_Path(m_sMotionSettingFilePath) == true )
	{	//! ��� ���� �ʱ� ���� ������ ���Ͽ��� �о� �����Ѵ�. 
		if ( AxmMotLoadParaAll(pszPath) != AXT_RT_SUCCESS )
		{
			sMsg.Format("��� �Ķ���� ������ �����ϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_SYSTEMLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_SYSTEMLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			return false;
		}
	}
	else
	{	//! ��� ���� �ʱ� ���� ���Ͽ� ���� �Ѵ�. 
		AxmMotSaveParaAll(pszPath);    
	}


	m_sMotionSettingFilePath.ReleaseBuffer();

	//! �ʱ� ����
	for(int iCnt = 0; iCnt < UNIT_CH; iCnt++){
		AmpDisableAll(iCnt);
		Axl_Axisconfig(iCnt);
	}


#endif


	return true;
}


void AxlMotorSet::Axl_Close()
{
	int iUnit = 0, iAxis;

#ifdef ON_LINE_MOTOR

	//! ��� Motor�� �����Ѵ�. 
	for(iAxis=0; iAxis<CUR_MOTOR_NO*UNIT_CH; iAxis++)
		AxmMoveSStop(iAxis);		

	TCHAR* pszPath = NULL;
	pszPath = m_sMotionSettingFilePath.GetBuffer( m_sMotionSettingFilePath.GetLength() + 1 );
	//! ��� ���� ���� �������� ���Ͽ� ���� �Ѵ�. 
	AxmMotSaveParaAll(pszPath);
	m_sMotionSettingFilePath.ReleaseBuffer();

	AxlClose();

#endif


	for (iUnit = 0; iUnit < UNIT_CH; iUnit++)
		for ( iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++ )
			m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;	//�������� �ʱ�ȭ
}


//! ��� ��ü�� Axis�� �ʱ� ������ �����Ѵ�. 
//! [���� ����] 
//!    1) �ݵ��, 'Axl_Init' �Լ��� 'AxmMotLoadParaAll' �Լ� ȣ�� �Ŀ� �� �Լ��� ȣ���Ͽ��� �Ѵ�. 
//!    2) PCI-R1604�� A4N, A5N��������̺��� ���(PM����) Limit/Home/Servo On/Inposition�� Level�� ���� �Ͻ� �� ���� High�Ǵ� Low�� ������ �Ǿ��ֽ��ϴ�. 
//!		  Limit ������ A/B���� �ٲٰ��� �Ҷ��� ��������̺��� �Ķ���͸� �ٲپ�� �մϴ�.
int AxlMotorSet::Axl_Axisconfig(int iUnit) 
{

#ifdef	ON_LINE_MOTOR
	DWORD Status = 0;
	DWORD dwReturn = 0;

	//! ���峻�� ���ͷ�Ʈ ����� ���� ��Ų��. (RTEX ���� ����)
	AxlInterruptDisable();


	//! �ý��ۿ� ������ �� ������ Ȯ��
	long lAxisCount = 0;
	AxmInfoGetAxisCount(&lAxisCount);
	if ( lAxisCount != CUR_MOTOR_NO*UNIT_CH )
	{
		sMsg.Format("���� ����̺� ���� ����ġ[%d / %d].", lAxisCount, CUR_MOTOR_NO*UNIT_CH);
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
		//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
		return -4;
	}

	//sMsg.Format("AXT ���� ����̺� �ν� �Ϸ�- [ Count : %d ].", lAxisCount);
	//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_SYSTEMLOG , (LPARAM)&sMsg);


	int    iFailed = -1;
	int iStartAxis = CUR_MOTOR_NO * iUnit;
	int iEndAxis = CUR_MOTOR_NO * (iUnit + 1);
	for(int i=iStartAxis; i<iEndAxis; i++)
	{
// 		if(i == 4) continue;
// 		if(i == 12) continue;
		dwReturn = AxmInfoIsInvalidAxisNo(i);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ����̺� �ν� ����.",  MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;
			continue;
		}


		dwReturn = AxmMotSetMoveUnitPerPulse(i, 1, 1);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ����̺� ���ڱ��� �ʱ�ȭ ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;	
		}


		dwReturn = AxmMotSetMaxVel(i, MOTOR_MAX_VEL[i%CUR_MOTOR_NO]);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ����̺� �ִ� �ӵ� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;			
		}


		dwReturn = AxmMotSetMinVel(i, 1);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ����̺� �ּ� �ӵ� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;			
		}

		dwReturn = AxmMotSetAccelUnit(i, UNIT_SEC2);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ����̺� ���ӵ� ���� ���� ���� .",  MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;			
		}


		dwReturn = AxmMotSetProfileMode(i, ASYM_S_CURVE_MODE);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� S-Curve ������ ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;
		}


		dwReturn = AxmMotSetAbsRelMode(i, POS_ABS_MODE);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� ���� ��ġ �̵� ��� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;			
		}

		DWORD	dwHomLevel;
		dwReturn = AxmHomeSetSignalLevel(i, LOW);
		dwReturn = AxmHomeGetSignalLevel(i, &dwHomLevel);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ %s ] ���� Home level ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			iFailed = i;			
		}




		//! OneHighLowHigh      1�޽� ���, PULSE(Active High)  +����(DIR=Low)  / -����(DIR=High)
		//! OneHighHighLow      1�޽� ���, PULSE(Active High)  +����(DIR=High) / -����(DIR=Low)
		//! OneLowLowHigh       1�޽� ���, PULSE(Active Low)   +����(DIR=Low)  / -����(DIR=High)
		//! OneLowHighLow       1�޽� ���, PULSE(Active Low)   +����(DIR=High) / -����(DIR=Low)
		//! TwoCcwCwHigh        2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active High
		//! TwoCcwCwLow         2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active Low
		//! TwoCwCcwHigh        2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active High
		//! TwoCwCcwLow         2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active Low		
		if( MOTOR_TYPE[i % CUR_MOTOR_NO] == 4/*STEPING*/)				
		{// Step ���� ����̺�� ��� ���, ����Ʈ ���� Ʋ��.
			dwReturn = AxmSignalSetInpos(i, UNUSED);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� Inposition ���� ����.",  MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}

			//dwReturn = AxmMotSetPulseOutMethod(i, TwoCcwCwHigh);//+-
			dwReturn = AxmMotSetPulseOutMethod(i, TwoCwCcwHigh);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� CW/CCW ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}

			dwReturn = AxmSignalSetStop(i, EMERGENCY_STOP, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� ��� ���� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;			
			}

			dwReturn = AxmSignalSetLimit(i, EMERGENCY_STOP, LOW, LOW);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� Limit ���� ���� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}

			dwReturn = AxmMotSetEncInputMethod(i, ObverseSqr4Mode);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� 4ü�� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;			
			}


			dwReturn = AxmSignalSetServoOnLevel(i, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� Servo In Level ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}
		}
		else
		{
			dwReturn = AxmSignalSetInpos(i, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� Inposition ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}

			dwReturn = AxmSignalSetStop(i, EMERGENCY_STOP, LOW);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� ��� ���� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;			
			}

			dwReturn = AxmSignalSetLimit(i, EMERGENCY_STOP, HIGH, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ %s ] ���� Limit ���� ���� ���� ����.", MOTOR_NAME[i%CUR_MOTOR_NO]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
				iFailed = i;
			}
		}

		dwReturn = AxmSignalSetServoAlarm(i, HIGH);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			iFailed = i;			
		}
	}

	if ( iFailed >= 0 )
	{
		return (-1000 - iFailed);
	}

#endif

	return 1;
}

bool AxlMotorSet::AmpEnable(int iUnit, int iAxis)
{//! ���� ���� Servo-On�� �ǽ�


	DWORD level = ENABLE;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
	StopAxis(iUnit, iAxis);

	//! �̹� Servo On �������� Ȯ���Ѵ�. 
	AxmSignalIsServoOn(iUseAxis, &level);
	if ( level == ENABLE )	
	{
		m_bAxisState[iUnit][iAxis] = true;
		return true;
	}

	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
	AxmSignalServoOn(iUseAxis, ENABLE);	

	Sleep(100);

	AxmSignalIsServoOn(iUseAxis, &level);
	if ( level != ENABLE )
	{
		m_bAxisState[iUnit][iAxis] = false;
		return false;
	}
#endif
	m_bAxisState[iUnit][iAxis] = true;
	return true;
}

bool AxlMotorSet::AmpEnableAll(int iUnit)
{
	bool rtnVal = true;
	DWORD level = ENABLE;
	int i = 0;

	StopAxisAll(iUnit);

	//int iRet_Axis = Axl_Axisconfig();
	//if(iRet_Axis != 1)
	//{
	//	sMsg.Format("���� ���� ����̺� �ʱ�ȭ ����..");
	//	::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG+DEF_MSGP_NG, (LPARAM)&sMsg);
	//	return	false;
	//}

	int iUseAxis; 

	for ( i = 0; i < CUR_MOTOR_NO; i++ )
	{
		if(i == MotLENS_Tx || i == MotLENS_Ty)	continue;
		iUseAxis = iUnit * CUR_MOTOR_NO + i;	//���� ���� ��� �� ��ȣ


//		m_bOrgFlag[iUnit][i] = false;

#ifdef ON_LINE_MOTOR

		AxmSignalIsServoOn(iUseAxis, &level);
		if ( level == ENABLE )	continue;

		AxmSignalServoOn(iUseAxis, ENABLE);	

		SleepHandle(100);
		AxmSignalIsServoOn(iUseAxis, &level);
		if ( level != ENABLE )	
			rtnVal = false;

#endif		
	}

	SleepHandle(500);

	//! 1���� Servo On ���а� ������, �ٽ� �õ��Ѵ�. 
	if ( rtnVal == false )
	{

		rtnVal = true;

		for ( i = 0; i < CUR_MOTOR_NO; i++ )
		{

			iUseAxis = iUnit * CUR_MOTOR_NO + i;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
			//! �̹� Servo On �������� Ȯ���Ѵ�. 
			AxmSignalIsServoOn(iUseAxis, &level);
			if ( level == ENABLE )	continue;

			AxmSignalServoOn(iUseAxis, ENABLE);	
			SleepHandle(300);
			AxmSignalIsServoOn(iUseAxis, &level);
			if ( level != ENABLE )	
				rtnVal = false;
#endif		
		}
	}

	if(!rtnVal)
	{
		sMsg.Format("[ ERROR ] AmpEnableAll ����.");
		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG, (LPARAM)&sMsg);
	}
	else
	{
		for ( i = 0; i < CUR_MOTOR_NO; i++ )
		{
			m_bAxisState[iUnit][i] = true;
		}
	}

	return rtnVal;
}

double AxlMotorSet::CalcAcc_G(int iUnit, int iAxis)
{
	return sysData.fMotorAcc[iUnit][iAxis] * (9.8 * 1000 * sysData.fMotorResol[iUnit][iAxis]);
}

double AxlMotorSet::CalcDec_G(int iUnit, int iAxis)
{
	return sysData.fMotorDec[iUnit][iAxis] * (9.8 * 1000 * sysData.fMotorResol[iUnit][iAxis]);

}
bool AxlMotorSet::AmpDisable(int iUnit, int iAxis)
{//! ���� ���� Servo-Off�� �ǽ�
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
	double dDecel = 0.;

#ifdef ON_LINE_MOTOR

	AxmMoveStop(iUseAxis, dDecel);	
	AxmSignalServoOn(iUseAxis, DISABLE);

	SleepHandle(10);

	if(MOTOR_TYPE[iAxis] == STEPING)	AxmSignalServoAlarmReset(iUseAxis, ENABLE);

#endif

	return true;
}

bool AxlMotorSet::AmpDisableAll(int iUnit)
{
	bool rtnVal = true;
	double dDecel = 0.;
	int iUseAxis = 0;	//���� ���� ��� �� ��ȣ

	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		iUseAxis = iUnit * CUR_MOTOR_NO + i;
		m_bOrgFlag[iUnit][i] = DEF_ORIGIN_NOT;

#ifdef ON_LINE_MOTOR
		AxmMoveStop(iUseAxis, dDecel);		
		AxmSignalServoOn(iUseAxis, DISABLE);
#endif

		//if(MOTOR_TYPE[i] == STEPING)	AxmSignalServoAlarmReset (iUseAxis, ENABLE);
	}

	SleepHandle(300);

	// 	for(int i=0; i<CUR_MOTOR_NO; i++)
	// 	{
	// 		iUseAxis = iUnit * CUR_MOTOR_NO + i;
	// 
	// 		if(MOTOR_TYPE[i] == STEPING)
	// 			AxmSignalServoAlarmReset (iUseAxis, DISABLE);

	//}

	return true;
}

bool AxlMotorSet::GetAmpEnable(int iUnit, int iAxis)
{//! ���� ���� Servo-On ��ȣ�� ��� ���¸� ��ȯ(T=ON, F=OFF ����)
	DWORD upStatus;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	// Servo �˶� Ȯ��.
	AxmSignalIsServoOn(iUseAxis, &upStatus);	// upStatus  1: Servo On
	if(upStatus == ACTIVE)	return true;

	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
	return false;
}


bool AxlMotorSet::GetAmpEnableAll(int iUnit)
{//! AAB Unit�� Servo-Off ���°� ���� ��� false
	DWORD upStatus;
	bool rtnVal = true;
	int iUseAxis = 0;	//���� ���� ��� �� ��ȣ

	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		iUseAxis = iUnit * CUR_MOTOR_NO + i;

		AxmSignalIsServoOn(iUseAxis, &upStatus);	// upStatus  1: Servo On
		if(upStatus != ACTIVE)
		{
			m_bOrgFlag[iUnit][i] = DEF_ORIGIN_NOT;
			rtnVal = false;
		}
	}

	return rtnVal;
}



bool AxlMotorSet::AmpFaultReset(int iUnit, int iAxis)
{//! �˶� Reset�� Servo-On �ǽ�
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ
	double dDecel = 0.;

	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;

#ifdef ON_LINE_MOTOR

	DWORD level;

	AxmMoveStop(iUseAxis, dDecel);	
	AxmSignalServoOn(iUseAxis, DISABLE);

	AxmSignalServoAlarmReset(iUseAxis, ENABLE);

	SleepHandle(100);

	AxmSignalServoAlarmReset(iUseAxis, DISABLE);
	AxmSignalServoOn(iUseAxis, ENABLE);	

	SleepHandle(100);

	AxmSignalIsServoOn(iUseAxis, &level);
	if ( level != ENABLE )	return false;
#endif

	return true;
}

bool AxlMotorSet::AmpFaultResetAll(int iUnit)
{	

	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		AmpDisable(iUnit, i);
	}

	SleepHandle(1000);

	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		AmpEnable(iUnit, i);
	}

	return true;
}

int	AxlMotorSet::AmpFaultCheck(int iUnit)
{//! �˶� ��ȣ Ȯ��
	DWORD upStatus;
	int iAmpFaultFlag = 0;

	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		// ���� �˶� ��ȣ Ȯ��
		AxmSignalReadServoAlarm( (iUnit*CUR_MOTOR_NO) + i, &upStatus );

		if(upStatus == ACTIVE)
		{
			iAmpFaultFlag += 0x01<< i;
			//ISJ
			//m_bOrgFlag[iUnit][i] = true;
			m_bOrgFlag[iUnit][i] = DEF_ORIGIN_NOT;
		}
	}

	return iAmpFaultFlag;
}


bool AxlMotorSet::GetAmpFault(int iUnit, int iAxis)
{//! ���� ���� �˶� ���� ��ȯ
	DWORD dwRet = 0, use = 0, upStatus = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ
	CString sMsg;

#ifdef	ON_LINE_MOTOR

	//kdu 150602
	// 	dwRet = AxmSignalGetServoAlarm(iUseAxis, &use);
	// 	if ( dwRet != AXT_RT_SUCCESS )
	// 	{	
	// 		sMsg.Format("�˶�  �ڵ� : %d || PCB : %d, LENS : %d, LOAD : %d", dwRet, flow[iUnit].iAAStep, flow[iUnit].iLENS_Step, flow[iUnit].iLENS_LoadingStep);
	// 		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);
	// 		return true;
	// 	}
	if ( use == UNUSED )	
		return true;


	// ���� �˶� ��ȣ Ȯ��
	dwRet = AxmSignalReadServoAlarm(iUseAxis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		//		sMsg.Format("�˶�  �ڵ� : %d || PCB : %d, LENS : %d, LOAD : %d", dwRet, flow[iUnit].iAAStep, flow[iUnit].iLENS_Step, flow[iUnit].iLENS_LoadingStep);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);
		m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
		return true;
	}

	if(upStatus == ACTIVE)
	{
		m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
		return true;
	}
#endif

	return false;
}


//! Motor�� ��ǥ ������ �����ߴ��� Ȯ��
bool AxlMotorSet::GetInposition(int iUnit, int iAxis)
{//! ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��

	//m_cri_Inposition.Lock();

	DWORD dwRet = 0, use = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	//! Inposition ��ȣ ���� Ȯ��
	dwRet = AxmSignalGetInpos(iUseAxis, &use);
	if ( dwRet != AXT_RT_SUCCESS )	
	{
		//m_cri_Inposition.Unlock();
		return false;
	}
	if ( use == UNUSED )		
	{
		//m_cri_Inposition.Unlock();
		return false;
	}

	//! Inpositon ��ȣ�� �Է� Ȯ��
	DWORD upStatus = INACTIVE;
	dwRet = AxmSignalReadInpos(iUseAxis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		//m_cri_Inposition.Unlock();
		return false;
	}

	if ( upStatus == INACTIVE )
	{
		//m_cri_Inposition.Unlock();
		return false;
	}

#endif

	//m_cri_Inposition.Unlock();
	return true;
}

bool AxlMotorSet::GetHomeSensor(int iUnit, int iAxis)
{//! ���� ���� Home ���� ��ȣ�� �Է� Ȯ��
	DWORD dwRet = 0, upStatus = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ


#ifdef ON_LINE_MOTOR

	//int ret = home_switch(axis);
	//return ret ? true : false;
	//! Home ��ȣ�� �Է� Ȯ��
	dwRet = AxmHomeReadSignal(iUseAxis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )	return false;
	if ( upStatus == INACTIVE )		return false;

#endif
	return true;
}

bool AxlMotorSet::GetNegaSensor(int iUnit, int iAxis)
{//! ���� ���� (-)����Ʈ ���� ��ȣ�� �Է� Ȯ��
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	DWORD dwRet = 0;
	/*DWORD StopMode, PositiveLevel, NegativeLevel*/;

	//kdu 150602
	// 	dwRet = AxmSignalGetLimit(iUseAxis, &StopMode, &PositiveLevel, &NegativeLevel);
	// 	if ( dwRet != AXT_RT_SUCCESS )		return false;
	// 	if ( NegativeLevel == UNUSED )		return false;

	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��
	DWORD upPositiveLevel, upNegativeLevel;
	dwRet = AxmSignalReadLimit(iUseAxis, &upPositiveLevel, &upNegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )		return false;
	if ( upNegativeLevel == ACTIVE )	return true;
#endif

	return false;
}


bool AxlMotorSet::GetPosiSensor(int iUnit, int iAxis)
{//! ���� ���� (+)����Ʈ ���� ��ȣ�� �Է� Ȯ��
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	DWORD dwRet = 0;
	/*DWORD StopMode, PositiveLevel, NegativeLevel*/;

	//kdu 150602
	// 	dwRet = AxmSignalGetLimit(iUseAxis, &StopMode, &PositiveLevel, &NegativeLevel);
	// 	if ( dwRet != AXT_RT_SUCCESS )		return false;
	// 	if ( PositiveLevel == UNUSED )		return false;

	//int ret = pos_switch(axis);
	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��
	DWORD upPositiveLevel, upNegativeLevel;
	dwRet = AxmSignalReadLimit(iUseAxis, &upPositiveLevel, &upNegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )		return false;
	if ( upPositiveLevel == ACTIVE )	return true;

#endif

	return false;
}

//! CRC ��ȣ ����(�޽� ����� ��� ����⿡ ������ ���)
//! Ư�� �������� ��� ������ �Ϸ�� �������� �Ǵ� ����Ʈ������ ������ ��쿡 
//! �ܺο��� CRC(Current Remaining Clear)��ȣ�� �޾� �������� ������ �ִ� �ܿ� �޽��� �����־�� �ϴ� ��찡 �߻��Ѵ�.
//! [���� ����] CRC ��ȣ ���� ����� �޽� ����� ��������(PCI-N804/404, RTEX-PM)������ ����� �� �ִ� ����̴�.
bool AxlMotorSet::ActCRC(int iUnit, int iAxis)
{
	DWORD dwReturn = 0;
	long  BoardNo = 0;
	long  ModulePos = 0;
	DWORD ModuleID = 0;
	DWORD uLevel, uMethod;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	//! ModuleID (Module�� ����)
	//! 'AXHS.h' ���� : 'C:\Program Files\EzSoftware UC\AXL(Library)\C, C++' �����ȿ� ���� (AXT_FUNC_MODULE)	
	//!      AXT_SMC_R1V04    = 0x09,   // CAMC-QI, 1 Axis, For RTEX SLAVE only	
	dwReturn = AxmInfoGetAxis(iUseAxis, &BoardNo, &ModulePos, &ModuleID);
	if ( dwReturn != AXT_RT_SUCCESS )	return false;
	if ( ModuleID != AXT_SMC_R1V04 )	return false;


	//! ���� �࿡ CRC ��ȣ ��� ���� ������ ��ȯ �Ѵ�.
	dwReturn = AxmCrcGetMaskLevel(iUseAxis, &uLevel, &uMethod);
	if ( dwReturn != AXT_RT_SUCCESS )	return false;
	//! CRC ��ȣ ������
	if ( uLevel == UNUSED )				return false;


	//! ���� �࿡ CRC ��ȣ�� ���������� ����� �߻���Ų��. (���� �ð� ���ȸ� �ۿ��Ŵ)
	AxmCrcSetOutput(iUseAxis, ENABLE);
	SleepHandle(10);
	AxmCrcSetOutput(iUseAxis, DISABLE);

	return true;
}

bool AxlMotorSet::GetStopAxis(int iUnit, int iAxis)
{//! ���� ���� ���� ��ȯ(T=����, F=������)

	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
	//! ���� ���� ���¸� Ȯ��
	DWORD Status;
	DWORD dwRet = AxmStatusReadInMotion(iUseAxis, &Status);
	if ( dwRet != AXT_RT_SUCCESS )			return false;
	if (Status == FALSE)					return true;	//! ���Ͱ� �����Ͽ����ϴ�.
#endif

	return false;
}



bool AxlMotorSet::StopAxis(int iUnit, int iAxis)
{//! ���� ����
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	AxmMoveSStop(iUseAxis);
	//Stop_Home_Search(iUnit, iAxis);

	double t_duration = 0.;
	while(!GetStopAxis(iUnit, iAxis))
	{
		DoSleepEvents();

		//t_duration = timeChk.Measure_Time(0);

		if(t_duration>1000)
		{
			sMsg.Format("[ %s ] ���� ���� �ð� �ʰ�.", MOTOR_NAME[iAxis]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

			return false;
		}

		SleepHandle(5);
	}	
#endif

	return true;
}

bool AxlMotorSet::StopAxisAll(int iUnit)
{
	bool runFlag[CUR_MOTOR_NO] = {0, };
	int iUseAxis = 0;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
	for(int i=0; i<CUR_MOTOR_NO; i++)
	{
		iUseAxis = iUnit * CUR_MOTOR_NO + i;	

		//! ���� ���� ���� ó���� ����ڰ� �Է��� ���ӵ���ŭ ��� ������ �����ϸ鼭 �����ϴ� �Լ��̴�. 
		AxmMoveSStop(iUseAxis);

		runFlag[i] = 1;
	}
#endif

	double t_duration = 0.0;
	int i = 0;
	while(1)
	{
		if(t_duration>1000)
		{
			sMsg.Format("��ü ���� ���� �ð� �ʰ�. [1 sec]");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			return false;
		}

		for(i=0; i<CUR_MOTOR_NO; i++)
		{
			if (runFlag[i])
			{
#ifdef ON_LINE_MOTOR
				if(!GetStopAxis( iUnit, i ))
#endif
				{
					runFlag[i] = 0;
					break;
				}
			}
		}

		if(i==CUR_MOTOR_NO)	break;

		DoSleepEvents();

		SleepHandle(5);
	}

	return true;
}


double AxlMotorSet::GetEncoderPos(int iUnit, int iAxis)
{//! Linear ���� ��ġ ��ǥ ��ȯ
	double pos = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
	if( MOTOR_TYPE[iAxis] == STEPING )	AxmStatusGetCmdPos(iUseAxis, &pos);	
	else								AxmStatusGetActPos(iUseAxis, &pos);
#endif

	pos /= sysData.fMotorResol[iUnit][iAxis];

	return (float)pos;
}

double AxlMotorSet::GetEncoderPos_Disp(int iUnit, int iAxis)
{//! Linear ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ
	double pos = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
	AxmStatusGetActPos(iUseAxis, &pos);
#endif

	pos /= sysData.fMotorResol[iUnit][iAxis];
	pos -= sysData.dOrgDataset[iUnit][iAxis];

	return (float)pos;
}

double AxlMotorSet::GetCommandPos(int iUnit, int iAxis)
{//! Steping ���� ��ġ ��ǥ ��ȯ
	double pos = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	AxmStatusGetCmdPos(iUseAxis, &pos);	
#endif

	pos /= sysData.fMotorResol[iUnit][iAxis];

	return (float)pos;
}

double AxlMotorSet::GetCommandPos_Disp(int iUnit, int iAxis)
{//! Steping ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ	
	double pos = 0;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR

	AxmStatusGetCmdPos(iUseAxis, &pos);	
#endif

	pos /= sysData.fMotorResol[iUnit][iAxis];
	pos -= sysData.dOrgDataset[iUnit][iAxis];

	return (float)pos;
}

int AxlMotorSet::GetEncoderPulse(int iUnit, int iAxis)
{
   double pos = 0.0;
   int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;   //���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
   if( MOTOR_TYPE[iAxis] == STEPING )   AxmStatusGetCmdPos(iUseAxis, &pos);   
   else                       			AxmStatusGetActPos(iUseAxis, &pos);
#endif

   return (int)pos;
}

int AxlMotorSet::GetCommandPulse(int iUnit, int iAxis)
{
   double pos = 0.0;
   int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;   //���� ���� ��� �� ��ȣ

#ifdef ON_LINE_MOTOR
   AxmStatusGetCmdPos(iUseAxis, &pos);   
#endif

   return (int)pos;
}


double AxlMotorSet::CalcAccFromAccTime(double dVel, double dTime)
{//! ���ӵ� = �ӵ�/(���� �ð�)���� ���
	double dRet = 0.;

	if ( fabs(dTime) < 0.001 )
	{						
		dRet = dVel / 2.;		//! ���� �ð��� �ʹ� ª�Ƽ� ������ ���� �ð��� 2 sec�� ����
	}
	else
	{
		dRet = dVel / dTime;
		dRet *= 1000.;			//! ���� ��ȯ msec -> sec
	}

	return dRet;
}



BOOL AxlMotorSet::JogMove(int iUnit, int iAxis, double dVel)
{//! ������ �ӵ��� �����Ѵ�.
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	if(GetAmpFault(iUnit, iAxis))	
		return FALSE;

	double dAcc = CalcAcc_G(iUnit,iAxis); //sysData.fMotorAcc[iUnit][iAxis];
	double dDec = CalcDec_G(iUnit,iAxis); //sysData.fMotorDec[iUnit][iAxis];

	int ret = 0;
	DWORD dwRet = 0;
#ifdef	ON_LINE_MOTOR
	dwRet = AxmMoveVel(iUseAxis, dVel, dAcc, dDec);
#endif
	ret = dwRet;

	return ret;
}

BOOL AxlMotorSet::JogStop(int iUnit, int iAxis)
{//! ������ ���� ������ ���������մϴ�.
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	AxmMoveSStop(iUseAxis);

#ifdef	ON_LINE_MOTOR
	//CTimeChecker timeChk;

	//timeChk.Init_Time();
	//timeChk.Start_Time();
	double t_duration = 0.0;

	while(!GetStopAxis(iUnit, iAxis))
	{
		DoSleepEvents();

		//t_duration = timeChk.Measure_Time(0);

		if(t_duration > 1000)
		{
			sMsg.Format("JOG �̵� ���� �ð� �ʰ�. [1 sec]");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP, (LPARAM)&sMsg);
			return false;
		}
	}
#endif

	return true;
}


//! Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
//! ���� �࿡�� ������ ��ȣ�� �����Ѵ�
//! ����Ʈ ������ȣ�� ���� ������ȣ, ���� ����̹��� Z�� ��ȣ �Ǵ� ���� �Է� ��ȣ���� Edge�� ������ �� �ִ�.
//! �ַ� �����˻� ���� �Լ� ���� �� ����ϴ� �Լ��̴�.
//! [���� ����] 
//!		1) End Limit ������ ã�� ���, ��ȣ�� Active Level ���¸� �����Ѵ�.
//!     2) Emergency Stop���� ����� ���, �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
bool AxlMotorSet::MoveAxis_To_Sensor(int axis, strMove_Input_Signal_Search *pstrInput)
{//! Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.

	//! �Է°� Ȯ��
	if ( pstrInput == NULL )	return false;

	double dVel   = pstrInput->dVel;
	double dAccel = pstrInput->dAccel;
	long lDetectSignal = -1, lSignalEdge = -1, lSignalMethod = -1;


	//! lDetectSignal ����
	if ( pstrInput->bEndLimit_Negative == true )	lDetectSignal = NegEndLimit;
	if ( pstrInput->bEndLimit_Positive == true )	lDetectSignal = PosEndLimit;
	if ( pstrInput->bHomeSensor == true )			lDetectSignal = HomeSensor;
	if ( pstrInput->bPhase_Encoder_Z == true )		lDetectSignal = EncodZPhase;

	//! lSignalEdge ����
	if ( pstrInput->bEdge_Down == true )			lSignalEdge = SIGNAL_DOWN_EDGE;
	if ( pstrInput->bEdge_Up == true )				lSignalEdge = SIGNAL_UP_EDGE;

	//! lSignalMethod ����
	if ( pstrInput->bEmergency == true )			lSignalMethod = EMERGENCY_STOP;
	if ( pstrInput->bSlowDown == true )				lSignalMethod = SLOWDOWN_STOP;


	//! �Է°� Ȯ��
	if ( lDetectSignal == NegEndLimit )		dVel = -1 * fabs(pstrInput->dVel);
	else									dVel = fabs(pstrInput->dVel);


	if ( dAccel < 0. )			return false;
	if ( lDetectSignal < 0 )	return false;
	if ( lSignalEdge < 0 )		return false;
	if ( lSignalMethod < 0 )	return false;


	DWORD dwReturn = 0;
	//! ���� �࿡�� ������ ��ȣ�� ������ ������ ���͸� �����δ�. 
	dwReturn = AxmMoveSignalSearch(axis, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
	if ( dwReturn != AXT_RT_SUCCESS )	return false;

	return true;
}


////! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
////! GUI ȭ�鿡�� ���� ���Ǵ� ����̶� ���� �Լ��� �����. 
//bool AxlMotorSet::Move_From_GUI(int iUnit, int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus)
//{//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
//	if ( pParentWnd == NULL )			return false;
//	if ( bMotorMoving[iUnit][axis] == true )	return false;
//	bMotorMoving[iUnit][axis] = true;
//
//	CString sGetTemp = _T("");
//	GetString_FromUI(iID_GUI, pParentWnd, sGetTemp);
//
//	double dGetTemp = atof(sGetTemp);
//	if ( dGetTemp > 3. )		dGetTemp = 3.;	
//	else if (dGetTemp < -3 )	dGetTemp = -3;
//
//	if ( bPlus == false )	dGetTemp *= (-1.);
//
//	MoveAxis(iUnit, axis, REL, dGetTemp, 4);
//	bMotorMoving[iUnit][axis] = false;
//
//	return true;
//}

bool AxlMotorSet::Move_From_AbsRel(int iUnit, int iAxis, double dVal, bool bPlus)
{
	if ( bMotorMoving[iUnit][iAxis] == true )	return false;

	bMotorMoving[iUnit][iAxis] = true;
	if( bPlus == false )	dVal *= (-1.);

	if( Task[iUnit].AutoFlag == 1 )		MoveAxis(iUnit, iAxis, REL, dVal, sysData.fMotorSpeed_Auto[iUnit][iAxis]);
	else								MoveAxis(iUnit, iAxis, REL, dVal, sysData.fMotorSpeed[iUnit][iAxis]);



	double dStartTime;
	dStartTime = ::func_TimeChecker_CurrentTime(true);

	while (1)
	{
		if(GetStopAxis(iUnit, iAxis))
		{
			break;
		}
		else if( ::func_TimeChecker_Compare(10000, dStartTime) == true )
		{
			CString sMsg="";
			//sMsg.Format("[ ERROR ] %s�� �̵� �ð� �ʰ�..", MOTOR_NAME[iAxis]);
			//::SendMessage(hMainDlg, WM_SEND_POPMSG, DEF_TYPE_NG, (LPARAM)&sMsg);
			break;
		}
		SleepHandle(SleepTimeDelay);
	}

	bMotorMoving[iUnit][iAxis] = false;
	return true;
}

bool AxlMotorSet::MoveAxis(int iUnit, int iAxis, bool AbSFlag, double dPos, double dVel)
{//! ���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
	// abs 1:absolute, 0:increment 

	int iUseAxis = 0;
	iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;

	if(GetAmpFault(iUnit, iAxis))
	{
		m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;
		sMsg.Format(" AMP Alarm �߻�. MoveAxis ���� ����.");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	if(!GetAmpEnable(iUnit, iAxis))
	{
		m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_NOT;

		sMsg.Format("[ %s ] ���� Servo Off ���� �Դϴ�. MoveAxis ���� ����.", MOTOR_NAME[iAxis]);
		//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_LOG_ALARM_NG_MGL, (LPARAM)&sMsg);
		//DoModal �� ����
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	if(AbSFlag == ABS)
	{
		double curPos;

		if( MOTOR_TYPE[iAxis] == STEPING )	curPos = GetCommandPos(iUnit, iAxis);
		else								curPos = GetEncoderPos(iUnit, iAxis);
		if(fabs(curPos - dPos) < 0.0001)	return true;

		dPos *= sysData.fMotorResol[iUnit][iAxis];

		//if(m_bOrgFlag[iUnit][iAxis]==DEF_ORIGIN_NOT)
		//{
		//	char	motorMSG[256];
		//	wsprintf(motorMSG, "��  ���� ���� ���͸� ���� ���� �Ͻʽÿ�. [ %s �� ]", MOTOR_NAME[iAxis]);
		//	return false;
		//}
	}
	else if(AbSFlag==REL)
	{
      int iGetPos;
      if( MOTOR_TYPE[iAxis] == STEPING)   iGetPos = GetCommandPulse(iUnit, iAxis);//mm
      else								  iGetPos = GetEncoderPulse(iUnit, iAxis);

      dPos = (int)(dPos * sysData.fMotorResol[iUnit][iAxis]);
      dPos += iGetPos;
    }
	//else if(AbSFlag==REL)
	//{
	//	if( MOTOR_TYPE[iAxis] == STEPING)	dPos += GetCommandPos(iUnit, iAxis);
	//	else								dPos += GetEncoderPos(iUnit, iAxis);	
	//}
	else
	{
		sMsg.Format("[ %s ] ���� �̵� ��� ������.", MOTOR_NAME[iAxis]);
		//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_LOG_ALARM_NG_MGL, (LPARAM)&sMsg);
		//DoModal �� ����
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	
	//if(dPos>0)			dPos = (int)(dPos+0.5);

#ifdef ON_LINE_MOTOR
	long   axisAll[CUR_MOTOR_NO] = {0, };
	double posAll[CUR_MOTOR_NO] = {0, };
	double velAll[CUR_MOTOR_NO] = {0, };
	double accAll[CUR_MOTOR_NO] = {0, };
	double decAll[CUR_MOTOR_NO] = {0, };
	axisAll[0] = iUseAxis;
	posAll[0] = dPos;
	velAll[0] = dVel * sysData.fMotorResol[iUnit][iAxis];	//�̵� �ӵ�
	accAll[0] = CalcAcc_G(iUnit,iAxis);//sysData.fMotorAcc[iUnit][iAxis];		//! ���� ���ӵ�
	decAll[0] = CalcDec_G(iUnit,iAxis); //sysData.fMotorDec[iUnit][iAxis];

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.

	//AxmMoveStartMultiPos(1, axisAll, posAll, velAll, accAll, decAll);
	AxmMoveStartPos(axisAll[0], posAll[0], velAll[0], accAll[0], decAll[0]);
#endif
	//SleepHandle(5);
	return true;
}

bool AxlMotorSet::MoveAxisMulti(int iUnit, int iAxisCnt, short iAxis[CUR_MOTOR_NO], double pos[CUR_MOTOR_NO])
{//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�
	long   axis2[CUR_MOTOR_NO] = {0, };
	double pos2[CUR_MOTOR_NO] = {0, };
	double vel[CUR_MOTOR_NO] = {0, };
	double acc[CUR_MOTOR_NO] = {0, };
	double dec[CUR_MOTOR_NO] = {0, };

	int iCnt =0;
	double curPos;


	for(int i=0; i<iAxisCnt; i++)
	{
		double dEncordChk = 0.0;

		if(!GetStopAxis(iUnit, iAxis[i]))
		{
			SleepHandle(200);
			if(!GetStopAxis(iUnit, iAxis[i]))
			{
				//sMsg.Format("[ %s ] ���� �̵� �� �̵� ����� ȣ�� �Ǿ����ϴ�. [%d],[%d],[%d]", MOTOR_NAME[iAxis[i]], flow[iUnit].iAAStep, flow[iUnit].iLENS_Step, flow[iUnit].iLENS_LoadingStep);
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);
				return false;
			}
		}

		if(MOTOR_TYPE[ iAxis[i] ] == STEPING)	curPos = GetCommandPos(iUnit, iAxis[i]);
		else									curPos = GetEncoderPos(iUnit, iAxis[i]); 


		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		axis2[iCnt] = iAxis[i] + (iUnit * CUR_MOTOR_NO);
		pos2[iCnt] = pos[i] * sysData.fMotorResol[iUnit][iAxis[i]];
		dEncordChk = pos[i] * sysData.fMotorResol[iUnit][iAxis[i]];

		if(Task[iUnit].AutoFlag == 1)	vel[iCnt] = sysData.fMotorSpeed_Auto[iUnit][iAxis[i]] * sysData.fMotorResol[iUnit][iAxis[i]];
		else							vel[iCnt] = sysData.fMotorSpeed[iUnit][iAxis[i]] * sysData.fMotorResol[iUnit][iAxis[i]];

		acc[iCnt] = CalcAcc_G(iUnit,iAxis[i]);// sysData.fMotorAcc[iUnit][iAxis[i]]; //! ���� ���ӵ�
		dec[iCnt] = CalcDec_G(iUnit,iAxis[i]); //sysData.fMotorDec[iUnit][iAxis[i]];

		//if( fabs(dEncordChk-curPos) < 0.005) 			// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
		if( fabs(dEncordChk-curPos) < 0.001) 			// �̵��� ��ġ���� 1um �̻� ���̰� �������� �������� ����.	jjc 2015 12 29 �����Ҷ� ����
			continue;
		else
			iCnt++;
	}

	if(iCnt == 0)
		return true;
	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
#ifdef ON_LINE_MOTOR
	AxmMoveStartMultiPos(iCnt, axis2, pos2, vel, acc, dec);
#endif

	return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MOTOR HOME ����

bool AxlMotorSet::GetResult_Home_Search(int iUnit, int iAxis, CString &sResult)
{//! Home �˻� �۾� ��� ���ڿ��� �˾Ƴ���. 
	DWORD uHomeResult;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	AxmHomeGetResult(iUseAxis, &uHomeResult);

	bool bRet = false;

	switch(uHomeResult)
	{
	case HOME_SUCCESS:
		{
			sResult = _T("���� �˻��� ���������� ���� ���� ���");
			bRet = true;
		}
		break;
	case HOME_SEARCHING:
		{
			sResult = _T("���� �����˻��� �������� ���");
		}
		break;
	case HOME_ERR_GNT_RANGE:
		{
			sResult = _T("��Ʈ�� �������� Master��� Slave���� �����˻� ����� ������ OffsetRange�� ��������");
		}
		break;
	case HOME_ERR_USER_BREAK:
		{
			sResult = _T("���� �˻��� ����ڰ� ���� ����� �������� ���");
		}
		break;
	case HOME_ERR_VELOCITY:
		{
			sResult = _T("���� �˻��� �ӵ� ���� �� �� �� ���� 0���� �۰ų� �������");
		}
		break;
	case HOME_ERR_AMP_FAULT:
		{
			sResult = _T("���� �˻��� ������ �˶��� �߻��� ���");
		}
		break;
	case HOME_ERR_NEG_LIMIT:
		{
			sResult = _T("(+)�������� �������� ���� �� (-)����Ʈ ������ ������ ���");
		}
		break;
	case HOME_ERR_POS_LIMIT:
		{
			sResult = _T("(-)�������� �������� ���� �� (+)����Ʈ ������ ������ ���");
		}
		break;
	case HOME_ERR_NOT_DETECT:
		{
			sResult = _T("���������� �������� �������");
		}
		break;
	case HOME_ERR_UNKNOWN:
		{
			sResult = _T("�˼����� ä��(��) ��ȣ(0 ~ (�ִ���� - 1))�� �����˻��� �����Ϸ� �������");
		}
		break;
	default:
		{
			sResult = _T("Error");
		}
		break;
	}

	return bRet;
}

int AxlMotorSet::IsCompleted_Home_Search(int iUnit, int iAxis)
{//! Home�˻� ���� Ȯ��(0=Fail, 1=Success, 2=Searching)
	DWORD dwReturn = 0;
	DWORD uHomeResult;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	AxmHomeGetResult(iUseAxis, &uHomeResult);

	//! ���� �����˻��� �������� ���
	if ( uHomeResult == HOME_SEARCHING )		return 2;
	if ( uHomeResult == HOME_SUCCESS )			return 1; 
	if ( uHomeResult == HOME_ERR_AMP_FAULT)		return -1;
	if ( uHomeResult == HOME_ERR_NOT_DETECT)	return -2;
	if ( uHomeResult == HOME_ERR_NEG_LIMIT)		return -3;
	if ( uHomeResult == HOME_ERR_POS_LIMIT)		return -4;
	if ( uHomeResult == HOME_ERR_UNKNOWN)		return -5;

	return 0;
}

int AxlMotorSet::GetRate_Home_Search(int iUnit, int iAxis)
{//! Home �˻� �۾� ���� ���� ����, ���� %�� ��ȯ�ȴ�. 
	DWORD dwReturn = 0;
	DWORD uHomeStepNumber,uHomeMainStepNumber;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	//! �����˻� ���۵Ǹ� �������� Ȯ���� �� �ִ�. �����˻��� �Ϸ�Ǹ� �������ο� ������� 100�� ��ȯ�ϰ� �ȴ�.
	//! uHomeMainStepNumber�� ��Ʈ���� ��� ������,�����̺갡 �����ϴ��� Ȯ���� �� �̿��Ѵ�.
	//!  (�������� ������ ���� 0�� ����, �����̺굿���� ���� 10�� �����Ѵ�.)
	dwReturn = AxmHomeGetRate(iUseAxis, &uHomeMainStepNumber, &uHomeStepNumber);

	//! �����˻� ������(����: %)
	return uHomeStepNumber;
}

void AxlMotorSet::Stop_Home_Search(int iUnit, int iAxis)
{//! Home �˻� �߿� ������ ���� ��Ŵ
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	AxmHomeSetResult(iUseAxis, HOME_ERR_USER_BREAK);
	StopAxis(iUnit, iUseAxis);
}

int AxlMotorSet::Start_Home_Search(int iUnit, int iAxis)
{//! Home �˻� �۾� ����
	m_criHomeProc.Lock();

	DWORD dwReturn = 0;
	DWORD level;
	int iUseAxis = iUnit * CUR_MOTOR_NO + iAxis;	//���� ���� ��� �� ��ȣ

	AxmSignalIsServoOn(iUseAxis, &level);
	if ( level != ENABLE )
	{
		AxmSignalServoOn(iUseAxis, ENABLE);	

		for(int i=0; i<20; i++)
		{
			SleepHandle(5);
			CheckHandle();
		}

		AxmSignalIsServoOn(iUseAxis, &level);
		if ( level != ENABLE )
		{
			sMsg.Format("[ %s ] ���� ���� �˻� ���� ON ����.!!", MOTOR_NAME[iAxis]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  +DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  +DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			m_criHomeProc.Unlock();
			return -3;
		}
	}


	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++
	//!    DIR_CCW    00h     (-) ����
	//!    DIR_CW     01h     (+) ����
	//! [lHmsig]
	//!    PosEndLimit  00h   +Elm(End limit) +���� ����Ʈ ���� ��ȣ
	//!    NegEndLimit  01h   -Elm(End limit) -���� ����Ʈ ���� ��ȣ
	//!    HomeSensor   04h   IN0(ORG)  ���� ���� ��ȣ
	//! [dwZphas]
	//!    DISABLE      00h   Z�� ���� ������� ����
	//!    + ����       01h   Z�� ���� +������
	//!    - ����       02h   Z�� ���� -������

	long    lHmDir = DIR_CW;			//! �����˻��� �ʱ⿡ ������ ���⼳��
	DWORD   lHmsig =  HomeSensor;		//! ���� �˻��� ��� �� ��ȣ����
	DWORD   dwZphas = DISABLE;			//! 1�� �����˻� �Ϸ� �� ���ڴ� Z�� ���� ���� ����
	double  dwHClrTim = 2000.0;			//! �����˻� �Ϸ� �� ������ġ�� Encoder��ġ�� Clear�ϱ� ���� ����ϴ� �ð����� [mSec����]
	double  dwHOffset = 0.0;			//! �����˻��� �Ϸ�� �� �ⱸ �������� �̵� �� ���� �缳�� �� ��ġ

	lHmDir = MOTOR_HOME_DIR[iAxis];
	lHmsig = MOTOR_HOME_SIG[iAxis];

	//if (iAxis == MotPCB_Y)			// 20160318 
	//{
	//	lHmDir = DIR_CCW;
	//}

	dwReturn = AxmHomeSetMethod(iUseAxis, lHmDir, lHmsig, dwZphas, dwHClrTim, dwHOffset);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		sMsg.Format("[ %s ] ���� ���� �˻� ���� ����.!!(code : %d)", MOTOR_NAME[iAxis], (int)(dwReturn));
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		m_criHomeProc.Unlock();
		return -4;
	}

	dwReturn = SetHomeSearchSpeed(iUseAxis);	//���� �˻� �ӵ� ����
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		sMsg.Format("[ %s ] ���� ���� �ӵ� ���� ����.!!", MOTOR_NAME[iAxis]);
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		m_criHomeProc.Unlock();
		return -6;
	}

	dwReturn = AxmHomeSetStart(iUseAxis);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		//! Error Code Ȯ��
		//kdu Ȯ�ο�.
		CString sError = _T("");
		GetResult_Home_Search(iUnit, iAxis, sError);
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		m_criHomeProc.Unlock();
		return -7;
	}

	m_criHomeProc.Unlock();
	return 1;
}

DWORD AxlMotorSet::SetHomeSearchSpeed(int iUseAxis)
{//! ���� �˻��� ���Ǵ� �ӵ� ����
	/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ������ �����ϰ� �˻��ϱ� ���� ���� �ܰ��� �������� �����Ѵ�. �̶� �� ���ǿ� ��� �� �ӵ��� �����Ѵ�. 
	// �� �ӵ����� �������� ���� �����˻� �ð���, �����˻� ���е��� �����ȴ�. 
	// �� ���Ǻ� �ӵ����� ������ �ٲ㰡�鼭 �� ���� �����˻� �ӵ��� �����ϸ� �ȴ�. 
	// (�ڼ��� ������ AxmMotSaveParaAll ���� �κ� ����)
	// �����˻��� ���� �ӵ��� �����ϴ� �Լ�
	// [dVelFirst]- 1�������ӵ�   [dVelSecond]-�����ļӵ�   [dVelThird]- ������ �ӵ�  [dvelLast]- index�˻��� �����ϰ� �˻��ϱ�����. 
	// [dAccFirst]- 1���������ӵ� [dAccSecond]-�����İ��ӵ� 
	*/
	DWORD dwReturn = 0;
	double dVelFirst = 20000.;		
	double dVelSecond = 10000.;
	double dVelThird = 4000.;
	double dvelLast = 500.;	

	double dAccFirst = 0.1;		
	double dAccSecond = 0.1;

	//ISJ
	int iAxis = iUseAxis%CUR_MOTOR_NO;
	int iUnit = iUseAxis/CUR_MOTOR_NO;


	switch(iAxis)
	{
// 	case MotPCB_X:
// 	dVelFirst = 4000.;		dVelSecond = 1000.;	dVelThird = 500.;		dvelLast = 50.;
// 	break;
	case MotPCB_Y:
	dVelFirst = 50000;		dVelSecond = 20000;	dVelThird = 300;		dvelLast = 50;	
	break;
	case MotPCB_T:
	dVelFirst = 14000.;		dVelSecond = 7000.;	dVelThird = 300;		dvelLast = 50.;	
	break;
	case MotLENS_X:
	dVelFirst = 14000.;		dVelSecond = 10001;	dVelThird = 300;		dvelLast = 50.;	

	break;
	case MotLENS_Y:
	dVelFirst = 50000;		dVelSecond = 20000;	dVelThird = 300;		dvelLast = 50;	
	break;
	case MotLENS_Z:
	dVelFirst = 20000;		dVelSecond = 4000;	dVelThird = 300;		dvelLast = 50.;	
	break;
	case MotLENS_Tx:
	dVelFirst = 1000.;		dVelSecond = 500.;	dVelThird = 300;		dvelLast = 50.;	
	break;
	case MotLENS_Ty:
	dVelFirst = 200;		dVelSecond = 100.;	dVelThird = 40.;		dvelLast = 15.;	
	break;
	case MotCL_Y:
	dVelFirst = 50000;		dVelSecond = 20000;	dVelThird = 300;		dvelLast = 50;	
	break;
	default://��ȿ���� �ʴ� ���� ��ȣ
	dwReturn = AXT_RT_INVALID_MODULE_POS;
	break;	
	}


	if(dwReturn == 0)  
	{
	dwReturn = AxmHomeSetVel(iUseAxis, dVelFirst, dVelSecond, dVelThird, dvelLast, CalcAcc_G(iUnit,iAxis), CalcAcc_G(iUnit,iAxis));
	//dwReturn = AxmHomeSetVel(iUseAxis, dVelFirst, dVelSecond, dVelThird, dvelLast, dVelFirst, dVelSecond);
	}

	return dwReturn;
}

bool AxlMotorSet::HomeReadyProcess(int iUnit, int iType)
{
	CString sMsg;
	DWORD dwThreadID, dwExitCode = 0;
	int	nCurIdx=0;
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	GetExitCodeThread(m_hThreadAutoFocus[iUnit], &dwExitCode);
	if( dwExitCode == STILL_ACTIVE )
	{
		sMsg.Format(_T("AUTO MODE ���� PROCESS�� �̹� ���� ���Դϴ�.."));
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
		//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP,(LPARAM)&sMsg);
		return false;
	}
	if( (Motor.m_hThreadHomeAll[iUnit] != NULL) ||  ( Motor.m_hThreadHome[iUnit] != NULL ) )
	{
		sMsg.Format("���� �����߿� �Ұ����� �����Դϴ�. ");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	long  AxisCount;
	DWORD dwRet = 0;
	dwRet = AxmInfoGetAxisCount(&AxisCount);
	if ( dwRet != AXT_RT_SUCCESS )
	{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�
		sMsg.Format("[ ERROR ] AXT Board �ν� ����.. ���� �˻� ������ ���� �մϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}
	if ( AxisCount <= 0 )
	{
		sMsg.Format("[ ERROR ] ���� ����̺� ���� �̻�.. ���� �˻� ������ ���� �մϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}


	for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
	{
		if( !GetStopAxis(iUnit, iAxis) )
		{
			sMsg.Format("[ %s ] ���� ���� �� �Դϴ�.\n ���� ���� �� ���� ���� �ϼ���.", MOTOR_NAME[iAxis]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}

		if( GetAmpFault(iUnit, iAxis) )
		{//AmpFaultReset => MESSAGEâ���� �ڵ����� �˶� ���� �ص� ��. �ϴ� �˶��� �߻�.
			sMsg.Format("[ %s ] ���� �˶� �߻� ���Դϴ�.\n �˶� ���� �� ���� ���� �ϼ���.", MOTOR_NAME[iAxis]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	} 

	if( Task[iUnit].AutoFlag == 1)
	{
		sMsg.Format("�ڵ������߿� �Ұ����� �����Դϴ�. " );
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
		//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		return false;
	}

	if( !AmpEnableAll(iUnit) )
	{
		Sleep(200);
		if( !AmpEnableAll(iUnit) )
		{
			sMsg.Format(" ���� ��ü ���� ON ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	}

	
	iHomeUseStep = 1000;
	if(iType == 0)		//����
	{
		for(int i=0; i<CUR_MOTOR_NO; i++)
			Motor.iHomeProcUsing[iUnit][i] = 1;

		iHomeEndStep = 10000;

		if( iUnit == UNIT1 )
		{
			m_hThreadHomeAll[UNIT1] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_HomeReadyProcess1, 
				this, 0, &dwThreadID);
		}
		else if( iUnit == UNIT2 )
		{
			m_hThreadHomeAll[UNIT2] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_HomeReadyProcess2, 
				this, 0, &dwThreadID);
		}
	}
	else if(iType == 1)		//���� �غ�
	{
 		bool OrgRes = true; 
 		for(int Axis=0; Axis<(CUR_MOTOR_NO); Axis++)
 		{
 			if( Motor.m_bOrgFlag[iUnit][Axis] != DEF_ORIGIN_COMPLETE )
 				OrgRes = false;
 		}

 		if(OrgRes)		//������ ������ ���
		{//���� �غ�� �̵�
			iHomeUseStep = 10000;
			iHomeEndStep = 25000;

			if( iUnit == UNIT1 )
			{
				m_hThreadReady[UNIT1] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_ReadyProcess1, 
					this, 0, &dwThreadID);
			}
			else if( iUnit == UNIT2 )
			{
				m_hThreadReady[UNIT2] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_ReadyProcess2, 
					this, 0, &dwThreadID);
			}

		}
		else		//������ �� ������ ���
		{
			sMsg.Format("�����غ��� ������ �������� �ʽ��ϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	}

// 	if( iUnit == UNIT1 )
// 	{
// 		m_hThreadHomeAll[UNIT1] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_HomeReadyProcess1, 
// 			this, 0, &dwThreadID);
// 	}
// 	else if( iUnit == UNIT2 )
// 	{
// 		m_hThreadHomeAll[UNIT2] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_HomeReadyProcess2, 
// 			this, 0, &dwThreadID);
// 	}

	return true;
}

DWORD Thread_HomeReadyProcess1(AxlMotorSet* pAxlMotorSet)	
{// [ iUnit ] ��ü ���� ����
	int iUnit = UNIT1;

	CString sMsg, str;
	int iThreadStep = 0, iThreadEndStep = 0, iHomFlag = 0;
	DWORD dwRet = 0;
	long AxisCount = 0;

//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC

	iThreadStep = pAxlMotorSet->iHomeUseStep;
	iThreadEndStep = pAxlMotorSet->iHomeEndStep;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	//�����غ� Ǯ��
	Task[UNIT1].bRunningReady = false;

	pAxlMotorSet->bThreadHomeAll[iUnit] = true;

	while(pAxlMotorSet->bThreadHomeAll[iUnit])
	{
		Sleep(5);

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ][ ���� �˻� ] AXT Board �ν� ����.. ���� �˻� ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

			pAxlMotorSet->bThreadHomeAll[iUnit] = false;
			break;
		}

		bool bAlarmLimit = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			if(pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] == 2)
			{
				if( pAxlMotorSet->GetAmpFault(iUnit, iAxis) == true ){
					sMsg.Format("[ ��ü����] ���� �˻� �� AMP �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadHomeAll[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

				if( pAxlMotorSet->GetAmpEnable(iUnit, iAxis) == false ){
					sMsg.Format("[ ��ü����] ���� �˻� �� Servo Off �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadHomeAll[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

			}
		}
		if( bAlarmLimit == false )	break;

		
		str.Format("[ STEP : %d ]", iThreadStep);
		//pFrame->m_btn_title1.CallFlashFunction("Version", "%s", str);
		//sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iThreadStep);			//20160828 JJC
		pFrame->stcVersion[iUnit].SetWindowTextA(str);

		if(iThreadStep >= 1000 && iThreadStep <10000)		iThreadStep = pAxlMotorSet->HomeAllProcess_UNIT1(iThreadStep);		//��ü����
//		else if(iThreadStep >= 10000 && iThreadStep < 25000)	iThreadStep = pAxlMotorSet->ReadyAllProcess_UNIT1(iThreadStep);		//���� �غ�

		if(iThreadStep < 0)
		{//���� �߻� ���� ����
			for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				iHomFlag = pAxlMotorSet->IsCompleted_Home_Search(iUnit, iAxis);
				if( iHomFlag == 2 )
				{
					pAxlMotorSet->Stop_Home_Search(iUnit, iAxis);
					pAxlMotorSet->m_bOrgFlag[iUnit][iAxis] = false;
				}
			}
			break;
		}

		if( iThreadStep == 99999  ||  iThreadStep >= iThreadEndStep)
		{	//KYH ��ü�� Ȩ�˻� �� �����غ� �Ϸ� Message �߰�
			//KYH Progressâ ����.
			break;
		}


	}//while

	for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] = 0;	//��ü ���� ��û �ʱ�ȭ

	pAxlMotorSet->m_hThreadHomeAll[iUnit] = NULL;
	pAxlMotorSet->bThreadHomeAll[iUnit] = false;

	return true;
}

DWORD Thread_HomeReadyProcess2(AxlMotorSet* pAxlMotorSet)	
{// [ iUnit ] ��ü ���� ����
	int iUnit = UNIT2;

	CString sMsg, str;
	int iThreadStep = 0, iThreadEndStep = 0, iHomFlag = 0;
	DWORD dwRet = 0;
	long AxisCount = 0;

//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC

	iThreadStep = pAxlMotorSet->iHomeUseStep;
	iThreadEndStep = pAxlMotorSet->iHomeEndStep;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;


	//�����غ� Ǯ��
	Task[UNIT2].bRunningReady = false;

	pAxlMotorSet->bThreadHomeAll[iUnit] = true;

	while(pAxlMotorSet->bThreadHomeAll[iUnit])
	{
		Sleep(5);

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ][ ���� �˻� ] AXT Board �ν� ����.. ���� �˻� ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

			pAxlMotorSet->bThreadHomeAll[iUnit] = false;
			break;
		}

		bool bAlarmLimit = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			if(pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] == 2)
			{
				if( pAxlMotorSet->GetAmpFault(iUnit, iAxis) == true ){
					sMsg.Format("[ ��ü����] ���� �˻� �� AMP �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadHomeAll[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

				if( pAxlMotorSet->GetAmpEnable(iUnit, iAxis) == false ){
					sMsg.Format("[ ��ü����] ���� �˻� �� Servo Off �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadHomeAll[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

			}
		}
		if( bAlarmLimit == false )	break;

		str.Format("[ STEP : %d ]", iThreadStep);
	//	pFrame->m_btn_title2.CallFlashFunction("Version", "%s", str);
		//sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iThreadStep);			//20160828 JJC
		pFrame->stcVersion[iUnit].SetWindowTextA(str);


		if(iThreadStep >= 1000 && iThreadStep <10000)		iThreadStep = pAxlMotorSet->HomeAllProcess_UNIT2(iThreadStep);		//��ü����
	//	else if(iThreadStep >= 10000 && iThreadStep < 25000)	iThreadStep = pAxlMotorSet->ReadyAllProcess_UNIT2(iThreadStep);		//���� �غ�

		if(iThreadStep < 0)
		{//���� �߻� ���� ����
			for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				iHomFlag = pAxlMotorSet->IsCompleted_Home_Search(iUnit, iAxis);
				if( iHomFlag == 2 )
				{
					pAxlMotorSet->Stop_Home_Search(iUnit, iAxis);
					pAxlMotorSet->m_bOrgFlag[iUnit][iAxis] = false;
				}
			}
			break;
		}

		if( iThreadStep == 99999  ||  iThreadStep >= iThreadEndStep)
		{	//KYH ��ü�� Ȩ�˻� �� �����غ� �Ϸ� Message �߰�
			//KYH Progressâ ����.
			break;
		}


	}//while





	for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] = 0;	//��ü ���� ��û �ʱ�ȭ

	pAxlMotorSet->m_hThreadHomeAll[iUnit] = NULL;
	pAxlMotorSet->bThreadHomeAll[iUnit] = false;

	return true;
}
//----------------------------------------------------------------------------------�� �� �� ��
DWORD Thread_ReadyProcess1(AxlMotorSet* pAxlMotorSet)	
{// [ iUnit ] ���� �غ� ����
	int iUnit = UNIT1;

	CString sMsg, str;
	int iThreadStep = 0, iThreadEndStep = 0, iHomFlag = 0;
	DWORD dwRet = 0;
	long AxisCount = 0;

//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC

	iThreadStep = pAxlMotorSet->iHomeUseStep;
	iThreadEndStep = pAxlMotorSet->iHomeEndStep;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;


	pAxlMotorSet->bThreadReady[iUnit] = true;

	while(pAxlMotorSet->bThreadReady[iUnit])
	{
		Sleep(5);

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ][ ���� �˻� ] AXT Board �ν� ����.. ���� �˻� ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

			pAxlMotorSet->bThreadReady[iUnit] = false;
			break;
		}

		bool bAlarmLimit = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			if(pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] == 2)
			{
				if( pAxlMotorSet->GetAmpFault(iUnit, iAxis) == true ){
					sMsg.Format("[ ��ü����] ���� �˻� �� AMP �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadReady[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

				if( pAxlMotorSet->GetAmpEnable(iUnit, iAxis) == false ){
					sMsg.Format("[ ��ü����] ���� �˻� �� Servo Off �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadReady[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

			}
		}
		if( bAlarmLimit == false )	break;


		str.Format("[ STEP : %d ]", iThreadStep);
		//pFrame->m_btn_title1.CallFlashFunction("Version", "%s", str);

		//sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iThreadStep);			//20160828 JJC
		pFrame->stcVersion[iUnit].SetWindowTextA(str);

		if(iThreadStep >= 10000 && iThreadStep < 27000/*25000*/)	iThreadStep = pAxlMotorSet->ReadyAllProcess_UNIT1(iThreadStep);		//���� �غ�

		if(iThreadStep < 0)
		{//���� �߻� ���� ����
			for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				AxmMoveEStop(iAxis + (0 * CUR_MOTOR_NO));
			}
			break;
		}

		if( iThreadStep == 99999  ||  iThreadStep >= iThreadEndStep)
		{	//KYH ��ü�� Ȩ�˻� �� �����غ� �Ϸ� Message �߰�
			//KYH Progressâ ����.
			break;
		}


	}//while	 

	pAxlMotorSet->m_hThreadReady[iUnit] = NULL;
	pAxlMotorSet->bThreadReady[iUnit] = false;

	return true;
}

DWORD Thread_ReadyProcess2(AxlMotorSet* pAxlMotorSet)	
{// [ iUnit ] ��ü ���� ����
	int iUnit = UNIT2;

	CString sMsg, str;
	int iThreadStep = 0, iThreadEndStep = 0, iHomFlag = 0;
	DWORD dwRet = 0;
	long AxisCount = 0;

//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC

	iThreadStep = pAxlMotorSet->iHomeUseStep;
	iThreadEndStep = pAxlMotorSet->iHomeEndStep;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;


	pAxlMotorSet->bThreadReady[iUnit] = true;

	while(pAxlMotorSet->bThreadReady[iUnit])
	{
		Sleep(5);

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ][ ���� �˻� ] AXT Board �ν� ����.. ���� �˻� ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

			pAxlMotorSet->bThreadReady[iUnit] = false;
			break;
		}

		bool bAlarmLimit = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			if(pAxlMotorSet->iHomeProcUsing[iUnit][iAxis] == 2)
			{
				if( pAxlMotorSet->GetAmpFault(iUnit, iAxis) == true ){
					sMsg.Format("[ ��ü����] ���� �˻� �� AMP �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadReady[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

				if( pAxlMotorSet->GetAmpEnable(iUnit, iAxis) == false ){
					sMsg.Format("[ ��ü����] ���� �˻� �� Servo Off �˶� �߻�.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

					pAxlMotorSet->bThreadReady[iUnit] = false;
					bAlarmLimit = false;
					break;
				}

			}
		}
		if( bAlarmLimit == false )	break;


		str.Format("[ STEP : %d ]", iThreadStep);
	//	pFrame->m_btn_title2.CallFlashFunction("Version", "%s", str);
		//sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iThreadStep);			//20160828 JJC
		pFrame->stcVersion[iUnit].SetWindowTextA(str);

		//	if(iThreadStep >= 1000 && iThreadStep <10000)		iThreadStep = pAxlMotorSet->HomeAllProcess_UNIT1(iThreadStep);		//��ü����
		//	else 
		if(iThreadStep >= 10000 && iThreadStep < 25000)	iThreadStep = pAxlMotorSet->ReadyAllProcess_UNIT2(iThreadStep);		//���� �غ�
		//if(iThreadStep >= 10000 && iThreadStep < 27000)	iThreadStep = pAxlMotorSet->ReadyAllProcess_UNIT2(iThreadStep);		//���� �غ�

		if(iThreadStep < 0)
		{//���� �߻� ���� ����
			for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				AxmMoveEStop(iAxis + (1 * CUR_MOTOR_NO));
			}
			break;
		}

		if( iThreadStep == 99999  ||  iThreadStep >= iThreadEndStep)
		{	//KYH ��ü�� Ȩ�˻� �� �����غ� �Ϸ� Message �߰�
			//KYH Progressâ ����.
			break;
		}


	}//while

	pAxlMotorSet->m_hThreadReady[iUnit] = NULL;
	pAxlMotorSet->bThreadReady[iUnit] = false;

	return true;
}



//----------------------------------------------------------------------------------------------------------


//-----------------���� ����-----------------------------------------------
bool AxlMotorSet::HomeSingleProcess(int iUnit, int iAxis)
{
	CString sMsg;
	DWORD dwThreadID=0, dwExitCode=0;
	
	GetExitCodeThread(m_hThreadHome[iUnit], &dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
		sMsg.Format(_T("[ �������� ]���� ���� ���� PROCESS�� �̹� ���� ���Դϴ�.."));
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	for(int iAxiss = 0; iAxiss < CUR_MOTOR_NO; iAxiss++)
	{
		if( !GetStopAxis(iUnit, iAxiss) )
		{
			sMsg.Format("[ %s ] ���� ���� �� �Դϴ�.\n ���� ���� �� ���� ���� �ϼ���.", MOTOR_NAME[iAxiss]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
		m_bOrgFlag[iUnit][iAxiss] = DEF_ORIGIN_NOT;

		if( GetAmpFault(iUnit, iAxiss) )
		{
			sMsg.Format("[ %s ] ���� �˶� �߻� ���Դϴ�.\n �˶� ���� �� ���� ���� �ϼ���.", MOTOR_NAME[iAxiss]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	}

	//20151026 ksg
	if( Task[UNIT1].AutoFlag == 1) 
	{
		sMsg.Format("�ڵ������߿� �Ұ����� �����Դϴ�. " );
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
		//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		return false;
	}

	if( !AmpEnableAll(iUnit) )
	{
		Sleep(200);
		if( !AmpEnableAll(iUnit) )
		{
			sMsg.Format(" ���� ��ü ���� ON ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	}


//	iHomeProcUsing[iUnit][iAxis] = 1;

	if( iUnit == UNIT1 )
	{
		m_hThreadHome[UNIT1] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadHomeProcessAA1, 
			this, 0, &dwThreadID);
	}
	else if( iUnit == UNIT2 )
	{
		m_hThreadHome[UNIT2] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadHomeProcessAA2, 
			this, 0, &dwThreadID);
	}
	return true;
}

int AxlMotorSet::HomeSingleProcess_UNIT1(int iUseStep)
{// [ UNIT1 ] ���� ���� ����
	int iUnit = UNIT1;
	CString sMsg;
	int iThreadStep = 0, iHomFlag = 0;
	double dVel,dAcc;
	double dFTime = 0;
	DWORD dwReturn;

	iThreadStep = iUseStep;
	//���� ���������� ������ ��ȣ�� �����Ѵ�.
	int iSelectAxis = iSelectAxisss[iUnit];

	// 	for( int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++){	
	// 		if(iHomeProcUsing[iUnit][iAxis] == 1 )		//�䱸 ��ȣ Ȯ��
	// 		{
	// 			iSelectAxis	= iAxis;	
	// 			break;
	// 		}
	// 	}	

	switch(iThreadStep)
	{
	case 1000:  //CL Y   -�������� �̵�
		iThreadStep = 2000;
		break;
		dVel = 10 * sysData.fMotorResol[iUnit][MotCL_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);

		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 1500;
		}
		break;

	case 1500:// CL Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotCL_Y) &&
			GetNegaSensor(iUnit, MotCL_Y) )
		{
			iThreadStep = 2000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��������] CL Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;	
		}
		break;

	case 2000:
		if(ProcLENS[iUnit].OUT_LensGrip(BACKWARD))
		{
			iThreadStep = 2500;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else
		{
			sMsg.Format("[ ��������] LENS GRIP ���� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2000;
		}
		break;

	//20151108 ksg // ���� ���� �׸��� ������ �߰�
	case 2500:
		if(ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			iThreadStep = 2600; //3000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��������] LENS GRIP ���� ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

	//20151117 ksg
	case 2600:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dFTime))
		{
			iThreadStep = 2700;
		}
		break;

	//�Ǹ��� �߰�
	case 2700:
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 2800;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2800:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 3000;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 3000:
		//LENS Z�� ������ ���������� �����ġ, �ƴϸ� ���Ա���.
		if(Motor.GetAmpEnable(iUnit,MotLENS_Z ) == false)
		{
			if(Motor.AmpEnable(iUnit ,MotLENS_Z))		
				Motor.m_bAxisState[iUnit][MotLENS_Z] = true;
		}

		dVel = sysData.fMotorSpeed[iUnit][MotLENS_Z] * sysData.fMotorResol[iUnit][MotLENS_Z];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Z);

		dwReturn = AxmMoveSignalSearch(MotLENS_Z + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{ 
			sMsg.Format("[ ��������] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Z]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3500;			
		}

		break;

	case 3500://! Lens Z�� (+)Limit���� ��ȣ Ȯ��
		if( GetStopAxis(iUnit, MotLENS_Z) &&
			GetPosiSensor(iUnit, MotLENS_Z) )
		{

			if(iSelectAxis == MotLENS_X || iSelectAxis == MotLENS_Z || iSelectAxis == MotLENS_Tx || iSelectAxis == MotLENS_Ty)
			{
				iThreadStep = 4000;
			}
			else
			{
				iThreadStep = 5000;
			}
		}			
		else if(func_TimeChecker_Compare(TIME_DELAY + 5000, dFTime))	//�ð� �ʰ�
		{
			sMsg.Format("[ ��������] LENS Z�� (+)LIMIT �̵� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		break;

	case 4000:  //LENS Y   +�������� �̵�
		if(Motor.GetAmpEnable(iUnit,MotLENS_Y ) == false)
		{
			if(Motor.AmpEnable(iUnit ,MotLENS_Y))		
				Motor.m_bAxisState[iUnit][MotLENS_Y] = true;
		}

		dVel = sysData.fMotorSpeed[iUnit][MotLENS_Y] * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y); 

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit ), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 4500;
		}
		break;

	case 4500:// LENS Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetPosiSensor(iUnit, MotLENS_Y) )
		{
			iThreadStep = 5000;
		}
		else if(func_TimeChecker_Compare(TIME_DELAY + 10000, dFTime))	//�ð� �ʰ�
		{
			sMsg.Format("[ ��������] LENS Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}

		break;

	case 5000:
		//! PCB T�� ����Ʈ���� �̵�
		if( MotPCB_T == iSelectAxis )
		{
			/*dVel = 50 * sysData.fMotorResol[iUnit][MotPCB_T];
			dAcc = 1;
			dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO*iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);*/

			dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_T];
			dAcc = Motor.CalcAcc_G(iUnit,MotPCB_T);

			dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_T]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -5000;
			}
			else
			{
				iThreadStep = 5500;
				dFTime = func_TimeChecker_CurrentTime(true);
			}
		}
		else 
		{
			iThreadStep = 6000;
		}

		break;

	case 5500:
		if( GetStopAxis(iUnit, MotPCB_T) &&
			GetNegaSensor(iUnit, MotPCB_T) )
		{
			iThreadStep = 6000;
		}
		else if(func_TimeChecker_Compare(TIME_DELAY, dFTime))	//�ð� �ʰ�
		{
			StopAxis(iUnit,MotPCB_T);			//����    20160318 JJC
			sMsg.Format("[ ��������] PCB T (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		break;

	case 6000:

		//���õ� �� Ȩã�� ����
		if(Start_Home_Search(iUnit, iSelectAxis) < 0 )
		{	//������ 
			sMsg.Format("[ �������� ] ���� ���� ���� ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -6000;
		}
		else{	//�����϶� Using�� ���������� �ٲ�
			iHomeProcUsing[iUnit][iSelectAxis] = 2;
			iThreadStep = 7000;
		}

		break;

	case 7000:

		if( iHomeProcUsing[iUnit][iSelectAxis] != 0 )
		{
			iHomFlag = IsCompleted_Home_Search(iUnit, iSelectAxis);
			if( iHomFlag < 1 )
			{
				sMsg.Format("[ �������� ] ���� ���� Error �߻� ����.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -7000;
				break;

			}
			if( iHomFlag == 1 ){
				m_bOrgFlag[iUnit][iSelectAxis] = DEF_ORIGIN_COMPLETE;
				iThreadStep = 10000;
			}
		}

		break;


	case 10000:	//�Ϸ� ����
		sMsg.Format("[ �������� ] ���� ���� ������ �Ϸ�Ǿ����ϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
		break;

	default:
		iThreadStep = -8000;

		break;
	}

	return iThreadStep;
}


int AxlMotorSet::HomeSingleProcess_UNIT2(int iUseStep)
{// [ UNIT1 ] ���� ���� ����
	int iUnit = UNIT2;
	CString sMsg;
	int iThreadStep = 0, iHomFlag = 0;
	double dVel,dAcc;
	double dFTime = 0;
	DWORD dwReturn;

	iThreadStep = iUseStep;
	//���� ���������� ������ ��ȣ�� �����Ѵ�.
	int iSelectAxis = iSelectAxisss[iUnit];

	// 	for( int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++){	
	// 		if(iHomeProcUsing[iUnit][iAxis] == 1 )		//�䱸 ��ȣ Ȯ��
	// 		{
	// 			iSelectAxis	= iAxis;	
	// 			break;
	// 		}
	// 	}	

	switch(iThreadStep)
	{
	case 1000:  //CL Y   -�������� �̵�
		iThreadStep = 2000;
		break;
		dVel = 10 * sysData.fMotorResol[iUnit][MotCL_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);

		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 1500;
		}
		break;

	case 1500:// CL Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotCL_Y) &&
			GetNegaSensor(iUnit, MotCL_Y) )
		{
			iThreadStep = 2000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��������] CL Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;	
		}
		break;

	case 2000:
		if(ProcLENS[iUnit].OUT_LensGrip(BACKWARD))
		{
			iThreadStep = 2500;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else
		{
			sMsg.Format("[ ��������] LENS GRIP ���� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2000;
		}
		break;

		//20151108 ksg // ���� ���� �׸��� ������ �߰�
	case 2500:
		if(ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			iThreadStep = 2600; //3000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��������] LENS GRIP ���� ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

		//20151117 ksg
	case 2600:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dFTime))
		{
			iThreadStep = 2700;
		}
		break;

		//�Ǹ��� �߰�
	case 2700:
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 2800;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2800:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 3000;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 3000:
		//LENS Z�� ������ ���������� �����ġ, �ƴϸ� ���Ա���.
		if(Motor.GetAmpEnable(iUnit,MotLENS_Z ) == false)
		{
			if(Motor.AmpEnable(iUnit ,MotLENS_Z))		
				Motor.m_bAxisState[iUnit][MotLENS_Z] = true;
		}

		dVel = sysData.fMotorSpeed[iUnit][MotLENS_Z] * sysData.fMotorResol[iUnit][MotLENS_Z];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Z);

		dwReturn = AxmMoveSignalSearch(MotLENS_Z + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{ 
			sMsg.Format("[ ��������] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Z]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3500;			
		}

		break;

	case 3500://! Lens Z�� (+)Limit���� ��ȣ Ȯ��
		if( GetStopAxis(iUnit, MotLENS_Z) &&
			GetPosiSensor(iUnit, MotLENS_Z) )
		{

			if(iSelectAxis == MotLENS_X || iSelectAxis == MotLENS_Z || iSelectAxis == MotLENS_Tx || iSelectAxis == MotLENS_Ty)
			{
				iThreadStep = 4000;
			}
			else
			{
				iThreadStep = 5000;
			}
		}			
		else if(func_TimeChecker_Compare(TIME_DELAY + 5000, dFTime))	//�ð� �ʰ�
		{
			sMsg.Format("[ ��������] LENS Z�� (+)LIMIT �̵� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		break;

	case 4000:  //LENS Y   +�������� �̵�
		if(Motor.GetAmpEnable(iUnit,MotLENS_Y ) == false)
		{
			if(Motor.AmpEnable(iUnit ,MotLENS_Y))		
				Motor.m_bAxisState[iUnit][MotLENS_Y] = true;
		}

		dVel = sysData.fMotorSpeed[iUnit][MotLENS_Y] * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y); 

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit ), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 4500;
		}
		break;

	case 4500:// LENS Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetPosiSensor(iUnit, MotLENS_Y) )
		{
			iThreadStep = 5000;
		}
		else if(func_TimeChecker_Compare(TIME_DELAY + 10000, dFTime))	//�ð� �ʰ�
		{
			sMsg.Format("[ ��������] LENS Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}

		break;

	case 5000:
		//! PCB T�� ����Ʈ���� �̵�
		if( MotPCB_T == iSelectAxis )
		{
			/*dVel = 50 * sysData.fMotorResol[iUnit][MotPCB_T];
			dAcc = 1;
			dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO*iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);*/

			dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_T];
			dAcc = Motor.CalcAcc_G(iUnit,MotPCB_T);

			dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sMsg.Format("[ ��������] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_T]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -5000;
			}
			else
			{
				iThreadStep = 5500;
				dFTime = func_TimeChecker_CurrentTime(true);
			}
		}
		else 
		{
			iThreadStep = 6000;
		}
		break;

	case 5500:
		if( GetStopAxis(iUnit, MotPCB_T) &&
			GetNegaSensor(iUnit, MotPCB_T) )
		{
			iThreadStep = 6000;
		}
		else if(func_TimeChecker_Compare(TIME_DELAY, dFTime))	//�ð� �ʰ�
		{
			sMsg.Format("[ ��������] PCB T (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ�");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		break;

	case 6000:

		//���õ� �� Ȩã�� ����
		if(Start_Home_Search(iUnit, iSelectAxis) < 0 )
		{	//������ 
			sMsg.Format("[ �������� ] ���� ���� ���� ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -6000;
		}
		else{	//�����϶� Using�� ���������� �ٲ�
			iHomeProcUsing[iUnit][iSelectAxis] = 2;
			iThreadStep = 7000;
		}
		break;

	case 7000:

		if( iHomeProcUsing[iUnit][iSelectAxis] != 0 )
		{
			iHomFlag = IsCompleted_Home_Search(iUnit, iSelectAxis);
			if( iHomFlag < 1 )
			{
				sMsg.Format("[ �������� ] ���� ���� Error �߻� ����.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -7000;
				break;
			}
			if( iHomFlag == 1 ){
				m_bOrgFlag[iUnit][iSelectAxis] = DEF_ORIGIN_COMPLETE;
				iThreadStep = 10000;
			}
		}
		break;

	case 10000:	//�Ϸ� ����
		sMsg.Format("[ �������� ] ���� ���� ������ �Ϸ�Ǿ����ϴ�.");
		g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
		break;

	default:
		iThreadStep = -8000;

		break;
	}

	return iThreadStep;
}

DWORD ThreadHomeProcessAA1(AxlMotorSet* pAxlMotorSet)	
{// [ UNIT1 ] ���� ���� ����
	int iUnit = UNIT1;

	CString sMsg;
	int iThreadStep = 1000, iHomFlag = 0;
	double dFTime = 0;

	//���� ���������� ������ ��ȣ�� �����Ѵ�.
	int iSelectAxis;

	for( int iAxiss = 0; iAxiss < CUR_MOTOR_NO; iAxiss++){	
		if( pAxlMotorSet->iHomeProcUsing[iUnit][iAxiss] == 1 )		//�䱸 ��ȣ Ȯ��
		{
			pAxlMotorSet->iSelectAxisss[iUnit]	= iAxiss;	
			break;
		}
	}

	iSelectAxis = pAxlMotorSet->iSelectAxisss[iUnit];

	if(iSelectAxis < 0 || iSelectAxis > 8)
		return TRUE;

	pAxlMotorSet->bThreadHome[iUnit] = true;
	while(pAxlMotorSet->bThreadHome[iUnit])
	{

		Sleep(5);

		//���� ã�Ⱑ ���۵Ǿ��� �������� �˻�
		if(pAxlMotorSet->iHomeProcUsing[iUnit][iSelectAxis] == 2)
		{
			//Amp �˶� üũ
			if( pAxlMotorSet->GetAmpFault(iUnit, iSelectAxis) == true )
			{
				sMsg.Format("[ ��������] ���� �˻� �� AMP �˶� �߻�.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				pAxlMotorSet->bThreadHome[iUnit] = false;
				break;
			}

			//������ üũ
			if( pAxlMotorSet->GetAmpEnable(iUnit, iSelectAxis) == false )
			{
				sMsg.Format("[ ��������] ���� �˻� �� Servo Off �˶� �߻�.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

				pAxlMotorSet->bThreadHome[iUnit] = false;
				break;
			}	

		}
		
		if(iThreadStep >= 1000 && iThreadStep < 10000)		
			iThreadStep = pAxlMotorSet->HomeSingleProcess_UNIT1(iThreadStep);
		else
		{
			//���� Thread �Ϸ� ����
			break;
		}

		if( iThreadStep < 0 )
		{//���� �߻� ���� ����.

			iHomFlag = pAxlMotorSet->IsCompleted_Home_Search(iUnit, iSelectAxis);
			if( iHomFlag == 2 )
			{
				pAxlMotorSet->Stop_Home_Search(iUnit, iSelectAxis);
				pAxlMotorSet->m_bOrgFlag[iUnit][iSelectAxis] = false;
			}	
			break;	
		}

	}

	pAxlMotorSet->iHomeProcUsing[iUnit][iSelectAxis] = 0;	//���� ���� ��û �ʱ�ȭ

	pAxlMotorSet->m_hThreadHome[iUnit] = NULL;

	pAxlMotorSet->bThreadHome[iUnit] = false;

	return TRUE;
}

DWORD ThreadHomeProcessAA2(AxlMotorSet* pAxlMotorSet)	
{// [ UNIT2 ] ���� ���� ����
	int iUnit = UNIT2;

	CString sMsg;
	int iThreadStep = 1000, iHomFlag = 0;
	double dFTime = 0;

	//���� ���������� ������ ��ȣ�� �����Ѵ�.
	int iSelectAxis;

	for( int iAxiss = 0; iAxiss < CUR_MOTOR_NO; iAxiss++){	
		if( pAxlMotorSet->iHomeProcUsing[iUnit][iAxiss] == 1 )		//�䱸 ��ȣ Ȯ��
		{
			pAxlMotorSet->iSelectAxisss[iUnit]	= iAxiss;	
			break;
		}
	}

	iSelectAxis = pAxlMotorSet->iSelectAxisss[iUnit];

	if(iSelectAxis < 0 || iSelectAxis > 8)
		return TRUE;

	pAxlMotorSet->bThreadHome[iUnit] = true;

	while(pAxlMotorSet->bThreadHome[iUnit])
	{

		Sleep(5);

		//���� ã�Ⱑ ���۵Ǿ��� �������� �˻�
		//iSelectAxis = 5;
		if(pAxlMotorSet->iHomeProcUsing[iUnit][iSelectAxis] == 2)
		{
			//Amp �˶� üũ
			if( pAxlMotorSet->GetAmpFault(iUnit, iSelectAxis) == true )
			{
				sMsg.Format("[ ��������] ���� �˻� �� AMP �˶� �߻�.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				pAxlMotorSet->bThreadHome[iUnit] = false;
				break;
			}

			//������ üũ
			if( pAxlMotorSet->GetAmpEnable(iUnit, iSelectAxis) == false )
			{
				sMsg.Format("[ ��������] ���� �˻� �� Servo Off �˶� �߻�.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);

				pAxlMotorSet->bThreadHome[iUnit] = false;
				break;
			}	

		}

		if(iThreadStep >= 1000 && iThreadStep < 10000)		
			iThreadStep = pAxlMotorSet->HomeSingleProcess_UNIT2(iThreadStep);
		else
		{
			//���� Thread �Ϸ� ����
			break;
		}

		if( iThreadStep < 0 )
		{//���� �߻� ���� ����.

			iHomFlag = pAxlMotorSet->IsCompleted_Home_Search(iUnit, iSelectAxis);
			if( iHomFlag == 2 )
			{
				pAxlMotorSet->Stop_Home_Search(iUnit, iSelectAxis);
				pAxlMotorSet->m_bOrgFlag[iUnit][iSelectAxis] = false;
			}	
			break;	
		}

	}

	pAxlMotorSet->iHomeProcUsing[iUnit][iSelectAxis] = 0;	//���� ���� ��û �ʱ�ȭ

	pAxlMotorSet->m_hThreadHome[iUnit] = NULL;

	pAxlMotorSet->bThreadHome[iUnit] = false;

	return TRUE;
}

//---------------------------�Ŵ��� AA ----------------------------------
DWORD ThreadManualFocusUNIT1(AxlMotorSet* pAxlMotorSet)
{
	//int iUnit = UNIT1;
	CString sMsg, str;
	int iRtnStep = 0;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	DWORD dwRet = 0;
	long  AxisCount;

	Task[UNIT1].AutoFlag = 1;			//�ڵ� �÷��� ��
	Task[UNIT1].bRunningReady = false;	//�����غ� Ǯ��

	pAxlMotorSet->iManualStep[UNIT1] = 130000;

	while(pAxlMotorSet->m_hThreadManualFocus[UNIT1])
	{
		Sleep(5);
		int bChk = 0;

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ] AXT Board �ν� ����.. �Ŵ��� AA ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
			pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
			pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;
			return FALSE;
		}

		bool bAlarmLimit = true;
		for (int i=0; i<CUR_MOTOR_NO; i++)
		{
			if( pAxlMotorSet->GetAmpFault(UNIT1, i) )
			{
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� AMP �˶� ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;
				return FALSE;
			}

			if( !Motor.GetAmpEnable(UNIT1, i) )		//TRUE : ON
			{
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� SERVO OFF ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;
				return FALSE;
			}

			if( Motor.GetNegaSensor(UNIT1, i) )
			{//kdu ���� ������ �Ͻ�����
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� ( - ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;
				return FALSE;
			}

			if( Motor.GetPosiSensor(UNIT1, i) )
			{//kdu ���� ������ �Ͻ�����
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� ( + ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;
				return FALSE;
			}
		}
		if(Task[UNIT1].AutoFlag == 0 || Task[UNIT1].AutoFlag == 2)			break;

		if(pAxlMotorSet->iManualStep[UNIT1] >= 130000 && pAxlMotorSet->iManualStep[UNIT1] < 160000)			iRtnStep = ProcAA[UNIT1].RunProc_Manual_AA(pAxlMotorSet->iManualStep[UNIT1])	;					//0

		//!! Thread ���� ���� ó�� �� ���� ó��.(0 ���� �� ��� Alarm �߻� ����)
		pAxlMotorSet->iManualStep[UNIT1] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.

		if( iRtnStep < 0 || iRtnStep > 159000)
		{
			pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
			//����
			Task[UNIT1].AutoFlag = 0;
			break;
		}

		Sleep(5);
		DoSleepEvents();

	}

	pAxlMotorSet->m_bThreadManualFocus[UNIT1] = false;
	pAxlMotorSet->m_hThreadManualFocus[UNIT1] = NULL;

	return TRUE;
}


DWORD ThreadManualFocusUNIT2(AxlMotorSet* pAxlMotorSet)
{
	//int iUnit = UNIT2;
	CString sMsg, str;
	int iRtnStep = 0;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	DWORD dwRet = 0;
	long  AxisCount;

	Task[UNIT2].AutoFlag = 1;			//�ڵ� �÷��� ��
	Task[UNIT2].bRunningReady = false;	//�����غ� Ǯ��

	pAxlMotorSet->iManualStep[UNIT2] = 130000;

	while(pAxlMotorSet->m_hThreadManualFocus[UNIT2])
	{
		Sleep(5);
		int bChk = 0;

		dwRet = AxmInfoGetAxisCount(&AxisCount);
		if ( dwRet != AXT_RT_SUCCESS )
		{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
			sMsg.Format("[ ERROR ] AXT Board �ν� ����.. �ڵ� ������ ���� �մϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
			pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
			pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;
			return FALSE;
		}

		bool bAlarmLimit = true;
		for (int i=0; i<CUR_MOTOR_NO; i++)
		{
			if( pAxlMotorSet->GetAmpFault(UNIT2, i) )
			{
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� AMP �˶� ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;
				return FALSE;
			}

			if( !Motor.GetAmpEnable(UNIT2, i) )		//TRUE : ON
			{
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� SERVO OFF ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;
				return FALSE;
			}

			if( Motor.GetNegaSensor(UNIT2, i) )
			{//kdu ���� ������ �Ͻ�����
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� ( - ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;
				return FALSE;
			}

			if( Motor.GetPosiSensor(UNIT2, i) )
			{//kdu ���� ������ �Ͻ�����
				sMsg.Format("[ ERROR ] �Ŵ��� AA ���� �� ( + ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
				pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
				pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;
				return FALSE;
			}
		}
		if(Task[UNIT2].AutoFlag == 0 || Task[UNIT2].AutoFlag == 2)			break;

		if(pAxlMotorSet->iManualStep[UNIT2] >= 130000 && pAxlMotorSet->iManualStep[UNIT2] < 160000)			iRtnStep = ProcAA[UNIT2].RunProc_Manual_AA(pAxlMotorSet->iManualStep[UNIT2])	;					//0

		//!! Thread ���� ���� ó�� �� ���� ó��.(0 ���� �� ��� Alarm �߻� ����)
		pAxlMotorSet->iManualStep[UNIT2] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.

		if( iRtnStep < 0 || iRtnStep > 159000)
		{
			pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
			//����
			Task[UNIT2].AutoFlag = 0;
			break;
		}

		Sleep(5);
		DoSleepEvents();

	}

	pAxlMotorSet->m_bThreadManualFocus[UNIT2] = false;
	pAxlMotorSet->m_hThreadManualFocus[UNIT2] = NULL;

	return TRUE;
}

//-----------------------------------------------------------------------
 

//---------------------------�ھƵ�
DWORD ThreadAutoFocusUNIT1(AxlMotorSet* pAxlMotorSet)
{
	//int iUnit = UNIT1;
	CString sMsg, str;
	int iRtnStep = 0;
//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	DWORD dwRet = 0;
	//long  AxisCount;

	if( Task[UNIT1].AutoFlag != 2 )		//�Ͻ� ������ �ƴҶ�.
	{
		if(Task[UNIT1].iLensStatus != 0)
		{
			if(Task[UNIT1].iLensStatus == 1)		//����ǰ ����
			{
				ProcAA[UNIT1].bInspOk = true;
			}
			else if(Task[UNIT1].iLensStatus == 2)	//�ҷ� ����
			{
				ProcAA[UNIT1].bInspOk = false;
			}

			//���Ͼ�ε�
			pAxlMotorSet->iRunStep[UNIT1] = 250000;
		}
	}

	if( Task[UNIT1].AutoFlag == 2 )		//�Ͻ������� 
	{
		str.Format("###%d", Task[UNIT1].iExhaust);
		g_PushLogQueue(str, DEF_TYPE_ERRORLOG + GetUnitNum(UNIT1));
		//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + GetUnitNum(UNIT1), (LPARAM)&str);

		if(Task[UNIT1].iExhaust != 0)		// ����
		{
			pAxlMotorSet->iRunStep[UNIT1] = 400000;
		}
	}

	Task[UNIT1].AutoFlag = 1;			//�ڵ� �÷��� ��
	Task[UNIT1].bRunningReady = false;	//�����غ� Ǯ��

	while(pAxlMotorSet->m_hThreadAutoFocus[UNIT1])
	{
		Sleep(5);
		int bChk = 0;

		//dwRet = AxmInfoGetAxisCount(&AxisCount);
		//if ( dwRet != AXT_RT_SUCCESS )
		//{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
		//	sMsg.Format("[ ERROR ] AXT Board �ν� ����.. �ڵ� ������ ���� �մϴ�.");
		//	::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		//	pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
		//	pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;
		//	return FALSE;
		//}

		//bool bAlarmLimit = true;
		//for (int i=0; i<CUR_MOTOR_NO; i++)
		//{
		//	if( pAxlMotorSet->GetAmpFault(UNIT1, i) )
		//	{
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� AMP �˶� ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;
		//		return FALSE;
		//	}

		//	if( !Motor.GetAmpEnable(UNIT1, i) )		//TRUE : ON
		//	{
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� SERVO OFF ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;
		//		return FALSE;
		//	}

		//	if( Motor.GetNegaSensor(UNIT1, i) )
		//	{//kdu ���� ������ �Ͻ�����
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� ( - ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;
		//		return FALSE;
		//	}

		//	if( Motor.GetPosiSensor(UNIT1, i) )
		//	{//kdu ���� ������ �Ͻ�����
		//		sMsg.Format("[ ERROR ][ �ڵ�����] �ڵ� ���� �� ( + ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;
		//		return FALSE;
		//	}
		//}

		/*
		Lens Socket Loading : �޾Ƽ� ���͸� ��������
		PCB_Bonding  : ���͸� �������� ���� ������ġ ����
		Lens_Loading : ������ġ���� ������ �ް� ��� �׸�����
		Lens_Bonding : ������ġ�� �̵�
		Lens_Release : UV �� �� �׸� ���� ��� ������.
		PCB_Unloading : LENS, PCB �Ѵ� ����,�����ġ�� �̵�.
		ULD_MODE :  �������� GRIP ���� �ʰ� �̸��� ����.(������ġ�� �ϴ� �����ġ)


		//Ǫ��,
		Align �� PCB T �����°� ( �׸���� T ������) <-- RunProc_Lens_Loading 

		Align 
		������ ����
		ü�� ���� -> Lens_Loading���� 
		���� �˻�
		*/
		if( Task[UNIT1].AutoFlag == 0 )			break;
		
		if( Task[UNIT1].AutoFlag == MODE_PAUSE )
		{
			if( pAxlMotorSet->iRunStep[UNIT1] >= 198000 && pAxlMotorSet->iRunStep[UNIT1] < 210000 )
			{//! �Ͻ� �������濡��. UV_ON�ϰ�� ������ ������
				iRtnStep = ProcUV[UNIT1].RunProc_UV(pAxlMotorSet->iRunStep[UNIT1]);	

				pAxlMotorSet->iRunStepOrg[UNIT1] = pAxlMotorSet->iRunStep[UNIT1];
				pAxlMotorSet->iRunStep[UNIT1] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.

				if( iRtnStep < 0 )
				{//- �˶� �߻���, ����
					ProcLENS[UNIT1].OUT_UV_LAMP(UV_OFF);
					pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
					break;
				}
			}
			else
			{
				pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
				break;
			}

			continue;
		}

		if( Task[UNIT1].AutoFlag != 1 )
		{
			pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
			Task[UNIT1].AutoFlag = 2;		
			break;
		}

		str.Format("[ AUTO STEP : %d ]", pAxlMotorSet->iRunStep[UNIT1]);
	//	pFrame->m_btn_title1.CallFlashFunction("Version", "%s", str);
		//sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iRunStep[UNIT1]);			//20160828 JJC
		pFrame->stcVersion[UNIT1].SetWindowTextA(str);


		if(pAxlMotorSet->iRunStep[UNIT1] >= 10000 && pAxlMotorSet->iRunStep[UNIT1] < 30000)			iRtnStep = ProcPCB[UNIT1].RunProc_Socket_Loading(pAxlMotorSet->iRunStep[UNIT1])	;					//0
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 30000 && pAxlMotorSet->iRunStep[UNIT1] < 50000)	iRtnStep = ProcPCB[UNIT1].RunProc_Bonding(pAxlMotorSet->iRunStep[UNIT1]);							//O

		else if(pAxlMotorSet->iRunStep[UNIT1] >= 50000 && pAxlMotorSet->iRunStep[UNIT1] < 60000)	iRtnStep = ProcAA[UNIT1].AlignProc_Barrel(pAxlMotorSet->iRunStep[UNIT1]);																			
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 60000 && pAxlMotorSet->iRunStep[UNIT1] < 70000)	iRtnStep = ProcAA[UNIT1].AlignProc_Cylinder(pAxlMotorSet->iRunStep[UNIT1]);	
		//else if(pAxlMotorSet->iRunStep[UNIT1] >= 70000 && pAxlMotorSet->iRunStep[UNIT1] < 80000)		iRtnStep = ProcAA[UNIT1].AlignProc_Lens(pAxlMotorSet->iRunStep[UNIT1]);		//20160111JJC NV UT �÷ο� ��ħ	
#ifdef ON_MODEL_NV
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 70000 && pAxlMotorSet->iRunStep[UNIT1] < 79000)	iRtnStep = ProcAA[UNIT1].AlignProc_NVLens(pAxlMotorSet->iRunStep[UNIT1]);																			
#else
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 70000 && pAxlMotorSet->iRunStep[UNIT1] < 79000)	iRtnStep = ProcAA[UNIT1].AlignProc_Lens(pAxlMotorSet->iRunStep[UNIT1]);																			
#endif
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 79000 && pAxlMotorSet->iRunStep[UNIT1] < 90000)	iRtnStep = ProcAA[UNIT1].AlignProc_Chart(pAxlMotorSet->iRunStep[UNIT1]);																			

		//else if(pAxlMotorSet->iRunStep[UNIT1] >= 50000 && pAxlMotorSet->iRunStep[UNIT1] < 90000)	iRtnStep = ProcAA[UNIT1].RunProc_Align(pAxlMotorSet->iRunStep[UNIT1]);																			
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 90000 && pAxlMotorSet->iRunStep[UNIT1] < 110000)	iRtnStep = ProcLENS[UNIT1].RunProc_Lens_Loading(pAxlMotorSet->iRunStep[UNIT1]);//Lens Loading		//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 110000 && pAxlMotorSet->iRunStep[UNIT1] < 130000)	iRtnStep = ProcLENS[UNIT1].RunProc_Bonding(pAxlMotorSet->iRunStep[UNIT1]);	//Lens Bonding			//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 130000 && pAxlMotorSet->iRunStep[UNIT1] < 170000)	iRtnStep = ProcAA[UNIT1].RunProc_AA(pAxlMotorSet->iRunStep[UNIT1]);//AA								//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 170000 && pAxlMotorSet->iRunStep[UNIT1] < 190000)	iRtnStep = ProcAA[UNIT1].RunProc_ThroughFocus(pAxlMotorSet->iRunStep[UNIT1]);						//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 190000 && pAxlMotorSet->iRunStep[UNIT1] < 210000)	iRtnStep = ProcUV[UNIT1].RunProc_UV(pAxlMotorSet->iRunStep[UNIT1]);									//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 210000 && pAxlMotorSet->iRunStep[UNIT1] < 230000)	iRtnStep = ProcLENS[UNIT1].RunProc_Release(pAxlMotorSet->iRunStep[UNIT1]);							//O	
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 230000 && pAxlMotorSet->iRunStep[UNIT1] < 250000)	iRtnStep = ProcPCB[UNIT1].RunProc_UnLoading(pAxlMotorSet->iRunStep[UNIT1]);//PCB_UnLoading			//O
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 250000 && pAxlMotorSet->iRunStep[UNIT1] < 270000)	iRtnStep = ProcPCB[UNIT1].RunProc_Socket_Unloading(pAxlMotorSet->iRunStep[UNIT1]);					//0
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 300000 && pAxlMotorSet->iRunStep[UNIT1] < 330000)	iRtnStep = ProcAA[UNIT1].RunProc_ULD_MODE(pAxlMotorSet->iRunStep[UNIT1]);	//ULD MODE				//O
		//else if(pAxlMotorSet->iRunStep[UNIT1] >= 400000 && pAxlMotorSet->iRunStep[UNIT1] < 410000)	iRtnStep = ProcAA[UNIT1].RunProc_NG_MODE(pAxlMotorSet->iRunStep[UNIT1]);
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 400000 && pAxlMotorSet->iRunStep[UNIT1] < 406000)	iRtnStep = ProcAA[UNIT1].RunProc_NG_MODE(pAxlMotorSet->iRunStep[UNIT1]);//20160126JJC
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 410000 && pAxlMotorSet->iRunStep[UNIT1] < 415000)	iRtnStep = ProcPCB[UNIT1].RunProc_TiltSpecOut_Retry(pAxlMotorSet->iRunStep[UNIT1]);	//20160126JJC
		else if(pAxlMotorSet->iRunStep[UNIT1] >= 420000 && pAxlMotorSet->iRunStep[UNIT1] < 430000)	iRtnStep = ProcAA[UNIT1].AlignProc_AA_After_Align(pAxlMotorSet->iRunStep[UNIT1]);


		pAxlMotorSet->iRunStepOrg[UNIT1] = pAxlMotorSet->iRunStep[UNIT1];
		//!! Thread ���� ���� ó�� �� ���� ó��.(0 ���� �� ��� Alarm �߻� ����)
		pAxlMotorSet->iRunStep[UNIT1] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.

		if( iRtnStep < 0 )
		{
			CCPreFocusingAADlg* pFrame =(CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;
			sMsg.Format("#AA%d@ALARM&ERR$", (sysData.m_iUnitNo*2)+UNIT1+1);
			pFrame->HidataSocket_ClientSend(UNIT1, sMsg);

			ProcLENS[UNIT1].OUT_UV_LAMP(UV_OFF);
			gCommUV.UV_Trigger(UNIT1 , 2 , DIO_OFF , model.iUVLightVal[UNIT1]);
			pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
			//�Ͻ������� �ҵ�����.
			Task[UNIT1].AutoFlag = 2;
			break;
		}

		Sleep(5);
		DoSleepEvents();

	}

	pAxlMotorSet->m_bThreadAutoFocus[UNIT1] = false;
	pAxlMotorSet->m_hThreadAutoFocus[UNIT1] = NULL;

	return TRUE;
}


DWORD ThreadAutoFocusUNIT2(AxlMotorSet* pAxlMotorSet)
{
	//int iUnit = UNIT2;
	CString sMsg, str;
	int iRtnStep = 0;
//	char szVer[SIZE_OF_100BYTE];		//20160828 JJC
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	DWORD dwRet = 0;
	//long  AxisCount;

	if(Task[UNIT2].AutoFlag != 2)		//�Ͻ� ������ �ƴҶ�.
	{
		if(Task[UNIT2].iLensStatus != 0)
		{
			if(Task[UNIT2].iLensStatus == 1)		//����ǰ ����
			{
				ProcAA[UNIT2].bInspOk = true;
			}
			else if(Task[UNIT2].iLensStatus == 2)	//�ҷ� ����
			{
				ProcAA[UNIT2].bInspOk = false;
			}

			//���Ͼ�ε�
			pAxlMotorSet->iRunStep[UNIT2] = 250000;

		}
	}

	if( Task[UNIT2].AutoFlag == 2 )		//�Ͻ������� 
	{
		if(Task[UNIT2].iExhaust != 0)		// ����
		{
			pAxlMotorSet->iRunStep[UNIT2] = 400000;
		}
	}

	Task[UNIT2].AutoFlag = 1;
	//�����غ� Ǯ��
	Task[UNIT2].bRunningReady = false;

	while(pAxlMotorSet->m_hThreadAutoFocus[UNIT2])
	{
		Sleep(5);
		int bChk = 0;

		//dwRet = AxmInfoGetAxisCount(&AxisCount);
		//if ( dwRet != AXT_RT_SUCCESS )
		//{//! Servo ������ �������� ��� ��Ȳ�� ���� �Ͽ��� �Ѵ�.
		//	sMsg.Format("[ ERROR ] AXT Board �ν� ����.. �ڵ� ������ ���� �մϴ�.");
		//	::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
		//	pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
		//	pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;
		//	return FALSE;
		//}

		//bool bAlarmLimit = true;
		//for (int i=0; i<CUR_MOTOR_NO; i++)
		//{

		//	if( pAxlMotorSet->GetAmpFault(UNIT2, i) )
		//	{
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� AMP �˶� ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;
		//		return FALSE;
		//	}

		//	if( !Motor.GetAmpEnable(UNIT2, i) )		//TRUE : ON
		//	{
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� SERVO OFF ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;
		//		return FALSE;
		//	}

		//	if( Motor.GetNegaSensor(UNIT2, i) )
		//	{//kdu ���� ������ �Ͻ�����
		//		sMsg.Format("[ ERROR ][ �ڵ����� ] �ڵ� ���� �� ( - ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;
		//		return FALSE;
		//	}

		//	if( Motor.GetPosiSensor(UNIT2, i) )
		//	{//kdu ���� ������ �Ͻ�����
		//		sMsg.Format("[ ERROR ][ �ڵ�����] �ڵ� ���� �� ( + ) Limit ���� AXIS : %s",MOTOR_NAME[i]);
		//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);
		//		pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
		//		pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;
		//		return FALSE;
		//	}
		//}

		/*
		Lens Socket Loading : �޾Ƽ� ���͸� ��������
		PCB_Bonding  : ���͸� �������� ���� ������ġ ����
		Lens_Loading : ������ġ���� ������ �ް� ��� �׸�����
		Lens_Bonding : ������ġ�� �̵�
		Lens_Release : UV �� �� �׸� ���� ��� ������.
		PCB_Unloading : LENS, PCB �Ѵ� ����,�����ġ�� �̵�.
		ULD_MODE :  �������� GRIP ���� �ʰ� �̸��� ����.(������ġ�� �ϴ� �����ġ)


		//Ǫ��,
		Align �� PCB T �����°� ( �׸���� T ������) <-- RunProc_Lens_Loading 
		*/

		if( Task[UNIT2].AutoFlag == 0 )			break;

		if( Task[UNIT2].AutoFlag == MODE_PAUSE )
		{
			if( pAxlMotorSet->iRunStep[UNIT2] >= 198000 && pAxlMotorSet->iRunStep[UNIT2] < 210000 )
			{//! �Ͻ� �������濡��. UV_ON�ϰ�� ������ ������
				iRtnStep = ProcUV[UNIT2].RunProc_UV(pAxlMotorSet->iRunStep[UNIT2]);	

				pAxlMotorSet->iRunStepOrg[UNIT2] = pAxlMotorSet->iRunStep[UNIT2];
				pAxlMotorSet->iRunStep[UNIT2] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.

				if( iRtnStep < 0 )
				{//- �˶� �߻���, ����
					ProcLENS[UNIT2].OUT_UV_LAMP(UV_OFF);
					pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
					break;
				}
			}
			else
			{
				pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
				break;
			}

			continue;
		}

		if( Task[UNIT2].AutoFlag != 1 )
		{
			pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
			Task[UNIT2].AutoFlag = 2;
			break;
		}

		str.Format("[ AUTO STEP : %d ]", pAxlMotorSet->iRunStep[UNIT2]);
	//	pFrame->m_btn_title2.CallFlashFunction("Version", "%s", str);
//		sprintf_s(szVer, SIZE_OF_100BYTE, "[ STEP : %d ]", iRunStep[UNIT2]);			//20160828 JJC
		pFrame->stcVersion[UNIT2].SetWindowTextA(str);

		if(pAxlMotorSet->iRunStep[UNIT2] >= 10000 && pAxlMotorSet->iRunStep[UNIT2] < 30000)			iRtnStep = ProcPCB[UNIT2].RunProc_Socket_Loading(pAxlMotorSet->iRunStep[UNIT2])	;					//0
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 30000 && pAxlMotorSet->iRunStep[UNIT2] < 50000)	iRtnStep = ProcPCB[UNIT2].RunProc_Bonding(pAxlMotorSet->iRunStep[UNIT2]);
		//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 50000 && pAxlMotorSet->iRunStep[UNIT2] < 60000)	iRtnStep = ProcAA[UNIT2].AlignProc_Barrel(pAxlMotorSet->iRunStep[UNIT2]);																			
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 60000 && pAxlMotorSet->iRunStep[UNIT2] < 70000)	iRtnStep = ProcAA[UNIT2].AlignProc_Cylinder(pAxlMotorSet->iRunStep[UNIT2]);																			
	//	else if(pAxlMotorSet->iRunStep[UNIT2] >= 70000 && pAxlMotorSet->iRunStep[UNIT2] < 80000)	iRtnStep = ProcAA[UNIT2].AlignProc_Lens(pAxlMotorSet->iRunStep[UNIT2]);		//20160115JJC
#ifdef ON_MODEL_NV		//20160115JJC
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 70000 && pAxlMotorSet->iRunStep[UNIT2] < 79000)	iRtnStep = ProcAA[UNIT2].AlignProc_NVLens(pAxlMotorSet->iRunStep[UNIT2]);	
#else
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 70000 && pAxlMotorSet->iRunStep[UNIT2] < 79000/*80000*/)	iRtnStep = ProcAA[UNIT2].AlignProc_Lens(pAxlMotorSet->iRunStep[UNIT2]);																			
#endif
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 79000/*80000*/ && pAxlMotorSet->iRunStep[UNIT2] < 90000)	iRtnStep = ProcAA[UNIT2].AlignProc_Chart(pAxlMotorSet->iRunStep[UNIT2]);																			
		//else if(pAxlMotorSet->iRunStep[UNIT2] >= 50000 && pAxlMotorSet->iRunStep[UNIT2] < 90000)	iRtnStep = ProcAA[UNIT2].RunProc_Align(pAxlMotorSet->iRunStep[UNIT2]);	

		else if(pAxlMotorSet->iRunStep[UNIT2] >= 90000 && pAxlMotorSet->iRunStep[UNIT2] < 110000)	iRtnStep = ProcLENS[UNIT2].RunProc_Lens_Loading(pAxlMotorSet->iRunStep[UNIT2]);//Lens Loading	//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 110000 && pAxlMotorSet->iRunStep[UNIT2] < 130000)	iRtnStep = ProcLENS[UNIT2].RunProc_Bonding(pAxlMotorSet->iRunStep[UNIT2]);	//Lens Bonding			//O //�跲 ���� �߰�
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 130000 && pAxlMotorSet->iRunStep[UNIT2] < 170000)	iRtnStep = ProcAA[UNIT2].RunProc_AA(pAxlMotorSet->iRunStep[UNIT2]);//AA								//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 170000 && pAxlMotorSet->iRunStep[UNIT2] < 190000)	iRtnStep = ProcAA[UNIT2].RunProc_ThroughFocus(pAxlMotorSet->iRunStep[UNIT2]);						//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 190000 && pAxlMotorSet->iRunStep[UNIT2] < 210000)	iRtnStep = ProcUV[UNIT2].RunProc_UV(pAxlMotorSet->iRunStep[UNIT2]);								//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 210000 && pAxlMotorSet->iRunStep[UNIT2] < 230000)	iRtnStep = ProcLENS[UNIT2].RunProc_Release(pAxlMotorSet->iRunStep[UNIT2]);							//O	
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 230000 && pAxlMotorSet->iRunStep[UNIT2] < 250000)	iRtnStep = ProcPCB[UNIT2].RunProc_UnLoading(pAxlMotorSet->iRunStep[UNIT2]);//PCB_UnLoading			//O
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 250000 && pAxlMotorSet->iRunStep[UNIT2] < 270000)	iRtnStep = ProcPCB[UNIT2].RunProc_Socket_Unloading(pAxlMotorSet->iRunStep[UNIT2]);					//0
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 300000 && pAxlMotorSet->iRunStep[UNIT2] < 330000)	iRtnStep = ProcAA[UNIT2].RunProc_ULD_MODE(pAxlMotorSet->iRunStep[UNIT2]);	//ULD MODE				//O
		//else if(pAxlMotorSet->iRunStep[UNIT2] >= 400000 && pAxlMotorSet->iRunStep[UNIT2] < 410000)	iRtnStep = ProcAA[UNIT2].RunProc_NG_MODE(pAxlMotorSet->iRunStep[UNIT2]);
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 400000 && pAxlMotorSet->iRunStep[UNIT2] < 406000)	iRtnStep = ProcAA[UNIT2].RunProc_NG_MODE(pAxlMotorSet->iRunStep[UNIT2]);			//20160126JJC
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 410000 && pAxlMotorSet->iRunStep[UNIT2] < 415000)	iRtnStep = ProcPCB[UNIT2].RunProc_TiltSpecOut_Retry(pAxlMotorSet->iRunStep[UNIT2]);	//20160126JJC
		else if(pAxlMotorSet->iRunStep[UNIT2] >= 420000 && pAxlMotorSet->iRunStep[UNIT2] < 430000)	iRtnStep = ProcAA[UNIT2].AlignProc_AA_After_Align(pAxlMotorSet->iRunStep[UNIT2]);


		pAxlMotorSet->iRunStepOrg[UNIT2] = pAxlMotorSet->iRunStep[UNIT2];
		//!! Thread ���� ���� ó�� �� ���� ó��.(0 ���� �� ��� Alarm �߻� ����)
		pAxlMotorSet->iRunStep[UNIT2] = abs(iRtnStep);	//Error �߻�.. iRunStep���� -1�� ���ϸ� �絿�۽� ���� ��ȣ����.
		if( iRtnStep < 0 )
		{
			CCPreFocusingAADlg* pFrame =(CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;
			sMsg.Format("#AA%d@ALARM&ERR$", (sysData.m_iUnitNo*2)+UNIT2+1);
			pFrame->HidataSocket_ClientSend(UNIT2, sMsg);

			gCommUV.UV_Trigger(UNIT2 , 2 , DIO_OFF , model.iUVLightVal[UNIT2]);
			pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
			//�Ͻ������� �ҵ�����.
			Task[UNIT2].AutoFlag = 2;
			break;
		}

		Sleep(5);
		DoSleepEvents();
	}

	pAxlMotorSet->m_bThreadAutoFocus[UNIT2] = false;
	pAxlMotorSet->m_hThreadAutoFocus[UNIT2] = NULL;

	return TRUE;
}

//-------------------------�ڵ� ��

void AxlMotorSet::func_AutoProcStop(int iUnit)
{//! �ڵ���� ����
	CString sMsg;
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	//������ ���� ��ư�� ��ȣ�� ����ϰ� subâ ����
	theApp.g_DlgSlideBar->iUnitStatus = iUnit;
	//for( int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
	//{	
	//	//AxmMoveEStop(iAxis + (0 * CUR_MOTOR_NO));
	//}

#ifdef ON_LINE_SOKET
	sMsg.Format("#AA%d@ALARM&CLR$", (sysData.m_iUnitNo*2) + iUnit + 1);
	pFrame->HidataSocket_ClientSend(iUnit, sMsg);
	sMsg.Format("#AA%d@PCB&ESC$", (sysData.m_iUnitNo*2) + iUnit + 1);
	pFrame->HidataSocket_ClientSend(iUnit, sMsg);

	Sleep(50);
	sMsg.Format("#AA%d@ULD&ESC$", (sysData.m_iUnitNo*2) + iUnit + 1);
	pFrame->HidataSocket_ClientSend(iUnit, sMsg);
	sMsg.Format("#AA%d@LEN&ESC$", (sysData.m_iUnitNo*2) + iUnit + 1);
	pFrame->HidataSocket_ClientSend(iUnit, sMsg);
#endif

	if( Task[iUnit].AutoFlag != 0 )		Task[iUnit].AutoFlag = 0;

	m_hThreadHomeAll[iUnit] = false;	//��ü �������� ����
	m_hThreadHome[iUnit]	= false;	//���� �������� ����	
	m_hThreadReady[iUnit]	= false;	//�����غ� ����		
	bThreadReady[iUnit]		= false;

	Motor.iRunStep[iUnit] = 10000;
	Task[iUnit].iExhaust = 0;

	for(int i = 0; i < 50; i++)
		SocketFlow.bSocketFlag[iUnit][i] = FALSE;

	ProcLENS[iUnit].OUT_UV_LAMP(UV_OFF);
	 
	vision.SetGrabMode(iUnit, LIVE);
}

bool AxlMotorSet::ManualMode_Process(int iUnit)
{//! [��ü] �Ŵ��� ���� THREADȣ��
	CString sMsg;
	DWORD dwThreadID = 0, dwExitCode = 0;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;

	GetExitCodeThread(m_hThreadManualFocus[iUnit], &dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
		sMsg.Format(_T("Manual MODE ���� PROCESS�� �̹� ���� ���Դϴ�.."));
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
		//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP,(LPARAM)&sMsg);
		return false;
	}

	if( (Motor.m_hThreadHomeAll[iUnit] != NULL) ||  ( Motor.m_hThreadHome[iUnit] != NULL ) )
	{
		sMsg.Format("���������߿� �Ұ����� �����Դϴ�. ");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	if( Motor.m_hThreadReady[iUnit] != NULL)
	{
		sMsg.Format(" �����غ��߿� �Ұ����� �����Դϴ�. " );
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}


	m_bThreadManualFocus[iUnit] = true;

	if(iUnit == UNIT1){
		m_hThreadManualFocus[iUnit] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadManualFocusUNIT1, 
			this, 0, &dwExitCode);
	}
	else
	{
		m_hThreadManualFocus[iUnit] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadManualFocusUNIT2, 
			this, 0, &dwExitCode);
	}
	return true;

}

bool AxlMotorSet::AutoMode_Process(int iUnit)
{//! [��ü] AUTO ���� THREADȣ��
	CString sMsg;
	DWORD dwThreadID = 0, dwExitCode = 0;

	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;
	GetExitCodeThread(m_hThreadAutoFocus[iUnit], &dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
		sMsg.Format(_T("AUTO MODE ���� PROCESS�� �̹� ���� ���Դϴ�.."));
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP);
		//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP,(LPARAM)&sMsg);
		return false;
	}

	if( (Motor.m_hThreadHomeAll[iUnit] != NULL) ||  ( Motor.m_hThreadHome[iUnit] != NULL ) )
	{
		sMsg.Format("���������߿� �Ұ����� �����Դϴ�. ");
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	if( Motor.m_hThreadReady[iUnit] != NULL)
	{
		sMsg.Format(" �����غ��߿� �Ұ����� �����Դϴ�. " );
		g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
		return false;
	}

	//���� �غ� Ȯ��
	bool bReady = false;
	if( Task[iUnit].AutoFlag != 2 ){		//�Ͻ������� �����غ� Ȯ�� ���ϰ� �Ѿ
		if (ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Wait_Pos, AXIS_ALL)){
			if (ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_ALL)){
				if(ProcLENS[iUnit].IN_LensGrip(BACKWARD, false) && ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP, false) &&
					ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD, false))
				{
					bReady = true;
				}
			}
		}

		if( bReady == false)
		{
			sMsg.Format("�����غ� �Ϸ���� �ʾҽ��ϴ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			return false;
		}
	}

	//sMsg.Format("#AA%d@ALARM&CLR$", (sysData.m_iUnitNo*2) + UNIT1 + 1);
	//pFrame->HidataSocket_ClientSend(UNIT1, sMsg);

	m_bThreadAutoFocus[iUnit] = true;

	if(iUnit == UNIT1){
		m_hThreadAutoFocus[iUnit] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadAutoFocusUNIT1, 
			this, 0, &dwExitCode);
	}
	else
	{
		m_hThreadAutoFocus[iUnit] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadAutoFocusUNIT2, 
			this, 0, &dwExitCode);
	}
// 	sMsg.Format("[ THREAD ][ VCM ] AUTO_THREAD ���� ����. [ ID : %d ]", dwThreadID);
// 	::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG+DEF_LOG_OK, (LPARAM)&sMsg);

	return true;

}

//----------------------------��ü ����
int AxlMotorSet::HomeAllProcess_UNIT1(int iUseStep)
{// [ iUnit ] ��ü ���� ����
	int iUnit = UNIT1;

	//double dFTime = 0.0, dFTime2 = 0.0;
	CString sMsg;
	int iThreadStep = 1000, iHomFlag = 0;
	bool bHomeProc, bHomeError;
	DWORD 	dwReturn;
	double dVel, dAcc;
	DWORD dwRet = 0;

	iThreadStep = iUseStep;

	switch(iThreadStep)
	{
	case 1000:  //CL Y   +�������� �̵�
 		dVel = 10 * sysData.fMotorResol[iUnit][MotCL_Y];
 		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);
 
 		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
 
 		if ( dwReturn != AXT_RT_SUCCESS )
 		{
 			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
 			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 			iThreadStep = -1000;
 		}
 		else
 		{
 			dFTime = func_TimeChecker_CurrentTime(true);
 			iThreadStep = 1500;//2000;//1500;
 		}
		break;

	case 1500:// CL Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotCL_Y) &&
			GetPosiSensor(iUnit, MotCL_Y) )
		{
			iThreadStep = 2000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] CL Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

	case 2000:
		if( ProcLENS[iUnit].OUT_LensGrip(BACKWARD) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2500;
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS GRIP ���� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2000;
		}
		break;

	case 2500:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2600;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS GRIP ���� ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

	case 2600:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dFTime))			iThreadStep = 2700;
		break;

		//�Ǹ��� �߰�

	case 2700:
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2800;
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2800:
		if( ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2900;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2900:
		if(::func_TimeChecker_Compare(500, dFTime))			iThreadStep = 3000;
		break;

	case 3000: // Lens_Z �� (+)Limit���� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Z];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Z);

		dwReturn = AxmMoveSignalSearch(MotLENS_Z + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Z]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3500;			
		}
		break;

	case 3500://! Lens Z�� (+)Limit���� ��ȣ Ȯ��
		if( GetStopAxis(iUnit, MotLENS_Z) &&
			GetPosiSensor(iUnit, MotLENS_Z) )
		{
			iThreadStep = 3600;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Z�� (+)LIMIT �̵� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		break;

	case 3600:  //PCB Y   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_Y);

		dwReturn = AxmMoveSignalSearch(MotPCB_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3600;
		}
		else
		{
			dFTime2 = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3700;
		}
		break;

	case 3700:// PCB Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotPCB_Y) &&
			GetNegaSensor(iUnit, MotPCB_Y) )
		{
			iThreadStep = 4000;
		}
		else{
			if(func_TimeChecker_Compare(TIME_AXIS, dFTime2))	
			{
				sMsg.Format("[ ��ü����] PCB Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -3600;
			}
		}
		break;


		//-------------------------------

	case 4000:  //LENS X   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_X];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_X);

		dwReturn = AxmMoveSignalSearch(MotLENS_X  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_X]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 4500;
		}
		break;

	case 4500:// LENS X -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_X) &&
			GetNegaSensor(iUnit, MotLENS_X) )
		{
			iThreadStep = 5000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS X (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}

		break;

	case 5000:  //LENS Y   +�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y);

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 5500;
		}
		break;

	case 5500:  //PCB Y   -�������� �̵�
// 		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_Y];
// 		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_Y);
// 
// 		dwReturn = AxmMoveSignalSearch(MotPCB_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
// 
// 		if ( dwReturn != AXT_RT_SUCCESS )
// 		{
// 			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_Y]);
// 			::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
// 			iThreadStep = -5500;
// 		}
// 		else
// 		{
// 			dFTime2 = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6000;
//		}
		break;

	case 6000:// LENS Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetPosiSensor(iUnit, MotLENS_Y) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6500;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}

		break;
	case 6500:// PCB Y -���� ���� ��ȣ Ȯ��

// 		if( GetStopAxis(iUnit, MotPCB_Y) &&
// 			GetNegaSensor(iUnit, MotPCB_Y) )
// 		{
			iThreadStep = 7000;
// 		}
// 		else{
// 			if(func_TimeChecker_Compare(TIME_AXIS, dFTime2))	
// 			{
// 				sMsg.Format("[ ��ü����] PCB Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
// 				::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
// 				iThreadStep = -5500;
// 			}
// 		}
		break;
//-----------------
	case 6550:  //LENS Y   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y);

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6600;
		}
		break;


	case 6600:// LENS Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetNegaSensor(iUnit, MotLENS_Y) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 7000;//6700;//7000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}

		break;

	//case 6700:  //CL Y   +�������� �̵�
 //		dVel = 30 * sysData.fMotorResol[iUnit][MotCL_Y];
 //		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);
 //
 //		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
 //
 //		if ( dwReturn != AXT_RT_SUCCESS )
 //		{
 //			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
 //			::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 //			iThreadStep = -6700;
 //		}
 //		else
 //		{
 //			dFTime = func_TimeChecker_CurrentTime(true);
 //			iThreadStep = 6800;
 //		}
	//	break;

	//case 6800:// CL Y +���� ���� ��ȣ Ȯ��

	//	if( GetStopAxis(iUnit, MotCL_Y) &&
	//		GetPosiSensor(iUnit, MotCL_Y) )
	//	{
	//		iThreadStep = 7000;
	//	}
	//	else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))	
	//	{
	//		sMsg.Format("[ ��ü����] CL Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
	//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
	//		iThreadStep = -6700;
	//	}
	//	break;

//-------

	case 7000:		//PCB T -�������� ������.
		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_T];
		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_T);

		dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_T]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -7000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 7500;
		}

		break;

	case 7500:		//PCB T ���� Ȯ��.
		if( GetStopAxis(iUnit, MotPCB_T) &&
			GetNegaSensor(iUnit, MotPCB_T) )
		{
			iThreadStep = 8000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] PCB T (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -6000;
		}
		break;

	case 8000:	//��ü ���� �����˻�
		bHomeProc = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			//if(iAxis == MotCL_Y)		continue;
			if( iHomeProcUsing[iUnit][iAxis] == 1 )
			{
				if( Start_Home_Search(iUnit, iAxis) < 0 )
				{
					bHomeProc = false;
					break;
				}
				else
				{
					iHomeProcUsing[iUnit][iAxis] = 2;
				}
			}
		}

		if( bHomeProc == false )
		{
			sMsg.Format("[ ��ü ���� ] ���� ���� ���� ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -8000;
		}
		else
		{
			iThreadStep = 9000;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		break;

	case 9000:	//��ü ���� ���� ����.
		//if(func_TimeChecker_Compare(30000, dFTime) == false)
		if(func_TimeChecker_Compare(100000, dFTime) == false)
		{
			bHomeProc = true;
			bHomeError = true;

			for( int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				//if(iAxis == MotCL_Y) continue;
				if( iHomeProcUsing[iUnit][iAxis] != 0 )
				{
					iHomFlag = IsCompleted_Home_Search(iUnit, iAxis);
					if( iHomFlag < 1 )	bHomeError = false;		//���� ���� Error ���� �� ���
					if( iHomFlag == 2 )	bHomeProc = false;		//���� ���� ���� ���� ���.
					if( iHomFlag == 1 )	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_COMPLETE;	//���� �Ϸ�
				}


				if( bHomeError == false )	
				{//������ �˶�(���� ���� ����) 
					sMsg.Format("[ ��ü���� ] ���� ���� Error �߻� ����. �� : %s",  MOTOR_NAME[iAxis]);
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
					iThreadStep = -9000;

					for( int iAxis2 = 0; iAxis2 < CUR_MOTOR_NO; iAxis2++)
					{	
						AxmMoveSStop(iAxis2 + (iUnit * CUR_MOTOR_NO));
					}

					break;
				}
			}


			if( bHomeError == true )
			{
				bool bChk_origin = true;
				for(int iChk = 0; iChk < CUR_MOTOR_NO; iChk++)
				{
					if(m_bOrgFlag[iUnit][iChk] != DEF_ORIGIN_COMPLETE)				bChk_origin = false;

				}

				if(bChk_origin == true)
				{
					StartingPoint_Status[iUnit] = true;
					sMsg.Format("[ ��ü ���� ] ���� ���� ������ �Ϸ�Ǿ����ϴ�.");
					g_PushLogQueue(sMsg, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit));
					//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

					iThreadStep = 9500;
				}
			}

		}
		else
		{
			sMsg.Format("[ ��ü ���� ] ���� ���� �ð� �ʰ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

			iThreadStep = -9000;
		}

		break;

	case 9500:	//����
		iThreadStep = 10000;
		break;

	default:
		iThreadStep = -10000;

		break;
	}
	return iThreadStep;
}


int AxlMotorSet::HomeAllProcess_UNIT2(int iUseStep)
{// [ iUnit ] ��ü ���� ����
	int iUnit = UNIT2;

	//double dFTime = 0.0, dFTime2 = 0.0;
	CString sMsg;
	int iThreadStep = 1000, iHomFlag = 0;
	bool bHomeProc, bHomeError;
	DWORD 	dwReturn;
	double dVel, dAcc;
	DWORD dwRet = 0;

	iThreadStep = iUseStep;

	switch(iThreadStep)
	{
	case 1000:  //CL Y   +�������� �̵�
 		dVel = 10 * sysData.fMotorResol[iUnit][MotCL_Y];
 		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);
 
 		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
 
 		if ( dwReturn != AXT_RT_SUCCESS )
 		{
 			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
 			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 			iThreadStep = -1000;
 		}
 		else
 		{
 			dFTime = func_TimeChecker_CurrentTime(true);
 			iThreadStep = 1500;//2000;//1500;
 		}
		break;

	case 1500:// CL Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotCL_Y) &&
			GetPosiSensor(iUnit, MotCL_Y) )
		{
			iThreadStep = 2000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] CL Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

	case 2000:
		if( ProcLENS[iUnit].OUT_LensGrip(BACKWARD) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2500;
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS GRIP ���� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2000;
		}
		break;

	case 2500:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2600;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS GRIP ���� ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -1000;
		}
		break;

	case 2600:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dFTime))			iThreadStep = 2700;
		break;

		//�Ǹ��� �߰�

	case 2700:
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2800;
		}
		else
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2800:
		if( ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 2900;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))
		{
			sMsg.Format("[ ��ü����] LENS PUSHER ��� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -2700;
		}
		break;

	case 2900:
		if(::func_TimeChecker_Compare(500, dFTime))			iThreadStep = 3000;
		break;

	case 3000: // Lens_Z �� (+)Limit���� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Z];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Z);

		dwReturn = AxmMoveSignalSearch(MotLENS_Z + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Z]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3500;			
		}
		break;

	case 3500://! Lens Z�� (+)Limit���� ��ȣ Ȯ��
		if( GetStopAxis(iUnit, MotLENS_Z) &&
			GetPosiSensor(iUnit, MotLENS_Z) )
		{
			iThreadStep = 3600;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Z�� (+)LIMIT �̵� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3000;
		}
		break;

	case 3600:  //PCB Y   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_Y);

		dwReturn = AxmMoveSignalSearch(MotPCB_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -3600;
		}
		else
		{
			dFTime2 = func_TimeChecker_CurrentTime(true);
			iThreadStep = 3700;
		}
		break;

	case 3700:// PCB Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotPCB_Y) &&
			GetNegaSensor(iUnit, MotPCB_Y) )
		{
			iThreadStep = 4000;
		}
		else{
			if(func_TimeChecker_Compare(TIME_AXIS, dFTime2))	
			{
				sMsg.Format("[ ��ü����] PCB Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -3600;
			}
		}
		break;


		//-------------------------------

	case 4000:  //LENS X   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_X];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_X);

		dwReturn = AxmMoveSignalSearch(MotLENS_X  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_X]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 4500;
		}
		break;

	case 4500:// LENS X -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_X) &&
			GetNegaSensor(iUnit, MotLENS_X) )
		{
			iThreadStep = 5000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS X (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -4000;
		}

		break;

	case 5000:  //LENS Y   +�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y);

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 5500;
		}
		break;

	case 5500:  //PCB Y   -�������� �̵�
// 		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_Y];
// 		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_Y);
// 
// 		dwReturn = AxmMoveSignalSearch(MotPCB_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
// 
// 		if ( dwReturn != AXT_RT_SUCCESS )
// 		{
// 			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_Y]);
// 			::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
// 			iThreadStep = -5500;
// 		}
// 		else
// 		{
// 			dFTime2 = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6000;
//		}
		break;

	case 6000:// LENS Y +���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetPosiSensor(iUnit, MotLENS_Y) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6500;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}

		break;
	case 6500:// PCB Y -���� ���� ��ȣ Ȯ��

// 		if( GetStopAxis(iUnit, MotPCB_Y) &&
// 			GetNegaSensor(iUnit, MotPCB_Y) )
// 		{
			iThreadStep = 7000;
// 		}
// 		else{
// 			if(func_TimeChecker_Compare(TIME_AXIS, dFTime2))	
// 			{
// 				sMsg.Format("[ ��ü����] PCB Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
// 				::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
// 				iThreadStep = -5500;
// 			}
// 		}
		break;
//-----------------
	case 6550:  //LENS Y   -�������� �̵�
		dVel = 10 * sysData.fMotorResol[iUnit][MotLENS_Y];
		dAcc = Motor.CalcAcc_G(iUnit,MotLENS_Y);

		dwReturn = AxmMoveSignalSearch(MotLENS_Y  + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotLENS_Y]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 6600;
		}
		break;


	case 6600:// LENS Y -���� ���� ��ȣ Ȯ��

		if( GetStopAxis(iUnit, MotLENS_Y) &&
			GetNegaSensor(iUnit, MotLENS_Y) )
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 7000;//6700;//7000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] LENS Y (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -5000;
		}

		break;

	//case 6700:  //CL Y   +�������� �̵�
 //		dVel = 30 * sysData.fMotorResol[iUnit][MotCL_Y];
 //		dAcc = Motor.CalcAcc_G(iUnit,MotCL_Y);
 //
 //		dwReturn = AxmMoveSignalSearch(MotCL_Y  + (CUR_MOTOR_NO * iUnit), dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
 //
 //		if ( dwReturn != AXT_RT_SUCCESS )
 //		{
 //			sMsg.Format("[ ��ü����] %s (+) Limit �̵� ���� ����.", MOTOR_NAME[MotCL_Y]);
 //			::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 //			iThreadStep = -6700;
 //		}
 //		else
 //		{
 //			dFTime = func_TimeChecker_CurrentTime(true);
 //			iThreadStep = 6800;
 //		}
	//	break;

	//case 6800:// CL Y +���� ���� ��ȣ Ȯ��

	//	if( GetStopAxis(iUnit, MotCL_Y) &&
	//		GetPosiSensor(iUnit, MotCL_Y) )
	//	{
	//		iThreadStep = 7000;
	//	}
	//	else if(::func_TimeChecker_Compare(TIME_AXIS, dFTime))	
	//	{
	//		sMsg.Format("[ ��ü����] CL Y (+) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
	//		::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
	//		iThreadStep = -6700;
	//	}
	//	break;

//-------

	case 7000:		//PCB T -�������� ������.
		dVel = 10 * sysData.fMotorResol[iUnit][MotPCB_T];
		dAcc = Motor.CalcAcc_G(iUnit,MotPCB_T);

		dwReturn = AxmMoveSignalSearch(MotPCB_T + (CUR_MOTOR_NO * iUnit), -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);

		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sMsg.Format("[ ��ü����] %s (-) Limit �̵� ���� ����.", MOTOR_NAME[MotPCB_T]);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -7000;
		}
		else
		{
			dFTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 7500;
		}

		break;

	case 7500:		//PCB T ���� Ȯ��.
		if( GetStopAxis(iUnit, MotPCB_T) &&
			GetNegaSensor(iUnit, MotPCB_T) )
		{
			iThreadStep = 8000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dFTime))	
		{
			sMsg.Format("[ ��ü����] PCB T (-) LIMIT ���� ��ȣ Ȯ�� �ð� �ʰ� [ TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -6000;
		}
		break;

	case 8000:	//��ü ���� �����˻�
		bHomeProc = true;
		for(int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
		{
			//if(iAxis == MotCL_Y)		continue;
			if( iHomeProcUsing[iUnit][iAxis] == 1 )
			{
				if( Start_Home_Search(iUnit, iAxis) < 0 )
				{
					bHomeProc = false;
					break;
				}
				else
				{
					iHomeProcUsing[iUnit][iAxis] = 2;
				}
			}
		}

		if( bHomeProc == false )
		{
			sMsg.Format("[ ��ü ���� ] ���� ���� ���� ����.");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -8000;
		}
		else
		{
			iThreadStep = 9000;
			dFTime = func_TimeChecker_CurrentTime(true);
		}
		break;

	case 9000:	//��ü ���� ���� ����.
		//if(func_TimeChecker_Compare(30000, dFTime) == false)
		if(func_TimeChecker_Compare(100000, dFTime) == false)
		{
			bHomeProc = true;
			bHomeError = true;

			for( int iAxis = 0; iAxis < CUR_MOTOR_NO; iAxis++)
			{
				//if(iAxis == MotCL_Y) continue;
				if( iHomeProcUsing[iUnit][iAxis] != 0 )
				{
					iHomFlag = IsCompleted_Home_Search(iUnit, iAxis);
					if( iHomFlag < 1 )	bHomeError = false;		//���� ���� Error ���� �� ���
					if( iHomFlag == 2 )	bHomeProc = false;		//���� ���� ���� ���� ���.
					if( iHomFlag == 1 )	m_bOrgFlag[iUnit][iAxis] = DEF_ORIGIN_COMPLETE;	//���� �Ϸ�
				}


				if( bHomeError == false )	
				{//������ �˶�(���� ���� ����) 
					sMsg.Format("[ ��ü���� ] ���� ���� Error �߻� ����. �� : %s",  MOTOR_NAME[iAxis]);
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
					iThreadStep = -9000;

					for( int iAxis2 = 0; iAxis2 < CUR_MOTOR_NO; iAxis2++)
					{	
						AxmMoveSStop(iAxis2 + (iUnit * CUR_MOTOR_NO));
					}

					break;
				}
			}


			if( bHomeError == true )
			{
				bool bChk_origin = true;
				for(int iChk = 0; iChk < CUR_MOTOR_NO; iChk++)
				{
					if(m_bOrgFlag[iUnit][iChk] != DEF_ORIGIN_COMPLETE)				bChk_origin = false;

				}

				if(bChk_origin == true)
				{
					StartingPoint_Status[iUnit] = true;
					sMsg.Format("[ ��ü ���� ] ���� ���� ������ �Ϸ�Ǿ����ϴ�.");
					g_PushLogQueue(sMsg, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit));
					//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

					iThreadStep = 9500;
				}
			}

		}
		else
		{
			sMsg.Format("[ ��ü ���� ] ���� ���� �ð� �ʰ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg,WM_SEND_LOG, DEF_TYPE_SYSTEMLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

			iThreadStep = -9000;
		}

		break;

	case 9500:	//����
		iThreadStep = 10000;
		break;

	default:
		iThreadStep = -10000;

		break;
	}
	return iThreadStep;

}

//----------------�� �� �� ��
int AxlMotorSet::ReadyAllProcess_UNIT1(int iUseStep)
{
	int iUnit = UNIT1;
	CString sMsg;
	int iThreadStep = 0, iHomFlag = 0;
	/////////////////////////////////////////////////////////////		20160629
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;
	//////////////////////////////////////////////////////////
	DWORD dwRet = 0;
	
	iThreadStep = iUseStep;
	switch(iThreadStep)
	{//!! �����غ�
	case 10000:
	//	gMiu.PRIGrabber_Open(UNIT1);			//20160705  JJC �����غ� ������ �÷��� �׷��� ����
		dTime = func_TimeChecker_CurrentTime(true);
		iSocketCntChk[iUnit] = 0;
		iThreadStep = 10001;
		break;
	/////////////////////////////////////////////////////////����üũ/////////////////////////////////////
	case 10001://case 12000:	//20160629 JJC
		if(SocketFlow.bSocketFlag[iUnit][LEN_ITF_ACK] == TRUE)
		{
			SocketFlow.bSocketFlag[iUnit][LEN_ITF_ACK] = FALSE;
			bITF_CHK[iUnit] = false;
			iThreadStep = 10050;
			break;
		}
		else
		{
			if(bITF_CHK[iUnit] == false)
			{
				bITF_CHK[iUnit] = true;
				sMsg.Format("#AA%d@LEN_ITF&CHK$", (sysData.m_iUnitNo*2)+iUnit+1);
				pFrame->HidataSocket_ClientSend(iUnit, sMsg);

				sMsg.Format("[SOCKET_LOADING] LENS ���� üũ ��û. [ Step : %d ]", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

				dTime = func_TimeChecker_CurrentTime(true);
			}
			else if( func_TimeChecker_Compare(1000, dTime) )
			{
				iSocketCntChk[iUnit]++;
				bITF_CHK[iUnit] = false;

				if( iSocketCntChk[iUnit] > 10 )
				{
					sMsg.Format("�����غ� �� MAIN ���� Ȯ�� ���� �ð� �ʰ�..\n��� ���¸� Ȯ�� �ϼ���.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
					//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);	
					iThreadStep = -10000;
				}
			}
			break;
		}
		break;
		/////////////////////////////////////////////////////////����üũ/////////////////////////////////////

		/////////////////////////////////////////////////////////UV ����üũ//////////////////////////////////
	case 10050:
		if(sysData.m_iPassUV[UNIT1] == 1)
		{
			iThreadStep = 10100;
				break;
		}
		#ifdef USE_RS232_UV		
			gCommUV.UV_Trigger(UNIT1 , 2 , DIO_OFF , model.iUVLightVal[UNIT1]);		//�ѹ� ���� ��Ű�� 20160314 JJC
			
			
			//if(gCommUV.iRecvFlag[UNIT1] == UV_ACK)
			if(UV_Connect[UNIT1] == false)
			{
				sMsg.Format("�����غ� �� UV ��Ʈ�ѷ� �������¸� Ȯ�����ּ���.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT1), (LPARAM)&sMsg);	
				iThreadStep = -10000;
				break;
			}
			else
			{
				iThreadStep = 10100;
				break;
			}
		#else			//��� RS232 ��� UV �Ⱦ� ��
			iThreadStep = 10100;
			break;
		#endif


		/////////////////////////////////////////////////////////UV ����üũ//////////////////////////////////
	case 10100:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13100;
		}
		else
		{
			if( ProcLENS[iUnit].OUT_LensGrip(BACKWARD))
			{
				dTime = func_TimeChecker_CurrentTime(true);	
				iThreadStep = 13000;
			}
			else
			{
				sMsg.Format("[ �����غ� ] LENS GRIP ���� ��� ����");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -12000;
			}
		}
		break;

	case 13000:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13100;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ]LENS GRIP ���� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -12000;
		}
		break;

	case 13100:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dTime))
		{
			iThreadStep = 13500;
		}
		break;

	case 13500://! Pusher ��� ���
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13700;
		}
		else
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� ���� ");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -13500;
		}
		break;

	case 13700:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13800;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -13500;
		}
		break;

	case 13800:
		if(func_TimeChecker_Compare(1500, dTime))
		{
			iThreadStep = 14000;
		}
		break;

	case 14000://! Lens-Z�� �����ġ �̵�
		if(ProcLENS[iUnit].MotionMove_Lens_Teaching(Wait_Pos, AXIS_Z))	//! Z�� �����ġ �̵� ���
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 15000;
		}
		else
		{
			sMsg.Format("[ �����غ� ] LENS_Z�� �����ġ �̵� ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -14000;
			break;
		}
		break;

	case 15000:
		if( ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_Z) )	
		{
			iThreadStep = 16000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] Z�� �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -14000;
		}
		break;


	case 16000: //! LENS Z���� �� ���� �����ġ �̵�.. X,Y,Tx,Ty
		if(ProcLENS[iUnit].MotionMove_Lens_Teaching(Wait_Pos, AXIS_XYT))  
		{
			dTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 16500;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] LENS�� �����ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -16000;	
		}
		break;

	case 16500:
		if(ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_Z))
			iThreadStep = 17000;
		break;

	case 17000:	//! PCB X, Y, T ���Ͷ� ��ġ �̵�
		////////////////////////////////////////////////////////////////////////////////////////20160717 JJC		�����̱��� ����Ȯ��

		if(!ProcLENS[iUnit].IN_Lens_Centering(FORWARD) && !ProcLENS[iUnit].IN_Lens_Centering(BACKWARD))
		{
			sMsg.Format("Centering ���� ��ȣ �̻�! \n Centering ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher_FB(FORWARD) && !ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))
		{
			sMsg.Format("Pusher ������ ���� ��ȣ �̻�! \n Pusher ������ ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) && !ProcLENS[iUnit].IN_Lens_Pusher(CYL_DOWN))
		{
			sMsg.Format("Pusher ���, �ϰ� ���� ��ȣ �̻�! \n Pusher ���, �ϰ� ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_LensGrip(FORWARD) && !ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			sMsg.Format("Grip ���� ��ȣ �̻�! \n Grip ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		//////////////////////////////////////////////////////////////
		if( ProcPCB[iUnit].MotionMove_Pcb_Teaching(Interlock_Pos, AXIS_ALL) )
		{
			dSTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 17100;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] PCB�� ���Ͷ� ��ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;	
		}
		break;
	
	case 17100:
		if( ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Interlock_Pos, AXIS_ALL) )
		{
			iThreadStep = 18000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] PCB�� ���Ͷ� ��ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
		}
		break;
	

	case 18000:
		bReadyPusher[iUnit] = true;
		if(ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD) && ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 23000;
		}
		else
		{
			if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
			{
				iThreadStep = 19000;
				dTime = func_TimeChecker_CurrentTime(true);
			}
			else
			{
				sMsg.Format("[ ���� �غ� ] LENS PUSHER ��� ����. [STEP:%d].", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -18000;	
			}
		}
		break;

	case 19000:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))		//��� Ȯ��
		{
			iThreadStep = 20000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -18000;
		}
		break;

	case 20000:
		if(bReadyPusher[iUnit])
		{	
			if(ProcLENS[iUnit].OUT_Lens_Pusher_FB(BACKWARD))
			{
				dTime = func_TimeChecker_CurrentTime(true);
				iThreadStep = 21000;
			}
			else
			{
				sMsg.Format("[ ���� �غ� ] LENS PUSHER ���� ����. [STEP:%d].", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -20000;	
			}
		}	
		break;

	case 21000://! Pusher ���� Ȯ��
		if(ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))	
		{
			iThreadStep = 23000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ���� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -20000;
		}
		break;

	case 23000://! PCB�� ��� ��ġ �̵�
		////////////////////////////////////////////////////////////////////////////////////////20160717 JJC		�����̱��� ����Ȯ��

		if(!ProcLENS[iUnit].IN_Lens_Centering(FORWARD) && !ProcLENS[iUnit].IN_Lens_Centering(BACKWARD))
		{
			sMsg.Format("Centering ���� ��ȣ �̻�! \n Centering ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher_FB(FORWARD) && !ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))
		{
			sMsg.Format("Pusher ������ ���� ��ȣ �̻�! \n Pusher ������ ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) && !ProcLENS[iUnit].IN_Lens_Pusher(CYL_DOWN))
		{
			sMsg.Format("Pusher ���, �ϰ� ���� ��ȣ �̻�! \n Pusher ���, �ϰ� ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_LensGrip(FORWARD) && !ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			sMsg.Format("Grip ���� ��ȣ �̻�! \n Grip ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}

		//////////////////////////////////////////////////////////////
		if( ProcPCB[iUnit].MotionMove_Pcb_Teaching(Wait_Pos, AXIS_ALL) )
		{
			dSTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 24000;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] PCB�� ��� ��ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;	
		}
		break;

	case 24000:
		if(ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Wait_Pos, AXIS_ALL)
			&& ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_ALL))
		{
			/*
			sMsg.Format("[ ���� �غ� ] ���� �غ� �Ϸ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
			Task[iUnit].bRunningReady = true;
			iThreadStep = 99999;
			*/
			iThreadStep = 24500;
		}
		else if(!ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Wait_Pos, AXIS_ALL))
		{
			if(func_TimeChecker_Compare(TIME_AXIS, dSTime))
			{
			sMsg.Format("[ �����غ� ] PCB �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			}

		}
		else if(!ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_ALL))
		{
			if(func_TimeChecker_Compare(TIME_AXIS, dTime))
			{
			sMsg.Format("[ �����غ� ] LENS �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -16000;
			}
		}
		
		break;

	case 24500:	//CL Y�� ����
 		if( ProcCL[iUnit].MotionMove_CL_Teaching(Wait_Pos))
 		{
			dTime = func_TimeChecker_CurrentTime(true);		
			iThreadStep = 24600;
 		}
 		else
 		{
 			sMsg.Format("[ �����غ� ] CL_Y�� �����ġ �̵� ��� ����");
 			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 			iThreadStep = -24500;
 		}
		break;

	case 24600:
		if( ProcCL[iUnit].MoveCompleted_CL_Teaching(Wait_Pos))
		{
			sMsg.Format("[ ���� �غ� ] ���� �غ� �Ϸ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
			Task[iUnit].bRunningReady = true;
			iThreadStep = 99999;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] CL_Y�� �����ġ �̵� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -24500;
		}
		break;

	default:
		iThreadStep = -10000;
		break;
	}
	
	return iThreadStep;
}

int AxlMotorSet::ReadyAllProcess_UNIT2(int iUseStep)
{
	int iUnit = UNIT2;
	CCPreFocusingAADlg* pFrame = (CCPreFocusingAADlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	int iThreadStep = 0, iHomFlag = 0;
	DWORD dwRet = 0;

	iThreadStep = iUseStep;
	switch(iThreadStep)
	{//!! �����غ�
	case 10000:
	//	gMiu.PRIGrabber_Open(UNIT2);			//20160705  JJC �����غ� ������ �÷��� �׷��� ����
		dTime = func_TimeChecker_CurrentTime(true);
		iSocketCntChk[iUnit]	= 0;
		iThreadStep				= 10001;
		break;

	case 10001://case 12000:	//20160629 JJC
		if(SocketFlow.bSocketFlag[iUnit][LEN_ITF_ACK] == TRUE)
		{
			SocketFlow.bSocketFlag[iUnit][LEN_ITF_ACK] = FALSE;
			bITF_CHK[iUnit] = false;
			iThreadStep = 10050;
			break;
		}
		else
		{
			if(bITF_CHK[iUnit] == false)
			{
				bITF_CHK[iUnit] = true;
				sMsg.Format("#AA%d@LEN_ITF&CHK$", (sysData.m_iUnitNo*2)+iUnit+1);
				pFrame->HidataSocket_ClientSend(iUnit, sMsg);

				sMsg.Format("[SOCKET_LOADING] LENS ���� üũ ��û. [ Step : %d ]", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG + GetUnitNum(iUnit));
			//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG + GetUnitNum(iUnit), (LPARAM)&sMsg);

				dTime = func_TimeChecker_CurrentTime(true);
			}
			else if( func_TimeChecker_Compare(1000, dTime) )
			{
				iSocketCntChk[iUnit]++;
				bITF_CHK[iUnit] = false;

				if( iSocketCntChk[iUnit] > 10 )
				{
					sMsg.Format("�����غ� �� MAIN ���� Ȯ�� ���� �ð� �ʰ�..\n��� ���¸� Ȯ�� �ϼ���.");
					g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
					//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);	
					iThreadStep = -10000;
				}
			}
			break;
		}
		break;

		/////////////////////////////////////////////////////////UV ����üũ//////////////////////////////////
	case 10050:
		if(sysData.m_iPassUV[UNIT2] == 1)
		{
			iThreadStep = 10100;
				break;
		}
		#ifdef USE_RS232_UV		
			gCommUV.UV_Trigger(UNIT2 , 2 , DIO_OFF , model.iUVLightVal[UNIT2]);		//�ѹ� ���� ��Ű�� 20160314 JJC
			
			//
			/*if(gCommUV.iRecvFlag[UNIT2] == UV_ACK)*/
			if(UV_Connect[UNIT2] == false)	
			{
				sMsg.Format("�����غ� �� UV ��Ʈ�ѷ� �������¸� Ȯ�����ּ���.");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG + DEF_LOG_POPUP + GetUnitNum(UNIT2), (LPARAM)&sMsg);	
				iThreadStep = -10000;
				break;
			}
			else
			{
				iThreadStep = 10100;
				break;
			}
		#else			//��� RS232 ��� UV �Ⱦ� ��
			iThreadStep = 10100;
			break;
		#endif


		/////////////////////////////////////////////////////////UV ����üũ//////////////////////////////////

	case 10100://case 12000:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			iThreadStep = 13100;
			dTime = func_TimeChecker_CurrentTime(true);	
		}
		else
		{
			if( ProcLENS[iUnit].OUT_LensGrip(BACKWARD))
			{
				iThreadStep = 13000;
				dTime = func_TimeChecker_CurrentTime(true);	
			}
			else
			{
				sMsg.Format("[ �����غ� ] LENS GRIP ���� ��� ����");
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -12000;
			}
		}
		break;

	case 13000:
		if( ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13100;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ]LENS GRIP ���� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -12000;
		}
		break;

	case 13100:
		if(::func_TimeChecker_Compare(model.SilinderDelay[iUnit][DELAY_GRIP], dTime))
		{
			iThreadStep = 13500;
		}
		break;

	case 13500:
		if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13700;
		}
		else
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� ���� ");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -13500;
		}
		break;

	case 13700:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 13800;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -13500;
		}
		break;

	case 13800:
		if(func_TimeChecker_Compare(1500, dTime))
		{
			iThreadStep = 14000;
		}
		break;

	case 14000://! Lens-Z�� �����ġ �̵�
		if(ProcLENS[iUnit].MotionMove_Lens_Teaching(Wait_Pos, AXIS_Z))	//! Z�� �����ġ �̵� ���
		{
			dTime = func_TimeChecker_CurrentTime(true);	
			iThreadStep = 15000;
		}
		else
		{
			sMsg.Format("[ �����غ� ] LENS_Z�� �����ġ �̵� ��� ����");
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -14000;
			break;
		}
		break;

	case 15000:
		if(ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_Z))	//! Z�� �����ġ �̵� Ȯ��
		{
			iThreadStep = 16000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] Z�� �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -14000;
		}
		break;


	case 16000: //! LENS Z���� �� ���� �����ġ �̵�.. X,Y,Tx,Ty 
		if(ProcLENS[iUnit].MotionMove_Lens_Teaching(Wait_Pos, AXIS_XYT))  
		{
			dTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 16500;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] LENS�� �����ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -16000;	
		}
		break;

	case 16500:
		if(ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_Z))
			iThreadStep = 17000;
		break;


	case 17000:		//! PCB X, Y, T �����ġ �̵�
		////////////////////////////////////////////////////////////////////////////////////////20160717 JJC		�����̱��� ����Ȯ��

		if(!ProcLENS[iUnit].IN_Lens_Centering(FORWARD) && !ProcLENS[iUnit].IN_Lens_Centering(BACKWARD))
		{
			sMsg.Format("Centering ���� ��ȣ �̻�! \n Centering ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher_FB(FORWARD) && !ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))
		{
			sMsg.Format("Pusher ������ ���� ��ȣ �̻�! \n Pusher ������ ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) && !ProcLENS[iUnit].IN_Lens_Pusher(CYL_DOWN))
		{
			sMsg.Format("Pusher ���, �ϰ� ���� ��ȣ �̻�! \n Pusher ���, �ϰ� ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_LensGrip(FORWARD) && !ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			sMsg.Format("Grip ���� ��ȣ �̻�! \n Grip ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			break;
		}

		//////////////////////////////////////////////////////////////
		if(ProcPCB[iUnit].MotionMove_Pcb_Teaching(Interlock_Pos, AXIS_ALL))
		{
			dSTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 17100;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] PCB�� ���Ͷ� ��ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;	
		}
		break;

	case 17100:
		if( ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Interlock_Pos, AXIS_ALL) )
		{
			iThreadStep = 18000;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] PCB�� ���Ͷ� ��ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
		}
		break;

	case 18000:
		bReadyPusher[iUnit] = true;
		if(ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD) && ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))
		{
			iThreadStep = 23000;
		}
		else
		{
			if(ProcLENS[iUnit].OUT_Lens_Pusher(CYL_UP))
			{
				dTime = func_TimeChecker_CurrentTime(true);
				iThreadStep = 19000;
			}
			else
			{
				sMsg.Format("[ ���� �غ� ] LENS PUSHER ��� ����. [STEP:%d].", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -18000;	
			}
		}
		break;

	case 19000:
		if(ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP))		//��� Ȯ��
		{
			iThreadStep = 20000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ��� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -18000;
		}
		break;

	case 20000:
		if(bReadyPusher[iUnit])	
		{	
			if(ProcLENS[iUnit].OUT_Lens_Pusher_FB(BACKWARD))	// ���� �̵� ��� (�۾��� ��)
			{
				iThreadStep = 21000;
				dTime = func_TimeChecker_CurrentTime(true);
			}
			else
			{
				sMsg.Format("[ ���� �غ� ] LENS PUSHER ���� ����. [STEP:%d].", iThreadStep);
				g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
				iThreadStep = -20000;	
			}
		}
		break;

	case 21000://! Pusher ���� Ȯ��
		if(ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))	//���� Ȯ��
		{
			iThreadStep = 23000;
		}
		else if(::func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] LENS PUSHER ���� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -20000;
		}
		break;

	case 23000://! PCB�� ��� ��ġ �̵�
		////////////////////////////////////////////////////////////////////////////////////////20160717 JJC		�����̱��� ����Ȯ��

		if(!ProcLENS[iUnit].IN_Lens_Centering(FORWARD) && !ProcLENS[iUnit].IN_Lens_Centering(BACKWARD))
		{
			sMsg.Format("Centering ���� ��ȣ �̻�! \n Centering ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher_FB(FORWARD) && !ProcLENS[iUnit].IN_Lens_Pusher_FB(BACKWARD))
		{
			sMsg.Format("Pusher ������ ���� ��ȣ �̻�! \n Pusher ������ ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_Lens_Pusher(CYL_UP) && !ProcLENS[iUnit].IN_Lens_Pusher(CYL_DOWN))
		{
			sMsg.Format("Pusher ���, �ϰ� ���� ��ȣ �̻�! \n Pusher ���, �ϰ� ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}
		else if(!ProcLENS[iUnit].IN_LensGrip(FORWARD) && !ProcLENS[iUnit].IN_LensGrip(BACKWARD))
		{
			sMsg.Format("Grip ���� ��ȣ �̻�! \n Grip ���� ��ȣ�� Ȯ�����ּ���.  [ STEP : %d ]", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;
			break;
		}

		//////////////////////////////////////////////////////////////
		if( ProcPCB[iUnit].MotionMove_Pcb_Teaching(Wait_Pos, AXIS_ALL) )
		{
			dSTime = func_TimeChecker_CurrentTime(true);
			iThreadStep = 24000;
		}
		else
		{
			sMsg.Format("[ ���� �غ� ] PCB�� ��� ��ġ �̵� ����. [STEP:%d].", iThreadStep);
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -23000;	
		}
		break;

	case 24000:
		if(ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Wait_Pos, AXIS_ALL)
			&& ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_ALL))
		{
			/*
			sMsg.Format("[ ���� �غ� ] ���� �غ� �Ϸ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
			Task[iUnit].bRunningReady = true;
			iThreadStep = 99999;
			*/

			iThreadStep = 24500;
		}
		else if(!ProcPCB[iUnit].MoveCompleted_Pcb_Teaching(Wait_Pos, AXIS_ALL))
		{
			if(func_TimeChecker_Compare(TIME_AXIS, dSTime))
			{
			sMsg.Format("[ �����غ� ] PCB �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -17000;
			}

		}
		else if(!ProcLENS[iUnit].MoveCompleted_Lens_Teaching(Wait_Pos, AXIS_ALL))
		{
			if(func_TimeChecker_Compare(TIME_AXIS, dTime))
			{
			sMsg.Format("[ �����غ� ] LENS �����ġ �̵� �ð� �ʰ�  [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -16000;
			}
		}
		
		break;

	case 24500:	//CL Y�� ����
 		if( ProcCL[iUnit].MotionMove_CL_Teaching(Wait_Pos))
 		{
			dTime = func_TimeChecker_CurrentTime(true);		
			iThreadStep = 24600;
 		}
 		else
 		{
 			sMsg.Format("[ �����غ� ] CL_Y�� �����ġ �̵� ��� ����");
 			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
 			iThreadStep = -24500;
 		}
		break;

	case 24600:
		if( ProcCL[iUnit].MoveCompleted_CL_Teaching(Wait_Pos))
		{
			sMsg.Format("[ ���� �غ� ] ���� �غ� �Ϸ�.");
			g_PushLogQueue(sMsg, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_MOTIONLOG  + GetUnitNum(iUnit), (LPARAM)&sMsg);
			Task[iUnit].bRunningReady = true;
			iThreadStep = 99999;
		}
		else if(func_TimeChecker_Compare(TIME_AXIS, dTime))
		{
			sMsg.Format("[ �����غ� ] CL_Y�� �����ġ �̵� �ð� �ʰ� [TIME : %d (s)]", (TIME_AXIS / 1000));
			g_PushLogQueue(sMsg, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit));
				//::SendMessage(hMainDlg, WM_SEND_LOG, DEF_TYPE_ERRORLOG  + DEF_LOG_POPUP + GetUnitNum(iUnit), (LPARAM)&sMsg);
			iThreadStep = -24500;
		}
		break;

	default:
		iThreadStep = -10000;
		break;
	}

	return iThreadStep;
}
 
