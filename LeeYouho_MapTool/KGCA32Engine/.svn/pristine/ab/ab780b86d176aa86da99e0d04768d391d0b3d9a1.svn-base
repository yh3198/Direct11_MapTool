#pragma once
#include "BCore.h"
#include "BWaterMap.h"
#include "BRenderPlane.h"


struct cbFog
{
	D3DXVECTOR4 g_ClipPlane;
	D3DXVECTOR4 g_vCameraPos;
	D3DXVECTOR4 g_FogColor;
	D3DXVECTOR4 g_FogInfo;

		/*g_FogInfo information*/
		//float		g_fogMode;		x(감쇠방법)
		//float		g_fogDensity;	y(농도)
		//float		g_fogStart;		z(시작포그거리)
		//float		g_fogEnd;		w(종료포그거리)
};

class B_BoxObject : public BObject
{
public:
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR4 m_vColor;

public:
	B_BoxObject();
	~B_BoxObject();
};

class Sample : public BCore
{
public:
	/*Engine용*/
	vector<BWaterMap*> m_vWaterQuadTree;
	BRenderPlane m_MiniMap[3];


	/*맵 생성용 변수*/
	BMapTileRender m_MapQuadTree;
	BMap SampleMap;

	/*박스 충돌용 변수*/
	BSelect SampleSelect;
	BQuadTree SampleObjQuad;
	BBox* m_pBoxShape;
	B_BoxObject* m_pObject;
	int m_iDrawDepth;

public:
	/*Engine용*/
	void DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

	/*박스 충돌용 함수*/
	void DrawObject(BCamera* Camera);
	bool FindBoxAndDelete();

public:
	bool		Init();
	bool		Frame();
	bool		Render();	
	bool		Release();

public:
	Sample();
	~Sample();
};

