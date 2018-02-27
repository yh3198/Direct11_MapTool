#pragma once

//enum VertexControlerState
//{
//	vNone = 0,
//	vUp,
//	vDown,
//	vParallel,
//};

// KMapControlForm 폼 뷰입니다.
#include "KTextureFinder.h"
#include "afxwin.h"
class KMapControlForm : public CFormView
{
	DECLARE_DYNCREATE(KMapControlForm)

protected:
	KMapControlForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~KMapControlForm();

	// 리소스 전용 변수입니다.
public:
	vector<CString> m_FoundedTexName;
	vector<CString> m_FoundedHeightTexName;
	CListBox m_Texture;
	CListBox m_HeightTexture;
	int iPatchCount;
	int iCellCount;
	float fDistance;

	// 리소스 외 변수입니다.
	int VCtrlState = 0;
	vector<CString> vSelectedTexText;
	vector<CString> vSelectedHeightTexText;
	TCHAR* SetTexture[4];
	bool GetFolder = true;
	bool GetHeightFolder = true;
	TCHAR* SetHeightTexture = NULL;

public:
	KTextureFinder m_TexFinder;
	static KMapControlForm* CreateOne(CWnd* pParent);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KMAPCONTROLFORM };
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
	afx_msg void OnStnClickedMaptoolname();
	afx_msg void OnStnClickedMaptoolfunc5();
	afx_msg void OnBnClickedFindtex();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CButton m_HeightTexFinder;
	afx_msg void OnBnClickedHeightFindtex();
	afx_msg void OnBnClickedCreatemap();
	afx_msg void OnBnClickedMapvertexdown2();
	afx_msg void OnBnClickedMapvertexup();
	afx_msg void OnBnClickedMapvertexdown();
	afx_msg void OnBnClickedMapvertexparallel();
	afx_msg void OnBnClickedVertexcontroloff();
	float m_fRadius;
	afx_msg void OnBnClickedRadiusapply();
	float m_fHeightValue;
	afx_msg void OnBnClickedHeightValue();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeHeightMap();
	afx_msg void OnBnClickedExportMap();
	afx_msg void OnBnClickedButton6();
};


