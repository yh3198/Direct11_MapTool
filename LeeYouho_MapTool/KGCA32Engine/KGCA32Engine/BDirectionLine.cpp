#include "BDirectionLine.h"

/*������ # ���ؽ� ���� �����Լ�*/
HRESULT BDirectionLine::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	m_LineVertexList.resize(6);
	m_LineVertexList[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[2].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[2].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_LineVertexList[3].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LineVertexList[3].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_LineVertexList[4].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[4].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_LineVertexList[5].Position = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_LineVertexList[5].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexCnt = m_LineVertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [�޸� �Ҵ� ��ġ] GPU �޸𸮿� �Ҵ��ض�
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [�޸� �Ҵ� ũ��] �� X���� �Ҵ� �� ���̱� ������ *X ( ���� �þ�� ���� �� ���̴� )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [�޸� ��� �뵵] ���ؽ� ���ۿ����� ����ϰڴ�.
	bd.CPUAccessFlags = 0;								// CPU�� ���ʹ� �׼��� ���� �ʰڴ�.
	bd.MiscFlags = 0;									// �߰� ���� ����

	D3D11_SUBRESOURCE_DATA InitData;			// �ҽ� ���ҽ� �ʱ�ȭ�� �ʿ��� ������ ����
	InitData.pSysMem = (void*)&m_LineVertexList.at(0);				// �ش� ���ؽ��� �ҽ��� ����ϰڴ�.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));
	return hr;
}

/*������ # �ε��� ���� �����Լ�*/
HRESULT BDirectionLine::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	/* ���� ���ۿ� ����ִ� �ε����� �д� ������ ����ü */
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 5;

	UINT iNumIndex = m_IndexList.size();

	// �ε��� ���� ����
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���۷� ����ϰڴٴ� ����
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = (void*)&m_IndexList.at(0);
	S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer));

	return hr;
}

/*������ # ���� �����Լ�(�������� �缳��)*/
bool BDirectionLine::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);		//�������� ����
	pContext->IASetInputLayout(m_pVertexLayout);										//���̾ƿ� ����
																						/*���ؽ� ���� ����*/
	UINT stride = sizeof(PNCT_Vertex);		 // ���ؽ� �ϳ� ��ü�� ũ��
	UINT offset = 0;						 // ���� ������ġ
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);  // ���ؽ� ���� ���ε�
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	 // �ε��� ���� ���ε�
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);				 // ���ؽ� ���̴� ������� ����

																			 /*���̴� ����*/
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->HSSetShader(NULL, NULL, 0);
	pContext->DSSetShader(NULL, NULL, 0);
	pContext->GSSetShader(NULL, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	return true;
}

/*�����ڿ��� �⺻ ���������� �����մϴ�.*/
BDirectionLine::BDirectionLine()
{
	m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}


BDirectionLine::~BDirectionLine()
{
}
