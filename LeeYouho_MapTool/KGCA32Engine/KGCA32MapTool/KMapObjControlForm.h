#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// KMapObjControlForm 폼 뷰입니다.

class KMapObjControlForm : public CFormView
{
	DECLARE_DYNCREATE(KMapObjControlForm)

protected:
	KMapObjControlForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~KMapObjControlForm();
public:
	static KMapObjControlForm* CreateOne(CWnd* pParent);
	float m_fObjScaleX;
	float m_fObjScaleY;
	float m_fObjScaleZ;
	float m_fObjRotationX;
	float m_fObjRotationY;
	float m_fObjRotationZ;
	float m_fObjTransX;
	float m_fObjTransY;
	float m_fObjTransZ;
	CSliderCtrl m_CObjScaleX;
	CSliderCtrl m_CObjScaleY;
	CSliderCtrl m_CObjScaleZ;
	CSliderCtrl m_CObjRotationX;
	CSliderCtrl m_CObjRotationY;
	CSliderCtrl m_CObjRotationZ;
	CSliderCtrl m_CObjTransX;
	CSliderCtrl m_CObjTransY;
	CSliderCtrl m_CObjTransZ;
public:
	CListBox TMapObjBox;
	CString m_SelectedObj;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KMAPOBJCONTROLFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void ObjBoxAddList();
	afx_msg void ObjBoxDeleteList();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnObjectCreateMode();
	afx_msg void OnRotationApply();
	afx_msg void OnTranslateApply();
	virtual void OnInitialUpdate();
	afx_msg void OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
//	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
};


