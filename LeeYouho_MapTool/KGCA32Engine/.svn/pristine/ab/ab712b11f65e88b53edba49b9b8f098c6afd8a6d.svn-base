// KWaterMapControlPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KWaterMapControlPane.h"


// KWaterMapControlPane

IMPLEMENT_DYNAMIC(KWaterMapControlPane, CDockablePane)

KWaterMapControlPane::KWaterMapControlPane()
{

}

KWaterMapControlPane::~KWaterMapControlPane()
{
}


BEGIN_MESSAGE_MAP(KWaterMapControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// KWaterMapControlPane 메시지 처리기입니다.




int KWaterMapControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_WaterForm = KWaterMapControlForm::CreateOne(this);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void KWaterMapControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_WaterForm)
	{
		m_WaterForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int KWaterMapControlPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
