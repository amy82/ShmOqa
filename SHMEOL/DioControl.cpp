#include "stdafx.h"
#include "DioControl.h"
#include "DioDefine.h"

CDioControl::CDioControl(void)
{
	int i;
	for(i = 0; i < MAX_DIO_COUNT; i++)
	{
		m_dwDIn[i] = 0;
		m_dwDOut[i] = 0;
	}

	for (i = 0; i < 2; i++)
	{
		ezSInput[i] = 0;
		ezSOutput[i] = 0;
	}

	for (i = 0; i < MAX_DIO_BYTE_COUNT; i++)
	{
		m_dByteDOutArr[i][0] = 0;
		m_dByteDOutArr[i][1] = 0;
		m_dByteDOutArr[i][2] = 0;
		m_dByteDOutArr[i][3] = 0;
	}
	// DIO �ʱ�ȭ
	//this->InitDio();
}

CDioControl::~CDioControl(void)
{
	// DIO Close
	this->CloseDio();
}

//-----------------------------------------------------------------------------
//
// DIO ��� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
bool CDioControl::InitDio()
{
#ifndef ON_LINE_DIO
	return true;
#endif

#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	return true;
#endif
	TCHAR szLog[SIZE_OF_1K];
	DWORD dwRetVal;
	DWORD dwLevel;
	long lCount;

	dwLevel = HIGH;
	dwRetVal = 0;
	lCount = 0;

	if (AxlIsOpened() == false)
	{
		dwRetVal = AxlOpenNoReset(7);
		if (dwRetVal != AXT_RT_SUCCESS)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("DIO ���̺귯�� �ʱ�ȭ ����"));
			//AddLog(szLog, 1, 999);
            AddLog(szLog, 1, 0);
            AddLog(szLog, 1, 1);
			return false;
		}
	}

	// ������ DIO ��� ���� Ȯ��
	dwRetVal = AxdInfoGetModuleCount(&lCount);
	if (dwRetVal != AXT_RT_SUCCESS)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("DIO ��� ���� Ȯ�� ����"));
		//AddLog(szLog, 1, 999);
        AddLog(szLog, 1, 0);
        AddLog(szLog, 1, 1);
		return false;
	}

	if (lCount <= 0)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("�ý��ۿ� ������ DIO ����� ����"));
		//AddLog(szLog, 1, 999);
        AddLog(szLog, 1, 0);
        AddLog(szLog, 1, 1);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
// DIO ��� Close
//
//-----------------------------------------------------------------------------
void CDioControl::CloseDio()
{
#ifndef ON_LINE_DIO
	return;
#endif

	if (AxlIsOpened())
	{
		AxlClose();
	}
}

//-----------------------------------------------------------------------------
//
// Ezi INPUT READ
//
//-----------------------------------------------------------------------------
bool CDioControl::eZiReadIn(int nModuleNo)
{
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	int m_nPortNo = g_clSysData.m_nEzi_Port[nModuleNo];
	int iSlaveNo = 0;
	DWORD uLatch = 0;


	int nRtn = FAS_GetInput(m_nPortNo, iSlaveNo, &ezSInput[nModuleNo], &uLatch);

	if (nRtn == FMM_OK)
	{
		return true;
	}
#endif
	return false;
}
//-----------------------------------------------------------------------------
//
// Ezi OUTPUT READ
//
//-----------------------------------------------------------------------------
int CDioControl::eZiReadOut(int nModuleNo, int noffset)
{
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	int m_nPortNo = g_clSysData.m_nEzi_Port[nModuleNo];
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)
	DWORD uLatch = 0;
	DWORD dwPivot = 0x01;

	int nRtn = FAS_GetOutput(m_nPortNo, iSlaveNo, &ezSOutput[nModuleNo], &uLatch);

	if (nRtn == FMM_OK)
	{
		dwPivot = (dwPivot << (noffset-1));

		if (ezSOutput[nModuleNo] & dwPivot)
		{
			return 1;
		}
		return 0;
	}
#endif
	return -1;
}
//-----------------------------------------------------------------------------
//
// INPUT READ
//
//-----------------------------------------------------------------------------
bool CDioControl::ReadDIn(int nModuleNo)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwInputVal;
	bool bRetVal;
	int nIndex;

	dwInputVal = 0;
	bRetVal = false;

	nIndex = (nModuleNo / 2);
	//
	if (AxdiReadInportDword(nModuleNo, 0, &dwInputVal) == AXT_RT_SUCCESS)
	{
		m_dwDIn[nIndex] = dwInputVal;

		bRetVal = true;
	}

	return bRetVal;
}
//-----------------------------------------------------------------------------
//
// INPUT READ
//
//-----------------------------------------------------------------------------
int CDioControl::ReadBitIn(int nModuleNo, int noffset)
{
	DWORD dwInputVal = 0;
	bool bRetVal = false;

	//noffset = 0 ~ 31
	//value = 0(Off), 1(On)
	//
	DWORD rtn = AxdoReadOutportBit(nModuleNo, noffset, &dwInputVal);
	if (rtn == AXT_RT_SUCCESS)
	{
		return (int)dwInputVal;
	}

	return -1;
	//===============================================================================================//
	// lModuleNo   : ��� ��ȣ
	// lOffset     : �Է� ������ ���� Offset ��ġ
	// *upValue    : LOW(0)
	//             : HIGH(1)
	//===============================================================================================//
	//DWORD    __stdcall AxdiReadInportBit(long lModuleNo, long lOffset, DWORD *upValue);


}

DWORD CDioControl::GetEziOut(int nUnit)
{

#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	DWORD uState = 0x00;
	DWORD uGet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	nRtn = FAS_GetOutput(mport, iSlaveNo, &uGet, &uState);

	if (nRtn == FMM_OK)
	{
		return uGet;
	}
#endif
	return 0x00;
}

//-----------------------------------------------------------------------------
//
// OUTPUT READ
//
//-----------------------------------------------------------------------------
bool CDioControl::ReadDOut(int nModuleNo)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwInputVal;
	bool bRetVal;
	int nIndex;

	dwInputVal = 0;
	bRetVal = false;

	nIndex = (nModuleNo / 2);

	if (AxdoReadOutportDword(nModuleNo, 0, &dwInputVal) == AXT_RT_SUCCESS)
	{
		m_dwDOut[nIndex] = dwInputVal;

		bRetVal = true;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
// OUTPUT READ
//
//-----------------------------------------------------------------------------
bool CDioControl::ReadByteOut(int nModuleNo, int nOffset)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwInputVal;
	bool bRetVal;
	int nIndex;

	dwInputVal = 0;
	bRetVal = false;

	nIndex = (nModuleNo / 2);

	if (AxdoReadOutportByte(nModuleNo, nOffset, &dwInputVal) == AXT_RT_SUCCESS)
	{
		m_dByteDOutArr[nIndex][nOffset] = dwInputVal;

		bRetVal = true;
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//
// OUTPUT WRITE
//
//-----------------------------------------------------------------------------
bool CDioControl::WriteDOut(int nModuleNo, DWORD dwOut)
{
	bool bRetVal;
	int nIndex;

	bRetVal = false;

	nIndex = (nModuleNo / 2);

#ifndef ON_LINE_DIO
	m_dwDOut[nIndex] = dwOut; 
	return true;
#endif

	m_clMutexWrite.Lock();

	if (AxdoWriteOutportDword(nModuleNo, 0, dwOut) == AXT_RT_SUCCESS)
	{ 
		m_dwDOut[nIndex] = dwOut;

		bRetVal = true;
	}
	m_clMutexWrite.Unlock();

	return bRetVal;
}


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CDioControl::ByteWriteDOut(int nModuleNo, int nOffset, DWORD dwOut)
{

	int nIndex = (nModuleNo / 2);
	m_clByteCriticalSection[nIndex][nOffset].Lock();
	bool bRetVal = false;

	

#ifndef ON_LINE_DIO
	m_dByteDOutArr[nIndex][nOffset] = dwOut;
	m_clByteCriticalSection[nIndex][nOffset].Unlock();
	return true;
#endif

	if (AxdoWriteOutportByte(nModuleNo, nOffset, dwOut) == AXT_RT_SUCCESS)
	{
		m_dByteDOutArr[nIndex][nOffset] = dwOut;
		bRetVal = true;
	}

	m_clByteCriticalSection[nIndex][nOffset].Unlock();
	return bRetVal;


	// 0��° ��� ���� ����� Offset 0�������� byte ������ �����͸� ����Ѵ�.
	//AxdoWriteOutportByte(0, 0, 0xFF);
	//Function AxdoReadOutportByte(long lModuleNo, long lOffset, DWORD *upValue);
	//lOffset = 0 ~ 3
	//upValue 0 ~ FF
}

//-----------------------------------------------------------------------------
//
// PCB ��ǰ Ȯ��
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbSocketVacuumCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return bFlag;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	DWORD _shift = (nUnit * 16);
	// ����
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_VACCUM_ON << _shift)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// ����
	else
	{
		if (!(m_dwDIn[ch] & DIO_IN_PCB_SOCKET_VACCUM_ON << _shift))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_VACCUM_ON << _shift)
				{
					break;
				}
			}
			else
			{
				if (!(m_dwDIn[ch] & DIO_IN_PCB_SOCKET_VACCUM_ON << _shift))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// EPOXY ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EpoxyDischargeOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	DWORD dwOutputVal;
	int iModuleNo = (nUnit * 0) + 1;
	DWORD _shift = (nUnit * 16);

	dwOutputVal = m_dwDOut[ch];

	// ���� 
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_EPOXY_ON << _shift);//DIO_OUT_EPOXY_ON
	}
	// Ż��
	else
	{
		dwOutputVal = dwOutputVal & ~(DIO_OUT_EPOXY_ON << _shift);//DIO_OUT_EPOXY_ON
	}

	if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
	{
		return false;
	}
#endif
	return true;
}

//-----------------------------------------------------------------------------
//
// LENS ����/Ż��
//
//-----------------------------------------------------------------------------
bool CDioControl::LensBufferVacuumOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	return true;
}

bool CDioControl::Start_Button_LedOn(int nUnit, bool bFlag, bool bWait)
{
	int ch = 0;
	DWORD dwTickCount;
	DWORD dwState;
	int iModuleNo = 1;
	int mOffset = 1;
	///dwState = m_dwDOut[ch];
	dwState = m_dByteDOutArr[0][mOffset];

	if (bFlag == true)
	{
		dwState = dwState | (DIO_OUT_START_PB_LED_L | DIO_OUT_START_PB_LED_R);
	}
	else
	{
		dwState = dwState & ~(DIO_OUT_START_PB_LED_L | DIO_OUT_START_PB_LED_R);
	}

	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}

	return true;
}
bool CDioControl::Stop_Button_LedOn(int nUnit, bool bFlag, bool bWait)
{
	int ch = 0;
	DWORD dwTickCount;
	DWORD dwState;
	int iModuleNo = 1;
	int mOffset = 1;
	///dwState = m_dwDOut[ch];
	dwState = m_dByteDOutArr[0][mOffset];

	if (bFlag == true)
	{
		dwState = dwState | (DIO_OUT_STOP_PB_LED);
	}
	else
	{
		dwState = dwState & ~(DIO_OUT_STOP_PB_LED);
	}


	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}
	
	return true;
}
//-----------------------------------------------------------------------------
//
// PCB ����/Ż��
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbVacuum(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
	int ch = 0;
	DWORD dwTickCount;
	DWORD dwState;
	int iModuleNo = 1;
	int mOffset = 1;
	///dwState = m_dwDOut[ch];
	dwState = m_dByteDOutArr[0][mOffset];
	if (bFlag == true)
	{
		dwState = dwState | (DIO_OUT_PCB_VACUUM_ON);
		dwState = dwState & ~(DIO_OUT_PCB_VACUUM_OFF);
	}
	// Ż��
	else
	{
		dwState = dwState | (DIO_OUT_PCB_VACUUM_OFF);
		dwState = dwState & ~(DIO_OUT_PCB_VACUUM_ON);
	}

	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}
	Sleep(500);

	if (bFlag == false)
	{
		dwState = dwState & ~(DIO_OUT_PCB_VACUUM_OFF);
	}
	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}

	Sleep(100);
	g_clDioControl.Stop_Button_LedOn(nUnit, bFlag);
	/*if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
	{
		return false;
	}*/

	return true;
}

//-----------------------------------------------------------------------------
//
// PCB ����/Ż�� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbVacuumCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & DIO_IN_PCB_VACUUM)
		{
			return true;
		}
	}
	else
	{
		if (!(m_dwDIn[ch] & DIO_IN_PCB_VACUUM))
		{
			return true;
		}
	}


	return false;
}
//-----------------------------------------------------------------------------
//
// PCB STOP ��ư 
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbStopButtonCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & DIO_IN_STOP_PB)
		{
			return true;
		}
	}
	else
	{
		if (!(m_dwDIn[ch] & DIO_IN_STOP_PB))
		{
			return true;
		}
	}


	return false;
}


//-----------------------------------------------------------------------------
//
// PCB ��/�� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbSensorCheck(int nUnit,  bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;

	if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_CHECK)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//
// PCB Grip ����/����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbGrip(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	DWORD dwOutputVal;
	int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	DWORD _shift = (nUnit * 16);
	dwOutputVal = m_dwDOut[ch];
	TCHAR szLog[SIZE_OF_1K];


	// ����
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_FORWARD << _shift);
	}
	// Ż��
	else
	{
		dwOutputVal = dwOutputVal & ~(DIO_OUT_PCB_SOCKET_FORWARD << _shift);
	}
	//if (bFlag == true)
	//{
	//	dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_FORWARD << _shift);
	//	dwOutputVal = dwOutputVal & ~(DIO_OUT_PCB_SOCKET_BACKWARD << _shift);
	//}
	//// ����
	//else
	//{
	//	dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_BACKWARD << _shift);
	//	dwOutputVal = dwOutputVal & ~(DIO_OUT_PCB_SOCKET_FORWARD << _shift);
	//}
	if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
	{
		return false;
	}
	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ch] | (DIO_IN_PCB_SOCKET_FORWARD << _shift)) 
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & ~(DIO_IN_PCB_SOCKET_FORWARD << _shift))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}
//

//-----------------------------------------------------------------------------
//
// DOOR ��ư ����
//
//-----------------------------------------------------------------------------
bool CDioControl::DoorBbCheck(int nUnit, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	DWORD dwTickCount;
	int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	DWORD _shift = (nUnit * 16);
	// ����
	if (m_dwDIn[ch] & DIO_IN_DOOR_PB << _shift)
	{
		return true;
	}
	else
	{
		return false;
	}

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{
			if (m_dwDIn[ch] & DIO_IN_DOOR_PB << _shift)
			{
				break;
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// LENS BUFFER ��/�� ���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::LensBufferSensorCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
// LENS BUFFER ��/�� ���� ����
//
//-----------------------------------------------------------------------------

bool CDioControl::LensBufferVacuumOnCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	return false;
}


bool CDioControl::CurtainDoorCheck(int nUnit, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;
	if (!(m_dwDIn[ch] & DIO_IN_LIGHT_CURTAIN))
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//
// START ��ư ����
//
//-----------------------------------------------------------------------------
bool CDioControl::StartBbCheck(int nUnit, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;

	DWORD dwState = 0;
	dwState |= DIO_IN_START_PB_L;
	dwState |= DIO_IN_START_PB_R;

	if ((m_dwDIn[ch] & dwState) == dwState)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//
// PCB Grip ����/���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbGripCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	DWORD _shift = (nUnit * 16);
	// ����
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_FORWARD << _shift)
		{
			return true;
		}
		else
		{
			return false; 
		}
	}
	// ����
	else
	{
		if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_BACKWARD << _shift)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_FORWARD << _shift)
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_BACKWARD << _shift)
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// PCB Grip ���/�ϰ�
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbGripUp(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
	//int ch = nUnit * 0;
	//static DWORD dwTickCount;
	//DWORD dwOutputVal;
	//int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	//DWORD _shift = (nUnit * 16);
	//dwOutputVal = m_dwDOut[ch];

	//// ����
	//if (bFlag == true)
	//{
	//	dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_UP << _shift);
	//	dwOutputVal = dwOutputVal & ~(DIO_OUT_PCB_SOCKET_DOWN << _shift);
	//}
	//// ����
	//else
	//{
	//	dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_DOWN << _shift);
	//	dwOutputVal = dwOutputVal & ~(DIO_OUT_PCB_SOCKET_UP << _shift);
	//}

	//this->WriteDOut(iModuleNo, dwOutputVal);

	//if (bWait == true)
	//{
	//	dwTickCount = GetTickCount();
	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[ch] & (DIO_IN_PCB_SOCKET_UP << _shift))
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[ch] & (DIO_IN_PCB_SOCKET_DOWN << _shift))
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	//Sleep((DWORD)(g_clModelData[nUnit].m_dPCBGripUpDelay * 1000));

	return true;
}

//-----------------------------------------------------------------------------
//
// PCB Grip ���/�ϰ� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbGripUpCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//int ch = nUnit * 0;
	//static DWORD dwTickCount;
	//int iModuleNo = (nUnit * 0) + 1;// (nUnit * 2) + 1;
	//DWORD _shift = (nUnit * 16);
	//// ���
	//if (bFlag == true)
	//{
	//	if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_UP << _shift)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//// �ϰ�
	//else
	//{
	//	if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_DOWN << _shift)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//if (bWait == true)
	//{

	//	dwTickCount = GetTickCount();

	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_UP << _shift)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[ch] & DIO_IN_PCB_SOCKET_DOWN << _shift)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}


//-----------------------------------------------------------------------------
//
// LENS ��ǰ Ȯ��
//
//-----------------------------------------------------------------------------
bool CDioControl::LensSocketCheck(int nUnit, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	static DWORD dwTickCount;

	if (m_dwDIn[nUnit] & DIO_IN_LENS_SOCKET_CHECK)
	{
		return true;
	}
	else
	{
		return false;
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (m_dwDIn[nUnit] & DIO_IN_LENS_SOCKET_CHECK)
			{
				break;
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}


bool CDioControl::CurtainUpDown(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	DWORD dwTickCount;
	DWORD dwOutputVal;
	int iModuleNo = (nUnit * 2) + 1;

	dwOutputVal = m_dwDOut[nUnit];

	// ����
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | DIO_OUT_DOOR_SHUTTER_UP;
		dwOutputVal = dwOutputVal & ~DIO_OUT_DOOR_SHUTTER_DOWN;
	}
	// ����
	else
	{
		dwOutputVal = dwOutputVal | DIO_OUT_DOOR_SHUTTER_DOWN;
		dwOutputVal = dwOutputVal & ~DIO_OUT_DOOR_SHUTTER_UP;
	}

	if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
		return false;

	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_UP)
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_DOWN)
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > 5000)
			{
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}


bool CDioControl::CurtainUpDownCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	DWORD dwTickCount;

	// ����
	if (bFlag == true)
	{
		if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_UP)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// ����
	else
	{
		if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_DOWN)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_UP)
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[nUnit] & DIO_IN_DOOR_SHUTTER_DOWN)
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}
#endif
	return true;
}

//----------------------------------------------------------------------------
//
// PCB SOCKET UP / DOWN
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbSocketUp(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int outCh = 0;
	static DWORD dwTickCount;
	DWORD dwOutputVal;
	int iModuleNo = 1;
	
	DWORD dwState = 0;
	dwOutputVal = m_dwDOut[outCh];
	DWORD _shift = (nUnit * 16);
	// ����
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_UP << _shift);
		dwOutputVal = dwOutputVal & (~DIO_OUT_PCB_SOCKET_DOWN << _shift);
	}
	// ����
	else
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_PCB_SOCKET_DOWN << _shift);
		dwOutputVal = dwOutputVal & (~DIO_OUT_PCB_SOCKET_UP << _shift);
	}

	if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
		return false;

	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		while (1)
		{
			if (bFlag == true)
			{
				dwState |= (DIO_IN_PCB_SOCKET_UP << _shift);
				if ((m_dwDIn[nUnit] & dwState) == dwState)//if (m_dwDIn[nUnit] & DIO_IN_LENS_GRIP)
				{
					break;
				}
			}
			else
			{
				dwState |= (DIO_IN_PCB_SOCKET_DOWN << _shift);
				if ((m_dwDIn[nUnit] & dwState) == dwState)//if (m_dwDIn[nUnit] & DIO_IN_LENS_UNGRIP)
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// PCB SOCKET UP / DOWN ����
//
//-----------------------------------------------------------------------------
bool CDioControl::PcbSocketUpCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	DWORD dwTickCount;
	DWORD dwState = 0;
	int ioIndex = 0; 
	DWORD _shift = (nUnit * 16);
	// ����
	if (bFlag == true)
	{
		if (m_dwDIn[ioIndex] & (DIO_IN_PCB_SOCKET_UP << _shift))
		{
			return true;
		}
		else
		{
			if (bWait == false)
			{
				return false;
			}
		}
	}
	// ����
	else
	{
		if (m_dwDIn[ioIndex] & (DIO_IN_PCB_SOCKET_DOWN << _shift))
		{
			return true;
		}
		else
		{
			if (bWait == false)
			{
				return false;
			}
		}
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ioIndex] & (DIO_IN_PCB_SOCKET_UP << _shift))
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ioIndex] & (DIO_IN_PCB_SOCKET_DOWN << _shift))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// LENS Grip ����/����
//
//-----------------------------------------------------------------------------
bool CDioControl::LensGrip(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
// LENS Grip ����/���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::LensGripCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
	return true;
}

//-----------------------------------------------------------------------------
//
// LENS Stage Grip ����/����
//
//-----------------------------------------------------------------------------
bool CDioControl::LensStageGrip(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;
	//DWORD dwOutputVal;
	//int iModuleNo = (nUnit * 2) + 1;

	//dwOutputVal = m_dwDOut[nUnit];

	//// ����
	//if (bFlag == true)
	//{
	//	dwOutputVal = dwOutputVal | DIO_OUT_LENS_STAGE_GRIP_FOR;
	//	dwOutputVal = dwOutputVal & ~DIO_OUT_LENS_STAGE_GRIP_BACK;
	//}
	//// ����
	//else
	//{
	//	dwOutputVal = dwOutputVal | DIO_OUT_LENS_STAGE_GRIP_BACK;
	//	dwOutputVal = dwOutputVal & ~DIO_OUT_LENS_STAGE_GRIP_FOR;
	//}

	//this->WriteDOut(iModuleNo, dwOutputVal);

	//if (bWait == true)
	//{
	//	dwTickCount = GetTickCount();
	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_FOR)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_BACK)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
// LENS Stage Grip ����/���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::LensStageGripCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;

	//// ����
	//if (bFlag == true)
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_FOR)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//// ����
	//else
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_BACK)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//if (bWait == true)
	//{

	//	dwTickCount = GetTickCount();

	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_FOR)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_LENS_STAGE_GRIP_BACK)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
// �˻� FEED ����/ ����
//
//-----------------------------------------------------------------------------
bool CDioControl::FeedMove(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;
	//DWORD dwOutputVal;
	//int iModuleNo = (nUnit * 2) + 1;

	//dwOutputVal = m_dwDOut[nUnit];

	//// ����
	//if (bFlag == true)
	//{
	//	dwOutputVal = dwOutputVal | DIO_OUT_FEED_FORWARD;
	//	dwOutputVal = dwOutputVal & ~DIO_OUT_FEED_BACKWARD;
	//}
	//// ����
	//else
	//{
	//	dwOutputVal = dwOutputVal | DIO_OUT_FEED_BACKWARD;
	//	dwOutputVal = dwOutputVal & ~DIO_OUT_FEED_FORWARD;
	//}

	//this->WriteDOut(iModuleNo, dwOutputVal);

	//if (bWait == true)
	//{
	//	dwTickCount = GetTickCount();
	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_FEED_FORWARD)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_FEED_BACKWARD)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
// �˻� FEED ����/ ���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::FeedMoveCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;

	//// ����
	//if (bFlag == true)
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_FEED_FORWARD)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//// ����
	//else
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_FEED_BACKWARD)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//if (bWait == true)
	//{

	//	dwTickCount = GetTickCount();

	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_FEED_FORWARD)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_FEED_BACKWARD)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
// ���� ��� / �ϰ�
//
//-----------------------------------------------------------------------------
bool CDioControl::ContactUp(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	DWORD dwOutputVal;

	int iModuleNo = 1;

	
	dwOutputVal = m_dwDOut[ch];



	// ����
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_CONTACT_UP);
		dwOutputVal = dwOutputVal & ~(DIO_OUT_CONTACT_DOWN);
	}
	// ����
	else
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_CONTACT_DOWN);
		dwOutputVal = dwOutputVal & ~(DIO_OUT_CONTACT_UP);
	}

	

	this->WriteDOut(iModuleNo, dwOutputVal);

	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		while (1)
		{ 
			if (bFlag == true)
			{
				if (m_dwDIn[ch] & (DIO_IN_CONTACT_UP))
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & (DIO_IN_CONTACT_DOWN))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// ���� ��� / �ϰ� Ȯ��
//
//-----------------------------------------------------------------------------
bool CDioControl::ContactUpcheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;

	// ���
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & (DIO_IN_CONTACT_UP))
		{
			return true;
		}
		else
		{
			if (bWait == false)
			{
				return false;
			}
		}
	}
	// �ϰ�
	else
	{
		if (m_dwDIn[ch] & (DIO_IN_CONTACT_DOWN))
		{
			return true;
		}
		else
		{
			if (bWait == false)
			{
				return false;
			}
		}
	}

	

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{

			if (bFlag == true)
			{
				if (m_dwDIn[ch] & (DIO_IN_CONTACT_UP))
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & (DIO_IN_CONTACT_DOWN))
				{
					break;
				}
			}
			

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// DARK PUSHER ��� / �ϰ�
//
//-----------------------------------------------------------------------------
bool CDioControl::DarkPusher(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	DWORD dwOutputVal;
	int iModuleNo = 1;
	DWORD _shift = (nUnit * 16);
	dwOutputVal = m_dwDOut[ch];

	// ���
	if (bFlag == true)
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_DARK_PUSHER_UP); 
		dwOutputVal = dwOutputVal & ~(DIO_OUT_DARK_PUSHER_DOWN);
	}
	// �ϰ�
	else
	{
		dwOutputVal = dwOutputVal | (DIO_OUT_DARK_PUSHER_DOWN);
		dwOutputVal = dwOutputVal & ~(DIO_OUT_DARK_PUSHER_UP);
	}

	this->WriteDOut(iModuleNo, dwOutputVal);

	if (bWait == true)
	{
		dwTickCount = GetTickCount();
		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_UP))
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_DOWN)) 
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
// DARK PUSHER ��� / �ϰ� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::DarkPusherCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if 0
	int ch = nUnit * 0;
	static DWORD dwTickCount;
	DWORD _shift = (nUnit * 16);
	// ���
	if (bFlag == true)
	{
		if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_UP))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// �ϰ�
	else
	{
		if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_DOWN))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{

		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_UP))
				{
					break;
				}
			}
			else
			{
				if (m_dwDIn[ch] & (DIO_IN_DARK_PUSHER_DOWN))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
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
//	UV ���� On/Off
//
//-----------------------------------------------------------------------------
bool CDioControl::UVOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	return true;
}

//-----------------------------------------------------------------------------
//
//	UV ���� On/Off ����
//
//-----------------------------------------------------------------------------
bool CDioControl::UVOnCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;

	//// UV ON
	//if (bFlag == true)
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_UV_ON)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (!(m_dwDIn[nUnit] & DIO_IN_UV_ON))
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//if (bWait == true)
	//{
	//	dwTickCount = GetTickCount();

	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_UV_ON)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (!(m_dwDIn[nUnit] & DIO_IN_UV_ON))
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
//	UV SHUTTER OPEN/CLOSE
//
//-----------------------------------------------------------------------------
bool CDioControl::UVShutter(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;
	//DWORD dwOutput;
	//int iModuelNo = (nUnit * 2) + 1;

	//dwOutput = m_dwDOut[nUnit];

	//// SHUTTER OPEN
	//if (bFlag == true)
	//{
	//	dwOutput |= DIO_OUT_UV_SHUTTER;
	//}
	//// SHUTTER CLOSE
	//else
	//{
	//	dwOutput &= ~DIO_OUT_UV_SHUTTER;
	//}

	//this->WriteDOut(iModuelNo, dwOutput);

	//if (bWait == true)
	//{
	//	dwTickCount = GetTickCount();
	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_UV_SHUTTER_OPEN)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (!(m_dwDIn[nUnit] & DIO_IN_UV_SHUTTER_CLOSE))
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
//	UV SHUTTER ����
//
//-----------------------------------------------------------------------------
bool CDioControl::UVShutterCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	//static DWORD dwTickCount;

	//// OPEN
	//if (bFlag == true)
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_UV_SHUTTER_OPEN)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//// CLOSE
	//else
	//{
	//	if (m_dwDIn[nUnit] & DIO_IN_DARK_PUSHER_DOWN)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//if (bWait == true)
	//{

	//	dwTickCount = GetTickCount();

	//	while (1)
	//	{
	//		if (bFlag == true)
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_UV_SHUTTER_OPEN)
	//			{
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (m_dwDIn[nUnit] & DIO_IN_UV_SHUTTER_CLOSE)
	//			{
	//				break;
	//			}
	//		}

	//		if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
	//		{
	//			return false;
	//		}

	//		Sleep(10);
	//	}
	//}

	return true;
}

//-----------------------------------------------------------------------------
//
//	UV ERROR ����
//
//-----------------------------------------------------------------------------
bool CDioControl::UVErrorCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return false;
#endif

	/*static DWORD dwTickCount;

	if (bFlag == true)
	{
		if (m_dwDIn[nUnit] & DIO_IN_UV_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (!(m_dwDIn[nUnit] & DIO_IN_UV_ERROR))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_UV_ERROR)
				{
					break;
				}
			}
			else
			{
				if (!(m_dwDIn[nUnit] & DIO_IN_UV_ERROR))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}*/
	return true;
}

//-----------------------------------------------------------------------------
//
//	UV READY ���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::UVReadyCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	/*static DWORD dwTickCount;

	if (bFlag == true)
	{
		if (m_dwDIn[nUnit] & DIO_IN_UV_READY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (!(m_dwDIn[nUnit] & DIO_IN_UV_READY))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_UV_READY)
				{
					break;
				}
			}
			else
			{
				if (!(m_dwDIn[nUnit] & DIO_IN_UV_READY))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}*/
	return true;
}

//-----------------------------------------------------------------------------
//
//	UV ������ MAX/MIN ����
//
//-----------------------------------------------------------------------------
bool CDioControl::UVApertureCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	/*static DWORD dwTickCount;

	if (bFlag == true)
	{
		if (m_dwDIn[nUnit] & DIO_IN_UV_APERTURE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (!(m_dwDIn[nUnit] & DIO_IN_UV_APERTURE))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_UV_APERTURE)
				{
					break;
				}
			}
			else
			{
				if (!(m_dwDIn[nUnit] & DIO_IN_UV_APERTURE))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}*/
	return true;
}

//-----------------------------------------------------------------------------
//
//	UV ���ð� 
//
//-----------------------------------------------------------------------------
bool CDioControl::UVUseTimeCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	/*static DWORD dwTickCount;

	if (bFlag == true)
	{
		if (m_dwDIn[nUnit] & DIO_IN_UV_USE_TIME)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (!(m_dwDIn[nUnit] & DIO_IN_UV_USE_TIME))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (bWait == true)
	{
		dwTickCount = GetTickCount();

		while (1)
		{
			if (bFlag == true)
			{
				if (m_dwDIn[nUnit] & DIO_IN_UV_USE_TIME)
				{
					break;
				}
			}
			else
			{
				if (!(m_dwDIn[nUnit] & DIO_IN_UV_USE_TIME))
				{
					break;
				}
			}

			if ((GetTickCount() - dwTickCount) > MAX_WAIT_TIME)
			{
				return false;
			}

			Sleep(10);
		}
	}*/
	return true;
}


//-----------------------------------------------------------------------------
//
//	DOOR ���� ��ȯ
//
//-----------------------------------------------------------------------------
int CDioControl::GetDoorState()
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwState = 0;
	DWORD dwSumState = 0;


	dwState |= DIO_IN_DOOR_OPEN_1;
	dwSumState = m_dwDIn[0] & dwState;
	dwState |= DIO_IN_DOOR_OPEN_2;
	dwSumState = m_dwDIn[0] & dwState;
	dwState |= DIO_IN_DOOR_OPEN_3;
	dwSumState = m_dwDIn[0] & dwState;

	//1 = ���� ���� ����
	//2 = �޸� ���� ����
	//3 = ����,�޸� ����
	//4 = ����
	//5 = ��,�� ����
	//6 = �ڸ�, ����
	//7 = 3���� ����

	return dwSumState;
}

bool CDioControl::SetBuzzer(bool bOn, int nType)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON______EOL_MODE)
	DWORD dwState;
	int iModuleNo = 1;
	int mOffset = 0;
	///dwState = m_dwDOut[0];
	dwState = m_dByteDOutArr[0][mOffset];
	// BUZZER OFF
	dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);

	if (bOn == true)
	{
		switch (nType)
		{
		case 0:
			dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
			break;
		case 1:
			dwState |= DIO_OUT_BUZZER_1;
			dwState &= ~(DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
			break;
		case 2:
			dwState |= DIO_OUT_BUZZER_3;
			dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_4);
			break;
		default:
			dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
			break;
		}
	}
	/*if (this->WriteDOut(iModuleNo, dwState) == false) {
		return false;
	}*/
	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}
#else
	EziBuzzerOn(0, bOn);
#endif
	return true;
}

//-----------------------------------------------------------------------------
//
//	Ÿ������ ��Ʈ��
//
//-----------------------------------------------------------------------------
bool CDioControl::SetTowerLamp(int nType, bool bOn)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwState = 0;
	int iModuleNo = 1;
	int mOffset = 0;
	//dwState = m_dwDOut[0];
	dwState = m_dByteDOutArr[0][mOffset];
	switch (nType)
	{
		// ALL OFF
	case 0:
		dwState &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_G | DIO_OUT_TOWER_LAMP_Y);
		break;
		// LAMP RED
	case 1:
		if (bOn == true)
		{
			dwState |= DIO_OUT_TOWER_LAMP_R;
			dwState &= ~(DIO_OUT_TOWER_LAMP_G | DIO_OUT_TOWER_LAMP_Y);
		}
		else
		{
			dwState &= ~DIO_OUT_TOWER_LAMP_R;
		}
		break;
		// LAMP GREEN
	case 2:
		if (bOn == true)
		{
			dwState |= DIO_OUT_TOWER_LAMP_G;
			dwState &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_Y);
		}
		else
		{
			dwState &= ~DIO_OUT_TOWER_LAMP_G;
		}
		break;
		// LAMP YELLOW
	case 3:
		if (bOn == true)
		{
			dwState |= DIO_OUT_TOWER_LAMP_Y;
			dwState &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_G);
		}
		else
		{
			dwState &= ~DIO_OUT_TOWER_LAMP_Y;
		}
		break;
	}

	/*if (this->WriteDOut(iModuleNo, dwOutputVal) == false)
	{
		return false;
	}*/
	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CDioControl::SetBuzzer(int nType)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	DWORD dwState = 0;
	int nModuleNo = 1;
	int mOutIndex = 0;
	int mOffset = 1;
	////dwState = m_dwDOut[0];
	dwState = m_dByteDOutArr[0][mOffset];
	switch (nType)
	{
		// BUZZER OFF
	case 0:
		dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
		break;
		// BUZZER 1
	case 1:
		dwState |= DIO_OUT_BUZZER_1;
		dwState &= ~(DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
		break;
		// BUZZER 2
	case 2:
		dwState |= DIO_OUT_BUZZER_2;
		dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_3 | DIO_OUT_BUZZER_4);
		break;
		// BUZZER 3
	case 3:
		dwState |= DIO_OUT_BUZZER_3;
		dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_4);
		break;
		// BUZZER 4
	case 4:
		dwState |= DIO_OUT_BUZZER_4;
		dwState &= ~(DIO_OUT_BUZZER_1 | DIO_OUT_BUZZER_2 | DIO_OUT_BUZZER_3);
		break;
	}

	/*if(this->WriteDOut(nModuleNo, dwState) == false)
		return false;*/
	if (this->ByteWriteDOut(nModuleNo, mOffset, dwState) == false)
	{
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
//
//	DOOR UNLOCK ����
//
//-----------------------------------------------------------------------------
bool CDioControl::DoorUnLock(int index, bool bFlag)
{
#ifndef ON_LINE_DIO
	return false;
#endif
	DWORD dwState = 0;
	int nModuleNo = 1;
	int mOffset = 1;
	////dwState = m_dwDOut[0];
	dwState = m_dByteDOutArr[0][mOffset];

	//������� led�� ������ open�����̴�.
	if (bFlag == true)
	{
		dwState |= DIO_OUT_DOOR_UNLOCK_ALL;
		//dwState = dwState | (DIO_OUT_DOOR_UNLOCK_ALL);
	}
	else
	{
		dwState &= ~DIO_OUT_DOOR_UNLOCK_ALL;
		//dwState = dwState & ~(DIO_OUT_DOOR_UNLOCK_ALL);
	}

	/*if (this->WriteDOut(nModuleNo, dwState) == false)
	{
		return false;
	}*/

	if (this->ByteWriteDOut(nModuleNo, mOffset, dwState) == false)
	{
		return false;
	}

	return true;

}


//-----------------------------------------------------------------------------
//
//	DOOR UNLOCK���� ��ȯ
//
//-----------------------------------------------------------------------------
bool CDioControl::GetDoorUnLockState()
{
#ifndef ON_LINE_DIO
	return true;
#endif
	DWORD dwState = 0;
	dwState |= DIO_IN_DOOR_OPEN_1;
	dwState |= DIO_IN_DOOR_OPEN_2;
	dwState |= DIO_IN_DOOR_OPEN_3;


	DWORD dwSumState = m_dwDIn[0] & dwState;

	if (dwSumState > 0)
	{
		return false;
	}
	return true;
}



//-----------------------------------------------------------------------------
//
//	LG DOOR ��ȣ
//
//-----------------------------------------------------------------------------
bool CDioControl::LGDoorSignalOn(bool bFlag)
{
#ifndef ON_LINE_DIO
	return true;
#endif

	int ch = 0;
	DWORD dwTickCount;
	DWORD dwState;
	int iModuleNo = 1;
	int mOffset = 3;
	dwState = m_dByteDOutArr[0][mOffset];

	if (bFlag == true)
	{
		dwState = dwState | (DIO_OUT_LG_DOOR);
	}
	// Ż��
	else
	{
		dwState = dwState & ~(DIO_OUT_LG_DOOR);
	}

	if (this->ByteWriteDOut(iModuleNo, mOffset, dwState) == false)
	{
		return false;
	}

	return true;

}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
bool CDioControl::eZiWriteOut(int nUnit, DWORD dwPrevOut , DWORD dwCurOut)
{
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];


	uSet = dwPrevOut;

	uClear = dwCurOut;

	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}
#endif
	return false;
}

//----------------------------------------------------------------------------
//
// PCB SOCKET ���� / ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziPcbSocketFor(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)
	{
		uSet = EZDIO_OUT_SOCKET_Y_FOR;
		uClear = EZDIO_OUT_SOCKET_Y_BACK;
	}
	else
	{
		uSet = EZDIO_OUT_SOCKET_Y_BACK;
		uClear = EZDIO_OUT_SOCKET_Y_FOR;
	}
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}

	return false;
	//unsigned long uSet = 0x0000FF00; (Output 8~15 ���� On �մϴ�.)	1111 1111 0000 0000 (FF00)
	//unsigned long uClear = 0x000000FF; (Output 0~7 ���� Off �մϴ�.)	0000 0000 1111 1111 (00FF)
	//FMM_OK: ����� ���������� ����Ǿ����ϴ�.
	//FMM_INVALID_PORT_NUM : ������ Port �� nPort�� �������� �ʽ��ϴ�.
	//FMM_INVALID_SLAVE_NUM : �ش� Port �� iSlaveNo �� Slave �� �������� �ʽ��ϴ�.
	/*if (nRtn == FMM_OK)
	{
	return true;
	}else if (nRtn == FMM_INVALID_PORT_NUM)
	{
	return false;
	}else if (nRtn == FMM_INVALID_SLAVE_NUM)
	{
	return false;
	}*/
#endif
	return false;
}


//-----------------------------------------------------------------------------
//
// PCB SOCKET ���� / ���� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziPcbSocketForCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	if (bFlag == true)
	{
		if (ezSInput[nUnit] & EZDIO_IN_SOCKET_Y_FOR)
		{
			return true;
		}
	}
	else
	{
		if (ezSInput[nUnit] & EZDIO_IN_SOCKET_Y_BACK)
		{
			return true;
		}
	}
#endif

	return false;
}

//----------------------------------------------------------------------------
//
// �̹����� ��� / �ϰ�
//
//-----------------------------------------------------------------------------
bool CDioControl::EziOcUp(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)
	{
		uSet = EZDIO_OUT_OC_Z_UP;
		uClear = EZDIO_OUT_OC_Z_DOWN;
	}
	else
	{
		uSet = EZDIO_OUT_OC_Z_DOWN;
		uClear = EZDIO_OUT_OC_Z_UP;
	}
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}
	
#endif
	return false;
}


//-----------------------------------------------------------------------------
//
// �̹����� ��� / �ϰ� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziOcUpCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	if (bFlag == true)
	{
		if (ezSInput[nUnit] & EZDIO_IN_OC_Z_UP)
		{
			return true;
		}
	}
	else
	{
		if (ezSInput[nUnit] & EZDIO_IN_OC_Z_DOWN)
		{
			return true;
		}
	}
#endif

	return false;
}

//----------------------------------------------------------------------------
//
// ���� ���� / Ż��
//
//-----------------------------------------------------------------------------
bool CDioControl::EziVacuumOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)//
	{
		uSet = EZDIO_OUT_VACUUM_ON;
		uClear = EZDIO_OUT_VACUUM_OFF;
	}
	else
	{
		uSet = EZDIO_OUT_VACUUM_OFF;
		uClear = EZDIO_OUT_VACUUM_ON;
	}
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{

		if (bFlag == false)
		{
			Sleep(500);
			uSet = 0x00;
			uClear = EZDIO_OUT_VACUUM_OFF;
			FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

		}
		return true;
	}

#endif
	return false;
}

//-----------------------------------------------------------------------------
//
// ���� ���� / Ż�� ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziVacuumOnCheck(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	if (bFlag == true)
	{
		if (ezSInput[nUnit] & EZDIO_IN_PCB_VACUUM)
		{
			return true;
		}
	}
	else
	{
		//if (ezSInput[nUnit] & EZDIO_IN_PCB_VACUUM)
		if (!(ezSInput[nUnit] & DIO_IN_PCB_VACUUM))
		{
			return true;
		}
	}
#endif

	return false;
}

//----------------------------------------------------------------------------
//
// START BUTTON LED ON
//
//-----------------------------------------------------------------------------
bool CDioControl::EziStartBtnLedOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)
	{
		uSet = EZDIO_OUT_START_BUTTON_L | EZDIO_OUT_START_BUTTON_R;
	}
	else
	{
		uClear = EZDIO_OUT_START_BUTTON_L | EZDIO_OUT_START_BUTTON_R;
	}
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}

#endif
	return false;
}

//-----------------------------------------------------------------------------
//
// START BUTTON PRESS ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziStartBtnOnCheck(int nUnit, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	DWORD dwState = 0;
	dwState |= EZDIO_IN_START_BUTTON_L;
	dwState |= EZDIO_IN_START_BUTTON_R;

	//if (ezSInput[nUnit] & dwState)
	if ((ezSInput[nUnit] & dwState) == dwState)
	{
		return true;
	}
#endif

	return false;
}


//
//----------------------------------------------------------------------------
//
// ���� BUTTON LED ON
//
//-----------------------------------------------------------------------------
bool CDioControl::EziVacuumBtnLedOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)
	{
		uSet = EZDIO_OUT_PCB_BUTTON_VACUUM_LED;
	}
	else
	{
		uClear = EZDIO_OUT_PCB_BUTTON_VACUUM_LED;
	}
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}

#endif
	return false;
}

//----------------------------------------------------------------------------
//
// BUZZER ON
//
//-----------------------------------------------------------------------------
bool CDioControl::EziBuzzerOn(int nUnit, bool bFlag, bool bWait)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)		//socket For
	//
	//
	TCHAR szLog[SIZE_OF_1K];
	DWORD uClear = 0x00;
	DWORD uSet = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];

	if (bFlag == true)
	{
		uSet = EZDIO_OUT_BUZZER;
	}
	else
	{
		uClear = EZDIO_OUT_BUZZER;
	}

	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}

#endif
	return false;
}

//-----------------------------------------------------------------------------
//
//	Ÿ������ ��Ʈ��
//
//-----------------------------------------------------------------------------
bool CDioControl::EziSetEziTowerLamp(int nUnit, int nType, bool bOn)
{
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	DWORD uSet = 0x00;
	DWORD uClear = 0x00;
	BYTE iSlaveNo = 1; // Slave No (0 ~ 15)

	int nRtn = -1;
	int mport = g_clSysData.m_nEzi_Port[nUnit];


	switch (nType)
	{
		case LAMP_OFF:	// ALL OFF
			uClear = EZDIO_OUT_TOWER_LAMP_R | EZDIO_OUT_TOWER_LAMP_G | EZDIO_OUT_TOWER_LAMP_Y;
			break;
		case LAMP_RED:	// LAMP RED
			if (bOn == true)
			{
				uSet = EZDIO_OUT_TOWER_LAMP_R;
				uClear = EZDIO_OUT_TOWER_LAMP_G | EZDIO_OUT_TOWER_LAMP_Y;
			}
			else
			{
				uClear = EZDIO_OUT_TOWER_LAMP_R;
			}
			break;
		case LAMP_GREEN:	// LAMP GREEN
			if (bOn == true)
			{
				uSet = EZDIO_OUT_TOWER_LAMP_G;
				uClear = EZDIO_OUT_TOWER_LAMP_R | EZDIO_OUT_TOWER_LAMP_Y;
			}
			else
			{
				uClear = DIO_OUT_TOWER_LAMP_G;
			}
			break;
		case LAMP_YELLOW:	// LAMP YELLOW
			if (bOn == true)
			{
				uSet = EZDIO_OUT_TOWER_LAMP_Y;
				uClear = EZDIO_OUT_TOWER_LAMP_R | EZDIO_OUT_TOWER_LAMP_G;
			}
			else
			{
				uClear = EZDIO_OUT_TOWER_LAMP_Y;
			}
			break;
	}
	
	nRtn = FAS_SetOutput(mport, iSlaveNo, uSet, uClear);

	if (nRtn == FMM_OK)
	{
		return true;
	}
#endif
	return true;
}


//-----------------------------------------------------------------------------
//
// ���� ��ư ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziVacuumBtnCheck(int nUnit)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	if (ezSInput[nUnit] & EZDIO_IN_PCB_BUTTON_VACUUM)
	{
		return true;
	}
#endif

	return false;
}


//-----------------------------------------------------------------------------
//
// ��ǰ ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziPcbCheck(int nUnit)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	if (ezSInput[nUnit] & EZDIO_IN_PCB_CHECK)
	{
		return true;
	}
#endif

	return false;
}
//-----------------------------------------------------------------------------
//
// LIGHT CURTAIN ����
//
//-----------------------------------------------------------------------------
bool CDioControl::EziLightCurtainCheck(int nUnit)
{
#ifndef ON_LINE_DIO
	return true;
#endif
#if  (RIVIAN___MODE__CH == ON_OQA_MODE)
	//
	if (!(ezSInput[nUnit] & EZDIO_IN_LIGHT_CURTAIN))
	{
		return true;
	}
#endif

	return false;
}
