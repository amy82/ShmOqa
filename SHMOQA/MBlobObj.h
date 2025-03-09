#pragma once


typedef struct STRUC_FIND_BLOB		//!! Find Data 
{
	BOOL	bSearchComp;//! �˻� �Ϸ� ����
	DOUBLE	dResultX;	//! �˻� ���
	DOUBLE	dResultY;
	DOUBLE	dMarkSizeX;	//! �˻� �� MARK�� ũ��
	DOUBLE  dMarkSizeY;
	DOUBLE	dMarkRefX;	//! MARK�� �߽�
	DOUBLE	dMarkRefY;
	CRect   clRect;		// �˻��� �ܰ� ũ��
	int     nCenterX;
	int     nCenterY;
	DOUBLE	dTime;		//! ���� �ð�(ms)
	TCHAR	szFindMode[SIZE_OF_100BYTE];
} STRUC_FIND_BLOB;


class CMBlobObj
{
public:
	CMBlobObj(void);
	~CMBlobObj(void);

public:
	bool InitializeBlob(MIL_ID clMSystem);
	bool ReleaseBlob();

public:
	bool FindBlob(MIL_ID MilSystem,	MIL_ID MinBinImg, int nIndex, CRect clRect,	INT nThreshold, INT	nMinX, INT nMinY, INT nMaxX, INT nMaxY, STRUC_FIND_BLOB& stFindBlob, INT nOffsetX = 0, INT nOffsetY = 0);

public:
	LONG GetBlobCount();
	bool GetBlobMinMax(CDMPoint &dpMin, CDMPoint &dpMax);
	bool GetBlobSelect(double dCenX, double dCenY, STRUC_FIND_BLOB& stFindBlob);

private:
	MIL_ID	m_MilFeature;			//feature
	MIL_ID	m_MilBlobResult;
};

