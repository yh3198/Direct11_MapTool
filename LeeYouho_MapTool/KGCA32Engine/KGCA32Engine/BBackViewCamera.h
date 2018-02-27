#pragma once
#include "BCamera.h"

//====================================================================================//
/*BackView ī�޶��� ��� Init�� �Ѱ��� Ÿ�ٰ��� �������� ������ �մϴ�.*/
/*�̿��� ������ ī�޶� Ŭ������ �����մϴ�.*/
//====================================================================================//

class BBackViewCamera : public BCamera
{
public:
	D3DXVECTOR3 m_vCameraDestination;  // ī�޶� �����̵��� ��ǥ����
	D3DXVECTOR3 m_vObjectVector[3];	   // ����� ���庯ȯ ����� �������
	float m_fPrevRadius;			   // ���� ī�޶��� �Ÿ�
	D3DXVECTOR3 m_vHeroLook;		   // ����� �ü�����
	D3DXVECTOR3 m_vHeroUp;			   // ����� ���⺤��
	D3DXVECTOR3 m_vHeroSide;		   // ����� ���̵庤��

	D3DXVECTOR3 m_vDefaultEye;		   // ī�޶��� �ʱ� ��ġ
	D3DXVECTOR3 m_vDefaultLookAt;	   // ī�޶��� �ʱ� Ÿ��

public:
	/* ������ # ī�޶� ���� */
	virtual bool Init(D3DXVECTOR3 vTarget);
	/* ������ # ī�޶� ���� (������Ʈ�� �������� ����) */
	virtual bool Frame();

	/* ������ # ����� �����Լ�(������Ʈ�� ���⺤�͸� �������� �̵�)*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);

	/* ������ # ī�޶� ȸ�� �����Լ�*/
	virtual bool UpdateQuaternion();
	/* ������ # ī�޶� ȸ�� �ݿ��Լ�*/
	virtual bool UpdateVector();
	

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};

