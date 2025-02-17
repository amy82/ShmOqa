#pragma once

#include <afxmt.h>

#define MAX_DIO_COUNT		2
#define MAX_DIO_BYTE_COUNT		1
#define	MAX_WAIT_TIME		3000

class CDioControl
{
public:
	CDioControl(void);
	~CDioControl(void);

public:
	bool InitDio();
	void CloseDio();

	bool ReadDIn(int nModuleNo);

	int ReadBitIn(int nModuleNo, int noffset);

	bool eZiReadIn(int nModuleNo);
	int eZiReadOut(int nModuleNo, int noffset);
	bool eZiWriteOut(int nModuleNo, DWORD dwOut, DWORD dwCurOut);


	bool ReadDOut(int nModuleNo);
	bool WriteDOut(int nModuleNo, DWORD dwOut);
	bool ByteWriteDOut(int nModuleNo, int nOffset, DWORD dwOut);
	bool ReadByteOut(int nModuleNo, int nOffset);





public:
	DWORD GetDIn(int nIndex){ return m_dwDIn[nIndex];}
	DWORD GetDOut(int nIndex){ return m_dwDOut[nIndex];}

	DWORD GetEziIn(int nIndex) { return ezSInput[nIndex]; }
	DWORD GetEziOut(int nIndex);

	DWORD GetByteOut(int module, int nIndex) { return m_dByteDOutArr[module][nIndex]; }

public:
	int GetDoorState();
	bool SetTowerLamp(int nType, bool bOn);
	bool SetBuzzer(int nType);
	bool DoorUnLock(int index, bool flag);
	bool GetDoorUnLockState();
public:
	
	bool PcbSensorCheck(int nUnit, bool bWait = false);							// PCB ���Ͽ� ��ǰ�ִ��� ����

	bool LensGrip(int nUnit, bool bFlag, bool bWait = false);					// LENS GRIP / UNGRIP
	bool LensGripCheck(int nUnit, bool bFlag, bool bWait = false);				// LENS GRIP / UNGRIP  ����

	bool PcbSocketUp(int nUnit, bool bFlag, bool bWait = false);				// PCB SOCKET UP / DOWN
	bool PcbSocketUpCheck(int nUnit, bool bFlag, bool bWait = false);			// PCB SOCKET UP / DOWN ����

	//bool PcbSocketFor(int nUnit, bool bFlag, bool bWait = false);				// PCB SOCKET FOR / BACK
	//bool PcbSocketForCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB SOCKET FOR / BACK ����

	bool LensBufferVacuumOn(int nUnit, bool bFlag, bool bWait = false);			// LENS BUFFER ���� / Ż��
	bool LensBufferVacuumOnCheck(int nUnit, bool bFlag, bool bWait = false);	// LENS BUFFER ���� / Ż�� ����
	bool LensBufferSensorCheck(int nUnit, bool bFlag, bool bWait = false);		// LENS BUFFER ���� ����

	//
	bool ContactUp(int nUnit, bool bFlag, bool bWait = false);					// ���� UP / DOWN
	bool ContactUpcheck(int nUnit, bool bFlag, bool bWait = false);				// ���� UP / DOWN ����

public:
	//------------ezio
	bool EziPcbSocketFor(int nUnit, bool bFlag, bool bWait = false);			// PCB SOCKET FOR / BACK
	bool EziPcbSocketForCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB SOCKET FOR / BACK ����

	bool EziOcUp(int nUnit, bool bFlag, bool bWait = false);					// �̹����� FOR / BACK
	bool EziOcUpCheck(int nUnit, bool bFlag, bool bWait = false);				// �̹����� FOR / BACK ����

	bool EziVacuumOn(int nUnit, bool bFlag, bool bWait = false);				// ����/Ż��
	bool EziVacuumOnCheck(int nUnit, bool bFlag, bool bWait = false);			// ����/Ż�� ����

	bool EziStartBtnLedOn(int nUnit, bool bFlag, bool bWait = false);			// start button led
	bool EziStartBtnOnCheck(int nUnit,  bool bWait = false);

	bool EziVacuumBtnLedOn(int nUnit, bool bFlag, bool bWait = false);			// Vacuum button led
	bool EziBuzzerOn(int nUnit, bool bFlag, bool bWait = false);				// Buzzer

	bool EziPcbCheck(int nUnit);												// ��ǰ ����
	bool EziVacuumBtnCheck(int nUnit);
	bool EziLightCurtainCheck(int nUnit);										// Light Curtain ����

	bool EziSetEziTowerLamp(int nUnit, int nType, bool bOn);					//TowerLamp


public:

	bool SetBuzzer(bool bOn, int nType = 3);

	bool EpoxyDischargeOn(int nUnit, bool bFlag, bool bWait = false);	//EPOXY ����

	bool PcbVacuum(int nUnit, bool bFlag, bool bWait = false);			// PCB ����/Ż��
	bool PcbVacuumCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB ����/Ż�� ����

	bool PcbStopButtonCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB ���� ��ư ����
	
	bool PcbGrip(int nUnit, bool bFlag, bool bWait = false);			// PCB Grip ����/����
	bool PcbGripCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB Grip ����/���� ����

	bool LensSocketCheck(int nUnit, bool bWait = false);	// LENS ��ǰ Ȯ��

	bool Stop_Button_LedOn(int nUnit, bool bFlag, bool bWait = false);
	bool Start_Button_LedOn(int nUnit, bool bFlag, bool bWait);

	bool CurtainUpDown(int nUnit, bool bFlag, bool bWait = false);
	bool CurtainUpDownCheck(int nUnit, bool bFlag, bool bWait = false);


	bool DarkPusher(int nUnit, bool bFlag, bool bWait = false);			// Dark Pusher ���/�ϰ�
	bool DarkPusherCheck(int nUnit, bool bFlag, bool bWait = false);	// Dark Pusher ���/�ϰ� ����

	bool UVShutter(int nUnit, bool bFlag, bool bWait = false);			// UV Shutter Open/Close

	bool LGDoorSignalOn(bool bOn);

	bool UVOn(int nUnit, bool bFlag, bool bWait = false);				// UV ���� On/Off
	bool StartBbCheck(int nUnit, bool bWait = false);		// ��ŸƮ ��ư Press üũ
	bool DoorBbCheck(int nUnit, bool bWait);
	bool CurtainDoorCheck(int nUnit, bool bWait = false);
	
	//
	bool UVOnCheck(int nUnit, bool bFlag, bool bWait = false);			// UV ���� On/Off ����


	bool UVShutterCheck(int nUnit, bool bFlag, bool bWait = false);		// UV Shutter Open/Close ����

	bool UVErrorCheck(int nUnit, bool bFlag, bool bWait = false);		// UV Error ����
	bool UVReadyCheck(int nUnit, bool bFlag, bool bWait = false);		// UV Ready ����
	bool UVApertureCheck(int nUnit, bool bFlag, bool bWait = false);	// UV ������ MAX/MIN ����
	bool UVUseTimeCheck(int nUnit, bool bFlag, bool bWait = false);		// UV Use Time Check
	
	bool LensStageGrip(int nUnit, bool bFlag, bool bWait = false);		// LENS Stage Grip ����/����
	bool LensStageGripCheck(int nUnit, bool bFlag, bool bWait = false);	// LENS Stage Grip ����/���� ����
	bool FeedMove(int nUnit, bool bFlag, bool bWait = false);			// �˻� FEED ����/����
	bool FeedMoveCheck(int nUnit, bool bFlag, bool bWait = false);		// �˻� FEED ����/���� ����
	bool PcbGripUp(int nUnit, bool bFlag, bool bWait = false);			// PCB Grip ���/�ϰ�
	bool PcbGripUpCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB Grip ���/�ϰ� ����
	bool PcbSocketVacuumCheck(int nUnit, bool bFlag, bool bWait = false);		// PCB ��ǰ Ȯ��

public:
	DWORD m_dwDIn[MAX_DIO_COUNT];
	DWORD m_dwDOut[MAX_DIO_COUNT];

	DWORD ezSInput[2];
	DWORD ezSOutput[2];

	CCriticalSection m_clByteCriticalSection[MAX_DIO_BYTE_COUNT][4];
	DWORD m_dByteDOutArr[MAX_DIO_BYTE_COUNT][4];
	CMutex m_clMutexWrite;
};


