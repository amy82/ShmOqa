//-----------------------------------------------------------------------------
//
//	GlobalDeclare.h
//
//-----------------------------------------------------------------------------
#ifndef __GLOBALDECLARE_H__
#define __GLOBALDECLARE_H__

#include "AutoInspDlg.h"
#include "MessageAskDlg.h"
#include "MessageModalDlg.h"
#include "MessagePopupDlg.h"
#include "KeyPadDlg.h"
#include "KeyBoardDlg.h"
#include "Data.h"
#include "AXTDefine.h"
#include "AxlMotorSet.h"
#include "DioControl.h"
#include "Vision.h"
//#include "MBlobObj.h"
#include "ModelFinder.h"
//#include "ADO\SimpleAdo.h"
#include "UserPath.h"
#include "InterLockDlg.h"
#include "CMSMESSocket.h"

#include "Grabber\PRI\PriGrabberWrapper.h"
#include "LaonGrabberWrapper.h"//"Grabber\LAON\LaonGrabberWrapper.h"
#include "Algorithm\SFR\Inspection_Alg.h"
#include "PRIFunc_Insp.h"

#include "Utility\Device\Light\LightControl.h"
#include "Utility\Device\Light\OCControl.h"

extern CAutoInspDlg* g_pCarAABonderDlg;
extern CMessagePopupDlg* g_pMessagePopupDlg[MAX_POPUP_COUNT];
extern int g_nPopupIndex;
extern CModelList ModelList;
extern CModelType g_clModelType;
extern CSystemData g_clSysData;
extern CModelData g_clModelData[2];
extern CMarkData g_clMarkData[2];
extern CTaskWork g_clTaskWork[2];

//extern CSFRTeachingData g_clSFRTeacingData;

extern CVision g_clVision;
extern CModelFinder g_clModelFinder;
extern CAxlMotorSet g_clMotorSet;
extern CDioControl	g_clDioControl;

//extern CSimpleAdo g_clAdo;

extern CLaonGrabberWrapper g_clLaonGrabberWrapper[MAX_UNIT_COUNT];

//extern CInspection_Alg g_clAlg[MAX_UNIT_COUNT];
extern CPRIFunc_Insp g_clPriInsp[MAX_UNIT_COUNT];

extern CMandoSfrSpec g_clMandoSfrSpec[MAX_UNIT_COUNT];
extern CMandoInspLog g_clMandoInspLog[MAX_UNIT_COUNT];
extern CMesCommunication g_clMesCommunication[MAX_UNIT_COUNT];

//
extern CLightControl		TopChartControl[MAX_UNIT_COUNT];			//Top Chart
extern CLightControl		BackDuLightControl[MAX_UNIT_COUNT];		//align + oc

extern CLightControl		LightLeftChartControl[MAX_UNIT_COUNT];		//Left Side Chart
extern CLightControl		LightRightChartControl[MAX_UNIT_COUNT];		//Right Side Chart



//extern CeepromData			eepromData[MAX_UNIT_COUNT];
extern CVerifyData EEpromVerifyData;

extern CInterLockDlg* InterLockDlg;

extern CCMSMESSocket* g_clCMSMESSocket;
//extern CQueue<STRUC_LOG_NODE*> g_clLogQueue;

extern bool bCurrentConnect;
extern bool MesSpecLoadCheck;

extern int MesMaxCount1;
extern int MesMaxCount2;
extern int MesMaxCount3;
extern int MesMaxCount4;
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void AddLog(CString str, int mPopupShow, int index, bool bBuzzer = false);



void LogSave(CString logStr, int nUnit);
void ResultSavePath(TCHAR* sPath, CString logStr);
//
unsigned int g_GetCurrentPath(TCHAR* pPath);
void AddLog_old(TCHAR* szLog, int nLogType, int nUnit, bool bBuzzer = false);
void g_ShowMsgPopup(CString sTitle, CString sMsg, COLORREF bgColor , int TimeClose = 0);
bool g_ShowMsgModal(CString sTitle, CString sMsg, COLORREF bgColor);
int g_ShowMsgAsk(CString sTitle, CString sMsg, COLORREF bgColor, int nAskCount, TCHAR* pszAsk[]);

bool g_GetIllumination(int nUnit, BYTE* RawImage, double &dShiftX, double &dShiftY);			//MAIN , NARROW �̹����� ����
int g_Get_AA_MTF(int nUnit, BYTE* RawImage, int nMode);

int g_GetSFR(BYTE* ChartRawImage, int nUnit, int nMode);
void InterLockMsg(CString _title, CString _msg, COLORREF bgColor, bool bUse);
int findMinMax(int index);
int g_CalcVaration();
int g_AvrGetSFR(BYTE* ChartRawImage,int mCount);
void unpackRaw12(const unsigned char *src, unsigned short *dst, int rows, int cols);
void Raw12to8(unsigned short *pBuffer, unsigned char *pBuf, int rows, int cols);

bool g_GetSfrRectROI(int nUnit, int nMode);
bool Binary_Dump_Load(int nUnit);
bool MesDataSave(int nUnit = 0);
void MesDataLoad(int nUnit = 0);

bool g_FindFovPos(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, CRect* clRectRoi);


bool g_FindCirclePos(int nUnit, unsigned char* pImage,  CRect* clRectRoi);	//int nPitch, int nSizeX, int nSizeY,
bool g_OpencvFindCirclePos(int nUnit, BYTE* pImage, CRect* clRectRoi, bool bAutoMode = true);
bool g_FindRectPos(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, int nIndex, CRect& clRect);
bool g_FindSfrRectPos(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, int nIndex, CRect clRectRoi);
bool g_FindRectPos2(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, int nIndex, CRect& clRect);

//���� ��Ī ��Ʈ ã�� �߰�
bool findSmallSfrRectPos(int nUnit, unsigned char* ucImage, int pitch, int sizeX, int sizeY, int index, CRect rcRoi);
bool findRectPosPattern(int nUnit, unsigned char* ucImage, int pitch, int sizeX, int sizeY, int index, CRect& rcRoi);
bool FieldPatternMatching(int nUnit, int iFieldNo, double &dCenterPosX, double &dCenterPosY, double &dAngle, double &dRate);
bool CenterFieldPatternMatching(int nUnit, double &dCenterPosX, double &dCenterPosY);

//


bool g_GetSfrRectRoi_Ct(int nUnit, int nMode);
bool g_FindRectPos_Ct(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, int nIndex, CRect rcRoi, CPoint clCenter);
CRect g_GetField_RectLeft(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, CRect rcRoi, CPoint clCenter);
CRect g_GetField_RectRight(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, CRect rcRoi);
CRect g_GetField_RectTop(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, CRect rcRoi);
CRect g_GetField_RectBot(int nUnit, unsigned char* pImage, int nPitch, int nSizeX, int nSizeY, CRect rcRoi);

bool g_GetPatchCenterAndSFR(int nUnit, int nIndex);
bool g_FindOpticalCenter(int nUnit);
bool g_GetOpticAxis(int nUnit, int nScale, int nTh, double& dOffsetX, double& dOffsetY);
double g_CalcImageAlign(int nUnit);
void g_CheckOc(int nUnit, bool bUvShot = false);

void g_CheckEolOc(int nUnit);
void g_CheckEolRotation(int nUnit);
void g_CheckEolLight(int nUnit, int index = 0);		//index 0 = ��Ʈ��� , 1 = �̹����� ���

bool g_GetSfrMinMaxValue(int nUnit, bool bNgDraw = false);
bool g_DiffReset();
int findMax(double sfrData[], int totalCount, int a = 0, int b = 0, int c = 0, int d = 0);
int findMin(double sfrData[], int totalCount, int a = 0, int b = 0, int c = 0, int d = 0);
double findDiffData(double sfrData[], int totalCount, int index);

int g_CheckAlignLimit(int nUnit, double dX, double dY, double dTh);
int g_CheckAlignSpec(int nUnit, int nMarkType, double dX = 0.0, double dY = 0.0, double dTh = 0.0);
//bool g_CalcLaserTilt(int nUnit, int nIndex);
bool g_CalcLaserTilt(int nUnit, double &dTx, double& dTy);
int g_CheckTiltLimit(int nUnit, int nType, double dTx, double dTy);

void g_CalcAlignData(int nCh, CDMPoint clDPtFindMark, double dAngle, double& dX, double& dY, double& dT);

double g_CalcTiltX(int nUnit);
double g_CalcTiltY(int nUnit);

double getRadianFromAng(double Angle);



void g_Shift12to16BitMode(unsigned char* p12bitRaw, unsigned char* p16bitRaw, int nWidth, int nHeight);
void g_CCCR_Interpolation(unsigned short* pData, int nWidth, int nHeight);
void g_Shift16to12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


//bool g_SaveLGITLog(int nUnit, CString name, CString title, CString data, CString ver);
bool g_SaveLGITLog(int nUnit, TCHAR *name, string title, string data, string ver);
bool g_SaveSfrLog(int nUnit, int nMode);
bool g_SaveSfrSumLog(int nUnit);
//////bool g_FinalInspLog(int nUnit);

void eeprom_Data_Sum(CString WriteData, int WritebyteLength, int type, byte* rtnData );
bool EEPROM_Binary_Dump_Save(int nUnit, byte* TotalDumpData);

void EEPROM_Temp_DumpSave(int nUnit, byte* tempDumpData);
void EEPROM_Temp_DumpLoad(int nUnit, byte* tempDumpData);
float CovertToFloat(CString data, int type);
CString CovertToHex(CString data, int type);
CString CovertToChar(CString data, int type);

double ocRtn(double orgData);


void RawDataDelete(int nUnit);
void fileDelete(CString delFolder);
#endif