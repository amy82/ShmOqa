#pragma once

#include "../StdAfx.h"
//! AjinExTek Motion Controller Ŭ����
//! Created, 2014/12/10
//! Recently Modified,

//! 1] ���������� Motion Controller�� ��ġ ��� �����Ѵ�. ������, AxmMoveVel ó�� ������ �ӵ��� �����̴� ����� �����Ѵ�. 
//! 2] RTEX ��ǰ (PCI-R1604)������ Interrupt, Software Limit�� �������� �ʴ´�. 
//! 3] ���� �������� �� �� �ִ� ����� ����. 
//! 4] PCI-R1604 A4N, A5N�� ��� ������� �Է´��� �������� �ʴ´�.
//!     (��� ��⿡ �ִ� ������� �Է´� ���� ��ȣ�� �ְ� �Ǹ� ��ȣ�� ������ ESTOP�� �ڵ����� ���� ��� ��ǿ� ���ؼ� ����)

//! [���� ����] AA Bonder Vision�� Homing���� �ⱸ ����
//!		1) ��� ������ ���� ��Ų �Ŀ� Homing�� �����ؾ� �Ѵ�. 
//!		2) Lens Z�� Motor�� �������� �ö� �Ŀ� �ٸ� Motor���� Homing�ؾ� �Ѵ�.
//!		3) PCB Y�� Motor�� (-) Limit ���� ������ �� ���Ŀ�, PCB X, Theta �� Motor�� Homing�ؾ� �Ѵ�. 
//!		4) PCB Theta�� Motor�� Homing�ϱ� ����, PCB Cover ����� �Ǹ����� ��½��Ѿ� �Ѵ�. 

//! ������ MMC Motion Controller�� �Լ����� �ִ��� ���� �Ͽ�, �ҽ� �ڵ��� �������� ���δ�. 


//! �Ʒ� ����ü�� Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
struct strMove_Input_Signal_Search
{
	double dVel;
	double dAccel;

	//! ���� ��ȣ ���� (�Ʒ� 4���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEndLimit_Positive;
	bool bEndLimit_Negative;
	bool bHomeSensor;
	bool bPhase_Encoder_Z;		//! Encoder�� Z�� ��ȣ

	//! Edge ���� (�Ʒ� 2���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEdge_Down;
	bool bEdge_Up;

	//! ���� ��� (�Ʒ� 2���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEmergency;
	bool bSlowDown;

	strMove_Input_Signal_Search()
	{
		InitInfo();
	}

	void InitInfo()
	{
		dVel = 0.;
		dAccel = 0.;

		bEndLimit_Positive = false;
		bEndLimit_Negative = false;
		bHomeSensor = false;
		bPhase_Encoder_Z = false;

		bEdge_Down = false;
		bEdge_Up = false;

		bEmergency = false;
		bSlowDown = false;
	}

	strMove_Input_Signal_Search& operator=(const strMove_Input_Signal_Search &input)	//! '=' ������ overloading
	{
		dVel   = input.dVel;
		dAccel = input.dAccel;

		bEndLimit_Positive = input.bEndLimit_Positive;
		bEndLimit_Negative = input.bEndLimit_Negative;
		bHomeSensor        = input.bHomeSensor;
		bPhase_Encoder_Z   = input.bPhase_Encoder_Z;

		bEdge_Down = input.bEdge_Down;
		bEdge_Up   = input.bEdge_Up;

		bEmergency = input.bEmergency;
		bSlowDown  = input.bSlowDown;

		return *this;
	}
};//! struct strMove_Input_Signal_Search





class AxlMotorSet
{
public:
	AxlMotorSet(void);
	~AxlMotorSet(void);

private:
	//! �ش� ���� ����� ���Ͽ� Motion Board�� �������� ���Ϸ� ���� ����
	CString m_sMotionSettingFilePath;

public:
	CString sMsg;
	bool bMotorMoving[UNIT_CH][CUR_MOTOR_NO];							
	int  m_bOrgFlag[UNIT_CH][CUR_MOTOR_NO];				
	bool m_bAxisState[UNIT_CH][CUR_MOTOR_NO];

	bool bJogPlusFlag[UNIT_CH][CUR_MOTOR_NO];	//���׻���
	bool bJogMinusFlag[UNIT_CH][CUR_MOTOR_NO];	//���׻���
	bool bOldJogPlusFlag[UNIT_CH][MAX_MOTOR_NO];		
	bool bOldJogMinusFlag[UNIT_CH][MAX_MOTOR_NO];


	CCriticalSection m_criHomeProc;

	bool Axl_Init();
	void Axl_Close();
	int  Axl_Axisconfig(int iUnit);


	bool AmpEnable(int iUnit, int iAxis);				
	bool AmpEnableAll(int iUnit);						
	bool AmpDisable(int iUnit, int iAxis);				
	bool AmpDisableAll(int iUnit);
	bool GetAmpEnable(int iUnit, int iAxis);			
	bool GetAmpEnableAll(int iUnit);					

	bool AmpFaultReset(int iUnit, int iAxis);			
	bool AmpFaultResetAll(int iUnit);
	int	 AmpFaultCheck(int iUnit);						
	bool GetAmpFault(int iUnit, int iAxis);				

	bool GetInposition(int iUnit, int iAxis);			
	bool GetHomeSensor(int iUnit, int iAxis);			
	bool GetNegaSensor(int iUnit, int iAxis);			
	bool GetPosiSensor(int iUnit, int iAxis);			

	bool ActCRC(int iUnit, int iAxis);

	bool GetStopAxis(int iUnit, int iAxis);			
	bool StopAxis(int iUnit, int iAxis);				
	bool StopAxisAll(int iUnit);

	double GetEncoderPos(int iUnit, int iAxis);			
	double GetEncoderPos_Disp(int iUnit, int iAxis);	
	double GetCommandPos(int iUnit, int iAxis);			
	double GetCommandPos_Disp(int iUnit, int iAxis);	

	//! 20160503
	int GetEncoderPulse(int iUnit, int iAxis);   
	int GetCommandPulse(int iUnit, int iAxis);  

	double CalcAccFromAccTime(double dVel, double dTime);						
	double CalcAcc_G(int iUnit, int iAxis);							
	double CalcDec_G(int iUnit, int iAxis);

	BOOL JogMove(int iUnit, int iAxis, double dVel);								
	BOOL JogStop(int iUnit, int iAxis);												

	bool MoveAxis_To_Sensor(int axis, strMove_Input_Signal_Search *pstrInput);		

	bool Move_From_GUI(int iUnit, int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus = true);	//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)


	bool MoveAxis(int iUnit, int iAxis, bool AbSFlag, double dPos, double dVel);		
	bool MoveAxisMulti(int iUnit, int iAxisCnt, short iAxis[CUR_MOTOR_NO], double pos[CUR_MOTOR_NO]);	


	bool GetResult_Home_Search(int iUnit, int iAxis, CString &sResult);		
	int IsCompleted_Home_Search(int iUnit, int iAxis);						
	int  GetRate_Home_Search(int iUnit, int iAxis);							

	void Stop_Home_Search(int iUnit, int iAxis);							

	int  Start_Home_Search(int iUnit, int iAxis);												
	DWORD SetHomeSearchSpeed(int iUseAxis);									


	bool HomeSingleProcess(int iUnit, int iAxis);		//���� ����
	bool HomeReadyProcess(int iUnit, int iType);		//��ü ���� 0: ��������, 1: �����غ�				
	
	bool Move_From_AbsRel(int iUnit, int iAxis, double dVal, bool bPlus);

	int	iHomeUnit;
	int	iHomeUseStep;							
	int	iHomeEndStep;							
	int	iHomeProcUsing[UNIT_CH][CUR_MOTOR_NO];	
	int iRunStep[2];
	int iRunStepOrg[2];
	//kdu �Ŵ��� aa �߰�
	int iManualStep[2];
	int iSelectAxisss[2];		//������������ ���õ� �� ����Ǵ� ����
	bool bReadyPusher[2];		//���� �غ�� Ǫ�� ��ġ ���� ����
	int iSocketCntChk[2];
	double  dTime;
	double  dSTime;	
	double dFTime, dFTime2;
	bool   bITF_CHK[2];			///20160629  JJC

	HANDLE	m_hThreadHomeAll[UNIT_CH];		
	HANDLE	m_hThreadHome[UNIT_CH];			
	HANDLE	m_hThreadReady[UNIT_CH];			
	HANDLE m_hThreadAutoFocus[UNIT_CH];
	//�Ŵ��� ���� kdu 151123
	HANDLE m_hThreadManualFocus[UNIT_CH];

	//������ �������
	bool bThreadHomeAll[UNIT_CH];
	bool bThreadHome[UNIT_CH];
	bool bThreadReady[UNIT_CH];
	bool m_bThreadAutoFocus[UNIT_CH];
	//kdu �Ŵ��� ���� 151123
	bool m_bThreadManualFocus[UNIT_CH];


	void func_AutoProcStop(int iUnit);			//! �ڵ���� ����

	bool AutoMode_Process(int iUnit);	
	bool ManualMode_Process(int iUnit);

	int HomeAllProcess_UNIT1(int iUseStep);		//��ü����
	int HomeAllProcess_UNIT2(int iUseStep);

	int ReadyAllProcess_UNIT1(int iUseStep);	//���� �غ�
	int ReadyAllProcess_UNIT2(int iUseStep);

	int HomeSingleProcess_UNIT1(int iUseStep);	//���� ����
	int HomeSingleProcess_UNIT2(int iUseStep);

};//! class AxlMotorSet

DWORD ThreadHomeProcessAA1(AxlMotorSet* pAxlMotorSet);	
DWORD ThreadHomeProcessAA2(AxlMotorSet* pAxlMotorSet);	

DWORD Thread_HomeReadyProcess1(AxlMotorSet* pAxlMotorSet);	
DWORD Thread_HomeReadyProcess2(AxlMotorSet* pAxlMotorSet);		

DWORD Thread_ReadyProcess1(AxlMotorSet* pAxlMotorSet);	
DWORD Thread_ReadyProcess2(AxlMotorSet* pAxlMotorSet);		