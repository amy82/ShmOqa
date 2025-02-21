#include "StdAfx.h"
#include "FileThread.h"
#include "AutoInspDlg.h"
#include "GlobalDeclare.h"

CFileThread::CFileThread(void)
{
}


CFileThread::~CFileThread(void)
{
}

//-----------------------------------------------------------------------------
//
//	������ ������
//
//-----------------------------------------------------------------------------
void CFileThread::ThreadConstructor()
{
	m_dwTickCount = 0;
}

//-----------------------------------------------------------------------------
//
//	������ �Ҹ���
//
//-----------------------------------------------------------------------------
void CFileThread::TheradDestructor()
{

}

//-----------------------------------------------------------------------------
//
//	������ �ݹ�
//
//-----------------------------------------------------------------------------
void CFileThread::ThreadCallBack()
{
	// 300ms ���� ǥ��
	if ((GetTickCount() - m_dwTickCount) > 43200000)	//1�ð� = 3600 , 12�ð� 43200
	{
		m_dwTickCount = GetTickCount();

		RawDataDelete(0);
	}

}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CFileThread::ShowCurrDate()
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
void CFileThread::ShowCurrTime()
{
	SYSTEMTIME stSysTime;
	TCHAR szTime[SIZE_OF_100BYTE];

	GetLocalTime(&stSysTime);

	_stprintf_s(szTime, SIZE_OF_100BYTE, _T("%02d:%02d:%02d"), stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	g_pCarAABonderDlg->ShowCurrentTime(szTime);
}
