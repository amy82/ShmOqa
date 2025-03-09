#include "stdafx.h"
#include "AxlMotorSet.h"


//-----------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------
CAxlMotorSet::CAxlMotorSet()
{
	int i;
	int iUnit = 0;

	m_bAxlInitResult = false;
	m_bOqaOrgFlag = false;
	for(iUnit = 0; iUnit < MAX_UNIT_COUNT; iUnit++)
	{
		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			m_bOrgFlag[iUnit][i] = false;	//���� ���� �ʱ�ȭ
			m_nHomeProcUsing[iUnit][i] = 0;	//���� ���� �䱸 ���� �ʱ�ȭ

			m_bJogPlusFlag[iUnit][i] = false;
			m_bJogMinusFlag[iUnit][i] = false;
			m_bOldJogPlusFlag[iUnit][i] = false;	//! ���� ���� ���� (+)Jog ����
			m_bOldJogMinusFlag[iUnit][i] = false;	//! ���� ���� ���� (-)Jog ����
		}
	}
	
	//! Motor ���� ���� ���� ������ ���� ��� (Motion Controller Library���� �����ϴ� ���)
#ifdef _M_X64
	m_sMotionSettingFilePath = _T("C:/Program Files (x86)/EzSoftware RM/EzSoftware/MotionSetting.mot");
#else
	m_sMotionSettingFilePath = _T("C:/Program Files/EzSoftware RM/EzSoftware/MotionSetting.mot");
#endif
}

//-----------------------------------------------------------------------------
//
//	�Ҹ���
//
//-----------------------------------------------------------------------------
CAxlMotorSet::~CAxlMotorSet()
{
	this->Axl_Close();
}


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::Axl_Init()
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	long lBoardCount;
	DWORD dwRetVal;
	CFileFind clFinder;
	int i;
	int iUnit = 0;

	lBoardCount = 0;
	dwRetVal = 0;

	// �÷��� �ʱ�ȭ
	m_bAxlInitResult = false;
	m_bOqaOrgFlag = false;
	// ���� ���� �ʱ�ȭ
	for(iUnit = 0; iUnit < MAX_UNIT_COUNT; iUnit++)
	{
		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			m_bOrgFlag[iUnit][i] = false;
		}
	}

	//���̺귯�� �ʱ�ȭ  0 : �����Ⱚ..
	dwRetVal = AxlOpen(0);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("��� ���� �ʱ�ȭ�� ���� �Ͽ����ϴ�."));
		//AddLog(szLog, 1, 999);
        AddLog(szLog, 1, 0);
        AddLog(szLog, 1, 1);
		return false;
	}

	//! ��ϵ� AXL ��밡�� ������ ������ Ȯ���Ѵ�.	
	AxlGetBoardCount(&lBoardCount);
	if (lBoardCount < 1)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("��� ���� �ν��� �����Ͽ����ϴ�."));
		//AddLog(szLog, 1, 999);
        AddLog(szLog, 1, 0);
        AddLog(szLog, 1, 1);
		return false;
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("Motor ���� �ν� �Ϸ�[Count : %d]"), lBoardCount);
	//AddLog(szLog, 0, 999);
    AddLog(szLog, 0, 0);
    AddLog(szLog, 0, 1);

	TCHAR* pszPath = NULL;
	pszPath = m_sMotionSettingFilePath.GetBuffer(m_sMotionSettingFilePath.GetLength() + 1);

	//if (IsExist_Path(m_sMotionSettingFilePath) == true)
	if(clFinder.FindFile(pszPath) == TRUE)
	{
		//! ��� ���� �ʱ� ���� ������ ���Ͽ��� �о� �����Ѵ�. 
		if (AxmMotLoadParaAll(CT2A(pszPath)) != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("��� ���� �Ķ���� ���� ������ �����Ͽ����ϴ�."));
			//AddLog(szLog, 1, 999);
            AddLog(szLog, 1, 0);
            AddLog(szLog, 1, 1);
			return false;
		}
	}
	else
	{
		//! ��� ���� �ʱ� ���� ���Ͽ� ���� �Ѵ�. 
		AxmMotSaveParaAll(pszPath);//AxmMotSaveParaAll(CT2A(pszPath));
	}

	m_sMotionSettingFilePath.ReleaseBuffer();




	int MAX_AA_COUNT = 1;

	
	for(iUnit = 0; iUnit < MAX_AA_COUNT; iUnit++)
	{
		//! �ʱ� ����
		/*this->AmpDisableAll(iUnit);*/
		// ��Ǻ��� ��������
		if (this->Axl_Axisconfig(iUnit) != 1)
		{
			m_bAxlInitResult = false;
		}

		this->AmpDisableAll(iUnit);
	}

	m_bAxlInitResult = true;

	return m_bAxlInitResult;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::Axl_Close()
{
#ifndef ON_LINE_MOTOR
	return;
#endif

	int i;
	int iUnit = 0;

	int MAX_AA_COUNT = 2;
	// ��� ���͸� �����Ѵ�.
	for (i = 0; i < (MAX_MOTOR_COUNT * MAX_AA_COUNT); i++)
	{
		AxmMoveSStop(MOTOR_AXIS_NUMBER[i]);
	}

	TCHAR* pszPath = NULL;
	pszPath = m_sMotionSettingFilePath.GetBuffer(m_sMotionSettingFilePath.GetLength() + 1);
	//! ��� ���� ���� �������� ���Ͽ� ���� �Ѵ�. 
	AxmMotSaveParaAll(CT2A(pszPath));
	m_sMotionSettingFilePath.ReleaseBuffer();

	AxlClose();

	// ���� ���� �ʱ�ȭ
	for(iUnit = 0; iUnit < MAX_UNIT_COUNT; iUnit++)
	{
		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			m_bOrgFlag[iUnit][i] = false;
		}
	}
}

//-----------------------------------------------------------------------------
//! ��� ��ü�� Axis�� �ʱ� ������ �����Ѵ�. 
//! [���� ����] 
//!    1) �ݵ��, 'Axl_Init' �Լ��� 'AxmMotLoadParaAll' �Լ� ȣ�� �Ŀ� �� �Լ��� ȣ���Ͽ��� �Ѵ�. 
//!    2) PCI-R1604�� A4N, A5N��������̺��� ���(PM����) Limit/Home/Servo On/Inposition�� Level�� ���� �Ͻ� �� ���� High�Ǵ� Low�� ������ �Ǿ��ֽ��ϴ�. 
//!		  Limit ������ A/B���� �ٲٰ��� �Ҷ��� ��������̺��� �Ķ���͸� �ٲپ�� �մϴ�.
//-----------------------------------------------------------------------------
int CAxlMotorSet::Axl_Axisconfig(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return 1;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwStatus;
	DWORD dwRetVal;
	long lAxisCount;
	int nFailCount;
	int nUseAxis;
	int i;

	dwStatus = 0;
	dwRetVal = 0;
	lAxisCount = 0;
	nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;
	
	//! ���峻�� ���ͷ�Ʈ ����� ���� ��Ų��. (RTEX ���� ����)
	AxlInterruptDisable();

	//! �ý��ۿ� ������ �� ������ Ȯ��	
	AxmInfoGetAxisCount(&lAxisCount);
	int MAX_AA_COUNT = 0;
	int MotorTotalCount = 0;

	MotorTotalCount = MAX_MOTOR_COUNT;

	if (lAxisCount != MotorTotalCount)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("���� ����̺� ���� ����ġ[%d / %d]"), lAxisCount, MotorTotalCount);
		AddLog(szLog, 1, nUnit);

		return -4;
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("Motor ����̺� �ν� �Ϸ�[Count : %d]"), lAxisCount);
	AddLog(szLog, 0, nUnit);

	nFailCount = 0;
	for (i = 0; i < MAX_MOTOR_COUNT; i++)	
	{
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;

		dwRetVal = AxmInfoIsInvalidAxisNo(MOTOR_AXIS_NUMBER[nUseAxis]);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ����̺� �ν� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 1, nUnit);

			nFailCount++;

			continue;
		}

		dwRetVal = AxmMotSetMoveUnitPerPulse(MOTOR_AXIS_NUMBER[nUseAxis], 1, 1);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ����̺� ���ڱ��� �ʱ�ȭ ����"), MOTOR_NAME[i]);
			AddLog(szLog, 1, nUnit);

			nFailCount++;

			continue;
		}

		dwRetVal = AxmMotSetMinVel(MOTOR_AXIS_NUMBER[nUseAxis], 1);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ����̺� �ּ� �ӵ� ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 1, nUnit);

			nFailCount++;

			continue;
		}

#ifdef MOTOR_ACC_TYPE_SEC
		dwRetVal = AxmMotSetAccelUnit(MOTOR_AXIS_NUMBER[nUseAxis], SEC);	//
#else
		dwRetVal = AxmMotSetAccelUnit(MOTOR_AXIS_NUMBER[nUseAxis], UNIT_SEC2);	//
#endif

		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ����̺� ���ӵ� ���� ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 1, nUnit);

			nFailCount++;

			continue;
		}

		dwRetVal = AxmMotSetProfileMode(MOTOR_AXIS_NUMBER[nUseAxis], ASYM_S_CURVE_MODE);	//���� ���� ��� ����;.		ASYM_TRAPEZOIDE_MODE
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���Ī S-Curve ������ ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 1, nUnit);

			nFailCount++;

			continue;
		}

		// S-Curve Motion ������, Curve ���� ���� - Default ������ 100% 
		double dAccelJerk = 0.0, dDecelJerk = 0.0;

		dwRetVal = AxmMotSetAccelJerk(MOTOR_AXIS_NUMBER[nUseAxis], 30);
		dwRetVal = AxmMotSetDecelJerk(MOTOR_AXIS_NUMBER[nUseAxis], 30);

		AxmMotGetAccelJerk(MOTOR_AXIS_NUMBER[nUseAxis], &dAccelJerk);
		AxmMotGetDecelJerk(MOTOR_AXIS_NUMBER[nUseAxis], &dDecelJerk);

		if (dAccelJerk != 30.0 || dDecelJerk != 30.0) 
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ������ ���� ��ũ�� ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 0, nUnit);

			nFailCount++;
			continue;
		}

		dwRetVal = AxmMotSetAbsRelMode(MOTOR_AXIS_NUMBER[nUseAxis], POS_ABS_MODE);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� ��ġ �̵� ��� ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 0, nUnit);

			nFailCount++;
			continue;
		}

		DWORD	dwHomLevel;

		if (nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_PCB_Z || nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_PCB_TX || nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_PCB_TY)
		{
			dwRetVal = AxmHomeSetSignalLevel(MOTOR_AXIS_NUMBER[nUseAxis], HIGH);
		}
		else
		{
			dwRetVal = AxmHomeSetSignalLevel(MOTOR_AXIS_NUMBER[nUseAxis], LOW);		//�����ڵ�
		}

		
		dwRetVal = AxmHomeGetSignalLevel(MOTOR_AXIS_NUMBER[nUseAxis], &dwHomLevel);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� HOME LEVEL ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 0, nUnit);

			nFailCount++;
			continue;
		}

		DWORD	dwMaxVel;
		dwMaxVel = AxmMotSetMaxVel(MOTOR_AXIS_NUMBER[nUseAxis], MOTOR_SPEED_MAX[i] * g_clSysData.m_dMotorResol[nUnit][i]);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �ְ� �ӵ� ���� ����"), MOTOR_NAME[i]);
			AddLog(szLog, 0, nUnit);

			nFailCount++;
			continue;
		}

		//! OneHighLowHigh      1�޽� ���, PULSE(Active High)  +����(DIR=Low)  / -����(DIR=High)
		//! OneHighHighLow      1�޽� ���, PULSE(Active High)  +����(DIR=High) / -����(DIR=Low)
		//! OneLowLowHigh       1�޽� ���, PULSE(Active Low)   +����(DIR=Low)  / -����(DIR=High)
		//! OneLowHighLow       1�޽� ���, PULSE(Active Low)   +����(DIR=High) / -����(DIR=Low)
		//! TwoCcwCwHigh        2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active High
		//! TwoCcwCwLow         2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active Low
		//! TwoCwCcwHigh        2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active High
		//! TwoCwCcwLow         2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active Low
		switch (MOTOR_TYPE[i])
		{
		case LINEAR:
			dwRetVal = AxmSignalSetInpos(MOTOR_AXIS_NUMBER[nUseAxis], HIGH);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Inposition ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}

			dwRetVal = AxmSignalSetStop(MOTOR_AXIS_NUMBER[nUseAxis], EMERGENCY_STOP, LOW);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ��� ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			dwRetVal = AxmSignalSetLimit(MOTOR_AXIS_NUMBER[nUseAxis], EMERGENCY_STOP, LOW, LOW);
            //if (nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_PCB_Z || nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_LENS_Z)//(nUnit * MAX_MOTOR_COUNT) + i;
            //{
            //    dwRetVal = AxmSignalSetLimit(nUseAxis, EMERGENCY_STOP, LOW, LOW);
            //}
            //else
            //{
            //    dwRetVal = AxmSignalSetLimit(nUseAxis, EMERGENCY_STOP, HIGH, HIGH);
            //}
			
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Limit ���� ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}

			dwRetVal = AxmSignalSetServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], HIGH);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� SERVER Alarm ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			break;
			//==================================================================================�ڡڡڡڡڡ�
			//
			//
			//
			//
		case STEPING:
			dwRetVal = AxmSignalSetInpos(MOTOR_AXIS_NUMBER[nUseAxis], UNUSED);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Inposition ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			dwRetVal = AxmMotSetPulseOutMethod(MOTOR_AXIS_NUMBER[nUseAxis], TwoCcwCwHigh);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� CW/CCW ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}

			dwRetVal = AxmSignalSetStop(MOTOR_AXIS_NUMBER[nUseAxis], EMERGENCY_STOP, HIGH);// LOW);//
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ��� ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			//
			dwRetVal = AxmSignalSetLimit(MOTOR_AXIS_NUMBER[nUseAxis], EMERGENCY_STOP, LOW, LOW);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Limit ���� ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}

			
			//������ 4ü��
			dwRetVal = AxmMotSetEncInputMethod(MOTOR_AXIS_NUMBER[nUseAxis], ObverseSqr4Mode);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� 4ü�� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}

			dwRetVal = AxmSignalSetServoOnLevel(MOTOR_AXIS_NUMBER[nUseAxis], HIGH);
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Servo On Level ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			if (nUseAxis - (nUnit * MAX_MOTOR_COUNT) == MOTOR_PCB_Z)
			{
				dwRetVal = AxmSignalSetServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], LOW);
			}
			else
			{
				dwRetVal = AxmSignalSetServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], HIGH);
			}
			
			
			if (dwRetVal != AXT_RT_SUCCESS)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� SERVER Alarm ���� ���� ����"), MOTOR_NAME[i]);
				AddLog(szLog, 0, nUnit);

				nFailCount++;
			}
			break;
		}
	}

	if (nFailCount > 0)
	{
		return -nFailCount;
	}

	return 1;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::ReadBit(int nUnit, int nAxis, int nBit)
{
#ifndef ON_LINE_MOTOR
	return TRUE;
#endif

	DWORD dwOn = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	AxmSignalReadInputBit(MOTOR_AXIS_NUMBER[nUseAxis], nBit, &dwOn);

	if (dwOn == 1)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
//
//	���� �� AMP ENABLE
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpEnable(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwLevel = ENABLE;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	this->StopAxis(nUnit, nAxis);

	//! �̹� Servo On �������� Ȯ���Ѵ�. 
	AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
	if (dwLevel == ENABLE)
		return true;

	m_bOrgFlag[nUnit][nAxis] = false;
	AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);

	Sleep(150);

	AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
	if (dwLevel != ENABLE) {
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	UNIT ��ü AMP ENABLE
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpEnableAll(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwLevel;
	bool bRetVal;
	int i;
	int nUseAxis = 0;

	dwLevel = ENABLE;
	bRetVal = true;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	this->StopAxisAll(nUnit);
	Sleep(1000);
	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;
		m_bOrgFlag[nUnit][i] = false;

		AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
		if (dwLevel == ENABLE)
			continue;

		AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
		Sleep(200);

		AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);

		

		if (dwLevel != ENABLE)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] Servo On ����[%d]"), i);
			AddLog(szLog, 1, nUnit);
			bRetVal = false;
			//break;
		}
	}

	if (bRetVal == false)
	{
		//DoSleepEvents();

		for (i = 0; i < MAX_MOTOR_COUNT; i++)
		{
			nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;
			m_bOrgFlag[nUnit][i] = false;

			AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
			if (dwLevel == ENABLE)
				continue;

			AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
			Sleep(200);

			AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
			if (dwLevel != ENABLE)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] Servo On ����[%d]"), i);
				AddLog(szLog, 1, nUnit);
				bRetVal = false;
				//break;
			}
		}
	}

	if (bRetVal == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] AmpEnableAll ����"));
		AddLog(szLog, 1, nUnit);
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//! ���� ���� Servo-Off�� �ǽ�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpDisable(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	double dDecel = 0.;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	m_bOrgFlag[nUnit][nAxis] = false;

	AxmMoveStop(MOTOR_AXIS_NUMBER[nUseAxis], dDecel);
	AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);

	//DoSleepEvents();
	Sleep(300);

	if (MOTOR_TYPE[nAxis] == STEPING)
		AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);

	return true;
}

//-----------------------------------------------------------------------------
//
//	UNIT ��ü �� SERVO OFF
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpDisableAll(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	double dDecel = 0.0;
	int nUseAxis = 0;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;

		AxmMoveStop(MOTOR_AXIS_NUMBER[nUseAxis], dDecel);
		AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);


		AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
		Sleep(100);

		AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);
		////if (MOTOR_TYPE[i] == STEPING) 
		//{

		//	AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
		//}
		Sleep(200);
	}
	

	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ���� Servo-On ��ȣ�� ��� ���¸� ��ȯ(T=ON, F=OFF ����)
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetAmpEnable(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwStatus = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	// Servo �˶� Ȯ��.
	AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);	// upStatus  1: Servo On
	if (dwStatus == ACTIVE)
	{
		return true;
	}

	m_bOrgFlag[nUnit][nAxis] = false;

	return false;
}

//-----------------------------------------------------------------------------
//
//	Servo-Off ���°� ���� ��� false
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetAmpEnableAll(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwStatus = 0;
	int nUseAxis = 0;
	bool bRetVal = true;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;

		AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);	// upStatus  1: Servo On
		if (dwStatus != ACTIVE)
		{
			m_bOrgFlag[nUnit][i] = false;
			bRetVal = false;
			break;
		}
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetOrgFlagAll(int nUnit)
{
	int i;
	bool bRetVal = true;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for(i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		if(m_bOrgFlag[nUnit][i] == false)
		{
			bRetVal = false;
			break;
		}
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//	���� �� �˶� Reset�� Servo-On
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpFaultReset(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	double dDecel = 0.0;
	DWORD dwLevel = 0;
	int nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	// ���� �÷��� �ʱ�ȭ
	m_bOrgFlag[nUnit][nAxis] = false;

	AxmMoveStop(MOTOR_AXIS_NUMBER[nUseAxis], dDecel);
	AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);

	AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);

	//DoSleepEvents();
	Sleep(100);

	AxmSignalServoAlarmReset(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);
	AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);

	//DoSleepEvents();
	Sleep(100);

	AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
	if (dwLevel != ENABLE)
		return false;

	return true;
}

//-----------------------------------------------------------------------------
//
//	UNIT ��ü �� �˶� Reset�� Servo-On �ǽ�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::AmpFaultResetAll(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;	

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		this->AmpDisable(nUnit, i);
	}

	//DoSleepEvents();
	Sleep(1000);

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		this->AmpEnable(nUnit, i);
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	�˶� ��ȣ Ȯ��
//
//-----------------------------------------------------------------------------
int	CAxlMotorSet::AmpFaultCheck(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return 0;
#endif

	DWORD dwStatus = 0;
	int nAmpFaultFlag = 0;
	int nUseAxis = 0;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;	

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;
		// ���� �˶� ��ȣ Ȯ��
		AxmSignalReadServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);
		if (dwStatus == ACTIVE)
		{
			nAmpFaultFlag += 0x01 << i;
			m_bOrgFlag[nUnit][i] = true;
		}
	}

	return nAmpFaultFlag;
}
bool CAxlMotorSet::GetServoOn(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return false;
#endif

	DWORD dwLevel = 0;
	int nUseAxis = 0;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = nUnit * MAX_MOTOR_COUNT + nAxis;

	AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
	if (dwLevel == ENABLE)
		return true;

	return false;
}

//-----------------------------------------------------------------------------
//
//	���� ���� �˶� ���� ��ȯ
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetAmpFault(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return false;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStatus = 0;
	DWORD dwUse = 0;
	int nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = nUnit * MAX_MOTOR_COUNT + nAxis;
	
	dwRetVal = AxmSignalGetServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], &dwUse);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		return true;
	}

	if (dwUse == UNUSED)
		return true;


	// ���� �˶� ��ȣ Ȯ��
	dwRetVal = AxmSignalReadServoAlarm(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		m_bOrgFlag[nUnit][nAxis] = false;
		return true;
	}

	if (dwStatus == ACTIVE)
	{
		m_bOrgFlag[nUnit][nAxis] = false;
		return true;
	}

	return false;
}


//! Motor�� ��ǥ ������ �����ߴ��� Ȯ��
//-----------------------------------------------------------------------------
//
// ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetInposition(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStatus = 0;
	DWORD dwUse = 0;
	bool bRetVal = true;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	//! Inposition ��ȣ ���� Ȯ��
	//dwRet = AxmSignalGetInpos(iUseAxis, &use);
	//if ( dwRet != AXT_RT_SUCCESS )	return false;
	//if ( use == UNUSED )				return false;

	//! Inpositon ��ȣ�� �Է� Ȯ��	
	dwRetVal = AxmSignalReadInpos(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		bRetVal = false;
	}

	if (dwStatus == INACTIVE)
	{
		bRetVal = false;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
// ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��( Encord������ Ȯ��)
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetPcbYMotorPosCheck(int nUnit, int dPos)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	double dCurPos = 0.0;
	double dMotorPos = 0.0;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;


	dCurPos = this->GetEncoderPos(nUnit, MOTOR_PCB_Y);
	dMotorPos = g_clModelData[nUnit].m_stTeachData[dPos].dPos[MOTOR_PCB_Y];
	if (fabs(dMotorPos - dCurPos) > ENCORDER_GAP)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
// ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��( Encord������ Ȯ��)
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetPcbXYMotorPosCheck(int nUnit,int dPos)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	double dCurPos = 0.0;
	double dMotorPos = 0.0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;


	if (dPos == LASER_POS)
	{
		double dLaserPosX[4];
		double dLaserPosY[4];
		double currentX = this->GetEncoderPos(nUnit, MOTOR_PCB_X);
		double currentY = this->GetEncoderPos(nUnit, MOTOR_PCB_Y);
		dLaserPosX[0] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_X];
		dLaserPosX[1] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_X] - (g_clModelData[nUnit].m_dLaserMoveX[0] * -1);
		dLaserPosX[2] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_X] - (g_clModelData[nUnit].m_dLaserMoveX[0] * -1);
		dLaserPosX[3] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_X];

		dLaserPosY[0] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_Y];
		dLaserPosY[1] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_Y];
		dLaserPosY[2] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_Y] + g_clModelData[nUnit].m_dLaserMoveY[0];
		dLaserPosY[3] = g_clModelData[nUnit].m_stTeachData[LASER_POS].dPos[MOTOR_PCB_Y] + g_clModelData[nUnit].m_dLaserMoveY[0];
		for (int i = 0; i < 4; i++)
		{
			if (fabs(currentX - dLaserPosX[i]) < ENCORDER_GAP && fabs(currentY - dLaserPosY[i]) < ENCORDER_GAP)
			{
				return true;
			}
		}
	}
	else
	{
		for (int nAxis = MOTOR_PCB_X; nAxis < MOTOR_PCB_Y; nAxis++)
		{
			dCurPos = this->GetEncoderPos(nUnit, nAxis);
			dMotorPos = g_clModelData[nUnit].m_stTeachData[dPos].dPos[nAxis];
			if (fabs(dMotorPos - dCurPos) > ENCORDER_GAP)
			{
				return false;
			}
		}
	}
	return true;
}
bool CAxlMotorSet::GetLensXYMotorPosCheck(int nUnit, int dPos)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

bool CAxlMotorSet::GetPcbZMotorPosCheck(int nUnit, int dPos)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif


	double dCurPos = 0.0;
	double dMotorPos = 0.0;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	dCurPos = this->GetEncoderPos(nUnit, MOTOR_PCB_Z);
	dMotorPos = g_clModelData[nUnit].m_stTeachData[dPos].dPos[MOTOR_PCB_Z];
	if (fabs(dMotorPos - dCurPos) > ENCORDER_GAP)
	{
		return false;
	}
	return true;
}

bool CAxlMotorSet::GetLensZMotorPosCheck(int nUnit, int dPos)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}
//-----------------------------------------------------------------------------
//
// ���� ���� Home ���� ��ȣ�� �Է� Ȯ��
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetHomeSensor(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStatus = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	//int ret = home_switch(axis);
	//return ret ? true : false;
	//! Home ��ȣ�� �Է� Ȯ��
	dwRetVal = AxmHomeReadSignal(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwStatus == INACTIVE)
		return false;


	return true;
}

//-----------------------------------------------------------------------------
//
// ���� ���� (-)����Ʈ ���� ��ȣ�� �Է� Ȯ��
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetNegaSensor(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStopMode = 0;
	DWORD dwPositiveLevel = 0;
	DWORD dwNegativeLevel = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	dwRetVal = AxmSignalGetLimit(MOTOR_AXIS_NUMBER[nUseAxis], &dwStopMode, &dwPositiveLevel, &dwNegativeLevel);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwNegativeLevel == UNUSED)
		return false;

	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��

	dwRetVal = AxmSignalReadLimit(MOTOR_AXIS_NUMBER[nUseAxis], &dwPositiveLevel, &dwNegativeLevel);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwNegativeLevel == ACTIVE)
		return true;

	return false;
}

//-----------------------------------------------------------------------------
//
// ���� ���� (+)����Ʈ ���� ��ȣ�� �Է� Ȯ��
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetPosiSensor(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStopMode = 0;
	DWORD dwPositiveLevel = 0;
	DWORD dwNegativeLevel = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	dwRetVal = AxmSignalGetLimit(MOTOR_AXIS_NUMBER[nUseAxis], &dwStopMode, &dwPositiveLevel, &dwNegativeLevel);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwPositiveLevel == UNUSED)
		return false;

	//int ret = pos_switch(axis);
	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��

	dwRetVal = AxmSignalReadLimit(MOTOR_AXIS_NUMBER[nUseAxis], &dwPositiveLevel, &dwNegativeLevel);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwPositiveLevel == ACTIVE)
		return true;

	return false;
}

//-----------------------------------------------------------------------------
//
// ������ġ '0' SET
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::InitialPos(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return;
#endif
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	AxmStatusSetActPos(MOTOR_AXIS_NUMBER[nUseAxis], 0.0);
	AxmStatusSetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], 0.0);
}


//! CRC ��ȣ ����(�޽� ����� ��� ����⿡ ������ ���)
//! Ư�� �������� ��� ������ �Ϸ�� �������� �Ǵ� ����Ʈ������ ������ ��쿡 
//! �ܺο��� CRC(Current Remaining Clear)��ȣ�� �޾� �������� ������ �ִ� �ܿ� �޽��� �����־�� �ϴ� ��찡 �߻��Ѵ�.
//! [���� ����] CRC ��ȣ ���� ����� �޽� ����� ��������(PCI-N804/404, RTEX-PM)������ ����� �� �ִ� ����̴�.
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::ActCRC(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	long  lBoardNo = 0;
	long  lModulePos = 0;
	DWORD dwModuleID = 0;
	DWORD dwLevel = 0;
	DWORD dwMethod = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;
	
	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	//! ModuleID (Module�� ����)
	//! 'AXHS.h' ���� : 'C:\Program Files\EzSoftware RM\AXL(Library)\C, C++' �����ȿ� ���� (AXT_FUNC_MODULE)	
	//!      AXT_SMC_R1V04    = 0x09,   // CAMC-QI, 1 Axis, For RTEX SLAVE only	
	dwRetVal = AxmInfoGetAxis(MOTOR_AXIS_NUMBER[nUseAxis], &lBoardNo, &lModulePos, &dwModuleID);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	if (dwModuleID != AXT_SMC_R1V04)
		return false;


	//! ���� �࿡ CRC ��ȣ ��� ���� ������ ��ȯ �Ѵ�.
	dwRetVal = AxmCrcGetMaskLevel(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel, &dwMethod);
	if (dwRetVal != AXT_RT_SUCCESS)
		return false;

	//! CRC ��ȣ ������
	if (dwLevel == UNUSED)
		return false;


	//! ���� �࿡ CRC ��ȣ�� ���������� ����� �߻���Ų��. (���� �ð� ���ȸ� �ۿ��Ŵ)
	AxmCrcSetOutput(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
	Sleep(10);
	AxmCrcSetOutput(MOTOR_AXIS_NUMBER[nUseAxis], DISABLE);

	return true;
}


//-----------------------------------------------------------------------------
//
//	�������� ���� �������� ��ȯ
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetStopAxis(int nUnit, int nAxis)
{//! ���� ���� ���� ��ȯ(T=����, F=������)
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	DWORD dwStatus = 0;
	bool bRetVal = false;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	//! ���� ���� ���¸� Ȯ��
	dwRetVal = AxmStatusReadInMotion(MOTOR_AXIS_NUMBER[nUseAxis], &dwStatus);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		bRetVal = false;
	}

	if (dwStatus == FALSE)
	{
		bRetVal = true;
	}
	
	return bRetVal;
}


//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::StopAxis(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwTickCount;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmMoveStop(MOTOR_AXIS_NUMBER[nUseAxis] , 0.0);
	/*AxmMoveSStop(MOTOR_AXIS_NUMBER[nUseAxis]);*/
	//Stop_Home_Search(iAxis);

	dwTickCount = GetTickCount();
	while (!this->GetStopAxis(nUnit, nAxis))
	{
		if((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[FAIL][%s] ���� ���� �ð� �ʰ�"), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		Sleep(5);
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	UNIT ��ü �� ����
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::StopAxisAll(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	bool bRunFlag[MAX_MOTOR_COUNT] = { false, };
	TCHAR szLog[SIZE_OF_1K];
	DWORD dwTickCount;
	int nUseAxis;	//���� ���� ��� �� ��ȣ
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;	
	
	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		//! ���� ���� ���� ó���� ����ڰ� �Է��� ���ӵ���ŭ ��� ������ �����ϸ鼭 �����ϴ� �Լ��̴�. 
		nUseAxis = (nUnit * MAX_MOTOR_COUNT) + i;
		AxmMoveSStop(MOTOR_AXIS_NUMBER[nUseAxis]);

		bRunFlag[i] = true;
	}

	dwTickCount = GetTickCount();
	while (1)
	{
		if ((GetTickCount() - dwTickCount) > 1000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("��ü ���� ���� �ð� �ʰ�[1 sec]"));
			AddLog(szLog, 1, nUnit);

			return false;
		}

		for (i = 0; i< MAX_MOTOR_COUNT; i++)
		{
			if (bRunFlag[i])
			{
				if (this->GetStopAxis(nUnit, i))
				{
					bRunFlag[i] = false;
					break;
				}
			}
		}

		if (i == MAX_MOTOR_COUNT)
			break;

		//DoSleepEvents();
		Sleep(5);
	}

	return true;
}

//-----------------------------------------------------------------------------
//
// Linear ���� ��ġ ��ǥ ��ȯ
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetEncoderPos(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	if (MOTOR_TYPE[nAxis] == STEPING)
	{
		//if (nAxis == MotIndex_T)
		//{
		//	dPos = GetDDMotorPos(nAxis);
		//}
		//else
		//{
		AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);
		//dPos /= g_clSysData.m_dMotorResol[nAxis];
		//}
	}
	else
	{
		AxmStatusGetActPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);
		//AxmStatusGetCmdPos(nAxis, &dPos);
	}
	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];

	//	m_csEncoder.Unlock();

	return dPos; 
}

//-----------------------------------------------------------------------------
//
//	Linear ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetEncoderPos_Disp(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = nUnit * MAX_MOTOR_COUNT + nAxis;
	if (MOTOR_TYPE[nAxis] == STEPING)
	{
		/*if (nAxis == MotIndex_T)
		{
		dPos = GetDDMotorPos_Disp(nAxis);
		return dPos;
		}
		else
		{*/
		AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);
		//}
		//return (float)pos;
	}
	else
	{
		AxmStatusGetActPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);
	}

	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];
	dPos -= g_clSysData.m_dOrgDataset[nUnit][nAxis];

	return dPos;
}

//-----------------------------------------------------------------------------
//
// Steping ���� ��ġ ��ǥ ��ȯ
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetCommandPos(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];

	return dPos;
}

//-----------------------------------------------------------------------------
//
// Steping ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ	
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetCommandPos_Disp(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];
	dPos -= g_clSysData.m_dOrgDataset[nUnit][nAxis];

	return dPos;
}

//-----------------------------------------------------------------------------
//
// Linear ���� ��ġ ��ǥ Setting
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::SetEncoderPos(int nUnit, int nAxis, double dPos)
{
#ifndef ON_LINE_MOTOR
	return;
#endif
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	AxmStatusSetActPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos);
	AxmStatusSetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos);
}

//-----------------------------------------------------------------------------
//
// Steping ���� ��ġ ��ǥ Setting
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::SetCommandPos(int nUnit, int nAxis, double dPos)
{
#ifndef ON_LINE_MOTOR
	return;
#endif
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	//Steping ���ʹ� ���� Off�� ��ǥ Setting �ؾ���.
	this->AmpDisable(nUnit, nAxis);
	Sleep(100);

	AxmStatusSetActPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos);
	AxmStatusSetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos);
	Sleep(100);

	AmpEnable(nUnit, nAxis);
}

//-----------------------------------------------------------------------------
//
//	DD ���� ��ġ ��ǥ Setting
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetDDMotorPos(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];

	return dPos;
}

//-----------------------------------------------------------------------------
//
// DD ���� ��ġ ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetDDMotorPos_Disp(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	dPos /= g_clSysData.m_dMotorResol[nUnit][nAxis];
	dPos -= g_clSysData.m_dOrgDataset[nUnit][nAxis];

	return dPos;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetActPos(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis =0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	return dPos;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetEncoderPulse(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	if (MOTOR_TYPE[nAxis] == STEPING)
		AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);
	else
		AxmStatusGetActPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	return dPos;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetCommandPulse(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 0.0;
#endif

	double dPos = 0.0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dPos;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dPos;	

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmStatusGetCmdPos(MOTOR_AXIS_NUMBER[nUseAxis], &dPos);

	return dPos;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::SetDDMotorCalcPos(int nUnit, int nAxis, double dMaxDeg)
{
	double dCurEncoder = 0.0;
	double dCalEncoder = 0.0;
	double dResultEncoder = 0.0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;	

	dCurEncoder = GetActPos(nUnit , nAxis);
	dCalEncoder = dMaxDeg * g_clSysData.m_dMotorResol[nUnit][nAxis];

	//dResultEncoder = GetDoubleModulus(dCurEncoder, dCalEncoder);

	this->SetDDMotorPos(nUnit, nAxis, dResultEncoder);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::SetDDMotorPos(int nUnit, int nAxis, double dPos)
{
#ifndef ON_LINE_MOTOR
	return;
#endif
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	if (this->GetStopAxis(nUnit, nAxis))
	{
		AxmStatusSetPosMatch(MOTOR_AXIS_NUMBER[nUseAxis], dPos);
	}
}

//-----------------------------------------------------------------------------
//
//! ���ӵ� = �ӵ�/(���� �ð�)���� ���
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::CalcAccFromAccTime(double dVel, double dTime)
{
	double dRetVal;

	dRetVal = 0.;

	if (fabs(dTime) < 0.001)
	{
		dRetVal = dVel / 2.;		//! ���� �ð��� �ʹ� ª�Ƽ� ������ ���� �ð��� 2 sec�� ����
	}
	else
	{
		dRetVal = dVel / dTime;
		dRetVal *= 1000.;			//! ���� ��ȯ msec -> sec
	}

	return dRetVal;
}

//-----------------------------------------------------------------------------
//
//	������ �ӵ��� �����Ѵ�.
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::JogMove(int nUnit, int nAxis, double dVel)
{
#ifndef	ON_LINE_MOTOR
	return true;
#endif

	DWORD dwRetVal = 0;
	double dAcc = 0.0;
	bool bRetVal = true;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = nUnit * MAX_MOTOR_COUNT + nAxis;	

	if (this->GetAmpFault(nUnit, nAxis) == true)
		return false;


#ifdef MOTOR_ACC_TYPE_SEC
	dAcc = (g_clSysData.m_dMotorAccTime[nUnit][nAxis]);
#else
	dAcc = g_clSysData.m_dMotorAccTime[nUnit][nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ���� 
#endif

	dwRetVal = AxmMoveVel(MOTOR_AXIS_NUMBER[nUseAxis], dVel, dAcc, dAcc);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		bRetVal = false;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
// ������ ���� ������ ���������մϴ�.
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::JogStop(int nUnit, int nAxis)
{
#ifndef	ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwTickCount;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	
	AxmMoveSStop(MOTOR_AXIS_NUMBER[nUseAxis]);

	dwTickCount = GetTickCount();
	while (!GetStopAxis(nUnit, nAxis))
	{
		//DoSleepEvents();
		Sleep(10);

		if ((GetTickCount() - dwTickCount) > 1000)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[FAIL][%s] JOG �̵� ���� �ð� �ʰ�[1 sec]"), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}
	}

	return true;
}


//! Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
//! ���� �࿡�� ������ ��ȣ�� �����Ѵ�
//! ����Ʈ ������ȣ�� ���� ������ȣ, ���� ����̹��� Z�� ��ȣ �Ǵ� ���� �Է� ��ȣ���� Edge�� ������ �� �ִ�.
//! �ַ� �����˻� ���� �Լ� ���� �� ����ϴ� �Լ��̴�.
//! [���� ����] 
//!		1) End Limit ������ ã�� ���, ��ȣ�� Active Level ���¸� �����Ѵ�.
//!     2) Emergency Stop���� ����� ���, �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
//-----------------------------------------------------------------------------
//
// Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
//
//-----------------------------------------------------------------------------
//bool CAxlMotorSet::MoveAxis_To_Sensor(int axis, strMove_Input_Signal_Search *pstrInput)
//{
//#ifndef ON_LINE_MOTOR
//	return true;
//#endif
//
//	DWORD dwRetVal;
//	double dVel;
//	double dAccel;
//	long lDetectSignal, lSignalEdge, lSignalMethod;
//
//	lDetectSignal = lSignalEdge = lSignalMethod = -1;
//
//	if (pstrInput == NULL)
//		return false;
//
//	dwRetVal = 0;
//	dVel = pstrInput->dVel;
//	dAccel = 0.0;
//
//#ifdef MOTOR_ACC_TYPE_SEC
//	dAccel = pstrInput->dAccel;
//#else
//	dAccel = pstrInput->dAccel * (9.8 * 1000 * g_clSysData.m_dMotorResol[axis]);		//! ���� 
//#endif
//
//	
//	//! lDetectSignal ����
//	if (pstrInput->bEndLimit_Negative == true)
//		lDetectSignal = NegEndLimit;
//
//	if (pstrInput->bEndLimit_Positive == true)
//		lDetectSignal = PosEndLimit;
//
//	if (pstrInput->bHomeSensor == true)
//		lDetectSignal = HomeSensor;
//
//	if (pstrInput->bPhase_Encoder_Z == true)
//		lDetectSignal = EncodZPhase;
//
//	//! lSignalEdge ����
//	if (pstrInput->bEdge_Down == true)
//		lSignalEdge = SIGNAL_DOWN_EDGE;
//
//	if (pstrInput->bEdge_Up == true)
//		lSignalEdge = SIGNAL_UP_EDGE;
//
//	//! lSignalMethod ����
//	if (pstrInput->bEmergency == true)
//		lSignalMethod = EMERGENCY_STOP;
//	if (pstrInput->bSlowDown == true)
//		lSignalMethod = SLOWDOWN_STOP;
//
//	//! �Է°� Ȯ��
//	if (lDetectSignal == NegEndLimit)
//		dVel = -1 * fabs(pstrInput->dVel);
//	else
//		dVel = fabs(pstrInput->dVel);
//
//
//	if (dAccel < 0.)
//		return false;
//
//	if (lDetectSignal < 0)
//		return false;
//	
//	if (lSignalEdge < 0)
//		return false;
//	
//	if (lSignalMethod < 0)
//		return false;
//		
//	//! ���� �࿡�� ������ ��ȣ�� ������ ������ ���͸� �����δ�. 
//	dwRetVal = AxmMoveSignalSearch(axis, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
//	if (dwRetVal != AXT_RT_SUCCESS)
//		return false;
//
//	return true;
//}


//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
//! GUI ȭ�鿡�� ���� ���Ǵ� ����̶� ���� �Լ��� �����. 
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//bool CAxlMotorSet::Move_From_GUI(int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus)
//{//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
//	if (pParentWnd == NULL)			return false;
//	if (bMotorMoving[axis] == true)	return false;
//	bMotorMoving[axis] = true;
//
//	CString sGetTemp = _T("");
//	GetString_FromUI(iID_GUI, pParentWnd, sGetTemp);
//
//	double dGetTemp = atof(sGetTemp);
//	if (dGetTemp > 3.)		dGetTemp = 3.;
//	else if (dGetTemp < -3)	dGetTemp = -3;
//
//	if (bPlus == false)	dGetTemp *= (-1.);
//
//	MoveAxis(axis, REL, dGetTemp, 4/*, 20*/);
//	bMotorMoving[axis] = false;
//
//	return true;
//}
//

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveFromAbsRel(int nUnit, int nAxis, double dPos, bool bPlus)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	double dCurrPos = 0.0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;
	
	if (bPlus == false)
		dPos *= -1.;

	dCurrPos = this->GetEncoderPos(nUnit, nAxis);

	this->MoveAxis(nUnit, nAxis, REL, dPos, g_clSysData.m_dMotorSpeed[nUnit][nAxis]);

	dwTickCount = GetTickCount();
	while (1)
	{
		if (this->GetStopAxis(nUnit, nAxis))
		{
			break;
		}

		if ((GetTickCount() - dwTickCount) > AXIS_MOVE_TIME)
		{
			return false;
		}
		
		Sleep(10);
	}

	return true;
}

bool CAxlMotorSet::MovePcbDispenseStraightLineMotor(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, double dAcc, bool bWait)
{
	// abs 1:absolute, 0:increment
#ifndef ON_LINE_MOTOR
	return true;
#endif
	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxis(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, bool bWait)
{
	// abs 1:absolute, 0:increment
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	double dCurrPos = 0.0;
	double dAcc = 0.0;
	double dDec = 0.0;
	static DWORD dwTickCount;
	int nUseAxis = 0;
	int nStep = 100;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	if (this->GetAmpFault(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� AMP ALARM �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (!this->GetAmpEnable(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� SERVO OFF ���� �Դϴ�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (nAbsFlag == ABS)
	{
		if (MOTOR_TYPE[nAxis] == STEPING)
			dCurrPos = GetCommandPos(nUnit, nAxis);
		else
			dCurrPos = GetEncoderPos(nUnit, nAxis);

		if (fabs(dCurrPos - dPos) < 0.0001)
			return true;

		if (m_bOrgFlag[nUnit][nAxis] == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �������͸� ���� �����Ͻʽÿ�."), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}
	}
	else if (nAbsFlag == REL)
	{
		if (MOTOR_TYPE[nAxis] == STEPING)
			dPos += GetCommandPos(nUnit, nAxis);
		else
			dPos += GetEncoderPos(nUnit, nAxis);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �̵� ��� ������"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false; 
	}  
	//double tempResol = 5000.0;// g_clSysData.m_dMotorResol[nUnit][nAxis];     
	dPos *= g_clSysData.m_dMotorResol[nUnit][nAxis];
	if (dPos > 0)   
		dPos = (int)(dPos + 0.5);    

	//dPos += dCurrPos;

	dVel = dVel * g_clSysData.m_dMotorResol[nUnit][nAxis];	//�̵� �ӵ� 

#ifdef MOTOR_ACC_TYPE_SEC
	dAcc = g_clSysData.m_dMotorAccTime[nUnit][nAxis];		//! ���� 
	dDec = g_clSysData.m_dMotorDecTime[nUnit][nAxis];		//! ����
#else
													/*���ӵ� ���� (G)
													-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
	dAcc = g_clSysData.m_dMotorAccTime[nUnit][nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ���� 
	dDec = g_clSysData.m_dMotorDecTime[nUnit][nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ����
#endif

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos, dVel, dAcc, dDec);//z�� �۵��Ѵ�.
	if(bWait == false)
		return true;

	dwTickCount = GetTickCount();
	nStep = 100;

	while (1)
	{
		switch (nStep)
		{
		case 100:
			if ((this->GetStopAxis(nUnit, nAxis) == true) && ((this->GetEncoderPos(nUnit, nAxis) - dPos) < ENCORDER_GAP))
			{
				dwTickCount = GetTickCount();
				nStep = 200;
			}
			break;
		default:
			break;
		}

		if (nStep >= 200)
			break;

		if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			return false;
		}

		Sleep(10);
	}

	return true;
}
//-----------------------------------------------------------------------------
//
//	���� , � �������� ���� ����
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveStraightPcbXYAxis(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;
#if 0
	TCHAR szLog[SIZE_OF_1K];
	long MOTOR_DIR = DIR_CW;
	long   axisAll[2] = { 0, };
	double posAll[2] = { 0, };
	double velAll[1] = { 0, };
	double accAll[1] = { 0, };
	double decAll[1] = { 0, };



	double dAngle = 0.0;
	double CircleSpeed = 0.0;
	double TrinityCircleRadius = 0.0;
	double dPos[2];
	double dMidPos[2];
	double dEndPos[2];
	dMidPos[0] = dMidPos[1] = 0.0;
	dEndPos[0] = dEndPos[1] = 0.0;
	dPos[0] = dPos[1] = 0.0;
	int cnt = 2;

	axisAll[0] = (nUnit * MAX_MOTOR_COUNT) + MOTOR_PCB_X;
	axisAll[1] = (nUnit * MAX_MOTOR_COUNT) + MOTOR_PCB_Y;

	velAll[0] = g_clModelData[nUnit].m_EpoxyLineSpeed * g_clSysData.m_dMotorResol[nUnit][axisAll[0]];


#ifdef MOTOR_ACC_TYPE_SEC
	accAll[0] = g_clModelData[nUnit].m_EpoxyLineAccDec;		//! ���� 
	decAll[0] = g_clModelData[nUnit].m_EpoxyLineAccDec;		//! ���� 
#else
	/*���ӵ� ���� (G)
	-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
	decAll[0] = g_clModelData[nUnit].m_EpoxyLineAccDec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][axisAll[0]]);		//! ���� 
	accAll[0] = g_clModelData[nUnit].m_EpoxyLineAccDec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][axisAll[0]]);		//! ����
#endif
	long  lPosSize = 2;
	long  lCoordinate = 0;

	//AxmMotSetMoveUnitPerPulse(MOTOR_PCB_X, 1, 10000);	//Unit/Pulse ���� - X 
	//AxmMotSetMoveUnitPerPulse(MOTOR_PCB_Y, 1, 10000);	//					Y 

	 
	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lPosSize, axisAll);

	// �����ġ �������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	AxmContiBeginNode(lCoordinate);

	int mCircleResol = g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];//2000
	//===================================================================================================================================
	//���� 1 
	//�ϴ� ���� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] = g_clModelData[nUnit].m_EpoxyLineLength[0] * -1;
	posAll[0] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);

	posAll[1] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y];


	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 2
	//���� �ϴ� Ŀ��

	CircleSpeed = g_clModelData[nUnit].m_EpoxyCurveSpeed * mCircleResol;// g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	dMidPos[0] = g_clModelData[nUnit].m_EpoxyCurveCenterXPoint[0] * mCircleResol;
	dMidPos[1] = g_clModelData[nUnit].m_EpoxyCurveCenterYPoint[0] * mCircleResol;
	dEndPos[0] = g_clModelData[nUnit].m_EpoxyCurveEndXPoint[0] * mCircleResol;
	dEndPos[1] = g_clModelData[nUnit].m_EpoxyCurveEndYPoint[0] * mCircleResol;
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 3
	//���� ���� ����

	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];

	posAll[1] = g_clModelData[nUnit].m_EpoxyLineLength[1] * -1;
	posAll[1] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 4
	//���� ��� Ŀ��

	CircleSpeed = g_clModelData[nUnit].m_EpoxyCurveSpeed * mCircleResol;// g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	dMidPos[0] = g_clModelData[nUnit].m_EpoxyCurveCenterXPoint[1] * mCircleResol;
	dMidPos[1] = g_clModelData[nUnit].m_EpoxyCurveCenterYPoint[1] * mCircleResol;
	dEndPos[0] = g_clModelData[nUnit].m_EpoxyCurveEndXPoint[1] * mCircleResol;
	dEndPos[1] = g_clModelData[nUnit].m_EpoxyCurveEndYPoint[1] * mCircleResol; 
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 5 
	//��� ���� ����

	posAll[0] = posAll[1] = 0.0;
	posAll[0] = g_clModelData[nUnit].m_EpoxyLineLength[2] * 1; 
	posAll[0] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y];


	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 6
	//���� ��� Ŀ��

	CircleSpeed = g_clModelData[nUnit].m_EpoxyCurveSpeed *mCircleResol;//g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	dMidPos[0] = g_clModelData[nUnit].m_EpoxyCurveCenterXPoint[2] * mCircleResol;
	dMidPos[1] = g_clModelData[nUnit].m_EpoxyCurveCenterYPoint[2] * mCircleResol;
	dEndPos[0] = g_clModelData[nUnit].m_EpoxyCurveEndXPoint[2] * mCircleResol;
	dEndPos[1] = g_clModelData[nUnit].m_EpoxyCurveEndYPoint[2] * mCircleResol;
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 7 
	//���� ���� ����

	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];

	posAll[1] = g_clModelData[nUnit].m_EpoxyLineLength[3] * 1;
	posAll[1] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 8
	//���� �ϴ� Ŀ��


	CircleSpeed = g_clModelData[nUnit].m_EpoxyCurveSpeed *mCircleResol;//g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X]; 
	dMidPos[0] = g_clModelData[nUnit].m_EpoxyCurveCenterXPoint[3] * mCircleResol;
	dMidPos[1] = g_clModelData[nUnit].m_EpoxyCurveCenterYPoint[3] * mCircleResol;
	dEndPos[0] = g_clModelData[nUnit].m_EpoxyCurveEndXPoint[3] * mCircleResol; 
	dEndPos[1] = g_clModelData[nUnit].m_EpoxyCurveEndYPoint[3] * mCircleResol;
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================

	//===================================================================================================================================
	//���� 9 
	//������ ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] = -0.4;// (g_clModelData[nUnit].m_EpoxyLineLength[0] / 4) * -1; 
	posAll[0] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);

	posAll[1] *= g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y];


	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);  

	//===================================================================================================================================
	AxmContiEndNode(lCoordinate);
	// �����ġ ��������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	// ���Ӻ������� ����.
	AxmContiStart(lCoordinate, 0, 0);

	Sleep(10);
#endif
	return true;
}
//-----------------------------------------------------------------------------
//
//	���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveCirclePcbXYAxis(int nUnit, int dPos)//, double dAngle, double dVel, double dAcc)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
#if 0
	TCHAR szLog[SIZE_OF_1K];
	long MOTOR_DIR = DIR_CW;
	long   lAxisAll[2] = { 0, };
	double dPosAll[1] = { 0.0 };
	double dVelAll[1] = { 0.0 };
	double dAccAll[1] = { 0.0 };
	double dDecAll[1] = { 0.0 };
	double dCurrPos = 0.0;
	int nUseAxis = 0;
	

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;
	for (int nAxis = 0; nAxis <= MOTOR_PCB_Y; nAxis++)
	{
		// abs 1:absolute, 0:increment
		if (this->GetAmpFault(nUnit, nAxis))
		{
			m_bOrgFlag[nUnit][nAxis] = false;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] AMP ALARM �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (!this->GetAmpEnable(nUnit, nAxis))
		{
			m_bOrgFlag[nUnit][nAxis] = false;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] SERVO OFF �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}
	}

	lAxisAll[0] = (nUnit * MAX_MOTOR_COUNT) + MOTOR_PCB_X;
	lAxisAll[1] = (nUnit * MAX_MOTOR_COUNT) + MOTOR_PCB_Y;
//#ifdef MOTOR_ACC_TYPE_SEC
//	dAccAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec;//	dAcc;		//! ���� 
//	dDecAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec;//dAcc;		//! ����
//#else
//	/*���ӵ� ���� (G)
//	-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
//	dAccAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X]);		//! ���� 
//	dDecAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_Y]);		//! ����
//#endif
//
//	dVelAll[0] = g_clModelData[nUnit].m_EpoxyCurveSpeed * g_clSysData.m_dMotorResol[nUnit][MOTOR_PCB_X];	//�̵� �ӵ� 



	dAccAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec;		//! ���� 
	dDecAll[0] = g_clModelData[nUnit].m_EpoxyCurveAccDec;		//! ����

	dVelAll[0] = g_clModelData[nUnit].m_EpoxyCurveSpeed;//�̵� �ӵ� 

	


	//������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� ��ȣ ���� �ϴ� �Լ��̴�.���� ���� �� �Լ��� �����.
	double dCenterPos[2];
	double dEndPos[2];

	if (dPos < E_RIGHT_DOWN_SITE || dPos >= MAX_EPOXY_POSITION_TYPE)
	{
		return false;
	}

	dCenterPos[0] = g_clModelData[nUnit].m_EpoxyCurveCenterXPoint[dPos];
	dCenterPos[1] = g_clModelData[nUnit].m_EpoxyCurveCenterYPoint[dPos];
	dEndPos[0] = g_clModelData[nUnit].m_EpoxyCurveEndXPoint[dPos]; 
	dEndPos[1] = g_clModelData[nUnit].m_EpoxyCurveEndYPoint[dPos];
	switch (dPos)
	{
	case E_RIGHT_DOWN_SITE:
		MOTOR_DIR = DIR_CW;//DIR_CW; = ������ü�� �ð�������� ȸ��		//DIR_CCW
		break;
	case E_RIGHT_UP_SITE:
		MOTOR_DIR = DIR_CW;
		break;
	case E_LEFT_UP_SITE:
		MOTOR_DIR = DIR_CW; 
		break;
	case E_LEFT_DOWN_SITE:
		MOTOR_DIR = DIR_CW;
		break;
	default:
		MOTOR_DIR = DIR_CW;  
		break; 
	}

	AxmMotSetMoveUnitPerPulse(MOTOR_PCB_X, 1, 10000);	//Unit/Pulse ���� - X  
	AxmMotSetMoveUnitPerPulse(MOTOR_PCB_Y, 1, 10000);	//					Y  

	Sleep(10);
	long lCoordinate = 0;
	long lPosSize = 2;
	DWORD uAbsRelMode = 1;


	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lPosSize, lAxisAll);  
	AxmContiSetAbsRelMode(lCoordinate, uAbsRelMode);   // 1 = �����ġ����.  
	Sleep(10);
	DWORD dwReturn = 0;
	dwReturn = AxmCircleCenterMove(lCoordinate, lAxisAll, dCenterPos, dEndPos, dVelAll[0], dAccAll[0], dDecAll[0], MOTOR_DIR);
	//dwReturn = AxmCircleAngleMove(lCoordinate, lAxisAll, dCenterPos, 90, dVelAll[0], dAccAll[0], dDecAll[0], MOTOR_DIR); 
	int nStep = 0;
	DWORD dwTickCount;
	while (true)
	{
		switch (nStep)
		{
		case 0:
			nStep = 100;
			dwTickCount = GetTickCount();
			break;
		case 100:
			if (GetStopAxis(nUnit, MOTOR_PCB_X) == true && GetStopAxis(nUnit, MOTOR_PCB_Y) == true)
			{

				nStep = 200;
			}
			break;
		case 200:
			AxmMotSetMoveUnitPerPulse(MOTOR_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
			AxmMotSetMoveUnitPerPulse(MOTOR_PCB_Y, 1, 1); 
			nStep = 900;
			break;
		default:
			break;
		}

		if (nStep == 900)		break;
		if ((GetTickCount() - dwTickCount) > 60000 * 3)
		{
			StopAxis(nUnit, MOTOR_PCB_X);
			StopAxis(nUnit, MOTOR_PCB_Y);

			AxmMotSetMoveUnitPerPulse(MOTOR_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
			AxmMotSetMoveUnitPerPulse(MOTOR_PCB_Y, 1, 1);
			return false;
		}
			
		Sleep(10);
	}

	

#endif
	return true;
}
//-----------------------------------------------------------------------------
//
//	���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxis(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, double dAcc)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	double dPosAll[1] = { 0.0 };
	double dVelAll[1] = { 0.0 };
	double dAccAll[1] = { 0.0 };
	double dDecAll[1] = { 0.0 };
	double dCurrPos = 0.0;
	int nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;
	
	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	// abs 1:absolute, 0:increment
	if (this->GetAmpFault(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] AMP ALARM �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (!this->GetAmpEnable(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] SERVO OFF �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (nAbsFlag == ABS)
	{
		if (MOTOR_TYPE[nAxis] == STEPING)
			dCurrPos = this->GetCommandPos(nUnit, nAxis);
		else
			dCurrPos = this->GetEncoderPos(nUnit, nAxis);

		if (fabs(dCurrPos - dPos) < 0.0001)
			return true;

		if (m_bOrgFlag[nUnit][nAxis] == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ����  ���� ���Ͱ� ���� �ʾҽ��ϴ�."), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}
	}
	else if (nAbsFlag == REL)
	{
		if (MOTOR_TYPE[nAxis] == STEPING)
			dPos += GetCommandPos(nUnit, nAxis);
		else
			dPos += GetEncoderPos(nUnit, nAxis);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �̵� ��� ������."), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	dPos *= g_clSysData.m_dMotorResol[nUnit][nAxis];
	if (dPos > 0)
		dPos = (int)(dPos + 0.5);

	dPosAll[0] = dPos;
	dVelAll[0] = dVel * g_clSysData.m_dMotorResol[nUnit][nAxis];	//�̵� �ӵ�

#ifdef MOTOR_ACC_TYPE_SEC
	dAccAll[0] = dAcc;		//! ���� 
	dDecAll[0] = dAcc;		//! ����
#else
															/*���ӵ� ���� (G)
															-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
	dAccAll[0] = dAcc * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ���� 
	dDecAll[0] = dAcc * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ����
#endif

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos, dVelAll[0], dAccAll[0], dDecAll[0]);

	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
//
//-----------------------------------------------------------------------------
//bool CAxlMotorSet::MoveAxis(int nAxis, int nAbsFlag, double dPos, double dVel, bool bWait/* = true*/)
//{
//	// abs 1:absolute, 0:increment
//#ifndef ON_LINE_MOTOR
//	return true;
//#endif
//
//	char szLog[SIZE_OF_1K];
//	static DWORD dwTickCount;
//	double dCurrPos;
//	double dAcc;
//	double dDec;
//	int nStep;
//
//	if (nAxis < 0 || nAxis >= MAX_MOTOR_COUNT)
//		return false;
//
//	dCurrPos = 0.0;
//	dAcc = dDec = 0.0;
//
//	if (GetAmpFault(nAxis))
//	{
//		m_bOrgFlag[nAxis] = false;
//
//		sprintf_s(szLog, SIZE_OF_1K, "[%s] ���� AMP ALARM �߻�. MoveAxis ���� ����", g_szMotorName[nAxis]);
//		g_AddListLog(szLog);
//
//		return false;
//	}
//
//	if (!GetAmpEnable(nAxis))
//	{
//		m_bOrgFlag[nAxis] = false;
//
//		sprintf_s(szLog, SIZE_OF_1K, "[%s] ���� SERVO OFF ���� �Դϴ�. MoveAxis ���� ����", g_szMotorName[nAxis]);
//		g_AddListLog(szLog);
//
//		return false;
//	}
//
//	if (nAbsFlag == ABS)
//	{
//		if (MOTOR_TYPE[nAxis] == STEPING)
//			dCurrPos = GetCommandPos(nAxis);
//		else
//			dCurrPos = GetEncoderPos(nAxis);
//
//		if (fabs(dCurrPos - dPos) < 0.0001)
//			return true;
//
//		if (m_bOrgFlag[nAxis] == false)
//		{
//			sprintf_s(szLog, SIZE_OF_1K, "[%s] ���� �������͸� ���� �����Ͻʽÿ�.", g_szMotorName[nAxis]);
//			g_AddListLog(szLog);
//
//			return false;
//		}
//	}
//	else if (nAbsFlag == REL)
//	{
//		if (MOTOR_TYPE[nAxis] == STEPING)
//			dPos += GetCommandPos(nAxis);
//		else
//			dPos += GetEncoderPos(nAxis);
//	}
//	else
//	{
//		sprintf_s(szLog, SIZE_OF_1K, "[%s] ���� �̵� ��� ������", g_szMotorName[nAxis]);
//		g_AddListLog(szLog);
//
//		return false;
//	}
//
//	dPos *= g_clSysData.m_dMotorResol[nAxis];
//	if (dPos > 0)
//		dPos = (int)(dPos + 0.5);
//
//	dPos += dCurrPos;
//
//	dVel = dVel * g_clSysData.m_dMotorResol[nAxis];	//�̵� �ӵ�
//
//#ifdef MOTOR_ACC_TYPE_SEC
//	dAcc = g_clSysData.m_dMotorAccTime[nAxis];		//! ���� 
//	dDec = g_clSysData.m_dMotorDecTime[nAxis];		//! ����
//#else
//	/*���ӵ� ���� (G)
//	-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
//	dAcc = g_clSysData.m_dMotorAccTime[nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nAxis]);		//! ���� 
//	dDec = g_clSysData.m_dMotorDecTime[nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nAxis]);		//! ����
//#endif
//
//	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
//	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
//	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
//	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
//	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dDec);
//	
//	if (bWait == true)
//	{
//		nStep = 100;
//		dwTickCount = GetTickCount();
//
//		while (1)
//		{
//			switch (nStep)
//			{
//			case 100:
//				TRACE("%lf %lf\n", this->GetEncoderPos(nAxis), dPos);
//				if ((this->GetStopAxis(nAxis) == true) && ((this->GetEncoderPos(nAxis) - (dPos / g_clSysData.m_dMotorResol[nAxis])) < ENCORDER_GAP))
//				{
//					nStep = 200;
//				}
//				break;
//			default:
//				break;
//			}
//
//			if (nStep < 0)
//			{
//				return false;
//			}
//
//			if (nStep >= 200)
//			{
//				break;
//			}
//
//			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
//			{
//				return false;
//			}
//
//			Sleep(10);
//		}
//	}
//
//	return true;
//}

//-----------------------------------------------------------------------------
//
//	���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�. 
//
//-----------------------------------------------------------------------------
//bool CAxlMotorSet::MoveAxisComplete(int nAxis, int nAbsFlag, double dPos, double dVel, bool bWait/* = true*/)
bool CAxlMotorSet::MoveAxisComplete(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, int nSpeed, bool bWait/* = true*/, double dParamAcc/* = 0.0*/, double dParamDec/* = 0.0*/) //20161101
{
	// abs 1:absolute, 0:increment
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	double dCurrPos = 0.0;
	double dAcc = 0.0;
	double dDec = 0.0;	
	int nRetStep;
	int nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	if (this->GetAmpFault(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� AMP ALARM �߻�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (!this->GetAmpEnable(nUnit, nAxis))
	{
		m_bOrgFlag[nUnit][nAxis] = false;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� SERVO OFF ���� �Դϴ�. MoveAxis ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	if (nAbsFlag == ABS)
	{
		//if (MOTOR_TYPE[nAxis] == STEPING)
		//	dCurrPos = this->GetCommandPos(nUnit, nAxis);
		//else
			dCurrPos = this->GetEncoderPos(nUnit, nAxis);

		if (fabs(dCurrPos - dPos) < 0.0001)
			return true;

		if (m_bOrgFlag[nUnit][nAxis] == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �������͸� ���� �����Ͻʽÿ�."), MOTOR_NAME[nAxis]);
			AddLog(szLog, 1, nUnit);

			return false;
		}
	}
	else if (nAbsFlag == REL)
	{
		//if (MOTOR_TYPE[nAxis] == STEPING)
		//	dPos += this->GetCommandPos(nUnit, nAxis);
		//else
			dPos += this->GetEncoderPos(nUnit, nAxis);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �̵� ��� ������"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	dPos *= g_clSysData.m_dMotorResol[nUnit][nAxis];
	if (dPos > 0)
		dPos = (int)(dPos + 0.5);

	//dPos += dCurrPos;

	if ((dParamAcc == 0.0) && (dParamDec == 0.0))
	{
		dVel = g_clSysData.m_dMotorSpeed[nUnit][nAxis];
		dVel = dVel * g_clSysData.m_dMotorResol[nUnit][nAxis];	//�̵� �ӵ�
	}
	else
	{
		dVel = dVel * g_clSysData.m_dMotorResol[nUnit][nAxis];	//�̵� �ӵ�
	}

#ifdef MOTOR_ACC_TYPE_SEC
	dAcc = g_clSysData.m_dMotorAccTime[nUnit][nAxis];		//! ���� 
	dDec = g_clSysData.m_dMotorDecTime[nUnit][nAxis];		//! ����
#else
	/*���ӵ� ���� (G)
	-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
	if ((dParamAcc == 0.0) && (dParamDec == 0.0))
	{
		dAcc = g_clSysData.m_dMotorAccTime[nUnit][nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ���� 
		dDec = g_clSysData.m_dMotorDecTime[nUnit][nAxis] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ����
	}
	else
	{
		dAcc = dParamAcc * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ���� 
		dDec = dParamDec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);		//! ����
	}
#endif

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartPos(MOTOR_AXIS_NUMBER[nUseAxis], dPos, dVel, dAcc, dDec);

	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		nRetStep = 100;

		while (1)
		{
			switch (nRetStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, nAxis) == true) && ((this->GetEncoderPos(nUnit, nAxis) - (dPos / g_clSysData.m_dMotorResol[nUnit][nAxis])) < ENCORDER_GAP))
				{
					nRetStep = 200;
				}
				break;
			default:
				break;
			}

			if (nRetStep < 0)
			{
				return false;
			}

			if (nRetStep >= 200)
			{
				break;
			}

			if ((GetTickCount() - dwTickCount) > AXIS_MOVE_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT])
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	long   lAxisAll[MAX_MOTOR_COUNT] = { 0, };
	double dPosAll[MAX_MOTOR_COUNT] = { 0, };
	double dVelAll[MAX_MOTOR_COUNT] = { 0, };
	double dAccAll[MAX_MOTOR_COUNT] = { 0, };
	double dDecAll[MAX_MOTOR_COUNT] = { 0, };
	double dCurrPos = 0.0;
	int nCount;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (nAxisCnt > MAX_MOTOR_COUNT)
		return false;

	for (i = 0, nCount = 0; i < nAxisCnt; i++)
	{
		if (!m_bOrgFlag[nUnit][nAxis[i]])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� ���Ͱ� ���� �ʾҽ��ϴ�."), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (!GetStopAxis(nUnit, nAxis[i]))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �� �̵� ����� ȣ��Ǿ����ϴ�.(MoveAxisMulti)"), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (MOTOR_TYPE[nAxis[i]] == STEPING)
			dCurrPos = GetCommandPos(nUnit, nAxis[i]);
		else
			dCurrPos = GetEncoderPos(nUnit, nAxis[i]);


		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���.   MOTOR_AXIS_NUMBER
		lAxisAll[nCount] = MOTOR_AXIS_NUMBER[(nUnit * MAX_MOTOR_COUNT) + nAxis[i]];// (nUnit * MAX_MOTOR_COUNT) + nAxis[i];
		dPosAll[nCount] = dPos[i] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];
		dVelAll[nCount] = g_clSysData.m_dMotorSpeed[nUnit][nAxis[i]] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];

		// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
		if (fabs(dPosAll[nCount] - dCurrPos) < 0.005)
			continue;

#ifdef MOTOR_ACC_TYPE_SEC
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]];	//! ���ӵ�
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]];	//! ���ӵ�
#else
		/*���ӵ� ���� (G)
		-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ� 
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ�
#endif

		nCount++;
	}

	//�̵� ������ ���� ��� Return
	if (nCount == 0)
		return true;

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartMultiPos(nCount, lAxisAll, dPosAll, dVelAll, dAccAll, dDecAll);

	return true;
}


//-----------------------------------------------------------------------------
//
//	���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], int nSpeed)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	long   lAxisAll[MAX_MOTOR_COUNT] = { 0, };
	double dPosAll[MAX_MOTOR_COUNT] = { 0, };
	double dVelAll[MAX_MOTOR_COUNT] = { 0, };
	double dAccAll[MAX_MOTOR_COUNT] = { 0, };
	double dDecAll[MAX_MOTOR_COUNT] = { 0, };
	double dCurrPos = 0.0;
	int nCount;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (nAxisCnt > MAX_MOTOR_COUNT)
		return false;

	for (i = 0, nCount = 0; i < nAxisCnt; i++)
	{
		if (!m_bOrgFlag[nUnit][nAxis[i]])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� ���Ͱ� ���� �ʾҽ��ϴ�."), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (!GetStopAxis(nUnit, nAxis[i]))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �� �̵� ����� ȣ��Ǿ����ϴ�.(MoveAxisMulti)"), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (MOTOR_TYPE[nAxis[i]] == STEPING)
			dCurrPos = GetCommandPos(nUnit, nAxis[i]);
		else
			dCurrPos = GetEncoderPos(nUnit, nAxis[i]);


		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		lAxisAll[nCount] = (nUnit * MAX_MOTOR_COUNT) + nAxis[i];
		dPosAll[nCount] = dPos[i] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];
		dVelAll[nCount] = g_clSysData.m_dMotorSpeed[nUnit][nAxis[i]] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];

		// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
		if (fabs(dPosAll[nCount] - dCurrPos) < 0.005)
			continue;

#ifdef MOTOR_ACC_TYPE_SEC
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]];	//! ���ӵ�
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]];	//! ���ӵ�
#else
		/*���ӵ� ���� (G)
		-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ� 
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ�
#endif

		nCount++;
	}

	//�̵� ������ ���� ��� Return
	if (nCount == 0)
		return true;

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartMultiPos(nCount, lAxisAll, dPosAll, dVelAll, dAccAll, dDecAll);

	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], double dSpeed)
{
	TCHAR szLog[SIZE_OF_1K];
	long lAxisAll[MAX_MOTOR_COUNT] = { 0, };
	double dPosAll[MAX_MOTOR_COUNT] = { 0, };
	double dVelAll[MAX_MOTOR_COUNT] = { 0, };
	double dAccAll[MAX_MOTOR_COUNT] = { 0, };
	double dDecAll[MAX_MOTOR_COUNT] = { 0, };
	double dCurrPos = 0.0;
	int nCount;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (nAxisCnt > MAX_MOTOR_COUNT)
		return false;

	for (i = 0, nCount = 0; i < nAxisCnt; i++)
	{
		if (!m_bOrgFlag[nUnit][nAxis[i]])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �������Ͱ� ���� �ʾҽ��ϴ�."), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (!GetStopAxis(nUnit, nAxis[i]))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �� �̵� ����� ȣ��Ǿ����ϴ�.(MoveAxisMulti)"), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (MOTOR_TYPE[nAxis[i]] == STEPING)
			dCurrPos = GetCommandPos(nUnit, nAxis[i]);
		else
			dCurrPos = GetEncoderPos(nUnit, nAxis[i]);

		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		lAxisAll[nCount] = (nUnit * MAX_MOTOR_COUNT) + nAxis[i];
		dPosAll[nCount] = dPos[i] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];
		dVelAll[nCount] = dSpeed * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];

		// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
		if (fabs(dPosAll[nCount] - dCurrPos) < 0.005)
			continue;

#ifdef MOTOR_ACC_TYPE_SEC
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]];	//! ���ӵ�
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]];	//! ���ӵ�
#else
		/*���ӵ� ���� (G)
		-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
		dAccAll[nCount] = g_clSysData.m_dMotorAccTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ� 
		dDecAll[nCount] = g_clSysData.m_dMotorDecTime[nUnit][nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ�
#endif
		nCount++;
	}

	//�̵� ������ ���� ��� Return
	if (nCount == 0)
		return true;

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartMultiPos(nCount, lAxisAll, dPosAll, dVelAll, dAccAll, dDecAll);

	return true;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], double dVel[MAX_MOTOR_COUNT], double dAcc[MAX_MOTOR_COUNT], double dDec[MAX_MOTOR_COUNT])
{
	TCHAR szLog[SIZE_OF_1K];
	long lAxisAll[MAX_MOTOR_COUNT] = { 0, };
	double dPosAll[MAX_MOTOR_COUNT] = { 0, };
	double dVelAll[MAX_MOTOR_COUNT] = { 0, };
	double dAccAll[MAX_MOTOR_COUNT] = { 0, };
	double dDecAll[MAX_MOTOR_COUNT] = { 0, };
	double dCurrPos = 0;
	int nCount;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (nAxisCnt > MAX_MOTOR_COUNT)
		return false;

	for (i = 0, nCount = 0; i < nAxisCnt; i++)
	{
		if (!m_bOrgFlag[nUnit][nAxis[i]])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� �������Ͱ� ���� �ʾҽ��ϴ�."), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (!GetStopAxis(nUnit, nAxis[i]))
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �� �̵� ����� ȣ��Ǿ����ϴ�.(MoveAxisMulti)"), MOTOR_NAME[nAxis[i]]);
			AddLog(szLog, 1, nUnit);

			return false;
		}

		if (MOTOR_TYPE[nAxis[i]] == STEPING)
			dCurrPos = GetCommandPos(nUnit, nAxis[i]);
		else
			dCurrPos = GetEncoderPos(nUnit, nAxis[i]);

		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		lAxisAll[nCount] = (nUnit * MAX_MOTOR_COUNT) + nAxis[i];
		dPosAll[nCount] = dPos[i] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];
		dVelAll[nCount] = dVel[i] * g_clSysData.m_dMotorResol[nUnit][nAxis[i]];

		// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
		if (fabs(dPosAll[nCount] - dCurrPos) < 0.005)
			continue;

#ifdef MOTOR_ACC_TYPE_SEC
		dAccAll[nCount] = dAcc[i];	//! ���ӵ�
		dDecAll[nCount] = dDec[i];	//! ���ӵ�
#else
		/*���ӵ� ���� (G)
		-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
		//dAccAll[nCount] = dAcc[nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nAxis[i]]);//! ���ӵ� 
		//dDecAll[nCount] = dDec[nAxis[i]] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nAxis[i]]);//! ���ӵ�
		dAccAll[nCount] = dAcc[i] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ� 
		dDecAll[nCount] = dDec[i] * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis[i]]);//! ���ӵ�
#endif
		nCount++;
	}

	//�̵� ������ ���� ��� Return
	if (nCount == 0)
		return true;

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	AxmMoveStartMultiPos(nCount, lAxisAll, dPosAll, dVelAll, dAccAll, dDecAll);

	return true;
}

//-----------------------------------------------------------------------------
//
//	+/- Limit��ȣ�� ���ö����� �� �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveAxisLimit(int nUnit, int nAxis, double dVel, double dAcc, int nLimit, int nSignal, int nStopMode)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwRetVal = 0;
	int nUseAxis = 0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	dVel = dVel * g_clSysData.m_dMotorResol[nUnit][nAxis];
	
#ifndef MOTOR_ACC_TYPE_SEC
	/*���ӵ� ���� (G)
	-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
	dAcc = dAcc * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);//! ���ӵ� 	
#endif

	dwRetVal = AxmMoveSignalSearch(MOTOR_AXIS_NUMBER[nUseAxis], dVel, dAcc, nLimit, nSignal, nStopMode);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� Limit ��ġ �̵� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MOTOR HOME ����

//-----------------------------------------------------------------------------
//
//	Home �˻� �۾� ��� ���ڿ��� �˾Ƴ���. 
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetResultHomeSearch(int nUnit, int nAxis, CString &szResult)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	DWORD dwHomeResult = 0;
	int nUseAxis = 0;
	bool bRetVal = false;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmHomeGetResult(MOTOR_AXIS_NUMBER[nUseAxis], &dwHomeResult);

	switch (dwHomeResult)
	{
	case HOME_SUCCESS:
		szResult = _T("���� �˻��� ���������� ���� ���� ���");
		bRetVal = true;
		break;
	case HOME_SEARCHING:
		szResult = _T("���� �����˻��� �������� ���");
		break;
	case HOME_ERR_GNT_RANGE:
		szResult = _T("��Ʈ�� �������� Master��� Slave���� �����˻� ����� ������ OffsetRange�� ��������");
		break;
	case HOME_ERR_USER_BREAK:
		szResult = _T("���� �˻��� ����ڰ� ���� ����� �������� ���");
		break;
	case HOME_ERR_VELOCITY:
		szResult = _T("���� �˻��� �ӵ� ���� �� �� �� ���� 0���� �۰ų� �������");
		break;
	case HOME_ERR_AMP_FAULT:
		szResult = _T("���� �˻��� ������ �˶��� �߻��� ���");
		break;
	case HOME_ERR_NEG_LIMIT:
		szResult = _T("(+)�������� �������� ���� �� (-)����Ʈ ������ ������ ���");
		break;
	case HOME_ERR_POS_LIMIT:
		szResult = _T("(-)�������� �������� ���� �� (+)����Ʈ ������ ������ ���");
		break;
	case HOME_ERR_NOT_DETECT:
		szResult = _T("���������� �������� �������");
		break;
	case HOME_ERR_UNKNOWN:
		szResult = _T("�˼����� ä��(��) ��ȣ(0 ~ (�ִ���� - 1))�� �����˻��� �����Ϸ� �������");
		break;
	default:
		szResult = _T("Error");
		break;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//! Home�˻� ���� Ȯ��(0=Fail, 1=Success, 2=Searching)
//
//-----------------------------------------------------------------------------
int CAxlMotorSet::IsCompletedHomeSearch(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 1;
#endif

	DWORD dwRetVal = 0;
	DWORD dwHomeResult = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dwRetVal;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dwRetVal;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	AxmHomeGetResult(MOTOR_AXIS_NUMBER[nUseAxis], &dwHomeResult);

	switch (dwHomeResult)
	{
	case HOME_SEARCHING:		dwRetVal = 2;	break;
	case HOME_SUCCESS:			dwRetVal = 1;	break;
	case HOME_ERR_AMP_FAULT:	dwRetVal = -1;	break;
	case HOME_ERR_NOT_DETECT:	dwRetVal = -2;	break;
	case HOME_ERR_NEG_LIMIT:	dwRetVal = -3;	break;
	case HOME_ERR_POS_LIMIT:	dwRetVal = -4;	break;
	case HOME_ERR_UNKNOWN:		dwRetVal = -5;	break;
	default:					dwRetVal = 0;	break;
	}

	return (int)dwRetVal;
}

//-----------------------------------------------------------------------------
//
// Home �˻� �۾� ���� ���� ����, ���� %�� ��ȯ�ȴ�. 
//
//-----------------------------------------------------------------------------
int CAxlMotorSet::GetHomeSearchRate(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return 100;
#endif

	DWORD dwHomeMainStepNumber = 0;
	DWORD dwHomeStepNumber = 0;
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dwHomeStepNumber;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dwHomeStepNumber;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;	

	//! �����˻� ���۵Ǹ� �������� Ȯ���� �� �ִ�. �����˻��� �Ϸ�Ǹ� �������ο� ������� 100�� ��ȯ�ϰ� �ȴ�.
	//! uHomeMainStepNumber�� ��Ʈ���� ��� ������,�����̺갡 �����ϴ��� Ȯ���� �� �̿��Ѵ�.
	//!  (�������� ������ ���� 0�� ����, �����̺굿���� ���� 10�� �����Ѵ�.)
	AxmHomeGetRate(MOTOR_AXIS_NUMBER[nUseAxis], &dwHomeMainStepNumber, &dwHomeStepNumber);

	//! �����˻� ������(����: %)
	return dwHomeStepNumber;
}


//-----------------------------------------------------------------------------
//
//! ���� �˻��� ���Ǵ� �ӵ� ����
//
//-----------------------------------------------------------------------------
DWORD CAxlMotorSet::SetHomeSearchSpeed(int nAxis)
{
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
	double dAccCalFirst = 0.0;
	double dAccCalSecond = 0.0;
	int nUnit = 0;
	int nUseAxis = 0;
	
	nUnit = nAxis / MAX_MOTOR_COUNT;
	nUseAxis = nAxis % MAX_MOTOR_COUNT;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return AXT_RT_INVALID_MODULE_POS;

	if ((nUseAxis < 0) || (nUseAxis >= MAX_MOTOR_COUNT))
		return AXT_RT_INVALID_MODULE_POS;
	
	//! ���� �ӵ� ���� �Ұ�.	
	switch (nUseAxis)
	{
	//Y , TH , Z
	case MOTOR_PCB_X:
		dVelFirst = 5000;		dVelSecond = 1000.;		dVelThird = 500;		dvelLast = 100.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_Y:
		dVelFirst = 10000;		dVelSecond = 2000.;		dVelThird = 1000;		dvelLast = 200.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_Z:
		dVelFirst = 20000;		dVelSecond = 5000.;		dVelThird = 200;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_TH:
		dVelFirst = 4000;		dVelSecond = 2000;		dVelThird = 1000;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_TY:
		dVelFirst = 4000;		dVelSecond = 2000;		dVelThird = 1000;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_TX:
		dVelFirst = 4000;		dVelSecond = 2000;		dVelThird = 1000;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	
	/*case MOTOR_PCB_TY:
		dVelFirst = 4000;		dVelSecond = 2000.;		dVelThird = 1000;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;
	case MOTOR_PCB_TX:
		dVelFirst = 4000;		dVelSecond = 2000.;		dVelThird = 1000;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;*/
	
	}

#ifdef MOTOR_ACC_TYPE_SEC
	dAccCalFirst = dAccFirst;
	dAccCalSecond = dAccSecond; 
#else
	dAccCalFirst = dAccFirst * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nUseAxis]);
 	dAccCalSecond = dAccFirst * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nUseAxis]);		//! ���� 
#endif	

	dwReturn = AxmHomeSetVel(MOTOR_AXIS_NUMBER[nAxis], dVelFirst, dVelSecond, dVelThird, dvelLast, dAccCalFirst, dAccCalSecond);

	return dwReturn;
}


//-----------------------------------------------------------------------------
//
//	Home �˻� �۾� ����
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::StartHomeSearch(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	TCHAR szLog[SIZE_OF_1K];
	DWORD dwRetVal = 0;
	DWORD dwLevel = 0;
	int nUseAxis = 0;

	long    lHmDir = DIR_CW;			//! �����˻��� �ʱ⿡ ������ ���⼳��
	DWORD   dwHmSig = HomeSensor;		//! ���� �˻��� ��� �� ��ȣ����
	DWORD   dwZphas = DISABLE;			//! 1�� �����˻� �Ϸ� �� ���ڴ� Z�� ���� ���� ����
	double  dHClrTim = 2000.0;			//! �����˻� �Ϸ� �� ������ġ�� Encoder��ġ�� Clear�ϱ� ���� ����ϴ� �ð����� [mSec����]
	double  dHOffset = 0.0;				//! �����˻��� �Ϸ�� �� �ⱸ �������� �̵� �� ���� �缳�� �� ��ġ
	CString szResult = _T("");

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return false;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;
	
	dwRetVal = AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		return false;
	}

	if (dwLevel != ENABLE)
	{
		dwRetVal = AxmSignalServoOn(MOTOR_AXIS_NUMBER[nUseAxis], ENABLE);
		if (dwRetVal == AXT_RT_SUCCESS)
		{
			dwRetVal = AxmSignalIsServoOn(MOTOR_AXIS_NUMBER[nUseAxis], &dwLevel);
			if (dwRetVal == AXT_RT_SUCCESS)
			{
				if (dwLevel != ENABLE)
				{
					_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �˻� SERVO ON ����"), MOTOR_NAME[nAxis]);
					AddLog(szLog, 1, nUnit);

					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
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


	lHmDir = MOTOR_HOME_DIR[nAxis];
	dwHmSig = MOTOR_HOME_SIG[nAxis];

	dwRetVal = AxmHomeSetMethod(MOTOR_AXIS_NUMBER[nUseAxis], lHmDir, dwHmSig, dwZphas, dHClrTim, dHOffset);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �˻� ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	//���� �˻� �ӵ� ����
	dwRetVal = SetHomeSearchSpeed(nUseAxis);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%s] ���� ���� �ӵ� ���� ����"), MOTOR_NAME[nAxis]);
		AddLog(szLog, 1, nUnit);

		return false;
	}

	dwRetVal = AxmHomeSetStart(MOTOR_AXIS_NUMBER[nUseAxis]);
	if (dwRetVal != AXT_RT_SUCCESS)
	{ 
		this->GetResultHomeSearch(nUnit, nAxis, szResult);
		_stprintf_s(szLog, SIZE_OF_1K, _T("%s"), (TCHAR*)(LPCTSTR)szResult);
		AddLog(szLog, 0, nUnit);

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//
// Home �˻� �߿� ������ ���� ��Ŵ
//
//-----------------------------------------------------------------------------
void CAxlMotorSet::StopHomeSearch(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return;
#endif
	int nUseAxis = 0;
	
	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return;

	nUseAxis = (nUnit * MAX_MOTOR_COUNT) + nAxis;

	//AxmMoveSStop(iAxis)
	AxmHomeSetResult(MOTOR_AXIS_NUMBER[nUseAxis], HOME_ERR_USER_BREAK);
	this->StopAxis(nUnit, nAxis);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
double CAxlMotorSet::GetAccel_Cal(int nUnit, int nAxis, double dSec)
{
	double dRtnAcc = 0.0;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return dRtnAcc;

	if((nAxis < 0) || (nAxis >= MAX_MOTOR_COUNT))
		return dRtnAcc;

#ifdef MOTOR_ACC_TYPE_SEC
	dRtnAcc = dSec;
#else
	dRtnAcc = dSec * (9.8 * 1000 * g_clSysData.m_dMotorResol[nUnit][nAxis]);
#endif

	return dRtnAcc;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetHomeState(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetHomeState(int nUnit, int nAxis)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	bool bRetVal = true;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (m_bOrgFlag[nUnit][nAxis] == false)
	{
		bRetVal = false;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::GetAmpFaultState(int nUnit)
{
#ifndef ON_LINE_MOTOR
	return false;
#endif

	bool bRetVal = false;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < MAX_MOTOR_COUNT; i++)
	{
		if (this->GetAmpFault(nUnit, i) == true)
		{
			bRetVal = true;
			break;
		}
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB X,Y�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbXYMotor(int nUnit, int nPosi, double dOffsetX /*= 0.0*/, double dOffsetY /*= 0.0*/,  bool bWait /*= false*/, bool bManual /*false*/)
{
#ifndef ON_LINE_MODE
	return true;
#endif


	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;
	//if (g_clDioControl.DarkPusherCheck(nUnit, true) == false)
	//{
	//	AddLog(_T("[INFO] DARK PUSHER ��� Ȯ�� ����"), 1, nUnit);
	//	return false;
	//}
	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nPosi < 0) || (nPosi >= MAX_POS_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetX) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB X�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X], dOffsetX);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (nPosi == CHART_POS)
	{
		/*if (g_clDioControl.PcbGripCheck(nUnit, false, false) == true)
		{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[INFO] PCB�� GRIP�� ���� �ֽ��ϴ�."));
		AddLog(szLog, 1, nUnit, (g_clTaskWork[nUnit].m_nAutoFlag == MODE_AUTO) ? true : false);

		return false;
		}*/

	}

	nAxis[0] = MOTOR_PCB_X;
	nAxis[1] = MOTOR_PCB_Y;

	dPos[0] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_X] + dOffsetX;
	dPos[1] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_Y] + dOffsetY;
	



	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				nStep = 200;
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_X) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			case 300:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 400;
				}
				break;
			default:
				break;
			}

			if (nStep >= 400)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}


//-----------------------------------------------------------------------------
//
//	AAB : PCB X,Y,T�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbXYTMotor(int nUnit, int nPosi, double dOffsetX /*= 0.0*/, double dOffsetY /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/ ,bool bManual /*false*/)
{
#ifndef ON_LINE_MODE
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[3];
	double dPos[3];
	int nStep;
	int i;

	/*if (g_clDioControl.DarkPusherCheck(nUnit, true) == false)
	{
		AddLog(_T("[INFO] DARK PUSHER ��� Ȯ�� ����"), 1, nUnit);
		return false;
	}*/

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nPosi < 0) || (nPosi >= MAX_POS_COUNT))
		return false;

	for (i = 0; i < 3; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}
	if (nPosi == LASER_POS)
	{
		double dAlignPos = g_clModelData[nUnit].m_stTeachData[SENSOR_ALIGN_POS].dPos[MOTOR_PCB_Y] - 10.0;
		if (this->GetEncoderPos(nUnit, MOTOR_PCB_Y) < dAlignPos)
		{
			if (g_clMotorSet.GetPcbZMotorPosCheck(nUnit, WAIT_POS) == false)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("[INFO] PCB Z �����ġ�� �ƴմϴ�."));
				AddLog(szLog, 1, nUnit, true);
				return false;
			}
		}
		
	}
	else
	{
		if (fabs(dOffsetX) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("PCB X�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X], dOffsetX);
			AddLog(szLog, 1, nUnit);
			return false;
		}

		if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
			AddLog(szLog, 1, nUnit);
			return false;
		}

		if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
			AddLog(szLog, 1, nUnit);
			return false;
		}


		if (g_clMotorSet.GetPcbZMotorPosCheck(nUnit, WAIT_POS) == false)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[INFO] PCB Z �����ġ�� �ƴմϴ�."));
			AddLog(szLog, 1, nUnit, true);
			return false;
		}
	}
	//if (nPosi == CHART_POS || nPosi == DARK_POS || nPosi == OC_LIGHT_POS)		//, ,
	//{
	//	if (g_clDioControl.PcbSocketUpCheck(nUnit, false) == false)
	//	{
	//		_stprintf_s(szLog, SIZE_OF_1K, _T("[INFO] SOCKET ��� �����Դϴ�."));
	//		AddLog(szLog, 1, nUnit, (g_clTaskWork[nUnit].m_nAutoFlag == MODE_AUTO) ? true : false);

	//		return false;
	//	}
	
	

	nAxis[0] = MOTOR_PCB_X;
	nAxis[1] = MOTOR_PCB_Y;
	nAxis[2] = MOTOR_PCB_TH;

	dPos[0] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_X] + dOffsetX;
	dPos[1] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_Y] + dOffsetY;
	dPos[2] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_TH] + dOffsetTh;
	


	if (this->MoveAxisMulti(nUnit, 3, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[2]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_X) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			case 300:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 400;
				}
				break;
			default:
				break;
			}

			if (nStep >= 400)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB X,Y,T�� ������ġ���� ������ ��ŭ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbXYTMotor(int nUnit, double dOffsetX /*= 0.0*/, double dOffsetY /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[3];
	double dPos[3];
	int nStep;
	int i;

	/*if (g_clDioControl.DarkPusherCheck(nUnit, true) == false)
	{
		AddLog(_T("[INFO] DARK PUSHER ��� Ȯ�� ����"), 1, nUnit);
		return false;
	}*/

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < 3; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetX) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB X�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X], dOffsetX);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_X;
	nAxis[1] = MOTOR_PCB_Y;
	nAxis[2] = MOTOR_PCB_TH;

	dPos[0] = this->GetEncoderPos(nUnit, MOTOR_PCB_X) + dOffsetX;
	dPos[1] = this->GetEncoderPos(nUnit, MOTOR_PCB_Y) + dOffsetY;
	dPos[2] = this->GetEncoderPos(nUnit, MOTOR_PCB_TH) + dOffsetTh;

	if (this->MoveAxisMulti(nUnit, 3, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[2]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_X) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			case 300:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 400;
				}
				break;
			default:
				break;
			}

			if (nStep >= 400)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	AA : PCB Y,T�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbYTMotor(int nUnit, int nPosi, double dOffsetY /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/, bool bManual /*false*/)
{
#ifndef ON_LINE_MODE
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nPosi < 0) || (nPosi >= MAX_POS_COUNT))
		return false;
	if (nPosi == CHART_POS)
	{
		if (g_clDioControl.PcbSocketUpCheck(nUnit, true) == true)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("PCB SOCKET ��� �����Դϴ�."));
			AddLog(szLog, 1, nUnit);
			return false;
		}
	}
	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}


	nAxis[0] = MOTOR_PCB_Y;
	nAxis[1] = MOTOR_PCB_TH;

	dPos[0] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_Y] + dOffsetY;
	dPos[1] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_TH] + dOffsetTh;




	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	AA : PCB Y,T�� ������ġ���� ������ ��ŭ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbYTMotor(int nUnit, double dOffsetY /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;
	
	

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}


	if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_Y;
	nAxis[1] = MOTOR_PCB_TH;

	dPos[0] = this->GetEncoderPos(nUnit, MOTOR_PCB_Y) + dOffsetY;
	dPos[1] = this->GetEncoderPos(nUnit, MOTOR_PCB_TH) + dOffsetTh;

	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB X,T�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbXTMotor(int nUnit, int nPosi, double dOffsetX /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
#if 0
	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nPosi < 0) || (nPosi >= MAX_POS_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (nPosi != LASER_OUT_POS && nPosi != LASER_IN_POS)
	{
		if (fabs(dOffsetX) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X])
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("PCB X�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X], dOffsetX);
			AddLog(szLog, 1, nUnit);
			return false;
		}
	}

	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_X;
	nAxis[1] = MOTOR_PCB_TH;

	dPos[0] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_X] + dOffsetX;
	dPos[1] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_TH] + dOffsetTh;

	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_X) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			case 300:
				nStep = 400;
				break;
			default:
				break;
			}

			if (nStep >= 400)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB X,T�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbXTMotor(int nUnit, double dOffsetX /*= 0.0*/, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
#if 0
	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetX) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB X�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_X], dOffsetX);
		AddLog(szLog, 1, nUnit);
		return false;
	}	

	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_X;
	nAxis[1] = MOTOR_PCB_TH;
	
	dPos[0] = this->GetEncoderPos(nUnit, MOTOR_PCB_X) + dOffsetX;	
	dPos[1] = this->GetEncoderPos(nUnit, MOTOR_PCB_TH) + dOffsetTh;

	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TH) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_X) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_X) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;			
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}
//-----------------------------------------------------------------------------
//
//	AA : PCB T�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbTMotor(int nUnit,double dOffsetTh /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}


	if (fabs(dOffsetTh) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TH�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TH], dOffsetTh);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_TH;

	dPos[0] = this->GetEncoderPos(nUnit, MOTOR_PCB_TH) + dOffsetTh;

	if (this->MoveAxisMulti(nUnit, 1, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if (this->GetStopAxis(nUnit, MOTOR_PCB_TH) == true)
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				nStep = 300;
				break;
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
//
//	AAB : PCB Y�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbYMotor(int nUnit, int nPosi, double dOffsetY /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis = 0;
	double dPos = 0.0;
	int nStep;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if(nPosi < 0 || nPosi >= MAX_POS_COUNT)
		return false;
	

	if (fabs(dOffsetY) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Y], dOffsetY);
		AddLog(szLog, 1, nUnit);
		return false;
	}
	

	nAxis = MOTOR_PCB_Y;

	dPos = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_Y] + dOffsetY;

	if (this->MoveAxis(nUnit, nAxis, ABS, dPos, g_clSysData.m_dMotorSpeed[nUnit][MOTOR_PCB_Y]) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Y) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Y) - dPos) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;			
			default:
				break;
			}

			if (nStep >= 200)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB Z�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbZMotor(int nUnit, int nPosi, double dOffsetZ /*= 0.0*/, bool bWait /*= false*/, bool bManual)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif


	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis = 0;
	double dPos = 0.0;
	int nStep;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if(nPosi < 0 || nPosi >= MAX_POS_COUNT)
		return false;

	if (fabs(dOffsetZ) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Z])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Y�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Z], dOffsetZ);
		AddLog(szLog, 1, nUnit);
		return false;
	}
	
	nAxis = MOTOR_PCB_Z;

	dPos = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_Z] + dOffsetZ;


	if (this->MoveAxis(nUnit, nAxis, ABS, dPos, g_clSysData.m_dMotorSpeed[nUnit][MOTOR_PCB_Z]) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Z) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Z) - dPos) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			default:
				break;
			}

			if (nStep >= 200)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbZMotor(int nUnit, double dPos, bool bWait/* = false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif


	static DWORD dwTickCount;
	int nAxis;	
	int nStep;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	nAxis = MOTOR_PCB_Z;
	
	if (this->MoveAxis(nUnit, nAxis, ABS, dPos, g_clSysData.m_dMotorSpeed[nUnit][MOTOR_PCB_Z]) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_Z) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_Z) - dPos) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			default:
				break;
			}

			if (nStep >= 200)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB TX,TY�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbTxTyMotor(int nUnit, int nPosi, double dOffsetTx /*= 0.0*/, double dOffsetTy /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if ((nPosi < 0) || (nPosi >= MAX_POS_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetTx) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TX])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TX�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TX], dOffsetTx);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetTy) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TY])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TY�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TY], dOffsetTy);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_TY;
	nAxis[1] = MOTOR_PCB_TX;

	dPos[0] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_TY] + dOffsetTy; 
	dPos[1] = g_clModelData[nUnit].m_stTeachData[nPosi].dPos[MOTOR_PCB_TX] + dOffsetTx;

	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TX) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TX) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TY) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TY) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300;
				}
				break;
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : PCB TX,TY�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbTxTyMotor(int nUnit, double dOffsetTx /*= 0.0*/, double dOffsetTy /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis[2];
	double dPos[2];
	int nStep;
	int i;

	if((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	for (i = 0; i < 2; i++)
	{
		nAxis[i] = 0;
		dPos[i] = 0.0;
	}

	if (fabs(dOffsetTx) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TX])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TX�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TX], dOffsetTx);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	if (fabs(dOffsetTy) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TY])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB TY�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_TY], dOffsetTy);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis[0] = MOTOR_PCB_TY;
	nAxis[1] = MOTOR_PCB_TX;

	dPos[0] = this->GetEncoderPos(nUnit, MOTOR_PCB_TY) + dOffsetTy;
	dPos[1] = this->GetEncoderPos(nUnit, MOTOR_PCB_TX) + dOffsetTx;

	if (this->MoveAxisMulti(nUnit, 2, nAxis, dPos) == false)
	{
		return false;
	}

	if (bWait == true)
	{
		nStep = 100;
		dwTickCount = GetTickCount();

		while (1)
		{
			switch (nStep)
			{
			case 100:
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TY) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TY) - dPos[0]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 200;
				}
				break;
			case 200:
				
				if ((this->GetStopAxis(nUnit, MOTOR_PCB_TX) == true) && ((this->GetEncoderPos(nUnit, MOTOR_PCB_TX) - dPos[1]) < ENCORDER_GAP))
				{
					dwTickCount = GetTickCount();

					nStep = 300; 
				}
				break;
			default:
				break;
			}

			if (nStep >= 300)
				break;

			if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
			{
				this->StopAxisAll(nUnit);
				return false;
			}

			Sleep(10);
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//	AAB : LENS X,Y�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensXYMotor(int nUnit, int nPosi, double dOffsetX /*= 0.0*/, double dOffsetY /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS X,Y�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensXYMotor(int nUnit, double dOffsetX /*= 0.0*/, double dOffsetY /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS X�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensXMotor(int nUnit, int nPosi, double dOffsetX/* = 0.0*/, bool bWait/* = false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS Y�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensYMotor(int nUnit, int nPosi, double dOffsetY /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}


bool CAxlMotorSet::MoveLensZAAMotor(int nUnit, double dPos, double dOffsetZ /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}



//-----------------------------------------------------------------------------
//
//	AAB : LENS Z�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensZMotor(int nUnit, int nPosi, double dOffsetZ /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS Z�� ����̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensZMotor(int nUnit, double dOffsetZ /*= 0.0*/, double dVel, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	LENS Z�� Offset ��ġ�� �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MovePcbZMotorComplete(int nUnit, double dOffsetZ/* = 0.0*/, double dVel/* = 0.1*/, double dAcc/* = 0.1*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif


	static DWORD dwTickCount;
	TCHAR szLog[SIZE_OF_1K];
	int nAxis = 0;
	double dPos = 0.0;
	int nStep;

	if ((nUnit < 0) || (nUnit >= MAX_UNIT_COUNT))
		return false;

	if (fabs(dOffsetZ) > g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Z])
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("PCB Z�� ������ ��� ����[�� %.03lf]�� �ʰ� �Ͽ����ϴ�. ������ :%.03lf"), g_clModelData[nUnit].m_dErrLimit[MOTOR_PCB_Z], dOffsetZ);
		AddLog(szLog, 1, nUnit);
		return false;
	}

	nAxis = MOTOR_PCB_Z;

	dPos = this->GetEncoderPos(nUnit, MOTOR_PCB_Z) + dOffsetZ;

	if (this->MoveAxis(nUnit, nAxis, ABS, dPos, dVel, dAcc) == false)
	{
		return false;
	}

	nStep = 100;
	dwTickCount = GetTickCount();

	while (1)
	{
		switch (nStep)
		{
		case 100:
			if ((this->GetStopAxis(nUnit, MOTOR_PCB_Z) == true) && ((fabs(this->GetEncoderPos(nUnit, MOTOR_PCB_Z)) - fabs(dPos)) < ENCORDER_GAP))
			{
				dwTickCount = GetTickCount();

				nStep = 200;
			}
			break;
		default:
			break;
		}

		if (nStep >= 200)
			break;

		if ((GetTickCount() - dwTickCount) > MAX_MOTOR_MOVE_TIME)
		{
			this->StopAxisAll(nUnit);
			return false;
		}

		Sleep(10);
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//	LENS Z�� Offset ��ġ�� �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensZMotorComplete(int nUnit, double dOffsetZ/* = 0.0*/, double dVel/* = 0.1*/, double dAcc/* = 0.1*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

bool CAxlMotorSet::MoveLensZMotorCompleteAA(int nUnit, double dOffsetZ/* = 0.0*/, double dVel/* = 0.1*/, double dAcc/* = 0.1*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS TX,TY�� ƼĪ��ġ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensTxTyMotor(int nUnit, int nPosi, double dOffsetTx /*= 0.0*/, double dOffsetTy /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	AAB : LENS TX,TY�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::MoveLensTxTyMotor(int nUnit, double dOffsetTx /*= 0.0*/, double dOffsetTy /*= 0.0*/, bool bWait /*= false*/)
{
#ifndef ON_LINE_MOTOR
	return true;
#endif

	return true;
}


//-----------------------------------------------------------------------------
//
//	AAB : LENS TX,TY�� ������ġ���� ������ �̵�
//
//-----------------------------------------------------------------------------
bool CAxlMotorSet::Epoxy_Rect_Circle_Draw(int nUnit)		//�簢 , �� ���� ����
{
#ifndef ON_LINE_MOTOR
	return true;
#endif
#if 0
	TCHAR szLog[SIZE_OF_1K];
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	double dAcc = 0.0;
	double dDec = 0.0;
	double MoveStartTime = 0.0;
	double dTimeAngle = 90.0;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	bool mMotorRtn = false;
	double dTotalCurveTime = 0.0;
	double dTotalLineTime = 0.0;

	//1���� �ɸ��� �ð� = (������ * �ӵ�) / 360 /   ����/����
	// dEndDeg1ToTime = (( (dDrawLength * 1) * M_PI) / (sqrt(dCircleSpeed  * dCircleSpeed /2)) ) / 360;
	//Ŀ��
	double dCurveTotalLength = fabs(g_clModelData[nUnit].m_EpoxyCurveEndYPoint[0]);// (fabs(g_clModelData[nUnit].m_EpoxyCurveEndYPoint[0]) + fabs(g_clModelData[nUnit].m_EpoxyCurveEndYPoint[1]) + fabs(g_clModelData[nUnit].m_EpoxyCurveEndYPoint[2]) + fabs(g_clModelData[nUnit].m_EpoxyCurveEndYPoint[3]));
	dTotalCurveTime = (((dCurveTotalLength * 1) * M_PI) / (sqrt(g_clModelData[nUnit].m_EpoxyCurveSpeed  * g_clModelData[nUnit].m_EpoxyCurveSpeed / 2))) / 360;// 360;
		//dCurveTotalLength / g_clModelData[nUnit].m_EpoxyLineSpeed;
	//����
	dTotalLineTime = (g_clModelData[nUnit].m_EpoxyLineLength[0] + g_clModelData[nUnit].m_EpoxyLineLength[1] + g_clModelData[nUnit].m_EpoxyLineLength[2] + g_clModelData[nUnit].m_EpoxyLineLength[3]) / g_clModelData[nUnit].m_EpoxyLineSpeed;

	dTotalCurveTime *= 360;
	
	dEndDeg1ToTime = dTotalCurveTime + dTotalLineTime;//g_clModelData[nUnit].m_EpoxyLineLength[3] / g_clModelData[nUnit].m_EpoxyLineSpeed; 

	dEndDeg1ToTime -= g_clModelData[nUnit].m_EpoxyOffTime;


	if (dEndDeg1ToTime < 0.0)
	{
		dEndDeg1ToTime = 0.0;
	}

	int nstep = 100;
	while (1)
	{
		switch (nstep)
		{
		case 100:
			m_dwMotorTickCount = GetTickCount();
			g_clDioControl.EpoxyDischargeOn(nUnit, true);
			Sleep(10);
			if (g_clDioControl.EpoxyDischargeOn(nUnit, true) == false)
			{
				_stprintf_s(szLog, SIZE_OF_1K, _T("EPOXY ON FAIL"));
				AddLog(szLog, 1, nUnit);
				g_clDioControl.EpoxyDischargeOn(nUnit, true);
			}
			nstep = 140;
			break;
		case 140:
			//���� on Delay
			if (g_clModelData[nUnit].m_EpoxyOnTime == 0.0)
			{
				nstep = 170;
			}
			if ((GetTickCount() - m_dwMotorTickCount) > g_clModelData[nUnit].m_EpoxyOnTime * 1000)
			{
				nstep = 170;
			}
			break;
		case 170:
			mMotorRtn = MoveStraightPcbXYAxis(nUnit);

			if (mMotorRtn == true)
			{
				m_dwMotorTickCount = GetTickCount();
				nstep = 200;
			}
			else
			{
				nstep = 0;
			}
			break;
		case 200:
			nstep = 500;
			break;
		case 500:
			if (GetStopAxis(nUnit, MOTOR_PCB_X) == true && GetStopAxis(nUnit, MOTOR_PCB_Y) == true)
			{
				g_clDioControl.EpoxyDischargeOn(nUnit, false);
				Sleep(10);
				if (g_clDioControl.EpoxyDischargeOn(nUnit, false) == false)
				{
					_stprintf_s(szLog, SIZE_OF_1K, _T("EPOXY OFF FAIL"));
					AddLog(szLog, 1, nUnit);
					g_clDioControl.EpoxyDischargeOn(nUnit, false);
				}

				AxmMotSetMoveUnitPerPulse(MOTOR_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
				AxmMotSetMoveUnitPerPulse(MOTOR_PCB_Y, 1, 1);
				nstep = 900;
			}
			break;
		case 900: 
			nstep = 1000;
			break;
			/*if (g_clMotorSet.MoveAxisComplete(nUnit, MOTOR_PCB_X, REL, 3.0, g_clSysData.m_dMotorSpeed[nUnit][MOTOR_PCB_X], true) == false)
			{
				nstep = 1000;
				break;
			}*/
			break;
		default:
			nstep = 1000;
			break;
		}
		if (nstep == 1000)		break;

		if ((GetTickCount() - m_dwMotorTickCount) > 100000)
		{
			if (g_clDioControl.EpoxyDischargeOn(nUnit, false) == false)
			{
				g_clDioControl.EpoxyDischargeOn(nUnit, false);
			}
			this->StopAxisAll(nUnit);
			return false;
		}
	}
#endif
	return true;
}