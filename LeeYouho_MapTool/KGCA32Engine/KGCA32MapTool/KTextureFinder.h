#pragma once
#include "afxcmn.h"


// KTextureFinder 대화 상자입니다.

class KTextureFinder : public CDialogEx
{
	DECLARE_DYNAMIC(KTextureFinder)

public:
	CFileFind m_Finder;
	vector<CString> m_TexFileName;
	vector<CString> m_TexFolderName;

	CString CurrentFolder;

	KTextureFinder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTextureFinder();


	void InitTreeCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KTEXTUREFINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TexFindTree;
	CListCtrl m_TexFindList;
	vector<int> m_TexTextIndex;

	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTexfindtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedTexok();
	afx_msg void OnBnClickedTexcancel();
	afx_msg void OnLvnItemActivateTexfindlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedTexfindlist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PostNcDestroy();
};
