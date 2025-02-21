#pragma once


// UXGraph

//! �׷��� ��Ʈ�� Ŭ���� ����
#define BUFFER_SIZE_GRAPH_2D		3
#define BUFFER_SIZE_GRAPH_OVERLAP	5
#define BUFFER_SIZE_PIXEL_GRAPH		1024//8192

class CGraph2DWnd;

class UXGraph : public CWnd
{
	DECLARE_DYNAMIC(UXGraph)

public:
	UXGraph();
	virtual ~UXGraph();

protected:
	DECLARE_MESSAGE_MAP()
	//! �׷��� Ŀ���� ������ �ִ� �׷����� index, 0 based
	int m_aiIndex_Overlap_Cursor[2];

	CGraph2DWnd *m_apGraphWnd[2];
public:
	void init();
	afx_msg void OnPaint();

	//
	//
	bool MakeGraphWnd();
	int    m_aiCount_Visible[BUFFER_SIZE_GRAPH_2D];							//! ���� ���̴� �׷����� ����
	bool Init_Buffser(int iIndex_Plot, int iIndex_Overlap);
	bool   m_abFlag_Visible[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! true�̸� �׷����� ���� ���̰� �Ѵ�.
																				//! =========================================================================================================
																				//! �׷��� �Է� ������ ����	
	double m_adDataX[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP][BUFFER_SIZE_PIXEL_GRAPH];
	double m_adDataY[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP][BUFFER_SIZE_PIXEL_GRAPH];
	int    m_aiDataSize[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];

	//! �׷��� ���� Ư�� ������ X�࿡ �������� ǥ���Ѵ�.
	double m_adData_MTF4[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF(N/4) X�� ��
	double m_adData_MTF8[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF(N/8) X�� ��
	double m_adData_MTF50[BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF50	 X�� ��

	int DrawGraph(int iIndex_Plot);				/* iMode 0 : ������� 1 : �ڵ���� */
	void DrawBarGraph(int nID);
	int GetWindowRect_FromUI(int iUID, CWnd* pParentWnd, CRect &rcRet);
	void ReleaseGraphWnd();

};
