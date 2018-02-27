
// KGCA32MapToolView.cpp : CKGCA32MapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "KGCA32MapTool.h"
#endif

#include "KGCA32MapToolDoc.h"
#include "KGCA32MapToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKGCA32MapToolView

IMPLEMENT_DYNCREATE(CKGCA32MapToolView, CView)

BEGIN_MESSAGE_MAP(CKGCA32MapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKGCA32MapToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_FindTex, &CKGCA32MapToolView::OnBnClickedFindtex)
	ON_BN_CLICKED(ID_TEXOK, &CKGCA32MapToolView::OnBnClickedTexok)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CKGCA32MapToolView 생성/소멸

CKGCA32MapToolView::CKGCA32MapToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CKGCA32MapToolView::~CKGCA32MapToolView()
{
}

BOOL CKGCA32MapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CKGCA32MapToolView 그리기

void CKGCA32MapToolView::OnDraw(CDC* /*pDC*/)
{
	CKGCA32MapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CKGCA32MapToolView 인쇄


void CKGCA32MapToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKGCA32MapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CKGCA32MapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CKGCA32MapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CKGCA32MapToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CKGCA32MapToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKGCA32MapToolView 진단

#ifdef _DEBUG
void CKGCA32MapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CKGCA32MapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKGCA32MapToolDoc* CKGCA32MapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKGCA32MapToolDoc)));
	return (CKGCA32MapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CKGCA32MapToolView 메시지 처리기


void CKGCA32MapToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ResizeDevice(cx, cy);
}


void CKGCA32MapToolView::OnBnClickedFindtex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CKGCA32MapToolView::OnBnClickedTexok()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

#include  "MainFrm.h"
void CKGCA32MapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 클릭이 됐으면 클릭된 point (position)을 Sample에 넘긴다.

	I_Input.m_MFCDIMouseState[0] = KEY_PUSH;
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjCreateOne = true;
	CView::OnLButtonDown(nFlags, point);
}


void CKGCA32MapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	I_Input.m_MFCDIMouseState[0] = KEY_FREE;
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.bParallel = false;
	pApp->Sample.ColoringCnt = 0;
	if (pApp->Sample.m_vSelectedObjInfo.size())
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->m_wndMapObjToolCtrl.m_ObjForm->UpdateData(TRUE);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleX = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.x;
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjScaleX.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleX);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleY = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.y;
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjScaleY.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleY);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleZ = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.z;
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjScaleZ.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjScaleZ);

		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationX = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[0];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjRotationX.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationX);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationY = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[1];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjRotationY.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationY);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationZ = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[2];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjRotationZ.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjRotationZ);

		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransX = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate[0];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjTransX.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransX);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransY = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate[1];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjTransY.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransY);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransZ = pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate[2];
		pMain->m_wndMapObjToolCtrl.m_ObjForm->m_CObjTransZ.SetPos((int)pMain->m_wndMapObjToolCtrl.m_ObjForm->m_fObjTransZ);
		pMain->m_wndMapObjToolCtrl.m_ObjForm->UpdateData(FALSE);
	}
	CView::OnLButtonUp(nFlags, point);
}


void CKGCA32MapToolView::OnMouseLeave()
{
	m_bMouseTracking = FALSE;
	POINT MousePos;
	GetCursorPos(&MousePos);
	CRect ClientRect;
	GetClientRect(ClientRect);
	I_Input.m_MFCDIMouseState[0] = KEY_FREE;
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.bParallel = false;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseLeave();
}


void CKGCA32MapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;

		if (_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CKGCA32MapToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//if (pMsg->message == WM_RBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT)->m_hWnd) {
	//	// 아무런 처리 없이 그냥 return 한다.
	//	return TRUE;
	//}
	return CView::PreTranslateMessage(pMsg);
}
