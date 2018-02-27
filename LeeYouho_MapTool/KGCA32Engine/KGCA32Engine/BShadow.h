#pragma once
#include "BRenderPlane.h"

class BShadow
{
public:
	ID3D11VertexShader*		m_pShadowVS;	// ���ؽ� ���̴�
	ID3D11PixelShader*		m_pShadowPS;	// �ȼ� ���̴�
	ID3D11Buffer*			m_pShadowCB;	// �������
	SHADOW_CONSTANT_BUFFER  m_cbShadow;		// ������� ����

	BRenderPlane			m_ShadowPlane;	// ������ ������ �÷�
	D3DXVECTOR3				m_vLightPos;	// ������� ����Ʈ ������
	D3DXVECTOR3				m_vLightLookat;	// ������� ����Ʈ Ÿ��
	D3DXVECTOR3				m_vLightUp;		// ������� ����Ʈ ���⺤��
	D3DXMATRIX				m_matShadowView;// �����
	D3DXMATRIX				m_matShadowProj;// �������
	D3DXMATRIX				m_matTexture;	// �ؽ��� ��ġ�� ������ġ�� �������� ���� ��ȯ ���
	D3DXMATRIX				m_matWVPT;		// �������

	ID3D11Device*			m_pd3dDevice;			// ���� ����̽�

public:
	/*�⺻ ���� �Լ�*/
	void Init(ID3D11Device* Device);

	/*������ ���̴� �����Լ�*/
	HRESULT LoadShadowShader();
	/*������ ������� �����Լ�*/
	HRESULT CreateShadowCB();

public:
	BShadow();
	~BShadow();
};

