#pragma once



class CMesCommunication
{
public:
	CMesCommunication();
	~CMesCommunication();

	void Insp();
	void Save(int index);

	void secsGemLoad();
	void secsGemSave();
public:

	bool g_FinalEolLog(int index);
	bool g_Final_OqaLog(int index);

	bool g_FovVertexLog(int index);

	bool g_Grr____LaserMotorPos(int index);
	bool g_Grr____Align(int index);
	bool MesAASave(int index);
	bool MesEolSave(int index);

	std::vector<_PP_RECIPE_SPEC> RecipeIniLoad(CString recipefile);
	bool RecipeIniSave(std::vector<_PP_RECIPE_SPEC> ppRecipeVec);

	double mapKeyRtn(std::string strKey);

	std::vector<PP_RECIPE_SPEC> vPPRecipeSpecEquip;
public:
	CString m_sMesLotID;				// ���ڵ��
	int		m_nMesCnt;					// ����
	int		m_nMesFinalResult;			// �պ�

	DWORD m_dwCycleTactStartTime;
	DWORD m_dwCycleTactEndTime;
	DWORD m_dwMesCycleTime;
	int		m_nMesCycleTimeResult;

	double	m_dMesCurrent;				// ��������
	int		m_nMesCurrentResult;
	double	m_dMesVoltage;				// VOLTAGE		[4]
	int		m_nMesVoltageResult;		//[4]	

	double	m_dMesSensorVoltage[3];				// SENSOR VOLTAGE		[4]
	int		m_nMesSensorVoltageResult[3];		//[4]	

	double	m_dMesUVAfterOC[2];			//OC ���.[X/Y]  UV ��
	double	m_dMesUVAfterDeltaOC[2];	//OC ���.[X/Y]  UV �� OC_Y - Image Height / 2
	int		m_nMesUVAfterOCResult[2];


	int		m_nMesDefect[12];
	int		m_nMesDefectResult[12];


	double  m_dMesOC[2];
	double  m_dMesDeltaOC[2];

	double  m_dMesDeltaUVAfterOC[2];
	double	m_nMesBlemish[3];			//0 = LCB , 1 = FDF , 2 = RU_Ymean
	double	m_nMesBlemishMaxDefect[9];
	int	m_nMesBlemishMaxDefectResult[9];

	double	m_nMesColorUniformity[8];
	int		m_nMesColorUniformityResult[8];

	double	m_nMesRIRoi[2][5][4];//0 = white , 1 = dark
	int		m_nMesRIRoiResult[2][5][4];

	double	m_nMesSHMRI[20];		//lim240430

	double  m_nMesRICorner[4];
	double  m_dMesRiDiff;
	double  m_dMesRiMinDiff;
	double m_dMesRiCenterRaw;

	int		m_nMesRICornerResult[4];
	int		m_nMesRIDiffResult;
	int		m_nMesRIMinDiffResult;
	int		m_nMesRICenterRawResult;



	int		m_nMesRIResult[2][8];

	double	m_nMesFpn5000[3][4];		//0 = snr, 1 = c , 2 = r
	int		m_nMesFpn5000Result[3][4];

	double	m_nMesFpnDark[3][4];		//0 = snr, 1 = c , 2 = r
	int		m_nMesFpnDarkResult[3][4];

	double	m_nMesLensShading[2][12];		//0 = 3000k , 1 = 5000k
	int		m_nMesLensShadingResult[2][12];


	int		m_nMesStain;
	int		m_nMesUVBeforeOCResult;
	int		m_nMesDistortionResult;
	int		m_nMesStainResult;
	int		m_nMesDarkResult;

	byte	m_dI2cData[4];
	int		m_dI2cDataResult[4];

	byte	m_dIspData[2];
	int		m_dIspDataResult[2];

	int m_dChartBright[9];
	int	m_dChartBrightResult[9];
	int m_dOcBright[5];
	int	m_dOcBrightResult[5];

	byte mCrcData[5];
	//TCHAR	mStrData[MES_VERIFY_SPEC_COUNT][SIZE_OF_100BYTE];
	//byte mReadEEpromData[2500];
	//byte mMesGetEEpromData[2500];		//cal���� �����ִ� eeprom data ���� pc���� ���ڵ� ���� �� �޴´�.
	//byte compareData[300];

	double m_dGrrBeForeLaserPos[4];

	double m_dGrrLaserPos[4];
	double m_dGrrMotorPos[6];
	double m_dGrrAlignPos[3];


	double   m_dMesUvBeforeMTF[MAX_SFR_INSP_COUNT];
	double   m_dMesUvAfterMTF[MAX_SFR_INSP_COUNT];

	double m_dMesSaturationChart[MAX_LAST_INSP_COUNT];
	double m_dMesSaturationOc;

	double	m_dMes4F_Diff;
	double	m_dMes7F_Diff;
	int		m_dMes4F_DiffResult;
	int		m_dMes7F_DiffResult;
	double	m_nMesRI[2][8];		//0 = white , 1 = dark
	double	m_dMes7FVariation;
	int		m_dMes7FVariationResult;
	int		m_nMesUvBeforeMTFResult[MAX_SFR_INSP_COUNT];
	int		m_nMesUvAfterMTFResultUVAfter[MAX_SFR_INSP_COUNT];
	double	m_dMesMTF[MAX_SFR_INSP_COUNT];				//MTF �˻� �׸�
	double	m_dMesAreaAvrMTF[MAX_LAST_INSP_COUNT];
	double  m_dMesAreaAvrUVBeforeMTF[MAX_LAST_INSP_COUNT];
	double  m_dMesMTF_AVER[MAX_SFR_INSP_SUMAVE_CNT];

	int		m_nMesMTFResult[MAX_SFR_INSP_COUNT];
	int		m_nMesMTFAreaAvrResult[MAX_LAST_INSP_COUNT];
	int		m_nMesMTFResult_AVER[MAX_SFR_INSP_SUMAVE_CNT];
	int		m_nMesMTFResult_UVBeforeAVER[MAX_SFR_INSP_SUMAVE_CNT];




	double  m_dMesICOC[2];
	double  m_dMesICDeltaOC[2];


	int m_dMesOcLightTime[5];		//ch1 ~ ch4   ���Ƿ� �÷�����   4���� ���
	int m_dMesOcLightTimeResult[5];	//


	double  m_nMesColorSensitivity[3][7];	//[4];



	double	m_dMesDistortion;

	double	m_nMesColorReproduction[4];

	double	m_dMesSnr;
	double	m_dMesDr;
	double	m_dMesFov[2];	//0 = HFov , 1 = VFov


	double	m_dMesUvAfterRotate;
	double  m_dMesTilt[2];
	double	m_dMesUvBeforeRotate;


	double	m_dMesSaturationBrightAvg;
	int	m_dMesSaturationBrightAvgResult;

	double	m_dMesTemperature[4];
	int	m_dMesTemperatureResult[4];//�µ� 0,1,2  ����3

	double		m_dMesLightChartValue[MAX_LAST_INSP_COUNT];
	double		m_dMesLightOcValue;


	//IR Filter  m_stIRFilterResult
	double m_dMesIRFilterValue[3];
	int	m_dMesIRFilterResult[3];


	double	m_dMesIrROIBrightAvg;
	int	m_dMesIrROIBrightAvgResult;

	double	m_dMesIrRoiBrightVar;
	int	m_dMesIrRoiBrightVarResult;
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	//result

	int		m_dMesOCResult[2];
	int		m_dMesDeltaOCResult[2];
	int		m_dMesUVAfterOCResult[2];
	int		m_dMesUVAfterDeltaOCResult[2];


	int		m_dMesICOCResult[2];
	int		m_dMesICDeltaOCResult[2];
	int		m_nMesColorSensitivityResult[3][7];//[4];

	int		m_nMesRIRICornerResult[4];


	int		m_nMesColorReproductionResult[4];
	int		m_nMesBlemishResult[3];
	int		m_dMesDistortionResult;
	int		m_dMesSnrResult;
	int		m_dMesDrResult;
	int		m_dMesFovResult[2];
	int		m_dMesRotateResult;
	int		m_dMesRotateUVAfterResult;
	int		m_dMesTiltResult[2];
	int		m_dMesEEPROMResult[4];
	int		m_dMesNoiseResult;


	int		m_dMesLightChartResult[MAX_LAST_INSP_COUNT];
	int		m_dMesLightOcResult;
	CString m_sendData;
	CString m_getData;

	//----------------------------------------------------------------------------------------------------------------------
public:

	double m_dMesFocalLengthFx;
	double m_dMesFocalLengthFy;

	double m_dMesPrincipalPointCx;
	double m_dMesPrincipalPointCy;

	double m_dMesRadialDistortionK1;
	double m_dMesRadialDistortionK2;
	double m_dMesRadialDistortionK3;

	double m_dMesTangentialDistortionP1;
	double m_dMesTangentialDistortionP2;


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

	double   m_UvBeforeSFR_0F_DiffResult;
	double   m_UvBeforeSFR_5F_DiffResult;
	double   m_UvBeforeSFR_85F_DiffResult;
	double   m_UvBeforeSFR_5F_Diff_AResult;
	double   m_UvBeforeSFR_85F_Diff_AResult;

	double   m_UvAfterSFR_0F_DiffResult;
	double   m_UvAfterSFR_5F_DiffResult;
	double   m_UvAfterSFR_85F_DiffResult;
	double   m_UvAfterSFR_5F_Diff_AResult;
	double   m_UvAfterSFR_85F_Diff_AResult;
	int		m_nMesShading65FCResult[2];
	int		m_nMesShading85FCResult[2];


	double	m_dMesMTF67lp[MAX_SFR_INSP_COUNT];			//MTF �˻� �׸�
	double  m_dMesMTF67lpUVAfter[MAX_SFR_INSP_COUNT];
	int		m_nMesMTF67lpResult[MAX_SFR_INSP_COUNT];
	int		m_nMesMTF67lpResultUVAfter[MAX_SFR_INSP_COUNT];
	int		m_nMesDark;
	CString m_sMesfuseID;				// FUSE ���̵�
	int		m_nsMesfuseIDResult;

	//CString m_sMesI2cSensorID;				// i2c ���� ���̵�
	TCHAR m_sMesI2cSensorID[SIZE_OF_100BYTE];
	int		m_nMesI2CResult;

	double	m_dMesShading65FC[2];
	double	m_dMesShading85FC[2];
	CString m_sMesI2C;					// I2C
	CString m_sBlackSpot;				// �̹�
	CString m_sStain;					// �̹�
	CString m_sDefect;					// defect
	CString m_sNoise;					// Noise

	double	m_dMesUVBeforeOC[2];		//OC ���.[X/Y]  UV ��
	//----------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------

	std::vector<int> m_uAlarmList;


	TCHAR	vMesModelList[g_MaxModelCount][SIZE_OF_100BYTE];

	std::vector<std::string> vMesApdData;
	CString m_sMesOperatorID;				// OPERATOR ID
	CString m_sEquipmentID;
	CString m_sEquipmentName;
	CString m_sRecipeId;
	CString m_sErcmdCode;
	CString m_sErcmdText;
	//
	CString m_sLotID;
	CString m_sProcessID;

	int m_dLotProcessingState;
	CString m_dEqpDefectCode;
	int m_dAlarmCode;
	//Progress Status
	int m_dEqupControlState[2];		//0 = Prev , 1 = Current
	int m_dEqupOperationMode[2];	//0 = EquipmentOperationMode , 1 = EqpOperationModeChangeOrderType
	int m_dControlStateChangeOrder;
	int m_dProcessState[2];		//0 = Prev Process State, 1 = Current Process State
	bool m_bLgit_Pause_req;

	CString m_sMesPPID;
	CString m_sMesRecipeRevision;
	int m_dPPChangeArr[2];	//0 = PPChangedState , 1 = PPChangeOrderType
							//use ECID count = 12

	int EstablishCommunicationsTimeout;		//ECID 101
	int HeartBeatRate;						//ECID 102
	int DefaultCommState;					//ECID 103
	int DefaultCtrlState;					//ECID 104
	int DefaultOfflineSubstate;				//ECID 105
	int DefCtrlOfflineState;				//ECID 106
	int TimeFormat;							//ECID 107
	int DefaultOnlineSubState;				//ECID 108
	int ConversationTimeoutCount;			//ECID 109

											//EquipmentID_1;						// ECID 201
											//g_clMesCommunication[m_nUnit].m_sEquipmentID
	int m_dTerminalId;
	bool IdleReasonReportUsage;				// ECID 221
	int IdleSetTimeInterval;				// ECID 222 Min

};