// KTextureFinder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KTextureFinder.h"
#include "afxdialogex.h"


// KTextureFinder 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTextureFinder, CDialogEx)

KTextureFinder::KTextureFinder(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KTEXTUREFINDER, pParent)
{

}

KTextureFinder::~KTextureFinder()
{
}

void KTextureFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TexFindTree, m_TexFindTree);
	DDX_Control(pDX, IDC_TexFindList, m_TexFindList);
}


BEGIN_MESSAGE_MAP(KTextureFinder, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TexFindTree, &KTextureFinder::OnTvnSelchangedTexfindtree)
	ON_BN_CLICKED(IDCANCEL, &KTextureFinder::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &KTextureFinder::OnBnClickedOk)
	ON_BN_CLICKED(ID_TEXOK, &KTextureFinder::OnBnClickedTexok)
	ON_BN_CLICKED(ID_TEXCANCEL, &KTextureFinder::OnBnClickedTexcancel)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_TexFindList, &KTextureFinder::OnLvnItemActivateTexfindlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TexFindList, &KTextureFinder::OnLvnItemchangedTexfindlist)
END_MESSAGE_MAP()


// KTextureFinder 메시지 처리기입니다.
void KTextureFinder::InitTreeCtrl()
{
	// ======================= 시작시 트리생성 ======================= //
	HTREEITEM hItem = m_TexFindTree.InsertItem(_T("E:", hItem));
	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("E:\\*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			m_TexFindTree.InsertItem(finder.GetFileName(), hItem);
		}
	}
	m_TexFindTree.EnsureVisible(hItem);
	DWORD dwStyle = m_TexFindList.GetExtendedStyle();
	m_TexFindList.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_BORDERSELECT | LVS_EX_CHECKBOXES | LVS_EX_AUTOCHECKSELECT);
}


BOOL KTextureFinder::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitTreeCtrl();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTextureFinder::OnTvnSelchangedTexfindtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ============================= Tree 구현코드 ============================= //
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	// TreeView의 폴더를 마우스로 클릭하면, 하위폴더 목록을 TreeView에 보여준다.
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem; // 현재 선택한 아이템

	// 선택된 아이템의 하위 목록을 트리 컨트롤에 추가한다.
	// 선택된 아이템의 경로를 읽어온다.
	CString pathSelected; // 최종으로 얻어 올 경로

	HTREEITEM hParentItem = hSelected; // 현재 선택된 아이템을 첫번째 시작으로 한다.
	while (hParentItem != NULL) // 최상위 부모가 없을때까지
	{
		pathSelected = _T("\\") + pathSelected;
		pathSelected = m_TexFindTree.GetItemText(hParentItem) + pathSelected;
		hParentItem = m_TexFindTree.GetParentItem(hParentItem);
	}
	//완성된 경로의 뒤에 "*.*"를 추가하여 최종으로 선택된 아이템의 경로를 완성한다.
	CurrentFolder = pathSelected;
	pathSelected = pathSelected + _T("*.*");
	

	CFileFind finder;
	// CFileFind를 이용하여 읽어 온 경로의 파일 또는 폴더를 찾는다.
	BOOL bWorking = finder.FindFile(pathSelected);

	// 선택한 아이템이 선택한 적이 있는지 없는지 검사
	// 없는 경우 = 해당 아이템의 GetItemData()는 0으로 초기화 되어 있다.
	if (m_TexFindTree.GetItemData(hSelected) == 0)
	{
		// 선택한 아이템의 하위목록을 하나씩 검사하면서 폴더의 경우만 Tree 아이템으로 삽입한다.
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots()) continue;
			if (finder.IsDirectory())
			{
				m_TexFindTree.InsertItem(finder.GetFileName(), hSelected);
			}
		}
		// 해당 아이템은 선택된 적이 있음을 함수로 표시한다.
		m_TexFindTree.SetItemData(hSelected, 1);
		m_TexFindTree.EnsureVisible(hSelected);
	}
	// ========================================================================= //
	// ============================= List 구현코드 ============================= //
	
	// TreeView의 폴더를 마우스로 클릭하면 리스트를 비운다.
	if (pNMTreeView->itemNew.hItem != pNMTreeView->itemOld.hItem)
	{
		m_TexFindList.DeleteAllItems();
	}

	// 트리뷰에서 선택한 아이템의 파일만을 보여준다.
	bWorking = finder.FindFile(pathSelected);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (!finder.IsDirectory())
		{
			m_TexFindList.InsertItem(1, finder.GetFileName());
		}
	}
	// ========================================================================== //
	*pResult = 0;
}


void KTextureFinder::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
	this->OnFinalRelease();
}


void KTextureFinder::OnBnClickedOk()
{
	CDialogEx::OnOK();
	this->OnFinalRelease();
}


void KTextureFinder::OnBnClickedTexok()
{
	if (m_TexFileName.size())
	{
		for (int iText = 0; iText < m_TexFileName.size(); iText++)
		{
			AfxMessageBox(m_TexFileName[iText]);
		}
	}
	//AfxMessageBox(m_TexFileName);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void KTextureFinder::OnBnClickedTexcancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void KTextureFinder::OnLvnItemActivateTexfindlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void KTextureFinder::OnLvnItemchangedTexfindlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_TexTextIndex.clear();
	m_TexFileName.clear();
	m_TexFolderName.clear();
	POSITION pos;
	pos = m_TexFindList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nSelected = m_TexFindList.GetNextSelectedItem(pos);
		m_TexTextIndex.push_back(nSelected);
	}
	
	for (int TexCount = 0; TexCount < m_TexTextIndex.size(); TexCount++)
	{
		CString Text = m_TexFindList.GetItemText(m_TexTextIndex[TexCount], 0);
		CString CFolder = CurrentFolder;
		m_TexFolderName.push_back(CFolder);
		m_TexFileName.push_back(Text);
	}

	/*int nSelectedItem = m_TexFindList.GetNextItem(-1, LVNI_SELECTED);
	m_TexFileName = m_TexFindList.GetItemText(nSelectedItem, 0);*/

	*pResult = 0;
}


void KTextureFinder::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Finder.Close();
	m_TexFolderName.clear();
	m_TexTextIndex.clear();
	CurrentFolder.Empty();
	m_TexFindTree.OnFinalRelease();
	m_TexFindList.OnFinalRelease();
	CDialogEx::PostNcDestroy();
}
