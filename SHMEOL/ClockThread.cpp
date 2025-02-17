#include "StdAfx.h"
#include "ClockThread.h"
#include "AutoInspDlg.h"
#include "GlobalDeclare.h"

CClockThread::CClockThread(void)
{
}


CClockThread::~CClockThread(void)
{
}

//-----------------------------------------------------------------------------
//
//	������ ������
//
//-----------------------------------------------------------------------------
void CClockThread::ThreadConstructor()
{
	m_dwTickCount = 0;
}

//-----------------------------------------------------------------------------
//
//	������ �Ҹ���
//
//-----------------------------------------------------------------------------
void CClockThread::TheradDestructor()
{

}

//-----------------------------------------------------------------------------
//
//	������ �ݹ�
//
//-----------------------------------------------------------------------------
void CClockThread::ThreadCallBack()
{
	// 300ms ���� ǥ��
	if((GetTickCount() - m_dwTickCount) > 500)
	{
		// ���� ��¥ ǥ��
		this->ShowCurrDate();

		// ���� �ð� ǥ��
		this->ShowCurrTime();

		m_dwTickCount = GetTickCount();
		
	}
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CClockThread::ShowCurrDate()
{
	SYSTEMTIME stSysTime;
	TCHAR szDate[SIZE_OF_100BYTE];

	GetLocalTime(&stSysTime);

	_stprintf_s(szDate, SIZE_OF_100BYTE, _T("%04d-%02d-%02d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
	g_pCarAABonderDlg->ShowCurrentDate(szDate);
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CClockThread::ShowCurrTime()
{
	SYSTEMTIME stSysTime;
	TCHAR szTime[SIZE_OF_100BYTE];

	GetLocalTime(&stSysTime);

	_stprintf_s(szTime, SIZE_OF_100BYTE, _T("%02d:%02d:%02d"), stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	g_pCarAABonderDlg->ShowCurrentTime(szTime);
}
