#pragma once
#include "BFrustum.h"

//====================================================================================//
/* ī�޶󿡴� ���������� ����ϰ� �ֽ��ϴ�. */
/* ����� �Լ� CreateFrustum / �Լ� FrustumRender�� ���� ����Ͻ� �� �ֽ��ϴ�. */
//====================================================================================//

class BCamera : public BFrustum
{
public:
	D3DXVECTOR3 m_vPos;			//ī�޶� ������
	D3DXVECTOR3 m_vTarget;		//ī�޶� Ÿ��
	D3DXVECTOR3 m_vUp;			//ī�޶� ���⺤��
	D3DXVECTOR3 m_vLook;		//ī�޶� �ü�����
	D3DXVECTOR3 m_vSide;		//ī�޶� ���̵庤��

	float m_fPitch;			//x�� ����ȸ��
	float m_fYaw;			//y�� ����ȸ��
	float m_fRoll;			//z�� ����ȸ��
	float m_SpeedUp;		//ī�޶� ȸ�� �ӵ�

	bool bCameraChange;		//ī�޶��� ������ üũ ( ī�޶��� �̵�, ȸ���� ���� ��� �߰� ������ ���̱� ���� ����)

public:
	/*�⺻ ���� �Լ�*/
	virtual bool Init(D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 15.0f, -10.0f), D3DXVECTOR3 Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	/*ī�޶� �⺻��� �����Լ� (View, Proj)*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);
	bool SetProjMatrix(int iWidth, int iHeight, float fFov = D3DX_PI * 0.25, float fNear = 1.0f, float fFar = 1000.0f); 
	
	/*���콺�� ���� ȸ���� ��� �Լ�*/
	bool UpdateQuaternion();

	/*ȸ���� �ݿ� �Լ�*/
	virtual bool UpdateVector();
	
	/*ī�޶� �̵� �ݿ� �Լ�*/
	virtual void MoveLook(float fValue);
	virtual void MoveSide(float fValue);
	virtual void MoveUp(float fValue);

public:
	BCamera();
	virtual ~BCamera();
};

