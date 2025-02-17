#pragma once

#include "AXTDefine.h"
#include "Library/Inspection/Include/LibACMISCommon/ACMISCommon.h"


///////////////////////////////////////////////////////////////////////////////
//
//	ModelInfo
//
class CModelInfo
{
public:
	CModelInfo()
	{
		m_nNo = 0;
		memset(m_szName, 0x00, sizeof(m_szName));
	}

public:
	int m_nNo;
	TCHAR m_szName[SIZE_OF_100BYTE];
};
///////////////////////////////////////////////////////////////////////////////
//
//	ModelList
//
class CModelList
{
public:
	CModelList();
	~CModelList();

public:
	void ModelListLoad();
	void ModelListSave();
	void Insert(TCHAR* szModelName);
	void Delete(TCHAR* szModelName);
	CModelInfo GetModelInfo(int nIndex) { return m_clModelList[nIndex]; }

public:
	CArray<CModelInfo, CModelInfo&> m_clModelList;
	int m_nCurrentNo;
	int m_nTotalCount;

	TCHAR m_szCurrentModel[SIZE_OF_100BYTE];					// MODEL NAME



public:
	int iniRecipeListLoad();
	void xmlRecipeCreate(CString copyPPid, CString createPPid);
	void xmlRecipeDelete(CString PPid);
	void RecipeModelLoad();
	void RecipeModelSave();
	std::vector<std::string> m_vRecipeVec;



public:
	bool RecipeModelDel(CString PPid);

	void RecipeListLoad();
	void RecipeListSave();
	
	CArray<CModelInfo, CModelInfo&> m_clRecipeList;
};


class CModelType
{
public:
	CModelType();
	~CModelType();

public:
	void mTLoad();
	void mTSave();
	void CopyBackup();

public:
	//int m_nModelIndex;		//1 = MODERATE , 2 = NARROW
	//TCHAR m_szModelTypeName[SIZE_OF_100BYTE];		// �� ��
};
///////////////////////////////////////////////////////////////////////////////
//
//	SystemData Class
//
class CSystemData
{
public:
	CSystemData();
	~CSystemData();

public:
	void sDLoad();
	void sDSave();

	void commonDataLoad();
	void commonDataSave();

	

	void sDCopyBackup();

	void SfrOffsetLoad();
	void OcOffsetLoad();
public:
	int m_nSysNo;
	int m_nUnitNo;
	TCHAR m_szModelName[SIZE_OF_100BYTE];					// MODEL NAME


	double	m_dSfrBasicOffset[MAX_LAST_INSP_COUNT];
	double	m_dSfrOutOffset[MAX_LAST_INSP_COUNT];

	double m_dOcOffset[2];

	TCHAR m_szMesIp[SIZE_OF_100BYTE];			// MES IP
	int m_nMesPort;								// MES ��Ʈ
	TCHAR m_szFactoryCd[SIZE_OF_100BYTE];		// �����ڵ�
	TCHAR m_szFactoryCd_RH[SIZE_OF_100BYTE];	// �����ڵ�

	TCHAR m_szFactoryComplCd[SIZE_OF_100BYTE];		// �����ڵ�
	TCHAR m_szFactoryComplCd_RH[SIZE_OF_100BYTE];	// �����ڵ�

	
	TCHAR m_szPassword[SIZE_OF_100BYTE];		// ��й�ȣ
	TCHAR m_szAAIp[SIZE_OF_100BYTE];			// MAIN PC IP
	TCHAR m_szInspIp[SIZE_OF_100BYTE];			// ȭ��˻�� PC IP
	
	double	m_dMotorSpeed[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//�ӵ�
	double	m_dMotorAccTime[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];	//����
	double  m_dMotorDecTime[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];	//����
	double	m_dMotorResol[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//���ڱ���
	double	m_dOrgDataset[MAX_UNIT_COUNT][MAX_MOTOR_COUNT];		//��������

	CDMPoint m_clCamResol[MAX_UNIT_COUNT];					// CAM X,Y ���ش�
	CDMPoint m_clCamAngle[MAX_UNIT_COUNT];					// CAM ANGLE X,Y ���ش�

	double	m_dLimitSFRBalance[MAX_UNIT_COUNT];

	CDMPoint m_clOCSpec[MAX_UNIT_COUNT];
	double m_dOCSpecTh[MAX_UNIT_COUNT];

	int m_nFileDeleteDay;
	int m_nAAPort;				// MAIN PC Port
	int m_nInspPort;			// ȭ��˻�� PC Port
	//Rs232
	int m_nTopLightPort[2];		// ž��Ʈ ��Ʈ
	int m_nLeftLightPort[2];		// left ��Ʈ ��Ʈ
	int m_nRightLightPort[2];		//right��Ʈ ��Ʈ
	int m_nEzi_Port[2];			// �̹� ���� ��Ʈ
	int m_nAlign_Oc_Port[2];		//ALIGN + OC  (��� ������Ʈ�ѷ�)

	int m_nOC_IRPort[2];			// IR ���� ��Ʈ�ѷ� ��Ʈ
	int m_nLaserPort[2];			// Laser ��Ʈ
	int m_nBcrPort[2];			// ���ڵ� ��Ʈ
	int m_nUvPort[2];			// UV ��Ʈ
	
	//
	int m_nMaxPinCount;

	int m_nDomeLightPort[MAX_UNIT_COUNT];	// DOME ���� ��Ʋ�ѷ� ��Ʈ

	int m_nSFR_Type[MAX_LAST_INSP_COUNT];

	TCHAR m_szMesPath[SIZE_OF_1K];				// MES LOF DATA ���� ���

	//int m_nModelIndex;		//0==LH , 1 = RH
};


typedef struct STRUC_TEACH_DATA
{
	double dPos[MAX_MOTOR_COUNT];
	double dOffset[MAX_MOTOR_COUNT];
} STRUC_TEACH_DATA;

typedef struct STRUC_AA_INFO
{
	double dMoveVel;			/* �̵� �ӵ� */
	double dMoveAcc;			/* ������ */
	double dMoveDistance;		/* �̵��Ÿ� : mm */
	int	nDelayTime;				/* ���� : msec */
	int	nMaxStepCount;			/* MAX STEP ��*/	
	float fLimitRoughMTF;			/* Rough �̵� Limit MTF �� */
	float fLimitFineMTF;		/* Fine �̵� Limit MTF �� */
	int nAvgSfrCount;			/* SFR ��� ī��Ʈ*/
}STRUC_AA_INFO;

class CSFRTeachingData
{
public:
	CSFRTeachingData();
	//~CSFRTeachingData();

public:
	double dSFRPcbX;
	double dSFRPcbY;
	double dSFRPcbZ;
	double dSFRPcbTx;
	double dSFRPcbTy;
};



class CSFRInfo
{
public:
	CSFRInfo()
	{
		int i;
		double dOffsetX, dOffsetY;
		for (i = 0; i < MAX_LAST_INSP_COUNT; i++)
		{
			m_nSizeX[i] = 80;// 50;
			m_nSizeY[i] = 80;// 70;
		}
		

		m_nCenSizeX = 150;
		m_nCenSizeY = 150;
		for (i = 0; i < (MAX_LAST_INSP_COUNT); i++)
		{
			m_clPtOffset[i].x = m_clPtOffset[i].y = 0;

			switch (i)
			{
			case 0:		dOffsetX = 2.15;	dOffsetY = 2.31;	break;
			case 1:		dOffsetX = 4.25;	dOffsetY = 4.82;	break;
			case 2:		dOffsetX = 1.38;	dOffsetY = 4.82;	break;
			case 3:		dOffsetX = 1.38;	dOffsetY = 1.53;	break;
			case 4:		dOffsetX = 4.25;	dOffsetY = 1.53;	break;
			case 5:		dOffsetX = 1.51;	dOffsetY = 2.41;	break;
			case 6:		dOffsetX = 3.20;	dOffsetY = 2.41;	break;
			case 7:		dOffsetX = 19.2;	dOffsetY = 20.6;	break;
			case 8:		dOffsetX = 1.15;	dOffsetY = 19.0;	break;
			case 9:		dOffsetX = 1.15;	dOffsetY = 1.27;	break;
			case 10:	dOffsetX = 20.9;	dOffsetY = 1.28;	break;
			case 11:	dOffsetX = 2.19/*1.36*/;	dOffsetY = 20.6/*1.50*/;	break;
			case 12:	dOffsetX = 2.19;	dOffsetY = 1.23;	break;
			/*case 13:	dOffsetX = 5.61;	dOffsetY = 1.51;	break;
			case 14:	dOffsetX = 11.1;	dOffsetY = 2.41;	break;
			case 15:	dOffsetX = 19.2;	dOffsetY = 20.6;	break;
			case 16:	dOffsetX = 1.15;	dOffsetY = 19.0;	break;
			case 17:	dOffsetX = 1.15;	dOffsetY = 1.27;	break;
			case 18:	dOffsetX = 20.9;	dOffsetY = 1.28;	break;*/
			}
			
			m_clPtOffset[i].x = (LONG)((double)CCD2_CAM_SIZE_X / dOffsetX);
			m_clPtOffset[i].y = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) / dOffsetY);
		}

		m_clRectCircle[0].left = (LONG)((double)CCD2_CAM_SIZE_X * 0.25);
		m_clRectCircle[0].top = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.03);
		m_clRectCircle[0].right = (LONG)((double)CCD2_CAM_SIZE_X * 0.41);
		m_clRectCircle[0].bottom = (LONG)((double)(CCD2_CAM_SIZE_Y - 4) * 0.28);

		m_clRectCircle[1].left = (LONG)((double)CCD2_CAM_SIZE_X * 0.59);
		m_clRectCircle[1].top = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.03);
		m_clRectCircle[1].right = (LONG)((double)CCD2_CAM_SIZE_X * 0.73);
		m_clRectCircle[1].bottom = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.28);

		m_clRectCircle[2].left = (LONG)((double)CCD2_CAM_SIZE_X * 0.25);
		m_clRectCircle[2].top = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.72);
		m_clRectCircle[2].right = (LONG)((double)CCD2_CAM_SIZE_X * 0.41);
		m_clRectCircle[2].bottom = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.97);

		m_clRectCircle[3].left = (LONG)((double)CCD2_CAM_SIZE_X * 0.59);
		m_clRectCircle[3].top = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.72);
		m_clRectCircle[3].right = (LONG)((double)CCD2_CAM_SIZE_X * 0.73);
		m_clRectCircle[3].bottom = (LONG)((double)(CCD2_CAM_SIZE_Y/* - 4*/) * 0.97);
	}

public:
    CRect m_clRectCircle[4];
    CPoint m_clPtCircle[4];
	

	double m_dSfrOcX;
	double m_dSfrOcY;

    CRect m_clRectFov[MAX_FOV_COUNT];
   // CPoint m_clPtFov[MAX_FOV_COUNT];
	CPoint m_clPtFovOffset[MAX_FOV_COUNT];


	int m_nFovSizeX[MAX_FOV_COUNT];
	int m_nFovSizeY[MAX_FOV_COUNT];

	double m_dFovOcX;
	double m_dFovOcY;

    CRect m_clRectSnr[MAX_SNR_COUNT];
    CPoint m_clPtSnr[MAX_SNR_COUNT];
	double m_dSnrOcX[2];
	double m_dSnrOcY[2];


	CPoint m_clPtOffset[MAX_LAST_INSP_COUNT];
	int m_nSizeX[MAX_LAST_INSP_COUNT];
	int m_nSizeY[MAX_LAST_INSP_COUNT];

	int m_nCenSizeX;
	int m_nCenSizeY;
};
enum AA_SFR_SPEC {
    AA_CL_00F_V = 0, AA_CR_OOF_V, AA_CT_00F_H, AA_CB_00F_H, AA_LT_04F_V,
    AA_LT_04F_H, AA_RT_04F_V, AA_RT_04F_H, AA_RB_04F_V, AA_RB_04F_H,
    AA_LB_04F_V, AA_LB_04F_H, AA_L_04F_V, AA_L_04F_H, AA_R_04F_V,
    AA_R_04F_H, AA_LT_07F_V, AA_LT_07F_H, AA_RT_07F_V, AA_RT_07F_H,
    AA_RB_07F_V, AA_RB_07F_H, AA_LB_07F_V, AA_LB_07F_H, MAX_SFR_AA_INSP_CNT
};

enum AA_INSP_SUMAVE_SPEC {
	_CENTER = 0,
	_AVER_07F_TOP,
	_AVER_07F_BOTTOM,
	_AVER_07F_LEFT,
	_AVER_07F_RIGHT,

	MAX_SFR_INSP_SUMAVE_CNT
};

//enum AA_INSP_SUMAVE_SPEC {
//	_CENTER = 0,
//	_AVER_05F_LT,
//	_AVER_05F_RT,
//	_AVER_05F_LB,
//	_AVER_05F_RB,
//	_AVER_07F_LT,
//	_AVER_07F_RT,
//	_AVER_07F_LB,
//	_AVER_07F_RB,
//	MAX_SFR_INSP_SUMAVE_CNT
//};



///////////////////////////////////////////////////////////////////////////////
//
//	ModelData Class
//
class CModelData
{
public:
	CModelData();
	~CModelData();

public:
	void Load(TCHAR* szModelName);
	void Save(TCHAR* szModelName);

	void LoadTeachData(TCHAR* szModelName);
	void SaveTeachData(TCHAR* szModelName);

	void ModelCopyBackup(TCHAR* szModelName);
	void TeachCopyBackup(TCHAR* szModelName);

    void PatternSave(TCHAR* szModelName);
    void PatternLoad(TCHAR* szModelName);

	bool FinaLogCopy(TCHAR* szTempLog);
	void AcmisDataLoad(TCHAR* szModelName);
	void AcmisDataSave(TCHAR* szModelName);

	void LensShadingSpecLoad(TCHAR* szModelName);
	void LensShadingSpecSave(TCHAR* szModelName);
	
	void LotDataLoad();
	void LotDataSave();

	void EpoxyDataLoad(TCHAR* szModelName);
	void EpoxyDataSave(TCHAR* szModelName);
	
	void SetUnit(int nUnit);
	void ModulTypeLoad();


	void VarationLoad();
public:
	int m_nUnit;
	/*double m_LS_5000k_SpecMin[4][221];
	double m_LS_5000k_SpecMax[4][221];

	double m_LS_3000k_SpecMin[4][221];
	double m_LS_3000k_SpecMax[4][221];*/
	//-------------------------------------------------------------------------
	//
	//Epoxy ���� 
	double m_EpoxyLineSpeed;
	double m_EpoxyLineAccDec;
	double m_EpoxyCurveSpeed;
	double m_EpoxyCurveAccDec;
	double m_EpoxyOnTime;
	double m_EpoxyOffTime;

	double m_EpoxyLineLength[4];
	double m_EpoxyCurveCenterXPoint[4];
	double m_EpoxyCurveCenterYPoint[4];
	double m_EpoxyCurveEndXPoint[4];
	double m_EpoxyCurveEndYPoint[4];


	//Epoxy Inspection ����
	CPoint m_ResingRectStart[4];			// �����κ� �˻� ���� �������� (mm)
	CPoint m_ResingRectSize[4];				// �����κ� �˻� ���� ũ��(mm)
	CDMPoint m_CircleDrawCenter[4];			// ��ȣ �߽���
	CDMPoint	m_CircleDrawSize[4];
	CDMPoint	m_ResinDrawSize;
	CDMPoint	m_dResinInspOffset[2];			// 0:���� Offset, 1:�ٱ� Offset
	int		m_nResinInspLimit;				// ���� �˻� OK / NG ���� Limit

	double	m_dResinInspRectSpec;			// �����κ� �˻� Spec �߰�(%)
	double	m_dResinInspGapLength;			// HOLE ���� ������ ���� Spec(mm)
	double	m_dResinInspHoleSpec;			// HOLE �˻� Spec �߰�(%)

	int nVarRepetCount;		//�ݺ�Ƚ��
	int nVarMode;			//0 = Min , 1 = Max , 2 = Average 

	CDMPoint	m_sfrOffset[20];//sfr ���� x�� ���ϱ� , y�� �����ֱ�

	//-------------------------------------------------------------------------
	int sfrParameter[4];	//0.DataFormat , 1.OutMode , 2.SensorType , 3.DemosaicMethod
	int blemishParameter[4];
	int colorParameter[4];
	int riParameter[4];

	int	m_dSfrBlackLevel;
	int	m_dBlemishBlackLevel;
	int	m_dColorBlackLevel;
	int	m_dRiBlackLevel;

	int m_nSfr8BitUse;
	int m_nBlemish8BitUse;
	int m_nColor8BitUse;
	int m_nRi8BitUse;

	//-------------------------------------------------------------------------
	//
	//��ACMIS �˻� Spec Data
	//-------------------------------------------------------------------------
//TAllDefectPixel_ONSEMI_RCCB m_DefectSpec;
	int n_DefectCountLimit;
	int n_ClusterCountLimit;
	//Stain

	//fov , distortion 
	double m_FovSpec[9];//

    //oc , rotate
    double m_OcSpec[2];//

    //Color Sensitivity ColorMean WhiteBalance
    //double m_ColorSensitivitySpec[3][g_ColorSenscount];//	 0 = 3000k , 1 = 5000k ,2 = Dark

    //SNR
    double m_SnrSpec[2];//

	//Uniform 
	//double m_UniformSpec[g_ColorUniformitycount];// [14];//

	//Fpn snr row column noise 
	//double m_FpnSpec[g_Fpncount];

	//Blemish Stain  
	//double m_BlemishSpec[g_Blemishcount];
	//double m_LcbSpec[g_StainLcbcount];
	//double m_YmeanSpec[g_StainYmeancount];
	//double m_FDFSpec[g_StainFdfcount];

	int m_LcbOffsetSpec[4];
	int m_YmeanOffsetSpec[4];
	int m_FDFOffsetSpec[4];

	//Defect
	//double m_DefectSpec[g_Defectcount];
	//double m_ICSpec[g_Iccount];
	//Chart  
	//Saturation  
	//double m_SaturationSpec[6];
	//IR FILTER
	//double m_IrFilterSpec[g_Ircount];

	//Color Reproduction  
	//double m_ColorReproductionSpec[32];

	//TEMPERATURE
	double m_TemperatureSpec[8];

	//TEMPERATURE
	//double INSP_Voltage_Spec;
	//double m_VoltageSpec[8];
    //RI  
    //double m_RISpec[g_Ricount];//m_RISpec[6] = CornerVariationMin , m_RISpec[7] = CornerVariationMax
	//COLOR SHADING  
    //double m_ColorShadingSpec[g_ColorShadingcount];

	//Lens shading Roi �� 221�� (���� 17�� , ���� 13��)
	CRect m_LensShadingRoi[221];


	


    double m_RirOI[100][10];
	//OC Offset X,Y
	double m_dOcOffsetX[2];
	double m_dOcOffsetY[2];
	//IC Offset X,Y
	double m_d_IC_OffsetX[2];	//������� �ۼ�Ʈ ���ϱ�
	double m_d_IC_OffsetY[2];

	//oc limit min,max
	double m_dOCSpecLimit[2];	//0 = min , 1 = max
	//Current 
	double m_CurrentSpec[2];//
	double m_7FVariation[2];
	double m_dRotateSpecLimit[2];	//0 = min , 1 = max

	double m_dTiltxSpecLimit[2];	//0 = min , 1 = max
	double m_dTiltySpecLimit[2];	//0 = min , 1 = max

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	TCHAR m_szIniPath[SIZE_OF_1K];				// �������� Ini���� ���
	int m_nWidth;								// CCD SENSOR SIZE X
	int m_nHeight;								// CCD SENSOR SIZE Y


	int DefectSizeX;
	int DefectSizeY;


	CSFRInfo m_clSfrInfo;
	STRUC_TEACH_DATA m_stTeachData[MAX_POS_COUNT];

	int m_nRoiSizeX;
	int m_nRoiSizeY;
	double m_dCcdCellSize;

	int m_nMTF_FinalMode;			//MTF Final ������ Point ��(17P=0, 24P=1)
	int m_nCcdFlipUse;				//CCD ���� ���� ��뿩��	
	int m_nOldChartMTF;				//����Ʈ MTF �˻� ����(0=�̻��, 1=���)

	double m_dMTFOffset;			//�ű� ��Ʈ MTF OFFSET ����ġ

	STRUC_AA_INFO m_stAFInfo[2];			/* AutoFocus Rough/Fine */
	double	m_dLimitSFRCent;
	double m_d2ndAAMoveDistance;				// 2�� AA ���� ���� �̵� �Ÿ�
	double	m_dLimitSFRSide;

	int m_nCountOfCheckSFR;					// SFR ���� �Ǵ� ������ ī��Ʈ
	double m_dSfrMoveDistance;				// SFR ���ҽ� �̵� �Ÿ�

	int m_nSfrMode;							// SFR ���

	int m_nSFR_N_POS;

	double m_dLaserMoveX[2];		// 0 : ����, 1 : ����
	double m_dLaserMoveY[2];		// 0 : ����, 1 : ����

	double m_dLaserComplMoveX[2];	// ����ǰ ��� -> 0 : ����, 1 : ����
	double m_dLaserComplMoveY[2];	// ����ǰ ��� -> 0 : ����, 1 : ����

	
	int m_nStainCount[2];			//! Stain �ҷ� ī��Ʈ [0] ȭ��Ʈ  [1] ��	
	
	int m_dUvTime;
	
	double m_dEscapePosPcbZ;
	
	int m_nSensorAlignRetryCount;
	double m_dSensorAlignDelay;
	int m_nLensAlignRetryCount;
	double m_dLensAlignDelay;
	int m_nCcdRetryCount;

	int m_nContactRetryCount;
	double m_nContactRetryDelay;

	int m_nLaserRetryCount;			// Laser Retry Count
	double m_dLaserDelay;

	int m_nDiffRetryCount;			// Diff Retry Count
	double m_dDiffDelay;
		
	int m_nRawSumCount;				// Raw Sum Count

	double m_dOcAlignSpec[2];		//Oc Spec X , Y

	double m_dSensorAlignSpec[3];		//SensorAlign Spec X , Y , Th
	double m_dHolderAlign[3];			//HolderAlign Spec X , Y , Th
	double m_dLensAlignSpec[3];			//LensAlign Spec   X , Y , Th
	
	int m_nDryRunMode;					// 0 : NORMAL / 1 : ������ ���
	int m_nCOMPLETE_MODE;				// 0 : AA Mode / 1 : ����ǰ ���

	int m_nUvUse;					// UV ��ȭ ��뿩��

	int m_nLaserInUse;				// LASER ���� ��뿩��
	int m_nLaserOutUse;				// LASER �ܺ� ��뿩��
	int m_nUnloadAfter1stAAUse;		// 1�� AA�� ���� ��뿩��
    //
	int m_nStainInspUse;			// STAIN �˻� ��뿩��
	int m_nDarkInspUse;				// DARK �˻� ��뿩��
	int m_nNgImageSaveUse;

	int m_nSaturationInspUse;		// SATURATION �˻� ��뿩��
	int m_nIRFilterInspUse;			// IR FILTER �˻� ��뿩��.
	int m_nColorReproductionInspUse;// COLOR REPRODUCTION �˻� ��뿩��.





	//
	int m_nRITestUse;				//ri�˻�
	int m_nCurrentUse;				//���� ���� ��뿩��
    int m_nSnrInspUse;
    int m_nDefectInspUse;
    int m_nRIInspUse;
    int m_nColorUniformityInspUse;
    int m_m_nColorSensitivityInspUse;
	//
	int m_nMesPass;			//1�̸� ������
    //
    //

	int m_nDoorLockUse;				//DOOR LOCK ��뿩��
	int m_nCcdDefectInfpUse;		// CCD �̹��˻� ��뿩��
	int m_nFpsStopUse;				// FPS ���� �� ���Ͻ� CCD STOP ��뿩��
	int m_nPatternChartUse;
	int m_nSfrAvrCheck;				//SFR ���� ������� ����
	float m_fFpsStopVal;			// FPS ���� ��
	int m_nJxlEffort;
	int m_nLaserControlLock;		// Laser Control ��뿩��
	int m_nMtfOverlayUse;
	int m_nDomeChartUse;
	int m_nUVLampAlarmUse;
	int m_nAAPass;
	int m_nInspectionPass;
	int m_nEpoxyPass;
	int m_nEpoxyInspPass;

	double m_dOffsetPrevUV_X;
	double m_dOffsetPrevUV_Y;
	double m_dOffsetPrevUV_Z;
	double m_dOffsetPrevUV_Tx;
	double m_dOffsetPrevUV_Ty;
	double m_dOffsetPrevUV_Th;
	
	double m_dEolPcbOffset_Tx;
	double m_dEolPcbOffset_Ty;

	double m_dPCBVacuumDelay;
	double m_dPCBGripDelay;
	double m_dLENSGripDelay;
	double m_dPCBGripUpDelay;
	double m_dLaserTimeOut;
	double m_dFinalDelay;
	double m_dBondingDelay;
	double m_dCcdDelay;
	
	int m_nPinStopUse;				// ������ ��뷮 STOP ��뿩��
	int m_nSensorCnt;				// ������ ��뷮
	int m_nLight[MAX_LIGHT_COUNT];
	int m_CurrentChartIndex;
	int m_nMasterModeUse;
	int m_nInspOKPass;				//eol insp ��ǰ ���� �н� ���� �� ���ڵ� Ȯ�� �� �ٷ� �����û
	int m_nAlignPass;
	int m_nRawSavePass;
	int m_nJxlSavePass;
	int m_nIdleReasonPass;


	int m_LxData[2];		//0 = 5000k , 1 = 6500K
	int m_LxVal;
	//double m_dOcOffsetX;
	//double m_dOcOffsetY;

	double m_dOkLimit[MAX_MOTOR_COUNT];
	double m_dErrLimit[MAX_MOTOR_COUNT];

	double m_dStainLsbCenter;
	double m_dStainLsbCorner;
	double m_dStainLsbEdge;

	double m_dStainYMeanCenter;
	double m_dStainYMeanCorner;
	double m_dStainYMeanEdge;

	int m_nMarkNo;
	int m_nMinMatching;

	int m_nDirection[MAX_SFR_INSP_COUNT];
	int m_nPatternDirection[MAX_SFR_INSP_COUNT];
    
    double m_dLinePulse;
	
	int		m_dDemosaic;

    CPoint m_nPatternPos[MAX_LAST_INSP_COUNT];
    CPoint m_nPatternSize[MAX_LAST_INSP_COUNT];
    CPoint m_MTF_ROI_Pos[2][MAX_SFR_INSP_COUNT];		//0 = �ڵ� ã�� 1 = �����̹��� ã��

	double m_dChartRotation;

    double m_dChartHfovMin;
    double m_dChartHfovMax;

    double m_dChartVfovMin;
    double m_dChartVfovMax;

	double m_dChartDistortionMin;
	double m_dChartDistortionMax;




	

	//int m_nModelType;
};


typedef struct STRUC_SFR_INSP
{
	int nDataCount[MAX_SFR_INSP_COUNT];

	float fMotorPos[MAX_CCD_STEP_COUNT][6];
	float fImageAlign[MAX_CCD_STEP_COUNT][4];

	CRect clRectROI[4];

	CRect	clRect[MAX_LAST_INSP_COUNT];			// íƮ�� ���̴� 19�� �簢�� �ν� ��ǥ..
	CRect	_64_Sfr_Rect[MAX_SFR_INSP_COUNT];		// íƮ�� ���̴� 64�� SFR �˻� ����..

	bool mChartRectFind[MAX_LAST_INSP_COUNT];



	CDMPoint	Sfr_Pos[MAX_CCD_STEP_COUNT][MAX_LAST_INSP_COUNT];			// AA Tile ��� �� �ڳ� �� ��ũ ��ǥ
	float	fSFR_AVR_N4[MAX_CCD_STEP_COUNT][MAX_LAST_INSP_COUNT];
	int nMaxIndex[MAX_LAST_INSP_COUNT];
	double dMaxPos[MAX_LAST_INSP_COUNT];


	float	fSfrN8[MAX_CCD_STEP_COUNT][5];

	float	fSftAll[MAX_CCD_STEP_COUNT][20];


	double dAver_MTF[MAX_SFR_INSP_COUNT][10];	//max 10ȸ
	double dAver_Var[10];//max 20ȸ

	float	_64_fSfrN4[MAX_SFR_INSP_COUNT];
	float	_64_fSfrN8[MAX_SFR_INSP_COUNT];

    double	_fAverSfr[MAX_SFR_INSP_COUNT];

	double dSfrDataX[MAX_SFR_INSP_COUNT][MAX_SFR_DATA_COUNT];
	double dSfrDataY[MAX_SFR_INSP_COUNT][MAX_SFR_DATA_COUNT];

	

	double dTiltX;
	double dTiltY;
	double dLogTiltX[2];
	double dLogTiltY[2];
	double dLogMotorTX;
	double dLogMotorTY;


} STRUC_SFR_INSP;



///////////////////////////////////////////////////////////////////////////////
//
//	MarkData Class
//
class CMarkData
{
public:
	CMarkData();
	~CMarkData();

	void SetUnit(int nUnit) { m_nUnit = nUnit; }
	void LoadData(TCHAR* szModelName);
	void SaveData(TCHAR* szModelName);
	void CopyBackup(TCHAR* szModelName);

public:
	int m_nSmooth[MAX_MARK_COUNT];
	CRect m_clRectRoi[MAX_MARK_COUNT];
	int m_nUnit;
};


///////////////////////////////////////////////////////////////////////////////
//
//	TaskWork Calss
//
class CTaskWork
{
public:
	CTaskWork();
	~CTaskWork();

public:
	void LoadData();
	void SaveData();

	void PinLoadData();
	void PinSaveData();
    void InitLogSet();
	void InitSfrInsp();
    void InitSfrSpec();
	void SetUnit(int nUnit);
public:
	int bRecv_Lgit_Pp_select;

	int bRecv_S6F12_Process_State_Change;
	int bRecv_S6F12_PP_Selected;
	int bRecv_S6F12_PP_UpLoad_Completed;
	int bRecv_S6F12_Lot_Processing_Started;
	int bRecv_S6F12_Lot_Apd;
	int bRecv_S6F12_Lot_Processing_Completed;
	int bRecv_S6F12_Lot_Processing_Completed_Ack;

	int bRecv_S7F25_Formatted_Process_Program;

	int bRecv_S2F49_PP_UpLoad_Confirm;		//Confirm or Fail
	int bRecv_S2F49_LG_Lot_Start;	//Start or Id Fail
public:
	int m_nUnit;							// UNIT NO

	TCHAR m_szLotID[SIZE_OF_100BYTE];		// LOT ID
	TCHAR m_szChipID[SIZE_OF_100BYTE];		// CHIIP ID(BCR)


	//EEPROM
	//00000000000000000000000PT00463986-A23010A000000000001
	TCHAR m_szManufacturerSerialNumber[SIZE_OF_100BYTE];		//=2304000000000001
	TCHAR m_szRivianPartNumber[SIZE_OF_100BYTE];				//=00000000000000000000000PT00463986-A
	TCHAR m_szTraceabilityDate[SIZE_OF_100BYTE];				//=23010
	TCHAR m_szTraceabilityLocation[SIZE_OF_100BYTE];			//=A
	TCHAR m_szTraceabilityToolNumber[SIZE_OF_100BYTE];			//=000000 
	TCHAR m_szTraceabilitySerialNumber[SIZE_OF_100BYTE];		//=000001
	TCHAR m_szImagePath[SIZE_OF_1K];		// ADD 

	TCHAR m_szI2cSensorLot[SIZE_OF_100BYTE];	//I2C Sensor Id Read


	TCHAR m_szFuseId[SIZE_OF_100BYTE];	//I2C Sensor Id Read
	int m_nAutoFlag;						// AUTO 
	int m_nStartStep;
	int m_nEndStep;

	bool m_bAreaSensorRun;

	int m_nStartLensStep;
	int m_nEndLensStep;

	int m_nCurrentPcbStep;
	int m_nCurrentLensStep;

	DWORD m_dwTickCount;
	DWORD m_dwLensTickCount;
	DWORD m_dwPcbTickCount;
	DWORD m_dwSupportTickCount;

	int mVariationIndex;
	int m_LightTimeCheck;
	
	int mRepetCount;
	int m_ManualMarkIndex;			//�������� ��ũ �����Ҷ� ��� , pcb �����̳� , lens �����̳� 
	bool m_bDispenseCompleteCheck;
	bool m_bLensLaserCompleteCheck;

	int m_nDrawBarStep;
	int m_nCountOfTotalStepAA;
	int m_nCountOfStepAA;
	int m_nCountOfSecondStepAA;
	bool m_bFlagDecreaseSFR;
	bool m_bSecondAA;
	bool m_bFlagFineMove;
	bool m_bFlagRoughMove;
	bool m_bSfrResult;
	int m_nResultCode;

	bool m_bRecvMesBcr;				//mes bcr ���� ����.

	CRect m_FindCircleRect[MAX_FOV_COUNT];
	CRect m_FindFovRect[MAX_FOV_COUNT];
    CPoint m_clPtCircle[4];
	CDMPoint m_clPtChartOc;


    CPoint m_clPtFov[MAX_FOV_COUNT];
    CPoint m_clPtSnr[MAX_FOV_COUNT];
	double m_dFOV[5];

	double m_dOpticalPosX;
	double m_dOpticalPosY;
	double m_dOpticalShiftX;
	double m_dOpticalShiftY;

	double m_dOutTx;
	double m_dOutTy;

	int m_nOpticalTh;
	CRect m_clOpticalRoi;

	int m_nTotalWorkCount;
	int m_nTotalOkCount;
	int m_nTotalNgCount;
	int m_nUsePinCount;
	int m_nCcdCount;

	int m_MinMaxIndex[12];
	double m_AligData[MAX_SFR_INSP_COUNT];

	int m_nMTFNgCount;
	int m_nOCNgCount;
	int m_nDistortionNgCount;
	int m_nCurrentNgCount;
	int m_nTestPatternNgCount;
	int m_nDefectInspNgCount;
	int m_nStainInspNgCount;
	int m_nDarkInspNgCount;
	int m_nUniformityNgCount;

	DWORD m_dwCycleTactStartTime;
	DWORD m_dwCycleTactEndTime;

	double m_dTotalTactTime;
	DWORD m_dwTotalTactTime;
	double m_dAlignTackTime;
	DWORD m_dwAlignTackTime;
	double m_dBcrTackTime;
	DWORD m_dwBcrTackTime;
	double m_dLaserTactTime;
	DWORD m_dwLaserTactTime;
	double m_dAATactTime;
	DWORD m_dwAATactTime;
	double m_dInspTactTime;
	DWORD m_dwInspTactTime;	
		
	//STURC_OS_CHECK	m_stOsCheck;
	STRUC_SFR_INSP	m_stSfrInsp;
	//STRUC_DEFECT_INSP m_stDefectInsp;

	int m_nRecvAlignData;
	CDMotor m_dAlignData;
	CDMPoint dpFind;
	double m_dSensorAlign[3];		// X,Y,T

	double m_dHolderAlign[3];		// X,Y,T

	double m_dLensAlign[3];		// X,Y,T


	double m_dAADataX;
	double m_dAADataY;
	double m_dAADataTh;

	double m_dDBDataX;
	double m_dDBDataY;
	double m_dDBDataTh;
	double m_dDBDataInTx;
	double m_dDBDataInTy;
	double m_dDBDataOutTx;
	double m_dDBDataOutTy;

	bool m_bRecvLaserTilt;
	int m_nLaserTiltIndex;
	double m_dLaserVal[4];
	CDMPoint m_clDpLaserPos[4];
	double m_dLaserTiltX[2];
	double m_dLaserTiltY[2];
	
	double m_dDataOffsetX[2];
	double m_dDataOffsetY[2];
	double m_dDataOffsetTh[2];

	double m_dImgShiftX;
	double m_dImgShiftY;
	double m_dImgShiftTh;

	double m_dEolChartShiftX;
	double m_dEolChartShiftY;


	double m_dOcResultX;
	double m_dOcResultY;
    double m_d0cResultTH;
	int m_nRetryCount;
	int m_nWideCenterRetryCount;
	float m_fFps;
	int m_nEmission;
	bool m_nEmissionRun;
	int m_nEpoxyCompl;					// ���� �Ϸ� �ߴ��� Ȯ��.
	int m_nLensLoading;					// Lens �Ѱ�� �� Ȯ��.

	int m_nRawSumCount;

	int grabFailCount;
	/*bool m_bFlagMoveLensBonding;
	bool m_bFlagMoveLensComplete;	
	bool m_bFlagMoveLensLoading;*/		// LENS PICKUP �̵� ��..Z
	int m_ContactCount;
	int m_IoRetry;
	bool m_bLensTransferChk;			// MAIN LENS TRANSFER ��ġ üũ ����
	bool m_bWaitLensBonding;			// LENS BONDING �����...
	bool m_bMovingLens;					// LENS �̵���...
	bool m_bMoveLensLoading;			// LENS BUFFER�� LOADING ��...
	
	bool m_bLensMovePickup;				// LENS BUFFER�� PICKUP�Ϸ� �̵���..
	bool m_bLensBufferLensOn;				// LENS BUFFER�� ��ǰ ���� Ȯ��
	int  m_nComplType;					// ����ǰ OK/�̹�NG/�ػ��NG

	bool m_bLensSocketIF[MAX_LENS_IF_COUNT];
	bool m_bPcbSocketIF[MAX_PCB_IF_COUNT];
	bool m_bUldSocketIF[MAX_ULD_IF_COUNT];
	
	bool m_bDoorInterlock;
	bool m_bLensInterLock;
	bool m_bPcbInterLock;

	bool m_bManualAAMove;
	bool m_bManualLaserPos;				// TRUE : ����, FALSE : �ܺ�

	bool m_bManualTest;
	bool m_bInputSignal;
	bool m_bOutputSignal;

	bool m_bPcbExist;

	bool m_bOutputCheck[20];		
	//0 = Relative Illumination			[Color Shading] 2��
	//1 = Color Uniformity				[Color Uniformity]
	//2 = SNR & Row/Col. Noise			[FPN] 2��
	//3 = Blemish						[LCB, Ymean, FDF]
	//4 = Defect (light & dark)			[All Defect Pixel (Rivian)]
	//5 = White Balance					[Relative Illumination X]
	//6 = Lens Shading					[Color Shading]
	//7 = Dark Pixel Uniformity (dark)	[Color Sensitivity - Mean]
	//8 = White Balance (dark)			[FPN]
	//9 = Row/Col. Noise (dark)			[IR Filter]
	bool bIdleTimeExceed;
	TCHAR m_szIdleStartTime[SIZE_OF_100BYTE];
	TCHAR m_szIdleEndTime[SIZE_OF_100BYTE];


	//[0]BCR , [1]���� , [2]I2C , [3]OC , [4]MTF , [5]DISTORTION , [6]STAIN , [7]RI , [8]DEFECT
    std::vector<int> m_vDirection;
    std::vector<double> m_vFrquency;
    std::vector<double> m_vOffset;
    std::vector<double> m_vSFR;
    std::vector<double> m_vThreshold;
    std::vector<TROIPoint> m_vROI;
};




class CMandoInspLog
{
public:
	CMandoInspLog();
	~CMandoInspLog();

	void InitData();
	void SaveLogAfterAA(int nUnit);			//AA��(UV��) Log Data

	void DrawNGOverlay(int nCh);	//NG�׸� Overlay Draw
	void NGCheck(CString strNg);
public:
	SYSTEMTIME	m_stInspTime;		//�˻� ���� �ð� ����

	CString		m_sBarcodeID;			//���ڵ��
	double		m_dTilteOffset[2];	//PCB<->Lens ���� ���� ����[TX, TY]
	double		m_dPCBOffset[3];	//PCB<->Lens Offset ����[X/Y/T]
	bool		m_bInspRes;			//���
	CString		m_sNGList;



	CRectPos m_ChartVertex[50];	//�輺�� å�� SHM�𵨻�� ��Ʈ ������ ��ǥ 23�� ���

	CPoint m_ShmFovPoint[MAX_FOV_FIND_COUNT];
	CString		m_sI2C;				//TestPattern �˻� Ʋ�� ����
	bool m_TpTestResult[2];
	//-------------------------------------------------------------------------------------
	//�˻� ���
	//-------------------------------------------------------------------------------------
	//RI = func_Insp_ColorShading
	double m_dRiMinR[2];	//0 = 5000k , 1 = Dark
	double m_dRiMinGr[2];	//0 = 5000k , 1 = Dark
	double m_dRiMinGb[2];	//0 = 5000k , 1 = Dark
	double m_dRiMinB[2];	//0 = 5000k , 1 = Dark

	double m_dRiMinRG[2];	//0 = 5000k , 1 = Dark
	double m_dRiMinBG[2];	//0 = 5000k , 1 = Dark
	double m_dRiMaxRG[2];	//0 = 5000k , 1 = Dark
	double m_dRiMaxBG[2];	//0 = 5000k , 1 = Dark
	//
	//
    double      m_dOCResult[2];
	double		m_dFov;
	double		m_dDistortion;
	double		m_dSnr;
	double		m_dRotate;
	double      m_Tilt[2];
	double		m_dCurrent;			//���� ������
	double		m_dDefect[2];		//Defect�˻�[White/Black]
	double		m_dStain[2];
	double		m_Dark;
	double		m_RI;
	double		m_ColorUniformity;		//�̹��� ���� �ʿ�
	double      m_ColorSensitivity;

	//�µ� ����
	double      m_dTemperatureResult[3];
	double      m_dHumidity;

	//voltage
	double      m_dVoltageVal[4];
	double		dVoltage;		

	//Saturation
	double      m_dSaturationResult[3];	//0 = min , 1 = max

	//Ir Filter
	double      m_dIrFilterResult[3];

	//Color reproduction
	double      m_dColorReproductionResult[4];
	//
	//Lens Shading
	int m_LogLensShadingR[2][221];
	int m_LogLensShadingGr[2][221];
	int m_LogLensShadingGb[2][221];
	int m_LogLensShadingB[2][221];

	double	m_LogLensShadingAvg[2][12];		//min , avg , max
    //-------------------------------------------------------------------------------------
    //DEFECT
	double  m_LogDefectDarkInDark;
	double  m_LogDefectBrightInDark;
	double  m_LogDefectDarkInBright;
	double  m_LogDefectBrightInBright;

	double  m_LogDefectClusterDarkInDark;
	double  m_LogDefectClusterBrightInDark;
	double  m_LogDefectClusterDarkInBright;
	double  m_LogDefectClusterBrightInBright;

	double m_Hot;
	double m_Dead;
	double m_Row;
	double m_Column;


	double  m_LogDefectBright;
    double  m_LogDefectDark;
    double  m_LogDefectHot;

	double  m_LogDefectBrightCouplet;
	double  m_LogDefectDarkCouplet;
	double  m_LogDefectHotCouplet;

    double  m_LogDefectBrightCluster;
    double  m_LogDefectDarkCluster;
    double  m_LogDefectHotCluster;

    double  m_LogDefectDarkHorLine;
    double  m_LogDefectDarkVerLine;
    double  m_LogDefectBrightHorLine;
    double  m_LogDefectBrightVerLine;
    double  m_LogDefectColumnFPN[4];//Red Cr Cb Blue 1 0 3 2
    //

	double		m_UvBefore_OC_X;          //890
	double		m_UvBefore_OC_Y;          //930
	double		m_UvBefore_OC_DelatX;     //-20
	double		m_UvBefore_OC_DelatY;     //20
	double		m_UvAfter_OC_X;          //890
	double		m_UvAfter_OC_Y;          //930
	double		m_UvAfter_OC_DelatX;     //-20
	double		m_UvAfter_OC_DelatY;     //20


	double		m_LogIC_DelatX;     //-20
	double		m_LogIC_DelatY;     //20
    //
    double      m_LogDistortion;

    //
	double   m_UvBeforeSFR[MAX_SFR_INSP_COUNT];
	double   m_UvAfterSFR[MAX_SFR_INSP_COUNT];

	double      m_4FDiff;
	double      m_7FDiff;


	double	m_Uniformity[10];
	double   m_UvBeforeSFR_0F_Diff;
	double   m_UvBeforeSFR_5F_Diff;
	double   m_UvBeforeSFR_85F_Diff;
	double   m_UvBeforeSFR_5F_Diff_A;
	double   m_UvBeforeSFR_85F_Diff_A;

	double   m_UvAfterSFR_0F_Diff;
	double   m_UvAfterSFR_5F_Diff;
	double   m_UvAfterSFR_85F_Diff;
	double   m_UvAfterSFR_5F_Diff_A;
	double   m_UvAfterSFR_85F_Diff_A;


	double  m_UvBefore_Rotate;
	double  m_UvAfter_Rotate;

	double m_SaturationBrightAvg;
    //
    double  m_LogBlemishBlackSpot;
    double  m_LogBlemishLcb;
	double  m_LogBlemishRU_Ymean;
	double  m_LogBlemishFPF;

	double  m_LogBlemishBlackSpotMaxCenter;

	double  m_LogBlemishLcbMaxCenter;
	double  m_LogBlemishLcbMaxEdge;
	double  m_LogBlemishLcbMaxCorner;

	double  m_LogBlemishYmeanMaxCenter;
	double  m_LogBlemishYmeanMaxEdge;
	double  m_LogBlemishYmeanMaxCorner;

	double m_LogBlemishFdfMaxCenter;
	double m_LogBlemishFdfMaxEdge;
	double m_LogBlemishFdfMaxCorner;
    //
	double m_Log_ColorSensitivity_Result[3][7];//[4];	//[0 = 3000k , 1 = 5000k , 2 = Dark][0=R,1=Gr,2=Gb,3=B]
	



    double m_Log_CS_RG_6500K;
    double m_Log_CS_BG_6500K;
    double m_Log_CS_GrGb_6500K;
    double m_Log_CS_RC_5000k;
    double m_Log_CS_BC_5000k;
    double m_Log_CS_CrCb_5000k;
    
    //
    //RI
    double m_Log_RI_RED[100];
    double m_Log_RI_CLEARR[100];
    double m_Log_RI_CLEARB[100];
    double m_Log_RI_BLUE[100];
	//
	double m_Log_RI[5][2];
	double m_Log_Shading[4];		//
	//
	double m_Log_RI_CenterIntensity;
	double m_Log_RI_Up_Ratio;
	double m_Log_RI_Left_Ratio;
	double m_Log_RI_Right_Ratio;
	double m_Log_RI_Down_Ratio;
	double m_Log_RI_Difference;
	//
	double m_Log_RI_CornerVar_R;
	double m_Log_RI_CornerVar_Cr;
	double m_Log_RI_CornerVar_Cb;
	double m_Log_RI_CornerVar_B;
    //
    double m_Log_RI_Center_R;
    double m_Log_RI_Center_Cr;
    double m_Log_RI_Center_Cb;
    double m_Log_RI_Center_B;
    //
    double m_Log_RI_LT_Ratio_R;
    double m_Log_RI_LT_Ratio_Cr;
    double m_Log_RI_LT_Ratio_Cb;
    double m_Log_RI_LT_Ratio_B;
    //
    double m_Log_RI_RT_Ratio_R;
    double m_Log_RI_RT_Ratio_Cr;
    double m_Log_RI_RT_Ratio_Cb;
    double m_Log_RI_RT_Ratio_B;
    //
    double m_Log_RI_LB_Ratio_R;
    double m_Log_RI_LB_Ratio_Cr;
    double m_Log_RI_LB_Ratio_Cb;
    double m_Log_RI_LB_Ratio_B;
    //
    double m_Log_RI_RB_Ratio_R;
    double m_Log_RI_RB_Ratio_Cr;
    double m_Log_RI_RB_Ratio_Cb;
    double m_Log_RI_RB_Ratio_B;

    //
    double m_Log_SNR_SNR;
    double m_Log_SNR_DR;
	double m_Log_Fpn5000kResult[3][4];	////0 = SNR , 1 = ROW , 2 = COLUMN
	double m_Log_FpnDarkResult[3][4];	////0 = SNR , 1 = ROW , 2 = COLUMN

	//double m_Log_SnrResult[3][4];	
	//double m_Log_NoiseResult[3][4];

    //
    double  m_Log_CU_LT_Delta_RG;
    double  m_Log_CU_LT_Delta_RB;
    double  m_Log_CU_LT_Delta_BG;

    double  m_Log_CU_RT_Delta_RG;
    double  m_Log_CU_RT_Delta_RB;
    double  m_Log_CU_RT_Delta_BG;

    double  m_Log_CU_LB_Delta_RG;
    double  m_Log_CU_LB_Delta_RB;
    double  m_Log_CU_LB_Delta_BG;

    double  m_Log_CU_RB_Delta_RG;
    double  m_Log_CU_RB_Delta_RB;
    double  m_Log_CU_RB_Delta_BG;
    //
    double  m_Log_FOV_H_FOV;
    double  m_Log_FOV_V_FOV;
    double  m_Log_FOV_D_FOV;
    

    //
    //-------------------------------------------------------------------------------------

	int iNGCnt;
	CString sDispNG[200];
	double getMaxMin(int pos, int type);		//lim 240430
	//CMandoInspLog();
	

    //-------------------------------------------------------------------------------------




	double		m_dShading65FC[2];
	double		m_dShading85FC[2];





	//double		m_d60lpmm[24];		//60lp/mm (17Point)
	//double		m_d67lpmm[24];     //67lp/mm (17Point)

	double		m_dMinData[3];    //20150601

								  //! Mainȭ�� NG List Overlay ���� ����
	int			m_nNGCnt;
	CString		m_sDispNG[50];
	CString		m_sSendNGName;
};


class CMandoSfrSpec
{
public:
	CMandoSfrSpec();
	~CMandoSfrSpec();

	void SetUnit(int nUnit){ m_nUnit = nUnit; }

	void Load();
	void Save();
public:
	//�ű� ���� -> 60LP, C/P�� ����, Only 67LP��.
	//���� 60LP, C/P �߰��� �κ��� ����, C/P�� �����Ҽ� �� ������
	//�迭 3���� �׷��� ����, [0]�迭, [2]�迭�� ��� ���� �ʴ´�.
	// AA��� ����
	double	m_dAASFR_Spec[MAX_SFR_INSP_COUNT][2];//[3];	
	//210825
	// [INDEX][0][0]  : AA
	// [INDEX][0][1]  : EOL


	// [0][0] : UV�� 60lp, [0][1] : UV�� 67lp, [0][2] : UV�� C/P, 
	// [1][0] : UV�� 60,   [1][1] : UV�� 67lp, [1][2] : UV�� C/P
	// ȭ��˻�� ����

	double	m_dAASFR_SumAveSpec[MAX_SFR_INSP_SUMAVE_CNT][2];	//0 = UV�� / EOL , 1 = AA UV�ĸ� ���
	double	m_dAASFR_DiffMaxSpec[5][2];	//0 = AA , 1 = EOL
	double m_dINSPSFR_Spec[MAX_SFR_INSP_COUNT][2];

	bool m_bInsp60Lp;
	bool m_bInsp67Lp;
	bool m_bInspCp;
	double m_dSfrOffsetMultiply;
private:
	int m_nUnit;
};

