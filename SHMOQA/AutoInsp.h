
// TESLAInspInsp.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once
//#include "AutoInspDlg.h"
#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTESLAInspInspApp:
// �� Ŭ������ ������ ���ؼ��� TESLAInspInsp.cpp�� �����Ͻʽÿ�.
//

class CAutoInspApp : public CWinApp
{
public:
    CAutoInspApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	//CAutoInspDlg *MainDlg;
	DECLARE_MESSAGE_MAP()
};

extern CAutoInspApp theApp;