#pragma once
#include "BTimer.h"
#include "BInput.h"
#include "BWrite.h"
#include "BBackViewCamera.h"
#include "BDirectionLine.h"
#include "BPlane.h"
#include "BSphere.h"
#include "BSkyObj.h"
#include "BSound.h"
#include "BWaterMap.h"
#include "BRenderPlane.h"
#include "BAseObj.h"
#include "BParser.h"
#include "BShadow.h"


/*�� ���� include*/
#include "BMapLOD.h"
#include "BMapTileRender.h"
#include "BSelect.h"

class BCore : public BWindowLib
{
public:
	D3DXMATRIX m_matWorld;  // ������ǥ (���Ͱ�) * ���� ��� = ������ǥ
	D3DXMATRIX m_matView;	// ������ǥ * ����� = ����ǥ (ī�޶��� �̵��� ���� ��ǥ)
	D3DXMATRIX m_matProj;	// ����ǥ * ������� = ������ǥ (ī�޶��� ������ ���� ��ǥ)

	//ID3D11RasterizerState* m_pRS;				// ������ ���� ( ���̾� & �� )
	ComPtr<ID3D11RasterizerState> m_pRS[10];
	bool m_bWireFrameRender;					// ������ ���� �¿����� ����

	BTimer m_Timer;
	BWrite m_Writer;
	BSkyObj m_SkyBox;
	BCamera* m_pMainCamera;
	BCamera* m_pDebugCamera;
	BCamera* m_pTreeCamera;
	BBackViewCamera* m_pBackViewCamera;
	
	BDirectionLine m_DirectionLine;
	bool m_DirectionLineOn;

	D3DXVECTOR4 vDirValue; /*ī�޶� ȸ��, �̵� ����*/

	/*���÷� ����*/
	ID3D11SamplerState* m_pSamplerState;
	D3D11_FILTER m_DXFilter;
	UINT m_iFilterCnt;
	D3D11_TEXTURE_ADDRESS_MODE m_TexAddressMode;
	UINT m_iTexAddressMode;

	D3D11_SHADER_RESOURCE_VIEW_DESC m_DescSRV;
	int m_iMinLod;

public:
	virtual bool		Init();
	virtual	bool		Frame();
	virtual	bool		Render();
	virtual bool		Release();
	virtual	bool		Run();

	virtual bool PreInit();
	virtual bool PreRender();
	virtual bool PreFrame();

	virtual bool PostInit();
	virtual bool PostRender();
	virtual bool PostFrame();

	bool BInit();
	bool BRender();
	bool BFrame();
	bool BRelease();

	bool CreateDxResource();
	bool DeleteDxResource();

public:
	BCore();
	virtual ~BCore();
};

