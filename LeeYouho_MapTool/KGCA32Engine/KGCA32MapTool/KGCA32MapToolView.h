
// KGCA32MapToolView.h : CKGCA32MapToolView Ŭ������ �������̽�
//

#pragma once

class CKGCA32MapToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CKGCA32MapToolView();
	DECLARE_DYNCREATE(CKGCA32MapToolView)

// Ư���Դϴ�.
public:
	CKGCA32MapToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	BOOL m_bMouseTracking = FALSE;


// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CKGCA32MapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedFindtex();
	afx_msg void OnBnClickedTexok();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // KGCA32MapToolView.cpp�� ����� ����
inline CKGCA32MapToolDoc* CKGCA32MapToolView::GetDocument() const
   { return reinterpret_cast<CKGCA32MapToolDoc*>(m_pDocument); }
#endif

