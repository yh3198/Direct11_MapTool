#pragma once
#include "BStd.h"

class BDxRT
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; // ����Ÿ�ٺ�
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView; // ���̽��ٽǺ�
	ComPtr<ID3D11ShaderResourceView> m_pSRV;    // ���̴� ���ҽ���
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV; // ���̹��� ���ҽ� ��
	ComPtr<ID3D11Texture2D> m_pTexture;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc; // ���̹��ۿ�
	D3D11_TEXTURE2D_DESC m_TexDesc; // �ؽ��Ŀ�
	DXGI_FORMAT m_DSFormat; // ���� ���ٽ� ���ۿ� ����
	D3D11_VIEWPORT m_vp;  // ����Ʈ
	D3DXMATRIX m_matProj; // �������

	D3D11_VIEWPORT m_vpOld; // ���� ����Ʈ
	UINT m_nViewPorts; // ����Ʈ�� ��
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;

public:
	// Sample���� Set�Լ��� �⺻ ���� ����
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	// Sample���� Create �Լ��� �����س��� 
	HRESULT Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	/*Create ���ο��� ���Ǵ� �Լ�*/
	HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height);

	// Sample�� Render���� �����ϴ� �Լ�
	bool Begin(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor,
		         bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	// Sample���� Create�� ������ ���ڵ��� �޾ƿ� �����Ű�� �Լ�
	void Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, bool bDepthClear = true, bool bStencilClear = true);

	// Sample���� Begin�� ������ ���� ������ ����Ʈ, ���̽��ٽ�, ����Ÿ���� �������� �ǵ����� ���� �Լ�
	bool End(ID3D11DeviceContext* pContext);

public:
	BDxRT();
	~BDxRT();
};

