// KTextureControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KTextureControlForm.h"
#include "KGlobalDlg.h"

KTextureControlForm* g_MapTexContral;

// KTextureControlForm

IMPLEMENT_DYNCREATE(KTextureControlForm, CFormView)

KTextureControlForm::KTextureControlForm()
	: CFormView(IDD_KTEXTURECONTROLFORM)
	, m_fSpreatRadius(0)
	, m_iSpreatRange(0)
{
	g_MapTexContral = this;
}

KTextureControlForm::~KTextureControlForm()
{
}

KTextureControlForm* KTextureControlForm::CreateOne(CWnd* pParent)
{
	KTextureControlForm* pForm = new KTextureControlForm;
	if (!pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL))
	{
		return NULL;
	}
	return pForm;
}


void KTextureControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SpreatRadius, m_fSpreatRadius);
	DDX_Control(pDX, IDC_LIST1, m_SplattingRed);
	DDX_Control(pDX, IDC_LIST3, m_SplattingGreen);
	DDX_Control(pDX, IDC_LIST4, m_SplattingBlue);
	DDX_Control(pDX, IDC_LIST5, m_SplattingAlpha);
	DDX_Text(pDX, IDC_EDIT3, m_iSpreatRange);
}

BEGIN_MESSAGE_MAP(KTextureControlForm, CFormView)
	ON_BN_CLICKED(IDC_SpreatModeOn, &KTextureControlForm::OnBnClickedSpreatmodeon)
	ON_BN_CLICKED(IDC_SpreatModeOff, &KTextureControlForm::OnBnClickedSpreatmodeoff)
	ON_BN_CLICKED(IDC_BUTTON1, &KTextureControlForm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &KTextureControlForm::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KTextureControlForm::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &KTextureControlForm::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &KTextureControlForm::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON2, &KTextureControlForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &KTextureControlForm::OnBnClickedGreenSearch)
	ON_BN_CLICKED(IDC_BUTTON4, &KTextureControlForm::OnBnClickedBlue)
	ON_BN_CLICKED(IDC_BUTTON5, &KTextureControlForm::OnBnClickedAlpha)
	ON_BN_CLICKED(IDC_RADIO5, &KTextureControlForm::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &KTextureControlForm::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &KTextureControlForm::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_BUTTON7, &KTextureControlForm::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &KTextureControlForm::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &KTextureControlForm::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &KTextureControlForm::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &KTextureControlForm::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &KTextureControlForm::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_SpreatModeOn2, &KTextureControlForm::OnBnClickedSpreatmodeon2)
	ON_BN_CLICKED(IDC_SpreatModeOff2, &KTextureControlForm::OnBnClickedSpreatmodeoff2)
	ON_BN_CLICKED(IDC_RADIO8, &KTextureControlForm::OnBnClickedRadio8)
	//ON_BN_CLICKED(IDC_A4, &KTextureControlForm::OnBnClickedA4)
END_MESSAGE_MAP()


// KTextureControlForm 진단입니다.

#ifdef _DEBUG
void KTextureControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KTextureControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// KTextureControlForm 메시지 처리기입니다.

// 스프레팅 모드 온
void KTextureControlForm::OnBnClickedSpreatmodeon()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_vMapQuadTree[0]->CreateSRV(pApp->Sample.m_vMapQuadTree[0]->SpreatingTexTure, pApp->Sample.m_vMapQuadTree[0]->StagingTexTure, pApp->Sample.m_vMapQuadTree[0]->desc);
	pApp->Sample.m_VCtrlState = vNone;
	pApp->Sample.m_VSpreatState = vSpreatApply;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedSpreatmodeoff()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VSpreatState = vSpreatNone;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_fSpreatingRadius = m_fSpreatRadius;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedRadio1()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iSpreatingColor = vRed;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedRadio2()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iSpreatingColor = vGreen;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedRadio3()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iSpreatingColor = vBlue;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedRadio4()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iSpreatingColor = vAlpha;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// Red Search Button Down
void KTextureControlForm::OnBnClickedButton2()
{
	m_RedString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_RedString = m_TexFinder.m_TexFileName[0];
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
	m_SplattingRed.ResetContent();
	m_SplattingRed.AddString(m_RedString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_RedString;
	m_RedString = DataPos;
	SplattingTexture[0] = (TCHAR*)(LPCTSTR)m_RedString;
	pApp->Sample.m_vMapQuadTree[0]->SplattingTextureLoad(pApp->Sample.m_pd3dDevice, SplattingTexture[0], 0);
	pApp->Sample.m_mMapSplattingTexture[0] = SplattingTexture[0];
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// Green Search Button Down
void KTextureControlForm::OnBnClickedGreenSearch()
{
	m_GreenString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_GreenString = m_TexFinder.m_TexFileName[0];
			}
			m_TexFinder.m_TexFileName.clear();
			m_TexFinder.OnFinalRelease();
		}
		else
		{
			return;
		}
	}
	// 지금은 0번만 사용하고 있어서 0번으로 했지만 여러개중 선택하게 되면 충돌체크를 해서 충돌한 쿼드트리를 찾아서 넣어줘야 한다
	m_SplattingGreen.ResetContent();
	m_SplattingGreen.AddString(m_GreenString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_GreenString;
	m_GreenString = DataPos;
	SplattingTexture[1] = (TCHAR*)(LPCTSTR)m_GreenString;
	pApp->Sample.m_vMapQuadTree[0]->SplattingTextureLoad(pApp->Sample.m_pd3dDevice, SplattingTexture[1], 1);
	pApp->Sample.m_mMapSplattingTexture[1] = SplattingTexture[1];
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedBlue()
{
	m_BlueString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_BlueString = m_TexFinder.m_TexFileName[0];
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
	m_SplattingBlue.ResetContent();
	m_SplattingBlue.AddString(m_BlueString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_BlueString;
	m_BlueString = DataPos;
	SplattingTexture[2] = (TCHAR*)(LPCTSTR)m_BlueString;
	pApp->Sample.m_vMapQuadTree[0]->SplattingTextureLoad(pApp->Sample.m_pd3dDevice, SplattingTexture[2], 2);
	pApp->Sample.m_mMapSplattingTexture[2] = SplattingTexture[2];
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedAlpha()
{
	m_AlphaString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // 상호참조로 모달리스를 해결하는 방법이 있으나 추후 연구
		if (m_TexFinder.DoModal() == IDOK)
		{
			if (m_TexFinder.m_TexFileName.size())
			{
				m_AlphaString = m_TexFinder.m_TexFileName[0];
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
	m_SplattingAlpha.ResetContent();
	m_SplattingAlpha.AddString(m_AlphaString);

	CString DataPos = L"../../data/";// 데이터 위치를 한정시킨다.
	DataPos += m_AlphaString;
	m_AlphaString = DataPos;
	SplattingTexture[3] = (TCHAR*)(LPCTSTR)m_AlphaString;
	pApp->Sample.m_vMapQuadTree[0]->SplattingTextureLoad(pApp->Sample.m_pd3dDevice, SplattingTexture[3], 3);
	pApp->Sample.m_mMapSplattingTexture[3] = SplattingTexture[3];
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// 몬스터 스폰지역 설정
void KTextureControlForm::OnBnClickedRadio5()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iObjSpreatingColor = vRed;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// 이동 불가지역 설정
void KTextureControlForm::OnBnClickedRadio6()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iObjSpreatingColor = vGreen;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// 아이템 스폰지역 설정
void KTextureControlForm::OnBnClickedRadio7()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iObjSpreatingColor = vBlue;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//지우기 설정
void KTextureControlForm::OnBnClickedRadio8()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iObjSpreatingColor = vAlpha;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// ObjSpreatingMode On
void KTextureControlForm::OnBnClickedButton7()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_vMapQuadTree[0]->CreateSRV(pApp->Sample.m_vMapQuadTree[0]->ObjPosTexture, pApp->Sample.m_vMapQuadTree[0]->StagingObjPosTexture, pApp->Sample.m_vMapQuadTree[0]->Objdesc);
	pApp->Sample.m_VCtrlState = vNone;
	pApp->Sample.m_VSpreatState = vSpreatNone;
	pApp->Sample.m_VObjSpreatState = vSpreatApply;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// ObjSpreatingMode Off
void KTextureControlForm::OnBnClickedButton8()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_VObjSpreatState = vSpreatNone;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedButton9()
{
	UpdateData(TRUE);
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_iSpreatingRange = m_iSpreatRange;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// 알파텍스쳐에 의한 랜더링 모드 ON
void KTextureControlForm::OnBnClickedButton10()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_MapSettingRenderMode = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 알파텍스쳐에 의한 랜더링 모드 Off
void KTextureControlForm::OnBnClickedButton11()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_MapSettingRenderMode = false;
	pApp->Sample.m_VSpreatState = vSpreatNone;
	pApp->Sample.m_VObjSpreatState = vSpreatNone;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KTextureControlForm::OnBnClickedButton12()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_MapSetting.SettingSoundInfo();
	pApp->Sample.m_MapSetting.SettingObjInfo();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// GradientMode On
void KTextureControlForm::OnBnClickedSpreatmodeon2()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_ModeGradient = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// GradientMode Off
void KTextureControlForm::OnBnClickedSpreatmodeoff2()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.m_ModeGradient = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




void KTextureControlForm::OnBnClickedA4()
{
	int k = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
