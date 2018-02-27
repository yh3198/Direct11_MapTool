// KTextureControlPane.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KTextureControlPane.h"


// KTextureControlPane

IMPLEMENT_DYNAMIC(KTextureControlPane, CDockablePane)

KTextureControlPane::KTextureControlPane()
{

}

KTextureControlPane::~KTextureControlPane()
{
}


BEGIN_MESSAGE_MAP(KTextureControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// KTextureControlPane �޽��� ó�����Դϴ�.




int KTextureControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_TexForm = KTextureControlForm::CreateOne(this);
	return 0;
}

void KTextureControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_TexForm)
	{
		m_TexForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

int KTextureControlPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
