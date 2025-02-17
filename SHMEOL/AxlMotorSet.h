#pragma once


//! 1] ���������� Motion Controller�� ��ġ ��� �����Ѵ�. ������, AxmMoveVel ó�� ������ �ӵ��� �����̴� ����� �����Ѵ�. 
//! 2] RTEX ��ǰ (PCI-R1604)������ Interrupt, Software Limit�� �������� �ʴ´�. 
//! 3] ���� �������� �� �� �ִ� ����� ����. 
//! 4] PCI-R1604 A4N, A5N�� ��� ������� �Է´��� �������� �ʴ´�.
//!     (��� ��⿡ �ִ� ������� �Է´� ���� ��ȣ�� �ְ� �Ǹ� ��ȣ�� ������ ESTOP�� �ڵ����� ���� ��� ��ǿ� ���ؼ� ����)

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
		dVel = input.dVel;
		dAccel = input.dAccel;

		bEndLimit_Positive = input.bEndLimit_Positive;
		bEndLimit_Negative = input.bEndLimit_Negative;
		bHomeSensor = input.bHomeSensor;
		bPhase_Encoder_Z = input.bPhase_Encoder_Z;

		bEdge_Down = input.bEdge_Down;
		bEdge_Up = input.bEdge_Up;

		bEmergency = input.bEmergency;
		bSlowDown = input.bSlowDown;

		return *this;
	}
};//! struct strMove_Input_Signal_Search



class CAxlMotorSet
{
public:
	CAxlMotorSet();
	~CAxlMotorSet();


public:
	DWORD m_dwMotorTickCount;

	bool Axl_Init();
	void Axl_Close();
	int  Axl_Axisconfig(int nUnit);

	bool ReadBit(int nUnit, int nAxis, int nBit);

	bool AmpEnable(int nUnit, int nAxis);
	bool AmpEnableAll(int nUnit);
	bool AmpDisable(int nUnit, int nAxis);
	bool AmpDisableAll(int nUnit);
	bool GetAmpEnable(int nUnit, int nAxis);
	bool GetAmpEnableAll(int nUnit);

	bool GetOrgFlagAll(int nUnit);

	bool AmpFaultReset(int nUnit, int nAxis);
	bool AmpFaultResetAll(int nUnit);
	int	 AmpFaultCheck(int nUnit);
	bool GetAmpFault(int nUnit, int nAxis);
	bool GetServoOn(int nUnit, int nAxis);
	
	bool GetInposition(int nUnit, int nAxis);

	bool GetPcbYMotorPosCheck(int nUnit, int dPos);				//PCB Y �� ��ġ Ȯ��
	bool GetPcbXYMotorPosCheck(int nUnit, int dPos);			//PCB X,Y �� ��ġ Ȯ��
	bool GetPcbZMotorPosCheck(int nUnit, int dPos);				//PCB Z �� ��ġ Ȯ��
	bool GetLensXYMotorPosCheck(int nUnit, int dPos);			//LENS X,Y �� ��ġ Ȯ��
	bool GetLensZMotorPosCheck(int nUnit, int dPos);			//LENS Z �� ��ġ Ȯ��
	bool GetHomeSensor(int nUnit, int nAxis);
	bool GetNegaSensor(int nUnit, int nAxis);
	bool GetPosiSensor(int nUnit, int nAxis);
	void InitialPos(int nUnit, int nAxis);

	bool ActCRC(int nUnit, int nAxis);

	bool GetStopAxis(int nUnit, int nAxis);
	bool StopAxis(int nUnit, int nAxis);
	bool StopAxisAll(int nUnit);

	double GetEncoderPos(int nUnit, int nAxis);
	double GetEncoderPos_Disp(int nUnit, int nAxis);

	double GetCommandPos(int nUnit, int nAxis);
	double GetCommandPos_Disp(int nUnit, int nAxis);
	double GetDDMotorPos(int nUnit, int nAxis);
	double GetDDMotorPos_Disp(int nUnit, int nAxis);
	double GetActPos(int nUnit, int nAxis);
	double GetEncoderPulse(int nUnit, int nAxis);
	double GetCommandPulse(int nUnit, int nAxis);

	void SetEncoderPos(int nUnit, int nAxis, double dPos);
	void SetCommandPos(int nUnit, int nAxis, double dPos);
	void SetDDMotorCalcPos(int nUnit, int nAxis, double dMaxDeg);
	void SetDDMotorPos(int nUnit, int nAxis, double dPos);
	double CalcAccFromAccTime(double dVel, double dTime);

	//bool SetJogSpeed(int iID_GUI, CWnd* pParentWnd, double *pOutSpeedGain);
	bool JogMove(int nUnit, int nAxis, double dVel);
	bool JogStop(int nUnit, int nAxis);

	//bool MoveAxis_To_Sensor(int nAxis, strMove_Input_Signal_Search *pstrInput);

	//bool Move_From_GUI(int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus = true);	//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
	bool MoveFromAbsRel(int nUnit, int nAxis, double dPos, bool bPlus);

	bool MoveAxis(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, bool bWait = false);
	bool MoveAxis(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, double dAcc);
	//
	//����,� ���� ����
	bool MoveStraightPcbXYAxis(int nUnit);
	//� ����
	bool MoveCirclePcbXYAxis(int nUnit, int dPos);// , double dAngle, double dVel, double dAcc);
	//
	//bool MoveAxisComplete(int nAxis, int nAbsFlag, double dPos, double dVel, bool bWait = true);
	bool MoveAxisComplete(int nUnit, int nAxis, int nAbsFlag, double dPos, double dVel, int nSpeed, bool bWait = true, double dParamAcc = 0.0, double dParamDec = 0.0); //20161101
	bool MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT]);
	bool MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], int nSpeed);
	bool MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], double dSpeed);
	//bool MoveAxisMulti(int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], double dSpeed, bool bWait = true);
	bool MoveAxisMulti(int nUnit, int nAxisCnt, int nAxis[MAX_MOTOR_COUNT], double dPos[MAX_MOTOR_COUNT], double dVel[MAX_MOTOR_COUNT], double dAcc[MAX_MOTOR_COUNT], double dDec[MAX_MOTOR_COUNT]);

	bool MoveAxisLimit(int nUnit, int nAxis, double dVel, double dAcc, int nLimit, int nSignal, int nStopMode);
	bool GetResultHomeSearch(int nUnit, int nAxis, CString &szResult);
	int IsCompletedHomeSearch(int nUnit, int nAxis);
	int GetHomeSearchRate(int nUnit, int nAxis);

	DWORD SetHomeSearchSpeed(int nAxis);
	bool StartHomeSearch(int nUnit, int nAxis);
	void StopHomeSearch(int nUnit, int nAxis);

	double GetAccel_Cal(int nUnit, int nAxis, double dSec);

	//bool HomeProcess(int nAxis, int nHomeDir);
	//bool HomeProcessAll();

	bool GetHomeState(int nUnit);
	bool GetHomeState(int nUnit, int nAxis);
	bool GetAmpFaultState(int nUnit);


	// AA PCB �� �̵�
	bool MovePcbTMotor(int nUnit, double dOffsetTh /*= 0.0*/, bool bWait /*= false*/);
	bool MovePcbYMotor(int nUnit, int nPosi, double dOffsetY = 0.0, bool bWait = false);
	bool MovePcbYTMotor(int nUnit, int nPosi, double dOffsetY = 0.0, double dOffsetTh = 0.0, bool bWait = false, bool bManual = false);
	bool MovePcbYTMotor(int nUnit, double dOffsetY = 0.0, double dOffsetTh = 0.0, bool bWait = false);

	bool MovePcbXYMotor(int nUnit, int nPosi, double dOffsetX = 0.0, double dOffsetY = 0.0, bool bWait = false, bool bManual = false);

	bool MovePcbXYTMotor(int nUnit, int nPosi, double dOffsetX = 0.0, double dOffsetY = 0.0, double dOffsetTh = 0.0, bool bWait = false , bool bManual = false);
	bool MovePcbXYTMotor(int nUnit, double dOffsetX = 0.0, double dOffsetY = 0.0, double dOffsetTh = 0.0, bool bWait = false);
	bool MovePcbXTMotor(int nUnit, int nPosi, double dOffsetX = 0.0, double dOffsetTh = 0.0, bool bWait = false);
	bool MovePcbXTMotor(int nUnit, double dOffsetX = 0.0, double dOffsetTh = 0.0, bool bWait = false);


	bool MovePcbZMotor(int nUnit, int nPosi, double dOffsetZ = 0.0, bool bWait = false, bool bManual = false);
	bool MovePcbZMotor(int nUnit, double dPos, bool bWait = false);
	bool MovePcbZMotorComplete(int nUnit, double dOffsetZ = 0.0, double dVel = 0.1, double dAcc = 0.1);
	bool MovePcbTxTyMotor(int nUnit, int nPosi, double dOffsetTx = 0.0, double dOffsetTy = 0.0, bool bWait = false);
	bool MovePcbTxTyMotor(int nUnit, double dOffsetTx = 0.0, double dOffsetTy = 0.0, bool bWait = false);

	// AA LENS �� �̵�
	bool MoveLensXYMotor(int nUnit, int nPosi, double dOffsetX = 0.0, double dOffsetY = 0.0, bool bWait = false);
	bool MoveLensXYMotor(int nUnit, double dOffsetX = 0.0, double dOffsetY = 0.0, bool bWait = false);

	bool MoveLensXMotor(int nUnit, int nPosi, double dOffsetX = 0.0, bool bWait = false);
	bool MoveLensYMotor(int nUnit, int nPosi, double dOffsetY = 0.0, bool bWait = false);
	bool MoveLensZMotor(int nUnit, int nPosi, double dOffsetZ = 0.0, bool bWait = false);
	bool MoveLensZMotor(int nUnit, double dOffsetZ = 0.0, double dVel = 0.1, bool bWait = false);
	bool MoveLensZMotorComplete(int nUnit, double dOffsetZ = 0.0, double dVel = 0.1, double dAcc = 0.1);
	bool MoveLensTxTyMotor(int nUnit, int nPosi, double dOffsetTx = 0.0, double dOffsetTy = 0.0, bool bWait = false);
	bool MoveLensTxTyMotor(int nUnit, double dOffsetTx = 0.0, double dOffsetTy = 0.0, bool bWait = false);

	bool MoveLensZAAMotor(int nUnit, double dPos, double dOffsetZ = 0.0, bool bWait = false);

	bool MoveLensZMotorCompleteAA(int nUnit, double dOffsetZ = 0.0, double dVel = 0.1, double dAcc = 0.1);


	//EPOXY 
	bool Epoxy_Rect_Circle_Draw(int nUnit);		//�簢 , �� ���� ����
	bool Epoxy_Circle_Draw(int nUnit);			//������
	bool Epoxy_Rect_Draw(int nUnit);				//�簢����
	bool MovePcbDispenseStraightLineMotor(int nUnit, int nAxis, int bAbsFlag, double dPos, double dVel, double dAcc, bool bWait = false);
	//func_Epoxy_Rect_Circle_Draw();



private:
	//! �ش� ���� ����� ���Ͽ� Motion Board�� �������� ���Ϸ� ���� ����
	CString m_sMotionSettingFilePath;

public:
	bool m_bOrgFlag[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];
	bool m_bMotorMoving[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];
	bool m_bJogPlusFlag[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];
	bool m_bJogMinusFlag[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];
	bool m_bOldJogPlusFlag[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];
	bool m_bOldJogMinusFlag[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];

	int	m_nHomeProcUsing[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];

	bool m_bAxlInitResult;

	bool m_bOqaOrgFlag;
};

