// KMapControlPane.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KMapControlPane.h"


// KMapControlPane

IMPLEMENT_DYNAMIC(KMapControlPane, CDockablePane)

KMapControlPane::KMapControlPane()
{

}

KMapControlPane::~KMapControlPane()
{
}


BEGIN_MESSAGE_MAP(KMapControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// KMapControlPane �޽��� ó�����Դϴ�.



int KMapControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_MapForm = KMapControlForm::CreateOne(this);
	return 0;
}


int KMapControlPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}


void KMapControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_MapForm)
	{
		m_MapForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
