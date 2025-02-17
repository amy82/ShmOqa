// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInsp.h"
#include "CcdMesSpecDlg.h"
#include "afxdialogex.h"


// CCcdMesSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdMesSpecDlg, CDialogEx)

CCcdMesSpecDlg::CCcdMesSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdMesSpecDlg::IDD, pParent)
{
	m_nUnit = UNIT_AA1;
}

CCcdMesSpecDlg::~CCcdMesSpecDlg()
{
}

void CCcdMesSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CCD_MES_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_MES_SPEC_LOAD, m_clColorButtonLoad);
	DDX_Control(pDX, IDC_BUTTON_CCD_MES_SPEC_CLOSE, m_clColorButtonClose);
	
}


BEGIN_MESSAGE_MAP(CCcdMesSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()

    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_MES_SPEC_GRID1, &CCcdMesSpecDlg::OnNMDblClickedMesGrid1)

	ON_BN_CLICKED(IDC_BUTTON_CCD_MES_SPEC_SAVE, &CCcdMesSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MES_SPEC_CLOSE, &CCcdMesSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_BN_CLICKED(IDC_BUTTON_CCD_MES_SPEC_LOAD, &CCcdMesSpecDlg::OnBnClickedButtonCcdMesSpecLoad)
END_MESSAGE_MAP()


// CCcdMesSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB_DLG_BG);
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CCcdMesSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
    this->InitMesGridCtrl1();
    
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	

}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::InitMesGridCtrl1()
{
#if 0
    TCHAR* pszCol[] = { _T("No"), _T("Item"), _T("Min"), _T("Max") };

    CRect rect;
    int i, j;
    int DLG = IDC_STATIC_CCD_MES_SPEC_GRID1;

    CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = MES_VERIFY_SPEC_COUNT + 1;// ;//�Ʒ�
	int SpecCol = 4;// 3;//��
    int margin = 4;
    int gridHeight = 25;
	int gridWidth0 = 30;
	int gridWidth1 = 240;
	int gridWidth2 = 100;// 75;
    int totalWidth = gridWidth0 + gridWidth1 + (gridWidth2*(SpecCol - 2));
	int viewRow = 30;
	int xMargin = 21;

	if (viewRow > MES_VERIFY_SPEC_COUNT + 1)
	{
		viewRow = MES_VERIFY_SPEC_COUNT + 1;
		xMargin = 4;
		margin = 4;
	}
    //
    pWnd->GetWindowRect(rect);
    ScreenToClient(rect);

    rect.right = totalWidth + xMargin;
    //rect.bottom = (gridHeight*SpecRow) + margin;
	rect.bottom = (gridHeight*viewRow) + margin;
    pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


    GetDlgItem(DLG)->GetWindowRect(rect);
    ScreenToClient(rect);
	m_clGridMesSpec1.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

    m_clGridMesSpec1.SetTextBkColor(RGB_COLOR_WHITE);
    m_clGridMesSpec1.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGridMesSpec1.SetFixedTextColor(RGB_COLOR_WHITE);
    m_clGridMesSpec1.SetReference_Setting();
    m_clGridMesSpec1.EnableSelection(FALSE);
    m_clGridMesSpec1.SetRowCount(SpecRow);
    m_clGridMesSpec1.SetColumnCount(SpecCol);
    m_clGridMesSpec1.SetFixedRowCount(1);
    m_clGridMesSpec1.SetFixedColumnCount(1);

    for (i = 0; i < SpecRow; i++)
    {
		m_clGridMesSpec1.SetRowHeight(i, gridHeight);
		m_clGridMesSpec1.SetItemText(i + 1, 0, i);
		m_clGridMesSpec1.SetItemText(i + 1, 1, MesSpecList[i].c_str());

        for (j = 0; j < SpecCol; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
					m_clGridMesSpec1.SetColumnWidth(j, gridWidth0);
                }
				else if (j == 1)
				{
					m_clGridMesSpec1.SetColumnWidth(j, gridWidth1);
				}
                else
                {
					m_clGridMesSpec1.SetColumnWidth(j, gridWidth2);
                }

				m_clGridMesSpec1.SetItemText(i, j, pszCol[j]);
            }
			if (j == 0)
			{
				m_clGridMesSpec1.SetItemFormat(i, j, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				m_clGridMesSpec1.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
        }
    }
#endif
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
        this->ShowMesGridSpec();
	}
}
//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ Get
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::GetMesSpec()
{
	int i = 0;
	CString tmpStr = "";
	for (i = 0; i < MES_VERIFY_SPEC_COUNT; i++)
	{
		tmpStr = m_clGridMesSpec1.GetItemText(i + 1, 2);	//Min Spec
		_stprintf_s(EEpromVerifyData.vMinData[i], SIZE_OF_100BYTE, tmpStr);
		tmpStr = m_clGridMesSpec1.GetItemText(i + 1, 3);	//Max Spec
		_stprintf_s(EEpromVerifyData.vMaxData[i], SIZE_OF_100BYTE, tmpStr);



	}
	tmpStr.Empty();
}
//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::ShowMesGridSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i = 0;
	int mMesval1 = 0;
	for (i = 0; i < MES_VERIFY_SPEC_COUNT; i++)
	{
		m_clGridMesSpec1.SetItemText(i + 1, 2, EEpromVerifyData.vMinData[i]);
		m_clGridMesSpec1.SetItemText(i + 1, 3, EEpromVerifyData.vMaxData[i]);
	}
	m_clGridMesSpec1.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ���Ұ��մϴ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}*/

	if (g_ShowMsgModal(_T("Ȯ��"), _T("SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false) {
		return;
	}

	this->GetMesSpec();
    
	MesDataSave();
	MesDataLoad();

	this->ShowMesGridSpec();
	AddLog(_T("[INFO] SPEC ����"), 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnNMDblClickedMesGrid1(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    //if (nRow >= 1 && nCol >= 1)
	if ((nRow >= 5 && nRow <= 13) && nCol == 3)
	{
		return;
	}
	if ((nRow == 39) && nCol == 3)
	{
		return;
	}
	if ((nRow >= 21 && nRow <= 24) && nCol == 2)
	{
		return;
	}
	if ((nRow >= 1 ) && nCol >= 2)	//ApsSpecRow ���� 1 �Ѱ� Epoxy ���� �����ؼ� ����
    {
        sData = m_clGridMesSpec1.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
				if (nRow == 3 || nRow == 4)//if (nRow == 67 || nRow == 68)
				{
					sData.Format(_T("%.0lf"), pDlg->GetReturnVal());
				}
				else
				{
					sData.Format(_T("%.06lf"), pDlg->GetReturnVal());
				}
               
				m_clGridMesSpec1.SetItemText(nRow, nCol, sData);

				m_clGridMesSpec1.Invalidate();
            }

            delete pDlg;
        }
    }
}
#if 0
void CCcdMesSpecDlg::OnNMDblClickedMesGrid2(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridMesSpec2.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
                sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridMesSpec2.SetItemText(nRow, nCol, sData);

				m_clGridMesSpec2.Invalidate();
            }

            delete pDlg;
        }
    }
}

void CCcdMesSpecDlg::OnNMDblClickedMesGrid3(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridMesSpec3.GetItemText(nRow, nCol);
        CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
        if (pDlg != NULL)
        {
            if (pDlg->DoModal() == IDOK)
            {
                sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridMesSpec3.SetItemText(nRow, nCol, sData);

				m_clGridMesSpec3.Invalidate();
            }

            delete pDlg;
        }
    }
}


void CCcdMesSpecDlg::OnNMDblClickedMesGrid4(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridMesSpec4.GetItemText(nRow, nCol);
		CKeyPadDlg* pDlg = new CKeyPadDlg(sData, 10);
		if (pDlg != NULL)
		{
			if (pDlg->DoModal() == IDOK)
			{
				sData.Format(_T("%.03lf"), pDlg->GetReturnVal());
				m_clGridMesSpec4.SetItemText(nRow, nCol, sData);

				m_clGridMesSpec4.Invalidate();
			}

			delete pDlg;
		}
	}
}

#endif
//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdMesSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CCcdMesSpecDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCcdMesSpecDlg::OnBnClickedButtonCcdMesSpecLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	g_clSysData.OcOffsetLoad();
	MesDataLoad();	//load button

	this->ShowMesGridSpec();
}
