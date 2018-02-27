#pragma once
#include "BStd.h"
#include "BCamera.h"
#include "BDxRTHeight.h"
#include "BNPlane.h"

//====================================================================================//
/* 특정 위치에서 랜더링을 하기위한 클래스 입니다. */
/* RT의 세팅을 통해 내부 텍스쳐에 랜더링을 한 후 m_pPlaneShape을 통해 화면 랜더링에 이용합니다. */
/* Create로 인자값을 생성한 후 Set으로 ViewPort를 어떻게 잡을것인지 변환합니다.*/
/* 해당 클래스에서 뷰포트 세팅을 위해 선언합니다.*/
//====================================================================================//

class BRenderPlane : public BCamera
{
public:
	shared_ptr<BNPlane>  m_pPlaneShape;     // 랜더용 평면
	shared_ptr<BDxRTHeight> m_pMiniMapRT;   // 텍스쳐 생성용 랜더타겟
	D3D11_VIEWPORT m_vp;				    // 뷰포트
	ID3D11BlendState* m_pMiniMapAlphaBlend; // 전용 알파블랜드값

public:
	/*생성 및 세팅용 함수*/
	virtual bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile,DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,  FLOAT Width = 1024, FLOAT Height = 1024);
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);

	/*Rander 과정용 함수*/
	bool BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor);
	bool Render(ID3D11DeviceContext* pContext, ComPtr<ID3D11ShaderResourceView> SRV);
	bool AfterRender(ID3D11DeviceContext* pContext);

public:
	BRenderPlane();
	virtual ~BRenderPlane();
};

