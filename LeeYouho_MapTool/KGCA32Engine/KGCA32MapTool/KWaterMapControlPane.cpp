// KWaterMapControlPane.cpp : ���� �����Դϴ�.
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



// KWaterMapControlPane �޽��� ó�����Դϴ�.




int KWaterMapControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_WaterForm = KWaterMapControlForm::CreateOne(this);

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void KWaterMapControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_WaterForm)
	{
		m_WaterForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


int KWaterMapControlPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
