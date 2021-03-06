// KWaterMapControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KWaterMapControlForm.h"


// KWaterMapControlForm

IMPLEMENT_DYNCREATE(KWaterMapControlForm, CFormView)

KWaterMapControlForm* KWaterMapControlForm::CreateOne(CWnd* pParent)
{
	KWaterMapControlForm* pForm = new KWaterMapControlForm;
	if (!pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL))
	{
		return NULL;
	}
	return pForm;
}

KWaterMapControlForm::KWaterMapControlForm()
	: CFormView(IDD_KWATERMAPCONTROLFORM)
	, m_fWaterHeight(0)
	, m_fWaterWave(0)
{
}

KWaterMapControlForm::~KWaterMapControlForm()
{
}

void KWaterMapControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_WaterHeight);
	DDX_Text(pDX, IDC_EDIT1, m_fWaterHeight);
	DDX_Control(pDX, IDC_SLIDER2, m_WaterWave);
	DDX_Text(pDX, IDC_EDIT2, m_fWaterWave);
	DDX_Control(pDX, IDC_LIST1, m_strWTexControl);
	DDX_Control(pDX, IDC_LIST5, m_strWNTexControl);
	DDX_Control(pDX, IDC_LIST2, m_CreatedWaterList);
}

BEGIN_MESSAGE_MAP(KWaterMapControlForm, CFormView)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &KWaterMapControlForm::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER1, &KWaterMapControlForm::OnTRBNThumbPosChangingSlider1)
	ON_STN_CLICKED(IDC_MapToolFunc4, &KWaterMapControlForm::OnStnClickedMaptoolfunc4)
	ON_BN_CLICKED(IDC_FindTex, &KWaterMapControlForm::OnBnClickedFindtex)
	ON_BN_CLICKED(IDC_FindTex3, &KWaterMapControlForm::OnBnClickedFindtex3)
	ON_BN_CLICKED(IDC_BUTTON1, &KWaterMapControlForm::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &KWaterMapControlForm::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &KWaterMapControlForm::OnEnChangeEdit2)
END_MESSAGE_MAP()


// KWaterMapControlForm 진단입니다.

#ifdef _DEBUG
void KWaterMapControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KWaterMapControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// KWaterMapControlForm 메시지 처리기입니다.


void KWaterMapControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	UpdateData(TRUE);
	m_WaterHeight.SetRange(-10, 10);
	m_fWaterHeight = 0.0f;
	m_WaterHeight.SetPos(10);

	m_WaterWave.SetRange(1, 4);
	m_fWaterWave = 2.0f;
	m_WaterWave.SetPos(2);

	m_WaterTexString = L"water.bmp";
	m_strWTexControl.ResetContent();
	m_strWTexControl.AddString(m_WaterTexString);

	CString pDataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	pDataPos += m_WaterTexString;
	m_WaterTexString = pDataPos;
	m_WaterTex = (TCHAR*)(LPCTSTR)m_WaterTexString;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_WaterNormalTexString = L"HR01_N.dds";
	m_strWNTexControl.ResetContent();
	m_strWNTexControl.AddString(m_WaterNormalTexString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_WaterNormalTexString;
	m_WaterNormalTexString = DataPos;
	m_WaterNormalTex = (TCHAR*)(LPCTSTR)m_WaterNormalTexString;

	UpdateData(FALSE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void KWaterMapControlForm::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fWaterHeight = m_WaterHeight.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void KWaterMapControlForm::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void KWaterMapControlForm::OnStnClickedMaptoolfunc4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// water 이미지 검색 시
void KWaterMapControlForm::OnBnClickedFindtex()
{
	m_WaterTexString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_WaterTexString = m_TexFinder.m_TexFileName[0];
			}
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
		else
		{
			return;
		}
	}
	// 지금은 0번만 사용하고 있어서 0번으로 했지만 여러개중 선택하게 되면 충돌체크를 해서 충돌한 쿼드트리를 찾아서 넣어줘야 한다.
	m_strWTexControl.ResetContent();
	m_strWTexControl.AddString(m_WaterTexString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_WaterTexString;
	m_WaterTexString = DataPos;
	m_WaterTex = (TCHAR*)(LPCTSTR)m_WaterTexString;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// Normal 이미지 검색 시
void KWaterMapControlForm::OnBnClickedFindtex3()
{
	m_WaterNormalTexString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_WaterNormalTexString = m_TexFinder.m_TexFileName[0];
			}
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
		else
		{
			return;
		}
	}
	// 지금은 0번만 사용하고 있어서 0번으로 했지만 여러개중 선택하게 되면 충돌체크를 해서 충돌한 쿼드트리를 찾아서 넣어줘야 한다.
	m_strWNTexControl.ResetContent();
	m_strWNTexControl.AddString(m_WaterNormalTexString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_WaterNormalTexString;
	m_WaterNormalTexString = DataPos;
	m_WaterNormalTex = (TCHAR*)(LPCTSTR)m_WaterNormalTexString;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 워터 생성 버튼을 눌렀을 때
void KWaterMapControlForm::OnBnClickedButton1()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	// 넘겨주고 있는 숫자 0은 0번째 water로 생성됨을 나타낸다.
	// 따라서 맵 선택을 구현하면 선택된 맵의 번호를 받아와야 한다.
	if (!pApp->Sample.m_vMapQuadTree.size()) // 맵이 없다면 생성불가
	{
		return;
	}
	pApp->Sample.CreateWaterMap(pApp->Sample.m_bSelectedMap, /* pApp->Sample.m_bSelectedMap->m_iMaxDepthLimit*/ m_fWaterWave, pApp->Sample.m_bSelectedMap->m_fMinDivideSize,
								0, m_WaterTex, m_WaterNormalTex, m_fWaterHeight, m_fWaterWave);
	m_CreatedWaterList.AddString(L"생성된 WaterMap");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// Height가 변경되었을 때
void KWaterMapControlForm::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(TRUE);
	if (m_fWaterHeight < -10)
		m_fWaterHeight = -10;
	if (m_fWaterHeight > 10)
		m_fWaterHeight = 10;
	UpdateData(FALSE);
	m_WaterHeight.SetPos((int)m_fWaterHeight);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KWaterMapControlForm::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fWaterWave < 1)
		m_fWaterWave = 1;
	if (m_fWaterWave > 4)
		m_fWaterWave = 4;
	UpdateData(FALSE);
	m_WaterWave.SetPos((int)m_fWaterWave);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
