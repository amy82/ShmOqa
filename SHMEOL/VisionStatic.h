#pragma once

#define SQUARE_RESET_SIZE_X		100
#define SQUARE_RESET_SIZE_Y		100

#define RESET_SIZE_X		100
#define RESET_SIZE_Y		100

#define MOUSE_DRAG_SIZE_X		20//10
#define MOUSE_DRAG_SIZE_Y		20//10

// CVisitionStatic

class CVisionStatic : public CStatic
{
	DECLARE_DYNAMIC(CVisionStatic)

public:
	CVisionStatic();
	virtual ~CVisionStatic();

	
protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetDragUse(bool bUse) { m_bDragUse = bUse; }
	void SetDragFlag(int nFlag) { m_nDragFlag = nFlag; }
	void SetDrawLine(bool bDraw) { m_bDrawLine = bDraw; }
private:
	int m_nUnit;

	//CamAlign Drag
	int m_nDragFlag;
	bool m_bDragUse;
	bool m_bDrawLine;

	CDMRect m_clDRect;

	bool m_bClickFlag;
	CPoint m_clClickPoint;
	CRect m_clRectRoi;

	int m_nType;
public:
	void SetInit(int nCamNo, double dZoomFac, CPoint clPtSize);
	void SetLock(bool bLock) { m_bLock = bLock; };
	void DisplayZoom();
	CDrawBox GetDrawBox() { return m_clDrawBox; };

	void LoadStandardCursor();
	void ChangeCursor(int nType);
	void DestroyCursor();

	int GetLineType();
	void InitScroll();

	void PlusZoom();
	void MinusZoom();

	void ImageScroll();
	
	void DrawMeasure(CPoint clPoint);
	void ClearMeasure();
	void DrawDistance();
	int ChangeCursorMeasureMode(CPoint clPoint);

	void DrawRect(CPoint clPoint, CPoint clPtSize);
	
    
    void SetSfrRoi();
    void SetFovRoi();
    void SetSnrRoi();
	void InitSfrRoi();
	void InitFovRoi();
    

	void DrawCamAlign(int nIndex);
    void DrawRectSnr(int nIndex);
    void DrawRectFov(int nIndex);
	void DrawRectSfr(int nIndex);
    void MoveRectSnr(int nMoveType, int nType, int nMoveSize);
    void MoveRectFov(int nMoveType, int nType, int nMoveSize);
    void MoveRectSfr(int nMoveType, int nType, int nMoveSize);

    
    int GetSelectedSNRNo(CPoint point);
    int GetSelectedFovNo(CPoint point);
    int GetSelectedSfrNo(CPoint point);
	int GetSelectedCursor(CPoint point);
	void RegistCircleMark();
    void RegistFovMark(); //fov , distortion , rotate  ������ũ ����
    void RegistSnrMark();
	void SaveSfrRoi();

	CRect GetRect() { return m_clRect; }
private:
	void SetDrawMoveSfr(int nRefIndex, CPoint clPoint);
	void SetDrawMoveFov(int nRefIndex, CPoint clPoint);
	void SetDrawMoveCircle(int nRefIndex, CPoint clPoint);

public:
	int m_nCamNo;				// CAM NO
	CPoint m_clCamSize;			// IMAGE SIZE
	CPoint m_clDisplaySize;		// DISPLAY SIZE
	double m_dZoomFac;			// ZOOM ����
	
	int m_nDragType;			// DRAG Ÿ��(DRAW/ȭ���̵�/MEASURE)
	bool m_bDrag;				// DRAG �÷���
	bool m_bRButtonUp;			// RBUTTONUP �÷���

	int m_nMeasureStep;			// �Ÿ� ���� �׸��� ���(0:ù��° ��ǥ, 1:����ǥ, 2:�Ϸ�)
	CRect m_clRect;
	CPoint m_clMeasure[2];		// Measure���� ��ġ(����, ��)

	CPoint m_clScrollPos;		// V/H Scroll ���� ��
	bool m_bClick;				// LBUTTONDOWN �÷���

	int m_nDragDistType;
	CPoint m_clClickDist;		// ���� �Ÿ� ��� ���� ��ǥ

	int m_nBoxDrawType;			// Box ���� �ڽ� ����
	CDrawBox m_clDrawBox;

	int m_nCursorType;

    bool m_FovSetMode;          //Fov ���� ���̰�
    bool m_SnrSetMode;          //Snr ���� ���̰�

	HCURSOR m_hCursorStd;
	HCURSOR m_hCursorWidth;
	HCURSOR m_hCursorHeight;
	HCURSOR m_hCursorMove;
	HCURSOR m_hCursorNWSE;
	HCURSOR m_hCursorNESW;

	bool m_bLock;

	CRect m_clRectROI[MAX_LAST_INSP_COUNT];
	CPoint m_clPtSfrOffset[MAX_LAST_INSP_COUNT];

    CRect m_clRectCircle[4];


	//FOV
	CPoint m_clPtFovOffset[MAX_FOV_COUNT];
    CRect m_clRectFov[MAX_FOV_COUNT];



    CRect m_clRectDrawSnr[MAX_SNR_COUNT];
	
private:
    int	m_nSelectIndexCCD;
    int	m_nSelectIndexFOV;
    int	m_nSelectIndexSNR;

	int m_nSfrSizeX[MAX_LAST_INSP_COUNT];
	int m_nSfrSizeY[MAX_LAST_INSP_COUNT];

	int m_nFovSizeX[MAX_FOV_COUNT];
	int m_nFovSizeY[MAX_FOV_COUNT];

	int m_nCenterSfrSizeX;	//���� ��Ʈ�� �簢�� �ΰ��� 
	int m_nCenterSfrSizeY;
    //
    
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


