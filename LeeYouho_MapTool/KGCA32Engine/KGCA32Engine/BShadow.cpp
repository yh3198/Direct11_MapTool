#include "BShadow.h"

void BShadow::Init(ID3D11Device* Device)
{
	m_pd3dDevice = Device;
	m_ShadowPlane.Create(m_pd3dDevice, L"../../data/shader/TestShader.hlsl", DXGI_FORMAT_R32_TYPELESS);
	m_ShadowPlane.Set(m_pd3dDevice, 200, 0, 200, 200);
	LoadShadowShader();
	CreateShadowCB();
	m_vLightPos = D3DXVECTOR3(128, 80, -128);
	m_vLightLookat = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vLightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matShadowView, &m_vLightPos, &m_vLightLookat, &m_vLightUp);
	float fMaxViewDistance = sqrt(131072);
	D3DXMatrixOrthoOffCenterLH(&m_matShadowProj, -fMaxViewDistance / 2, fMaxViewDistance / 2, -fMaxViewDistance / 2, fMaxViewDistance / 2, 0.0f, 1000.0f);
	m_matTexture = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f, -0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
}

/*������ ���̴� �����Լ�*/
HRESULT BShadow::LoadShadowShader()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // ���̴� ����� ���
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;

	ID3DBlob* pError = NULL; // �̰��� �������ν� ������ �� ������ �޾� �� �� �ִ�.
							 /* vs_5_0 == direct11 �� 5.0�� ��� * pVSBuf ����� ��� */

	if (FAILED(D3DX11CompileFromFile(L"../../data/Shader/ShadowShader.hlsl", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pShadowVS));
	}

	if (FAILED(D3DX11CompileFromFile(L"../../data/Shader/ShadowShader.hlsl", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pShadowPS));
	}

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	return hr;
}
/*������ ������� �����Լ�*/
HRESULT BShadow::CreateShadowCB()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;					//CPU���� GPU�� Access �����ϰ� ���ִ� �÷���
	cbDesc.CPUAccessFlags = 0;							//CPU���� GPU�� Access �����ϰ� ���ִ� �÷���
	cbDesc.ByteWidth = sizeof(SHADOW_CONSTANT_BUFFER);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	S_RETURN(m_pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pShadowCB));
	return hr;
}


BShadow::BShadow()
{
}


BShadow::~BShadow()
{
}
