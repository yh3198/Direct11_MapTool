// KMapObjControlPane.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KMapObjControlPane.h"


// KMapObjControlPane

IMPLEMENT_DYNAMIC(KMapObjControlPane, CDockablePane)

KMapObjControlPane::KMapObjControlPane()
{

}

KMapObjControlPane::~KMapObjControlPane()
{
}


BEGIN_MESSAGE_MAP(KMapObjControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// KMapObjControlPane �޽��� ó�����Դϴ�.




int KMapObjControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_ObjForm = KMapObjControlForm::CreateOne(this);
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


int KMapObjControlPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}


void KMapObjControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_ObjForm)
	{
		m_ObjForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
