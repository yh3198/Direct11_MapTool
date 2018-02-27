// KMapControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KMapControlForm.h"
#include "KGlobalDlg.h"

KMapControlForm* g_MapContral;

// KMapControlForm

IMPLEMENT_DYNCREATE(KMapControlForm, CFormView)

KMapControlForm::KMapControlForm()
	: CFormView(IDD_KMAPCONTROLFORM)
	, iPatchCount(0)
	, iCellCount(0)
	, fDistance(0)
	, m_fRadius(0)
	, m_fHeightValue(0)
{
	g_MapContral = this;
}

KMapControlForm::~KMapControlForm()
{
}



KMapControlForm* KMapControlForm::CreateOne(CWnd* pParent)
{
	KMapControlForm* pForm = new KMapControlForm;
	if (!pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL))
	{
		return NULL;
	}
	return pForm;
}



void KMapControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_Texture);
	DDX_Control(pDX, IDC_FindTex2, m_HeightTexFinder);
	DDX_Control(pDX, IDC_LIST1, m_HeightTexture);
	DDX_Text(pDX, IDC_PatchCount, iPatchCount);
	DDX_Text(pDX, IDC_CellCount, iCellCount);
	DDX_Text(pDX, IDC_Distance, fDistance);
	DDX_Text(pDX, IDC_ControlerRadius, m_fRadius);
	DDX_Text(pDX, IDC_ControlerRadius2, m_fHeightValue);
}

BEGIN_MESSAGE_MAP(KMapControlForm, CFormView)
	ON_STN_CLICKED(IDC_MapToolName, &KMapControlForm::OnStnClickedMaptoolname)
	ON_STN_CLICKED(IDC_MapToolFunc5, &KMapControlForm::OnStnClickedMaptoolfunc5)
	ON_BN_CLICKED(IDC_FindTex, &KMapControlForm::OnBnClickedFindtex)
	ON_BN_CLICKED(IDCANCEL, &KMapControlForm::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &KMapControlForm::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FindTex2, &KMapControlForm::OnBnClickedHeightFindtex)
	ON_BN_CLICKED(IDC_CreateMap, &KMapControlForm::OnBnClickedCreatemap)
	//ON_BN_CLICKED(IDC_MapVertexDown2, &KMapControlForm::OnBnClickedMapvertexdown2)
	ON_BN_CLICKED(IDC_MapVertexUp, &KMapControlForm::OnBnClickedMapvertexup)
	ON_BN_CLICKED(IDC_MapVertexDown, &KMapControlForm::OnBnClickedMapvertexdown)
	ON_BN_CLICKED(IDC_MapVertexParallel, &KMapControlForm::OnBnClickedMapvertexparallel)
	ON_BN_CLICKED(IDC_VertexControloff, &KMapControlForm::OnBnClickedVertexcontroloff)
	ON_BN_CLICKED(IDC_RadiusApply, &KMapControlForm::OnBnClickedRadiusapply)
	ON_BN_CLICKED(IDC_RadiusApply2, &KMapControlForm::OnBnClickedHeightValue)
	ON_LBN_SELCHANGE(IDC_LIST2, &KMapControlForm::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST1, &KMapControlForm::OnLbnSelchangeHeightMap)
	ON_BN_CLICKED(IDC_BUTTON1, &KMapControlForm::OnBnClickedExportMap)
	ON_BN_CLICKED(IDC_BUTTON6, &KMapControlForm::OnBnClickedButton6)
END_MESSAGE_MAP()


// KMapControlForm 진단입니다.

#ifdef _DEBUG
void KMapControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KMapControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// KMapControlForm 메시지 처리기입니다.


void KMapControlForm::OnStnClickedMaptoolname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnStnClickedMaptoolfunc5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedFindtex()
{
	m_Texture.ResetContent();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				for (int TexCnt = 1; TexCnt < m_TexFinder.m_TexFileName.size(); TexCnt++)
				{
					m_FoundedTexName.push_back(m_TexFinder.m_TexFileName[TexCnt]);
				}
				m_FoundedTexName.push_back(m_TexFinder.m_TexFileName[0]);
			}	
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
	}
	for (int iTex = 0; iTex < m_FoundedTexName.size(); iTex++)
	{
		m_Texture.AddString(m_FoundedTexName[iTex]);
	}
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void KMapControlForm::OnBnClickedHeightFindtex()
{
	m_HeightTexture.ResetContent();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				for (int TexCnt = 1; TexCnt < m_TexFinder.m_TexFileName.size(); TexCnt++)
				{
					m_FoundedHeightTexName.push_back(m_TexFinder.m_TexFileName[TexCnt]);
				}
				m_FoundedHeightTexName.push_back(m_TexFinder.m_TexFileName[0]);
			}
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
	}
	for (int iTex = 0; iTex < m_FoundedHeightTexName.size(); iTex++)
	{
		m_HeightTexture.AddString(m_FoundedHeightTexName[iTex]);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void KMapControlForm::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void KMapControlForm::OnBnClickedCreatemap()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	UpdateData(TRUE);
	if (vSelectedTexText.size())
	{
		if (!GetFolder)
		{
			ZeroMemory(SetTexture, sizeof(SetTexture));
			for (int iTex = 0; iTex < vSelectedTexText.size(); iTex++)
			{
				CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
				DataPos += vSelectedTexText[iTex];
				vSelectedTexText[iTex] = DataPos;
				SetTexture[iTex] = (TCHAR*)(LPCTSTR)vSelectedTexText[iTex];
			}
			GetFolder = true;
		}
	}
	if (vSelectedHeightTexText.size())
	{
		if (!GetHeightFolder)
		{
			CString HDataPos = L"../../data/";
			HDataPos += vSelectedHeightTexText[0];
			vSelectedHeightTexText[0] = HDataPos;
			SetHeightTexture = (TCHAR*)(LPCTSTR)vSelectedHeightTexText[0];
		}
		GetHeightFolder = true;
	}
	pApp->Sample.CreateMap(iCellCount, iPatchCount, fDistance, SetTexture, vSelectedTexText.size(), SetHeightTexture);
	/*생성후 첫 프레임은 돌려준다.*/
	for (int iMap = 0; iMap < pApp->Sample.m_vMapQuadTree.size(); iMap++)
	{
		pApp->Sample.m_vMapQuadTree[iMap]->Frame(pApp->Sample.m_pMainCamera);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedMapvertexdown2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedMapvertexup()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VSpreatState = vSpreatNone;
	pApp->Sample.m_VObjSpreatState = vSpreatNone;

	VCtrlState = vUp;
	pApp->Sample.m_VCtrlState = VCtrlState;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedMapvertexdown()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VSpreatState = vSpreatNone;
	pApp->Sample.m_VObjSpreatState = vSpreatNone;

	VCtrlState = vDown;
	pApp->Sample.m_VCtrlState = VCtrlState;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedMapvertexparallel()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VSpreatState = vSpreatNone;
	pApp->Sample.m_VObjSpreatState = vSpreatNone;

	VCtrlState = vParallel;
	pApp->Sample.m_VCtrlState = VCtrlState;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedVertexcontroloff()
{
	VCtrlState = vNone;
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VCtrlState = VCtrlState;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedRadiusapply()
{
	UpdateData(TRUE);
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_fRadius = m_fRadius;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapControlForm::OnBnClickedHeightValue()
{
	UpdateData(TRUE);
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_fHeightValue = m_fHeightValue;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/*텍스쳐 리스트 선택시*/
void KMapControlForm::OnLbnSelchangeList2()
{
	vSelectedTexText.clear();
	GetFolder = false;
	
	int iCount = m_Texture.GetSelCount();
	CArray<int, int> arrListSel;
	arrListSel.SetSize(iCount);
	m_Texture.GetSelItems(iCount, arrListSel.GetData());

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		int iSel = arrListSel[iCnt];
		CString selectname;
		m_Texture.GetText(iSel, selectname);
		vSelectedTexText.push_back(selectname);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/*높이맵 텍스쳐 리스트 선택시*/
void KMapControlForm::OnLbnSelchangeHeightMap()
{
	vSelectedHeightTexText.clear();
	GetHeightFolder = false;
	
	int iCount = m_HeightTexture.GetSelCount();
	CArray<int, int> arrListSel;
	arrListSel.SetSize(iCount);
	m_HeightTexture.GetSelItems(iCount, arrListSel.GetData());

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		int iSel = arrListSel[iCnt];
		CString selectname;
		m_HeightTexture.GetText(iSel, selectname);
		vSelectedHeightTexText.push_back(selectname);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 맵 저장
void KMapControlForm::OnBnClickedExportMap()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.MapExport();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 맵 로드
void KMapControlForm::OnBnClickedButton6()
{
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
			KTextureControlForm* AlphaTex = g_MapTexContral;
			CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
			DataPos += m_TexFinder.m_TexFileName[0];
			TCHAR* ImportFile;
			ImportFile = (TCHAR*)(LPCTSTR)DataPos;

			if (m_TexFinder.m_TexFileName.size())
			{
				pApp->Sample.MapImport(ImportFile);
			}
			/*생성후 첫 프레임은 돌려준다.*/
			for (int iMap = 0; iMap < pApp->Sample.m_vMapQuadTree.size(); iMap++)
			{
				pApp->Sample.m_vMapQuadTree[iMap]->Frame(pApp->Sample.m_pMainCamera);
				AlphaTex->m_SplattingRed.ResetContent();
				AlphaTex->m_SplattingRed.AddString(pApp->Sample.m_mMapSplattingTexture[0] + 11); // +11 = L"../../data/" 삭제
				AlphaTex->m_SplattingGreen.ResetContent();
				AlphaTex->m_SplattingGreen.AddString(pApp->Sample.m_mMapSplattingTexture[1] + 11);
				AlphaTex->m_SplattingBlue.ResetContent();
				AlphaTex->m_SplattingBlue.AddString(pApp->Sample.m_mMapSplattingTexture[2] + 11);
				AlphaTex->m_SplattingAlpha.ResetContent();
				AlphaTex->m_SplattingAlpha.AddString(pApp->Sample.m_mMapSplattingTexture[3] + 11);
			}
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
