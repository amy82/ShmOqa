// AgentListenSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "AgentListenSocket.h"
#include "AutoInspDlg.h"

// CAgentListenSocket

CAgentListenSocket::CAgentListenSocket()
{
	m_pMainDlg =  NULL;
}

CAgentListenSocket::~CAgentListenSocket()
{
}


// CAgentListenSocket ��� �Լ�

//-----------------------------------------------------------------------------
//
//	OnAccept
//
//-----------------------------------------------------------------------------
void CAgentListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(m_pMainDlg != NULL)
	{
		m_pMainDlg->AcceptInspConn();
	}

	CSocket::OnAccept(nErrorCode);
}
