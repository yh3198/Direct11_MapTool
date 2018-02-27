#pragma once
#include "BStd.h"
#include "BCamera.h"
#include "BDxRT.h"

/*�̴ϸʰ� ȭ�� ��ü�� ������ �ϱ����� ���� Ŭ����*/
class BRenderPlane : public BCamera
{
public:
	shared_ptr<BPlane>  m_pPlaneShape;
	shared_ptr<BDxRT> m_pMiniMapRT;
	D3D11_VIEWPORT m_vp;

	ID3D11BlendState* m_pMiniMapAlphaBlend;

public:
	/*Create�� ���ڰ��� ������ �� Set���� ViewPort�� ��� ���������� ��ȯ*/
	bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile);

	/*Sample���� ����Ʈ ������ ���� ����*/
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);

	/*Rander*/
	bool BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor);
	bool Render(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* OldRenderTargetView, ID3D11DepthStencilView* OldDepthStencilView, ID3D11DepthStencilState* m_pDSS);
	bool AfterRender(ID3D11DeviceContext* pContext);
public:


public:
	BRenderPlane();
	~BRenderPlane();
};

