#pragma once
#include "BStd.h"

//====================================================================================//
/*Create ȣ�� �� Begin -> Apply -> ������ ���� -> End ������ ȣ���ؼ� ����ؾ� �մϴ�.*/
//====================================================================================//

class BDxRT
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// ����Ÿ��
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;		// ���̽��ٽ�
	ComPtr<ID3D11ShaderResourceView> m_pSRV;				// ���̴� ���ҽ�
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV;				// ���̹��� ���ҽ�
	ComPtr<ID3D11Texture2D> m_pTexture;						// ���ҽ� �ؽ���

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;		// ���̹��ۿ�
	D3D11_TEXTURE2D_DESC m_TexDesc;							// �ؽ��Ŀ�
	DXGI_FORMAT m_DSFormat;									// ���� ���ٽ� ���� ����
	D3D11_VIEWPORT m_vp;									// ����Ʈ
	D3DXMATRIX m_matProj;									// ������� (����Ʈ ���ÿ�)
	UINT m_nViewPorts;										// ����Ʈ�� ��

	/*Get ��� ���ҿ� ����*/
	D3D11_VIEWPORT m_vpOld;									// ������ ����Ʈ 
	ID3D11RenderTargetView* m_pOldRTV;						// ������ ����Ÿ��
	ID3D11DepthStencilView* m_pOldDSV;						// ������ ���̹���

public:
	/*����Ʈ �����Լ�*/
	virtual HRESULT Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	/*����Ʈ �����Լ�(Create ���� ȣ��)*/
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	/*���̹��� �����Լ�(Create ���� ȣ��)*/
	HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height);

	//======/* ����Ʈ ���� �� ������ ���� ���ú����Լ� ( Begin -> Apply -> End ��) */======//
	/* ����Ʈ ���� ���� �Լ� */
	bool Begin(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor,
		         bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	/* Create�� ������ ���� ���� �Լ� */
	void Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, bool bDepthClear = true, bool bStencilClear = true);

	/* Begin�� ������ ���� ������ ����Ʈ, ���̽��ٽ�, ����Ÿ�� ���� ���� �Լ� */
	bool End(ID3D11DeviceContext* pContext);
	//=====================================================================================//

public:
	BDxRT();
	virtual ~BDxRT();
};

