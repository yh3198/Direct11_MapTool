#pragma once
#include "BRenderPlane.h"

//====================================================================================//
/*높이값 전용 랜더플랜 클래스입니다.*/
/*기존 랜더플랜과 사용법은 동일합니다.*/
//====================================================================================//

class BRenderPlaneHeight : public BRenderPlane
{
public:
	bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, FLOAT Width, FLOAT Height);
public:
	BRenderPlaneHeight();
	~BRenderPlaneHeight();
};

