
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "KMapControlPane.h"
#include "KTextureControlPane.h"
#include "KWaterMapControlPane.h"
#include "KMapObjControlPane.h"
#include "KTextureFinder.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	KMapControlPane			m_wndMapToolCtrl;		// �� ��Ʈ�� ���̾�α�
	KTextureControlPane		m_wndTexToolCtrl;		// �ؽ��� ��Ʈ�� ���̾�α�
	KWaterMapControlPane	m_wndWaterToolCtrl;		// ���͸� ��Ʈ�� ���̾�α�
	KMapObjControlPane		m_wndMapObjToolCtrl;	// �� ������Ʈ ��Ʈ�� ���̾�α�
	KTextureFinder			m_wndTexFindCtrl;		// �ؽ��� ���δ� ���̾�α�
	

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// ������ �޽��� �� �Լ�
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


