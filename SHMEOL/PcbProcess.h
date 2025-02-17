#pragma once
class CPcbProcess
{
public:
	CPcbProcess(void);
	~CPcbProcess(void);

public:
	void SetUnit(int nUnit){ m_nUnit = nUnit;}

	//EOL
	int HomeProcess(int nStep);					//  ����(10000 ~ 20000)
	int AutoReadyProcess(int nStep);			//  �����غ�(20000 ~ 30000)
	//-------------------------------------------------------------------------------------------------------------------
	int Auto_M_PCBLoading(int nStep);			//  MAIN -> PCB �ε�            (30000 ~ 40000)					  		
	int AutoPcbSensorAlign(int nStep);			//  �׸��� Lens Holder Align	(40000 ~ 50000)
	int AutoPcbLaser(int nStep);				//  Pcb ����ǰ �˻�� Laser		(50000 ~ 60000)
	int Auto_OC_Insp(int nStep);				//	�̹��˻�					(60000 ~ 70000)
    int AutoChartInsp(int nStep);			    //  CHART�˻�					(80000 ~ 90000)
	int AutoEOLFinalSFR(int nStep);				//  Final SFR					(110000 ~ 120000)
	
	//OQA
	int OqaHomeProcess(int nStep);					//  ����					(10000 ~ 20000)
	int OqaAutoReadyProcess(int nStep);				//  �����غ�				(20000 ~ 30000)
	int OqaAuto_M_PCBLoading(int nStep);			//  MAIN -> PCB �ε�            (30000 ~ 40000)			
	int OqaAuto_OC_Insp(int nStep);					//	�̹��˻�				(60000 ~ 70000)
	int OqaAutoEOLFinalSFR(int nStep);				//  Final SFR				(110000 ~ 120000)
	//-------------------------------------------------------------------------------------------------------------------
	//AA
	int AutoChartMoving(int nStep);				//  Pcb , Lens ���� Chart�̵�		(60000 ~ 70000)
	int AutoFocus(int nStep);					//  AA(60000 ~ 70000)
	int AutoUV(int nStep);						//  UV(70000 ~ 80000)		
	int AutoLensLoading(int nStep);				//  Lens Loading					(50000 ~ 60000)
	
	

	//EOL
	int AutoEpoxyLensLaser(int nStep);				//  Pcb Epoxy , Lens Laser����		(60000 ~ 70000)
	int HomeProcess_IMAGE(int nStep);			//  ����(10000 ~ 20000)	ȭ�����
	int Auto_DarkImage_Get(int nStep);			//90000 ~ 99000
	//����ǰ�˻�

	int AutoBrightInsp(int nStep);			//�̹����� ��� �˻� (SATURATION , IR FILTER)




	int AutoPcbAlign(int nStep);				//  Holder Align                (40000 ~ 50000)
	int AutoPcbBcr(int nStep);					//  Holder BCR                (50000 ~ 60000)
    int AutoPcbLaserTilt(int nStep);			//  LaserTilt->Holder Align     (50000 ~ 60000)		//��� XXXXXXXXXXXXX
    //
    int AutoDarkInsp(int nStep);			    //  Dark �˻�     (60000 ~ 70000)
    int AutoOC6500KInsp(int nStep);			    //  Defect , stain,RI,Color Uniformity, R/C,B/C,Cr/Cb �˻�     (70000 ~ 80000)
    //

	

    //
	//ColorSensitivity = �˻縸 5000k , 6500k 
	//
	

	//
	int VaccumRetry;
	bool m_nDiffTestOk;
private:
	bool CheckDecreaseSFR();
	bool GetSfrMaxPos();
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

