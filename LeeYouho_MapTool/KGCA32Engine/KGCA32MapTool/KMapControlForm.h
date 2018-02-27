#pragma once

//enum VertexControlerState
//{
//	vNone = 0,
//	vUp,
//	vDown,
//	vParallel,
//};

// KMapControlForm �� ���Դϴ�.
#include "KTextureFinder.h"
#include "afxwin.h"
class KMapControlForm : public CFormView
{
	DECLARE_DYNCREATE(KMapControlForm)

protected:
	KMapControlForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~KMapControlForm();

	// ���ҽ� ���� �����Դϴ�.
public:
	vector<CString> m_FoundedTexName;
	vector<CString> m_FoundedHeightTexName;
	CListBox m_Texture;
	CListBox m_HeightTexture;
	int iPatchCount;
	int iCellCount;
	float fDistance;

	// ���ҽ� �� �����Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


