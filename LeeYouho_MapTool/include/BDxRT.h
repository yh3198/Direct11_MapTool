#pragma once
#include "BStd.h"

//====================================================================================//
/*Create 호출 후 Begin -> Apply -> 랜더링 과정 -> End 순으로 호출해서 사용해야 합니다.*/
//====================================================================================//

class BDxRT
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// 랜더타겟
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;		// 깊이스텐실
	ComPtr<ID3D11ShaderResourceView> m_pSRV;				// 쉐이더 리소스
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV;				// 깊이버퍼 리소스
	ComPtr<ID3D11Texture2D> m_pTexture;						// 리소스 텍스쳐

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;		// 깊이버퍼용
	D3D11_TEXTURE2D_DESC m_TexDesc;							// 텍스쳐용
	DXGI_FORMAT m_DSFormat;									// 깊이 스텐실 버퍼 포맷
	D3D11_VIEWPORT m_vp;									// 뷰포트
	D3DXMATRIX m_matProj;									// 투영행렬 (뷰포트 세팅용)
	UINT m_nViewPorts;										// 뷰포트의 수

	/*Get 비용 감소용 변수*/
	D3D11_VIEWPORT m_vpOld;									// 설정전 뷰포트 
	ID3D11RenderTargetView* m_pOldRTV;						// 설정전 랜더타겟
	ID3D11DepthStencilView* m_pOldDSV;						// 설정전 깊이버퍼

public:
	/*뷰포트 생성함수*/
	virtual HRESULT Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	/*뷰포트 세팅함수(Create 내부 호출)*/
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	/*깊이버퍼 갱신함수(Create 내부 호출)*/
	HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height);

	//======/* 뷰포트 변경 후 랜더를 위한 세팅변경함수 ( Begin -> Apply -> End 순) */======//
	/* 뷰포트 변경 적용 함수 */
	bool Begin(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor,
		         bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	/* Create로 생성한 인자 적용 함수 */
	void Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, bool bDepthClear = true, bool bStencilClear = true);

	/* Begin에 저장한 이전 값으로 뷰포트, 깊이스텐실, 랜더타겟 상태 복구 함수 */
	bool End(ID3D11DeviceContext* pContext);
	//=====================================================================================//

public:
	BDxRT();
	virtual ~BDxRT();
};

