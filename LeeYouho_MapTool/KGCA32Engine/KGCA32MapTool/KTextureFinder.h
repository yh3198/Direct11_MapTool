#pragma once
#include "afxcmn.h"


// KTextureFinder ��ȭ �����Դϴ�.

class KTextureFinder : public CDialogEx
{
	DECLARE_DYNAMIC(KTextureFinder)

public:
	CFileFind m_Finder;
	vector<CString> m_TexFileName;
	vector<CString> m_TexFolderName;

	CString CurrentFolder;

	KTextureFinder(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~KTextureFinder();


	void InitTreeCtrl();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KTEXTUREFINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
