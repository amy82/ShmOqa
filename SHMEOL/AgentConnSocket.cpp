// AgentConnSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "AgentConnSocket.h"
#include "AutoInspDlg.h"

// CAgentConnSocket

CAgentConnSocket::CAgentConnSocket()
{
	m_pMainDlg = NULL;
	m_bStxState = false;
}

CAgentConnSocket::~CAgentConnSocket()
{
}


// CAgentConnSocket ��� �Լ�

//-----------------------------------------------------------------------------
//
//	OnClose
//
//-----------------------------------------------------------------------------
void CAgentConnSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(m_pMainDlg != NULL)
	{
		m_pMainDlg->CloseAAMain();
	}

	CSocket::OnClose(nErrorCode);
}

//-----------------------------------------------------------------------------
//
//	OnReceive
//
//-----------------------------------------------------------------------------
void CAgentConnSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	unsigned char sRecvBuff[SIZE_OF_1K];
	int nRecvLen;

	nRecvLen = this->Receive(sRecvBuff, sizeof(sRecvBuff));
	if(nRecvLen > 0)
	{
		this->PareRecvBuff(sRecvBuff, nRecvLen);
	}

	CSocket::OnReceive(nErrorCode);
}

//-----------------------------------------------------------------------------
//
//	��Ŷ �Ľ�
//
//-----------------------------------------------------------------------------
void CAgentConnSocket::PareRecvBuff(unsigned char* pRecvBuff, int nRecvLen)
{
	CString sData = _T("");
	int i;

	for(i = 0; i < nRecvLen; i++)
	{
		// STX �� ���� ����
		if(m_bStxState == false)
		{
			// STX ����?
			if(/*pRecvBuff[i] == '@' || */pRecvBuff[i] == '#')
			{
				// ��Ŷ ���� �ʱ�ȭ
				memset(m_sPacketBuff, 0x00, sizeof(m_sPacketBuff));
				m_nPacketBuffPosi = 0;

				// STX�� ��Ŷ ���ۿ� ��´�.
				m_sPacketBuff[m_nPacketBuffPosi] = pRecvBuff[i];

				// STX ���� �÷��׸� �ø���.
				m_bStxState = true;
			}
		}
		// STX ���� ����
		else
		{
			// ��Ŷ ���� �����÷ο� üũ
			if(m_nPacketBuffPosi > (sizeof(m_sPacketBuff) - 1))
			{
				m_bStxState = false;
				continue;
			}

			m_nPacketBuffPosi++;
			m_sPacketBuff[m_nPacketBuffPosi] = pRecvBuff[i];

			// ETX üũ
			if(m_sPacketBuff[m_nPacketBuffPosi] == '$')
			{
				sData = CA2T((char*)m_sPacketBuff);
				m_pMainDlg->RecvAAMainData(sData);

				m_bStxState = false;
			}
		}
	}
}
