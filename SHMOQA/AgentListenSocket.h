#pragma once

// CAgentListenSocket ��� ����Դϴ�.
class CAutoInspDlg;

class CAgentListenSocket : public CSocket
{
public:
	CAgentListenSocket();
	virtual ~CAgentListenSocket();

public:
	void SetMainDlgPtr(CAutoInspDlg* pDlg){ m_pMainDlg = pDlg;}

private:
    CAutoInspDlg* m_pMainDlg;

public:
	virtual void OnAccept(int nErrorCode);
};


