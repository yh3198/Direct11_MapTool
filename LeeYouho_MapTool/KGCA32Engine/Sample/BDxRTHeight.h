#pragma once
#include "BDxRT.h"
#include "BMap.h"

class BDxRTHeight : public BDxRT
{
public:
	ComPtr<ID3D11Texture2D> m_pHeightTexture;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	HRESULT DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap);
public:
	BDxRTHeight();
	~BDxRTHeight();
};

