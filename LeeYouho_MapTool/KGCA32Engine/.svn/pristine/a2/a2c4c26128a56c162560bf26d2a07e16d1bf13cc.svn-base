#pragma once


// KTextureControlPane
#include "KTextureControlForm.h"
class KTextureControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(KTextureControlPane)
public:
	KTextureControlForm* m_TexForm;

public:
	KTextureControlPane();
	virtual ~KTextureControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


