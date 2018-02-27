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
		//float		g_fogMode;		x(������)
		//float		g_fogDensity;	y(��)
		//float		g_fogStart;		z(�������װŸ�)
		//float		g_fogEnd;		w(�������װŸ�)
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
	/*Engine��*/
	vector<BWaterMap*> m_vWaterQuadTree;
	BRenderPlane m_MiniMap[3];


	/*�� ������ ����*/
	BMapTileRender m_MapQuadTree;
	BMap SampleMap;

	/*�ڽ� �浹�� ����*/
	BSelect SampleSelect;
	BQuadTree SampleObjQuad;
	BBox* m_pBoxShape;
	B_BoxObject* m_pObject;
	int m_iDrawDepth;

public:
	/*Engine��*/
	void DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

	/*�ڽ� �浹�� �Լ�*/
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

