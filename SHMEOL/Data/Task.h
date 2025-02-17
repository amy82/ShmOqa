#pragma once
class CTask
{
public:
	CTask();
	~CTask();

public:
	//////////////////////////////////////////////////////////////////////////
	//��������
	//0 = ����
	//1 = ������
	//2 = �Ͻ� ����
	byte m_Unit_State_Pcb;
	byte m_Unit_State_Lens;
	byte m_Unit_State_Epoxy;
	byte m_Unit_State_Loading;
	byte m_Unit_State_UnLoading;
	//
	byte m_Unit_Ready_Pcb;
	byte m_Unit_Ready_Lens;
	byte m_Unit_Ready_Epoxy;
	byte m_Unit_Ready_Loading;
	byte m_Unit_Ready_UnLoading;
	//////////////////////////////////////////////////////////////////////////
	//�Ͻ������� Step ����
	byte m_Old_Step_Pcb;
	byte m_Old_Step_Lens;
	byte m_Old_Step_Epxy;
	byte m_Old_Step_Loading;
	byte m_Old_Step_UnLoading;

	//���� �˶� �߻� ����
	bool	m_Alarm_Pcb;
	bool	m_Alarm_Lens;
	bool	m_Alarm_Epoxy;
	bool	m_Alarm_Loading;
	bool	m_Alarm_UnLoading;

	int		PcbReadyStep;
	int		LensReadyStep;
	int		LoadingReadyStep;
	int		UnloadingReadyStep;

	///int		LensTask, PCBTask, EpoxyTask, UnloadingTask;
	//int		LensPickupTask, PCBPickupTask, UnloadingPickupTask;
	//AA�� ���� üũ
	//��������
	//0 = ����
	//1 = ������
	//2 = �Ͻ� ����
	byte		m_State_AA[4];
};

