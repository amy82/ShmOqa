// ModelSelectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ModelSelectDlg.h"
#include "AutoInsp.h"
#include "afxdialogex.h"


// CModelSelectDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CModelSelectDlg, CDialogEx)

CModelSelectDlg::CModelSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MODEL_SELECT, pParent)
{

}

CModelSelectDlg::~CModelSelectDlg()
{
}

void CModelSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MODERATE_MODEL, m_ColorbuttonModelModerate);
	DDX_Control(pDX, IDC_BUTTON_NARROW_MODEL, m_ColorbuttonModelNarrow);
}


BEGIN_MESSAGE_MAP(CModelSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SELECT_EXIT, &CModelSelectDlg::OnBnClickedButtonModelSelectExit)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MODERATE_MODEL, &CModelSelectDlg::OnBnClickedButtonModerateModel)
	ON_BN_CLICKED(IDC_BUTTON_NARROW_MODEL, &CModelSelectDlg::OnBnClickedButtonNarrowModel)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_SELECT_LOAD, &CModelSelectDlg::OnBnClickedButtonModelSelectLoad)
END_MESSAGE_MAP()


// CModelSelectDlg �޽��� ó�����Դϴ�.

void CModelSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB_DLG_BG);
}


void CModelSelectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//this->ShowCurrentModel();
}


BOOL CModelSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	this->CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CModelSelectDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

void CModelSelectDlg::ShowCurrentModel()
{
	/*if (g_clModelType.m_nModelIndex == MODERATE_MODEL)
	{
		m_ColorbuttonModelNarrow.state = 0;
		m_ColorbuttonModelModerate.state = 1;

		m_ColorbuttonModelNarrow.Invalidate();
		m_ColorbuttonModelModerate.Invalidate();
	}
	else
	{
		m_ColorbuttonModelModerate.state = 0;
		m_ColorbuttonModelNarrow.state = 1;

		m_ColorbuttonModelNarrow.Invalidate();
		m_ColorbuttonModelModerate.Invalidate();
	}*/
	
}

void CModelSelectDlg::OnBnClickedButtonModelSelectExit()
{
	ShowWindow(SW_HIDE);
}

void CModelSelectDlg::OnBnClickedButtonModerateModel()
{
	m_ColorbuttonModelNarrow.state = 0;
	m_ColorbuttonModelModerate.state = 1;

	m_ColorbuttonModelNarrow.Invalidate();
	m_ColorbuttonModelModerate.Invalidate();
}


void CModelSelectDlg::OnBnClickedButtonNarrowModel()
{
	m_ColorbuttonModelModerate.state = 0;
	m_ColorbuttonModelNarrow.state = 1;

	m_ColorbuttonModelNarrow.Invalidate();
	m_ColorbuttonModelModerate.Invalidate();
}


void CModelSelectDlg::OnBnClickedButtonModelSelectLoad()
{

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_AUTO)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�ڵ� ���� �� ��� �Ұ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �ڵ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}

	if (g_clTaskWork[m_nUnit].m_nAutoFlag == MODE_PAUSE)
	{
		g_ShowMsgPopup(_T("ERROR"), _T("�Ͻ� ���� �� ��� �Ұ�."), RGB_COLOR_RED);
		AddLog(_T("[INFO] �Ͻ� ���� �� ��� �Ұ�"), 1, m_nUnit);
		return;
	}
	CString strModelName = _T("");
	int i;

	//g_clAdo.Disconnect();

	if (m_ColorbuttonModelModerate.state == 1)
	{
		//g_clModelType.m_nModelIndex = MODERATE_MODEL;

		strModelName = _T("MODERATE");
		_tcscpy_s(g_clSysData.m_szModelName, SIZE_OF_100BYTE, (TCHAR*)(LPCTSTR)strModelName);
		g_clModelType.mTSave();

		// MODEL TYPE LOAD
		g_clModelType.mTLoad();

		for (i = 0; i < MAX_UNIT_COUNT; i++)
		{
			g_clModelData[i].Load(g_clSysData.m_szModelName);
			//g_clModelData[i].EpoxyDataLoad(g_clSysData.m_szModelName);
			g_clMarkData[i].LoadData(g_clSysData.m_szModelName);
			g_clModelData[i].PatternLoad(g_clSysData.m_szModelName);        //���� �̹��� �ε�
			g_clModelData[i].AcmisDataLoad(g_clSysData.m_szModelName);
			
			
		}
	}
	else
	{
		//g_clModelType.m_nModelIndex = NARROW_MODEL;
		strModelName = _T("NARROW");
		_tcscpy_s(g_clSysData.m_szModelName, SIZE_OF_100BYTE, (TCHAR*)(LPCTSTR)strModelName);
		g_clModelType.mTSave();

		// MODEL TYPE LOAD
		g_clModelType.mTLoad();

		for (i = 0; i < MAX_UNIT_COUNT; i++)
		{
			g_clModelData[i].Load(g_clSysData.m_szModelName);
			//g_clModelData[i].EpoxyDataLoad(g_clSysData.m_szModelName);
			g_clMarkData[i].LoadData(g_clSysData.m_szModelName);
			g_clModelData[i].PatternLoad(g_clSysData.m_szModelName);        //���� �̹��� �ε�
			g_clModelData[i].AcmisDataLoad(g_clSysData.m_szModelName);
		}
	}
}
