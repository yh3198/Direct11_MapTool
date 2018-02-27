#pragma once
#include "BWindowLib.h"
#include "BObjStd.h"

//====================================================================================//
/*각종 오브젝트 상속의 기본 클래스입니다.*/
/*Create 함수를 이용하여 오브젝트 생성이 가능합니다.*/
//====================================================================================//

class BObject
{
public:
	vector<PNCT_Vertex> m_VertexList;			 // 버텍스 벡터
	vector<DWORD> m_IndexList;					 // 버텍스 인덱스
	UINT m_VertexCnt;							 // 버텍스 갯수
	VSConstantB m_cbData;						 // 상수버퍼 맴버
	B_BOX m_tBox;								 // 최대 최소값

	ID3D11Buffer*			m_pVertexBuffer;	 // 버텍스 버퍼
	ID3D11Buffer*			m_pIndexBuffer;		 // 인덱스 버퍼
	ID3D11Buffer*			m_pConstantBuffer;   // 상수 버퍼
	ID3D11InputLayout*		m_pVertexLayout;	 // 버텍스 레이아웃
	
	vector<D3DXVECTOR3>		m_TangentList;		 // 접선벡터 리스트
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	 // 접선벡터 버퍼
	UINT m_TangentCnt;

	vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;			 // 텍스쳐 리소스
	vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;	 // 노말 텍스쳐 리소스
	INT m_iCurIndex;												 // 텍스쳐 인덱스

	ID3D11VertexShader*		m_pVS;				// 버텍스 쉐이더
	ID3D11PixelShader*		m_pPS;				// 픽셀 쉐이더
	ID3D11GeometryShader*	m_pGS;				// 기하 쉐이더

	ID3D11Device* m_pd3dDevice;					// 디바이스 인터페이스
	ID3D11DeviceContext* m_pContext;			// 메인 컨텍스트

	D3DXMATRIX m_matWorld;  // 로컬좌표 (백터값) * 월드 행렬 = 월드좌표
	D3DXMATRIX m_matView;	// 월드좌표 * 뷰행렬 = 뷰좌표 (카메라의 이동에 따른 좌표)
	D3DXMATRIX m_matProj;	// 뷰좌표 * 투영행렬 = 투영좌표 (카메라의 투영에 따른 좌표)

	D3DXMATRIX m_matTrans;  // 이동행렬
	D3DXMATRIX m_matRotate; // 회전행렬
	D3DXMATRIX m_matScale;  // 크기행렬


	/*오브젝트의 위치 벡터*/
	D3DXVECTOR3 m_vPos;			// 위치벡터
	D3DXVECTOR3 m_vTarget;		// 방향벡터
	D3DXVECTOR3 m_vUp;			// 상향벡터

	/*위치를 나타내는 방향벡터*/
	D3DXVECTOR3 m_vLook;		// 방향벡터
	D3DXVECTOR3 m_vSide;		// 사이드벡터

	/*오브젝트 회전 정보*/
	float m_fPitch;				//x축 기준회전
	float m_fYaw;				//y축 기준회전
	float m_fRoll;				//z축 기준회전
	float m_SpeedUp;			//속도값

	// 랜더링용 변수
	UINT m_iPrimitiveType;		// 랜더링 설정값
	int m_iNumFace;				// 총 삼각형의 갯수
	int m_iNumVertices;			// 총 버텍스의 갯수
	vector<int> m_Normallookuptable;	// 노말 룩업테이블 인덱스
	vector<D3DXVECTOR3> m_FaceNormals;  // 정점노말
	
public:
	/*기본 구성 함수*/
	virtual bool Init();
	virtual bool Frame();
	virtual bool PostFrame();
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();

	/*오브젝트 생성 함수 (다중 텍스쳐)*/
	virtual HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, TCHAR** pLoadTextureString = NULL, int TextureCnt= 1);
	/*오브젝트 생성 함수 (단일 텍스쳐)*/
	HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString);
	/*레이아웃, 쉐이더 생성함수(Create 내부 포함)*/
	virtual HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*버텍스 버퍼 생성함수*/
	virtual HRESULT CreateVertexBuffer();
	/*인덱스 버퍼 생성함수*/
	virtual HRESULT CreateIndexBuffer();
	/*상수 버퍼 생성함수*/
	virtual HRESULT CreateConstantBuffer();
	/*텍스쳐 로드함수 (단일 텍스쳐)*/
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath);
	/*텍스쳐 로드함수 (다중 텍스쳐)*/
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);
	/*min max 박스 생성함수*/
	void UpdateMinMaxBox();
	
	/*행렬 설정함수*/
	virtual void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	/*오브젝트 이동함수 (Init 내부호출)*/
	virtual bool UpdateVector();
	/*오브젝트 회전함수 (PostFrame 내부호출)*/
	virtual bool UpdateQuaternion();
	/*상태값 설정함수*/
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);

	/*접선벡터 생성함수*/
	virtual bool UpdateNormal();
	/*페이스 노말 생성함수 (UpdateNormal 내부 호출)*/
	virtual bool InitFaceNormal();
	/*룩업테이블(인덱스) 생성함수 (UpdateNormal 내부 호출)*/
	virtual void GenNormalLookupTable();
	/*페이스노말 기반 버텍스 노말벡터 연산함수 (UpdateNormal 내부 호출)*/
	virtual void CalcPerVertexNormalsFastLookup();
	/*접선벡터 갱신함수 (UpdateNormal 내부 호출)*/
	virtual bool UpDateTangentBuffer();
	/*접선벡터 연산함수(UpDateTangentBuffer 내부 호출)*/
	void UpDateTangentSpaceVectors(D3DXVECTOR3 *v0, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0, D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);
	
#pragma region 재정의용 가상함수
	/*버퍼 갱신함수 (재정의용 가상함수)*/
	virtual bool UpdateBuffer() { return true; };
	/*min max 박스 갱신함수 (재정의용 가상함수)*/
	virtual void UpdateVertexMinMax() { return; };
#pragma endregion

public:
	BObject();
	virtual ~BObject();
};

