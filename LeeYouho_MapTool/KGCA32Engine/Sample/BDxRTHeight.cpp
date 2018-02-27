#include "BDxRTHeight.h"

HRESULT BDxRTHeight::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
{
	HRESULT hr = S_OK;
	Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

	m_TexDesc.Width = (UINT)Width;
	m_TexDesc.Height = (UINT)Height;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;

	if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf()))) // 
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf()))) // 랜더타겟뷰 객체를 생성한다.
	{
		return hr;
	}

	m_TexDesc.Usage = D3D11_USAGE_STAGING;
	m_TexDesc.BindFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, &m_pHeightTexture);

	//백버퍼의 크기가 변경되면 깊이스텐실뷰도 재생성
	if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
	{
		return hr;
	}
	return hr;
}

HRESULT BDxRTHeight::DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap)
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)m_pHeightTexture.Get(), 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		int Row = m_TexDesc.Height;
		int Col = m_TexDesc.Width;
		float Ratio = 255.0f / m_pMap->m_tBox.vMax.y;
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		for (int y = 0; y < Row; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < Col; x++)
			{
				float fDest = m_pMap->m_VertexList[y * Row + x].Position.y * Ratio;
				*pDest++ = fDest;
				*pDest++ = fDest;
				*pDest++ = fDest;
				*pDest++ = fDest;
			}
			pDestBytes = pDestBytes + MappedFaceDest.RowPitch;
		}
	}
	m_pContext->Unmap(m_pHeightTexture.Get(), 0);
	m_pContext->CopyResource(m_pTexture.Get(), m_pHeightTexture.Get());
	FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf()));
	return hr;
}

BDxRTHeight::BDxRTHeight()
{
}


BDxRTHeight::~BDxRTHeight()
{
}
