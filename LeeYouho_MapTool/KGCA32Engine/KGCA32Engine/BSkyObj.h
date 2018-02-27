#pragma once
#include "BBox.h"

//====================================================================================//
/*스카이박스 제작 클래스입니다.*/
/*박스와 사용법은 동일합니다.*/
/*Create 생성시 6장의 텍스쳐가 필요합니다.*/
/*스카이박스 텍스쳐순서*/
// 앞, 위, 오른쪽, 아래, 뒤, 왼쪽
//====================================================================================//


class BSkyObj : public BBox
{
public:
	ID3D11ShaderResourceView* m_pTextureSkySRV[6]; // 스카이 박스 텍스쳐

public:
	/*재정의 # 행렬 설정 함수*/
	void BSkyObj::SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);

public:
	BSkyObj();
	~BSkyObj();
};