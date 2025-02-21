#pragma once

//! ��⿡�� ������ ���� ��Ʈ�ѷ��� ���� ����� Ŭ����

//! RS-232C ��� Ŭ������ �����´�. (�θ� Ŭ����)
//! �θ� Ŭ������ �Լ�, public ��� ������ �ٷ� ��� ����

#include "../../RS232C/Handler_RS232C.h"

class CLightControl : public CHandler_RS232C
{
public:
	CLightControl(void);
	~CLightControl(void);

private:
	int m_iNo_Machine;	//! ���� ��Ʈ�ѷ� ��ġ�� ���� ��ȣ, 0 based
	
	TCHAR m_acRecvStorage[16384];
	int m_iRecvSize;
	void OutputRecvData(void *pProcPtr, TCHAR *pcBuff, int iDataSize, int iCnt_Recv);

	int SendData_Light_Controller(BYTE *pbySend, int iSendSize);

	//! Channel On/Off
	//! [���� ����] 'iNoChannel' ���� ���� ��Ʈ�ѷ��� ���� ä�� �Է°��̴�. 
	int SetChannel_OnOff(int iNoChannel, bool bSwitch_On);

	//! �� Channel�� ��Ⱚ(PWM��) ����
	//! [���� ����] 'iNoChannel' ���� ���� ��Ʈ�ѷ��� ���� ä�� �Է°��̴�. 
	int SetChannel_Value(int iNoChannel, int iValue);

protected:
	//! �θ� Ŭ������ ���� �Լ����� ��ü���� ����� �����Ѵ�. 

	//! RS-232C ����� ����� �Ŀ� �� ó�� 1�� �� �����ؾ� �� ����� �����ϴ�. 
	virtual void ReadyDevice();
	//! ���� ó�� ��ü �Լ�
	virtual void ProcessReceiveData(void *pProcPtr, BYTE *pbyBuff, int iSize, bool bFlag_Active, int iCnt_Recv);
	//! ���� ó�� �� ��ó�� �Լ�
	virtual void PostProcRecvData();

public:
	//! ���� ��Ʈ�ѷ��� ����
	//! [���� ����] ���� �����Ͱ� ���� ��쿡��, �ݵ��, SetReceiveProcPtr �Լ��� ���� ó���� �� Dialog�� GUI Ŭ������ �����ϰ�, �� �Լ��� ȣ���Ѵ�. 
	bool Connect_Device(CString sPort, int iNoMachine = 0, int Baud = 9600);
	bool Connect_BackDu_Device(CString sPort, int iNoMachine = 0);
	//! ���� ����
	void Close_Device();

	//! ���ڿ� ���� ����
	int SendData_Light_Controller(CString sSend);	

	//! �� ��ġ������ ���� ���� ����
	//! 0 : Lens, 1 : PCB, 2 : Resin, 3 : ����, 4 : MTF, 5 : �̹��˻�
	//! ���� ���� ���� ����, Demo ���������� Model �������� ȣȯ�� ���� �߰�
	//! [���� ����] 'iChannel'���� ���� ���� ��Ʈ�ѷ��� ä���� �ƴϴ�. 
	void ctrlLedVolume(int iChannel, int iValue);
	void dpctrlLedVolume(int iChannel, int iValue);

	//�̹����� ��Ʈ DPS-50V5-4 FLL R80 H4C = 
	
	bool DPS_Check_Time();//�˻��ʱ� ��ȸ ���ð� Ȯ���ؾߵȴ� (20,000 �ð� �˶����)
	bool DPS_Get_State(int mode, int channel = 1);		//���� Ȯ�� �Լ� 
	//1(3F) = Ready ���� üũ 
	//2 = Level
	//3 = On/Off 
	//4 = �ð� ���� �б� (���� �ð�)
	//5 = �ð� ���� �б� (���� �ð�)
	bool DPS_Light_OnOffLevel(int channel, int onoff, int data);
	int DPS_SetChannel_Value(int iNoChannel, int iValue);
	//
	//
	bool SendLightONOFF(bool Use);

	bool SendDomeChartLightONOFF(bool Use);
	bool SendDomeChartLightValue(int mCh, int data);

	bool SendLightChange(int Use);
	bool SendLightValue(int data);
	bool SendLightLxStep(int data);
	bool LxSetDataSave();
public:
};

