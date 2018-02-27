#pragma once
#include "afxcmn.h"



// KWaterMapControlForm 폼 뷰입니다.
#include "KTextureFinder.h"
#include "afxwin.h"

class KWaterMapControlForm : public CFormView
{
	DECLARE_DYNCREATE(KWaterMapControlForm)

protected:
	KWaterMapControlForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~KWaterMapControlForm();
public:
	KTextureFinder m_TexFinder;
	CString m_WaterTexString;
	CString m_WaterNormalTexString;
	TCHAR* m_WaterTex;
	TCHAR* m_WaterNormalTex;

public:
	static KWaterMapControlForm* CreateOne(CWnd* pParent);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KWATERMAPCONTROLFORM };
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
	CSliderCtrl m_WaterHeight;
	float m_fWaterHeight;
	CSliderCtrl m_WaterWave;
	float m_fWaterWave;

	bool m_bHSelected;

	virtual void OnInitialUpdate();
	

	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedMaptoolfunc4();
	afx_msg void OnBnClickedFindtex();
	CListBox m_strWTexControl;
	CListBox m_strWNTexControl;
	afx_msg void OnBnClickedFindtex3();
	afx_msg void OnBnClickedButton1();
	CListBox m_CreatedWaterList;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
};


