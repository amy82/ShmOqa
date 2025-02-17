// BaseThread.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BaseThread.h"


// CBaseThread

IMPLEMENT_DYNCREATE(CBaseThread, CWinThread)

CBaseThread::CBaseThread()
{
	m_bWorking = false;
	m_bPause = false;
	m_hThread = NULL;
	ThreadForceStop = false;		//init
	m_nThreadID = 0;
}

CBaseThread::~CBaseThread()
{
}

BOOL CBaseThread::InitInstance()
{
	// TODO:  ���⿡�� �� �����忡 ���� �ʱ�ȭ�� �����մϴ�.
	return TRUE;
}

int CBaseThread::ExitInstance()
{
	// TODO:  ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CBaseThread, CWinThread)
END_MESSAGE_MAP()


// CBaseThread �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	 ������ ����
//
//-----------------------------------------------------------------------------
bool CBaseThread::StartThread()
{
	// �����尡 �������̸�
	if (m_bWorking == true)
	{
		return false;
	}

	m_bWorking = true;
	ThreadForceStop = false;		//StartThread
	if (::AfxBeginThread(ThreadFunc, (LPVOID)this) == NULL)
	{
		m_bWorking = false;
		return false;
	}
	
	return true;
}
bool CBaseThread::GetThreadRunning()
{
	if (m_hThread != NULL || m_bWorking)
	{
		if (::WaitForSingleObject(m_hThread, 100) == WAIT_TIMEOUT)
		{
			return true;	//thread Run....
		}
		return true;//thread Run....
	}
	return false;
}

bool CBaseThread::GetForceStop()
{
	return ThreadForceStop;
}
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
UINT CBaseThread::ThreadFunc(void* pParam)
{
	MSG stMessage;
	CBaseThread* pThread = (CBaseThread*)pParam;

	pThread->m_hThread = AfxGetThread()->m_hThread;
	pThread->m_nThreadID = AfxGetThread()->m_nThreadID;

	pThread->ThreadConstructor();

	while (pThread->m_bWorking == true)
	{
		if (pThread->ThreadForceStop)
		{
			break;
		}
		if (PeekMessage(&stMessage, NULL, 0, 0, PM_NOREMOVE) != 0)
		{
			if (GetMessage(&stMessage, NULL, 0, 0) == -1)
			{
				break;
			}
			else
			{
				TranslateMessage(&stMessage);
				DispatchMessage(&stMessage);

				// TO DO : �޽��� ó��
			}
		}

		if (pThread->m_bPause == false)
		{
			pThread->ThreadCallBack();
		}
		
		Sleep(MAX_THREAD_SLEEP_INTERVAL);
	}

	pThread->ThreadDestructor();

	pThread->m_hThread = NULL;
	pThread->m_bWorking = false;
	//TRACE(_T("\n ThreadFunc Exit Type: %s\n"), typeid(*pThread).name());
	::AfxEndThread(0, TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------------
//
//	������ ����
//
//-----------------------------------------------------------------------------
void CBaseThread::EndThread()
{
	m_bPause = false;
	ThreadForceStop = true;		//EndThread
								//m_bWorking = false;		//������ while ���������� ����ǰ� �ּ�ó��
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------