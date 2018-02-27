#include "BBox.h"


/*������ # ���ؽ� ���� �����Լ�*/
HRESULT BBox::CreateVertexBuffer()		// ���ؽ� ���� ����
{
	HRESULT hr = S_OK;

	m_VertexList.resize(24);

	m_tBox.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_tBox.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_tBox.vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_tBox.fExtent[0] = 1.0f;
	m_tBox.fExtent[1] = 1.0f;
	m_tBox.fExtent[2] = 1.0f;

	m_VertexList[0].Position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f); // ��
	m_VertexList[1].Position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[2].Position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[3].Position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	m_VertexList[4].Position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); // ��
	m_VertexList[5].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[6].Position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[7].Position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);

	m_VertexList[8].Position = D3DXVECTOR3(1.0f, 1.0f, -1.0f); // ������
	m_VertexList[9].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[10].Position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[11].Position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

	m_VertexList[12].Position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); //�Ʒ�
	m_VertexList[13].Position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[14].Position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[15].Position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);

	m_VertexList[16].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //����
	m_VertexList[17].Position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[18].Position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[19].Position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	m_VertexList[20].Position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); //����
	m_VertexList[21].Position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[22].Position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[23].Position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);

	m_VertexList[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_VertexList[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_VertexList[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_VertexList[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	m_VertexList[4].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_VertexList[5].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_VertexList[6].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_VertexList[7].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_VertexList[8].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[9].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[10].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[11].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_VertexList[12].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_VertexList[13].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_VertexList[14].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_VertexList[15].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	m_VertexList[16].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VertexList[17].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VertexList[18].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VertexList[19].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_VertexList[20].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_VertexList[21].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_VertexList[22].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_VertexList[23].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_VertexList[0].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[2].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[4].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[5].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[6].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[7].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[8].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[9].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[10].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[11].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[12].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[13].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[14].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[15].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[16].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[17].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[18].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[19].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[20].Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[21].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[22].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[23].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[0].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[1].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[2].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[3].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexList[4].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[5].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[6].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[7].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexList[8].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[9].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[10].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[11].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexList[12].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[13].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[14].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[15].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexList[16].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[17].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[18].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[19].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexList[20].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[21].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[22].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[23].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_VertexCnt = m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [�޸� �Ҵ� ��ġ] GPU �޸𸮿� �Ҵ��ض�
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [�޸� �Ҵ� ũ��] �� X���� �Ҵ� �� ���̱� ������ *X ( ���� �þ�� ���� �� ���̴� )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [�޸� ��� �뵵] ���ؽ� ���ۿ����� ����ϰڴ�.
	bd.CPUAccessFlags = 0;								// CPU�� ���ʹ� �׼��� ���� �ʰڴ�.
	bd.MiscFlags = 0;									// �߰� ���� ����

	D3D11_SUBRESOURCE_DATA InitData;			// �ҽ� ���ҽ� �ʱ�ȭ�� �ʿ��� ������ ����
	InitData.pSysMem = (void*)&m_VertexList.at(0);				// �ش� ���ؽ��� �ҽ��� ����ϰڴ�.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));
	return hr;
}

/*������ # �ε��� ���� �����Լ�*/
HRESULT BBox::CreateIndexBuffer()		// �ε��� ���� ����
{
	HRESULT hr = S_OK;

	/* ���� ���ۿ� ����ִ� �ε����� �д� ������ ����ü */
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 5; m_IndexList[iIndex++] = 6;
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 6; m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 9; m_IndexList[iIndex++] = 10;
	m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

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

/*������ # MinMax �ڽ� �����Լ�*/
void BBox::UpdateVertexMinMax()
{
	m_tBox.vCenter.x = m_matWorld._41;
	m_tBox.vCenter.y = m_matWorld._42;
	m_tBox.vCenter.z = m_matWorld._43;

	D3DXVec3TransformCoord(&m_tBox.vAxis[0], &m_tBox.vAxis[0], &m_matWorld);
	D3DXVec3TransformCoord(&m_tBox.vAxis[1], &m_tBox.vAxis[1], &m_matWorld);
	D3DXVec3TransformCoord(&m_tBox.vAxis[2], &m_tBox.vAxis[2], &m_matWorld);
	D3DXVec3Normalize(&m_tBox.vAxis[0], &m_tBox.vAxis[0]);
	D3DXVec3Normalize(&m_tBox.vAxis[1], &m_tBox.vAxis[1]);
	D3DXVec3Normalize(&m_tBox.vAxis[2], &m_tBox.vAxis[2]);

	D3DXVECTOR3 vMax, vMin, vHalf;
	D3DXVec3TransformCoord(&vMax, &m_tBox.vMax, &m_matWorld);
	vHalf = vMax - m_tBox.vCenter;

	m_tBox.fExtent[0] = D3DXVec3Dot(&m_tBox.vAxis[0], &vHalf);
	m_tBox.fExtent[1] = D3DXVec3Dot(&m_tBox.vAxis[1], &vHalf);
	m_tBox.fExtent[2] = D3DXVec3Dot(&m_tBox.vAxis[2], &vHalf);

	D3DXVECTOR3 vVertex[8];
	vVertex[0] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMin.z);
	vVertex[1] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMin.z);
	vVertex[2] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMin.z);
	vVertex[3] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMin.z);

	vVertex[4] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMax.z);
	vVertex[5] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMax.z);
	vVertex[6] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMax.z);
	vVertex[7] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMax.z);

	vMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
	vMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

	for (int iVer = 0; iVer < 8; iVer++)
	{
		D3DXVec3TransformCoord(&vVertex[iVer], &vVertex[iVer], &m_matWorld);
		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;
		if (vVertex[iVer].z > vMax.z) vMax.z = vVertex[iVer].z;

		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
		if (vVertex[iVer].z < vMin.z) vMin.z = vVertex[iVer].z;
	}

	m_tBox.vMin = vMin;
	m_tBox.vMax = vMax;
}



BBox::BBox()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
}
BBox::~BBox()
{
}
