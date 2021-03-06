
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "KMapControlPane.h"
#include "KTextureControlPane.h"
#include "KWaterMapControlPane.h"
#include "KMapObjControlPane.h"
#include "KTextureFinder.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	KMapControlPane			m_wndMapToolCtrl;		// 맵 컨트롤 다이얼로그
	KTextureControlPane		m_wndTexToolCtrl;		// 텍스쳐 컨트롤 다이얼로그
	KWaterMapControlPane	m_wndWaterToolCtrl;		// 워터맵 컨트롤 다이얼로그
	KMapObjControlPane		m_wndMapObjToolCtrl;	// 맵 오브젝트 컨트롤 다이얼로그
	KTextureFinder			m_wndTexFindCtrl;		// 텍스쳐 파인더 다이얼로그
	

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedTexok();
};


