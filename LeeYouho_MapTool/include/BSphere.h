#pragma once
#include "BObject.h"

//====================================================================================//
/*������Ʈ�� ������ �����մϴ�.*/
/*UpdateQuaternion�� ���� ���� ������ ��Ʈ�� �մϴ�.*/
//====================================================================================//


class BSphere : public BObject
{
public:
	vector<D3DXVECTOR3> m_VertexArray;		// ���� ���ؽ�����Ʈ (�����)
	UINT m_Slices;							// ���� �����̽� ��
	double m_fRadius;						// ���� ������
	D3DXVECTOR3 m_vCenter;					// ���� ����

public:
	/*������ # ���ؽ� ���� �����Լ�*/
	virtual HRESULT CreateVertexBuffer();
	/*������ # ���� ����*/
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	/*���� �̵� �Լ�*/
	bool UpdateQuaternion();
	

public:
	BSphere();
	~BSphere();
};

