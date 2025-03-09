#pragma once

#include "Library/Inspection/Include/LibACMISCommon/ACMISCommon.h"
#include "Library/Inspection/Include/LibACMISSoftISP/SoftISP.h"
#include "Library/Inspection/Include/LibACMISResolution/ACMISResolutionSFR.h"
#include "Library/Inspection/Include/LibACMISShading/ACMISShadingDef.h"
#include "Library/Inspection/Include/LibACMISShading/ACMISShadingAlgorithm.h"
#include "Library/Inspection/Include/LibACMISResolution/ACMISFiducialMark.h"

#include "Library/Inspection/Include/LibACMISCertification/ACMISCertification.h"
#include "Library/Inspection/Include/LibACMISCertification/ACMISCertificationDef.h"


#include "Library/Inspection/Include/LibACMISBlemish/ACMISImageBlemish.h"
#include "Library/Inspection/Include/LibACMISBlemish/ACMISImageStain.h"
#include "Library/Inspection/Include/LibACMISBlemish/ACMISBlemishDef.h"


#include "Library/Inspection/Include/LibACMISEtc/ACMISEtcAlgorithm.h"
#include "Library/Inspection/Include/LibACMISEtc/ACMISEtcDef.h"
#include "Library/Inspection/Include/LibACMISSoftISPBasic/RawImageProcessing.h"


typedef struct __TStainSpec
{
    TBlackSpotContrastN stSpecBlackSpot;
    TLCBSpecN stSpecLCB;
    TRU_YmeanSpecN stSpecYmean;
} TStainSpec;

class CAps_Insp
{
public:
	CAps_Insp(void);
	~CAps_Insp(void);
public:
	int mInspRetryCount;
	bool Func_FinalImageSave(BYTE* RawImage);	//mMode = 0 (CHART) , 1 = STAIN
	//PPL
	bool PPL_Func_Chart_Insp(BYTE* img, bool bAutoMode = false);
	bool PPL_Func_OC_Insp(BYTE* img, bool bAutoMode = false);
public:
	void SetUnit(int nUnit){ m_nUnit = nUnit;}

	//-------------------------------------------------------------------------------------------------------
	//RI
	//Relative Illumination (RI)
	bool func_Insp_Shm_Illumination(BYTE* img, bool bAutoMode = false);

	//-------------------------------------------------------------------------------------------------------
	//COLOR REPRODUCTION

	//
	//-------------------------------------------------------------------------------------------------------
	//
	//1. Relative Illumination - Color Shading (5000k , Dark �ι� ����)
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_ColorShading(BYTE* img, int index, bool bAutoMode = false);

	//-------------------------------------------------------------------------------------------------------
	//
	//2.Color Uniformity - Color Uniformity (5000K )
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_Uniformity(BYTE* img, int index, bool bAutoMode = false);
	//-------------------------------------------------------------------------------------------------------
	//
	//3.SNR & Row / Col. Noise - FPN (5000 , dark)
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_SnrRowColNoise(BYTE* img, int index, bool bAutoMode = false);
	//-------------------------------------------------------------------------------------------------------
	//
	//4.Blemish - LCB , Ymean , FDF
	//
	//-------------------------------------------------------------------------------------------------------
	int BlackSpotInsp(BYTE* img, int nWidth, int nHeight, TDATASPEC& tDataSpec, IplImage* bmpImg, int mRetryCount);
	int LCBInsp(BYTE* img,  int mRetryCount = 0, bool bAutoMode = false);
	int Blemish_YmeanInsp(BYTE* img, bool bAutoMode = false);
	int FDFInsp(BYTE* img, bool bAutoMode = false);

	//-------------------------------------------------------------------------------------------------------
	//
	//5.Defect - All Defect Pixel (rivian)
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_Defect(BYTE* lowImage, BYTE* midImage,bool bAutoMode = false);
	//-------------------------------------------------------------------------------------------------------
	//
	//6.White Balance (5000K , 3000K �ι��˻�) - Color Sensitivity
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_WhiteBalance(BYTE* img, int index, bool bAutoMode = false);
	//-------------------------------------------------------------------------------------------------------
	//
	//7.Lens Shading - Relative Illumination X 3000k , 5000k �ι� 221��
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_LensShading(BYTE* img, int index, bool bAutoMode = false);

	//-------------------------------------------------------------------------------------------------------
	//
	//8.Dark Pixel Uniformity(dark) - Color Shading
	//
	//-------------------------------------------------------------------------------------------------------
	//1���ϰ� ����
	//-------------------------------------------------------------------------------------------------------
	//
	//9.White Balance (dark)
	//
	//-------------------------------------------------------------------------------------------------------
		//6���� ����
	//-------------------------------------------------------------------------------------------------------
	//
	//10.Row/Col. Noise
	//
	//-------------------------------------------------------------------------------------------------------
	//3���� ����
	bool func_Insp_LightTest(int mLightIndex, bool bAutoMode = true);	//0 = ��Ʈ , 1,2,3,4 = �̹�����
	//-------------------------------------------------------------------------------------------------------
	//
	//11.IR Filter (IR)
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_Insp_IRFilterTest(const BYTE* img, bool bAutoMode = false);//IR Filter IR ���� ��� �˻�
	//

	//-------------------------------------------------------------------------------------------------------
	//
	//EEPROM
	//
	//-------------------------------------------------------------------------------------------------------
	bool func_EEprom_CheckSum_Check(bool bAutoMode);
	bool func_EEprom_Write(bool bAutoMode);

	bool func_EEprom_FuseID(bool bAutoMode);
	//
	//End
	//

	int func_EEprom_Parse(TCHAR *ptszLot);
	bool func_Insp_3M_Register(bool mDarkMode);
	bool func_Insp_Test_FuseIDRead(bool bAutoMode);
	bool func_Insp_Test_SensorRead(bool bAutoMode);
	
	bool func_Insp_Ultra_CurrentMeasure(bool bLogDraw = true, bool bAutoMode = false);			//���� ���� �˻�
	bool func_Insp_CurrentMeasure(bool bAutoMode = false);			//���� ���� �˻�
	

	bool func_Insp_Stain(BYTE* img, bool bAutoMode = false, int mRCount = 0, bool bUse8BitOnly = false);
	bool func_Insp_Shm_Fov_Distortion(BYTE* img, bool bAutoMode = false);

	bool func_Insp_Fov_Distortion_Rotate(BYTE* img, int index, bool bAutoMode = false, bool bUvShot = true);//index = ������ũ������ , fov��ũ ������ ����
	
	bool func_Insp_RotateTilt(BYTE* img,bool bAutoMode = false);
	bool FnShmEdgeFind(BYTE* img, bool bAutoMode = false);
	bool FnShmCornerFind(BYTE* img, bool bAutoMode = false);
	bool FnShmFastCornerFind(BYTE* img, bool bAutoMode = false);

	bool func_Insp_Illumination(BYTE* img, bool bAutoMode = false, bool bUse8BitOnly = false);//lim 240430

	int Inspect_RelativeIllumination(const BYTE* pBuffer, int nFieldIndex, int nImageWidth, int nImageHeight, TRelativeIlluminationSpecN& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRI, bool bUse8BitOnly = false);
	int Inspect_RelativeIlluminationX(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeIlluminationSpecX& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRI, bool bUse8BitOnly = false); //lim 240430
	int Inspect_RelativeUniformity(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeUniformitySpec& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRU, bool bUse8BitOnly = false);
	int Inspect_ColorSensitivity(const BYTE* pBuffer, bool bUse8BitOnly = false);
	

	
	//SNR
	bool func_Insp_Snr(BYTE* img,bool bAutoMode = false);
	void DisplaySNRGraphics(IplImage* _cvImgBuf, const TSNRResult* _SNRResult, int _nImageWidth, CvScalar color);
	
	int func_ModelLotCheck(TCHAR *ptszImgPath);
	
	CString SetDir_Check(CString sPath);
	
	

	//�����ϴ� �ڵ�?
	bool func_Insp_Distortion(BYTE* img, bool bAutoMode = false);
	bool func_Insp_ErrorFlag_Read(bool bAutoMode = true);
	bool func_Insp_Version_Read(bool bAutoMode = true);
	
	bool func_Insp_Supply_Voltage_Read(bool bAutoMode);				
	bool func_Insp_Sensor_Voltage_Read(bool bAutoMode);				
	bool  func_Insp_TestPattern(bool bAutoMode = false);				//Test Pattern �˻�

	bool func_Random_CurrentMeasure(bool bLogDraw, bool bAutoMode);
	int func_CalibrateImageEmbedded(TCHAR *ptszImgPath, CString &sResult);
	int func_EvaluateUndistortImage(TCHAR *ptszImgPath, CString &sResult);
	int func_EvaluateDefault(TCHAR *ptszImgPath, CString &sResult);
	int func_Insp_Ic(const BYTE* img);
	bool func_Insp_Saturation(const BYTE* img);//Saturation rgb ���� ��� �˻�
	//Color Reproduction
	bool func_Insp_Color_reproduction(const BYTE* img, bool bAutoMode = false);
	//Temperature 3�� ���� ��� �� Read/ �µ��� �ʹ� ���ų� ���� �÷� ����
	bool func_Insp_Temperature(bool bAutoMode);

	BYTE CRC_CalculateCRC8( BYTE Crc_StartValue8, BYTE* Crc_DataPtr, int Crc_Length);
	unsigned short CRC_CalculateCRC16_CCIT(BYTE* Crc_DataPtr, unsigned int Crc_Length);
	void CRC_FinalChecksum(BYTE crcValue);

private:
	int m_nUnit;
};
