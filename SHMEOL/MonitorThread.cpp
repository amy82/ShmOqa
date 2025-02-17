#include "StdAfx.h"
#include "MonitorThread.h"


CMonitorThread::CMonitorThread(void)
{
	int i = 0;

	for (i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_bPrevOrgFlag[i] = false;
	}
	m_bPrevDoorState = 0;
	m_bPrevVacuumState = false;
	m_bLampFlick = false;

	iSlaveNo = 0;
	uLatch = 0;


	m_nPortNo = 7;// ConfigData.iCommPort[COMM_SENSOR_LIGHT];
}


CMonitorThread::~CMonitorThread(void)
{
}


//-----------------------------------------------------------------------------
//
//	
//
//-----------------------------------------------------------------------------
void CMonitorThread::ThreadConstructor()
{

}

//-----------------------------------------------------------------------------
//
//	������ �Ҹ���
//
//-----------------------------------------------------------------------------
void CMonitorThread::ThreadDestructor()
{

}

//-----------------------------------------------------------------------------
//
//	������ �ݹ�
//
//-----------------------------------------------------------------------------
void CMonitorThread::ThreadCallBack()
{	
#ifdef ON_LINE_DIO

#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//read io

	g_clDioControl.eZiReadIn(0);

	///FAS_GetInput(m_nPortNo, iSlaveNo, &g_clDioControl.ezSInput, &uLatch);
#else
	g_clDioControl.ReadDIn(0);

	this->CheckDioState();
	// �������� üũ(Ÿ������)
	this->CheckAutoRunState();
	
#endif
	this->CheckAutoRunState();
	this->CheckCurtainState();
	this->CheckPcbVacuumBbState();
	Sleep(10);
#endif

	//if ((GetTickCount() - m_dwTickCount) > 500)
	//{
		//g_pCarAABonderDlg->ShowIconState(0);
		//g_pCarAABonderDlg->ShowIconState(1);

		//for(i = 0; i < MAX_UNIT_COUNT; i++)
		//{
		//	// �����尡 ���� ���̸�
		//	if(g_pCarAABonderDlg->m_clActiveAlignThread[i].GetThreadRunning() == true)
		//	{
		//		// ���� ���� ���� �ƴϸ�
		//		if (g_clTaskWork[i].m_nCurrentPcbStep < 10000 && g_clTaskWork[i].m_nCurrentPcbStep >= 20000)
		//		{
		//			bRetVal = g_clMotorSet.GetOrgFlagAll(i);
		//			if(bRetVal != m_bPrevOrgFlag[i])
		//			{
		//				g_pCarAABonderDlg->ShowHomeState(i);
		//			}
		//		}
		//	}
		//	else
		//	{
		//		bRetVal = g_clMotorSet.GetOrgFlagAll(i);
		//		if(bRetVal != m_bPrevOrgFlag[i])
		//		{
		//			g_pCarAABonderDlg->ShowHomeState(i);
		//		}
		//	}
		//}

		//m_dwTickCount = GetTickCount();
	//}
}
//-----------------------------------------------------------------------------
//
//   �������� üũ(Ÿ������)
//
//-----------------------------------------------------------------------------
void CMonitorThread::CheckAutoRunState()
{
	int i = 0; 
	//bool bAutocheck = true;
	//for (i = 0; i < MAX_UNIT_COUNT; i++)
	//{
	//	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag != MODE_AUTO)
	//	{
	//		bAutocheck = false;
	//	}
	//}
	//if (bAutocheck == true)
	//{
	//	//���Ķ� �� �ڵ�������
	//}

	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		return;
	}

	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_PAUSE)
	{
		// LAMP GREEN FLICK

		if ((GetTickCount() - m_dwTickCount) > 300)
		{
			if (m_bLampFlick == false)
			{
				//g_clDioControl.SetTowerLamp(LAMP_RED, true);
				g_clDioControl.EziSetEziTowerLamp(0, LAMP_RED, true);
			}
			else
			{
				//g_clDioControl.SetTowerLamp(LAMP_OFF, false);
				g_clDioControl.EziSetEziTowerLamp(0, LAMP_OFF, false);
			}
			m_bLampFlick = !m_bLampFlick;
			m_dwTickCount = GetTickCount();
		}
	}
	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		//g_clDioControl.SetTowerLamp(LAMP_GREEN, true);
		g_clDioControl.EziSetEziTowerLamp(0, LAMP_GREEN, true);
	}
	//
	//

	//if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_STOP)
	//{
	//	g_clDioControl.SetTowerLamp(LAMP_RED, true);
	//}
	//else if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	//{
	//	g_clDioControl.SetTowerLamp(LAMP_GREEN, true);
	//}
	//else if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_READY)
	//{
	//	// LAMP GREEN FLICK

	//	if ((GetTickCount() - m_dwTickCount) > 300)
	//	{
	//		if (m_bLampFlick == false) 
	//		{
	//			g_clDioControl.SetTowerLamp(LAMP_YELLOW, true);
	//		}
	//		else 
	//		{
	//			g_clDioControl.SetTowerLamp(LAMP_OFF, false);
	//		}
	//		m_bLampFlick = !m_bLampFlick;

	//		m_dwTickCount = GetTickCount();
	//	}
	//}
	//else
	//{
	//	g_clDioControl.SetTowerLamp(LAMP_YELLOW, true);
	//}
}



void CMonitorThread::CheckCurtainState()
{
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	if (g_clTaskWork[UNIT_AA1].m_nCurrentPcbStep >= 30120 && g_clTaskWork[UNIT_AA1].m_nCurrentPcbStep < 30125)
		return;


	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		if (g_clDioControl.CurtainDoorCheck(UNIT_AA1, true) == true)
		{
			TCHAR szLog[SIZE_OF_1K];
			g_pCarAABonderDlg->PauseAutoProcess(UNIT_AA1);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] �ڵ� ���� �� CURTAIN ����. �Ͻ� ����"));
			AddLog(szLog, 1, UNIT_AA1, true);
			return;
		}
	}



	//g_clTaskWork[m_nUnit].m_nCurrentPcbStep  30120
#else
	if (g_clTaskWork[UNIT_AA1].m_nCurrentPcbStep >= 30120 && g_clTaskWork[UNIT_AA1].m_nCurrentPcbStep < 30150)
		return;


	if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
	{
		if (g_clDioControl.EziLightCurtainCheck(UNIT_AA1) == true)
		{
			TCHAR szLog[SIZE_OF_1K];
			g_pCarAABonderDlg->PauseAutoProcess(UNIT_AA1);
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] �ڵ� ���� �� CURTAIN ����. �Ͻ� ����"));
			AddLog(szLog, 1, UNIT_AA1, true);
			return;
		}

		
	}
#endif
}

//-----------------------------------------------------------------------------
//
//   DIO ���� �����
//
//-----------------------------------------------------------------------------
void CMonitorThread::CheckDioState()
{
#ifdef ON_LINE_DIO
	TCHAR szLog[SIZE_OF_1K];
	int bState = 0;

	bState = g_clDioControl.GetDoorState();	// �� �ݰ������� true, ������ false.

	if (bState != m_bPrevDoorState)			// �ʱ� m_bPrevDoorState = false;
	{
		if (bState > 0)//// == false)
		{
			//g_clDioControl.LGDoorSignalOn(true);

			g_pCarAABonderDlg->m_clButtonExDoor[UNIT_AA1].state = 1;

			if (g_clModelData[UNIT_AA1].m_nDoorLockUse == 1)
			{
				if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO)
				{
					g_pCarAABonderDlg->PauseAutoProcess(UNIT_AA1);
					_stprintf_s(szLog, SIZE_OF_1K, _T("[ERROR] �ڵ� ���� �� DOOR ����. UNIT%d �Ͻ� ������") , UNIT_AA1 + 1);
					AddLog(szLog, 1, UNIT_AA1, true);
				}
			}
		}
		else
		{
			//g_clDioControl.LGDoorSignalOn(false);
			g_pCarAABonderDlg->m_clButtonExDoor[UNIT_AA1].state = 0;
		}

	}

	if (m_bPrevDoorState != bState)
	{
		m_bPrevDoorState = bState;
		g_pCarAABonderDlg->m_clButtonExDoor[UNIT_AA1].Invalidate();
	}
#endif
}

void CMonitorThread::CheckPcbVacuumBbState()
{
#ifdef ON_LINE_DIO
		if (g_clTaskWork[UNIT_AA1].m_nAutoFlag == MODE_AUTO && g_clTaskWork[UNIT_AA1].m_nCurrentPcbStep != 30120)
		{
			return;
		}
		bool bState = false;
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
		bState = g_clDioControl.PcbStopButtonCheck(UNIT_AA1, true);
		if (bState != m_bPrevVacuumState)
		{
			if (bState == true)
			{
				int offset = 8;		// DIO_OUT_PCB_VACUUM_ON
				int nrtn = g_clDioControl.ReadBitIn(1, offset);	//���� , Ż�� OUT ��ȣ Ȯ��

				if (nrtn == 1)	//DIO_OUT_PCB_VACUUM_ON : ON ����
				{
					g_clDioControl.PcbVacuum(UNIT_AA1, false);
				}
				else if (nrtn == 0)	//DIO_OUT_PCB_VACUUM : OFF ����
				{
					g_clDioControl.PcbVacuum(UNIT_AA1, true);
				}
			}
			//���� , Ż��
			//DIO_OUT_PCB_VACUUM_ON = on���¸� off
			//DIO_OUT_PCB_VACUUM_OFF = off ���¸� on �ϱ�
			m_bPrevVacuumState = bState;
		}
#else
		bState = g_clDioControl.EziVacuumBtnCheck(UNIT_AA1);
		if (bState != m_bPrevVacuumState)
		{
			if (bState == true)
			{
				int offset = 6;		// EZDIO_OUT_VACUUM_ON
				int nrtn = g_clDioControl.eZiReadOut(0 , offset);	//���� , Ż�� OUT ��ȣ Ȯ��
				if (nrtn == 1)
				{
					g_clDioControl.EziVacuumOn(UNIT_AA1, false);
					Sleep(100);
					g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, false);
				}
				else if (nrtn == 0)
				{
					g_clDioControl.EziVacuumOn(UNIT_AA1, true);
					Sleep(100);
					g_clDioControl.EziVacuumBtnLedOn(UNIT_AA1, true);
				}
				
			}
			m_bPrevVacuumState = bState;
		}
#endif
#endif
}
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
