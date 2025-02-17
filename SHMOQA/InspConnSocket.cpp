// InspConnSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "InspConnSocket.h"
#include "AutoInspDlg.h"

// CInspConnSocket

CInspConnSocket::CInspConnSocket()
{
	m_pMainDlg = NULL;
	m_bStxState = false;
}

CInspConnSocket::~CInspConnSocket()
{
}


// CInspConnSocket ��� �Լ�

//-----------------------------------------------------------------------------
//
//	OnClose
//
//-----------------------------------------------------------------------------
void CInspConnSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(m_pMainDlg != NULL)
	{
		m_pMainDlg->CloseInspConn(this, false);
	}

	CSocket::OnClose(nErrorCode);
}

//-----------------------------------------------------------------------------
//
//	OnReceive
//
//-----------------------------------------------------------------------------
void CInspConnSocket::OnReceive(int nErrorCode)
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
//	��Ŷ ���� �Ľ�
//
//-----------------------------------------------------------------------------
void CInspConnSocket::PareRecvBuff(unsigned char* pRecvBuff, int nRecvLen)
{
	CString sPacket = _T("");
	int i;

	for(i = 0; i < nRecvLen; i++)
	{
		// STX �ȹ��� ����
		if(m_bStxState == false)
		{
			if(pRecvBuff[i] == '#')
			{
				m_nPacketBuffPosi = 0;
				memset(m_sPacketBuff, 0x00, sizeof(m_sPacketBuff));

				m_sPacketBuff[m_nPacketBuffPosi] = pRecvBuff[i];

				m_bStxState = true;
			}
		}
		// STX ���� ����
		else
		{
			// ��Ŷ ���� �����÷ο� �˻�
			if(m_nPacketBuffPosi >= (sizeof(m_sPacketBuff) - 1))
			{
				m_bStxState = false;
				continue;
			}

			m_nPacketBuffPosi++;
			m_sPacketBuff[m_nPacketBuffPosi] = pRecvBuff[i];

			// ETX �˻�
			if(m_sPacketBuff[m_nPacketBuffPosi] == '@')
			{
				sPacket = CA2T((char*)m_sPacketBuff);
				g_pCarAABonderDlg->ProcessInspPacket(sPacket);

				m_bStxState = false;
			}
		}
	}
}