#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "BParser.h"
#include "BObject.h"
#include "BLine.h"

//====================================================================================//
/*ASE ������ �ε�, ��ȯ, ������Ʈȭ ����, ������ ����ϴ� Ŭ���� �Դϴ�.*/
/*�ִϸ��̼� �������� ��� Frame�� ������ 1ȸ�� ȣ���ϸ� �˴ϴ�.*/
/*�ε� ��ȯ�۾��� ����Ǹ� BMesh2�� ���·� m_pData�� ���� �����ǰ� �˴ϴ�.*/
/*�ν��Ͻ��� �������Դϴ�. ������ ������Ʈ�� ����� �����մϴ�.*/
//====================================================================================//


#pragma region ASE�ε� ���� ����ü
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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT")		// ���Ͽ�
};
enum AseSectionType { SCENE, MATERIAL_LIST, GEOM_OBJECT, HELPER_OBJECT,};

static TCHAR* AseMaterialTokens[] =
{
	_T("*MATERIAL"),
	_T("*SUBMATERIAL"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
};
enum AseAniType { CONTROL_POS_SAMPLE = 70, CONTROL_ROT_SAMPLE, CONTROL_SCL_SAMPLE, TM_MATERIAL_REF, NEW_GEOM, NEW_HELPER,};


/*�ִϸ��̼� ���� ����ü*/
struct BScene
{
	int iVersion;		// ����
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
	int iNumMesh;		// �޽�������Ʈ ����
	int iMaxWeight;		// ���� �� ����ġ
	int iBindPose;		// ���ε� ���� �ִϸ��̼� ����
};

/*�ؽ��� ����ü*/
struct BTextexMap
{
	DWORD m_dwIndex;				// ������Ʈ �ؽ��� �����ڿ��� ���͸����� �ؽ��� ���ϸ��� Ȯ�� �� DX�ؽ��� ����, ���� �ε��� ����
	DWORD m_dwType;					// �ؽ��� �� Ÿ�� �ε���
	T_STR m_strName;				// �ؽ��� �̸�
	T_STR m_strClassName;			// �ؽ��� Ŭ���� �̸�
	T_STR m_strTextureName;			// �ؽ��� �̸�
	T_STR m_strNormalTextureName;	// �븻 �ؽ��� �̸�
	BTextexMap() : m_dwType(1), m_dwIndex(0) {}
};

/*���͸��� ���� ����ü*/
struct BMtrl
{
	T_STR m_strName;			 // ������Ʈ��
	T_STR m_strClassName;		 // ������Ʈ Ŭ����(����) ��
	vector<BTextexMap> m_TexMaps;// �ؽ��� Ÿ�Ե�
	vector<BMtrl> m_SubMaterial; // ���� ���͸��� ���庤��
	DWORD m_dwSubCount;			 // ���� ���͸��� ��
	DWORD m_dwTexMapCount;		 // �ؽ���Ÿ�� ��
	DWORD m_dwIndex;			 // ������Ʈ ����

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


/*���ý� ����Ʈ ����ü*/
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

///*HelperObject ����ü(�̻��)*/
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

/*������Ʈ ���� ����ü*/
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

	vector<BAnimTrack>		m_PosTrack;		// �̵� Ʈ�� 
	vector<BAnimTrack>		m_RotTrack;		// ȸ�� Ʈ��
	vector<BAnimTrack>		m_SclTrack;		// ���� Ʈ��												
	vector<BAnimTrack>		m_VisTrack;		// ���� Ʈ��

	B_BOX m_BBox;

	BGeomMesh()
	{
		m_iNumFace = 0;
		m_iMtrlRef = -1;
	}
};
#pragma endregion 
#pragma region ������Ʈ ���� ����ü
class BMesh2
{
public:
	vector<shared_ptr<BMesh2>>	m_pSubMesh;							// ���� �Ž�
	BMesh2*						m_pParent;							// �θ� �Ž�
	vector<BMesh2*>				m_pChildMesh;						// �ڽ� �Ž�
	OBJECTCLASSTYPE				m_ClassType;						// �Ž� Ÿ��
	int							m_iDiffuseTex[10];					// �������� �ؽ��� �ε��� �迭
	D3DXMATRIX					m_matWorld;							// �������
	D3DXMATRIX					m_matInverse;						// ���� �����
	T_STR						m_strNodeName;						// ����
	T_STR						m_strParentName;					// �θ����
	T_STR						m_TextureName;						// �ؽ��ĸ�
	T_STR						m_NormalTextureName;				// �븻�ؽ��ĸ�
	int							m_iNumFace;							// ���̽� ������
	vector<PNCT_Vertex>			m_FaceList;							// ���̽�����Ʈ
	vector<PNCT_Vertex>			m_VertexArray;						// ���ؽ�����Ʈ
	vector<DWORD>				m_IndexArray;						// �ε�������Ʈ
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;					// ���ؽ�����
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;						// �ε�������
	vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;			// �ؽ��� ���ҽ�
	vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;	// �븻�ؽ��� ���ҽ�
	

	bool	   m_bUsed;				// �ִϸ��̼� ��뿩�� ����
	D3DXMATRIX m_matWorldScale;		// ������� ������		
	D3DXMATRIX m_matWorldTrans;		// ������� �̵�
	D3DXMATRIX m_matWorldRotate;	// ������� ȸ��
	D3DXMATRIX m_matCalculation;	// ������� ��������

	vector<shared_ptr<BAnimTrack>> m_pPosTrack;	// ��ġƮ��
	vector<shared_ptr<BAnimTrack>> m_pRotTrack;	// ȸ��Ʈ��
	vector<shared_ptr<BAnimTrack>> m_pSclTrack;	// ũ��Ʈ��
	vector<shared_ptr<BAnimTrack>> m_pVisTrack; // ����ġ���� (���� �̻��)
	float m_fVisibility;						// ����ġ���� (���� �̻��)
	
	vector<D3DXVECTOR3>		m_TangentList;		// �������� ����Ʈ
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// �������� ����
	vector<D3DXVECTOR3>		m_FaceNormals;		// �����븻
	vector<int> m_Normallookuptable;			// ������̺�

	B_BOX m_tBox;								// �浹 ���� �ڽ�

	/*�浹�ڽ� �����Լ�*/
	void SetUniqueBuffer();
	/*���ؽ� �ߺ�Ȯ�� ���� �Լ�*/
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
	/*������Ʈ ���� ����*/
	T_STR				m_szDirName;			// ��θ�
	T_STR				m_szName;				// ���ϸ�
	BParser				m_Parser;				// Ase �ؼ��� ���� Ŭ����
	BScene				m_Scene;				// �ִϸ��̼� ����
	vector<BMtrl>		m_Material;				// ���͸��� ����
	vector<BGeomMesh>	m_GeomObject;			// ������Ʈ ����
	
	vector<shared_ptr<BMesh2>> m_pData;	 // ���� ������Ʈ ������

	/*������Ʈ �ִϸ��̼� ���� ����*/
	float m_fElapseTime;				 // ����ð�
	int m_iStartFrame;					 // ���۽ð�
	int m_iLastFrame;					 // �Ҹ�ð�
	
	/*�ν��Ͻ� ���� ����*/
	ComPtr<ID3D11Buffer> m_BufferInstance;	// �ν��Ͻ� ����
	vector<D3DXMATRIX> m_pInstanceList;		// �ν��Ͻ� ��������Ʈ
	vector<D3DXMATRIX> m_pInvInstaneList;	// �ν��Ͻ� ����� ��������Ʈ (�븻���ν� ���)
	vector<B_BOX> m_tBoundingBox;			// �浹�ڽ�
	vector<BLine> m_BoxLine;				// �浹�ڽ� �������� ����
	vector<InstancingInfo> m_InstanceInfo;	// �ν��Ͻ� ���� ������
	int m_iInstanceCount;					// �ν��Ͻ� ����

	vector<int> m_DrawInstanceList;			// ������ �ν��Ͻ� �ε�������Ʈ
	vector<D3DXMATRIX> m_pDrawInstanceList;	// ������ �ν��Ͻ� ��������Ʈ

	/*�븻���� ���� ����*/
	NORMAL_CONSTANT_BUFFER  m_cbNormal;		// �븻���� ��������
	ID3D11Buffer*			m_pNormalCB;	// �븻���� ����

	/*������ ��� ����*/
	D3DXMATRIX m_matShadowView;				// ������ �����
	D3DXMATRIX m_matShadowProj;				// ������ �������

public:
	/*�⺻ ���� �Լ�*/
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext, int DrawCount);
	bool Release();


	//=============================================//
	/*���� �ε� �Լ�*/
	bool Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName = NULL);
	bool Load(const TCHAR* strFileName, ID3D11Device* pd3dDevice);
	//=============================================//
	//=============================================//
	/*���� �ε� �����Լ�(Load �Լ� ���� ȣ��)*/
	bool LoadScene();
	bool LoadMaterial();
	bool LoadObject();
	bool LoadHelperObject();
	bool LoadTM(BGeomMesh* pObject);
	bool LoadAnimation(BGeomMesh* pGeomObj);
	//=============================================//
	/*���ؽ� ���� �ε��Լ�*/
	bool GetVertexListFromString(TCHAR* strVertexList, DWORD dwNumFace, vector<D3DXVECTOR3>& VertexList, TCHAR* strToken = _T("*"));
	/*���̽� ���� �ε��Լ�(Į��, �븻)*/
	bool GetFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken = _T("*"));
	/*���̽� ���� �ε��Լ�(������)*/
	bool GetPositionFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken = _T("*"));
	/*�븻 ���� �ε� �Լ�*/
	bool GetNormalListFromString(TCHAR* strFaceList, DWORD dwNumFace, BGeomMesh* GeomObject, TCHAR* strToken = _T("*"));
	/*��ū �˻� �Լ� (Load ���� ȣ��)*/
	int SearchTokenArray(INT NumObjType, TCHAR** pObjectType);


	/*���� ��ȯ �Լ�*/
	bool Convert(ID3D11Device* pd3dDevice);
	/*0������ �ִϸ��̼� ���� �Լ�(Convert ���� ȣ��)*/
	bool InheriteCollect();
	/*�θ��� �˻��Լ� (InheriteCollect ���� ȣ��)*/
	BMesh2* SearchToCollects(T_STR m_strParentName);
	/*�ε��� ����ȭ �Լ� (������͸��� ������)*/
	bool SetTriangleBuffer(BMesh2* pMesh, BGeomMesh* pAseMesh, DWORD dwSubMtrl = 0, DWORD dwMask = 0x00000000);
	/*�ε��� ����ȭ �Լ� (������͸��� ����)*/
	bool SetTriangleSubBuffer(vector<shared_ptr<BMesh2>>& pSubMesh, BGeomMesh* pAseMesh);

	/*������ # �Ž� ������Ʈ �����Լ�*/
	HRESULT Create(ID3D11Device* pDevice);
	/*������ # �Ž� ���ؽ� �����Լ�*/
	HRESULT CreateVertexBuffer();
	/*������ # �Ž� �ε��� �����Լ�*/
	HRESULT CreateIndexBuffer();
	/*������ # �Ž� �ؽ���(���̴����ҽ�) �����Լ�*/
	HRESULT TextureLoad(ID3D11Device* pDevice);
	
	
	/*������ ī�޶� �����Լ�*/
	bool SetCamera(D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);
	/*������ ��� �����Լ�*/
	void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	/*������ �ν��Ͻ� ����Ʈ �����Լ�*/
	bool SetDrawObj(vector<int> DrawList);
	/*��ü �Ž� �����Լ�*/	
	bool Draw(ID3D11DeviceContext* pContext, BAseObj* pParent);
	/*�κ� �Ž� �����Լ�*/
	void DrawMesh(ID3D11DeviceContext* pContext, BMesh2* pMesh);
	/*�ν��Ͻ� �Ž� �����Լ�*/
	bool DrawInstance(ID3D11DeviceContext* pContext, BAseObj* pParent, int DrawCount);


	/*�ؽ��� �ε� �Լ�(����)*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);
	/*�ؽ��� �ε� �Լ�(����)*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);
	/*�븻���� �ؽ��� �ε� �Լ�*/
	void NormalTextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);

	/*�ִϸ��̼� ������ ��ũ���� �Լ�(���� ��ũ�帮��Ʈ)*/
	BAnimTrack* SetDoublyLinkedList(BAnimTrack* pCurrentTrack, BAnimTrack* pPrev);
	/*�ִϸ��̼� ������ ��� ������� ����(���������̼�)�Լ�*/
	void Interpolate(BMesh2* pMesh, D3DXMATRIX* matParent, float fFrameTick);
	/*�ִϸ��̼� Ʈ�� ��ȯ �Լ�*/
	//pEndTrack�� ������ flase�� ���� (������ ����� ���� ���)
	bool GetAnimationTrack(float fFrame, vector<shared_ptr<BAnimTrack>> pTrackList, BAnimTrack** pStartTrack, BAnimTrack** pEndTrack);
	

	/*������ # �ν��Ͻ� ���� ���̴� ���̾ƿ� �����Լ�*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*�ν��Ͻ� ����, ���� �����Լ�*/
	void InitInstanceBuffer(int iInstanceCount, int ObjNum);
	/*�ν��Ͻ� ���� ���� �Լ� (InitInstanceBuffer ���� ȣ��)*/
	HRESULT CreateInstanceBuffer(UINT ListSize, UINT InstnaceSize, vector<D3DXMATRIX> m_pInstance);
	/*�ν��Ͻ� ���� �����Լ�*/
	void UpdateInstanceInfo(ID3D11DeviceContext* pContext, vector<InstancingInfo> InstanceInfo, BMesh2* MeshData);


	/*�浹�ڽ� ���� �Լ�*/
	bool CreateBox();
	/*�浹�ڽ� ���� �����Լ�*/
	void UpdateBoundingBox();
	/*�浹�ڽ� ������ �Լ�*/
	bool RenderBoundingBoxLine(ID3D11DeviceContext* m_pImmediateContext, D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);

	
	/*�븻���� ���� ������� �����Լ�*/
	HRESULT CreateNormalCB();
	/*�븻���ο� ī�޶� ��ġ, �ü����� �����Լ�*/
	void SettingCameraForNormal(D3DXVECTOR3 Pos, D3DXVECTOR3 Look);
	/*������ # �������� ���� �����Լ� (UpdateNormal ���� ȣ��)*/
	bool UpDateTangentBuffer();
	/*������ # ���̽� �븻 �����Լ� (UpdateNormal ���� ȣ��)*/
	bool InitFaceNormal();
	/*������̺�(�ε���) �����Լ� (UpdateNormal ���� ȣ��)*/
	void GenNormalLookupTable();
	/*���̽��븻 ��� ���ؽ� �븻���� �����Լ� (UpdateNormal ���� ȣ��)*/
	void CalcPerVertexNormalsFastLookup();
	
public:
	BAseObj();
	virtual ~BAseObj();
};

