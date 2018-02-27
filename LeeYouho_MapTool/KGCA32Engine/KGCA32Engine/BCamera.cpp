#include "BCamera.h"
#include "BInput.h"
#include "BTimer.h"

//====================================================================================//
/* ī�޶󿡴� ���������� ����ϰ� �ֽ��ϴ�. */
/* ����� �Լ� CreateFrustum / �Լ� FrustumRender�� ���� ����Ͻ� �� �ֽ��ϴ�. */
//====================================================================================//

#pragma region �⺻ ���� �Լ�
bool BCamera::Init(D3DXVECTOR3 Pos, D3DXVECTOR3 Target)
{
	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	m_SpeedUp = 0.0f;

	D3DXMatrixIdentity(&m_matWorld);
	m_vPos = Pos;
	m_vTarget = Target;
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	SetViewMatrix(m_vPos, m_vTarget, m_vUp);
	SetProjMatrix(g_iWindowWidth, g_iWindowHeight);
	return true;
}
bool BCamera::Frame()
{
	bCameraChange = false;
	// ��
	if (g_InputData.bWKey)
	{
		MoveLook((g_fSecPerFrame * m_SpeedUp));
		bCameraChange = true;
	}
	// �Ʒ�
	if (g_InputData.bSKey)
	{
		MoveLook((g_fSecPerFrame * m_SpeedUp) * -1.0f);
		bCameraChange = true;
	}
	// ����
	if (g_InputData.bAKey)
	{
		MoveSide((g_fSecPerFrame * m_SpeedUp) * -1.0f);
		bCameraChange = true;
	}
	// ������
	if (g_InputData.bDKey)
	{
		MoveSide(g_fSecPerFrame * m_SpeedUp);
		bCameraChange = true;
	}
	// ��
	if (g_InputData.bQKey)
	{
		MoveUp(10.0f * (g_fSecPerFrame)* m_SpeedUp);
		bCameraChange = true;
	}
	// �Ʒ�
	if (g_InputData.bEKey)
	{
		MoveUp(10.0f * (g_fSecPerFrame) * -1.0f * m_SpeedUp);
		bCameraChange = true;
	}
	/*ī�޶� �̵����ӵ�*/
	if (g_InputData.bSpace)	m_SpeedUp += g_fSecPerFrame * 10.0f;
	else					m_SpeedUp -= g_fSecPerFrame * 10.0f;
	// �ּҰ����� ����
	if (m_SpeedUp < 1.0f) m_SpeedUp = 1.0f;

	if (I_Input.m_DIMouseState.rgbButtons[1] & 0x80)
	{
		m_fPitch += D3DXToRadian(I_Input.m_DIMouseState.lY) * 0.1f;
		m_fYaw += D3DXToRadian(I_Input.m_DIMouseState.lX) * 0.1f;
		bCameraChange = true;
	}
	UpdateQuaternion();
	return true;
}
bool BCamera::Render()
{
	return true;
}
bool BCamera::Release()
{
	return true;
}
#pragma endregion

#pragma region ī�޶� �⺻��� �����Լ� (View, Proj)
bool BCamera::SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp)
{
	m_vPos = vPos;
	m_vTarget = vTarget;
	m_vUp = vUp;

	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &m_vUp);
	D3DXMATRIX InvmatView;
	D3DXMatrixInverse(&InvmatView, NULL, &m_matView);
	D3DXVECTOR3* pZBasis = (D3DXVECTOR3*)&InvmatView._31;
	m_fYaw = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fPitch = -atan2f(pZBasis->y, fLen);

	UpdateVector();
	return true;
}
bool BCamera::SetProjMatrix(int iWidth, int iHeight, float fFov, float fNear, float fFar)
{
	float fAspect = iWidth / (float)iHeight;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
	return true;
}
#pragma endregion

#pragma region ī�޶� ȸ�� ���� �Լ�
/*���콺�� ���� ȸ���� ��� �Լ�*/
bool BCamera::UpdateQuaternion()
{
	D3DXQUATERNION qRotation;
	// x�� ��İ� y�� ����� ���յǾ� ���ο� ����� ����
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);
	D3DXMATRIX matRotation;
	ZeroMemory(&matRotation, sizeof(matRotation));
	D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
	// ������� ��ķ� ��ȯ���ִ� �Լ� ( ��ȣ ��ȯ ���� )
	// �̴� ������Ʈ ����̹Ƿ� ī�޶� ��ķ��� ��ȯ�� �ʿ� ( ����� )
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	// �̰��� ����ķ� �̿�
	// ȸ������� ��������Ƿ� �̵������ �߰�
	UpdateVector();

	m_matView._41 = -(D3DXVec3Dot(&m_vPos, &m_vSide));
	m_matView._42 = -(D3DXVec3Dot(&m_vPos, &m_vUp));
	m_matView._43 = -(D3DXVec3Dot(&m_vPos, &m_vLook));
	return true;
}
/*ȸ���� �ݿ� �Լ�*/
bool BCamera::UpdateVector()
{
	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	return true;
}
#pragma endregion

#pragma region ī�޶� �̵� �ݿ� �Լ�
void BCamera::MoveLook(float fValue)
{
	m_vPos += m_vLook * fValue;
}
void BCamera::MoveSide(float fValue)
{
	m_vPos += m_vSide * fValue;
}
void BCamera::MoveUp(float fValue)
{
	m_vPos += m_vUp * fValue;
}
#pragma endregion


BCamera::BCamera()
{
}
BCamera::~BCamera()
{
}
