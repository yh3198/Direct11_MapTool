// KMapObjControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCA32MapTool.h"
#include "KMapObjControlForm.h"


// KMapObjControlForm

KMapObjControlForm* KMapObjControlForm::CreateOne(CWnd* pParent)
{
	KMapObjControlForm* pForm = new KMapObjControlForm;
	if (!pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL))
	{
		return NULL;
	}
	return pForm;
}


IMPLEMENT_DYNCREATE(KMapObjControlForm, CFormView)

KMapObjControlForm::KMapObjControlForm()
	: CFormView(IDD_KMAPOBJCONTROLFORM)
	, m_fObjScaleX(0)
	, m_fObjScaleY(0)
	, m_fObjScaleZ(0)
	, m_fObjRotationX(0)
	, m_fObjRotationY(0)
	, m_fObjRotationZ(0)
	, m_fObjTransX(0)
	, m_fObjTransY(0)
	, m_fObjTransZ(0)
{

}

KMapObjControlForm::~KMapObjControlForm()
{
}

void KMapObjControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, TMapObjBox);
	DDX_Text(pDX, IDC_EDIT3, m_fObjScaleX);
	DDX_Text(pDX, IDC_EDIT4, m_fObjScaleY);
	DDX_Text(pDX, IDC_EDIT5, m_fObjScaleZ);
	DDX_Text(pDX, IDC_EDIT6, m_fObjRotationX);
	DDX_Text(pDX, IDC_EDIT7, m_fObjRotationY);
	DDX_Text(pDX, IDC_EDIT8, m_fObjRotationZ);
	DDX_Text(pDX, IDC_EDIT9, m_fObjTransX);
	DDX_Text(pDX, IDC_EDIT10, m_fObjTransY);
	DDX_Text(pDX, IDC_EDIT11, m_fObjTransZ);
	DDX_Control(pDX, IDC_SLIDER3, m_CObjScaleX);
	DDX_Control(pDX, IDC_SLIDER4, m_CObjScaleY);
	DDX_Control(pDX, IDC_SLIDER5, m_CObjScaleZ);
	DDX_Control(pDX, IDC_SLIDER6, m_CObjRotationX);
	DDX_Control(pDX, IDC_SLIDER7, m_CObjRotationY);
	DDX_Control(pDX, IDC_SLIDER8, m_CObjRotationZ);
	DDX_Control(pDX, IDC_SLIDER9, m_CObjTransX);
	DDX_Control(pDX, IDC_SLIDER10, m_CObjTransY);
	DDX_Control(pDX, IDC_SLIDER11, m_CObjTransZ);
}

BEGIN_MESSAGE_MAP(KMapObjControlForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &KMapObjControlForm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KMapObjControlForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &KMapObjControlForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &KMapObjControlForm::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &KMapObjControlForm::ObjBoxAddList)
	ON_BN_CLICKED(IDC_BUTTON13, &KMapObjControlForm::ObjBoxDeleteList)
	ON_LBN_SELCHANGE(IDC_LIST1, &KMapObjControlForm::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON6, &KMapObjControlForm::OnObjectCreateMode)
	ON_BN_CLICKED(IDC_BUTTON14, &KMapObjControlForm::OnRotationApply)
	ON_BN_CLICKED(IDC_BUTTON15, &KMapObjControlForm::OnTranslateApply)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &KMapObjControlForm::OnNMReleasedcaptureSlider3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &KMapObjControlForm::OnNMReleasedcaptureSlider4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER5, &KMapObjControlForm::OnNMReleasedcaptureSlider5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER6, &KMapObjControlForm::OnNMReleasedcaptureSlider6)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER7, &KMapObjControlForm::OnNMReleasedcaptureSlider7)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER8, &KMapObjControlForm::OnNMReleasedcaptureSlider8)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER9, &KMapObjControlForm::OnNMReleasedcaptureSlider9)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER10, &KMapObjControlForm::OnNMReleasedcaptureSlider10)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER11, &KMapObjControlForm::OnNMReleasedcaptureSlider11)
	ON_EN_CHANGE(IDC_EDIT3, &KMapObjControlForm::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &KMapObjControlForm::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &KMapObjControlForm::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &KMapObjControlForm::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &KMapObjControlForm::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &KMapObjControlForm::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &KMapObjControlForm::OnEnChangeEdit9)
	ON_EN_CHANGE(IDC_EDIT10, &KMapObjControlForm::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT11, &KMapObjControlForm::OnEnChangeEdit11)
	ON_WM_MOUSEHOVER()
	ON_BN_CLICKED(IDC_BUTTON16, &KMapObjControlForm::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &KMapObjControlForm::OnBnClickedButton17)
END_MESSAGE_MAP()


// KMapObjControlForm 진단입니다.

#ifdef _DEBUG
void KMapObjControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KMapObjControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// KMapObjControlForm 메시지 처리기입니다.

// ObjCreateMode ON
void KMapObjControlForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	//m_CreateObjInfo
	if (pApp->Sample.m_MapObjMgr.size())
	{
		for (int iObj = 0; iObj < pApp->Sample.m_MapObjMgr.size(); iObj++)
		{
			if (pApp->Sample.m_MapObjMgr[iObj]->m_szName == (TCHAR*)(LPCTSTR)m_SelectedObj)
			{
				pApp->Sample.m_CreateObjType.resize(1);
				pApp->Sample.m_CreateObjType[0] = iObj;
			}
		}
	}
	
	pApp->Sample.ObjSettingMode = true;
	pApp->Sample.ObjSelectingMode = false;
	pApp->Sample.ObjMovingMode = false;
}

// ObjCreateMode OFF
void KMapObjControlForm::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_SelectedObj
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjSettingMode = false;
	pApp->Sample.ObjSelectingMode = false;
	pApp->Sample.ObjMovingMode = false;
}

// SelectingMode ON
void KMapObjControlForm::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjSelectingMode = true;
	pApp->Sample.ObjSettingMode = false;
	pApp->Sample.ObjMovingMode = false;
}

// SelectingMode OFF
void KMapObjControlForm::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjSelectingMode = false;
	pApp->Sample.ObjSettingMode = false;
	pApp->Sample.ObjMovingMode = false;
}


void KMapObjControlForm::ObjBoxAddList()
{
	CString selFileName;
	CString BoxListName;
	CFileDialog dlg(TRUE, L"*.ASE", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT, L"ASE Files(*.ASE)|*.ASE|All Files(*.*)|*.*|", this);
	POSITION Pos = dlg.GetStartPosition();
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (dlg.DoModal() == IDOK)
	{
		while(Pos)
		{
			BoxListName.Empty();
			selFileName = dlg.GetNextPathName(Pos);
			TCHAR Drive[MAX_PATH];
			TCHAR Dir[MAX_PATH];
			TCHAR FName[MAX_PATH];
			TCHAR Ext[MAX_PATH];
			_tsplitpath(selFileName, Drive, Dir, FName, Ext);
			BoxListName += FName;
			BoxListName += Ext;
			int CheckSameObj = TMapObjBox.FindStringExact(-1, BoxListName);
			if (CheckSameObj == LB_ERR)
			{
				pApp->Sample.m_MapObjTotalCnt++;
				pApp->Sample.m_MapObjMgr.resize(pApp->Sample.m_MapObjTotalCnt);
				pApp->Sample.m_DrawObjList.resize(pApp->Sample.m_MapObjTotalCnt);
				pApp->Sample.m_MapObjMgr[pApp->Sample.m_MapObjTotalCnt - 1] = new BAseObj;
				CString LoadObj;
				LoadObj += L"../../data/";
				LoadObj += BoxListName;
				pApp->Sample.m_MapObjMgr[pApp->Sample.m_MapObjTotalCnt - 1]->Load(pApp->Sample.m_pd3dDevice, (TCHAR*)(LPCTSTR)LoadObj);
				pApp->Sample.m_MapObjMgr[pApp->Sample.m_MapObjTotalCnt - 1]->Frame();
				TMapObjBox.AddString(BoxListName);
				
			}
			else
			{
				AfxMessageBox(_T("중복된 오브젝트명입니다."));
			}
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::ObjBoxDeleteList()
{
	int iCount = TMapObjBox.GetCurSel();
	if (iCount != LB_ERR)
	{
		TMapObjBox.DeleteString(iCount);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 오브젝트 리스트박스 이름 선택시
void KMapObjControlForm::OnLbnSelchangeList1()
{
	m_SelectedObj.Empty();

	int iCount = TMapObjBox.GetCurSel();
	if (iCount != LB_ERR)
	{
		TMapObjBox.GetText(iCount, m_SelectedObj);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnObjectCreateMode()//OnScaleApply
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (pApp->Sample.m_vSelectedObjInfo.size())
	{
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.x = m_fObjScaleX;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.y = m_fObjScaleY;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Scale.z = m_fObjScaleZ;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnRotationApply()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (pApp->Sample.m_vSelectedObjInfo.size())
	{
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[0] = m_fObjRotationX;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[1] = m_fObjRotationY;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Angle[2] = m_fObjRotationZ;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnTranslateApply()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	if (pApp->Sample.m_vSelectedObjInfo.size())
	{
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate.x = m_fObjTransX;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate.y = m_fObjTransY;
		pApp->Sample.m_MapObjMgr[pApp->Sample.m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[pApp->Sample.m_vSelectedObjInfo[0].m_iObjNum].Translate.z = m_fObjTransZ;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_CObjScaleX.SetRange(0.0f, 1000.0f);
	m_CObjScaleY.SetRange(0.0f, 1000.0f);
	m_CObjScaleZ.SetRange(0.0f, 1000.0f);
	m_fObjScaleX = 0.0f;
	m_fObjScaleY = 0.0f;
	m_fObjScaleZ = 0.0f;
	m_CObjScaleX.SetPos(m_fObjScaleX);
	m_CObjScaleY.SetPos(m_fObjScaleY);
	m_CObjScaleZ.SetPos(m_fObjScaleZ);

	m_CObjRotationX.SetRange(-360.0f, 360.0f);
	m_CObjRotationY.SetRange(-360.0f, 360.0f);
	m_CObjRotationZ.SetRange(-360.0f, 360.0f);
	m_fObjRotationX = 0.0f;
	m_fObjRotationY = 0.0f;
	m_fObjRotationZ = 0.0f;
	m_CObjRotationX.SetPos(50);
	m_CObjRotationY.SetPos(50);
	m_CObjRotationZ.SetPos(50);

	m_CObjTransX.SetRange(-128.0f, 128.0f);
	m_CObjTransY.SetRange(-200.0f, 200.0f);
	m_CObjTransZ.SetRange(-128.0f, 128.0f);
	m_fObjTransX = 0.0f;
	m_fObjTransY = 0.0f;
	m_fObjTransZ = 0.0f;
	m_CObjTransX.SetPos(35);
	m_CObjTransY.SetPos(40);
	m_CObjTransZ.SetPos(35);

	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

//Scale X Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjScaleX = m_CObjScaleX.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Scale Y Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjScaleY = m_CObjScaleY.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Scale Z Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjScaleZ = m_CObjScaleZ.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Rotation X Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjRotationX = m_CObjRotationX.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Rotation Y Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjRotationY = m_CObjRotationY.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Rotation Z Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjRotationZ = m_CObjRotationZ.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Translate X Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjTransX = m_CObjTransX.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Translate Y Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjTransY = m_CObjTransY.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Translate Z Slider
void KMapObjControlForm::OnNMReleasedcaptureSlider11(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_fObjTransZ = m_CObjTransZ.GetPos();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//Scale X value
void KMapObjControlForm::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjScaleX < 0)
		m_fObjScaleX = 0;
	if (m_fObjScaleX > 1000)
		m_fObjScaleX = 1000;
	UpdateData(FALSE);
	m_CObjScaleX.SetPos((int)m_fObjScaleX);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Scale Y value
void KMapObjControlForm::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjScaleY < 0)
		m_fObjScaleY = 0;
	if (m_fObjScaleY > 1000)
		m_fObjScaleY = 1000;
	UpdateData(FALSE);
	m_CObjScaleY.SetPos((int)m_fObjScaleY);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Scale Z value
void KMapObjControlForm::OnEnChangeEdit5()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjScaleZ < 0)
		m_fObjScaleZ = 0;
	if (m_fObjScaleZ > 1000)
		m_fObjScaleZ = 1000;
	UpdateData(FALSE);
	m_CObjScaleZ.SetPos((int)m_fObjScaleZ);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Rotation X value
void KMapObjControlForm::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjRotationX < -360)
		m_fObjRotationX = -360.0f;
	if (m_fObjRotationX > 360)
		m_fObjRotationX = 360.0f;
	UpdateData(FALSE);
	m_CObjRotationX.SetPos((int)m_fObjRotationX);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Rotation Y value
void KMapObjControlForm::OnEnChangeEdit7()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjRotationY < -360)
		m_fObjRotationY = -360.0f;
	if (m_fObjRotationY > 360)
		m_fObjRotationY = 360.0f;
	UpdateData(FALSE);
	m_CObjRotationY.SetPos((int)m_fObjRotationY);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Rotation Z value
void KMapObjControlForm::OnEnChangeEdit8()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjRotationZ < -360)
		m_fObjRotationZ = -360.0f;
	if (m_fObjRotationZ > 360)
		m_fObjRotationZ = 360.0f;
	UpdateData(FALSE);
	m_CObjRotationZ.SetPos((int)m_fObjRotationZ);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Translate X value
void KMapObjControlForm::OnEnChangeEdit9()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjTransX < -128)
		m_fObjTransX = -128.0f;
	if (m_fObjTransX > 128)
		m_fObjTransX = 128.0f;
	UpdateData(FALSE);
	m_CObjTransX.SetPos((int)m_fObjTransX);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Translate Y value
void KMapObjControlForm::OnEnChangeEdit10()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjTransY < -128)
		m_fObjTransY = -128.0f;
	if (m_fObjTransY > 128)
		m_fObjTransY = 128.0f;
	UpdateData(FALSE);
	m_CObjTransY.SetPos((int)m_fObjTransY);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Translate Z value
void KMapObjControlForm::OnEnChangeEdit11()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	if (m_fObjTransZ < -128)
		m_fObjTransZ = -128.0f;
	if (m_fObjTransZ > 128)
		m_fObjTransZ = 128.0f;
	UpdateData(FALSE);
	m_CObjTransZ.SetPos((int)m_fObjTransZ);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFormView::OnMouseHover(nFlags, point);
}


//DROPEFFECT KMapObjControlForm::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	return CFormView::OnDragOver(pDataObject, dwKeyState, point);
//}


void KMapObjControlForm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	int iCheck = 0;

	for (int iObj = 0; iObj < pApp->Sample.m_MapObjMgr.size(); iObj++)
	{
		iCheck = TMapObjBox.FindStringExact(-1, (LPCTSTR)pApp->Sample.m_MapObjMgr[iObj]->m_szName.c_str());
		if (iCheck == LB_ERR)
		{
			TMapObjBox.AddString((LPCTSTR)pApp->Sample.m_MapObjMgr[iObj]->m_szName.c_str());
		}
	}
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//ObjMovingMode On
void KMapObjControlForm::OnBnClickedButton16()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjMovingMode = true;
	pApp->Sample.ObjSelectingMode = false;
	pApp->Sample.ObjSettingMode = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KMapObjControlForm::OnBnClickedButton17()
{
	CKGCA32MapToolApp* pApp = (CKGCA32MapToolApp*)AfxGetApp();
	pApp->Sample.ObjMovingMode = false;
	pApp->Sample.ObjSelectingMode = false;
	pApp->Sample.ObjSettingMode = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
