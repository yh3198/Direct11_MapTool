#include "BSphere.h"



/*������ # ���ؽ� ���� �����Լ�*/
HRESULT BSphere::CreateVertexBuffer()		// ���ؽ� ���� ����
{
	HRESULT hr = S_OK;

	m_VertexArray.clear();
	m_VertexArray.resize(m_Slices*m_Slices);

	for (DWORD jCnt = 0; jCnt < m_Slices; jCnt++)
	{
		FLOAT theta = (D3DX_PI*jCnt) / (m_Slices);
		for (DWORD iCnt = 0; iCnt < m_Slices; iCnt++)
		{
			int iPos = jCnt * m_Slices + iCnt;
			FLOAT phi = (2 * D3DX_PI*iCnt) / (m_Slices);
			m_VertexArray[iPos].x = m_fRadius * (float)(sin(theta)*cos(phi)) + m_vCenter.x;
			m_VertexArray[iPos].z = m_fRadius * (float)(sin(theta)*sin(phi)) + m_vCenter.y;
			m_VertexArray[iPos].y = m_fRadius * (float)(cos(theta)) + m_vCenter.z;
		}
	}
	const double PI = 3.141592653589793238462643383279502884197;
	int iPos = 0;
	int iNext = 0;
	for (DWORD j = 0; j < m_Slices; j++)
	{
		for (DWORD i = 0; i < m_Slices; i++)
		{
			if (i == m_Slices - 1) iNext = 0;
			else iNext = i + 1;
			PNCT_Vertex v0, v1, v2, v3, v4, v5;
			iPos = (j*m_Slices * 6) + (i * 6);

			v0.Position = m_VertexArray[j*m_Slices + i];
			v1.Position = m_VertexArray[j*m_Slices + iNext];

			if (j != m_Slices - 1) v2.Position = m_VertexArray[((j + 1)*m_Slices) + i];
			else  v2.Position = D3DXVECTOR3(0, -1, 0);

			v3.Position = v2.Position;
			v4.Position = v1.Position;

			if (j != m_Slices - 1) v5.Position = m_VertexArray[((j + 1)*m_Slices) + iNext];
			else  v5.Position = D3DXVECTOR3(0, -1, 0);

			D3DXVec3Normalize(&v0.Normal, &v0.Position);
			D3DXVec3Normalize(&v1.Normal, &v1.Position);
			D3DXVec3Normalize(&v2.Normal, &v2.Position);
			D3DXVec3Normalize(&v3.Normal, &v3.Position);
			D3DXVec3Normalize(&v4.Normal, &v4.Position);
			D3DXVec3Normalize(&v5.Normal, &v5.Position);

			v0.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			v1.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			v2.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			v3.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			v4.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			v5.Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

			v0.Texture = D3DXVECTOR2(atan2(v0.Normal.z, v0.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v0.Normal.y) / PI + 0.5f);
			v1.Texture = D3DXVECTOR2(atan2(v1.Normal.z, v1.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v1.Normal.y) / PI + 0.5f);
			v2.Texture = D3DXVECTOR2(atan2(v2.Normal.z, v2.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v2.Normal.y) / PI + 0.5f);
			v3.Texture = D3DXVECTOR2(atan2(v3.Normal.z, v3.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v3.Normal.y) / PI + 0.5f);
			v4.Texture = D3DXVECTOR2(atan2(v4.Normal.z, v4.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v4.Normal.y) / PI + 0.5f);
			v5.Texture = D3DXVECTOR2(atan2(v5.Normal.z, v5.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v5.Normal.y) / PI + 0.5f);

			if (fabs(v1.Texture.x - v0.Texture.x) > 0.9f)
			{
				if (fabs(v1.Texture.x) <= 0.01f)
				{
					v1.Texture.x = 1.0f;
				}
				if (fabs(v0.Texture.x) <= 0.01f)
				{
					v0.Texture.x = 1.0f;
				}
			}
			if (fabs(v2.Texture.x - v0.Texture.x) > 0.9f)
			{
				if (fabs(v2.Texture.x) <= 0.01f)
				{
					v2.Texture.x = 1.0f;
				}
				if (fabs(v0.Texture.x) <= 0.01f)
				{
					v0.Texture.x = 1.0f;
				}
			}
			if (fabs(v4.Texture.x - v3.Texture.x) > 0.9f)
			{
				if (fabs(v4.Texture.x) <= 0.01f)
				{
					v4.Texture.x = 1.0f;
				}
				if (fabs(v3.Texture.x) <= 0.01f)
				{
					v3.Texture.x = 1.0f;
				}
			}
			if (fabs(v5.Texture.x - v3.Texture.x) > 0.9f)
			{
				if (fabs(v5.Texture.x) <= 0.01f)
				{
					v5.Texture.x = 1.0f;
				}
				if (fabs(v3.Texture.x) <= 0.01f)
				{
					v3.Texture.x = 1.0f;
				}
			}
			m_VertexList.push_back(v0);
			m_VertexList.push_back(v1);
			m_VertexList.push_back(v2);
			m_VertexList.push_back(v3);
			m_VertexList.push_back(v4);
			m_VertexList.push_back(v5);
		}
	}
	m_VertexArray.clear();
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

/*������ # ���� ����*/
bool BSphere::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->Draw(m_VertexList.size(), 0);
	return true;
}

/*���� �̵� �Լ�*/
bool BSphere::UpdateQuaternion()
{
	/*���� ������Ʈ�� ���*/
	//D3DXQUATERNION qRotation;
	//// x�� ��İ� y�� ����� ���յǾ� ���ο� ����� ����
	//D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);
	//D3DXMATRIX matRotation;
	//ZeroMemory(&matRotation, sizeof(matRotation));
	//D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
	//m_matWorld = m_matWorld * matRotation;
	//m_fYaw = 0.0f;
	//m_fPitch = 0.0f;
	//m_fRoll = 0.0f;
	// ������� ��ķ� ��ȯ���ִ� �Լ� ( ��ȣ ��ȯ ���� )
	// �̴� ������Ʈ ����̹Ƿ� ī�޶� ��ķ��� ��ȯ�� �ʿ� ( ����� )

	// ȸ������� ��������Ƿ� �̵������ �߰�
	UpdateVector();

	m_matWorld._41 = m_vPos.x;// D3DXVec3Dot(&m_vPos, &m_vSide);
	m_matWorld._42 = m_vPos.y;// D3DXVec3Dot(&m_vPos, &m_vUp);
	m_matWorld._43 = m_vPos.z;// D3DXVec3Dot(&m_vPos, &m_vLook);
	return true;
}



BSphere::BSphere()
{
	m_fRadius = 1;
	m_vCenter = D3DXVECTOR3(0, 0, 0);
	m_Slices = 10;
}
BSphere::~BSphere()
{
}