#pragma once
class CPcbProcess
{
public:
	CPcbProcess(void);
	~CPcbProcess(void);

public:
	void SetUnit(int nUnit){ m_nUnit = nUnit;}

	//-------------------------------------------------------------------------------------------------------------------
	//OQA
	int OqaHomeProcess(int nStep);					//  원점					(10000 ~ 20000)
	int OqaAutoReadyProcess(int nStep);				//  운전준비				(20000 ~ 30000)
	int OqaAuto_M_PCBLoading(int nStep);			//  MAIN -> PCB 로딩            (30000 ~ 40000)			
	int OqaAuto_OC_Insp(int nStep);					//	이물검사				(60000 ~ 70000)
	int OqaAutoEOLFinalSFR(int nStep);				//  Final SFR				(110000 ~ 120000)
	//-------------------------------------------------------------------------------------------------------------------

	bool nRunOnlineControlState;		//true = Online , false = Offline 
	int nRunTimeOutSec;			//Default = 60;
	int nLotProcessingComplete_ACK;


	int VaccumRetry;
	bool m_nDiffTestOk;
private:
	int mOcLightChannel;
	int mLightRetry;
	bool m_bHomeProc;
	bool m_bHomeError;	
	int m_nUnit;
	bool m_bFlagSocketReq;
	int	m_nSocketCnt;
	DWORD m_dwDelayCount;
	int m_nBCR_Recount;
	double m_dBCR_Error_Offset;
	CString m_strCalTotalResult;
	CString m_strCalResult;
	CString sSendMsg;

	int nDiffRetry;

	int rawSumCount;
	int TestPatternRetry;
	int VarRetry;
};

