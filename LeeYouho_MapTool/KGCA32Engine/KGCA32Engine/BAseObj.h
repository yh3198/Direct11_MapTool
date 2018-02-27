#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "BParser.h"
#include "BObject.h"
#include "BLine.h"

//====================================================================================//
/*ASE 파일을 로드, 변환, 오브젝트화 생성, 랜더를 담당하는 클래스 입니다.*/
/*애니메이션 미적용일 경우 Frame은 생성시 1회만 호출하면 됩니다.*/
/*로드 변환작업이 종료되면 BMesh2의 형태로 m_pData에 값이 생성되게 됩니다.*/
/*인스턴싱이 적용중입니다. 랜더는 오브젝트와 방식이 동일합니다.*/
//====================================================================================//


#pragma region ASE로드 관련 구조체
const enum OBJECTCLASSTYPE 
{
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

static TCHAR* AseSectionHeaderTokens[] =
{
	_T("*SCENE"),
	_T("*MATERIAL_LIST"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT")		// 리턴용
};
enum AseSectionType { SCENE, MATERIAL_LIST, GEOM_OBJECT, HELPER_OBJECT,};

static TCHAR* AseMaterialTokens[] =
{
	_T("*MATERIAL"),
	_T("*SUBMATERIAL"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseMaterialType { MATERIAL = 0, SUBMATERIAL, MAP_SUBNO, BITMAP_FILE, RETURN_GEOM_OBJECT, RETURN_HELPER_OBJECT,};

static TCHAR* AseObjectTokens[] = 
{
	_T("*NODE_NAME"),
	_T("*NODE_PARENT"),
	_T("*NODE_TM"),
	_T("*MESH"),
	_T("*TM_ANIMATION"),
	_T("*TM_VISIBILITY"),
	_T("*MATERIAL_REF"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseObjectType {NODE_NAME = 0, PARENT_NAME, NODE_TM, MESH, TM_ANIMATION, TM_VISIBILITY,	MATERIAL_REF, NEW_GEOM_OBJECT, NEW_HELPER_OBJECT,};

static TCHAR* AseHelperObjectTokens[] =
{
	_T("*NODE_NAME"),
	_T("*NODE_PARENT"),
	_T("*HELPER_CLASS"),
	_T("*NODE_TM"),
	_T("*BOUNDINGBOX_MIN"),
	_T("*TM_ANIMATION"),

	_T("*GEOMOBJECT"),	// Return
	_T("*HELPEROBJECT"), // Return
};
enum AseHelperObjectType {HELPER_NODE_NAME = 0, HELPER_NODE_PARENT, HELPER_CLASS, HELPER_MODE_TM,	BOUNDINGBOX_MIN, HELPER_TM_ANIMATION, HELPER_GEOM_OBJECT, HELPER_HELPER_OBJECT};

static TCHAR* AseAniTokens[] =
{
	_T("*CONTROL_POS_SAMPLE"),
	_T("*CONTROL_ROT_SAMPLE"),
	_T("*CONTROL_SCALE_SAMPLE"),
	_T("*MATERIAL_REF"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseAniType { CONTROL_POS_SAMPLE = 70, CONTROL_ROT_SAMPLE, CONTROL_SCL_SAMPLE, TM_MATERIAL_REF, NEW_GEOM, NEW_HELPER,};


/*애니메이션 정보 구조체*/
struct BScene
{
	int iVersion;		// 버전
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
	int iNumMesh;		// 메쉬오브젝트 개수
	int iMaxWeight;		// 정점 당 가중치
	int iBindPose;		// 바인딩 포즈 애니메이션 여부
};

/*텍스쳐 구조체*/
struct BTextexMap
{
	DWORD m_dwIndex;				// 오브젝트 텍스쳐 관리자에서 메터리얼의 텍스쳐 파일명을 확인 후 DX텍스쳐 생성, 참조 인덱스 저장
	DWORD m_dwType;					// 텍스쳐 맵 타입 인덱스
	T_STR m_strName;				// 텍스쳐 이름
	T_STR m_strClassName;			// 텍스쳐 클래스 이름
	T_STR m_strTextureName;			// 텍스쳐 이름
	T_STR m_strNormalTextureName;	// 노말 텍스쳐 이름
	BTextexMap() : m_dwType(1), m_dwIndex(0) {}
};

/*메터리얼 정보 구조체*/
struct BMtrl
{
	T_STR m_strName;			 // 오브젝트명
	T_STR m_strClassName;		 // 오브젝트 클래스(종류) 명
	vector<BTextexMap> m_TexMaps;// 텍스쳐 타입들
	vector<BMtrl> m_SubMaterial; // 서브 메터리얼 저장벡터
	DWORD m_dwSubCount;			 // 서브 메터리얼 수
	DWORD m_dwTexMapCount;		 // 텍스쳐타입 수
	DWORD m_dwIndex;			 // 오브젝트 순서

	BMtrl() {}
	~BMtrl() { stl_wipe_vector(m_SubMaterial); stl_wipe_vector(m_TexMaps); }
};

struct BFaceList
{
	union
	{
		struct { DWORD _0, _1, _2; };
		DWORD v[3];
	};
	DWORD dwMtrl;

	BFaceList() : dwMtrl(0), _0(0), _1(0), _2(0) {}
};


/*버택스 리스트 구조체*/
struct BVertexList
{
	DWORD dwNumVertex;
	DWORD dwNumFace;
	vector<D3DXVECTOR3> pVertexList;
	vector<BFaceList> pFaceList;
	vector<DWORD> pSubListMtl;

	BVertexList() : dwNumVertex(0), dwNumFace(0) {}
	~BVertexList() { stl_wipe_vector(pVertexList); stl_wipe_vector(pVertexList); }
};

///*HelperObject 구조체(미사용)*/
//struct B_BOX
//{
//	D3DXVECTOR3 vCenter;
//	D3DXVECTOR3 vPos[8];
//
//	D3DXVECTOR3 vMax;
//	D3DXVECTOR3 vMin;
//
//	D3DXVECTOR3 vAxis[3];
//	float fExtent[3];
//};

/*오브젝트 정보 구조체*/
struct BGeomMesh
{
	T_STR m_strNodeName;
	T_STR m_strParentName;
	OBJECTCLASSTYPE m_ClassType;
	D3DXMATRIX m_matWorld;
	DWORD m_iNumFace;
	int m_iMtrlRef;
	BVertexList			m_PosVertexList;
	BVertexList			m_TexVertexList;
	BVertexList			m_ColVertexList;
	vector<D3DXVECTOR3>	m_pNorVertexList;

	D3DXMATRIX m_matWorldTrans;
	D3DXMATRIX m_matWorldRotate;
	D3DXMATRIX m_matWorldScale;

	vector<BAnimTrack>		m_PosTrack;		// 이동 트랙 
	vector<BAnimTrack>		m_RotTrack;		// 회전 트랙
	vector<BAnimTrack>		m_SclTrack;		// 신축 트랙												
	vector<BAnimTrack>		m_VisTrack;		// 알파 트랙

	B_BOX m_BBox;

	BGeomMesh()
	{
		m_iNumFace = 0;
		m_iMtrlRef = -1;
	}
};
#pragma endregion 
#pragma region 오브젝트 정보 구조체
class BMesh2
{
public:
	vector<shared_ptr<BMesh2>>	m_pSubMesh;							// 서브 매쉬
	BMesh2*						m_pParent;							// 부모 매쉬
	vector<BMesh2*>				m_pChildMesh;						// 자식 매쉬
	OBJECTCLASSTYPE				m_ClassType;						// 매쉬 타입
	int							m_iDiffuseTex[10];					// 적용중인 텍스쳐 인덱스 배열
	D3DXMATRIX					m_matWorld;							// 월드행렬
	D3DXMATRIX					m_matInverse;						// 월드 역행렬
	T_STR						m_strNodeName;						// 노드명
	T_STR						m_strParentName;					// 부모노드명
	T_STR						m_TextureName;						// 텍스쳐명
	T_STR						m_NormalTextureName;				// 노말텍스쳐명
	int							m_iNumFace;							// 페이스 갯수값
	vector<PNCT_Vertex>			m_FaceList;							// 페이스리스트
	vector<PNCT_Vertex>			m_VertexArray;						// 버텍스리스트
	vector<DWORD>				m_IndexArray;						// 인덱스리스트
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;					// 버텍스버퍼
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;						// 인덱스버퍼
	vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;			// 텍스쳐 리소스
	vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;	// 노말텍스쳐 리소스
	

	bool	   m_bUsed;				// 애니메이션 사용여부 변수
	D3DXMATRIX m_matWorldScale;		// 월드행렬 스케일		
	D3DXMATRIX m_matWorldTrans;		// 월드행렬 이동
	D3DXMATRIX m_matWorldRotate;	// 월드행렬 회전
	D3DXMATRIX m_matCalculation;	// 월드행렬 최종연산

	vector<shared_ptr<BAnimTrack>> m_pPosTrack;	// 위치트랙
	vector<shared_ptr<BAnimTrack>> m_pRotTrack;	// 회전트랙
	vector<shared_ptr<BAnimTrack>> m_pSclTrack;	// 크기트랙
	vector<shared_ptr<BAnimTrack>> m_pVisTrack; // 가중치관련 (현재 미사용)
	float m_fVisibility;						// 가중치관련 (현재 미사용)
	
	vector<D3DXVECTOR3>		m_TangentList;		// 접선벡터 리스트
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// 접선벡터 버퍼
	vector<D3DXVECTOR3>		m_FaceNormals;		// 정점노말
	vector<int> m_Normallookuptable;			// 룩업테이블

	B_BOX m_tBox;								// 충돌 전용 박스

	/*충돌박스 갱신함수*/
	void SetUniqueBuffer();
	/*버텍스 중복확인 연산 함수*/
	int IsEqulVertexList(vector<PNCT_Vertex>&  VertexArray, PNCT_Vertex& Vertex);
	

	BMesh2::BMesh2()
	{
		//m_iDiffuseTex = -1;
		memset(m_iDiffuseTex, -1, sizeof(m_iDiffuseTex));
		D3DXMatrixIdentity(&m_matWorld);
		m_pParent = 0;
		D3DXMatrixIdentity(&m_matWorldTrans);
		D3DXMatrixIdentity(&m_matWorldRotate);
		D3DXMatrixIdentity(&m_matWorldScale);
		D3DXMatrixIdentity(&m_matCalculation);
	}
};
#pragma endregion 

class BAseObj : public BObject
{
public:
	/*오브젝트 정보 변수*/
	T_STR				m_szDirName;			// 경로명
	T_STR				m_szName;				// 파일명
	BParser				m_Parser;				// Ase 해석을 위한 클래스
	BScene				m_Scene;				// 애니메이션 정보
	vector<BMtrl>		m_Material;				// 메터리얼 정보
	vector<BGeomMesh>	m_GeomObject;			// 오브젝트 정보
	
	vector<shared_ptr<BMesh2>> m_pData;	 // 실제 오브젝트 데이터

	/*오브젝트 애니메이션 정보 변수*/
	float m_fElapseTime;				 // 경과시간
	int m_iStartFrame;					 // 시작시간
	int m_iLastFrame;					 // 소멸시간
	
	/*인스턴싱 정보 변수*/
	ComPtr<ID3D11Buffer> m_BufferInstance;	// 인스턴스 버퍼
	vector<D3DXMATRIX> m_pInstanceList;		// 인스턴스 정보리스트
	vector<D3DXMATRIX> m_pInvInstaneList;	// 인스턴스 역행렬 정보리스트 (노말맵핑시 사용)
	vector<B_BOX> m_tBoundingBox;			// 충돌박스
	vector<BLine> m_BoxLine;				// 충돌박스 랜더링용 라인
	vector<InstancingInfo> m_InstanceInfo;	// 인스턴싱 상태 상세정보
	int m_iInstanceCount;					// 인스턴싱 수량

	vector<int> m_DrawInstanceList;			// 랜더링 인스턴싱 인덱스리스트
	vector<D3DXMATRIX> m_pDrawInstanceList;	// 랜더링 인스턴싱 정보리스트

	/*노말맵핑 정보 변수*/
	NORMAL_CONSTANT_BUFFER  m_cbNormal;		// 노말맵핑 구성정보
	ID3D11Buffer*			m_pNormalCB;	// 노말맵핑 버퍼

	/*쉐도우 행렬 변수*/
	D3DXMATRIX m_matShadowView;				// 쉐도우 뷰행렬
	D3DXMATRIX m_matShadowProj;				// 쉐도우 투영행렬

public:
	/*기본 구성 함수*/
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext, int DrawCount);
	bool Release();


	//=============================================//
	/*파일 로드 함수*/
	bool Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName = NULL);
	bool Load(const TCHAR* strFileName, ID3D11Device* pd3dDevice);
	//=============================================//
	//=============================================//
	/*파일 로드 보조함수(Load 함수 내부 호출)*/
	bool LoadScene();
	bool LoadMaterial();
	bool LoadObject();
	bool LoadHelperObject();
	bool LoadTM(BGeomMesh* pObject);
	bool LoadAnimation(BGeomMesh* pGeomObj);
	//=============================================//
	/*버텍스 정보 로드함수*/
	bool GetVertexListFromString(TCHAR* strVertexList, DWORD dwNumFace, vector<D3DXVECTOR3>& VertexList, TCHAR* strToken = _T("*"));
	/*페이스 정보 로드함수(칼라, 노말)*/
	bool GetFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken = _T("*"));
	/*페이스 정보 로드함수(포지션)*/
	bool GetPositionFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken = _T("*"));
	/*노말 정보 로드 함수*/
	bool GetNormalListFromString(TCHAR* strFaceList, DWORD dwNumFace, BGeomMesh* GeomObject, TCHAR* strToken = _T("*"));
	/*토큰 검색 함수 (Load 내부 호출)*/
	int SearchTokenArray(INT NumObjType, TCHAR** pObjectType);


	/*정보 변환 함수*/
	bool Convert(ID3D11Device* pd3dDevice);
	/*0프레임 애니메이션 생성 함수(Convert 내부 호출)*/
	bool InheriteCollect();
	/*부모노드 검색함수 (InheriteCollect 내부 호출)*/
	BMesh2* SearchToCollects(T_STR m_strParentName);
	/*인덱스 최적화 함수 (서브메터리얼 미포함)*/
	bool SetTriangleBuffer(BMesh2* pMesh, BGeomMesh* pAseMesh, DWORD dwSubMtrl = 0, DWORD dwMask = 0x00000000);
	/*인덱스 최적화 함수 (서브메터리얼 포함)*/
	bool SetTriangleSubBuffer(vector<shared_ptr<BMesh2>>& pSubMesh, BGeomMesh* pAseMesh);

	/*재정의 # 매쉬 오브젝트 생성함수*/
	HRESULT Create(ID3D11Device* pDevice);
	/*재정의 # 매쉬 버텍스 생성함수*/
	HRESULT CreateVertexBuffer();
	/*재정의 # 매쉬 인덱스 생성함수*/
	HRESULT CreateIndexBuffer();
	/*재정의 # 매쉬 텍스쳐(쉐이더리소스) 생성함수*/
	HRESULT TextureLoad(ID3D11Device* pDevice);
	
	
	/*랜더링 카메라 세팅함수*/
	bool SetCamera(D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);
	/*랜더링 행렬 세팅함수*/
	void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	/*랜더링 인스턴싱 리스트 갱신함수*/
	bool SetDrawObj(vector<int> DrawList);
	/*전체 매쉬 랜더함수*/	
	bool Draw(ID3D11DeviceContext* pContext, BAseObj* pParent);
	/*부분 매쉬 랜더함수*/
	void DrawMesh(ID3D11DeviceContext* pContext, BMesh2* pMesh);
	/*인스턴싱 매쉬 랜더함수*/
	bool DrawInstance(ID3D11DeviceContext* pContext, BAseObj* pParent, int DrawCount);


	/*텍스쳐 로드 함수(단일)*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);
	/*텍스쳐 로드 함수(다중)*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);
	/*노말맵핑 텍스쳐 로드 함수*/
	void NormalTextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);

	/*애니메이션 프레임 링크생성 함수(더블 링크드리스트)*/
	BAnimTrack* SetDoublyLinkedList(BAnimTrack* pCurrentTrack, BAnimTrack* pPrev);
	/*애니메이션 프레임 기반 최종행렬 생성(인터폴레이션)함수*/
	void Interpolate(BMesh2* pMesh, D3DXMATRIX* matParent, float fFrameTick);
	/*애니메이션 트랙 반환 함수*/
	//pEndTrack이 없으면 flase를 리턴 (보간할 대상이 없을 경우)
	bool GetAnimationTrack(float fFrame, vector<shared_ptr<BAnimTrack>> pTrackList, BAnimTrack** pStartTrack, BAnimTrack** pEndTrack);
	

	/*재정의 # 인스턴싱 전용 쉐이더 레이아웃 생성함수*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*인스턴스 버퍼, 정보 생성함수*/
	void InitInstanceBuffer(int iInstanceCount, int ObjNum);
	/*인스턴스 버퍼 생성 함수 (InitInstanceBuffer 내부 호출)*/
	HRESULT CreateInstanceBuffer(UINT ListSize, UINT InstnaceSize, vector<D3DXMATRIX> m_pInstance);
	/*인스턴스 정보 갱신함수*/
	void UpdateInstanceInfo(ID3D11DeviceContext* pContext, vector<InstancingInfo> InstanceInfo, BMesh2* MeshData);


	/*충돌박스 생성 함수*/
	bool CreateBox();
	/*충돌박스 정보 갱신함수*/
	void UpdateBoundingBox();
	/*충돌박스 랜더링 함수*/
	bool RenderBoundingBoxLine(ID3D11DeviceContext* m_pImmediateContext, D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);

	
	/*노말맵핑 정보 상수버퍼 생성함수*/
	HRESULT CreateNormalCB();
	/*노말맵핑용 카메라 위치, 시선벡터 설정함수*/
	void SettingCameraForNormal(D3DXVECTOR3 Pos, D3DXVECTOR3 Look);
	/*재정의 # 접선벡터 버퍼 갱신함수 (UpdateNormal 내부 호출)*/
	bool UpDateTangentBuffer();
	/*재정의 # 페이스 노말 생성함수 (UpdateNormal 내부 호출)*/
	bool InitFaceNormal();
	/*룩업테이블(인덱스) 생성함수 (UpdateNormal 내부 호출)*/
	void GenNormalLookupTable();
	/*페이스노말 기반 버텍스 노말벡터 연산함수 (UpdateNormal 내부 호출)*/
	void CalcPerVertexNormalsFastLookup();
	
public:
	BAseObj();
	virtual ~BAseObj();
};

