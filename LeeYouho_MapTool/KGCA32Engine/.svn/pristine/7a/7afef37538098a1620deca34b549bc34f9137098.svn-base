// KMapObjControlPane.cpp : 구현 파일입니다.
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



// KMapObjControlPane 메시지 처리기입니다.




int KMapObjControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_ObjForm = KMapObjControlForm::CreateOne(this);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
