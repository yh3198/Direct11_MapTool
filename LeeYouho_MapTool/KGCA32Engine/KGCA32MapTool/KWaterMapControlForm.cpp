// KWaterMapControlForm.cpp : ���� �����Դϴ�.
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


// KWaterMapControlForm �����Դϴ�.

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


// KWaterMapControlForm �޽��� ó�����Դϴ�.


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

	CString pDataPos = L"../../data/";// ������ ��ġ�� ������Ų��.
	pDataPos += m_WaterTexString;
	m_WaterTexString = pDataPos;
	m_WaterTex = (TCHAR*)(LPCTSTR)m_WaterTexString;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_WaterNormalTexString = L"HR01_N.dds";
	m_strWNTexControl.ResetContent();
	m_strWNTexControl.AddString(m_WaterNormalTexString);

	CString DataPos = L"../../data/";// ������ ��ġ�� ������Ų��.
	DataPos += m_WaterNormalTexString;
	m_WaterNormalTexString = DataPos;
	m_WaterNormalTex = (TCHAR*)(LPCTSTR)m_WaterNormalTexString;

	UpdateData(FALSE);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void KWaterMapControlForm::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fWaterHeight = m_WaterHeight.GetPos();
	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void KWaterMapControlForm::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void KWaterMapControlForm::OnStnClickedMaptoolfunc4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// water �̹��� �˻� ��
void KWaterMapControlForm::OnBnClickedFindtex()
{
	m_WaterTexString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // ��ȣ������ ��޸����� �ذ��ϴ� ����� ������ ���� ����
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
	// ������ 0���� ����ϰ� �־ 0������ ������ �������� �����ϰ� �Ǹ� �浹üũ�� �ؼ� �浹�� ����Ʈ���� ã�Ƽ� �־���� �Ѵ�.
	m_strWTexControl.ResetContent();
	m_strWTexControl.AddString(m_WaterTexString);

	CString DataPos = L"../../data/";// ������ ��ġ�� ������Ų��.
	DataPos += m_WaterTexString;
	m_WaterTexString = DataPos;
	m_WaterTex = (TCHAR*)(LPCTSTR)m_WaterTexString;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// Normal �̹��� �˻� ��
void KWaterMapControlForm::OnBnClickedFindtex3()
{
	m_WaterNormalTexString.Empty();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (m_TexFinder.GetSafeHwnd() == NULL)
	{
		/*m_TexFinder.Create(IDD_KTEXTUREFINDER);
		m_TexFinder.ShowWindow(SW_SHOW);*/ // ��ȣ������ ��޸����� �ذ��ϴ� ����� ������ ���� ����
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
	// ������ 0���� ����ϰ� �־ 0������ ������ �������� �����ϰ� �Ǹ� �浹üũ�� �ؼ� �浹�� ����Ʈ���� ã�Ƽ� �־���� �Ѵ�.
	m_strWNTexControl.ResetContent();
	m_strWNTexControl.AddString(m_WaterNormalTexString);

	CString DataPos = L"../../data/";// ������ ��ġ�� ������Ų��.
	DataPos += m_WaterNormalTexString;
	m_WaterNormalTexString = DataPos;
	m_WaterNormalTex = (TCHAR*)(LPCTSTR)m_WaterNormalTexString;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// ���� ���� ��ư�� ������ ��
void KWaterMapControlForm::OnBnClickedButton1()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	// �Ѱ��ְ� �ִ� ���� 0�� 0��° water�� �������� ��Ÿ����.
	// ���� �� ������ �����ϸ� ���õ� ���� ��ȣ�� �޾ƿ;� �Ѵ�.
	if (!pApp->Sample.m_vMapQuadTree.size()) // ���� ���ٸ� �����Ұ�
	{
		return;
	}
	pApp->Sample.CreateWaterMap(pApp->Sample.m_bSelectedMap, /* pApp->Sample.m_bSelectedMap->m_iMaxDepthLimit*/ m_fWaterWave, pApp->Sample.m_bSelectedMap->m_fMinDivideSize,
								0, m_WaterTex, m_WaterNormalTex, m_fWaterHeight, m_fWaterWave);
	m_CreatedWaterList.AddString(L"������ WaterMap");
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// Height�� ����Ǿ��� ��
void KWaterMapControlForm::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	UpdateData(TRUE);
	if (m_fWaterHeight < -10)
		m_fWaterHeight = -10;
	if (m_fWaterHeight > 10)
		m_fWaterHeight = 10;
	UpdateData(FALSE);
	m_WaterHeight.SetPos((int)m_fWaterHeight);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void KWaterMapControlForm::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(TRUE);
	if (m_fWaterWave < 1)
		m_fWaterWave = 1;
	if (m_fWaterWave > 4)
		m_fWaterWave = 4;
	UpdateData(FALSE);
	m_WaterWave.SetPos((int)m_fWaterWave);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}