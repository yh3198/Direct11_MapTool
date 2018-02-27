#pragma once
#include "BMap.h"
//====================================================================================//
/*노말맵핑을 위한 Tangent 값이 추가된 형태의 맵입니다.*/
/*사용법은 기존 맵 클래스와 동일합니다.*/
//====================================================================================//


class BNMap : public BMap
{
public:
	ID3D11VertexShader*		m_pNoneVS; // Clip 전용 버텍스 쉐이더

public:
	/*쉐이더 레이아웃 생성 함수*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	
	/*행렬 세팅용 함수*/
	void SetMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);
	
	/*설정된 Clip에 의한 랜더링 전용 함수*/
	bool RenderNoneClip(ID3D11DeviceContext* pContext);

public:
	BNMap();
	~BNMap();
};

