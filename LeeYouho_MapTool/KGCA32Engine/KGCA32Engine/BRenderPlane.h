#pragma once
#include "BStd.h"
#include "BCamera.h"
#include "BDxRTHeight.h"
#include "BNPlane.h"

//====================================================================================//
/* Ư�� ��ġ���� �������� �ϱ����� Ŭ���� �Դϴ�. */
/* RT�� ������ ���� ���� �ؽ��Ŀ� �������� �� �� m_pPlaneShape�� ���� ȭ�� �������� �̿��մϴ�. */
/* Create�� ���ڰ��� ������ �� Set���� ViewPort�� ��� ���������� ��ȯ�մϴ�.*/
/* �ش� Ŭ�������� ����Ʈ ������ ���� �����մϴ�.*/
//====================================================================================//

class BRenderPlane : public BCamera
{
public:
	shared_ptr<BNPlane>  m_pPlaneShape;     // ������ ���
	shared_ptr<BDxRTHeight> m_pMiniMapRT;   // �ؽ��� ������ ����Ÿ��
	D3D11_VIEWPORT m_vp;				    // ����Ʈ
	ID3D11BlendState* m_pMiniMapAlphaBlend; // ���� ���ĺ��尪

public:
	/*���� �� ���ÿ� �Լ�*/
	virtual bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile,DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,  FLOAT Width = 1024, FLOAT Height = 1024);
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);

	/*Rander ������ �Լ�*/
	bool BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor);
	bool Render(ID3D11DeviceContext* pContext, ComPtr<ID3D11ShaderResourceView> SRV);
	bool AfterRender(ID3D11DeviceContext* pContext);

public:
	BRenderPlane();
	virtual ~BRenderPlane();
};

