#pragma once
#include "../AXTDefine.h"
class CModuleData
{
public:
	CModuleData();
	~CModuleData();

public:
	void Load();
	void Save();
	void CopyBackup();

public:
	int m_nSysNo;
	int m_nUnitNo;

	TCHAR m_szMesIp[SIZE_OF_100BYTE];			// MES IP
	int m_nMesPort;								// MES ��Ʈ
	TCHAR m_szFactoryCd[SIZE_OF_100BYTE];		// �����ڵ�

	TCHAR m_szModelName[SIZE_OF_100BYTE];		// �� ��
	TCHAR m_szPassword[SIZE_OF_100BYTE];		// ��й�ȣ
	TCHAR m_szAAIp[SIZE_OF_100BYTE];			// MAIN PC IP
	TCHAR m_szInspIp[SIZE_OF_100BYTE];			// ȭ��˻�� PC IP

	double	m_dMotorSpeed[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//�ӵ�
	double	m_dMotorAccTime[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];	//����
	double  m_dMotorDecTime[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];	//����
	double	m_dMotorResol[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//���ڱ���
	double	m_dOrgDataset[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//��������

	double	m_dLimitSFRBalance[MAX_UNIT_COUNT];

	CDPoint m_clOCSpec[MAX_UNIT_COUNT];
	double m_dOCSpecTh[MAX_UNIT_COUNT];

	int m_nAAPort;				// MAIN PC Port
	int m_nInspPort;			// ȭ��˻�� PC Port
	int m_nLightPort;			// ���� ��Ʈ�ѷ� ��Ʈ
	int m_nDomeLightPort[2];	// DOME ���� ��Ʋ�ѷ� ��Ʈ
	int m_nLaserPort[2];		// �������� ��Ʈ

	int m_nSFR_Type[MAX_LAST_INSP_COUNT + MAX_DOME_INSP_ADD];

	TCHAR m_szMesPath[SIZE_OF_1K];				// MES LOF DATA ���� ���
};

