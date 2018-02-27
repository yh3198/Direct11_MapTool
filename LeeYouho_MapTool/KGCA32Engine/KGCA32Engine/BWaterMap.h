#pragma once
#include "BRenderPlaneHeight.h"
#include "BMapTileRender.h"
#include "BNMap.h"

#define FOGMODE_NONE    0 // 포그 미사용
#define FOGMODE_LINEAR  1 // 포그 사용
#pragma region 포그, 라이팅 관련 구조체
/*포그 구조체*/
struct cbFog
{
	D3DXVECTOR4 g_ClipPlane;
	D3DXVECTOR4 g_vCameraPos;
	D3DXVECTOR4 g_FogColor;
	D3DXVECTOR4 g_FogInfo;
	float		g_fogMode;		//x(감쇠방법)
	float		g_fogDensity;	//y(농도)
	float		g_fogStart;		//z(시작포그거리)
	float		g_fogEnd;		//w(종료포그거리)
};

/*노말맵핑 구조체*/
struct cbTanLight
{
	D3DXMATRIX      matNormal;
	D3DXVECTOR4		vLightPos;
	D3DXVECTOR4		vEye;
	D3DXVECTOR4		vEyeDir;
};

/*라이팅 구조체*/
struct cbLightDefault
{
	D3DXVECTOR4				cb_AmbientLightColor;
	D3DXVECTOR4				cb_DiffuseLightColor;
	D3DXVECTOR4				cb_SpecularLightColor;
};
#pragma endregion

//====================================================================================//
/*워터맵은 LOD, 쿼드트리가 적용된 상태로 기존 맵과 동일하게 사용하시면 됩니다.*/
/*맵과 동일하게 프로스텀 내에 있는 부분만을 랜더링하여 최적화되어 있습니다.*/
/*워터맵 클래스는 별도의 WaterFrame 함수를 보유하고 있습니다 Frame과 WaterFrame 둘 다 돌아가야 합니다.*/
/*Build호출 시 넘기는 pMap은 실제 맵이 아닌 워터맵으로 생성할 맵입니다.*/
//====================================================================================//


class BWaterMap : public BMapTileRender
{
public:
	BMap* m_pWaterTexMap;					// 워터맵의 원본맵 ( 워터맵의 크기 위치등을 결정 )
	BMapTileRender* m_pWaterReflectMap;		// 워터맵의 반사, 굴절 쉐도우 리소스를 만들기 위한 랜더맵
	TCHAR* m_strWaterTex;					// 워터맵의 텍스쳐 명
	TCHAR* m_strWaterNormalTex;				// 워터맵의 노말텍스쳐 명

	int m_iSelectedMap;						// 워터맵으로 선택된 맵의 번호 ( 기존 맵 벡터, 리스트로 이용시 사용 )
	float m_fWaterHeight;					// 워터맵의 높이값
	float m_fWaterWave;						// 워터맵의 웨이브값
	D3DXMATRIX m_matNormal;					// 워터맵의 노말행렬

	cbFog m_Fog;							// 포그 정보
	cbTanLight m_TanLight;					// 노말 라이트 정보
	cbLightDefault m_LightDefault;			// 라이트 정보

	ComPtr<ID3D11Buffer> m_pFogCB;			// 포그 상수버퍼
	ComPtr<ID3D11Buffer> m_pTanLightCB;		// 노말 라이트 상수버퍼
	ComPtr<ID3D11Buffer> m_pLightDefaultCB; // 라이트 상수버퍼

	vector<D3DXVECTOR3>		m_vTangentList; // 접선벡터 리스트
	ComPtr<ID3D11Buffer>	m_pTangentVB;	// 접센벡터 버퍼
	
	ComPtr<ID3D11ShaderResourceView> m_WaterMapUpsideSRV; // m_pReflectRV // 반사 리소스
	ComPtr<ID3D11ShaderResourceView> m_WaterBottomSRV;	  // m_pRefractRV // 굴절 리소스
	ComPtr<ID3D11ShaderResourceView> m_WaterHeightSRV;	  // 워터맵에 적용되고 있는 현재 맵의 높이맵 리소스 ( 이를 통해 높이에 따라 포그를 설정 )
	ComPtr<ID3D11ShaderResourceView> m_WaterNormalSRV;	  // 노말 리소스
	ComPtr<ID3D11ShaderResourceView> m_WaterSRV;		  // 텍스쳐 리소스

	ID3D11VertexShader*		m_pWaterVS; // 버텍스 쉐이더
	ID3D11PixelShader*		m_pWaterPS; // 픽셀 쉐이더
		
	BRenderPlaneHeight m_HeightMap;		// 적용중인 높이맵

	bool bHeightChange;					// 높이 변경여부 판단변수
	bool bRequestCreate;				// 높이맵 갱신 요청여부 판단변수

	BMapTileRender* m_SelectedMap;		// 워터맵 적용중인 쿼드트리 ( Import시 사용 )
	BRenderPlane m_MiniMap[3];			// 반사 굴절 내부 리소스를 만들기 위한 미니맵
	
public:
	/*기본 구성 함수*/
	bool Frame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	bool Render();
	bool PostRender(ID3D11DeviceContext* pContext);
	
	/*재정의 # 워터맵 생성함수*/
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex = NULL, TCHAR* strNormalTex = NULL, TCHAR* strHeightTex = NULL);
	/*워터맵 전용 레이아웃 생성함수 (Build 내부 호출)*/
	HRESULT CreateWaterMapInputLayout(const TCHAR* pShaderFile);
	/*상수버퍼 생성함수 (Build 내부 호출)*/
	HRESULT CreateWaterConstantBuffer();
	/*재정의 # 워터맵 바운딩박스 생성함수*/
	void ComputeBoundingBox(BNode* pNode);

	/*워터맵 전용 프레임 함수(Frame 내부 호출)*/
	bool WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	/*반사 뷰행렬 생성함수 (WaterFrame 내부 호출)*/
	D3DXMATRIX* D3DXMatrixReflect_Fixed(__inout D3DXMATRIX *pOut, __in const D3DXPLANE *pPlane);
	/*반사, 굴절 리소스 생성 함수 (WaterFrame 내부 호출)*/
	void DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	
	/*재정의 # 텍스쳐 생성 함수*/
	HRESULT TextureLoad(const TCHAR* strWaterTex, const TCHAR* strNormalTex, const TCHAR* strHeightTex);
	/*워터 텍스쳐 생성함수*/
	HRESULT SetWaterTexture(const TCHAR* strFilePath);
	/*워터 노말텍스쳐 생성함수*/
	HRESULT SetWaterNormalTexture(const TCHAR* strFilePath);
	/*워터 높이텍스쳐 생성함수*/
	HRESULT SetHeightTexture(const TCHAR* strFilePath);

	/*워터 높이값 설정함수 (별도 호출 필수 / 실수 값으로 설정)*/
	bool SetMapHeight(float Height = 0.0f);
	/*워터 높이값 설정함수 (별도 호출 필수 / 월드행렬 값으로 설정)*/
	bool SetMapHeight();
	/*워터맵 기본 요소 설정함수(별도 호출 필수 / 디바이스 카메라 맵)*/
	void Update(ID3D11Device* pd3dDevice, BCamera* pCamera, BMapTileRender* SelectedMap);
	/*워터맵 행렬 설정함수*/
	void SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);



public:
	BWaterMap();
	~BWaterMap();
};

