#include "BRenderPlane.h"


#pragma region ���� �� ���ÿ� �Լ�
/*������ �Լ�*/
bool BRenderPlane::Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, DXGI_FORMAT format, FLOAT Width, FLOAT Height)
{
	m_pPlaneShape = make_shared<BNPlane>();
	m_pMiniMapRT = make_shared<BDxRTHeight>();
	m_pMiniMapRT->m_DSFormat = format;
	TCHAR* NullTex = NULL;
	m_pPlaneShape->Create(pd3dDevice, pLoadShaderFile, NullTex);
	m_pMiniMapRT->Create(pd3dDevice, DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024);

	DWORD dwWidth = m_pMiniMapRT->m_TexDesc.Width;
	DWORD dwHeight = m_pMiniMapRT->m_TexDesc.Height;
	D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
	SetViewMatrix(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), vUpVector);
	SetProjMatrix(D3DX_PI * 0.25f, (float)dwWidth / (float)dwHeight, 0.1f, 1000.0f);

	return true;
}
/*���ÿ� �Լ�*/
void BRenderPlane::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
	m_pPlaneShape->m_pTextureSRV.resize(1);

	HRESULT hr = S_OK;
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = FALSE; // �����׽������� �۾��Ѵ�.
											  /*�� 8���� ����Ÿ���� ���� �� ������ ������ 0�� Ÿ�ٸ��� ���*/
											  /*�÷�����*/
											  // FinalColor = DestColor * (1.0f - SrcAlpha) + SrcColor * SrcAlpha // ���� �������� ������ �����İ� �����ϴ�. y = ax + d 
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// D3D11_BLEND_SRC_ALPHA;		//�ҽ� ���İ��� �������� ���
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;   //�ҽ� ���İ��� ������ �������� ���

															   /*���Ŀ���*/
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//�ҽ� ���İ��� �״�� ���
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//��� ���İ��� ������� ����

	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 2����ŷ �� ����� �Ѹ��� ��
	if (FAILED(hr = pDevice->CreateBlendState(&BlendState, &m_pMiniMapAlphaBlend)))
	{
		return;
	}
	return;
}
#pragma endregion

#pragma region ���� ������ �Լ�
bool BRenderPlane::BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor)
{
	if (m_pMiniMapRT->Begin(pContext, OldViewPort, OldDepthStencilView, OldRenderTargetView, vColor) == false)
	{
		m_pMiniMapRT->End(pContext);
		return false;
	}
	return true;
}
bool BRenderPlane::Render(ID3D11DeviceContext* pContext, ComPtr<ID3D11ShaderResourceView> SRV)
{
	//m_pPlaneShape->m_pTextureSRV[0] = m_pMiniMapRT->m_pSRV;
	m_pPlaneShape->m_pTextureSRV[0] = SRV;
	m_pPlaneShape->m_iCurIndex = 1;
	pContext->RSSetViewports(1, &m_vp);
	D3DXMATRIX m_matTemp;
	D3DXMatrixIdentity(&m_matTemp);
	m_pPlaneShape->SetMatrix(m_matTemp, m_matTemp, m_matTemp);
	pContext->UpdateSubresource(m_pPlaneShape->m_pConstantBuffer, 0, NULL, &m_pPlaneShape->m_cbData, 0, 0);
	pContext->OMSetBlendState(m_pMiniMapAlphaBlend, 0, -1);
	m_pPlaneShape->Render(pContext);
	return true;
}
bool BRenderPlane::AfterRender(ID3D11DeviceContext* pContext)
{
	m_pMiniMapRT->End(pContext);
	return true;
}

#pragma endregion


BRenderPlane::BRenderPlane()
{
	m_pPlaneShape = nullptr;
}
BRenderPlane::~BRenderPlane()
{
}