#pragma once
#include "KWaterMapControlForm.h"

// KWaterMapControlPane

class KWaterMapControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(KWaterMapControlPane)

public:
	KWaterMapControlForm* m_WaterForm;
	KWaterMapControlPane();
	virtual ~KWaterMapControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


