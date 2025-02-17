#ifndef _BUTTONCTDEFINE_H
#define _BUTTONCTDEFINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonCT window

class CButtonCT : public CButton
{
// Construction
public:
	CButtonCT();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonCT)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonCT();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonCT)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	bool _grident;
	int _selected;
	//
	int state;
	int FontSize;
	int FontBold;
	void btStateChange(int _state);
	void FontSet(CDC *pDC, CRect rt);
	void DrawColorRect(CDC *pDC, CRect rt);
	void DrawGradientRect(CDC *pDC, CRect rt);
	COLOR16 RGB_Rtn(int value, bool before);

	CFont			m_Font;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCLASS_H__8E7A4504_82E5_4C81_8A30_642AC65A4550__INCLUDED_)
