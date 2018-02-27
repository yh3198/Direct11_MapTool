#pragma once


// KMapObjControlPane
#include "KMapObjControlForm.h"
class KMapObjControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(KMapObjControlPane)
public:
	KMapObjControlForm* m_ObjForm;
public:
	KMapObjControlPane();
	virtual ~KMapObjControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


