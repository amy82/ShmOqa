#include "StdAfx.h"
#include "Aps_Insp.h"
#include "GlobalDeclare.h"
#include <random>
//#include <ppl.h>
#include <bitset>

//#define R_RESULT_PASS							0	//��ǰ 
//#define R_RESULT_FAIL							1	//�ҷ�
//#define R_FAIL_NOISE							2
//#define R_FAIL_BLACK_SPOT						3
//#define R_FAIL_STAIN							4
//#define R_FAIL_INSPECTION						5

using namespace ACMISSoftISP;
typedef struct __TShadingSpec
{
	TRelativeUniformitySpec  m_stRelativeUniformitySpec;
	TColorSensitivitySpecN m_stColorSensitivitySpec;
	TRelativeIlluminationSpecX m_stRelativeIlluminationXSpec;
	TRelativeIlluminationSpecN m_stRelativeIlluminationNSpec;
} TShadingSpec;


CAps_Insp::CAps_Insp(void)
{
	mInspRetryCount = 0;
	//cTestPat_Ref_buf = NULL;
	//imagePattern = cvCreateImage(cvSize(g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight), IPL_DEPTH_8U, 3);
}


CAps_Insp::~CAps_Insp(void)
{
	//cvReleaseImage(&imagePattern);
}
///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
bool CAps_Insp::Func_FinalImageSave(BYTE* RawImage)
{
	int i = 0;
	
	char strTmp[1024];
	TCHAR szLog[SIZE_OF_1K];
	CvPoint pt;
	CvFont cvfont;
	/// Font Face
	int myFontFace = 0;
	int mthickness = 3;
	CvScalar mColor = Scalar(170, 0, 0);
		
	/// Font Scale
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	

	double nModelFontScale = 1.0;
	double nRoiScale = 1.0;
	nModelFontScale = 1.0;
	nRoiScale = 1;


	double nFontSize = 1.1;
	double myFontScale = 1.2 * nModelFontScale;
	cvInitFont(&cvfont, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_NORMAL, nFontSize, nFontSize, 0, 1, 10);

	ACMISSoftISP::xMakeBMP(RawImage, (byte*)g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer,
	g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n);

	Mat MatImage = Mat(g_clModelData[m_nUnit].m_nHeight, g_clModelData[m_nUnit].m_nWidth, CV_8UC3);
	MatImage.data = g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer;
	//memcpy(testImage.data, (char*)g_clLaonGrabberWrapper[m_nUnit].m_pFrameBMPBuffer, g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 3);
	
	
	//eol SFR
	for (i = 0; i < MAX_SFR_INSP_COUNT; i++)
	{
		if (i < 4)
		{
			//0,1,2,3
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[0].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[0].top + (280 * nRoiScale) + (60 * i);
		}
		else if (i < 6)
		{
			//4,5
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[4].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[1].top + (235 * nRoiScale) +  (60 * (i - 4));
		}
		else if (i < 8)
		{
			//6,7
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[6].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[2].top + (235 * nRoiScale) + (60 * (i - 6)); 
		}
		else if (i < 10)
		{
			//8,9
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[8].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[3].top - (100 * nRoiScale) + (60 * (i - 8));
		}
		else if (i < 12)
		{
			//4
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[10].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[4].top - (100 * nRoiScale) + (60 * (i - 10));
		}
		else if (i < 14)
		{
			//12,13
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[12].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[5].top + (270 * nRoiScale) + (60 * (i - 12));
		}
		else if (i < 16)
		{
			//14,15
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[14].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[6].top + (270 * nRoiScale) + (60 * (i - 14));
		}
		else if (i < 18)
		{
			//16,17
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[16].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[7].top - (170 * nRoiScale) + (60 * (i - 16));
		}
		else
		{
			//18,19
			pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[18].left - (100 * nRoiScale);
			pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp.clRect[8].top - (170 * nRoiScale) + (60 * (i - 18));
		}
		sprintf_s(strTmp, "[%d] %.6lf", i, g_clMesCommunication[m_nUnit].m_dMesUvAfterMTF[i]);
		putText(MatImage, strTmp, pt, myFontFace, myFontScale, mColor, mthickness);


	
		sprintf_s(strTmp, "%d", i);
		pt.x = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].left;
		pt.y = (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].top + (40);
		putText(MatImage, strTmp, pt, myFontFace, 1.1, Scalar(100, 255, 20), mthickness);	//myFontScale
		rectangle(MatImage, cvPoint((int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].left, (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].top),
				cvPoint((int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].right, (int)g_clTaskWork[m_nUnit].m_stSfrInsp._64_Sfr_Rect[i].bottom), Scalar(150, 200, 150), 2);
	}
	//oc
	pt.x = (int)nWidth / 2 - (250 * nRoiScale);// 1300;
	pt.y = (int)(300 * nRoiScale);
	sprintf_s(strTmp, "OC X: %.3lf, OC Y: %.3lf", g_clMesCommunication[m_nUnit].m_dMesUVAfterDeltaOC[0], g_clMesCommunication[m_nUnit].m_dMesUVAfterDeltaOC[1]);
	putText(MatImage, strTmp, pt, myFontFace, myFontScale, mColor, mthickness);
	//FOV
	pt.x = (int)nWidth / 2 - (250 * nRoiScale);//(int)1300;
	pt.y = (int)(350 * nRoiScale);
	sprintf_s(strTmp, "TILT X: %.3lf, TILT Y: %.3lf", g_clMesCommunication[m_nUnit].m_dMesTilt[0], g_clMesCommunication[m_nUnit].m_dMesTilt[1]);
	putText(MatImage, strTmp, pt, myFontFace, myFontScale, mColor, mthickness);
	pt.x = (int)nWidth / 2 - (250 * nRoiScale);//(int)1300;
	pt.y = (int)(400 * nRoiScale);
	sprintf_s(strTmp, "ROTATION: %.3lf", g_clMesCommunication[m_nUnit].m_dMesUvAfterRotate);
	putText(MatImage, strTmp, pt, myFontFace, myFontScale, mColor, mthickness);
		


	//===============================================================================================================================
	//jpg Save Path
	TCHAR szPath[SIZE_OF_1K];
	TCHAR szFilePath[SIZE_OF_1K];
	SYSTEMTIME stSysTime;
	CFileFind clFinder;
	CString sFileMode = _T("");
	CFile clFile;
	FILE* fp;
	int nStepNo;
	::GetLocalTime(&stSysTime);

	if (_tcslen(g_clTaskWork[m_nUnit].m_szLotID) <= 0)
		_tcscpy_s(g_clTaskWork[m_nUnit].m_szLotID, SIZE_OF_100BYTE, _T("(NULL)"));

	if (_tcslen(g_clTaskWork[m_nUnit].m_szChipID) <= 0)
		_tcscpy_s(g_clTaskWork[m_nUnit].m_szChipID, SIZE_OF_100BYTE, _T("(NULL)"));

	if (clFinder.FindFile(BASE_LOG_PATH) == FALSE)
		CreateDirectory(BASE_LOG_PATH, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\IMAGE"), BASE_LOG_PATH);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\IMAGE\\%04d%02d"), BASE_LOG_PATH,  stSysTime.wYear, stSysTime.wMonth);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\IMAGE\\%04d%02d\\%02d"), BASE_LOG_PATH,  stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);

	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\IMAGE\\%04d%02d\\%02d\\%s"), BASE_LOG_PATH,  stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, g_clTaskWork[m_nUnit].m_szChipID);
	if (clFinder.FindFile(szPath) == FALSE)
		CreateDirectory(szPath, NULL);


	_stprintf_s(szFilePath, SIZE_OF_1K, _T("%s\\%s_%d_ChartImage%02d%02d%02d.jpg"), szPath, g_clTaskWork[m_nUnit].m_szChipID, g_clSysData.m_nUnitNo, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);

	imwrite(szFilePath, MatImage);

	return true;
}


int CAps_Insp::func_ModelLotCheck(TCHAR *BcrLot)
{
	int leng = _tcslen(BcrLot);
	if (leng < 9)
	{
		return -1;
	}
#if (____MACHINE_NAME == MODEL_FRONT_100)
	const TCHAR compareStr[] = _T("002");
#else
	const TCHAR compareStr[] = _T("001");
#endif
	int nRtn = 0;
	nRtn = _tcsncmp(&BcrLot[6], compareStr, 3);		//0�̸� ��ġ

	return nRtn;

}
bool CAps_Insp::PPL_Func_Chart_Insp(BYTE* RawImage, bool bAutoMode)	//EOL ȭ�� �˻�
{
	TCHAR szLog[SIZE_OF_1K];
	bool TotalRtn = true;
	//
	// -SFR 


	return TotalRtn;
}
bool CAps_Insp::PPL_Func_OC_Insp(BYTE* RawImage, bool bAutoMode)
{
	TCHAR szLog[SIZE_OF_1K];
	bool TotalRtn = true;
	//Stain
	//Blemish �˻�
	if (g_clModelData[m_nUnit].m_nStainInspUse == 0)
	{
		g_clMandoInspLog[m_nUnit].m_LogBlemishBlackSpot = 0;
		g_clMandoInspLog[m_nUnit].m_LogBlemishLcb = 0;
		g_clMandoInspLog[m_nUnit].m_LogBlemishRU_Ymean = 0;
		g_clMandoInspLog[m_nUnit].m_LogBlemishFPF = 0;
		
		g_clMesCommunication[m_nUnit].m_nMesBlemish[0] = 0;
		g_clMesCommunication[m_nUnit].m_nMesBlemish[1] = 0;
		g_clMesCommunication[m_nUnit].m_nMesBlemish[2] = 0;
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[0] = 1;
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[1] = 1;
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[2] = 1;
		////
		_stprintf_s(szLog, SIZE_OF_1K, _T("[AUTO] Blemish �˻� PASS"));
		AddLog(szLog, 0, m_nUnit);
	}
	else
	{
		g_clApsInsp[m_nUnit].func_Insp_Stain(RawImage, true);
	}
	

	return TotalRtn;
}
///-------------------------------------------------------------------------------------
bool CAps_Insp::func_EEprom_CheckSum_Check(bool bAutoMode)
{
#if 0
	unsigned char szTemp[MAX_PATH];
	char szData[MAX_PATH];
	unsigned int nAddress;
	unsigned int checkSumTemp = 0;
	int endCheckSum = 0;
	int totlaCount = 0;
	int leng = 0;
	int i = 0;
	int j = 0;
	bool rtn = false;
	bool bCheckSumrtn = true;
	byte ReadChecksumData[2];
	byte TotalReadData[200];
	memset(TotalReadData, 0x00, sizeof(TotalReadData));

	SYSTEMTIME eTime;
	::GetLocalTime(&eTime);
	// 
	for (i = 0; i < 4; i++)
	{
		g_clMesCommunication[m_nUnit].m_dMesEEPROMResult[i] = 1;
	}
	CString _RtnData = _T("");
	TCHAR szLog[SIZE_OF_1K];

	int startAddress = 32;
	int endAddress = 115; 		//checksum Address 114�� 115 �� checksum

	unsigned short mslaveAddr = 0x52;	//Sensor PCB Slave Address

	eepromData[m_nUnit].EEPROM_TotalRead_Type(mslaveAddr, startAddress, endAddress, TotalReadData, 1);//�ִ� 32���� 32 ~ 113���� + üũ�� 2����Ʈ

	ReadChecksumData[0] = TotalReadData[82];	//Checksum(2byte)
	ReadChecksumData[1] = TotalReadData[83];


	endCheckSum = 82;//  // 32~113 �� 82�� �о�ߵ� 

	unsigned short crcAfter = 0;

	crcAfter = CRC_CalculateCRC16_CCIT(TotalReadData, endCheckSum);

	CString ReadCheckSum;
	CString CalCheckSum;
	ReadCheckSum.Format("%02X%02X", ReadChecksumData[1], ReadChecksumData[0]);		//1,0������ ���� little endian���� ����ż�
	CalCheckSum.Format("%04X", crcAfter);
	rtn = eepromData[m_nUnit].CompareHexData(ReadCheckSum, CalCheckSum, ASCII_MODE);
	if (rtn == true)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]CheckSum OK : %s/%s"), ReadCheckSum, CalCheckSum);
		AddLog(szLog, 0, m_nUnit);
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_dMesEEPROMResult[0] = 0;
		bCheckSumrtn = false;
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]CheckSum NG : %s/%s"), ReadCheckSum, CalCheckSum);
		AddLog(szLog, 0, m_nUnit);
	}
	//������ �� 3����
	/*
	[EEPROM] Signature Code	Variable	Variable	������ �˻� �ʿ� (5041, 2bytes)
	[EEPROM] Parameter Version	Variable	Variable	������ �˻� �ʿ� (02 00 00 00 00 00 00 00, 8bytes)(0200000000000000, 8bytes)
	[EEPROM] Calibration Model	Variable	Variable	������ �˻� �ʿ� (00 00 00 00, 4bytes)(00000000, 4bytes)

	*/
	byte SignatureData[2] = { 0x50 , 0x41 };
	int SignatureStartAddress = 0;
	//[EEPROM] Signature Code[0x50(P) , 0x41(A)] = 2byte
	for (i = 0; i < 2; i++)
	{
		if (TotalReadData[i + SignatureStartAddress] != SignatureData[i])
		{
			g_clMesCommunication[m_nUnit].m_dMesEEPROMResult[1] = 0;
			bCheckSumrtn = false;
			_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Signature Code NG %d/%d"), TotalReadData[i], SignatureData[i]);
			AddLog(szLog, 0, m_nUnit);
		}
	}
	//[EEPROM] Parameter Version
	byte ParameterVersionData[8] = { 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	int ParameterVersionStartAddress = 2;
	for (i = 0; i < 8; i++)
	{
		if (TotalReadData[i + ParameterVersionStartAddress] != ParameterVersionData[i])
		{
			g_clMesCommunication[m_nUnit].m_dMesEEPROMResult[2] = 0;
			bCheckSumrtn = false;
			_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Parameter Version NG %d/%d"), TotalReadData[i + 2], ParameterVersionData[i]);
			AddLog(szLog, 0, m_nUnit);
		}
	}
	//[EEPROM] Calibration Model
	byte CalibrationModelData[4] = { 0x00 , 0x00 , 0x00 , 0x00 };
	int CalibrationModelStartAddress = 10;
	for (i = 0; i < 4; i++)
	{
		if (TotalReadData[i + CalibrationModelStartAddress] != CalibrationModelData[i])
		{
			g_clMesCommunication[m_nUnit].m_dMesEEPROMResult[3] = 0;
			bCheckSumrtn = false;
			_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Calibration Model NG %d/%d"), TotalReadData[i + 10], CalibrationModelData[i]);
			AddLog(szLog, 0, m_nUnit);
		}
	}
	//Data Read
	//���⼭ ������ �Ǽ��� ����

	int address = 0;

	address = 32;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 2, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Signature Code Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szSignaturecode, SIZE_OF_100BYTE, _T("%s"), _RtnData);			//2byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Signature Code:%s"), g_clMesCommunication[m_nUnit].m_szSignaturecode);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 34;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 8, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Parameter Version Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szParameterVersion, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//8byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM] Parameter Version:%s"), g_clMesCommunication[m_nUnit].m_szParameterVersion);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 42;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Calibration Model Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szCalibrationModel, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Calibration Model:%s"), g_clMesCommunication[m_nUnit].m_szCalibrationModel);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 46;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Focal Length (fx) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szFocalLengthFx, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Focal Length (fx):%s"), g_clMesCommunication[m_nUnit].m_szFocalLengthFx);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 50;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Focal Length (fy) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szFocalLengthFy, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Focal Length (fy):%s"), g_clMesCommunication[m_nUnit].m_szFocalLengthFx);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 54;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Principal Point (cx) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPrincipalPointCx, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Principal Point (cx):%s"), g_clMesCommunication[m_nUnit].m_szPrincipalPointCx);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 58;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Principal Point (cy) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPrincipalPointCy, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Principal Point (cy):%s"), g_clMesCommunication[m_nUnit].m_szPrincipalPointCy);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 62;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k1) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szRadialDistortionK1, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k1):%s"), g_clMesCommunication[m_nUnit].m_szRadialDistortionK1);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 66;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k2) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szRadialDistortionK2, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k2):%s"), g_clMesCommunication[m_nUnit].m_szRadialDistortionK2);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 70;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k3) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szRadialDistortionK3, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Radial Distortion (k3):%s"), g_clMesCommunication[m_nUnit].m_szRadialDistortionK3);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 98;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Tangential Distortion (P1) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szTangentialDistortionP1, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Tangential Distortion (P1):%s"), g_clMesCommunication[m_nUnit].m_szTangentialDistortionP1);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 102;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Tangential Distortion (P2) Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szTangentialDistortionP2, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Tangential Distortion (P2):%s"), g_clMesCommunication[m_nUnit].m_szTangentialDistortionP2);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 114;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 2, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Checksum Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szCheckSum, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//2byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Checksum:%s"), g_clMesCommunication[m_nUnit].m_szCheckSum);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 116;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 11, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Production date Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szProductiondate, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//11byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Production date:%s"), g_clMesCommunication[m_nUnit].m_szProductiondate);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 127;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]RMS Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szRMS, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]RMS:%s"), g_clMesCommunication[m_nUnit].m_szRMS);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 131;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]FOV Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szFOV, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]FOV:%s"), g_clMesCommunication[m_nUnit].m_szFOV);
	AddLog(szLog, 0, m_nUnit);
	//
	address = 135;
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 4, HEX_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Calibration Temperature Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szCalibrationTemperature, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//4byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Calibration Temperature:%s"), g_clMesCommunication[m_nUnit].m_szCalibrationTemperature);
	AddLog(szLog, 0, m_nUnit);

	//�߰��Ȱ� 2�� 210819
	address = 139;  //16byte
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 16, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]PCBA Part Number Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPartNumberVersion, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//16byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]PCBA Part Number and Version :%s"), g_clMesCommunication[m_nUnit].m_szPartNumberVersion);
	AddLog(szLog, 0, m_nUnit);

	address = 155;	//13byte
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 13, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Imager ES Version Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szImagerESVersionInfo, SIZE_OF_100BYTE, _T("%s"), _RtnData);		//13byte
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Imager ES Version Information :%s"), g_clMesCommunication[m_nUnit].m_szImagerESVersionInfo);
	AddLog(szLog, 0, m_nUnit);


	Sleep(5);
	//==============================================================================================================================================
	//
	//
	//power pcb read �� 3�� 210819
	mslaveAddr = 0x50;		//Power PCB Slave Address


	address = 32;	//16byte
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 16, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]PPCBA Part Number Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPowerPcbPartNumberVersion, SIZE_OF_100BYTE, _T("%s"), _RtnData);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM Power]PPCBA Part Number and Version :%s"), g_clMesCommunication[m_nUnit].m_szPowerPcbPartNumberVersion);
	AddLog(szLog, 0, m_nUnit);

	address = 48;	//13byte
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 13, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Motional Part Number Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPowerPcbMotionalPartNumber, SIZE_OF_100BYTE, _T("%s"), _RtnData);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM Power]Motional Part Number :%s"), g_clMesCommunication[m_nUnit].m_szPowerPcbMotionalPartNumber);
	AddLog(szLog, 0, m_nUnit);

	address = 48;	//11byte
	_RtnData = eepromData[m_nUnit].EEPROM_Read(mslaveAddr, address, 11, ASCII_MODE, true);
	if (_RtnData == "FALSE")
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]Production date Read Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(g_clMesCommunication[m_nUnit].m_szPowerProductiondata, SIZE_OF_100BYTE, _T("%s"), _RtnData);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM Power]Production date :%s"), g_clMesCommunication[m_nUnit].m_szPowerProductiondata);
	AddLog(szLog, 0, m_nUnit);


	//====================================================================================================================================================
	//�Ǽ� ��ȯ

	g_clMesCommunication[m_nUnit].m_dMesFocalLengthFx = CovertToFloat(g_clMesCommunication[m_nUnit].m_szFocalLengthFx, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]FocalLengthFx:%lf"), g_clMesCommunication[m_nUnit].m_dMesFocalLengthFx);
	AddLog(szLog, 0, m_nUnit);
	g_clMesCommunication[m_nUnit].m_dMesFocalLengthFy = CovertToFloat(g_clMesCommunication[m_nUnit].m_szFocalLengthFy, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]FocalLengthFy:%lf"), g_clMesCommunication[m_nUnit].m_dMesFocalLengthFy);
	AddLog(szLog, 0, m_nUnit);

	g_clMesCommunication[m_nUnit].m_dMesPrincipalPointCx = CovertToFloat(g_clMesCommunication[m_nUnit].m_szPrincipalPointCx, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]PrincipalPointCx:%lf"), g_clMesCommunication[m_nUnit].m_dMesPrincipalPointCx);
	AddLog(szLog, 0, m_nUnit);
	g_clMesCommunication[m_nUnit].m_dMesPrincipalPointCy = CovertToFloat(g_clMesCommunication[m_nUnit].m_szPrincipalPointCy, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]PrincipalPointCy:%lf"), g_clMesCommunication[m_nUnit].m_dMesPrincipalPointCy);
	AddLog(szLog, 0, m_nUnit);


	g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK1 = CovertToFloat(g_clMesCommunication[m_nUnit].m_szRadialDistortionK1, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]RadialDistortionK1:%lf"), g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK1);
	AddLog(szLog, 0, m_nUnit);
	g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK2 = CovertToFloat(g_clMesCommunication[m_nUnit].m_szRadialDistortionK2, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]RadialDistortionK2:%lf"), g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK2);
	AddLog(szLog, 0, m_nUnit);
	g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK3 = CovertToFloat(g_clMesCommunication[m_nUnit].m_szRadialDistortionK3, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]RadialDistortionK3:%lf"), g_clMesCommunication[m_nUnit].m_dMesRadialDistortionK3);
	AddLog(szLog, 0, m_nUnit);


	g_clMesCommunication[m_nUnit].m_dMesTangentialDistortionP1 = CovertToFloat(g_clMesCommunication[m_nUnit].m_szTangentialDistortionP1, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]TangentialDistortionP1:%lf"), g_clMesCommunication[m_nUnit].m_dMesTangentialDistortionP1);
	AddLog(szLog, 0, m_nUnit);
	g_clMesCommunication[m_nUnit].m_dMesTangentialDistortionP2 = CovertToFloat(g_clMesCommunication[m_nUnit].m_szTangentialDistortionP2, FLOAT_MODE);
	_stprintf_s(szLog, SIZE_OF_1K, _T("EEPROM]TangentialDistortionP2:%lf"), g_clMesCommunication[m_nUnit].m_dMesTangentialDistortionP2);
	AddLog(szLog, 0, m_nUnit);

	_RtnData.Empty();
	/////delete[] TotalReadData;		//new ���������� ���
	return bCheckSumrtn;
#endif
	return false;
}

int CAps_Insp::func_EEprom_Parse(TCHAR *ptszLot)
{
	int nRtn = 0;
	//_stprintf_s(g_clTaskWork[m_nUnit].m_szChipID, SIZE_OF_100BYTE, _T("00000000000000000000000PT00463985-A23040A000000000001"));	//�ѱ��� 33 + "-" +19  = 53
	int _leng = _tcslen(g_clTaskWork[m_nUnit].m_szChipID);
	if (_leng < 53)
	{
		return -1;	//���� ���� ����

	}
	int findPos = 0;
	CString mLotTemp;
	mLotTemp.Format(_T("%s"), g_clTaskWork[m_nUnit].m_szChipID);
	findPos = mLotTemp.Find(_T("-"));	//33


	//CString temp = mLotTemp.Mid(findPos);	// = "-"
	CString FirstLot = mLotTemp.Left(findPos + 2);		//00000000000000000000000PT00463985-A
	_stprintf_s(g_clTaskWork[m_nUnit].m_szRivianPartNumber, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)FirstLot);
	_leng = _tcslen(g_clTaskWork[m_nUnit].m_szRivianPartNumber);
	if (_leng < 35) 
	{
		return -2;	//���� ���� ����

	}
	CString LastLot = mLotTemp.Mid(findPos + 2);		//23040A000000000001
	findPos = LastLot.Find(_T("A"));		//Location Index

	//
	
	//Traceability Code - Date
	_stprintf_s(g_clTaskWork[m_nUnit].m_szTraceabilityDate, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)LastLot.Left(5));		//���� ���ۺ��� �
	//Traceability Code - Location
	_stprintf_s(g_clTaskWork[m_nUnit].m_szTraceabilityLocation, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)LastLot.Mid(5,1));
	//Traceability Code - Tool Number
	_stprintf_s(g_clTaskWork[m_nUnit].m_szTraceabilityToolNumber, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)LastLot.Mid(6,6));
	//Traceability Code - Serial Numer
	_stprintf_s(g_clTaskWork[m_nUnit].m_szTraceabilitySerialNumber, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)LastLot.Right(6));	//������ ������ ���� �

	LastLot.Replace(_T("0A"), _T(""));
	_stprintf_s(g_clTaskWork[m_nUnit].m_szManufacturerSerialNumber, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)LastLot);
	_leng = _tcslen(g_clTaskWork[m_nUnit].m_szManufacturerSerialNumber);	//16��
	if (_leng < 16)
	{
		return -3;	//���� ���� ����

	}
	mLotTemp.Empty();
	FirstLot.Empty();
	LastLot.Empty();
	return nRtn;
}

bool CAps_Insp::func_EEprom_Write(bool bAutoMode)
{

	return false;
}

///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------
bool CAps_Insp::func_EEprom_FuseID(bool bAutoMode)
{
	/*CString sLog = _T("");
	CString strData = _T("");
	CString _addr = _T("");
	CString wData = _T("");
	unsigned char szTemp[MAX_PATH];
	char szData[MAX_PATH];
	unsigned int nAddress;
	double dData = 0.0;
	int totlaCount = 0;
	int errorCode = 0;
	int wLength = 0;
	int leng = 0;
	int i = 0;
	int j = 0;
	int k = 0;

	byte rtnHexData[MAX_PATH];
	byte *totalData = new byte[2000];
	memset(rtnHexData, 0x00, sizeof(rtnHexData));

	memset(totalData, 0x00, sizeof(byte) * 2000);
	CTime cTime = CTime::GetCurrentTime();*/

	int writingByteNum;

	if (!g_clLaonGrabberWrapper[m_nUnit].m_pBoard->SetWPDisable())
	{
		AddLog(_T("[eeprom] Write Disable fail!!!!!"), 1, m_nUnit);
		return false;
	}
	else {
		AddLog(_T("[eeprom] Write Disable Success"), 1, m_nUnit);
	}

	CString m_strSlaveAddr, m_strAddr;
	CString m_strRegData;
	unsigned int nSlaveAddress;
	unsigned int nAddress;
	unsigned char pData[1025];
	int errorCode = 0;
	byte m_cAddrLength;
	DWORD m_cRegDataLength;

	nSlaveAddress = 0X10;
	nAddress = 0X301A;
	m_cAddrLength = 2;
	m_cRegDataLength = 2;

	sscanf_s(m_strSlaveAddr.GetBuffer(0), "%x", &nSlaveAddress);
	sscanf_s(m_strAddr.GetBuffer(0), "%x", &nAddress);

	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, m_cAddrLength, pData, (unsigned short)m_cRegDataLength);

	if (errorCode)
	{
		m_strRegData.Format("READ Error : %d", errorCode);
		m_strRegData.AppendFormat(" %s", g_clLaonGrabberWrapper[m_nUnit].GetErrorStr(errorCode));
		return false;
	}
	else
	{
		m_strRegData = "";
		for (int i = 0; i < (int)m_cRegDataLength; i++)
		{
			m_strRegData.AppendFormat("%02X ", pData[i]);
		}
	}

	if (m_strRegData == "0x10DC")
	{
		errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, m_cAddrLength, pData, (unsigned short)m_cRegDataLength);

		if (errorCode)
		{
			m_strRegData.Format("0x10FC WRITE Error : %d", errorCode);
			m_strRegData.AppendFormat(" %s", g_clLaonGrabberWrapper[m_nUnit].GetErrorStr(errorCode));
			return false;
		}
		else
		{
		}

	}


	return true;

}

///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------
bool CAps_Insp::FnShmFastCornerFind(BYTE* ChartRawImage, bool bAutoMode)
{
	int i = 0;
	int j = 0;
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szPos[SIZE_OF_100BYTE];
	TCHAR szFilePath[SIZE_OF_1K];

	

	TDATASPEC tspec;
	tspec.eOutMode = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eOutMode;
	tspec.eDataFormat = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDataFormat;
	tspec.eSensorType = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eSensorType;
	tspec.eDemosaicMethod = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDemosaicMethod;

	BYTE* m_pBMPTempBuffer;

	m_pBMPTempBuffer = new BYTE[g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize()];
	memset(m_pBMPTempBuffer, 0, g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize());

	ACMISSoftISP::xMakeBMP(ChartRawImage, (byte*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, tspec);

	Mat OrgImage(Size(g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight), CV_8UC3);
	std::memcpy(OrgImage.data, (char*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 3);

	//MbufGet(vision.MilGrabImageChild[3], OrgImage.data);//MbufGet(vision.MilGrabImageChild[1], OrgImage.data);
	if (bAutoMode == false)
	{
		imwrite("d:\\OrgImage.jpg", OrgImage);
	}
	if (m_pBMPTempBuffer)
	{
		delete m_pBMPTempBuffer;
	}
	m_pBMPTempBuffer = NULL;

	int thresh_value = 50;// 70;  //���� 12.28 = 70
	int max_value = 255;
	//
	int mRoiX = 0;
	int mRoiY = 0;

	int dDrawTextX = 0;
	int dDrawTextY = 0;

	int mRoiWidth = g_clModelData[m_nUnit].m_nWidth - mRoiX;
	int mRoiHeight = g_clModelData[m_nUnit].m_nHeight - mRoiY;
	
	int offsetX = ((g_clModelData[m_nUnit].m_nWidth/2) - (g_clTaskWork[m_nUnit].m_clPtCircle[0].x + g_clTaskWork[m_nUnit].m_clPtCircle[1].x + g_clTaskWork[m_nUnit].m_clPtCircle[2].x + g_clTaskWork[m_nUnit].m_clPtCircle[3].x) / 4) * -1;
	int offsetY = ((g_clModelData[m_nUnit].m_nHeight / 2) - (g_clTaskWork[m_nUnit].m_clPtCircle[0].y + g_clTaskWork[m_nUnit].m_clPtCircle[1].y + g_clTaskWork[m_nUnit].m_clPtCircle[2].y + g_clTaskWork[m_nUnit].m_clPtCircle[3].y) / 4) * -1;

	// Shi-Tomasi �ڳ� ���� �Ķ���� ����
#if (____MACHINE_NAME == MODEL_FRONT_100)
	//100���� 8�� ���� ���� ��ũ
	//new 16�� ������
	FOV_AREA_CHECK nFovMode[MAX_FOV_FIND_COUNT] = {
		LT_FOV_PIONT, RT_FOV_PIONT,
		LT_FOV_PIONT, BR_FOV_PIONT,
		LT_FOV_PIONT, BR_FOV_PIONT,
		BL_FOV_PIONT, RT_FOV_PIONT,
		LT_FOV_PIONT, RT_FOV_PIONT,BL_FOV_PIONT, BR_FOV_PIONT,
		BR_FOV_PIONT, BL_FOV_PIONT,RT_FOV_PIONT, LT_FOV_PIONT,
	};
	int maxCorners = 6;					//������ �ڳ��� �ִ� �����Դϴ�.
#else
	//150���� 2�� ���� , 6�� ������
	//new 14�� ������
	FOV_AREA_CHECK nFovMode[MAX_FOV_FIND_COUNT] = {
		BL_FOV_PIONT, RT_FOV_PIONT,
		LT_FOV_PIONT, BR_FOV_PIONT,
		LT_FOV_PIONT, BR_FOV_PIONT,
		BL_FOV_PIONT, RT_FOV_PIONT,
		BL_FOV_PIONT, BR_FOV_PIONT,LT_FOV_PIONT, RT_FOV_PIONT,
		LT_FOV_PIONT, BR_FOV_PIONT };// , LT_FOV_PIONT, BR_FOV_PIONT
	int maxCorners = 4;					//������ �ڳ��� �ִ� �����Դϴ�.
#endif
	
	int nIndex = 0;
	Mat grayImg;
	CPoint m_clPtCircle[MAX_FOV_COUNT];



	double qualityLevel = 0.05;//0.05;			//�ڳ� ǰ���� ���ϴ� ���Դϴ�. �Ϲ������� 0�� 1 ������ ���� ���
	double minDistance = 50;//80;			//����� �ڳʵ� ������ �ּ� �Ÿ��Դϴ�. 
	int blockSize = 3;					//�ڳ� ������ ���� ����� ũ���Դϴ�. 
	bool useHarrisDetector = false;		//true�� �����ϸ� Harris �ڳ� ����⸦ ����ϰ�, false�� �����ϸ� Shi-Tomasi ����⸦ ����մϴ�.
	double kvalue = 0.04;				//useHarrisDetector�� true�� ���� ���,�Ϲ������� 0.04���� 0.06 ������ ���� ���

	




	for (int k = 0; k < MAX_FOV_COUNT; k++)
	{
		mRoiX = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].x + offsetX;
		mRoiY = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].y + offsetY;
		if (mRoiX < 0) mRoiX = 0;
		if (mRoiY < 0) mRoiY = 0;

		mRoiWidth = (int)(g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeX[k]);
		mRoiHeight = (int)(g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeY[k]);

		if ((mRoiX + mRoiWidth) >= g_clModelData[m_nUnit].m_nWidth)
		{
			mRoiWidth = g_clModelData[m_nUnit].m_nWidth - mRoiX - 1;
		}

		if ((mRoiY + mRoiHeight) >= g_clModelData[m_nUnit].m_nHeight)
		{
			mRoiHeight = g_clModelData[m_nUnit].m_nHeight - mRoiY - 1;
		}
		if (mRoiWidth < 0) mRoiWidth = 1;
		if (mRoiHeight < 0) mRoiHeight = 1;
		

		Mat roiimg = OrgImage(cvRect(mRoiX, mRoiY, mRoiWidth, mRoiHeight));
		cv::cvtColor(roiimg, grayImg, cv::COLOR_BGR2GRAY);
		

		// �ڳ� ã�� Ư¡�� ����
		vector<Point2f> corners;
		Mat img_corners;		//�ڳ� ã��

		//��ã��
		std::vector<cv::Vec3f> circles;

		
		cvtColor(grayImg, img_corners, COLOR_GRAY2BGR);
		
		{
			// Shi-Tomasi �ڳ� ����
			goodFeaturesToTrack(grayImg, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, kvalue);
			if (corners.empty())
			{
				return false;
			}
			int msize = corners.size();
			if (msize < 1) msize = 1;
			CPoint m_clPtCornerTemp[MAX_FOV_FIND_COUNT];
			//sort(corners.begin(), corners.end(), [](const Point2f& p1, const Point2f& p2) {
				//return p1.x < p2.x;
			//});

			Point2f topLeft = corners[0];		// TL ������ ã��
			Point2f bottomLeft = corners[0];	// BR ������ ã��
			Point2f topRight = corners[0];		// TR ������ ã��
			Point2f bottomRight = corners[0];	// BR ������ ã��

			int nCornerSize = corners.size();
			for (i = 0; i < nCornerSize; i++)
			{
				if (corners[i].x + corners[i].y < topLeft.x + topLeft.y)
				{
					topLeft = corners[i];
				}
				if (corners[i].x - corners[i].y > topRight.x - topRight.y)
				{
					topRight = corners[i];
				}
				if (corners[i].x + corners[i].y > bottomRight.x + bottomRight.y) 
				{
					bottomRight = corners[i];
				}
				if (corners[i].x - corners[i].y < bottomLeft.x - bottomLeft.y) 
				{
					bottomLeft = corners[i];
				}
				circle(img_corners, corners[i], 5, Scalar(0, 255, 0), 2, 8, 0);
				if (i < MAX_FOV_FIND_COUNT)
				{
					m_clPtCornerTemp[i].x = corners[i].x;
					m_clPtCornerTemp[i].y = corners[i].y;
				}

				g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[i].x = corners[i].x + mRoiX;
				g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[i].y = corners[i].y + mRoiY;

				_stprintf_s(szLog, SIZE_OF_1K, _T("[Fov Corner #%d] x:%d, y:%d"), k,
					g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[i].x, g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[i].y);

				AddLog(szLog, 0, m_nUnit);
			}

			m_clPtCircle[LT_FOV_PIONT].x = topLeft.x + mRoiX;
			m_clPtCircle[LT_FOV_PIONT].y = topLeft.y + mRoiY;

			m_clPtCircle[RT_FOV_PIONT].x = topRight.x + mRoiX;
			m_clPtCircle[RT_FOV_PIONT].y = topRight.y + mRoiY;

			m_clPtCircle[BL_FOV_PIONT].x = bottomLeft.x + mRoiX;
			m_clPtCircle[BL_FOV_PIONT].y = bottomLeft.y + mRoiY;

			m_clPtCircle[BR_FOV_PIONT].x = bottomRight.x + mRoiX;
			m_clPtCircle[BR_FOV_PIONT].y = bottomRight.y + mRoiY;


			dDrawTextX = 10;
			if (m_clPtCircle[nFovMode[nIndex]].x > (g_clModelData[m_nUnit].m_nWidth / 2))
			{
				dDrawTextX = -50;

			}
			if (k < 4)
			{
				//����� 2����
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].x = m_clPtCircle[nFovMode[nIndex]].x; 
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].y = m_clPtCircle[nFovMode[nIndex]].y;
				g_clVision.DrawMOverlayCross(m_nUnit, m_nUnit, m_clPtCircle[nFovMode[nIndex]], 20, M_COLOR_CYAN, 2);// TRUE); 
				_stprintf_s(szPos, SIZE_OF_100BYTE, _T("H-%d"), nIndex);
				
				g_clVision.DrawMOverlayText(m_nUnit, m_clPtCircle[nFovMode[nIndex]].x + dDrawTextX, m_clPtCircle[nFovMode[nIndex]].y - 40, szPos, M_COLOR_GREEN, _T("Arial"), 13, 17);
				nIndex++;
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].x = m_clPtCircle[nFovMode[nIndex]].x;
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].y = m_clPtCircle[nFovMode[nIndex]].y;
				g_clVision.DrawMOverlayCross(m_nUnit, m_nUnit, m_clPtCircle[nFovMode[nIndex]], 20, M_COLOR_CYAN, 2);// TRUE); 
				_stprintf_s(szPos, SIZE_OF_100BYTE, _T("H-%d"), nIndex);
				g_clVision.DrawMOverlayText(m_nUnit, m_clPtCircle[nFovMode[nIndex]].x + dDrawTextX, m_clPtCircle[nFovMode[nIndex]].y - 40, szPos, M_COLOR_GREEN, _T("Arial"), 13, 17);
				nIndex++;
			}
			else
			{

				//K = 4,5,6,7
				//8,9,10,11
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].x = m_clPtCircle[nFovMode[nIndex]].x;
				g_clMandoInspLog[m_nUnit].m_ShmFovPoint[nIndex].y = m_clPtCircle[nFovMode[nIndex]].y;
				g_clVision.DrawMOverlayCross(m_nUnit, m_nUnit, m_clPtCircle[nFovMode[nIndex]], 20, M_COLOR_CYAN, 2);// TRUE); 
				_stprintf_s(szPos, SIZE_OF_100BYTE, _T("V-%d"), nIndex - 8);
				g_clVision.DrawMOverlayText(m_nUnit, m_clPtCircle[nFovMode[nIndex]].x + dDrawTextX, m_clPtCircle[nFovMode[nIndex]].y - 40, szPos, M_COLOR_GREEN, _T("Arial"), 13, 17);
				nIndex++;
			}


		}

#if 0		//�� ã�� ����
		if (nFovMode[k] == CIRCLE_FOV_PIONT)	//k < 4)
		{
			// Hough Circle Transform ����
			int mdp = 1;						//dp=1�̸� �Է� �̹����� ������ �ػ󵵸� ���
			int mMinDist = grayImg.rows/2;		// / 8;//����� ���� �߽ɵ� ������ �ּ� �Ÿ��Դϴ�
			int mParam1 = 100;// 200;			//ĳ�� ���� Ž���⿡ ���޵Ǵ� ���� �Ӱ谪�Դϴ�. 
			int mParam2 = 10;					//�߽� ������ ���� ������� �Ӱ谪�Դϴ�. �� ���� �������� �� ���� ���� ����Ǹ�, ����� �߸��� ���� ���Ե� �� �ֽ��ϴ�.
			int mMinRadius = 10;					//������ ���� �ּ� �������Դϴ�. 
			int mMaxRadius = 35;					//������ ���� �ִ� �������Դϴ�.   �簢���� ������ max ������ �߿���
			cv::HoughCircles(grayImg, circles, cv::HOUGH_GRADIENT, mdp, mMinDist, mParam1, mParam2, mMinRadius, mMaxRadius);
			if (circles.empty() )
			{ 
				return false;
			}
			cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
			int radius = cvRound(circles[0][2]);
			g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[0].x = circles[0][0] + mRoiX;
			g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[0].y = circles[0][1] + mRoiY;

			_stprintf_s(szLog, SIZE_OF_1K, _T("[Fov Circle #%d] x:%d, y:%d"), k,
				g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[0].x, g_clMandoInspLog[m_nUnit].m_ChartVertex[k].Pos[0].y);
			AddLog(szLog, 0, m_nUnit);

			circle(img_corners, center, 3, Scalar(0, 255, 0), -1, 8, 0);


			m_clPtCircle[k].x = circles[0][0] + mRoiX;
			m_clPtCircle[k].y = circles[0][1] + mRoiY;
		}

		else
#endif
		
		
		


		
		//���⼭ �������߿� �ϳ� ���� �ɵ� 
		//
		if (bAutoMode == false)
		{


			_stprintf_s(szFilePath, SIZE_OF_1K, _T("%s\\fovroi%d.jpg"), BASE_LOG_PATH,  k + 1);
			imwrite(szFilePath, img_corners);

			//namedWindow("Shi-Tomasi Corners", WINDOW_NORMAL);// WINDOW_AUTOSIZE);
			//imshow("Shi-Tomasi Corners", img_corners);

			//waitKey(0);
		}
	}
	if (bAutoMode == false)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[MANUAL] FOV IMAGE SAVE :%s"), BASE_LOG_PATH);
		AddLog(szLog, 0, m_nUnit);
	}

	g_clVision.DrawOverlayAll(m_nUnit);

	return true;
}


///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------
bool CAps_Insp::FnShmCornerFind(BYTE* ChartRawImage, bool bAutoMode)
{
	int i = 0;
	int j = 0;
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szFilePath[SIZE_OF_1K];


	TDATASPEC tspec;
	tspec.eOutMode = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eOutMode;
	tspec.eDataFormat = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDataFormat;
	tspec.eSensorType = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eSensorType;
	tspec.eDemosaicMethod = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDemosaicMethod;

	BYTE* m_pBMPTempBuffer;

	m_pBMPTempBuffer = new BYTE[g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize()];
	memset(m_pBMPTempBuffer, 0, g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize());

	ACMISSoftISP::xMakeBMP(ChartRawImage, (byte*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, tspec);

	Mat OrgImage(Size(g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight), CV_8UC3);
	std::memcpy(OrgImage.data, (char*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 3);

	//MbufGet(vision.MilGrabImageChild[3], OrgImage.data);//MbufGet(vision.MilGrabImageChild[1], OrgImage.data);
	if (bAutoMode == false)
	{
		imwrite("d:\\OrgImage.jpg", OrgImage);
	}
	if (m_pBMPTempBuffer)
	{
		delete m_pBMPTempBuffer;
	}
	m_pBMPTempBuffer = NULL;

	int thresh_value = 50;// 70;  //���� 12.28 = 70
	int max_value = 255;
	//
	int mRoiX = 0;
	int mRoiY = 0;

	int mRoiWidth = g_clModelData[m_nUnit].m_nWidth - mRoiX;
	int mRoiHeight = g_clModelData[m_nUnit].m_nHeight - mRoiY;

	int k = 0;
	int thresh = 245;
	for (k = 0; k < MAX_FOV_COUNT; k++)
	{
		mRoiX = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].x;
		mRoiY = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].y;

		mRoiWidth = (int)(g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeX[k]);
		mRoiHeight = (int)(g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeY[k]);

		Mat im = OrgImage(cvRect(mRoiX, mRoiY, mRoiWidth, mRoiHeight));
		//Mat im = OrgImage(cvRect(0, 0, g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight));
		//Mat GaussianImage = Mat::zeros(im.rows, im.cols, CV_8UC3);//CV_8UC1);
		//GaussianBlur(im, GaussianImage, Size(7, 7), 1);

		cv::Mat grayImage;
		cv::cvtColor(im, grayImage, cv::COLOR_BGR2GRAY);

		namedWindow("grayImage", WINDOW_NORMAL);
		imshow("grayImage", grayImage);
		waitKey(0);
		Mat harris = Mat::zeros(grayImage.size(), CV_32FC1);//CV_8UC1);

		Mat binaryImage = Mat::zeros(grayImage.rows, grayImage.cols, CV_8UC1);
		//threshold(GaussianImage, binaryImage, thresh_value, max_value, THRESH_BINARY);// THRESH_BINARY); THRESH_BINARY_INV
		threshold(grayImage, binaryImage, thresh_value, max_value, THRESH_BINARY);

		namedWindow("binaryImage", WINDOW_NORMAL);
		imshow("binaryImage", binaryImage);
		waitKey(0);
		//cornerHarris(grayimg, harris, 3, 3, 0.84);
		// Harris Corner
		// cornerHarris(�Է� �̹���(Grayscale), ��� �̹���, ���� �ȼ� ũ��(Blocksize), Sobel ksize, Harris parameter, �ȼ� ������)
		cornerHarris(binaryImage, harris, 2, 5, 0.05, BORDER_DEFAULT);	//2,5,0.05
		//harris = cv::dilate(harris, None);
		cv:dilate(harris, harris, cv::Mat(), cv::Point(-1, -1), 1);  
		//cv:erode(harris, harris, cv::Mat(), cv::Point(-1, -1), 1);


		//namedWindow("harris", WINDOW_NORMAL);
		//imshow("harris", harris);
		waitKey(0);
		Mat harris_norm;
		//normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);
		// ����ȭ(Normalizing)
		// normalize(�Է� �̹���, ��� �̹���, normalize range(low), normalize range(high), �ȼ� ������) 
		normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
		Mat cs_abs;
		//convertScaleAbs(harris_norm, cs_abs);
		// �̺��� ����� ���밪�� �����Ͽ� 8bit�� unsigned int������ �ٲپ� ǥ��
		convertScaleAbs(harris_norm, cs_abs);
		// Drawing a circle around corners

		for (int j = 0; j < harris_norm.rows; j += 1)
		{
			for (int i = 0; i < harris_norm.cols; i += 1)
			{
				if ((int)harris_norm.at<float>(j, i) >= thresh) 
				{
					circle(cs_abs, Point(i, j), 7, Scalar(255, 255, 255), 1, 8, 0);

					_stprintf_s(szLog, SIZE_OF_1K, _T("[Corner] x:%d, y:%d"), i + mRoiX, j + mRoiY);
					AddLog(szLog, 0, m_nUnit);
				}
			}
		}
		namedWindow("cs_absImage", WINDOW_NORMAL);
		imshow("cs_absImage", cs_abs);
		waitKey(0);

		break;
	}

	if (bAutoMode == false)
	{
		_stprintf_s(szFilePath, SIZE_OF_1K, _T("d:\\fovResult.jpg"));
		imwrite(szFilePath, OrgImage);
	}

	return false;
}
///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------
bool CAps_Insp::FnShmEdgeFind(BYTE* ChartRawImage, bool bAutoMode)
{
	int i = 0;
	int j = 0;
	TCHAR szLog[SIZE_OF_1K];
	TCHAR szFilePath[SIZE_OF_1K];


	TDATASPEC tspec;
	tspec.eOutMode = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eOutMode;
	tspec.eDataFormat = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDataFormat;
	tspec.eSensorType = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eSensorType;
	tspec.eDemosaicMethod = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n.eDemosaicMethod;

	BYTE* m_pBMPTempBuffer;

	m_pBMPTempBuffer = new BYTE[g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize()];
	memset(m_pBMPTempBuffer, 0, g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameBMPSize());

	ACMISSoftISP::xMakeBMP(ChartRawImage, (byte*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, tspec);

	Mat OrgImage(Size(g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight), CV_8UC3);
	std::memcpy(OrgImage.data, (char*)m_pBMPTempBuffer, g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 3);

	//MbufGet(vision.MilGrabImageChild[3], OrgImage.data);//MbufGet(vision.MilGrabImageChild[1], OrgImage.data);
	if (bAutoMode == false)
	{
		imwrite("d:\\OrgImage.jpg", OrgImage);
	}
	if (m_pBMPTempBuffer)
		delete m_pBMPTempBuffer;
	m_pBMPTempBuffer = NULL;

	//double centX = Task.d_mark_pos_x[PCB_Holder_MARK][1];
	//double centY = Task.d_mark_pos_y[PCB_Holder_MARK][1];
	int mRoiX = 0;
	int mRoiY = 0;

	int mRoiWidth = g_clModelData[m_nUnit].m_nWidth - mRoiX;
	int mRoiHeight = g_clModelData[m_nUnit].m_nHeight - mRoiY;

	int pimplescount = 0;
	int mParam1 = 10;	//���� threshold , 
	int mPrarm2 = 10;	//threshold ���� ���� ū ������ ��ȯ 
	int mMinRadius = 200;
	int mMaxRadius = 400;
	int StatinCount = 0;
	int nFindCount = 0;
	int thresh_value = 77;// 70;  //���� 12.28 = 70
	int max_value = 255;
	Point topStart(0, 0);
	Point topEnd(mRoiWidth, 0);
	Scalar white = Scalar(255);
	char strTmp[1024];
	double area = 0.0;
	int mcount = 0;
	int crosssize = 4;
	int cx = 0;
	int cy = 0;
	int minSize = 200;
	int maxSize = 650;
	int xLimit = 0;
	int yLimit = 0;
	int boundWidMax = 300;
	int linewidth = 1;


	size_t mcircleSize = 0;
	// ��ũ��ġ ���� ǥ��
	Scalar color(0, 0, 0); // �ʷϻ�
	cv::Point mPoint[10];
	bool bCenterCheck = false;
	int myFontFace = 0;
	double myFontScale = 1.1;
	int mthickness = 3;
	CvScalar mColor = Scalar(150, 200, 150);
	CvScalar mCircleColor;
	int colordiff = 2;


	

	int k = 0;
	for (k = 0; k < MAX_FOV_COUNT; k++)
	{
		mRoiX = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].x;
		mRoiY = g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].y;

		mRoiWidth = (int)(g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeX[k]);
		mRoiHeight = (int)( g_clModelData[m_nUnit].m_clSfrInfo.m_nFovSizeY[k]);

		Mat im = OrgImage(cvRect(mRoiX, mRoiY, mRoiWidth, mRoiHeight));
		Mat GaussianImage = Mat::zeros(im.rows, im.cols, CV_8UC3);//CV_8UC1);
		GaussianBlur(im, GaussianImage, Size(7, 7), 1);
		if (bAutoMode == false)
		{
			_stprintf_s(szFilePath, SIZE_OF_1K, _T("d:\\fovroi%d.jpg"), k+1);
			imwrite(szFilePath, GaussianImage);
		}
		// �׷��̽����Ϸ� ��ȯ
		cv::Mat grayImage;
		cv::cvtColor(GaussianImage, grayImage, cv::COLOR_BGR2GRAY);
		std::vector<cv::Vec3f> circles;
		Mat binaryImage = Mat::zeros(grayImage.rows, grayImage.cols, CV_8UC1);
		//threshold(GaussianImage, binaryImage, thresh_value, max_value, THRESH_BINARY);// THRESH_BINARY); THRESH_BINARY_INV
		threshold(grayImage, binaryImage, thresh_value, max_value, THRESH_BINARY);
		//thresh_value ������ ũ�� max_value ������ �ٲ۴�.

		if (bAutoMode == false)
		{
			imwrite("d:\\binaryImage.jpg", binaryImage);
		}



		std::vector<std::vector<Point>> contours;	//RETR_LIST, RETR_EXTERNAL
		vector<Point> approx;
		cv::findContours(binaryImage, contours, RETR_TREE, CHAIN_APPROX_NONE);		//RETR_CCOMP, RETR_LIST
		//CHAIN_APPROX_NONE , CHAIN_APPROX_SIMPLE

		for (size_t i = 0; i < contours.size(); i++)
		{
			if (!contours[i].empty())
			{
				area = contourArea(contours[i]);
				//if (area > 0.0 && area < 10000.0)//if (area > Ư��_����_�Ӱ谪) 
				mcircleSize = contours[i].size();
				Rect boundingRectData = boundingRect(contours[i]);

				Moments mmt = moments(contours[i]);
				double cx = mmt.m10 / mmt.m00 + g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].x;
				double cy = mmt.m01 / mmt.m00 + g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].y;



				contours[i].clear();
				boundingRectData.x = boundingRectData.x + g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].x;
				boundingRectData.y = boundingRectData.y + g_clModelData[m_nUnit].m_clSfrInfo.m_clPtFovOffset[k].y;
				
				rectangle(OrgImage, boundingRectData, Scalar(0, 0, 255), 3, 8, 0);

				int nIndex = 0;
				sprintf_s(strTmp, "[%d]", k+1);
				cv::putText(OrgImage, strTmp, Point(cx - 40, cy + 15), myFontFace, myFontScale, mColor, mthickness);

				
			}

		}
	}



	
//#if (____MACHINE_NAME == MODEL_FRONT_100)
//	minSize = 50;
//	maxSize = 1000;
//	xLimit = 0;
//	yLimit = 0;
//	boundWidMax = 300;
//
//	//top
//	topStart.x = 0;
//	topStart.y = 0;
//	topEnd.x = mRoiWidth;
//	topEnd.y = 0;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//	//bottom
//	topStart.x = 0;
//	topStart.y = mRoiHeight - linewidth;
//	topEnd.x = mRoiWidth;
//	topEnd.y = mRoiHeight - linewidth;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//	//left
//	topStart.x = 0;
//	topStart.y = 0;
//	topEnd.x = 0;
//	topEnd.y = mRoiHeight;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//	//right
//	topStart.x = mRoiWidth - 1;
//	topStart.y = 0;
//	topEnd.x = mRoiWidth - 1;
//	topEnd.y = mRoiHeight;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//#else
//	minSize = 200;
//	maxSize = 650;
//	xLimit = 0;
//	yLimit = 0;
//	boundWidMax = 300;
//
//
//	//LT
//	topStart.x = 200;
//	topStart.y = 200;//190;
//	topEnd.x = 400;
//	topEnd.y = 190;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//
//	//RT
//	topStart.x = 1550;
//	topStart.y = 200;
//	topEnd.x = 1750;
//	topEnd.y = 200;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//	//BL
//	topStart.x = 200;
//	topStart.y = 880;//884;
//	topEnd.x = 400;
//	topEnd.y = 880;//884;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//	//BR
//	topStart.x = 1550;
//	topStart.y = 890;
//	topEnd.x = 1750;
//	topEnd.y = 890;
//	line(binaryImage, topStart, topEnd, white, linewidth);
//#endif


	
#if 1
	//size_t mcircleSize = 0;
	//// ��ũ��ġ ���� ǥ��
	//Scalar color(0, 0, 0); // �ʷϻ�
	//cv::Point mPoint[10];
	//bool bCenterCheck = false;
	//int myFontFace = 0;
	//double myFontScale = 1.1;
	//int mthickness = 3;
	//CvScalar mColor = Scalar(150, 200, 150);
	//CvScalar mCircleColor;
	//int colordiff = 2;

	//

	//std::vector<std::vector<Point>> contours;	//RETR_LIST, RETR_EXTERNAL
	//vector<Point> approx;
	//cv::findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_NONE);

	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	if (!contours[i].empty())
	//	{
	//		area = contourArea(contours[i]);
	//		//if (area > 0.0 && area < 10000.0)//if (area > Ư��_����_�Ӱ谪) 
	//		mcircleSize = contours[i].size();
	//		Rect boundingRectData = boundingRect(contours[i]);

	//		contours[i].clear();
	//	}

	//}
	return true;

#else
	//
	//
	//
	//
	//
	//
	// ���� ã��
	std::vector<std::vector<Point>> contours;	//RETR_LIST, RETR_EXTERNAL
	vector<Point> approx;
	//findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_NONE);//CHAIN_APPROX_SIMPLE, CHAIN_APPROX_NONE
	findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_NONE);// CHAIN_APPROX_NONE); 


	size_t mcircleSize = 0;
	// ��ũ��ġ ���� ǥ��
	Scalar color(0, 0, 0); // �ʷϻ�
	cv::Point mPoint[10];
	bool bCenterCheck = false;
	int myFontFace = 0;
	double myFontScale = 1.1;
	int mthickness = 3;
	CvScalar mColor = Scalar(150, 200, 150);
	CvScalar mCircleColor;
	int colordiff = 2;
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (!contours[i].empty())
		{
			area = contourArea(contours[i]);
			//if (area > 0.0 && area < 10000.0)//if (area > Ư��_����_�Ӱ谪) 
			mcircleSize = contours[i].size();
			Rect boundingRectData = boundingRect(contours[i]);

			if (mcircleSize > minSize && mcircleSize < maxSize && boundingRectData.width < boundWidMax)//300)//mcircleSize > minSize && mcircleSize < maxSize)
			{

				Moments mmt = moments(contours[i]);
				double cx = mmt.m10 / mmt.m00;
				double cy = mmt.m01 / mmt.m00;


				int nIndex = 0;// sfrBoundCheck(cx, cy);
				//nIndex > 0  && 
				if (nIndex > 0 && cy > yLimit && cx > xLimit && cx < g_clModelData[m_nUnit].m_nWidth - xLimit&& cy < g_clModelData[m_nUnit].m_nHeight - yLimit)		//���� ���� ������ 100 �߰�
				{
					//drawContours(OrgImage, contours, static_cast<int>(i), color, 3);

					rectangle(OrgImage, boundingRectData, Scalar(0, 0, 255), 3, 8, 0);
					//line(OrgImage, Point(cx - crosssize, cy - crosssize), Point(cx + crosssize, cy + crosssize), Scalar::all(255), 2, 8, 0);
					//line(OrgImage, Point(cx + crosssize, cy - crosssize), Point(cx - crosssize, cy + crosssize), Scalar::all(255), 2, 8, 0);
					sprintf_s(strTmp, "[%d]", nIndex);
					putText(OrgImage, strTmp, Point(cx - 40, cy + 15), myFontFace, myFontScale, mColor, mthickness);

					_stprintf_s(szLog, SIZE_OF_1K, _T("[Box #%d]Center:%.3lf, %.3lf / Size:%zu"), nIndex, cx, cy, mcircleSize);
					AddLog(szLog, 0, m_nUnit);
					mcount++;


					approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
					//sfrPosCheck(approx);

					//std::sort(approx.begin(), approx.end(), [](const Point& a, const Point& b) {
					//return a.x < b.x;
					//});

					std::sort(approx.begin(), approx.end(), [](const Point& a, const Point& b)
					{
						if (a.x != b.x) {
							return a.x < b.x;  // y ��ǥ�� �������� ����
						}
						return a.y < b.y;  // y ��ǥ�� ������ ��� x ��ǥ�� �������� ����
					});

					for (j = 0; j < approx.size(); ++j)
					{
						mPoint[j].x = approx.at(j).x;
						mPoint[j].y = approx.at(j).y;
					}
					//int Centerx = approx.at(0).x + (approx.at(3).x - approx.at(0).x);
					cv::Point tempPoint;
					if (approx.size() > 3)
					{
						if (mPoint[0].y > mPoint[1].y)
						{
							tempPoint = mPoint[0];
							mPoint[0] = mPoint[1];
							mPoint[1] = tempPoint;
						}
						if (mPoint[2].y > mPoint[3].y)
						{
							tempPoint = mPoint[2];
							mPoint[2] = mPoint[3];
							mPoint[3] = tempPoint;
						}
					}



				}





			}
			contours[i].clear();
		}

	}
	nFindCount = mcount;
	_stprintf_s(szLog, SIZE_OF_1K, _T("[Circle]Find Count : %d"), nFindCount);// contours[i][0].x, contours[i][0].y);
	AddLog(szLog, 0, m_nUnit);

	return false;
#endif
}

///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------



///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------


///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
///-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//	Lens Shading (Relative Illumination X) 3000k , 5000k
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_LensShading(BYTE* pBuffer,int index,  bool bAutoMode)	//R/C,B/C,Cr/Cb
{

	return true;
}

//-----------------------------------------------------------------------------
//
//	White Balance (Color Sensitivity) 3000 , 5000K , Dark ���� �˻�
//
//-----------------------------------------------------------------------------
//bool CPRIFunc_Insp::func_Insp_ColorSensitivity(BYTE* _____img, int index, bool bAutoMode)	//R/C,B/C,Cr/Cb
bool CAps_Insp::func_Insp_WhiteBalance(BYTE* _____img, int index, bool bAutoMode)	//R/C,B/C,Cr/Cb
{
	
	return false;
}


//-----------------------------------------------------------------------------
//
//	Relative Illumination (RI) �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Shm_Illumination(BYTE* rawImage, bool bAutoMode)
{
	//TRelativeIlluminationSpecX& _Spec,
	bool bRes = true;
	int i = 0;
	int nResult = R_RESULT_PASS;
	char strTmp[1024];
	TCHAR szLog[SIZE_OF_1K];

	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;

	bool bUse8BitOnly = false;
	TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dRi_Tdataspec;

	if (g_clModelData[m_nUnit].m_nRi8BitUse == 1)
	{
		bUse8BitOnly = true;
	}
	TRelativeIlluminationSpecN tRelativeIlluminationSpecN;
	// Image buffers
	std::vector<RECT> vROI;
	std::vector<double> vOffset;

	memset(&tRelativeIlluminationSpecN, 0x00, sizeof(tRelativeIlluminationSpecN));

	//TROIData tROI;
	//tRelativeIlluminationSpecN.dSpecRIcornerMin = g_clModelData[m_nUnit].m_RISpec[0];// 60.0;		//g_clModelData[m_nUnit].m_RISpec[i]
	//tRelativeIlluminationSpecN.dSpecRIcornerMax = g_clModelData[m_nUnit].m_RISpec[1];//100.0;
	//tRelativeIlluminationSpecN.dSpecRIminMin = g_clModelData[m_nUnit].m_RISpec[2];//5;
	//tRelativeIlluminationSpecN.dSpecRIminMax = g_clModelData[m_nUnit].m_RISpec[3];//100;
	//tRelativeIlluminationSpecN.dCenterIntensity = g_clModelData[m_nUnit].m_RISpec[4];//2000;
	//tRelativeIlluminationSpecN.nSpecRINormalizeIndex = g_clModelData[m_nUnit].m_RISpec[5];//0;


	tRelativeIlluminationSpecN.dSpecRIcornerMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[26]);
	tRelativeIlluminationSpecN.dSpecRIcornerMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[27]);
	tRelativeIlluminationSpecN.dSpecRIminMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[28]);
	tRelativeIlluminationSpecN.dSpecRIminMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[29]);
	tRelativeIlluminationSpecN.dCenterIntensity = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[30]);
	tRelativeIlluminationSpecN.nSpecRINormalizeIndex = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[31]);


	tRelativeIlluminationSpecN.tROI.ROICount = 5;
	vROI.resize(tRelativeIlluminationSpecN.tROI.ROICount);
	tRelativeIlluminationSpecN.tROI.pROIData = vROI.data();
	tRelativeIlluminationSpecN.tROI.eROIType = ROIType_RECT;
	tRelativeIlluminationSpecN.tROI.dOffset = vOffset.data();

	g_clVision.ClearOverlay(m_nUnit);
	RECT rtRefEdge;
	int roiSize = 50;
	//double nfield = 0.85;
	//int fieldGapW = (nWidth * nfield);
	//int fieldGapH = (nHeight * nfield);

	vROI[0].left = 935;		// nWidth / 2 - (roiSize / 2);
	vROI[0].top = 515;		// nHeight / 2 - (roiSize / 2);
	vROI[0].right = vROI[0].left + roiSize;
	vROI[0].bottom = vROI[0].top + roiSize;

	sprintf_s(strTmp, "#1");
	g_clVision.DrawMOverlayText(m_nUnit, vROI[0].left+10, vROI[0].top-50, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);

	//LT
	vROI[1].left = 262;// 118;	// nWidth - fieldGapW;
	vROI[1].top = 136;//55;	// nHeight - fieldGapH;
	vROI[1].right = vROI[1].left + roiSize;
	vROI[1].bottom = vROI[1].top + roiSize;

	sprintf_s(strTmp, "#2");
	g_clVision.DrawMOverlayText(m_nUnit, vROI[1].left+12, vROI[1].bottom + 10, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);

	//RT
	vROI[2].left = 1607;//1751;	// fieldGapW;
	vROI[2].top = 136;//55;		// nHeight - fieldGapH;
	vROI[2].right = vROI[2].left + roiSize;
	vROI[2].bottom = vROI[2].top + roiSize;

	sprintf_s(strTmp, "#3");
	g_clVision.DrawMOverlayText(m_nUnit, vROI[2].left+12, vROI[2].bottom + 10, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);

	//BL
	vROI[3].left = 262;//118;	// nWidth - fieldGapW;
	vROI[3].top = 893;//974;	// fieldGapH;
	vROI[3].right = vROI[3].left + roiSize;
	vROI[3].bottom = vROI[3].top + roiSize;

	sprintf_s(strTmp, "#4");
	g_clVision.DrawMOverlayText(m_nUnit, vROI[3].left+5, vROI[3].top - 50, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	//BR
	vROI[4].left = 1607;//1751;	// fieldGapW;
	vROI[4].top = 893;//974;	// fieldGapH;
	vROI[4].right = vROI[4].left + roiSize;
	vROI[4].bottom = vROI[4].top + roiSize;

	sprintf_s(strTmp, "#5");
	g_clVision.DrawMOverlayText(m_nUnit, vROI[4].left+5, vROI[4].top - 50, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);

	
	for (i = 0; i < 5; i++)
	{
		rtRefEdge.left = vROI[i].left;
		rtRefEdge.top = vROI[i].top;
		rtRefEdge.right = vROI[i].right;
		rtRefEdge.bottom = vROI[i].bottom;

		g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rtRefEdge.left, rtRefEdge.top, rtRefEdge.right, rtRefEdge.bottom, M_COLOR_BLUE, 1, FALSE);
		//vision.boxlist[CCD].addList(rtRefEdge, PS_SOLID, M_COLOR_BLUE);
	}
	bool bRaw = true;
	bool bEnableRelativeIllumination = true;
	bool bEnableRelativeIlluminationX = true;
	bool bEnableRelativeUniformity = true;
	bool bEnableColorSensitivity = true;
	bool bSaveResultImage = false;
	//----------------------------------------------------------------------
	// Inspect Main Camera
	//----------------------------------------------------------------------

	std::shared_ptr<CACMISShadingRelativeIllumination> m_pRelativeIllumination = std::make_shared<CACMISShadingRelativeIllumination>();

	//Inspection
	if (!m_pRelativeIllumination->Inspect(rawImage, nWidth, nHeight, tRelativeIlluminationSpecN, tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, 
		bUse8BitOnly, false, tDataSpec.eDemosaicMethod))
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("15"));
		g_clMesCommunication[m_nUnit].m_nMesRICorner[0] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesRICorner[1] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesRICorner[2] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesRICorner[3] = 0.0;

		g_clMesCommunication[m_nUnit].m_dMesRiDiff = 0.0;
		g_clMesCommunication[m_nUnit].m_dMesRiCenterRaw = 0.0;

		g_clMesCommunication[m_nUnit].m_dMesRiMinDiff = 0.0;

		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;
		g_clMesCommunication[m_nUnit].m_nMesRICornerResult[0] = 0;
		g_clMesCommunication[m_nUnit].m_nMesRICornerResult[1] = 0;
		g_clMesCommunication[m_nUnit].m_nMesRICornerResult[2] = 0;
		g_clMesCommunication[m_nUnit].m_nMesRICornerResult[3] = 0;

		g_clMesCommunication[m_nUnit].m_nMesRIMinDiffResult = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] Test Fail"));
		AddLog(szLog, 0, m_nUnit);

		return false;
	}

	TRelativeIlluminationResultN m_stRelativeIlluminationResult;
	m_stRelativeIlluminationResult = *m_pRelativeIllumination->GetInspectionResult(0);  //dRicorner

	g_clMesCommunication[m_nUnit].m_nMesRICorner[0] = m_stRelativeIlluminationResult.dRIcornerRatio[1];
	g_clMesCommunication[m_nUnit].m_nMesRICorner[1] = m_stRelativeIlluminationResult.dRIcornerRatio[2];
	g_clMesCommunication[m_nUnit].m_nMesRICorner[2] = m_stRelativeIlluminationResult.dRIcornerRatio[3];
	g_clMesCommunication[m_nUnit].m_nMesRICorner[3] = m_stRelativeIlluminationResult.dRIcornerRatio[4];

	g_clMesCommunication[m_nUnit].m_dMesRiDiff = m_pRelativeIllumination->GetMaxDiff();
	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] MaxDiff : %.6lf"), g_clMesCommunication[m_nUnit].m_dMesRiDiff);
	AddLog(szLog, 0, m_nUnit);


	g_clMesCommunication[m_nUnit].m_dMesRiCenterRaw = m_stRelativeIlluminationResult.dRIcenterRAW;
	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RIcenterRAW : %.6lf"), g_clMesCommunication[m_nUnit].m_dMesRiCenterRaw);
	AddLog(szLog, 0, m_nUnit);



	double dMinTemp = g_clMesCommunication[m_nUnit].m_nMesRICorner[0];
	for (i = 0; i < 4; i++)
	{
		if (dMinTemp > g_clMesCommunication[m_nUnit].m_nMesRICorner[i])
		{
			dMinTemp = g_clMesCommunication[m_nUnit].m_nMesRICorner[i];
		}
	}

	g_clMesCommunication[m_nUnit].m_dMesRiMinDiff = dMinTemp;

	double mRi_Value = 0.0;

	double mRi_SpecMin = 0.0;
	double mRi_SpecMax = 0.0;



	//mRi_Value = g_clMesCommunication[m_nUnit].m_dMesRiCenterRaw;
	//mRi_SpecMin = (_ttof(EEpromVerifyData.vMinData[34]));
	//mRi_SpecMax = (_ttof(EEpromVerifyData.vMaxData[34]));


	//if (mRi_Value < mRi_SpecMin || mRi_Value > mRi_SpecMax)
	//{
	//	g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;
	//	g_clMesCommunication[m_nUnit].m_nMesRICenterRawResult = 0;

	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RIcenterRAW Spec Out: %.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//	AddLog(szLog, 0, m_nUnit);
	//	g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [RIcenterRAW NG]");
	//	if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
	//	{
	//		g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG RIcenterRAW :%.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//		g_clMandoInspLog[m_nUnit].m_nNGCnt++;
	//	}
	//	//g_clMesCommunication[m_nUnit].m_nMesRIRICornerResult[0] = 0;

	//	g_clMandoInspLog[m_nUnit].m_bInspRes = false;
	//}
	//else
	//{
	//	g_clMesCommunication[m_nUnit].m_nMesRICenterRawResult = 1;
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RIcenterRAW Spec In: %.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//	AddLog(szLog, 0, m_nUnit);
	//}
	
	for (i = 0; i < 4; i++)
	{
		mRi_Value = g_clMesCommunication[m_nUnit].m_nMesRICorner[i];
		//mRi_SpecMin = (_ttof(EEpromVerifyData.vMinData[34 + i]));
		//mRi_SpecMax = (_ttof(EEpromVerifyData.vMaxData[34 + i]));
		mRi_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[8 + (2 * i)]);
		mRi_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[8 + (2 * i + 1)]);



		if (mRi_Value < mRi_SpecMin || mRi_Value > mRi_SpecMax)
		{
			g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("15"));
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;
			g_clMesCommunication[m_nUnit].m_nMesRICornerResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RIcorner %d Spec Out: %.6lf [%.2lf~%.2lf]"),i,  mRi_Value, mRi_SpecMin, mRi_SpecMax);
			AddLog(szLog, 0, m_nUnit);
			g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [RIcorner NG]");
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG RIcorner %d :%.6lf [%.2lf~%.2lf]"), i, mRi_Value, mRi_SpecMin, mRi_SpecMax);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}
			//g_clMesCommunication[m_nUnit].m_nMesRIRICornerResult[0] = 0;

			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			g_clTaskWork[m_nUnit].m_bOutputCheck[2] = false;	//OC

		}
		else
		{
			g_clMesCommunication[m_nUnit].m_nMesRICornerResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RIcorner %d Spec In: %.6lf [%.2lf~%.2lf]"), i, mRi_Value, mRi_SpecMin, mRi_SpecMax);
			AddLog(szLog, 0, m_nUnit);
		}
	}


	

	//mRi_Value = g_clMesCommunication[m_nUnit].m_dMesRiDiff;
	//mRi_SpecMin = (_ttof(EEpromVerifyData.vMinData[36]));
	//mRi_SpecMax = (_ttof(EEpromVerifyData.vMaxData[36]));


	//if (mRi_Value < mRi_SpecMin || mRi_Value > mRi_SpecMax)
	//{
	//	g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;//XX
	//	g_clMesCommunication[m_nUnit].m_nMesRIDiffResult = 0;

	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] MaxDiff Spec Out: %.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//	AddLog(szLog, 0, m_nUnit);
	//	g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [MaxDiff NG]");
	//	if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
	//	{
	//		g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG MaxDiff :%.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//		g_clMandoInspLog[m_nUnit].m_nNGCnt++;
	//	}
	//	//g_clMesCommunication[m_nUnit].m_nMesRIRICornerResult[0] = 0;

	//	g_clMandoInspLog[m_nUnit].m_bInspRes = false;
	//	g_clTaskWork[m_nUnit].m_bOutputCheck[2] = false;	//OC

	//}
	//else
	//{
	//	g_clMesCommunication[m_nUnit].m_nMesRIDiffResult = 1;
	//	_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] MaxDiff Spec In: %.6lf [%.2lf~%.2lf]"), mRi_Value, mRi_SpecMin, mRi_SpecMax);
	//	AddLog(szLog, 0, m_nUnit);
	//}


	mRi_Value = g_clMesCommunication[m_nUnit].m_dMesRiMinDiff;
	//mRi_SpecMin = (_ttof(EEpromVerifyData.vMinData[38]));
	//mRi_SpecMax = (_ttof(EEpromVerifyData.vMaxData[38]));

	mRi_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[61]);

	if (mRi_Value < mRi_SpecMin)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("15"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;
		g_clMesCommunication[m_nUnit].m_nMesRIMinDiffResult = 0;

		_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RICorner Min Spec Out: %.6lf [%.2lf ~]"), mRi_Value, mRi_SpecMin);
		AddLog(szLog, 0, m_nUnit);
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [RICorner Min NG]");
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG RICorner Min :%.6lf [%.2lf]"), mRi_Value, mRi_SpecMin);
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		} 
		//g_clMesCommunication[m_nUnit].m_nMesRIRICornerResult[0] = 0; 

		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nMesRIMinDiffResult = 1;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[RI] RICorner Min Spec In: %.6lf [%.2lf ~ ]"), mRi_Value, mRi_SpecMin);
		AddLog(szLog, 0, m_nUnit);
	}

	
	g_clVision.DrawOverlayAll(m_nUnit);
	//if (MandoInspLog.dRiDiff < MandoSfrSpec.INSP_RIDiff_Spec)
	//{
	//	sTemp.Format("RI MaxDiff Spec NG: %.6lf (Spec:%.6lf)", MandoInspLog.dRiDiff, MandoSfrSpec.INSP_RIDiff_Spec);
	//	//! Main Displayȭ�� Overlay NG List
	//	MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("RI MaxDiff fail: %.6lf (Spec:%.6lf)", MandoInspLog.dRiDiff, MandoSfrSpec.INSP_RIDiff_Spec);
	//	MandoInspLog.iNGCnt++;
	//	MandoInspLog.bInspRes = false;//LCB
	//								  //bResult = false;
	//	theApp.MainDlg->putListLog(sTemp);
	//	MandoInspLog.sNGList += sTemp;
	//}
	//else
	//{
	//	sTemp.Format("RI MaxDiff Spec IN: %.6lf (Spec:%.6lf)", MandoInspLog.dRiDiff, MandoSfrSpec.INSP_RIDiff_Spec);
	//	theApp.MainDlg->putListLog(sTemp);
	//}


	g_SaveLGITLog(m_nUnit, "RI", m_pRelativeIllumination->GetLogHeader(), m_pRelativeIllumination->GetLogData(), m_pRelativeIllumination->GetVersion());
	return true;
}

bool CAps_Insp::func_Insp_ColorShading(BYTE* pBuffer, int index, bool bAutoMode)
{
	
	return false;
}


//-----------------------------------------------------------------------------
//
//	IR Filter IR ���� ��� �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_IRFilterTest(const BYTE* pBuffer, bool bAutoMode)
{
	
	return false;
}

//-----------------------------------------------------------------------------
//
//	Color Reproduction �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Color_reproduction(const BYTE* pBuffer, bool bAutoMode)
{
#if 0
	TCHAR szLog[SIZE_OF_1K];
	int i = 0;
	int nResult = R_RESULT_PASS;
	const DPOINT* dResultPos;
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dColor_Tdataspec;
	bool bUse8BitOnly = false;

	if (g_clModelData[m_nUnit].m_nColor8BitUse == 1)
	{
		bUse8BitOnly = true;
	}

	g_clVision.ClearOverlay(m_nUnit);
	CACMISColorReproduction *m_pColorReproduction = new CACMISColorReproduction();

	std::vector<RECT> vROI;
	std::vector<double> vOffset;
	std::vector<double> vThreshold;
	std::vector<TColorLab> vColorLab;

	//TColorReproductionSpecN m_stColorReproductionSpec;
	TColorReproductionSpecN m_stColorReproductionSpec;
	m_stColorReproductionSpec.tROI.ROICount = 4;
	vROI.resize(m_stColorReproductionSpec.tROI.ROICount);
	vOffset.resize(m_stColorReproductionSpec.tROI.ROICount);
	vThreshold.resize(m_stColorReproductionSpec.tROI.ROICount);
	vColorLab.resize(m_stColorReproductionSpec.tROI.ROICount);

	

	//vThreshold[0] = g_clModelData[m_nUnit].m_ColorReproductionSpec[0];//20.0;
	//vThreshold[1] = g_clModelData[m_nUnit].m_ColorReproductionSpec[1];//20.0;
	//vThreshold[2] = g_clModelData[m_nUnit].m_ColorReproductionSpec[2];//20.0;
	//vThreshold[3] = g_clModelData[m_nUnit].m_ColorReproductionSpec[3];//20.0;

	//vColorLab[0].L = g_clModelData[m_nUnit].m_ColorReproductionSpec[4];//32.404;
	//vColorLab[0].a = g_clModelData[m_nUnit].m_ColorReproductionSpec[5];//44.222;
	//vColorLab[0].b = g_clModelData[m_nUnit].m_ColorReproductionSpec[6];//37.356;

	//vColorLab[1].L = g_clModelData[m_nUnit].m_ColorReproductionSpec[7];//11.326;
	//vColorLab[1].a = g_clModelData[m_nUnit].m_ColorReproductionSpec[8];//33.245;
	//vColorLab[1].b = g_clModelData[m_nUnit].m_ColorReproductionSpec[9];//-51.351;

	//vColorLab[2].L = g_clModelData[m_nUnit].m_ColorReproductionSpec[10];//2.151;
	//vColorLab[2].a = g_clModelData[m_nUnit].m_ColorReproductionSpec[11];//0.0;
	//vColorLab[2].b = g_clModelData[m_nUnit].m_ColorReproductionSpec[12];//-0.001;

	//vColorLab[3].L = g_clModelData[m_nUnit].m_ColorReproductionSpec[13];//26.191;
	//vColorLab[3].a = g_clModelData[m_nUnit].m_ColorReproductionSpec[14];//-25.316;
	//vColorLab[3].b = g_clModelData[m_nUnit].m_ColorReproductionSpec[15];//28.054;
	//
	//
	//
	for (i = 0; i < 4; i++)
	{
		vThreshold[i] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[71 + i]);

		vColorLab[i].L = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[75 + (3 * i)]);
		vColorLab[i].a = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[76 + (3 * i)]);
		vColorLab[i].b = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[77 + (3 * i)]);

	}


	int offsetX = ((g_clModelData[m_nUnit].m_nWidth / 2) - (g_clTaskWork[m_nUnit].m_clPtCircle[0].x + g_clTaskWork[m_nUnit].m_clPtCircle[1].x + g_clTaskWork[m_nUnit].m_clPtCircle[2].x + g_clTaskWork[m_nUnit].m_clPtCircle[3].x) / 4) * -1;
	int offsetY = ((g_clModelData[m_nUnit].m_nHeight / 2) - (g_clTaskWork[m_nUnit].m_clPtCircle[0].y + g_clTaskWork[m_nUnit].m_clPtCircle[1].y + g_clTaskWork[m_nUnit].m_clPtCircle[2].y + g_clTaskWork[m_nUnit].m_clPtCircle[3].y) / 4) * -1;

	//vROI[0].left = g_clModelData[m_nUnit].m_ColorReproductionSpec[16] + offsetX;
	//vROI[0].top = g_clModelData[m_nUnit].m_ColorReproductionSpec[17] + offsetY;
	//vROI[0].right = g_clModelData[m_nUnit].m_ColorReproductionSpec[18] + offsetX;
	//vROI[0].bottom = g_clModelData[m_nUnit].m_ColorReproductionSpec[19] + offsetY;
	////
	//vROI[1].left = g_clModelData[m_nUnit].m_ColorReproductionSpec[20] + offsetX;
	//vROI[1].top = g_clModelData[m_nUnit].m_ColorReproductionSpec[21] + offsetY;
	//vROI[1].right = g_clModelData[m_nUnit].m_ColorReproductionSpec[22] + offsetX;
	//vROI[1].bottom = g_clModelData[m_nUnit].m_ColorReproductionSpec[23] + offsetY;
	////
	//vROI[2].left = g_clModelData[m_nUnit].m_ColorReproductionSpec[24] + offsetX;
	//vROI[2].top = g_clModelData[m_nUnit].m_ColorReproductionSpec[25] + offsetY;
	//vROI[2].right = g_clModelData[m_nUnit].m_ColorReproductionSpec[26] + offsetX;
	//vROI[2].bottom = g_clModelData[m_nUnit].m_ColorReproductionSpec[27] + offsetY;
	////
	//vROI[3].left = g_clModelData[m_nUnit].m_ColorReproductionSpec[28] + offsetX;
	//vROI[3].top = g_clModelData[m_nUnit].m_ColorReproductionSpec[29] + offsetY;
	//vROI[3].right = g_clModelData[m_nUnit].m_ColorReproductionSpec[30] + offsetX;
	//vROI[3].bottom = g_clModelData[m_nUnit].m_ColorReproductionSpec[31] + offsetY;


	for (i = 0; i < 4; i++)
	{
		vROI[i].left = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[87 + (4 * i)]);
		vROI[i].top = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[88 + (4 * i)]);
		vROI[i].right = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[89 + (4 * i)]);
		vROI[i].bottom = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[90 + (4 * i)]);

	}



	m_stColorReproductionSpec.tROI.pROIData = vROI.data();
	m_stColorReproductionSpec.tROI.dOffset = vOffset.data();
	m_stColorReproductionSpec.tROI.eROIType = ROIType_RECT;
	m_stColorReproductionSpec.pdThresholdDelta = vThreshold.data();

	m_stColorReproductionSpec.ptRefColorLab = vColorLab.data();
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[ColorReproduction] Roi #%d / Left:%d,Top:%d,Right:%d,Bottom:%d"), i + 1, vROI[i].left, vROI[i].top, vROI[i].right, vROI[i].bottom);
		AddLog(szLog, 0, m_nUnit);
	}
	
	char strTmp[1024];
	
	int dTextX = 0;
	int dTextY = 0;

	dTextX = vROI[0].left;
	dTextY = vROI[0].top - 60;
	if (dTextY < 0)
	{
		dTextY = 0;
	}
	sprintf_s(strTmp, "#1");
	g_clVision.DrawMOverlayText(m_nUnit, dTextX, dTextY, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	//g_clVision.DrawMOverlayText(m_nUnit, vROI[4].left, vROI[4].top - 50, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	dTextX = vROI[1].left;
	dTextY = vROI[1].top - 60;
	if (dTextY < 0)
	{
		dTextY = 0;
	}
	sprintf_s(strTmp, "#2");
	g_clVision.DrawMOverlayText(m_nUnit, dTextX, dTextY, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	dTextX = vROI[2].left;
	dTextY = vROI[2].top - 60;
	if (dTextY < 0)
	{
		dTextY = 0;
	}
	sprintf_s(strTmp, "#3");
	g_clVision.DrawMOverlayText(m_nUnit, dTextX, dTextY, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	dTextX = vROI[3].left;
	dTextY = vROI[3].top - 60;
	if (dTextY < 0)
	{
		dTextY = 0;
	}
	sprintf_s(strTmp, "#4");
	g_clVision.DrawMOverlayText(m_nUnit, dTextX, dTextY, strTmp, M_COLOR_CYAN, _T("Arial"), 8, 0);
	//
	g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, vROI[0].left, vROI[0].top, vROI[0].right, vROI[0].bottom, M_COLOR_BLUE, 1, FALSE);
	g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, vROI[1].left, vROI[1].top, vROI[1].right, vROI[1].bottom, M_COLOR_BLUE, 1, FALSE);
	g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, vROI[2].left, vROI[2].top, vROI[2].right, vROI[2].bottom, M_COLOR_BLUE, 1, FALSE);
	g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, vROI[3].left, vROI[3].top, vROI[3].right, vROI[3].bottom, M_COLOR_BLUE, 1, FALSE);

	//ROI VIEW
	vOffset[0] = 0.0;
	vOffset[1] = 0.0;
	vOffset[2] = 0.0;
	vOffset[3] = 0.0;

	for (int i = 0; i < 4; i++)
	{
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[i] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[i] = 1;
	}
	double mColorValue = 0.0;
	double mColorSpec = 0.0;
	int mIndex = 0;

	bool result = m_pColorReproduction->Inspect(pBuffer, nWidth, nHeight, m_stColorReproductionSpec, 
		tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly, 
		false, tDataSpec.eDemosaicMethod);
	if (result == false)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("32"));
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[0] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[1] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[2] = 0.0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[3] = 0.0;

		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;

		g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[0] = 0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[1] = 0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[2] = 0;
		g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[3] = 0;


		_stprintf_s(szLog, SIZE_OF_1K, _T("[ColorReproduction] Test Fail"));

		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [ColorReproduction Test NG]");
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG ColorReproduction Test Fail"));
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}

		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		AddLog(szLog, 0, m_nUnit);
		return false;
	}

	for (int i = 0; i < m_pColorReproduction->GetInspectionRegionCount(); i++)
	{
		const TColorReproductionResult* pResult = m_pColorReproduction->GetInspectionResult(i); // ��� ��
		//printf(��[ColorReproduction]Region = %d, ROI(% 4d, % 4d, % 4d % 4d), Delta = %.3f, L = %.3f, a = %.3f, b = %.3f\n��,
		//	pResult->nIndex, pResult->rtROI.left, pResult->rtROI.top, pResult->rtROI.right, pResult->rtROI.bottom, pResult ->dDelta, pResult->tColorLab.L, pResult->tColorLab.a, pResult->tColorLab.b);

		g_clMandoInspLog[m_nUnit].m_dColorReproductionResult[i] = pResult->dDelta;
		g_clMesCommunication[m_nUnit].m_nMesColorReproduction[i] = pResult->dDelta;

		_stprintf_s(szLog, SIZE_OF_1K, _T("	[ColorReproduction] Region = %d, ROI(%4d, %4d, %4d %4d), Delta = %.3f, L = %.3f, a = %.3f, b = %.3f"),
			pResult->nIndex, pResult->rtROI.left, pResult->rtROI.top, pResult->rtROI.right, pResult->rtROI.bottom, pResult->dDelta, pResult->tColorLab.L, pResult->tColorLab.a, pResult->tColorLab.b);
		AddLog(szLog, 0, m_nUnit);
	}

	double mColor_Value = 0.0;
	double mColor_SpecMin = 0.0;
	double mColor_SpecMax = 0.0;


	for (i = 0; i < 4; i++)
	{
		mColor_Value = g_clMesCommunication[m_nUnit].m_nMesColorReproduction[i];
		//mColor_SpecMin = (_ttof(EEpromVerifyData.vMinData[20 + i]));
		//mColor_SpecMax = (_ttof(EEpromVerifyData.vMaxData[20 + i]));
		mColor_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[29 + i]);



		if (mColor_Value > mColor_SpecMax)
		{
			g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("32"));
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//ColorReproduction
			g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ColorReproduction] Region = %d,  Delta Spec Out: %.6lf [ ~ %.3lf]"), i+1, mColor_Value, mColor_SpecMax);
			AddLog(szLog, 0, m_nUnit);
			g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [ColorReproduction NG]");
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG ColorReproduction %d :%.6lf [ ~ %.3lf]"), i + 1, mColor_Value, mColor_SpecMax);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}

			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			g_clTaskWork[m_nUnit].m_bOutputCheck[2] = false;

		}
		else
		{
			g_clMesCommunication[m_nUnit].m_nMesColorReproductionResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("[ColorReproduction] Region = %d,  Delta Spec In: %.6lf [ ~ %.3lf]"), i + 1, mColor_Value, mColor_SpecMax);
			AddLog(szLog, 0, m_nUnit);
		}
	}
	
	g_clVision.DrawOverlayAll(m_nUnit);
	

	g_SaveLGITLog(m_nUnit, "Color Reproduction", m_pColorReproduction->GetLogHeader(), m_pColorReproduction->GetLogData(), m_pColorReproduction->GetVersion());

	delete m_pColorReproduction;

#endif
	return true;
}


int CAps_Insp::Inspect_RelativeUniformity(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeUniformitySpec& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRU, bool bUse8BitOnly)
{
	int nResult = R_RESULT_PASS;

	std::shared_ptr<CACMISShadingRelativeUniformity> m_pRelativeUniformity = std::make_shared<CACMISShadingRelativeUniformity>();

	//Logging
	std::cout << "[Relative Uniformity] Version = " << m_pRelativeUniformity->GetVersion() << std::endl;


	//Inspection
	if (!m_pRelativeUniformity->Inspect(pBuffer, nImageWidth, nImageHeight, _Spec, tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly))
	{
		std::cout << "[Relative Uniformity] Inspection Fail! " << std::endl;
		return FALSE;
	}

	//Result 
	TRelativeUniformityResult m_stRelativeUniformityResult;

	m_stRelativeUniformityResult = *m_pRelativeUniformity->GetInspectionResult();

	int roiSize = _Spec.nROISize;
	if (_Spec.dSpecMax < m_stRelativeUniformityResult.dResultMax)
		nResult = R_RESULT_FAIL;

	//Logging
	std::cout << "[Relative Uniformity] Max= " << m_stRelativeUniformityResult.dResultMax << std::endl;
	std::cout << "[Relative Uniformity] Block( " << m_stRelativeUniformityResult.nMaxBlockX << " , " << m_stRelativeUniformityResult.nMaxBlockY << " )";
	std::cout << "X= " << (m_stRelativeUniformityResult.nMaxBlockX)*(_Spec.nROISize) << ", ";
	std::cout << "Y= " << (m_stRelativeUniformityResult.nMaxBlockY)*(_Spec.nROISize) << ", ";
	std::cout << "Width= " << _Spec.nROISize << ", " << "Height= " << _Spec.nROISize << std::endl;
	std::cout << "[Relative Uniformity] Ref Block( " << m_stRelativeUniformityResult.nRefBlockX << " , " << m_stRelativeUniformityResult.nRefBlockY << " )";
	std::cout << "X= " << (m_stRelativeUniformityResult.nRefBlockX)*(_Spec.nROISize) << ", ";
	std::cout << "Y= " << (m_stRelativeUniformityResult.nRefBlockY)*(_Spec.nROISize) << ", ";
	std::cout << "Width= " << _Spec.nROISize << ", " << "Height= " << _Spec.nROISize << std::endl;

	//Graphic
	RECT rect, rectRef;

	rect.left = m_stRelativeUniformityResult.nMaxBlockX * roiSize;
	rect.top = m_stRelativeUniformityResult.nMaxBlockY * roiSize;
	rect.right = rect.left + roiSize;
	rect.bottom = rect.top + roiSize;

	rectRef.left = m_stRelativeUniformityResult.nRefBlockX * roiSize;
	rectRef.top = m_stRelativeUniformityResult.nRefBlockY * roiSize;
	rectRef.right = rectRef.left + roiSize;
	rectRef.bottom = rectRef.top + roiSize;

	cvRectangle(cvImgRU, cvPoint(rect.left, rect.top), cvPoint(rect.right, rect.bottom), CV_LIGHTGREEN);
	cvRectangle(cvImgRU, cvPoint(rectRef.left, rectRef.top), cvPoint(rectRef.right, rectRef.bottom), CV_YELLOW);

	return nResult;
}
int CAps_Insp::Inspect_ColorSensitivity(const BYTE* pBuffer,  bool bUse8BitOnly)
{
    int nWidth = g_clModelData[m_nUnit].m_nWidth;
    int nHeight = g_clModelData[m_nUnit].m_nHeight;
    TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n;

	int nResult = R_RESULT_PASS;
	int i;
    int nResultRI = -1, nResultRU = -1, nResultCS = -1, nResultRI_X;
    char title[100];

    // Image buffers
    std::vector<BYTE> vBmpBuffer(nWidth * nHeight * 3, 0);

    // output image
    //IplImage *cvImgShading = cvCreateImage(cvSize(nWidth, nHeight), 8, 3);
   // cvImgShading->imageData = (char*)vBmpBuffer.data();

    // for display
    //if (bRaw)
    //{
    //    ACMISSoftISP::xMakeBMP(pBuffer, vBmpBuffer.data(), nWidth, nHeight, tDataSpec);
    //}
    //else
    //{
    //   // std::copy(pBuffer, pBuffer + sizeof(BYTE) * nWidth * nHeight * 3, vBmpBuffer.data());
    //}
    // Spec information
    TShadingSpec tShadingSpec;
    memset(&tShadingSpec, 0x00, sizeof(TShadingSpec));
    //----------------------------------------------------------------------
    // Spec - Color Sensitivity
    //----------------------------------------------------------------------

    // Initialize ROI regions and offset values
    RECT rtRoi;
    double dOffset = 0.0;

    SetRect(&rtRoi, 856, 432, 965, 507);

    tShadingSpec.m_stColorSensitivitySpec.tROI.eROIType = ROIType_RECT;
    tShadingSpec.m_stColorSensitivitySpec.tROI.ROICount = 1;
    tShadingSpec.m_stColorSensitivitySpec.tROI.dOffset = &dOffset;
    tShadingSpec.m_stColorSensitivitySpec.tROI.pROIData = &rtRoi;

    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMaxRG =   1.000000;
    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMinRG =   1.000000;
    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMaxBG =   1.000000;
    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMinBG =   1.000000;
    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMaxGrGb = 1.000000;
    tShadingSpec.m_stColorSensitivitySpec.dSpecDevMinGrGb = 1.000000;
    tShadingSpec.m_stColorSensitivitySpec.nTypicalValueType = 0;

    /*nResultCS = Inspect_ColorSensitivity(pBuffer, nWidth, nHeight, tShadingSpec.m_stColorSensitivitySpec, 
        tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, cvImgShading, bUse8BitOnly);
    std::cout << "nResult Color Sensitivity =" << nResultCS << std::endl;*/

    std::shared_ptr<CACMISShadingColorSensitivity> m_pColorSensitivity = std::make_shared<CACMISShadingColorSensitivity>();

    std::cout << "[Color Sensitivity] Version = " << m_pColorSensitivity->GetVersion() << std::endl;

    //Inspection
    /*if (!m_pColorSensitivity->Inspect(pBuffer, nWidth, nHeight, tShadingSpec.m_stColorSensitivitySpec, 
        tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, cvImgShading, bUse8BitOnly))
    {
        std::cout << "[Color Sensitivity] Inspection Fail! " << std::endl;
        return FALSE;
    }*/

    //Result
    //std::vector<TColorSensitivityResult> m_stColorSensitivityResult;
    //for (i = 0; i < _Spec.tROI.ROICount; i++)
    //{
    //    m_stColorSensitivityResult.push_back(*m_pColorSensitivity->GetInspectionResult(i));

    //    // Check Color Sensitivity RG
    //    if ((_Spec.dSpecDevMinRG > m_stColorSensitivityResult[i].tAdjustColorRatio.RG) || (_Spec.dSpecDevMaxRG < m_stColorSensitivityResult[i].tAdjustColorRatio.RG))
    //        nResult = R_RESULT_FAIL;
    //    // Check Color Sensitivity BG
    //    if ((_Spec.dSpecDevMinBG > m_stColorSensitivityResult[i].tAdjustColorRatio.BG) || (_Spec.dSpecDevMaxBG < m_stColorSensitivityResult[i].tAdjustColorRatio.BG))
    //        nResult = R_RESULT_FAIL;
    //    // Check Color Sensitivity GrGb
    //    if ((_Spec.dSpecDevMinGrGb > m_stColorSensitivityResult[i].tAdjustColorRatio.GrGb) || (_Spec.dSpecDevMaxGrGb < m_stColorSensitivityResult[i].tAdjustColorRatio.GrGb))
    //        nResult = R_RESULT_FAIL;

    //    //Logging
    //    TColorSensitivityResult* pResult = &m_stColorSensitivityResult[i];
    //    //printf("[Color Sensitivity] RG=%.4f, BG=%.4f, GrGb=%.4f \n", pResult->tColorRatio.RG, pResult->tColorRatio.BG, pResult->tColorRatio.GrGb);
    //    //printf("[Color Sensitivity] R=%.4f, B=%.4f, B=%.4f \n", pResult->tColorRatio.R, pResult->tColorRatio.G, pResult->tColorRatio.B);
    //    //printf("[Color Sensitivity] Gr=%.4f, Gb=%.4f \n", pResult->tColorRatio.Gr, pResult->tColorRatio.Gb);

    //    //Graphic
    //    const RECT* rt = m_pColorSensitivity->GetInspectionROI((EPos)i);
    //    cvRectangle(cvImgRI, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(0, 0, 255));
    //}



   /* if (ImagePath != nullptr && bSaveResultImage == true)
    {
        char filename[100];
        char *tmp = (char *)strrchr(ImagePath, '.');

        if (tmp != nullptr)
        {
            strncpy_s(filename, ImagePath, (int)(tmp - ImagePath));
        }
        else
        {
            strcpy_s(filename, ImagePath);
        }
        strcat_s(filename, "_Shading_result.bmp");
        printf("Saving Result image: %s\n", filename);
        cvSaveImage(filename, cvImgShading);
    }*/

    /*sprintf(title, "Shading%s%s", ImagePath != nullptr ? " - " : "", ImagePath != nullptr ? ImagePath : "");
    cvShowImage(title, cvImgShading);
    cvReleaseImage(&cvImgShading);
    cvWaitKey(0);*/
	//std::shared_ptr<CACMISShadingColorSensitivity> m_pColorSensitivity = std::make_shared<CACMISShadingColorSensitivity>();

	//std::cout << "[Color Sensitivity] Version = " << m_pColorSensitivity->GetVersion() << std::endl;

	////Inspection
	//if (!m_pColorSensitivity->Inspect(pBuffer, nWidth, nHeight, _Spec, tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly))
	//{
	//	std::cout << "[Color Sensitivity] Inspection Fail! " << std::endl;
	//	return FALSE;
	//}

	////Result
	//std::vector<TColorSensitivityResult> m_stColorSensitivityResult;
	//for (i = 0; i < _Spec.tROI.ROICount; i++)
	//{
	//	m_stColorSensitivityResult.push_back(*m_pColorSensitivity->GetInspectionResult(i));

	//	// Check Color Sensitivity RG
	//	if ((_Spec.dSpecDevMinRG > m_stColorSensitivityResult[i].tAdjustColorRatio.RG) || (_Spec.dSpecDevMaxRG < m_stColorSensitivityResult[i].tAdjustColorRatio.RG))
	//		nResult = R_RESULT_FAIL;
	//	// Check Color Sensitivity BG
	//	if ((_Spec.dSpecDevMinBG > m_stColorSensitivityResult[i].tAdjustColorRatio.BG) || (_Spec.dSpecDevMaxBG < m_stColorSensitivityResult[i].tAdjustColorRatio.BG))
	//		nResult = R_RESULT_FAIL;
	//	// Check Color Sensitivity GrGb
	//	if ((_Spec.dSpecDevMinGrGb > m_stColorSensitivityResult[i].tAdjustColorRatio.GrGb) || (_Spec.dSpecDevMaxGrGb < m_stColorSensitivityResult[i].tAdjustColorRatio.GrGb))
	//		nResult = R_RESULT_FAIL;

	//	//Logging
	//	TColorSensitivityResult* pResult = &m_stColorSensitivityResult[i];
	//	printf("[Color Sensitivity] RG=%.4f, BG=%.4f, GrGb=%.4f \n", pResult->tColorRatio.RG, pResult->tColorRatio.BG, pResult->tColorRatio.GrGb);
	//	printf("[Color Sensitivity] R=%.4f, B=%.4f, B=%.4f \n", pResult->tColorRatio.R, pResult->tColorRatio.G, pResult->tColorRatio.B);
	//	printf("[Color Sensitivity] Gr=%.4f, Gb=%.4f \n", pResult->tColorRatio.Gr, pResult->tColorRatio.Gb);

	//	//Graphic
	//	const RECT* rt = m_pColorSensitivity->GetInspectionROI((EPos)i);
	//	cvRectangle(cvImgRI, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(0, 0, 255));
	//}

	return nResult;
}





CString CAps_Insp::SetDir_Check(CString sPath)
{
	CString sRtn = _T(""), FolderName = _T("");
	CFileFind finder;
	BOOL IsFind;

	SYSTEMTIME time;
	::GetLocalTime(&time);

	FolderName.Format(_T("%s\\%04d%02d"), (TCHAR*)(LPCTSTR)sPath, time.wYear, time.wMonth);
	IsFind = finder.FindFile(FolderName);
	if (!IsFind)
	{
		CreateDirectory(FolderName, NULL);
	}

	sRtn.Format(_T("%s\\%04d%02d\\%02d"), (TCHAR*)(LPCTSTR)sPath, time.wYear, time.wMonth, time.wDay);
	IsFind = finder.FindFile(sRtn);
	if (!IsFind)
	{
		CreateDirectory(sRtn, NULL);
	}

	sRtn.Format(_T("%s\\%04d%02d\\%02d\\"), (TCHAR*)(LPCTSTR)sPath, time.wYear, time.wMonth, time.wDay);

	return sRtn;
}


//-----------------------------------------------------------------------------
//
//	�̹�(stain) �˻�
//
//-----------------------------------------------------------------------------

bool CAps_Insp::func_Insp_Stain(BYTE* img, bool bAutoMode, int mRCount, bool bUse8BitOnly)
{
    int nBlackLevel = 0;
    int nWidth = g_clModelData[m_nUnit].m_nWidth;
    int nHeight = g_clModelData[m_nUnit].m_nHeight;
	int nResult1 = 0;
	int nResult2 = 0;
	int nResult3 = 0;
    TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n;
    //
   // IplImage *cvStainImg = cvCreateImage(cvSize(nWidth, nHeight), 8, 3); // bmp for display

	

	g_clMesCommunication[m_nUnit].m_nMesBlemishResult[0] = 1;
	g_clMesCommunication[m_nUnit].m_nMesBlemishResult[1] = 1;
	g_clMesCommunication[m_nUnit].m_nMesBlemishResult[2] = 1;
    //
	//nResult1 = BlackSpotInsp((BYTE*)img, nWidth, nHeight, tDataSpec, cvStainImg, mRCount);

	/*nResult1 = LCBInsp((BYTE*)img,  mRCount);

	nResult2 = Blemish_YmeanInsp((BYTE*)img); 

	nResult3 = FDFInsp((BYTE*)img);*/
	//



    return true;
}



int CAps_Insp::LCBInsp(BYTE* img, int mRetryCount, bool bAutoMode)
{
    int nResult = R_RESULT_PASS;
    int nResultLCB = 0;
    
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec;
	bool bUse8BitOnly = false;

	if (g_clModelData[m_nUnit].m_nBlemish8BitUse == 1)
	{
		bUse8BitOnly = true;
	}

	int Index = 0;



	TCHAR szLog[SIZE_OF_1K];
    TLCBSpecN tStainSpec;
    memset(&tStainSpec, 0x00, sizeof(TLCBSpecN));
    int specCount = 0;
	//IplImage *bmpImg = cvCreateImage(cvSize(nWidth, nHeight), 8, 3); // bmp for display
	//bmpImg->imageData = (char*)g_clLaonGrabberWrapper[m_nUnit].m_pFrameBMPBuffer;

	ACMISSoftISP::xMakeBMP(img, (byte*)g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer,
		g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n);
	Mat bmpImg = Mat(g_clModelData[m_nUnit].m_nHeight, g_clModelData[m_nUnit].m_nWidth, CV_8UC3);
	bmpImg.data = g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer;
    //----------------------------------------------------------------------
    // Spec - LCB
    //----------------------------------------------------------------------
 //   tStainSpec.dCenterThreshold = g_clModelData[m_nUnit].m_LcbSpec[specCount++];// 35.0;
 //   tStainSpec.dEdgeThreshold = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//13.5;
 //   tStainSpec.dCornerThreshold = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//40.5;
 //   tStainSpec.nMaxSingleDefectNum = (int)g_clModelData[m_nUnit].m_LcbSpec[specCount++];//50000;
 //   tStainSpec.nMinDefectWidthHeight = (int)g_clModelData[m_nUnit].m_LcbSpec[specCount++];//12;
	//tStainSpec.dCenterMaxR = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//9;
	//tStainSpec.dCenterMaxGr = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCenterMaxGb = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCenterMaxB = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//9;
	//tStainSpec.dEdgeMaxR = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dEdgeMaxGr = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dEdgeMaxGb = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dEdgeMaxB = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCornerMaxR = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCornerMaxGr = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCornerMaxGb = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
	//tStainSpec.dCornerMaxB = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0;
 //   tStainSpec.tCircleSpec.bEnableCircle = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//true;
 //   tStainSpec.tCircleSpec.nPosOffsetX = (int)g_clModelData[m_nUnit].m_LcbSpec[specCount++];//5;
 //   tStainSpec.tCircleSpec.nPosOffsetY = (int)g_clModelData[m_nUnit].m_LcbSpec[specCount++];//5;
 //   tStainSpec.tCircleSpec.dRadiusRatioX = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.5;
 //   tStainSpec.tCircleSpec.dRadiusRatioY = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.5;
 //   tStainSpec.tCircleSpec.dThresholdRatio = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.5;
 //   tStainSpec.tCircleSpec.dROIRange = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.5;
 //   tStainSpec.tCircleSpec.nUsedFixedCircle = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//false;
 //   tStainSpec.tMultiCircleSpec.bEnableMultiCircle = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//false;
 //   tStainSpec.tMultiCircleSpec.dZoneSizeRatio[0] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.25;
 //   tStainSpec.tMultiCircleSpec.dZoneSizeRatio[1] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.35;
 //   tStainSpec.tMultiCircleSpec.dZoneSizeRatio[2] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//0.45;
 //   tStainSpec.tMultiCircleSpec.dThreshold[0] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//9.0;
 //   tStainSpec.tMultiCircleSpec.dThreshold[1] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//15.0;
 //   tStainSpec.tMultiCircleSpec.dThreshold[2] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];//50.0;
 //   tStainSpec.tMultiCircleSpec.nBlobSize[0] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];
 //   tStainSpec.tMultiCircleSpec.nBlobSize[1] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];
 //   tStainSpec.tMultiCircleSpec.nBlobSize[2] = g_clModelData[m_nUnit].m_LcbSpec[specCount++];


	tStainSpec.dCenterThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[38]);
	tStainSpec.dEdgeThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[39]);
	tStainSpec.dCornerThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[40]);
	tStainSpec.nMaxSingleDefectNum = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[41]);
	tStainSpec.nMinDefectWidthHeight = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[42]);

	tStainSpec.dCenterMaxR = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[43]);
	tStainSpec.dCenterMaxGr = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[44]);
	tStainSpec.dCenterMaxGb = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[45]);
	tStainSpec.dCenterMaxB = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[46]);

	tStainSpec.dEdgeMaxR = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[47]);
	tStainSpec.dEdgeMaxGr = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[48]);
	tStainSpec.dEdgeMaxGb = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[49]);
	tStainSpec.dEdgeMaxB = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[50]);

	tStainSpec.dCornerMaxR = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[51]);
	tStainSpec.dCornerMaxGr = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[52]);
	tStainSpec.dCornerMaxGb = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[53]);
	tStainSpec.dCornerMaxB = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[54]);

	Index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[55]);
	if (Index == 1)
	{
		tStainSpec.tCircleSpec.bEnableCircle = true;
	}
	else
	{
		tStainSpec.tCircleSpec.bEnableCircle = false;
	}
	tStainSpec.tCircleSpec.nPosOffsetX = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[56]);
	tStainSpec.tCircleSpec.nPosOffsetY = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[57]);
	tStainSpec.tCircleSpec.dRadiusRatioX = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[58]);
	tStainSpec.tCircleSpec.dRadiusRatioY = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[59]);

	tStainSpec.tCircleSpec.dThresholdRatio = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[60]);
	tStainSpec.tCircleSpec.dROIRange = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[61]);
	tStainSpec.tCircleSpec.nUsedFixedCircle = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[62]);

	Index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[63]);
	if (Index == 1)
	{
		tStainSpec.tMultiCircleSpec.bEnableMultiCircle = true;
	}
	else
	{
		tStainSpec.tMultiCircleSpec.bEnableMultiCircle = false;
	}
	tStainSpec.tMultiCircleSpec.dZoneSizeRatio[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[64]);
	tStainSpec.tMultiCircleSpec.dZoneSizeRatio[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[65]);
	tStainSpec.tMultiCircleSpec.dZoneSizeRatio[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[66]);
	tStainSpec.tMultiCircleSpec.dThreshold[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[67]);
	tStainSpec.tMultiCircleSpec.dThreshold[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[68]);
	tStainSpec.tMultiCircleSpec.dThreshold[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[69]);
	tStainSpec.tMultiCircleSpec.nBlobSize[0] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[70]);
	tStainSpec.tMultiCircleSpec.nBlobSize[1] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[71]);
	tStainSpec.tMultiCircleSpec.nBlobSize[2] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[72]);

    std::shared_ptr<CACMISImageStainLCBCommon> pInspectLCB = std::make_shared<CACMISImageStainLCBCommon>();

	pInspectLCB->SetInspectPosOffset(g_clModelData[m_nUnit].m_LcbOffsetSpec[0], g_clModelData[m_nUnit].m_LcbOffsetSpec[1], g_clModelData[m_nUnit].m_LcbOffsetSpec[2], g_clModelData[m_nUnit].m_LcbOffsetSpec[3]);
	bool nRtn = false;
	nRtn = pInspectLCB->Inspect((BYTE*)img, nWidth, nHeight, tStainSpec,tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly, false, tDataSpec.eDemosaicMethod);
	if (nRtn == false)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("16"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//LCB
		return -1;
	}
    if (nResultLCB > 0)
    {
        nResult = R_FAIL_STAIN;
    }
	int LCBBlobCount = pInspectLCB->GetDefectBlobCount();
	g_clMesCommunication[m_nUnit].m_dMesSaturationOc = pInspectLCB->GetSaturationResult(0);


	_stprintf_s(szLog, SIZE_OF_1K, _T("[LCB] Version %s"), pInspectLCB->GetVersion());
    AddLog(szLog, 0, m_nUnit);


    int ocx = 0, ocy = 0, radx = 0, rady = 0;
   
	//CvFont cvfont;
	CvPoint pt;
	int scale = (nWidth > 640 ? 1 : 2);
	double nFontSize = 1 / scale;
	//CString sTemp;

	//cvInitFont(&cvfont, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_NORMAL, nFontSize, nFontSize, 0, 1, 10);
	//sTemp.Format("LCB BlobCount : %d", LCBBlobCount);
	char strTmp[1024];
	sprintf_s(strTmp, "LCB BlobCount : %d", LCBBlobCount);
	pt.x = 50;
	pt.y = 80;
	//cvPutText(bmpImg, sTemp, pt, &cvfont, CV_BLUE);
	putText(bmpImg, strTmp, pt, 0, 1.5, Scalar(10, 10, 10), 3);

	double dFactorX = 0.0;
	double dFactorY = 0.0;

	dFactorX = ((double)g_clModelData[m_nUnit].DefectSizeX / (double)g_clModelData[m_nUnit].m_nWidth);
	dFactorY = ((double)g_clModelData[m_nUnit].DefectSizeY / (double)g_clModelData[m_nUnit].m_nHeight);

	CRect rcReduceBox;

	int dLcbSpecMin = 0;
	int dLcbSpecMax = 0;

	g_clMandoInspLog[m_nUnit].m_LogBlemishLcb = LCBBlobCount;
	g_clMesCommunication[m_nUnit].m_nMesBlemish[1] = g_clMandoInspLog[m_nUnit].m_LogBlemishLcb;

	//dLcbSpecMin = (_ttof(EEpromVerifyData.vMinData[39]));	//lcb
	//dLcbSpecMax = (_ttof(EEpromVerifyData.vMaxData[39]));

	dLcbSpecMax = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[18]);
    if (LCBBlobCount < dLcbSpecMin || LCBBlobCount > dLcbSpecMax)
    {
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("16"));
        //�̹��ҷ�
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[LCB] BlobCount Spec Out %d [%d~%d]"), LCBBlobCount, dLcbSpecMin, dLcbSpecMax);
		AddLog(szLog, 0, m_nUnit);

		g_clMesCommunication[m_nUnit].m_sStain.Format(_T("FAIL"));
		g_clTaskWork[m_nUnit].m_bOutputCheck[3] = false;	//STAIN LCB

		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[1] = 0;
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//LCB


        g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [STAIN LCB NG]");
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
        if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
        {
            g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [LCB BLOBCOUNT:%d]"), LCBBlobCount);
            g_clMandoInspLog[m_nUnit].m_nNGCnt++;
        }
		MgraColor(M_DEFAULT, M_COLOR_RED);
		for (int i = 0; i < LCBBlobCount; i++)
		{
			const RECT* rt = pInspectLCB->GetDefectBlobRect(i);
			//cvRectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(0, 0, 255), 2);
			if (bAutoMode == false)
			{
				g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rt->left, rt->top, rt->right, rt->bottom, M_COLOR_BLUE, 1, FALSE);
			}
			rectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(0, 0, 255), 2);

			//

			rcReduceBox.left = (int)(rt->left	* dFactorX + 0.5);
			rcReduceBox.top = (int)(rt->top		* dFactorY + 0.5);
			rcReduceBox.right = (int)(rt->right	* dFactorX + 0.5);
			rcReduceBox.bottom = (int)(rt->bottom	* dFactorY + 0.5);
			MgraRect(M_DEFAULT, g_clVision.MilDefectOverlayImage[m_nUnit], rcReduceBox.left, rcReduceBox.top, rcReduceBox.right, rcReduceBox.bottom);

		}
		
		g_clVision.DrawOverlayAll(m_nUnit);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[LCB] BlobCount Spec In %d [%d~%d]"), LCBBlobCount, dLcbSpecMin, dLcbSpecMax);
		AddLog(szLog, 0, m_nUnit);

		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[1] = 1;
		g_clMesCommunication[m_nUnit].m_sStain.Format(_T("PASS"));
	}

    



    if (tStainSpec.tCircleSpec.bEnableCircle || tStainSpec.tMultiCircleSpec.bEnableMultiCircle)
    {
        const TDefectResult* pCircleMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
        ocx = (int)pCircleMaxResult->dContrastMaxR;
        ocy = (int)pCircleMaxResult->dContrastMaxGb;
        radx = (int)pCircleMaxResult->dContrastMaxGr * tStainSpec.tCircleSpec.dRadiusRatioX;
        rady = (int)pCircleMaxResult->dContrastMaxB  *tStainSpec.tCircleSpec.dRadiusRatioY;
       
        if (tStainSpec.tCircleSpec.bEnableCircle)
        {
			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255), 2);
            g_clVision.DrawMOverlayCircle(m_nUnit, ocx - radx, ocy - rady, ocx + radx, ocy + rady, M_COLOR_MAGENTA, 1, FALSE);
        }
        else
        {
            double dRadYRatio = 1.0;
            if (tStainSpec.tCircleSpec.nUsedFixedCircle)
            {
                dRadYRatio = tStainSpec.tCircleSpec.dRadiusRatioY / tStainSpec.tCircleSpec.dRadiusRatioX;
            }
            //������x
            //������y
            int nZoneA_X = (int)(radx * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[0]);
            int nZoneA_Y = (int)(rady * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[0] * dRadYRatio);
            int nZoneB_X = (int)(radx * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[1]);
            int nZoneB_Y = (int)(rady * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[1] * dRadYRatio);
            int nZoneC_X = (int)(radx * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[2]);
            int nZoneC_Y = (int)(rady * tStainSpec.tMultiCircleSpec.dZoneSizeRatio[2] * dRadYRatio);

			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(nZoneA_X, nZoneA_Y), 0, 0, 360, CV_RGB(0, 255, 255), 2);
			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(nZoneB_X, nZoneB_Y), 0, 0, 360, CV_RGB(0, 255, 255), 2);
			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(nZoneC_X, nZoneC_Y), 0, 0, 360, CV_RGB(0, 255, 255), 2);
            //
            g_clVision.DrawMOverlayCircle(m_nUnit, ocx - nZoneA_X, ocy - nZoneA_Y, ocx + nZoneA_X, ocy + nZoneA_Y, M_COLOR_MAGENTA, 1, FALSE);
            g_clVision.DrawMOverlayCircle(m_nUnit, ocx - nZoneB_X, ocy - nZoneB_Y, ocx + nZoneB_X, ocy + nZoneB_Y, M_COLOR_MAGENTA, 1, FALSE);
            g_clVision.DrawMOverlayCircle(m_nUnit, ocx - nZoneC_X, ocy - nZoneC_Y, ocx + nZoneC_X, ocy + nZoneC_Y, M_COLOR_MAGENTA, 1, FALSE);
            //vision.ellipselist[CCD].addList(CPoint(ocx - nZoneA_X, ocy - nZoneA_Y), CPoint(ocx + nZoneA_X, ocy + nZoneA_Y), PS_SOLID, M_COLOR_CYAN);

        }
        if (tStainSpec.tCircleSpec.nUsedFixedCircle)
        {
            RECT rtRefEdge;
            RECT rtCenter;
            //oc threshold test roi - top
            rtRefEdge.left = max(nWidth / 2 - 100 / 2, 0);
            rtRefEdge.top = (LONG)(0.1 * nHeight / 2);
            rtRefEdge.right = min(rtRefEdge.left + 100 - 1, (LONG)nWidth - 1);
            rtRefEdge.bottom = min(rtRefEdge.top + 100 - 1, (LONG)nHeight - 1);

            //oc threshold test roi - center
            rtCenter.left = rtRefEdge.left;
            rtCenter.top = max(nHeight / 2 - 100 / 2, 0);
            rtCenter.right = rtRefEdge.right;
            rtCenter.bottom = min(nHeight / 2 + 100 / 2 - 1, nHeight - 1);

            //cvRectangle(bmpImg, cvPoint(rtRefEdge.left, rtRefEdge.top), cvPoint(rtRefEdge.right, rtRefEdge.bottom), CV_RGB(255, 86, 12), 2);
            //cvRectangle(bmpImg, cvPoint(rtCenter.left, rtCenter.top), cvPoint(rtCenter.right, rtCenter.bottom), CV_RGB(255, 86, 12), 2);

			rectangle(bmpImg, cvPoint(rtRefEdge.left, rtRefEdge.top), cvPoint(rtRefEdge.right, rtRefEdge.bottom), CV_RGB(255, 86, 12), 2);
			rectangle(bmpImg, cvPoint(rtCenter.left, rtCenter.top), cvPoint(rtCenter.right, rtCenter.bottom), CV_RGB(255, 86, 12), 2);
            g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rtRefEdge.left, rtRefEdge.top, rtRefEdge.right, rtRefEdge.bottom, M_COLOR_BLUE, 1, FALSE);
            g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rtCenter.left, rtCenter.top, rtCenter.right, rtCenter.bottom, M_COLOR_BLUE, 1, FALSE);
        }
        
    }

	int nCenterCount = pInspectLCB->GetDefectCount(EIMAGEREGION_CENTER);
	const TDefectResult* pCenterMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CENTER);

	g_clMandoInspLog[m_nUnit].m_LogBlemishLcbMaxCenter = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishLcbMaxEdge = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishLcbMaxCorner = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;

	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[0] = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[1] = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[2] = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;

	_stprintf_s(szLog, SIZE_OF_1K, _T("[LCB] Max Value (Center) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[0]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[LCB] Max Value (Edge) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[1]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[LCB] Max Value (Corner) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[2]);
	AddLog(szLog, 0, m_nUnit);

    if (nResult)
    {
        if (tStainSpec.tCircleSpec.bEnableCircle || tStainSpec.tMultiCircleSpec.bEnableMultiCircle)
        {
            int nCircleCount = pInspectLCB->GetDefectCount(EIMAGEREGION_CIRCLE);
            if (tStainSpec.tCircleSpec.nUsedFixedCircle)
            {
                RECT rtRefEdge;
                RECT rtCenter;
                //oc threshold test roi - top
                rtRefEdge.left = max(g_clModelData[m_nUnit].m_nWidth / 2 - 100 / 2, 0);
                rtRefEdge.top = (LONG)(0.1 * g_clModelData[m_nUnit].m_nHeight / 2);
                rtRefEdge.right = min(rtRefEdge.left + 100 - 1, (LONG)g_clModelData[m_nUnit].m_nWidth - 1);
                rtRefEdge.bottom = min(rtRefEdge.top + 100 - 1, (LONG)g_clModelData[m_nUnit].m_nHeight - 1);

                //oc threshold test roi - center
                rtCenter.left = rtRefEdge.left;
                rtCenter.top = max(g_clModelData[m_nUnit].m_nHeight / 2 - 100 / 2, 0);
                rtCenter.right = rtRefEdge.right;
                rtCenter.bottom = min(g_clModelData[m_nUnit].m_nHeight / 2 + 100 / 2 - 1, g_clModelData[m_nUnit].m_nHeight - 1);

                //cvRectangle(bmpImg, cvPoint(rtRefEdge.left, rtRefEdge.top), cvPoint(rtRefEdge.right, rtRefEdge.bottom), CV_RGB(255, 86, 12) , 2);
                //cvRectangle(bmpImg, cvPoint(rtCenter.left, rtCenter.top), cvPoint(rtCenter.right, rtCenter.bottom), CV_RGB(255, 86, 12), 2);

				rectangle(bmpImg, cvPoint(rtRefEdge.left, rtRefEdge.top), cvPoint(rtRefEdge.right, rtRefEdge.bottom), CV_RGB(255, 86, 12), 2);
				rectangle(bmpImg, cvPoint(rtCenter.left, rtCenter.top), cvPoint(rtCenter.right, rtCenter.bottom), CV_RGB(255, 86, 12), 2);
                g_clVision.DrawMOverlayBox(m_nUnit,  m_nUnit, rtRefEdge, M_COLOR_RED, 1, FALSE, PS_SOLID);
                g_clVision.DrawMOverlayBox(m_nUnit,  m_nUnit, rtCenter, M_COLOR_RED, 1, FALSE, PS_SOLID);
                //vision.boxlist[CCD].addList(rtRefEdge, PS_SOLID, M_COLOR_RED);
                //vision.boxlist[CCD].addList(rtCenter, PS_SOLID, M_COLOR_RED);

            }
        }
    }

	if (g_clModelData[m_nUnit].m_nNgImageSaveUse == 1)
	{
		if (LCBBlobCount > 0)
		{
			g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 1);
		}
	}else
	{

		g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 1);
	}
	
    g_SaveLGITLog(m_nUnit, "LCB", pInspectLCB->GetLogHeader(), pInspectLCB->GetLogData(), pInspectLCB->GetVersion());
    return LCBBlobCount;
}

int CAps_Insp::FDFInsp(BYTE* img, bool bAutoMode)
{
	int nResult = R_RESULT_PASS;

	int nBlackLevel = 0;
	//CString szLog = "";
	TCHAR szLog[SIZE_OF_1K];
	TFDFSpec stSpecFDF;
	memset(&stSpecFDF, 0x00, sizeof(TFDFSpec));
	int specCount = 0;
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	int index = 0;
	TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec;
	bool bUse8BitOnly = false;
	
	if (g_clModelData[m_nUnit].m_nBlemish8BitUse == 1)
	{
		bUse8BitOnly = true;
	}

	ACMISSoftISP::xMakeBMP(img, (byte*)g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer,
		g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n);
	Mat bmpImg = Mat(g_clModelData[m_nUnit].m_nHeight, g_clModelData[m_nUnit].m_nWidth, CV_8UC3);

	bmpImg.data = g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer;
	//----------------------------------------------------------------------
	// Spec - FDF
	//----------------------------------------------------------------------
	//stSpecFDF.dCenterThreshold = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//10.75;
	//stSpecFDF.dEdgeThreshold = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//15.0;
	//stSpecFDF.dCornerThreshold = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//15.0;
	//stSpecFDF.nMedianFilterWidth = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0;
	//stSpecFDF.nMedianFilterHeight = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//1000;
	//stSpecFDF.nWidthScaleRatio = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//70.0;
	//stSpecFDF.nHeightScaleRatio = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//4.2;
	//stSpecFDF.nMinDefectWidthHeight = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//64;
	//stSpecFDF.nWindowSize = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];// 100;
	//stSpecFDF.nEdgeSize = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];
	//stSpecFDF.bEnableChannel = (bool)g_clModelData[m_nUnit].m_FDFSpec[specCount++];
	//stSpecFDF.nMaxSingleDefectNum = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//15;

	//stSpecFDF.tCircleSpec.bEnableCircle = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//false;
	//stSpecFDF.tCircleSpec.nPosOffsetX = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//5;
	//stSpecFDF.tCircleSpec.nPosOffsetY = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//5;
	//stSpecFDF.tCircleSpec.dRadiusRatioX = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.45;
	//stSpecFDF.tCircleSpec.dRadiusRatioY = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.45;
	//stSpecFDF.tCircleSpec.dThresholdRatio = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.5;
	//stSpecFDF.tCircleSpec.dROIRange = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.5;
	//stSpecFDF.tCircleSpec.nUsedFixedCircle = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0;

	//stSpecFDF.tMultiCircleSpec.bEnableMultiCircle = (int)g_clModelData[m_nUnit].m_FDFSpec[specCount++];//false;
	//stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[0] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.2;
	//stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[1] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.4;
	//stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[2] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.52;
	//stSpecFDF.tMultiCircleSpec.dThreshold[0] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.45;
	//stSpecFDF.tMultiCircleSpec.dThreshold[1] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.45;
	//stSpecFDF.tMultiCircleSpec.dThreshold[2] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];//0.8;
	//stSpecFDF.tMultiCircleSpec.nBlobSize[0] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];
	//stSpecFDF.tMultiCircleSpec.nBlobSize[1] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];
	//stSpecFDF.tMultiCircleSpec.nBlobSize[2] = g_clModelData[m_nUnit].m_FDFSpec[specCount++];



	//
	//
	stSpecFDF.dCenterThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[101]);
	stSpecFDF.dEdgeThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[102]);
	stSpecFDF.dCornerThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[103]);

	stSpecFDF.nMedianFilterWidth = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[104]);
	stSpecFDF.nMedianFilterHeight = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[105]);
	stSpecFDF.nWidthScaleRatio = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[106]);
	stSpecFDF.nHeightScaleRatio = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[107]);
	stSpecFDF.nMinDefectWidthHeight = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[108]);
	stSpecFDF.nWindowSize = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[109]);
	stSpecFDF.nEdgeSize = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[110]);

	index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[111]);
	if (index == 1)
	{
		stSpecFDF.bEnableChannel = true;
	}
	else
	{
		stSpecFDF.bEnableChannel = false;
	}
	
	stSpecFDF.nMaxSingleDefectNum = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[112]);

	index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[113]);
	if (index == 1)
	{
		stSpecFDF.tCircleSpec.bEnableCircle = true;
	}
	else
	{
		stSpecFDF.tCircleSpec.bEnableCircle = false;
	}
	stSpecFDF.tCircleSpec.nPosOffsetX = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[114]);
	stSpecFDF.tCircleSpec.nPosOffsetY = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[115]);
	stSpecFDF.tCircleSpec.dRadiusRatioX = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[116]);
	stSpecFDF.tCircleSpec.dRadiusRatioY = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[117]);
	stSpecFDF.tCircleSpec.dThresholdRatio = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[118]);
	stSpecFDF.tCircleSpec.dROIRange = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[119]);
	stSpecFDF.tCircleSpec.nUsedFixedCircle = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[120]);
	index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[121]);
	if (index == 1)
	{
		stSpecFDF.tMultiCircleSpec.bEnableMultiCircle = true;
	}
	else
	{
		stSpecFDF.tMultiCircleSpec.bEnableMultiCircle = false;
	}
	stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[122]);
	stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[123]);
	stSpecFDF.tMultiCircleSpec.dZoneSizeRatio[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[124]);
	stSpecFDF.tMultiCircleSpec.dThreshold[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[125]);
	stSpecFDF.tMultiCircleSpec.dThreshold[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[126]);
	stSpecFDF.tMultiCircleSpec.dThreshold[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[127]);
	stSpecFDF.tMultiCircleSpec.nBlobSize[0] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[128]);
	stSpecFDF.tMultiCircleSpec.nBlobSize[1] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[129]);
	stSpecFDF.tMultiCircleSpec.nBlobSize[2] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[130]);
	//
	//
	//
	//
	//std::shared_ptr<CACMISImageStainRU_YmeanCommon> pInspectBlemish_Ymean = std::make_shared<CACMISImageStainRU_YmeanCommon>();//delete,x
	std::shared_ptr<CACMISImageFastDifferenceFiltering> m_pFDF = std::make_shared<CACMISImageFastDifferenceFiltering>();
	bool nRtn = false;
	m_pFDF->SetInspectPosOffset(g_clModelData[m_nUnit].m_FDFOffsetSpec[0], g_clModelData[m_nUnit].m_FDFOffsetSpec[1], g_clModelData[m_nUnit].m_FDFOffsetSpec[2], g_clModelData[m_nUnit].m_FDFOffsetSpec[3]);
	
	nRtn = m_pFDF->Inspect((BYTE*)img, nWidth, nHeight, stSpecFDF, tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly, false, tDataSpec.eDemosaicMethod);
	if (nRtn == false)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("18"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//FDF
		return -1;
	}
	int FdfBlobCount = 0;
	FdfBlobCount = m_pFDF->GetDefectBlobCount();

	//CvFont cvfont;
	CvPoint pt;
	int scale = (nWidth > 640 ? 1 : 2);
	double nFontSize = 1 / scale;
	//CString sTemp;
	//cvInitFont(&cvfont, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_NORMAL, nFontSize, nFontSize, 0, 1, 10);
	//sTemp.Format("Ymean BlobCount : %d", FdfBlobCount);
	char strTmp[1024];
	sprintf_s(strTmp, "FDF BlobCount : %d", FdfBlobCount);
	pt.x = 50;
	pt.y = 110;
	//cvPutText(bmpImg, sTemp, pt, &cvfont, CV_BLUE);
	putText(bmpImg, strTmp, pt, 0, 1.5, Scalar(10, 10, 10), 3);


	int dFdfSpecMin = 0;
	int dFdfSpecMax = 0;
	g_clMandoInspLog[m_nUnit].m_LogBlemishFPF = FdfBlobCount;

	g_clMesCommunication[m_nUnit].m_nMesBlemish[0] = FdfBlobCount;

	//dFdfSpecMin = (_ttof(EEpromVerifyData.vMinData[41]));	//FDF
	//dFdfSpecMax = (_ttof(EEpromVerifyData.vMaxData[41]));

	dFdfSpecMax = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[17]);


	if (FdfBlobCount < dFdfSpecMin || FdfBlobCount > dFdfSpecMax)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("18"));
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[FDF] BlobCount Spec Out %d [%d~%d]"), FdfBlobCount, dFdfSpecMin, dFdfSpecMax);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_bOutputCheck[3] = false;	//STAIN FDF
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[0] = 0;
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;//STAIN FDF
		nResult = R_FAIL_STAIN;// R_FAIL_BLACK_SPOT;
							   //�̹��ҷ�
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [STAIN FDF NG]");
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [FDF BLOBCOUNT:%d]"), FdfBlobCount);
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}
		
		for (int i = 0; i < FdfBlobCount; i++)
		{
			const RECT* rt = m_pFDF->GetDefectBlobRect(i);
			//cvRectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(138, 43, 226), 2);
			rectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(138, 43, 226), 2);
			if (bAutoMode == false)
			{
				g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rt->left, rt->top, rt->right, rt->bottom, M_COLOR_BLUE, 1, FALSE);
				//vision.boxlist[CCD].addList(rt, PS_SOLID, M_COLOR_CYAN);
			}
		}
		
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[0] = 1;
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[FDF] BlobCount Spec In %d [%d~%d]"), FdfBlobCount, dFdfSpecMin, dFdfSpecMax);
		AddLog(szLog, 0, m_nUnit);
	}

	//szLog.Format("[FDF] Version %s", m_pFDF->GetVersion());
	_stprintf_s(szLog, SIZE_OF_1K, _T("[FDF] Version %s"), m_pFDF->GetVersion());
	AddLog(szLog, 0, m_nUnit, false);
	//szLog.Format("[FDF] GetDefectBlobCount %d", FdfBlobCount);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[FDF] GetDefectBlobCount  %d"), FdfBlobCount);
	AddLog(szLog, 0, m_nUnit, false);
	

	

	if (stSpecFDF.tCircleSpec.bEnableCircle || stSpecFDF.tMultiCircleSpec.bEnableMultiCircle)
	{
		int ocx = 0, ocy = 0, radx = 0, rady = 0;
		const TDefectResult* pCircleMaxResult = m_pFDF->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
		

		ocx = (int)pCircleMaxResult->dContrastMaxR;
		ocy = (int)pCircleMaxResult->dContrastMaxGb;
		radx = (int)pCircleMaxResult->dContrastMaxGr* stSpecFDF.tCircleSpec.dRadiusRatioX;
		rady = (int)pCircleMaxResult->dContrastMaxB *stSpecFDF.tCircleSpec.dRadiusRatioY;

		if (stSpecFDF.tCircleSpec.bEnableCircle)
		{
			//std::cout << "[RUYMean] ocx=" << ocx << "ocy = " << ocy << "radx = " << radx << "rady = " << rady << std::endl;
			//cvEllipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255));
			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255));
			g_clVision.DrawMOverlayCircle(m_nUnit, ocx - radx, ocy - rady, ocx + radx, ocy + rady, M_COLOR_MAGENTA, 1, FALSE);
			//vision.ellipselist[CCD].addList(CPoint(ocx - radx, ocy - rady), CPoint(ocx + radx, ocy + rady), PS_SOLID, M_COLOR_CYAN);
		}
	}
	int nCircleCount = m_pFDF->GetDefectCount(EIMAGEREGION_CIRCLE);

	g_clMandoInspLog[m_nUnit].m_LogBlemishFdfMaxCenter = m_pFDF->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishFdfMaxEdge = m_pFDF->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishFdfMaxCorner = m_pFDF->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;

	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[3] = m_pFDF->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[4] = m_pFDF->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[5] = m_pFDF->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;

	_stprintf_s(szLog, SIZE_OF_1K, _T("[FDF] Max Value (Center) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[3]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[FDF] Max Value (Edge) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[4]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[FDF] Max Value (Corner) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[5]);
	AddLog(szLog, 0, m_nUnit);
	if (g_clModelData[m_nUnit].m_nNgImageSaveUse == 1)
	{
		if (FdfBlobCount > 0)
		{
			g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 0);
		}
	}
	else
	{

		g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 0);
	}
	//cvReleaseImage(&bmpImg);
	g_SaveLGITLog(m_nUnit, "FDF", m_pFDF->GetLogHeader(), m_pFDF->GetLogData(), m_pFDF->GetVersion());
	return FdfBlobCount;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
int CAps_Insp::Blemish_YmeanInsp(BYTE* img, bool bAutoMode)
{
	
    int nResult = R_RESULT_PASS;
   
    int nBlackLevel = 0;
    //CString szLog = "";
	TCHAR szLog[SIZE_OF_1K];
    //TStainSpec tStainSpec;
    TRU_YmeanSpecN stSpecYmean;
    memset(&stSpecYmean, 0x00, sizeof(TRU_YmeanSpecN));
    int specCount = 0;
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	TDATASPEC& tDataSpec = g_clLaonGrabberWrapper[m_nUnit].dBlemish_Tdataspec;
	bool bUse8BitOnly = false;

	int index = 0;
	if (g_clModelData[m_nUnit].m_nBlemish8BitUse == 1)
	{
		bUse8BitOnly = true;
	}
	//IplImage *bmpImg = cvCreateImage(cvSize(nWidth, nHeight), 8, 3); // bmp for display
	//bmpImg->imageData=	(char*)g_clLaonGrabberWrapper[m_nUnit].m_pFrameBMPBuffer;

	ACMISSoftISP::xMakeBMP(img, (byte*)g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer,
		g_clModelData[m_nUnit].m_nWidth, g_clModelData[m_nUnit].m_nHeight, g_clLaonGrabberWrapper[m_nUnit].dTDATASPEC_n);
	Mat bmpImg = Mat(g_clModelData[m_nUnit].m_nHeight, g_clModelData[m_nUnit].m_nWidth, CV_8UC3);
	bmpImg.data = g_clLaonGrabberWrapper[m_nUnit].m_pTestBuffer;
    //----------------------------------------------------------------------
    // Spec - Ymean
    //----------------------------------------------------------------------
    //stSpecYmean.nEdgeSize = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];// 100;
    //stSpecYmean.fCenterThreshold = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//10.75;
    //stSpecYmean.fEdgeThreshold = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//15.0;
    //stSpecYmean.fCornerThreshold = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//15.0;

    //stSpecYmean.nDefectBlockSize = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//15;
    //stSpecYmean.nLscBlockSize = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//64;
    //stSpecYmean.nCalcType = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0;
    //stSpecYmean.nMaxRecursiveCount = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//1000;
    //stSpecYmean.dMaxDefectSize = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//70.0;
    //stSpecYmean.dPixelSize = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//4.2;

    //stSpecYmean.tCircleSpec.bEnableCircle = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//false;

    //stSpecYmean.tCircleSpec.nPosOffsetX = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//5;
    //stSpecYmean.tCircleSpec.nPosOffsetY = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//5;
    //stSpecYmean.tCircleSpec.dRadiusRatioX = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.45;
    //stSpecYmean.tCircleSpec.dRadiusRatioY = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.45;

    //stSpecYmean.tCircleSpec.dThresholdRatio = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.5;
    //stSpecYmean.tCircleSpec.dROIRange = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.5;
    //stSpecYmean.tCircleSpec.nUsedFixedCircle = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0;
    //stSpecYmean.tMultiCircleSpec.bEnableMultiCircle = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//false;
    //stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[0] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.2;
    //stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[1] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.4;
    //stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[2] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.52;
    //stSpecYmean.tMultiCircleSpec.dThreshold[0] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.45;
    //stSpecYmean.tMultiCircleSpec.dThreshold[1] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.45;
    //stSpecYmean.tMultiCircleSpec.dThreshold[2] = g_clModelData[m_nUnit].m_YmeanSpec[specCount++];//0.8;
    //stSpecYmean.tMultiCircleSpec.nBlobSize[0] = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];
    //stSpecYmean.tMultiCircleSpec.nBlobSize[1] = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];
    //stSpecYmean.tMultiCircleSpec.nBlobSize[2] = (int)g_clModelData[m_nUnit].m_YmeanSpec[specCount++];



	//
	stSpecYmean.nEdgeSize = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[73]);
	stSpecYmean.fCenterThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[74]);
	stSpecYmean.fEdgeThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[75]);
	stSpecYmean.fCornerThreshold = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[76]);

	stSpecYmean.nDefectBlockSize = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[77]);
	stSpecYmean.nLscBlockSize = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[78]);
	stSpecYmean.nCalcType = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[79]);
	stSpecYmean.nMaxRecursiveCount = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[80]);
	stSpecYmean.dMaxDefectSize = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[81]);
	stSpecYmean.dPixelSize = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[82]);

	index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[83]);
	if (index == 1)
	{
		stSpecYmean.tCircleSpec.bEnableCircle = true;
	}
	else
	{
		stSpecYmean.tCircleSpec.bEnableCircle = false;
	}
	stSpecYmean.tCircleSpec.nPosOffsetX = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[84]);
	stSpecYmean.tCircleSpec.nPosOffsetY = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[85]);
	stSpecYmean.tCircleSpec.dRadiusRatioX = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[86]);
	stSpecYmean.tCircleSpec.dRadiusRatioY = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[87]);

	stSpecYmean.tCircleSpec.dThresholdRatio = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[88]);
	stSpecYmean.tCircleSpec.dROIRange = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[89]);
	stSpecYmean.tCircleSpec.nUsedFixedCircle = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[90]);
	index = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[91]);
	if (index == 1)
	{
		stSpecYmean.tMultiCircleSpec.bEnableMultiCircle = true;
	}
	else
	{
		stSpecYmean.tMultiCircleSpec.bEnableMultiCircle = false;
	}

	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[92]);
	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[93]);
	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[94]);
	stSpecYmean.tMultiCircleSpec.dThreshold[0] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[95]);
	stSpecYmean.tMultiCircleSpec.dThreshold[1] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[96]);
	stSpecYmean.tMultiCircleSpec.dThreshold[2] = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[97]);
	stSpecYmean.tMultiCircleSpec.nBlobSize[0] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[98]);
	stSpecYmean.tMultiCircleSpec.nBlobSize[1] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[99]);
	stSpecYmean.tMultiCircleSpec.nBlobSize[2] = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[100]);


	//
	//
    std::shared_ptr<CACMISImageStainRU_YmeanCommon> pInspectBlemish_Ymean = std::make_shared<CACMISImageStainRU_YmeanCommon>();//delete,x
	bool nRtn = false;
	pInspectBlemish_Ymean->SetInspectPosOffset(g_clModelData[m_nUnit].m_YmeanOffsetSpec[0], g_clModelData[m_nUnit].m_YmeanOffsetSpec[1], g_clModelData[m_nUnit].m_YmeanOffsetSpec[2], g_clModelData[m_nUnit].m_YmeanOffsetSpec[3]);

	nRtn = pInspectBlemish_Ymean->Inspect((BYTE*)img, nWidth, nHeight, stSpecYmean, tDataSpec.eDataFormat, tDataSpec.eOutMode, tDataSpec.eSensorType, tDataSpec.nBlackLevel, bUse8BitOnly, false, tDataSpec.eDemosaicMethod);
	if (nRtn == false)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("17"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//Ymean
		return -1;
	}

	int YmeanBlobCount = 0;
	YmeanBlobCount = pInspectBlemish_Ymean->GetDefectBlobCount();

	//CvFont cvfont;
	CvPoint pt;
	int scale = (nWidth > 640 ? 1 : 2);
	double nFontSize = 1 / scale;
	//CString sTemp;
	//cvInitFont(&cvfont, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_NORMAL, nFontSize, nFontSize, 0, 1, 10);
	//sTemp.Format("Ymean BlobCount : %d", YmeanBlobCount);

	char strTmp[1024];
	sprintf_s(strTmp, "Ymean BlobCount : %d", YmeanBlobCount);
	pt.x = 50;
	pt.y = 110;
	//cvPutText(bmpImg, sTemp, pt, &cvfont, CV_BLUE);
	putText(bmpImg, strTmp, pt, 0, 1.5, Scalar(10, 10, 10), 3);


	int dYmeanSpecMin = 0;
	int dYmeanSpecMax = 0;
	g_clMandoInspLog[m_nUnit].m_LogBlemishRU_Ymean = YmeanBlobCount;
	g_clMesCommunication[m_nUnit].m_nMesBlemish[2] = g_clMandoInspLog[m_nUnit].m_LogBlemishRU_Ymean;

	//dYmeanSpecMin = (_ttof(EEpromVerifyData.vMinData[40]));	//Ymean
	//dYmeanSpecMax = (_ttof(EEpromVerifyData.vMaxData[40]));

	dYmeanSpecMax = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[19]);

    if (YmeanBlobCount < dYmeanSpecMin || YmeanBlobCount > dYmeanSpecMax)
    {
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("17"));
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[Ymean] BlobCount Spec Out %d [%d ~ %d]"), YmeanBlobCount, dYmeanSpecMin, dYmeanSpecMax);
		AddLog(szLog, 0, m_nUnit);

		g_clTaskWork[m_nUnit].m_bOutputCheck[3] = false;	//STAIN YMEAN
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[2] = 0;
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//STAIN YMEAN

        nResult = R_FAIL_STAIN;// R_FAIL_BLACK_SPOT;
        //�̹��ҷ�
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
        g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [STAIN YMEAN NG]");
        if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
        {
            g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [YMEAN BLOBCOUNT:%d]"), YmeanBlobCount);
            g_clMandoInspLog[m_nUnit].m_nNGCnt++;
        }
		
			for (int i = 0; i < YmeanBlobCount; i++)
			{
				const RECT* rt = pInspectBlemish_Ymean->GetDefectBlobRect(i);
				//cvRectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(138, 43, 226), 2);
				rectangle(bmpImg, cvPoint(rt->left, rt->top), cvPoint(rt->right, rt->bottom), CV_RGB(138, 43, 226), 2);
				if (bAutoMode == false)
				{
					g_clVision.DrawMOverlayBox(m_nUnit, m_nUnit, rt->left, rt->top, rt->right, rt->bottom, M_COLOR_BLUE, 1, FALSE);
					//vision.boxlist[CCD].addList(rt, PS_SOLID, M_COLOR_CYAN);
				}
			}
		
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nMesBlemishResult[2] = 1;
		_stprintf_s(szLog, SIZE_OF_1K, _T("	[Ymean] BlobCount Spec In %d [%d ~ %d]"), YmeanBlobCount, dYmeanSpecMin, dYmeanSpecMax);
		AddLog(szLog, 0, m_nUnit);
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("[RUYMean] Version %s"), pInspectBlemish_Ymean->GetVersion());
    AddLog(szLog, 0, m_nUnit, false);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[RUYMean] GetDefectBlobCount %d"), YmeanBlobCount);
    AddLog(szLog, 0, m_nUnit, false);
    

	

	if (stSpecYmean.tCircleSpec.bEnableCircle || stSpecYmean.tMultiCircleSpec.bEnableMultiCircle)
	{
		int ocx = 0, ocy = 0, radx = 0, rady = 0;
		
		const TDefectResult* pCircleMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CIRCLE);

		ocx = (int)pCircleMaxResult->dContrastMaxR;
		ocy = (int)pCircleMaxResult->dContrastMaxGb;
		radx = (int)pCircleMaxResult->dContrastMaxGr* stSpecYmean.tCircleSpec.dRadiusRatioX;
		rady = (int)pCircleMaxResult->dContrastMaxB *stSpecYmean.tCircleSpec.dRadiusRatioY;

		if (stSpecYmean.tCircleSpec.bEnableCircle)
		{
			//std::cout << "[RUYMean] ocx=" << ocx << "ocy = " << ocy << "radx = " << radx << "rady = " << rady << std::endl;
			//cvEllipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255));
			ellipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255));
			g_clVision.DrawMOverlayCircle(m_nUnit, ocx - radx, ocy - rady, ocx + radx, ocy + rady, M_COLOR_MAGENTA, 1, FALSE);
			//vision.ellipselist[CCD].addList(CPoint(ocx - radx, ocy - rady), CPoint(ocx + radx, ocy + rady), PS_SOLID, M_COLOR_CYAN);
		}
	}

	int nCircleCount = pInspectBlemish_Ymean->GetDefectCount(EIMAGEREGION_CIRCLE);

	g_clMandoInspLog[m_nUnit].m_LogBlemishYmeanMaxCenter = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishYmeanMaxEdge = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMandoInspLog[m_nUnit].m_LogBlemishYmeanMaxCorner = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;

	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[6] = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CENTER)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[7] = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_EDGE)->dValue;
	g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[8] = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CORNER)->dValue;


	_stprintf_s(szLog, SIZE_OF_1K, _T("[Ymean] Max Value (Center) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[6]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[Ymean] Max Value (Edge) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[7]);
	AddLog(szLog, 0, m_nUnit);
	_stprintf_s(szLog, SIZE_OF_1K, _T("[Ymean] Max Value (Corner) %.6lf"), g_clMesCommunication[m_nUnit].m_nMesBlemishMaxDefect[8]);
	AddLog(szLog, 0, m_nUnit);

	if (g_clModelData[m_nUnit].m_nNgImageSaveUse == 1)
	{
		if (YmeanBlobCount > 0)
		{
			g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 2);
		}
	}
	else
	{

		g_clLaonGrabberWrapper[m_nUnit].JpgMatSave(bmpImg, 2);
	}
	//cvReleaseImage(&bmpImg);
    //CalibrationInspLog.func_LogSave_Stain("[RU_Ymean]", pInspectBlemish_Ymean->GetLogHeader(), pInspectBlemish_Ymean->GetLogData());
    g_SaveLGITLog(m_nUnit, "RUYMean", pInspectBlemish_Ymean->GetLogHeader(), pInspectBlemish_Ymean->GetLogData(), pInspectBlemish_Ymean->GetVersion());
    return YmeanBlobCount;
}
//-----------------------------------------------------------------------------
//
//	���� ���� ����
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Random_CurrentMeasure(bool bLogDraw, bool bAutoMode)
{
	//TCHAR szLog[SIZE_OF_1K];
	////double _ran = (double)rand()/ RAND_MAX;
	////double _ranCur = (82.0 + _ran * (85.0 - 82.0));
	//random_device rd;
	//mt19937 gen(rd());
	////uniform_int_distribution<int> dis(5, 10);
	//uniform_real_distribution<double> dis(355.0, 480.0);

	//double m_cur = dis(gen);

	//g_clMandoInspLog[m_nUnit].m_dCurrent = m_cur;
	//g_clMesCommunication[m_nUnit].m_dMesCurrent = m_cur;
	//g_clMesCommunication[m_nUnit].m_nMesCurrentResult = 1;

	//_stprintf_s(szLog, SIZE_OF_1K, _T("	���� ����ġ : %.03lf"), g_clMesCommunication[m_nUnit].m_dMesCurrent);
	//AddLog(szLog, 0, m_nUnit);

	return true;
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Ultra_CurrentMeasure(bool bLogDraw, bool bAutoMode)
{
	

	return false;
}


//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_CurrentMeasure(bool bAutoMode)
{
	double cur = 0.0;
	bool	bRes = true;
	TCHAR szLog[SIZE_OF_1K];

	TCHAR	szPath[SIZE_OF_1K];

	double currentValue[5];
	double dCurrentVal = 0.0;


	int errorCode = 0;
	bool bInspRtn = true;

	byte WData[1];
	int i = 0;
	byte pVolData1[8];
	memset(pVolData1, 0x00, 8);

	unsigned int nSlaveAddress = 0x41;
	unsigned int nAddress = 0x01;


	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 1, pVolData1, (unsigned short)2);	//Temperature1

	std::bitset<8> binaryByte1(pVolData1[0]);
	std::bitset<8> binaryByte2(pVolData1[1]);
	// 2���� ��ȯ
	std::bitset<16> combinedBinaryValue((binaryByte1.to_ulong() << 8) | binaryByte2.to_ulong());
	//2�� ���� ��ȯ
	std::bitset<16> twosComplement = ~combinedBinaryValue;
	twosComplement = twosComplement.to_ulong() + 1;
	//3 ~ 14 bit�����
	std::bitset<12> maskedValue = (twosComplement.to_ulong() >> 3) & ~(1 << 12);
	//���� ��ȯ
	int decimalValue = static_cast<int>(maskedValue.to_ulong());

	dCurrentVal = (decimalValue * 0.00004) / 0.47;

	g_clMesCommunication[m_nUnit].m_dMesCurrent = dCurrentVal;


	double mCurrent_Value = 0.0;
	double mCurrent_SpecMin = 0.0;
	double mCurrent_SpecMax = 0.0;

	mCurrent_Value = g_clMesCommunication[m_nUnit].m_dMesCurrent;
	//mCurrent_SpecMin = (_ttof(EEpromVerifyData.vMinData[0]));
	//mCurrent_SpecMax = (_ttof(EEpromVerifyData.vMaxData[0]));
	mCurrent_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[0]);
	mCurrent_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[1]);

	if (mCurrent_Value < mCurrent_SpecMin || mCurrent_Value > mCurrent_SpecMax)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("3"));

		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//CURRENT
		g_clMesCommunication[m_nUnit].m_nMesCurrentResult = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[CURRENT] Spec Out: %.6lf [%.3lf~%.3lf]"), mCurrent_Value, mCurrent_SpecMin, mCurrent_SpecMax);
		AddLog(szLog, 0, m_nUnit);
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [CURRENT NG]");
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG CURRENT :%.6lf [%.3lf~%.3lf]"), mCurrent_Value, mCurrent_SpecMin, mCurrent_SpecMax);
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}
		//g_clMesCommunication[m_nUnit].m_nMesRIRICornerResult[0] = 0;

		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		g_clTaskWork[m_nUnit].m_bOutputCheck[2] = false;	//OC

	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nMesCurrentResult = 1;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[CURRENT] Spec In: %.6lf [%.3lf~%.3lf]"), mCurrent_Value, mCurrent_SpecMin, mCurrent_SpecMax);
		AddLog(szLog, 0, m_nUnit);
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	ISP  Version Check
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Version_Read(bool bAutoMode)	//nMode 0 = verion,model / 1 = error flag
{
	int i = 0;
	int errorCode = 0;
	TCHAR szLog[SIZE_OF_1K];
	byte WData[1];
	CString sTemp = _T("");

	unsigned int nSlaveAddress = 0x00;
	unsigned int nAddress = 0x00;
	int nBitIndex = 0;
	const int dCheckDataCount = 2;

	int nAddrLength = 2;

	TCHAR* pszCol[] = {_T("[Nor Flash Memory] ISP Version"), _T("[Nor Flash Memory] ISP Model") };

	//0 = [Nor Flash Memory] ISP Version Check
	//1 = [Nor Flash Memory] ISP Model Check
	for (i = 0; i < dCheckDataCount; i++)			//Verify
	{
		g_clMesCommunication[m_nUnit].m_dIspData[i] = 0x00;
		g_clMesCommunication[m_nUnit].m_dIspDataResult[i] = 1;
	}
	
	int mI2c_SpecMax = 0;

	Sleep(10);

	for (i = 0; i < dCheckDataCount; i++)			//read
	{
		switch (i)
		{
		case 0:		//[Nor Flash Memory] ISP Version Check
			nSlaveAddress = 0x1A;
			nAddress = 0xC03C;
			nAddrLength = 2;
			break;
		case 1:		//[Nor Flash Memory] ISP Model Check
			nSlaveAddress = 0x1A;
			nAddress = 0xC040;
			nAddrLength = 2;
			break;
		default:
			break;
		}
		errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, WData, (unsigned short)1);	//Temperature1

		g_clMesCommunication[m_nUnit].m_dIspData[i] = WData[0];

		//mI2c_SpecMax = (_ttoi(EEpromVerifyData.vMaxData[2 + i]));
		mI2c_SpecMax = (int)g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[4 + i]);	//Version , Model ����

		if (g_clMesCommunication[m_nUnit].m_dIspData[i] == mI2c_SpecMax && errorCode == 0)
		{
			g_clMesCommunication[m_nUnit].m_dIspDataResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Verify Ok: %d[%d]"), pszCol[i], g_clMesCommunication[m_nUnit].m_dIspData[i], mI2c_SpecMax);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//I2C
			g_clMesCommunication[m_nUnit].m_dIspDataResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Verify Ng: %d[%d]"), pszCol[i], g_clMesCommunication[m_nUnit].m_dIspData[i], mI2c_SpecMax);
			AddLog(szLog, 0, m_nUnit);

			sTemp.Format("%s", szLog);

			g_clMandoInspLog[m_nUnit].m_sNGList += sTemp;
			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			//! Main Displayȭ�� Overlay NG List
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("%s"), szLog);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}
		}
		sTemp.Empty();
		Sleep(15);
	}

	if (g_clMesCommunication[m_nUnit].m_dIspDataResult[0] == 0)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("11"));
	}
	if (g_clMesCommunication[m_nUnit].m_dIspDataResult[1] == 0)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("12"));
	}
	return true;

}




//-----------------------------------------------------------------------------
//
//	Error Flag Check
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_ErrorFlag_Read(bool bAutoMode)	//nMode 0 = verion,model / 1 = error flag
{
	int i = 0;
	int errorCode = 0;
	TCHAR szLog[SIZE_OF_1K];
	byte WData[1];
	CString sTemp = _T("");

	unsigned int nSlaveAddress = 0x00;
	unsigned int nAddress = 0x00;
	int nBitIndex = 0;
	const int dCheckDataCount = 4;

	int nAddrLength = 2;
	byte VerifyData[dCheckDataCount] = {0x01 ,0x01 ,0x00 ,0x00 };

	TCHAR* pszCol[] = {
		_T("[Error Flag Check] Image Sensor") , _T("[Error Flag Check] PMIC")
		, _T("[Error Flag Check] Serializer1") , _T("[Error Flag Check] Serializer2") };


	//0 = [Error Flag Check] Image Sensor
	//1 = [Error Flag Check] PMIC
	//2 = [Error Flag Check] Serializer1
	//3 = [Error Flag Check] Serializer2
	for (i = 0; i < dCheckDataCount; i++)			//Verify
	{
		g_clMesCommunication[m_nUnit].m_dI2cData[i] = 0x00;
		g_clMesCommunication[m_nUnit].m_dI2cDataResult[i] = 1;
	}
	int mI2c_SpecMin = 0;
	int mI2c_SpecMax = 0;

	Sleep(10);

	for (i = 0; i < dCheckDataCount; i++)			//read
	{
		switch (i)
		{
		case 0:
			nBitIndex = 3;
			nSlaveAddress = 0x42;		//[Error Flag Check] Image Sensor
			nAddress = 0x02CD;
			nAddrLength = 2;
			break;
		case 1:
			nBitIndex = 3;
			nSlaveAddress = 0x42;		//[Error Flag Check] PMIC
			nAddress = 0x02D0;
			nAddrLength = 2;
			break;
		case 2:
			nBitIndex = 3;
			nSlaveAddress = 0x42;		//[Error Flag Check] Serializer1
			nAddress = 0x001D;
			nAddrLength = 2;
			break;
		case 3:
			nBitIndex = 2;
			nSlaveAddress = 0x42;		//[Error Flag Check] Serializer2
			nAddress = 0x001F;
			nAddrLength = 2;
			break;
		default:
			break;
		}
		errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, WData, (unsigned short)1);	//Temperature1



		byte bitData = (WData[0] & (1 << nBitIndex)) >> nBitIndex;
		g_clMesCommunication[m_nUnit].m_dI2cData[i] = bitData;

		//bit���
		
		//ndata = (WData[0] & (1 << nBitIndex)) >> nBitIndex;
		//_stprintf_s(szLog, SIZE_OF_1K, _T("Data :%d"), ndata);
		//AddLog(szLog, 0, 0);
		//return (data & (1 << n��°)) >>n��°;
		//data <<= n��°;
		//data >>=7;
		//return data



		if (g_clMesCommunication[m_nUnit].m_dI2cData[i] == VerifyData[i] && errorCode == 0)
		{
			g_clMesCommunication[m_nUnit].m_dI2cDataResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Verify Ok: %02X[%02X]"), pszCol[i], g_clMesCommunication[m_nUnit].m_dI2cData[i], VerifyData[i]);
			AddLog(szLog, 0, m_nUnit);
		}
		else
		{
			g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("6"));	//7,8,9 ������ 6���� ���� ������
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//I2C
			g_clMesCommunication[m_nUnit].m_dI2cDataResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Verify Ng: %02X[%02X]"), pszCol[i], g_clMesCommunication[m_nUnit].m_dI2cData[i], VerifyData[i]);
			AddLog(szLog, 0, m_nUnit);

			sTemp.Format("%s", szLog);

			g_clMandoInspLog[m_nUnit].m_sNGList += sTemp;
			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			//! Main Displayȭ�� Overlay NG List
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("%s"), szLog);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}
		}


		sTemp.Empty();
		Sleep(15);
	}
	
	return true;

}



//-----------------------------------------------------------------------------
//
//	SENSOR VOLTAGE �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Sensor_Voltage_Read(bool bAutoMode)
{
	int errorCode = 0;
	CString sTemp = _T("");
	byte WData[5];
	int i = 0;
	TCHAR szLog[SIZE_OF_1K];
	TCHAR* pszCol[] = { _T("[AVDD]"), _T("[OVDD]") , _T("[DVDD]") };

	unsigned int nSlaveAddress = 0x1A;
	unsigned int nAddress = 0x0000;


	double voltageValue = 0.0;
	double mvoltage_SpecMin = 0.0;
	double mvoltage_SpecMax = 0.0;

	//Read�� [12]bits�� 10���� ��ȯ
	//interface Voltage�� = ��ȯ�� (mV)
	//mV -> V�� ��ȯ�Ͽ� Log ���

	Sleep(10);
	for (i = 0; i < 3; i++)			//read
	{
		switch (i)
		{
		case 0:		//AVDD
			nSlaveAddress = 0x1A;
			nAddress = 0x1E40;
			break;
		case 1:		//OVDD
			nSlaveAddress = 0x1A;
			nAddress = 0x1E42;
			break;
		case 2:		//DVDD
			nSlaveAddress = 0x1A;
			nAddress = 0x1E44;
			break;
		default:
			break;
		}
		errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, WData, (unsigned short)2);

		//WData[0] = 0xfd;
		//WData[1] = 0x0c;
		// �� ����Ʈ�� 12��Ʈ ���ڷ� ����
		// wdata[0]�� ���� 8��Ʈ, wdata[1]�� ���� 4��Ʈ�� ����
		//int twelveBitValue = (WData[1] << 4) | (WData[0] & 0x0F);
		int twelveBitValue = (WData[1] << 8) | WData[0];


		g_clMesCommunication[m_nUnit].m_dMesSensorVoltage[i] = twelveBitValue / 1000.0;

		//mvoltage_SpecMin = (_ttof(EEpromVerifyData.vMinData[42 + i]));
		//mvoltage_SpecMax = (_ttof(EEpromVerifyData.vMaxData[42 + i]));
		mvoltage_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[20 + (2 * i)]);
		mvoltage_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[20 + (2 * i + 1)]);



		voltageValue = g_clMesCommunication[m_nUnit].m_dMesSensorVoltage[i];

		if (voltageValue < mvoltage_SpecMin || voltageValue > mvoltage_SpecMax || errorCode != 0)
		{
			g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("5"));
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;		//I2C
			g_clMesCommunication[m_nUnit].m_nMesSensorVoltageResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Spec Out: %.3lf[%.3lf ~ %.3lf]"), pszCol[i], voltageValue, mvoltage_SpecMin, mvoltage_SpecMax);
			AddLog(szLog, 0, m_nUnit);

			sTemp.Format("%s", szLog);

			g_clMandoInspLog[m_nUnit].m_sNGList += sTemp;
			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			//! Main Displayȭ�� Overlay NG List
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("%s"), szLog);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}

		}
		else
		{

			g_clMesCommunication[m_nUnit].m_nMesSensorVoltageResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Spec In: %.3lf[%.3lf ~ %.3lf]"), pszCol[i], voltageValue, mvoltage_SpecMin, mvoltage_SpecMax);
			AddLog(szLog, 0, m_nUnit);
		}
		sTemp.Empty();
		Sleep(15);
	}
	sTemp.Empty();
	return false;
}

//-----------------------------------------------------------------------------
//
// SUPPLY	VOLTAGE �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Supply_Voltage_Read(bool bAutoMode)
{
	double cur = 0.0;
	//SHM �輺�� å�Ӹ� 2312115 �߰�												//lim240430
	//Des Port A = 0x40 , B = 0x40 , C = 0x41 , D = 0x41 
	//Register Address A = 0x04 , B = 0x02 , C = 0x04 , D = 0x02 
	bool	bRes = false;
	int errorCode = 0;
	bool bInspRtn = true;

	byte WData[1];
	int i = 0;
	byte pVolData1[8];
	memset(pVolData1, 0x00, 8);

	unsigned int nSlaveAddress = 0x41;
	unsigned int nAddress = 0x02;
	unsigned short kkkk = 0x0000;

	Sleep(10);

	//errorCode = MIU.m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 1, pVolData1, (unsigned short)2);	//Temperature1
	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 1, pVolData1, (unsigned short)2);	//Temperature1

	//[14-3]bits�� 10������ ��ȯ
	//V_bus = ��ȯ �� * 0.008;

	int decimalValue = (pVolData1[0] << 8) | pVolData1[1];
	int sumData = (decimalValue >> 3) & ~(1 << 12);

	CString sTemp;
	double dVoltageVal = sumData * 0.008;
	g_clMesCommunication[m_nUnit].m_dMesVoltage = dVoltageVal;




	double mVoltageValue = g_clMesCommunication[m_nUnit].m_dMesVoltage;
	double mVoltage_SpecMin = 0.0;
	double mVoltage_SpecMax = 0.0;


	//mVoltage_SpecMin = (_ttof(EEpromVerifyData.vMinData[1]));	//11.5
	//mVoltage_SpecMax = (_ttof(EEpromVerifyData.vMaxData[1]));	//12.5
	mVoltage_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[2]);
	mVoltage_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[3]);
	//g_clMandoInspLog[m_nUnit].dVoltage = dVoltageVal;

	
	if (mVoltageValue < mVoltage_SpecMin || mVoltageValue > mVoltage_SpecMax)
	{
		g_clMesCommunication[m_nUnit].m_nMesVoltageResult = 0;
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("4"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//VOLTAGE//XX
		sTemp.Format("Voltage Spec NG: %.6lf [%.3lf~%.3lf]", mVoltageValue, mVoltage_SpecMin, mVoltage_SpecMax);
		//! Main Displayȭ�� Overlay NG List
		g_clMandoInspLog[m_nUnit].sDispNG[g_clMandoInspLog[m_nUnit].iNGCnt].Format("Voltage fail: %.6lf [%.3lf~%.3lf]", mVoltageValue, mVoltage_SpecMin, mVoltage_SpecMax);
		g_clMandoInspLog[m_nUnit].iNGCnt++;
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		AddLog(sTemp, 0, m_nUnit);
		g_clMandoInspLog[m_nUnit].m_sNGList += sTemp;
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nMesVoltageResult = 1;
		sTemp.Format("Voltage Spec IN: %.6lf [%.3lf~%.3lf]", mVoltageValue, mVoltage_SpecMin, mVoltage_SpecMax);
		AddLog(sTemp, 0, m_nUnit);  //theApp.MainDlg->putListLog(sTemp);
	}

	sTemp.Empty();
	return false;
}

//-----------------------------------------------------------------------------
//
//	TEMPERATURE �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Temperature(bool bAutoMode)
{
	TCHAR szLog[SIZE_OF_1K];
	int errorCode = 0;
	bool bInspRtn = true;

	byte WData[1];

	unsigned char pTemData1[6];
	memset(pTemData1, 0x00, 6);
	unsigned char pTemData2[2];
	memset(pTemData2, 0x00, 2);
	unsigned char pTemData3[2];
	memset(pTemData3, 0x00, 2);

	int	m_Str_DataLength = 2;

	unsigned int nSlaveAddress = 0x49;	//read 0x48(�µ�����1), 0x49(�µ�����2) , write 0x44 , read 0x44 (�½������� = write �� read)
	unsigned int nAddress = 0x00;		//(Address: 0x00 , 0x00,  0x2C) 
	////
	CString strTemp;
	CString m_Str_Data_Hex = _T("");
	CString rtnString;
	int tem1 = 0;
	int tem2 = 0;
	float tem3 = 0.0;

	WData[0] = 0x06;
	Sleep(10);
	errorCode += g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)0x44, (unsigned short)0x2C, 1, WData, (unsigned short)1);
	Sleep(20);
	errorCode += g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)0x44, (unsigned short)0x2C, 1, pTemData1, (unsigned short)6);	//Temperature1
	Sleep(1);
	errorCode += g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)0x48, (unsigned short)0x00, 1, pTemData2, (unsigned short)2);	//Temperature2
	Sleep(1);
	errorCode += g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)0x49, (unsigned short)0x00, 1, pTemData3, (unsigned short)2);	//Temperature3
	Sleep(1);
	if (errorCode)
	{
		m_Str_Data_Hex.Format("Error : %d", errorCode);
		return false;
	}
	//Slave Address = 0x44
	//Polynomial : 0x31 (x8*x5*x4+1)

	BYTE startCrc = 0xFF;
	BYTE* tmpArrPacket = new BYTE[4];
	int nLength = 2;
	tmpArrPacket[0] = pTemData1[0];// 0xBE;
	tmpArrPacket[1] = pTemData1[1];// 0xEF;
	startCrc = g_clApsInsp[m_nUnit].CRC_CalculateCRC8(startCrc, tmpArrPacket, nLength);

	if (pTemData1[2] == startCrc)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] TempReg CRC OK R:%d / C:%d"), pTemData1[2], startCrc);
		AddLog(szLog, 0, m_nUnit);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] TempReg CRC FAIL R:%d / C:%d"), pTemData1[2], startCrc);
		AddLog(szLog, 1, m_nUnit);
		bInspRtn = false;
	}
	

	startCrc = 0xFF;
	tmpArrPacket[0] = pTemData1[3];// 0xBE;
	tmpArrPacket[1] = pTemData1[4];// 0xEF; 
	startCrc = g_clApsInsp[m_nUnit].CRC_CalculateCRC8(startCrc, tmpArrPacket, nLength);
	if (pTemData1[5] == startCrc)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] Humidity CRC OK R:%d / C:%d"), pTemData1[5], startCrc);
		AddLog(szLog, 0, m_nUnit);
	}
	else
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] Humidity CRC FAIL R:%d / C:%d"), pTemData1[5], startCrc);
		AddLog(szLog, 1, m_nUnit);
		bInspRtn = false;
	}
	

	//Temperature = -45 + 175 * (Temp_reg / (2�� 16�� - 1))
	//Humidity = 100 * (Humi_reg / (2�� 16�� - 1))
	//2�� 16�� = 65536
	//tem1 = pTemData2[0];
	unsigned short t1Temp = 0;
	t1Temp = (unsigned short)((pTemData1[0] << 8) | pTemData1[1]); 
	g_clMandoInspLog[m_nUnit].m_dTemperatureResult[0] = -45.0 + 175.0 * ((double)t1Temp / (65536 - 1));
	//double Temperature = -45 + 175 * ((unsigned short)((Temp_reg[0] << 8) | Temp_reg[1]) / (double)(65535)-1);
	

	t1Temp = (unsigned short)((pTemData1[3] << 8) | pTemData1[4]);
	g_clMandoInspLog[m_nUnit].m_dHumidity = 100.0 * ((double)t1Temp / (65536 - 1));
	//double Humidity = 100 * ((unsigned short)((Humi_reg[0] << 8) | Humi_reg[1]) / (double)(65535)-1);
	/*
	Temp_reg 2Byte, CRC 1byte,
	Humi_reg 2Byte, CRC 1byte
	CRC �˻� ������ �� �Ʒ� �������� �½��� ����
	Temperature = -45 + 175 * (Temp_reg / (216-1))
	Humidity = 100 * (Humi_reg / (216 - 1) 
	*/

	//Slave Address = 0x48
	tem1 = pTemData2[0];
	pTemData2[1] = ((unsigned char)0xFF & (pTemData2[1] >> 4));
	tem2 = pTemData2[1];

	g_clMandoInspLog[m_nUnit].m_dTemperatureResult[1] = tem1 + (tem2 * 0.0625);

	//Slave Address = 0x49
	tem1 = pTemData3[0];
	pTemData3[1] = ((unsigned char)0xFF & (pTemData3[1] >> 4));
	tem2 = pTemData3[1];

	g_clMandoInspLog[m_nUnit].m_dTemperatureResult[2] = tem1 + (tem2 * 0.0625);


	double dTempValue = 0.0;
	double dMinSpec = 0.0;
	double dMaxSpec = 0.0;

	g_clMesCommunication[m_nUnit].m_dMesTemperature[0] = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[0];
	g_clMesCommunication[m_nUnit].m_dMesTemperature[1] = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[1];
	g_clMesCommunication[m_nUnit].m_dMesTemperature[2] = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[2];
	g_clMesCommunication[m_nUnit].m_dMesTemperature[3] = g_clMandoInspLog[m_nUnit].m_dHumidity;


	g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[0] = 1;
	g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[1] = 1;
	g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[2] = 1;
	g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[3] = 1;

	dMinSpec = g_clModelData[m_nUnit].m_TemperatureSpec[0];
	dMaxSpec = g_clModelData[m_nUnit].m_TemperatureSpec[1];
	dTempValue = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[0];

	if (dTempValue < dMinSpec || dTempValue > dMaxSpec)
	{
		//NG
		g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[0] = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #1 NG: %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
		bInspRtn = false;
	}
	else
	{
		//OK
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #1 OK : %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
	}
	dMinSpec = g_clModelData[m_nUnit].m_TemperatureSpec[2];
	dMaxSpec = g_clModelData[m_nUnit].m_TemperatureSpec[3];
	dTempValue = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[1];

	if (dTempValue < dMinSpec || dTempValue > dMaxSpec)
	{
		//NG
		g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[1] = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #2 NG: %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
		bInspRtn = false;
	}
	else
	{
		//OK
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #2 OK : %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
	}
	dMinSpec = g_clModelData[m_nUnit].m_TemperatureSpec[4];
	dMaxSpec = g_clModelData[m_nUnit].m_TemperatureSpec[5];
	dTempValue = g_clMandoInspLog[m_nUnit].m_dTemperatureResult[2];

	if (dTempValue < dMinSpec || dTempValue > dMaxSpec)
	{
		//NG
		g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[2] = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #3 NG: %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
		bInspRtn = false;
	}
	else
	{
		//OK
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Temperature] #3 OK : %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
	}

	dMinSpec = g_clModelData[m_nUnit].m_TemperatureSpec[6];
	dMaxSpec = g_clModelData[m_nUnit].m_TemperatureSpec[7];
	dTempValue = g_clMandoInspLog[m_nUnit].m_dHumidity;

	if (dTempValue < dMinSpec || dTempValue > dMaxSpec)
	{
		//NG
		g_clMesCommunication[m_nUnit].m_dMesTemperatureResult[3] = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Humidity] NG: %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
		bInspRtn = false;
	}
	else
	{
		//OK
		_stprintf_s(szLog, SIZE_OF_1K, _T("[Humidity] OK : %f(%.2lf~%.2lf)"), dTempValue, dMinSpec, dMaxSpec);
		AddLog(szLog, 0, m_nUnit);
	}


	if (bInspRtn == false)
	{
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;//XX
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [Temperature NG]");
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [Temperature]"));
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//	FUSE ID READ
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_3M_Register(bool mDarkMode)
{
	TCHAR szLog[SIZE_OF_1K];
	int errorCode = 0;
	byte WData[1];

	unsigned int nSlaveAddress = 0x1A;/// 0x50;  231110 ����
	unsigned int nAddress1 = 0x8A80;
	unsigned int nAddress2 = 0xBF61;
	if (mDarkMode == true)
	{
		//Dark Test
		WData[0] = 0x00;//HCG Mode
	}
	else
	{
		//White Test
		WData[0] = 0x01;//LCG Mode
	}
	
	//��������(0x301A)�� 5���� bit�� 1�� set����� Read����
	//0x301A Register(0x10DC --> 0x10FC)
	//Slave ID : 0x10
	Sleep(15);
	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress1, 2, WData, (unsigned short)1);
	if (errorCode)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[0x8A80] Write Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("[0x8A80] Set = %d"), WData[0]);
	AddLog(szLog, 0, m_nUnit);
	Sleep(15);
	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress2, 2, WData, (unsigned short)1);
	if (errorCode)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[0xBF61] Write Fail"));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	_stprintf_s(szLog, SIZE_OF_1K, _T("[0xBF61] Set = %d"), WData[0]);
	AddLog(szLog, 0, m_nUnit);
	return true;
}


//-----------------------------------------------------------------------------
//
//	FUSE ID READ
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Test_FuseIDRead(bool bAutoMode)
{
	TCHAR szLog[SIZE_OF_1K];
	int errorCode = 0;
	byte WData[1];

	unsigned char pData[MAX_PATH];
	memset(pData, 0x00, sizeof(pData));

	BYTE	m_Str_DataLength = (byte)8;

	unsigned int nSlaveAddress = 0x10;
	unsigned int nAddress = 0x31F4;		//(Address: 0x31F4 ~ 0x31FA) 
										//���� �����͸� �Ʒ��� ���� 16���� ���ڿ��� �����.
	g_clMesCommunication[m_nUnit].m_nMesI2CResult = 1;
	CString m_Str_Data_Hex = _T("");

	g_clMesCommunication[m_nUnit].m_sMesfuseID.Format(_T("EMPTY"));

	WData[0] = 0xFC;
	//��������(0x301A)�� 5���� bit�� 1�� set����� Read����
	//0x301A Register(0x10DC --> 0x10FC)
	//Slave ID : 0x10
	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)0x10, (unsigned short)0x301B, 2, WData, (unsigned short)1);
	Sleep(20);
	//errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)0x301B, 2, pData, (unsigned short)1);

	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, pData, (unsigned short)8);
	if (errorCode)
	{
		g_clMesCommunication[m_nUnit].m_nsMesfuseIDResult = 0;
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//XX
		_stprintf_s(szLog, SIZE_OF_1K, _T("FUSE ID NG %d"), errorCode);
		AddLog(szLog, 0, m_nUnit);
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [FUSE ID NG]");
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [FUSE ID]"));
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}

		return false;
	}
	else
	{
		g_clMesCommunication[m_nUnit].m_nsMesfuseIDResult = 1;
		CString strTemp;
		for (int i = 0; i < (int)m_Str_DataLength; i++)//nReadByteCount 
		{
			strTemp.AppendFormat("%02X", (char*)pData[i]);
		}
		_stprintf_s(g_clTaskWork[m_nUnit].m_szFuseId, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)strTemp);
		g_clMesCommunication[m_nUnit].m_sMesfuseID.Format(_T("%s"), g_clTaskWork[m_nUnit].m_szFuseId);
	}

	_stprintf_s(szLog, SIZE_OF_1K, _T("FUSE ID:%s"), g_clTaskWork[m_nUnit].m_szFuseId);
	AddLog(szLog, 0, m_nUnit);
	return true;


}

//-----------------------------------------------------------------------------
//
//	TEST I2C SENSOR READ �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Test_SensorRead(bool bAutoMode)
{
	TCHAR szLog[SIZE_OF_1K];
	int errorCode = 0;
	int i = 0;
	unsigned char pData[MAX_PATH];
	memset(pData, 0x00, sizeof(pData));

	BYTE	m_Str_DataLength = (byte)8;

	unsigned int nSlaveAddress = 0x1A;
	unsigned int nAddress = 0x7E82;		//0x765C;		//(Address: 0x765C ~ 0x7666) 

	//���� �����͸� �Ʒ��� ���� 16���� ���ڿ��� �����.
	g_clMesCommunication[m_nUnit].m_nMesI2CResult = 1;
	CString m_Str_Data_Hex = _T("");
	_stprintf_s(g_clMesCommunication[m_nUnit].m_sMesI2cSensorID, SIZE_OF_100BYTE, _T(""));


	Sleep(10);
	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, pData, (unsigned short)m_Str_DataLength);
	if (errorCode)
	{
		g_clMesCommunication[m_nUnit].m_nMesI2CResult = 0;
		m_Str_Data_Hex.Format("Error : %d", errorCode);
		return false;
	}
	else
	{
		CString strTemp = _T("");
		for (int i = 0; i < (int)m_Str_DataLength; i++)//nReadByteCount 
		{
			strTemp.AppendFormat("%02X", (char*)pData[i]);
		}
		_stprintf_s(g_clMesCommunication[m_nUnit].m_sMesI2cSensorID, SIZE_OF_100BYTE, _T("%s"), (TCHAR*)(LPCTSTR)strTemp);

		//g_clMesCommunication[m_nUnit].m_sMesI2cSensorID.Format(_T("%s"), g_clTaskWork[m_nUnit].m_szI2cSensorLot);
	}
	

	if (strcmp(g_clMesCommunication[m_nUnit].m_sMesI2cSensorID, "0000000000000000") == 0)// || strcmp(g_clTaskWork[m_nUnit].m_szI2cSensorLot, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") == 0)
	{
		g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("10"));
		g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//sensor id
		g_clMesCommunication[m_nUnit].m_nMesI2CResult = 0;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C]NG SENSOR ID:%s"), g_clMesCommunication[m_nUnit].m_sMesI2cSensorID);
		AddLog(szLog, 0, m_nUnit);
		g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [SENSOR ID NG]");
		g_clMandoInspLog[m_nUnit].m_bInspRes = false;
		if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
		{
			g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("NG [SENSOR ID]"));
			g_clMandoInspLog[m_nUnit].m_nNGCnt++;
		}
		return false;
	}
	_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C] SENSOR ID:%s"), g_clMesCommunication[m_nUnit].m_sMesI2cSensorID);
	AddLog(szLog, 0, m_nUnit);
	return true;
}
//-----------------------------------------------------------------------------
//
//	DISTORTION
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_Distortion(BYTE* img, bool bAutoMode)
{
	

	return true;
}

//-----------------------------------------------------------------------------
//
//	TEST PATTERN �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_TestPattern(bool bAutoMode)
{

	g_clTaskWork[m_nUnit].m_bOutputCheck[2] = true;//init

	TCHAR szLog[SIZE_OF_1K];
	TCHAR szPath[SIZE_OF_1K];
	int errorCode = 0;
	CFileFind clFinder;
	int readLen;
	byte nData[2];

	g_clMandoInspLog[m_nUnit].m_TpTestResult[0] = false;
	g_clMandoInspLog[m_nUnit].m_TpTestResult[1] = false;
	
	//-- Test Image ����
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			nData[0] = 0x01;
			nData[1] = 0x00;
			//nData = 0x0100;
		}
		else
		{
			nData[0] = 0x00;
			nData[1] = 0x03;
			//nData = 0x0003;
		}
		//if (m_bThreadUsed == false) return -1;  //Thread ��� ���ϴ� Pass
		//cTestPat_Ref_buf = (unsigned char*)malloc(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight);

		errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)0x10, (unsigned short)0x3070, 2, nData, (unsigned short)2);
		if (errorCode)
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C] SET WriteI2CBurst FAIL "));
			AddLog(szLog, 0, m_nUnit);
			return false;
		}
		else
		{

			Sleep(1500);
			if (memcmp(g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer_TP_Ref1[i], g_clLaonGrabberWrapper[m_nUnit].m_pFrameRawBuffer, g_clLaonGrabberWrapper[m_nUnit].m_pBoard->GetFrameRawSize()) == 0)		//��
			{
				//ok
				g_clMandoInspLog[m_nUnit].m_TpTestResult[i] = true;
				if (g_clMandoInspLog[m_nUnit].m_TpTestResult[0] == true && g_clMandoInspLog[m_nUnit].m_TpTestResult[1] == true)
				{
					g_clTaskWork[m_nUnit].m_bOutputCheck[2] = true;//i2c
					g_clMandoInspLog[m_nUnit].m_sI2C.Format("OK");
					g_clMesCommunication[m_nUnit].m_sMesI2C.Format(_T("100.00"));
					g_clMesCommunication[m_nUnit].m_nMesI2CResult = 1;

					_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C] TEST_PATTERN OK "));
					AddLog(szLog, 0, m_nUnit);
				}
			}
			else
			{
				//ng
				g_clTaskWork[m_nUnit].m_bOutputCheck[2] = false;
				g_clMandoInspLog[m_nUnit].m_TpTestResult[i] = false;
				_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C] TEST_PATTERN TEST #%d FAIL ") , i + 1);
				AddLog(szLog, 0, m_nUnit);
			}
		}
		Sleep(500);
	}

	
	nData[0] = 0x00;
	nData[1] = 0x00;

	errorCode = g_clLaonGrabberWrapper[m_nUnit].m_pBoard->WriteI2CBurst((unsigned short)0x10, (unsigned short)0x3070, 2, nData, (unsigned short)2);
	if (errorCode)
	{
		_stprintf_s(szLog, SIZE_OF_1K, _T("[I2C] INIT WriteI2CBurst FAIL "));
		AddLog(szLog, 0, m_nUnit);
		return false;
	}
	bool i2cRtn = g_clTaskWork[m_nUnit].m_bOutputCheck[2];
	return i2cRtn;
}

////////TESLA ADD
int CAps_Insp::func_CalibrateImageEmbedded(TCHAR *ptszImgPath, CString &sResult)
{
	return -1;
}
int CAps_Insp::func_EvaluateUndistortImage(TCHAR *ptszImgPath, CString &sResult)
{
	int nResult = -1;
	//DWORD dwStartTime = GetTickCount();
	//WCHAR wstrImagePath[1024];
	//int nLen = strlen(ptszImgPath) + 1;

	//MultiByteToWideChar(CP_ACP, 0, ptszImgPath, -1, wstrImagePath, nLen - 1);
	//wstrImagePath[nLen - 1] = 0;

	//EVALUATE_RESULT *ptagEvaluateResult = EvaluateUndistortImage(wstrImagePath, 36, 24, true);

	//if (ptagEvaluateResult != NULL)
	//{
	//	CString strResult = _T("Rectangle Test Result\r\n");
	//	CString strTemp;

	//	for (int i = 0; i < 12; i++)
	//	{
	//		strTemp.Format(_T("[%02d] %lf\r\n"), i, (ptagEvaluateResult->fRectangleLineResult[i]));
	//		strResult += strTemp;
	//	}

	//	strTemp.Format(_T("Col Straight Avrg : %lf\r\n"), ptagEvaluateResult->dStraightCols);
	//	strResult += strTemp;

	//	strTemp.Format(_T("Row Straight Avrg : %lf\r\n"), ptagEvaluateResult->dStraightRows);
	//	strResult += strTemp;

	//	strResult += _T("Dist (Average, Deviation, Min, Max)\r\n");

	//	strTemp.Format(_T("Col (%lf, %lf, %lf, %lf)\r\n"),
	//		ptagEvaluateResult->dColDistAverage,
	//		ptagEvaluateResult->dColDistDeviation,
	//		ptagEvaluateResult->dColDistMin,
	//		ptagEvaluateResult->dColDistMax);

	//	strResult += strTemp;

	//	strTemp.Format(_T("Row (%lf, %lf, %lf, %lf)\r\n"),
	//		ptagEvaluateResult->dRowDistAverage,
	//		ptagEvaluateResult->dRowDistDeviation,
	//		ptagEvaluateResult->dRowDistMin,
	//		ptagEvaluateResult->dRowDistMax);

	//	strResult += strTemp;

	//	DWORD dwEndTime = GetTickCount() - dwStartTime;
	//	strTemp.Format(_T("Time - %dms"), dwEndTime);

	//	strResult += strTemp;

	//	sResult = strResult;

	//	//for (auto val : ptagEvaluateResult->vecRectangleLineResult)
	//	//{
	//	//	if (val)
	//	//		delete val;
	//	//	val = NULL;
	//	//}
	//	//ptagEvaluateResult = NULL;
	//	//delete ptagEvaluateResult;
	//}
	//else
	//{
	//	CString strError;
	//	DWORD dwLastError = GetLastError();

	//	nResult = (int)dwLastError;

	//	switch (dwLastError)
	//	{
	//	case CALIBRATIONDLL_NOT_INITIALIZED:
	//		strError = _T("Blob parameters are not set.");
	//		break;
	//	case CALIBRATIONDLL_INVALID_IMAGE_PATH:
	//		strError = _T("Invalid Image Path.");
	//		break;
	//	case CALIBRATIONDLL_IMAGE_NOT_EXIST:
	//		strError = _T("Image file is not exist.");
	//		break;
	//	case CALIBRATIONDLL_EXE_NOT_EXIST:
	//		strError = _T("TeslaCal.exe is not exist.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_EXE_RUN:
	//		strError = _T("Failed to run TeslaCal.exe");
	//		break;
	//	case CALIBRATIONDLL_FAILED_CALIBRATION:
	//		strError = _T("Failed to run calibraation.");
	//		break;
	//	case CALIBRATIONDLL_INVALID_OBJECT_SIZE:
	//		strError = _T("Invalid object size.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_FIND_CIRCLE:
	//		strError = _T("Failed to find circles.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_GENERATE_AREA:
	//		strError = _T("Failed to generate areas.");
	//		break;
	//	default:
	//		strError = _T("Unknown error.");
	//		break;
	//	}
	//	sResult = strError;
	//}

	return nResult;
}
int CAps_Insp::func_EvaluateDefault(TCHAR *ptszImgPath, CString &sResult)
{
	//int nResult = -1;
	//DWORD dwStartTime = GetTickCount();
	//WCHAR wstrImagePath[1024];
	//int nLen = strlen(ptszImgPath) + 1;

	//MultiByteToWideChar(CP_ACP, 0, ptszImgPath, -1, wstrImagePath, nLen - 1);
	//wstrImagePath[nLen - 1] = 0;

	//dwStartTime = GetTickCount();

	//EVALUATE_RESULT *ptagEvaluateDefault = EvaluateUndistortDefault(wstrImagePath, 36, 24, true);
	//if (ptagEvaluateDefault != NULL)
	//{
	//	CString strResult = _T("Rectangle Test Result - Default Value\r\n");
	//	CString strTemp;

	//	for (int i = 0; i < 12; i++)
	//	{
	//		strTemp.Format(_T("[%02d] %lf\r\n"), i, (ptagEvaluateDefault->fRectangleLineResult[i]));
	//		strResult += strTemp;
	//	}

	//	strTemp.Format(_T("Col Straight Avrg : %lf\r\n"), ptagEvaluateDefault->dStraightCols);
	//	strResult += strTemp;

	//	strTemp.Format(_T("Row Straight Avrg : %lf\r\n"), ptagEvaluateDefault->dStraightRows);
	//	strResult += strTemp;

	//	strResult += _T("Dist (Average, Deviation, Min, Max)\r\n");

	//	strTemp.Format(_T("Col (%lf, %lf, %lf, %lf)\r\n"),
	//		ptagEvaluateDefault->dColDistAverage,
	//		ptagEvaluateDefault->dColDistDeviation,
	//		ptagEvaluateDefault->dColDistMin,
	//		ptagEvaluateDefault->dColDistMax);

	//	strResult += strTemp;

	//	strTemp.Format(_T("Row (%lf, %lf, %lf, %lf)\r\n"),
	//		ptagEvaluateDefault->dRowDistAverage,
	//		ptagEvaluateDefault->dRowDistDeviation,
	//		ptagEvaluateDefault->dRowDistMin,
	//		ptagEvaluateDefault->dRowDistMax);

	//	strResult += strTemp;

	//	DWORD dwEndTime = GetTickCount() - dwStartTime;
	//	strTemp.Format(_T("Time - %dms"), dwEndTime);

	//	strResult += strTemp;

	//	sResult = strResult;

	//	//for (auto val : ptagEvaluateDefault->vecRectangleLineResult)
	//	//{
	//	//	if (val)
	//	//		delete val;
	//	//	val = NULL;
	//	//}

	//	//delete ptagEvaluateDefault;
	//}
	//else
	//{
	//	CString strError;
	//	DWORD dwLastError = GetLastError();

	//	switch (dwLastError)
	//	{
	//	case CALIBRATIONDLL_NOT_INITIALIZED:
	//		strError = _T("Blob parameters are not set.");
	//		break;
	//	case CALIBRATIONDLL_INVALID_IMAGE_PATH:
	//		strError = _T("Invalid Image Path.");
	//		break;
	//	case CALIBRATIONDLL_IMAGE_NOT_EXIST:
	//		strError = _T("Image file is not exist.");
	//		break;
	//	case CALIBRATIONDLL_EXE_NOT_EXIST:
	//		strError = _T("TeslaCal.exe is not exist.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_EXE_RUN:
	//		strError = _T("Failed to run TeslaCal.exe");
	//		break;
	//	case CALIBRATIONDLL_FAILED_CALIBRATION:
	//		strError = _T("Failed to run calibraation.");
	//		break;
	//	case CALIBRATIONDLL_INVALID_OBJECT_SIZE:
	//		strError = _T("Invalid object size.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_FIND_CIRCLE:
	//		strError = _T("Failed to find circles.");
	//		break;
	//	case CALIBRATIONDLL_FAILED_GENERATE_AREA:
	//		strError = _T("Failed to generate areas.");
	//		break;
	//	default:
	//		strError = _T("Unknown error.");
	//		break;
	//	}
	//	sResult = strError;
	//}
	return -1;
}
void CAps_Insp::CRC_FinalChecksum(BYTE crcValue)
{
	crcValue ^= 0x00;
}

//-----------------------------------------------------------------------------
//
//	���� ��� �˻�
//
//-----------------------------------------------------------------------------
bool CAps_Insp::func_Insp_LightTest(int mLightIndex, bool bAutoMode)
{

	TCHAR szLog[SIZE_OF_1K];
	int i = 0;
	
	int width = 0;	
	int mStartX = 0;
	int mStartY = 0;
	int nWidth = g_clModelData[m_nUnit].m_nWidth;
	int nHeight = g_clModelData[m_nUnit].m_nHeight;
	int dRoiCount = 0;
	int dGreenBright = 0;

	CPoint m_clPtBrightPos[1];


	TCHAR* pszOcCol[] = { _T("[RI] Oc_Bright_Center") };

	m_clPtBrightPos[0].x = nWidth / 2;
	m_clPtBrightPos[0].y = nHeight / 2;

	int pos = 0;
	int mBrightValue = 0;
	double mBright_SpecMin = 0.0;
	double mBright_SpecMax = 0.0;


	TCHAR szPos[SIZE_OF_100BYTE];
	g_clVision.ClearOverlay(m_nUnit);



	for (i = 0; i < 1; i++)
	{
		width = (int)MbufInquire(g_clVision.m_MilCcdProcChild[m_nUnit][1], M_PITCH, NULL);
		pos = (m_clPtBrightPos[i].y * width) + m_clPtBrightPos[i].x;
		if (bAutoMode == false)
		{
			g_clVision.DrawMOverlayCross(m_nUnit, m_nUnit, m_clPtBrightPos[i], 20, M_COLOR_CYAN, 2);// TRUE); 
			_stprintf_s(szPos, SIZE_OF_100BYTE, _T("Roi-%d"), i);
		}

		dGreenBright = g_clVision.m_pImgBuff[m_nUnit][1][pos];
		//mBright_SpecMin = (_ttof(EEpromVerifyData.vMinData[4 + i]));	//11.5
		//mBright_SpecMax = (_ttof(EEpromVerifyData.vMaxData[4 + i]));	//12.5
		mBright_SpecMin = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[6]);
		mBright_SpecMax = g_clMesCommunication[m_nUnit].mapKeyRtn(RECIPE_PARAM_NAME[7]);

		g_clMesCommunication[m_nUnit].m_dOcBright[i] = dGreenBright;

		mBrightValue = g_clMesCommunication[m_nUnit].m_dOcBright[i];
		if (mBrightValue < mBright_SpecMin || mBrightValue > mBright_SpecMax)
		{
			g_clMesCommunication[m_nUnit].m_dEqpDefectCode.Format(_T("1"));
			g_clMesCommunication[m_nUnit].m_nMesFinalResult = 0;	//BRIGHT CHART
			g_clMesCommunication[m_nUnit].m_dOcBrightResult[i] = 0;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Spec Out :%d [%.1lf~%.1lf]"), pszOcCol[i], mBrightValue, mBright_SpecMin, mBright_SpecMax);

			g_clMandoInspLog[m_nUnit].m_sNGList += _T(" [OC BRIGHT NG]");
			g_clMandoInspLog[m_nUnit].m_bInspRes = false;
			if (g_clMandoInspLog[m_nUnit].m_nNGCnt < 30)
			{
				g_clMandoInspLog[m_nUnit].m_sDispNG[g_clMandoInspLog[m_nUnit].m_nNGCnt].Format(_T("%s"), szLog);
				g_clMandoInspLog[m_nUnit].m_nNGCnt++;
			}
		}
		else
		{
			g_clMesCommunication[m_nUnit].m_dOcBrightResult[i] = 1;
			_stprintf_s(szLog, SIZE_OF_1K, _T("%s Spec In :%d [%.1lf~%.1lf]"), pszOcCol[i], mBrightValue, mBright_SpecMin, mBright_SpecMax);
		}
		//CHART AVG
		AddLog(szLog, 0, m_nUnit, false);
	}

	g_clVision.DrawOverlayAll(m_nUnit);

	return true;
}
BYTE CAps_Insp::CRC_CalculateCRC8(BYTE Crc_StartValue8, BYTE* Crc_DataPtr, int Crc_Length)
{
	int count = 0;
	//const uint8_t crc8_table[] =
	static byte Crc8Table22[256] = 
	{
		0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53, 0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
		0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E, 0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
		0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4, 0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
		0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19, 0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
		0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40, 0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
		0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D, 0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
		0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7, 0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
		0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A, 0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
		0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75, 0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
		0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8, 0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
		0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2, 0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
		0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F, 0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
		0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66, 0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
		0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB, 0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
		0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1, 0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
		0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C, 0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4
	};

	BYTE crc_value = Crc_StartValue8;

	for (count = 0; count < Crc_Length; count++)
	{
		crc_value = crc_value ^ Crc_DataPtr[count];
		crc_value = Crc8Table22[crc_value];// &0xFF];
	}

	return crc_value;// crc_value;
}

unsigned short CAps_Insp::CRC_CalculateCRC16_CCIT(BYTE* Crc_DataPtr, unsigned int Crc_Length)
{
	static const unsigned short Crc16Table[256] =
	{
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
		0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
		0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
		0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
		0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
		0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
		0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
		0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
		0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
		0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
		0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
		0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
		0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
		0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
		0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
		0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
		0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
		0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
		0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
		0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
		0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
		0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
		0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
		0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
		0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
		0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
		0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
		0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
		0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
		0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
		0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
	};
	//unsigned short col_start;
	unsigned int i = 0;
	unsigned short crc = 0;

	for (i = 0; i < Crc_Length; i++)
	{
		crc = (crc << 8) ^ Crc16Table[((crc >> 8) ^ *Crc_DataPtr++) & 0x00FF];
	}
	return crc;
}