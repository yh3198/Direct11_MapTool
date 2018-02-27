#pragma once
#include "BPlane.h"

//====================================================================================//
/*노말맵핑을 위한 Tangent 값이 추가된 형태의 플랜입니다.*/
/*사용법은 기존 플랜 클래스와 동일합니다.*/
//====================================================================================//

class BNPlane : public BPlane
{
public:
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
public:
	BNPlane();
	~BNPlane();
};

