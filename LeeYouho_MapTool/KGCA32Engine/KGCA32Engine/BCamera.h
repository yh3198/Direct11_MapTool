#pragma once
#include "BFrustum.h"

//====================================================================================//
/* 카메라에는 프러스텀을 상속하고 있습니다. */
/* 사용은 함수 CreateFrustum / 함수 FrustumRender를 통해 사용하실 수 있습니다. */
//====================================================================================//

class BCamera : public BFrustum
{
public:
	D3DXVECTOR3 m_vPos;			//카메라 포지션
	D3DXVECTOR3 m_vTarget;		//카메라 타겟
	D3DXVECTOR3 m_vUp;			//카메라 상향벡터
	D3DXVECTOR3 m_vLook;		//카메라 시선벡터
	D3DXVECTOR3 m_vSide;		//카메라 사이드벡터

	float m_fPitch;			//x축 기준회전
	float m_fYaw;			//y축 기준회전
	float m_fRoll;			//z축 기준회전
	float m_SpeedUp;		//카메라 회전 속도

	bool bCameraChange;		//카메라의 움직임 체크 ( 카메라의 이동, 회전이 없을 경우 추가 연산을 줄이기 위한 변수)

public:
	/*기본 구성 함수*/
	virtual bool Init(D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 15.0f, -10.0f), D3DXVECTOR3 Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	/*카메라 기본행렬 생성함수 (View, Proj)*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);
	bool SetProjMatrix(int iWidth, int iHeight, float fFov = D3DX_PI * 0.25, float fNear = 1.0f, float fFar = 1000.0f); 
	
	/*마우스에 의한 회전량 계산 함수*/
	bool UpdateQuaternion();

	/*회전량 반영 함수*/
	virtual bool UpdateVector();
	
	/*카메라 이동 반영 함수*/
	virtual void MoveLook(float fValue);
	virtual void MoveSide(float fValue);
	virtual void MoveUp(float fValue);

public:
	BCamera();
	virtual ~BCamera();
};

