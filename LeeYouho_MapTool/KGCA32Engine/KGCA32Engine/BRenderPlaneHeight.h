#pragma once
#include "BRenderPlane.h"

//====================================================================================//
/*���̰� ���� �����÷� Ŭ�����Դϴ�.*/
/*���� �����÷��� ������ �����մϴ�.*/
//====================================================================================//

class BRenderPlaneHeight : public BRenderPlane
{
public:
	bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, FLOAT Width, FLOAT Height);
public:
	BRenderPlaneHeight();
	~BRenderPlaneHeight();
};

