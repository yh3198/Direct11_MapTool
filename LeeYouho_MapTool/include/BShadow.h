#pragma once
#include "BRenderPlane.h"

class BShadow
{
public:
	ID3D11VertexShader*		m_pShadowVS;	// 버텍스 쉐이더
	ID3D11PixelShader*		m_pShadowPS;	// 픽셀 쉐이더
	ID3D11Buffer*			m_pShadowCB;	// 상수버퍼
	SHADOW_CONSTANT_BUFFER  m_cbShadow;		// 상수버퍼 정보

	BRenderPlane			m_ShadowPlane;	// 쉐도우 랜더용 플랜
	D3DXVECTOR3				m_vLightPos;	// 쉐도우용 라이트 포지션
	D3DXVECTOR3				m_vLightLookat;	// 쉐도우용 라이트 타겟
	D3DXVECTOR3				m_vLightUp;		// 쉐도우용 라이트 상향벡터
	D3DXMATRIX				m_matShadowView;// 뷰행렬
	D3DXMATRIX				m_matShadowProj;// 투영행렬
	D3DXMATRIX				m_matTexture;	// 텍스쳐 위치를 투영위치로 가져오기 위한 변환 행렬
	D3DXMATRIX				m_matWVPT;		// 종합행렬

	ID3D11Device*			m_pd3dDevice;			// 메인 디바이스

public:
	/*기본 구성 함수*/
	void Init(ID3D11Device* Device);

	/*쉐도우 쉐이더 생성함수*/
	HRESULT LoadShadowShader();
	/*쉐도우 상수버퍼 생성함수*/
	HRESULT CreateShadowCB();

public:
	BShadow();
	~BShadow();
};

