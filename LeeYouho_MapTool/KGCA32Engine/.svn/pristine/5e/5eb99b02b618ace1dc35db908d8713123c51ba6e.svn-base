#pragma once


// KMapControlPane
#include "KMapControlForm.h"
class KMapControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(KMapControlPane)

public:
	KMapControlForm* m_MapForm;

public:
	KMapControlPane();
	virtual ~KMapControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


