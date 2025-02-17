#pragma once

// CMesConnSocket ��� ����Դϴ�.

class CAutoInspDlg;

class CMesConnSocket : public CSocket
{
public:
	CMesConnSocket();
	virtual ~CMesConnSocket();

public:
	void SetMainDlgPtr(CAutoInspDlg* pDlg){ m_pMainDlg = pDlg;}
	int IDENTITY;
private:
	void ParseRecvBuff(unsigned char* pRecvBuff, int nRecvLen);
	void ParseRecvBcrBuff(unsigned char* pRecvBuff, int nRecvLen);
	
	void ProcessPacket(CString sData);

private:
    CAutoInspDlg* m_pMainDlg;

	unsigned char m_sPacketBuff[SIZE_OF_1K];
	int m_nPacketBuffPosi;
	bool m_bStxState;
	
public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


