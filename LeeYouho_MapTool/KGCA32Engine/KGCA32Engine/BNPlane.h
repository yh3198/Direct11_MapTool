#pragma once
#include "BPlane.h"

//====================================================================================//
/*�븻������ ���� Tangent ���� �߰��� ������ �÷��Դϴ�.*/
/*������ ���� �÷� Ŭ������ �����մϴ�.*/
//====================================================================================//

class BNPlane : public BPlane
{
public:
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
public:
	BNPlane();
	~BNPlane();
};

