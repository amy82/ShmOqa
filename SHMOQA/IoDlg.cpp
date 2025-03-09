// IoDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoInspDlg.h"
#include "IoDlg.h"
#include "afxdialogex.h"

#define IO_CHANNEL_MAX 1
// IoDlg ��ȭ �����Դϴ�.
static TCHAR* szInput[IO_CHANNEL_MAX][32] = { {
        _T("   EMERGENCY S/W1(MAIN)"),
        _T("   TRAY ��Ż�� �з°�"),
        _T("   MAIN AIR"),
        _T("   DOOR SENSOR #1"),
        _T("   AREA SENSOR"),
        _T("   TRAY ��ΰ��� #1"),
        _T("   TRAY ��ΰ��� #2"),
        _T("   TRAY ��ΰ��� #3"),
        _T("   TRAY ��ΰ��� #4"),
        _T("   TRAY �Ϻΰ��� #1"),
        _T("   TRAY �Ϻΰ��� #2"),
        _T("   TRAY �Ϻΰ��� #3"),
        _T("   TRAY �Ϻΰ��� #4"),
        _T("   TRAY SLIDE RAIL ���� #1"),
        _T("   TRAY SLIDE RAIL ���� #2"),
        _T("   TRAY SLIDE RAIL ���� #3"),
        _T("   TRAY SLIDE RAIL ���� #4"),
        _T("   TRAY CLAMP ���� #1"),
        _T("   TRAY UNCLAMP ���� #1"),
        _T("   TRAY CLAMP ���� #2"),
        _T("   TRAY UNCLAMP ���� #2"),
        _T("   TRAY CLAMP ���� #3"),
        _T("   TRAY UNCLAMP ���� #3"),
        _T("   TRAY CLAMP ���� #4"),
        _T("   TRAY UNCLAMP ���� #4"),
        _T("   Socket ReTurn ����"),
        _T("   Socket Turn ����"),
        _T("   Socket Grip ���� ����"),
        _T("   Socket Grip ���� ����"),
        _T("   "),
        _T("   "),
        _T("   ��ǰ ����")
    } 
};

static TCHAR* szOutput[IO_CHANNEL_MAX][32] = { {
        _T("   SOCKET 90�� ȸ�� ����"),
        _T("   SOCKET 90�� ȸ�� ����"),
        _T("   SOCKET ����"),
        _T("   SOCKET ����"),
        _T("   GRIPPER #1 UP"),
        _T("   GRIPPER #1 DOWN"),
        _T("   GRIPPER #1 ����"),
        _T("   GRIPPER #1 ����"),
        _T("   GRIPPER #2 UP"),
        _T("   GRIPPER #2 DOWN"),
        _T("   GRIPPER #2 ����"),
        _T("   GRIPPER #2 ����"),
        _T("   TRAY Ż��"),
        _T("   TRAY ����"),
        _T("   TRAY CLAMP #1 ����"),
        _T("   TRAY CLAMP #1 ����"),
        _T("   TRAY CLAMP #2 ����"),
        _T("   TRAY CLAMP #2 ����"),
        _T("   TRAY CLAMP #3 ����"),
        _T("   TRAY CLAMP #3 ����"),
        _T("   TRAY CLAMP #4 ����"),
        _T("   TRAY CLAMP #4 ����"),
        _T("   BUZZER S1"),
        _T("   BUZZER S2"),
        _T("   BUZZER S3"),
        _T("   BUZZER S4"),
        _T("   TOWER LAMP-R"),
        _T("   TOWER LAMP-Y"),
        _T("   TOWER LAMP-G"),
        _T("   "),
        _T("   "),
        _T("   ")
    }
};

IMPLEMENT_DYNAMIC(CIoDlg, CDialogEx)

CIoDlg::CIoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_IO, pParent)
{
    m_nCurrentCh = 0;
}

CIoDlg::~CIoDlg()
{
}

void CIoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIoDlg, CDialogEx)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CIoDlg �޽��� ó�����Դϴ�.


void CIoDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
                       // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
    CRect rect;
    GetClientRect(rect);
    dc.FillSolidRect(rect, MAIN_BG_COLOR);
}


BOOL CIoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    setInterface();
    InitGridCtrl_In();
    InitGridCtrl_Out();
    IoInChChange();
    IoOutChChange();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
void CIoDlg::setInterface()
{
    //CCalibrationDlg* pFrame = (CCalibrationDlg*)AfxGetApp()->m_pMainWnd;

    //WINDOWPLACEMENT wndpl;

    //this->GetWindowPlacement(&wndpl);

    //wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos1.right + 100;
    //wndpl.rcNormalPosition.right = MAIN_DLG_SIZE_X;
    //wndpl.rcNormalPosition.top = pFrame->m_rectCamDispPos1.top - SUB_DLG_OFFSET_Y + 10;
    //wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 85;
    //this->MoveWindow(&wndpl.rcNormalPosition);
    ////
    //m_IoTitleView
    //    .SetBkColor(M_COLOR_WHITE)
    //    .SetTextColor(M_COLOR_BLACK)
    //    .SetFontBold(TRUE)
    //    .SetFontSize(15);

    //CString str = "";
    //str.Format("Ch-%d", m_nCurrentCh);
    //m_IoTitleView.SetWindowText(str);

    //m_btn_Left.state = 150;
    //m_btn_Right.state = 150;
}

void CIoDlg::InitGridCtrl_In()
{
    CRect rect;
    CWnd *pWnd = (CWnd*)GetDlgItem(IDC_STATIC_IO_IN_GRID);
    inRow = 33;// 65;//�Ʒ�
    inCol = 3;//��
    int margin = 4;// 20;
    int gridHeight = 25;
    int gridWidth1 = 60;
    int gridWidth2 = 300;
    int gridWidth3 = 60;
    int totalWidth = gridWidth1 + gridWidth2 + gridWidth3;
    //
    pWnd->GetWindowRect(rect);
    ScreenToClient(rect);

    rect.right = totalWidth + margin;
    rect.bottom = (gridHeight*inRow) + 4;	//rect.bottom = (gridHeight*inRow) +margin;
    pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.

    GetDlgItem(IDC_STATIC_IO_IN_GRID)->GetWindowRect(rect);
    ScreenToClient(rect);
    m_clGrid_IO_In.Create(rect, this, IDC_STATIC_IO_IN_GRID, WS_TABSTOP | WS_VISIBLE);

    m_clGrid_IO_In.SetTextBkColor(GRID_COLOR_WHITE);
    m_clGrid_IO_In.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGrid_IO_In.SetFixedTextColor(GRID_COLOR_WHITE);
    m_clGrid_IO_In.SetReference_Setting();
    m_clGrid_IO_In.EnableSelection(FALSE);
    m_clGrid_IO_In.SetRowCount(inRow);
    m_clGrid_IO_In.SetColumnCount(inCol);
    m_clGrid_IO_In.SetFixedRowCount(1);
    m_clGrid_IO_In.SetFixedColumnCount(1);
    CString tmpStr = "";


    m_clGrid_IO_In.SetItemText(0, 0, _T("No"));
    m_clGrid_IO_In.SetItemText(0, 1, "�Է� Comment");
    m_clGrid_IO_In.SetItemText(0, 2, "�Է�");
    //
    for (int i = 0; i<inRow - 1; i++)
    {
        tmpStr.Format("%d", i + 1);
        m_clGrid_IO_In.SetItemText(i + 1, 0, tmpStr);
    }
    //
    /*m_clGrid_IO_In.SetItemText(1, 1, "EMERGENCY S/W #1");
    m_clGrid_IO_In.SetItemText(5, 1, "MAIN AIR");
    m_clGrid_IO_In.SetItemText(7, 1, "DOOR SENSOR #1");
    m_clGrid_IO_In.SetItemText(8, 1, "DOOR SENSOR #2");
    m_clGrid_IO_In.SetItemText(9, 1, "DOOR SENSOR #3");
    m_clGrid_IO_In.SetItemText(10, 1, "DOOR SENSOR #4");
    m_clGrid_IO_In.SetItemText(11, 1, "DOOR SENSOR #5");
    m_clGrid_IO_In.SetItemText(12, 1, "DOOR SENSOR #6");
    m_clGrid_IO_In.SetItemText(19, 1, "UV - ON");
    m_clGrid_IO_In.SetItemText(25, 1, "EPOXY ���� ON");*/
    //m_clGrid_IO_In.SetItemText(31, 1, "LIGHT CURTAIN");
    //-============================================================================================================
    //-32
    /*m_clGrid_IO_In.SetItemText(33, 1, "SENSOR CONTACT");
    m_clGrid_IO_In.SetItemText(34, 1, "HOLDER GRIP ����");
    m_clGrid_IO_In.SetItemText(35, 1, "HOLDER GRIP ����");
    m_clGrid_IO_In.SetItemText(36, 1, "DOOR OPEN");
    m_clGrid_IO_In.SetItemText(37, 1, "DOOR CLOSE");
    m_clGrid_IO_In.SetItemText(39, 1, "START PB");
    m_clGrid_IO_In.SetItemText(40, 1, "DOOR PB");
    m_clGrid_IO_In.SetItemText(50, 1, "LENS GRIP ����");
    m_clGrid_IO_In.SetItemText(51, 1, "LENS GRIP ����");
    m_clGrid_IO_In.SetItemText(52, 1, "PCB GRIP ����");
    m_clGrid_IO_In.SetItemText(53, 1, "PCB GRIP ����");
    m_clGrid_IO_In.SetItemText(57, 1, "Ŭ�� ���� CLOSE");
    m_clGrid_IO_In.SetItemText(62, 1, "LASER/CAM ����");
    m_clGrid_IO_In.SetItemText(63, 1, "LASER/CAM ����");
    */

    //
    int i = 0, j = 0;
    for (i = 0; i < inRow; i++)
    {
        m_clGrid_IO_In.SetRowHeight(i, gridHeight);

        for (j = 0; j < inCol; j++)
        {
            m_clGrid_IO_In.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            if (i == 0)
            {
                if (j == 0)
                {
                    m_clGrid_IO_In.SetColumnWidth(j, gridWidth1);
                }
                else if (j == 1) {
                    m_clGrid_IO_In.SetColumnWidth(j, gridWidth2);
                }
                else {
                    m_clGrid_IO_In.SetColumnWidth(j, gridWidth3);
                }
            }
        }
    }
    m_clGrid_IO_In.Invalidate();
}
void CIoDlg::InitGridCtrl_Out()
{
    CRect rect;
    CWnd *pWnd = (CWnd*)GetDlgItem(IDC_STATIC_IO_OUT_GRID);
    outRow = 33;// 65;//�Ʒ�
    outCol = 3;//��
    inRow = 33;// 65;//�Ʒ�
    inCol = 3;//��
    int margin = 4;// 20;
    int gridHeight = 25;
    int gridWidth1 = 60;
    int gridWidth2 = 300;
    int gridWidth3 = 60;
    int totalWidth = gridWidth1 + gridWidth2 + gridWidth3;
    //
    pWnd->GetWindowRect(rect);
    ScreenToClient(rect);

    rect.right = totalWidth + margin;
    rect.bottom = (gridHeight*inRow) + 4;
    pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.

    GetDlgItem(IDC_STATIC_IO_OUT_GRID)->GetWindowRect(rect);
    ScreenToClient(rect);
    m_clGrid_IO_Out.Create(rect, this, IDC_STATIC_IO_OUT_GRID, WS_TABSTOP | WS_VISIBLE);

    m_clGrid_IO_Out.SetTextBkColor(GRID_COLOR_WHITE);
    m_clGrid_IO_Out.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGrid_IO_Out.SetFixedTextColor(GRID_COLOR_WHITE);
    m_clGrid_IO_Out.SetReference_Setting();
    m_clGrid_IO_Out.EnableSelection(FALSE);
    m_clGrid_IO_Out.SetRowCount(outRow);
    m_clGrid_IO_Out.SetColumnCount(outCol);
    m_clGrid_IO_Out.SetFixedRowCount(1);
    m_clGrid_IO_Out.SetFixedColumnCount(1);

    CString tmpStr = "";


    m_clGrid_IO_Out.SetItemText(0, 0, _T("No"));
    m_clGrid_IO_Out.SetItemText(0, 1, "��� Comment");
    m_clGrid_IO_Out.SetItemText(0, 2, "���");

    //
    for (int i = 0; i<outRow - 1; i++)
    {
        tmpStr.Format("%d", i + 1);
        m_clGrid_IO_Out.SetItemText(i + 1, 0, tmpStr);
    }
    //
    //m_clGrid_IO_Out.SetItemText(8, 1, "TOWER LAMP - R");	
    //m_clGrid_IO_Out.SetItemText(9, 1, "TOWER LAMP - Y");	
    //m_clGrid_IO_Out.SetItemText(10, 1, "TOWER LAMP - G");	
    ////
    //m_clGrid_IO_Out.SetItemText(12, 1, "BUZZER1");
    //m_clGrid_IO_Out.SetItemText(13, 1, "BUZZER2");	
    //m_clGrid_IO_Out.SetItemText(14, 1, "BUZZER3");	
    //m_clGrid_IO_Out.SetItemText(15, 1, "BUZZER4");																//												//
    //m_clGrid_IO_Out.SetItemText(18, 1, "UV - ON");				
    //m_clGrid_IO_Out.SetItemText(19, 1, "UV - EMG");			
    //
    //m_clGrid_IO_Out.SetItemText(24, 1, "EPOXY ���� ON");			
    /*m_clGrid_IO_Out.SetItemText(32, 1, "HOLDER GRIP ����");
    m_clGrid_IO_Out.SetItemText(33, 1, "HOLDER GRIP ����");
    m_clGrid_IO_Out.SetItemText(34, 1, "DOOR CLOSE");
    m_clGrid_IO_Out.SetItemText(35, 1, "DOOR OPEN");
    m_clGrid_IO_Out.SetItemText(36, 1, "LENS GRIP ����");
    m_clGrid_IO_Out.SetItemText(37, 1, "LENS GRIP ����");
    m_clGrid_IO_Out.SetItemText(38, 1, "PCB GRIP ����");
    m_clGrid_IO_Out.SetItemText(39, 1, "PCB GRIP ����");
    m_clGrid_IO_Out.SetItemText(42, 1, "LASER/CAM ����");
    m_clGrid_IO_Out.SetItemText(43, 1, "LASER/CAM ����");
    m_clGrid_IO_Out.SetItemText(48, 1, "START PB");
    m_clGrid_IO_Out.SetItemText(49, 1, "DOOR PB");*/


    //
    int i = 0, j = 0;
    for (i = 0; i < outRow; i++)
    {
        m_clGrid_IO_Out.SetRowHeight(i, gridHeight);

        for (j = 0; j < outCol; j++)
        {
            m_clGrid_IO_Out.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            if (i == 0)
            {
                if (j == 0)
                {
                    m_clGrid_IO_Out.SetColumnWidth(j, gridWidth1);
                }
                else if (j == 1) {
                    m_clGrid_IO_Out.SetColumnWidth(j, gridWidth2);
                }
                else {
                    m_clGrid_IO_Out.SetColumnWidth(j, gridWidth3);
                }
            }
        }
    }
    m_clGrid_IO_Out.Invalidate();
}
void CIoDlg::IoInChChange()
{
    int i = 0;

    for (i = 1; i < 33; i++)
    {
        m_clGrid_IO_In.SetItemText(i, 1, szInput[m_nCurrentCh][i - 1]);
        if (_tcslen(szInput[m_nCurrentCh][i - 1]) <= 0)
        {
            m_clGrid_IO_In.SetItemBkColor(i, 2, GRID_COLOR_DIO_IN);// RGB_COLOR_WHITE);
        }
    }

    m_clGrid_IO_In.Invalidate();
}

void CIoDlg::IoOutChChange()
{
    int i = 0;

    for (i = 1; i < 33; i++)
    {
        m_clGrid_IO_Out.SetItemText(i, 1, szOutput[m_nCurrentCh][i - 1]);
        if (_tcslen(szOutput[m_nCurrentCh][i - 1]) <= 0)
        {
            m_clGrid_IO_Out.SetItemBkColor(i, 2, GRID_COLOR_DIO_OUT);// RGB_COLOR_WHITE);
        }
    }

    m_clGrid_IO_Out.Invalidate();
}