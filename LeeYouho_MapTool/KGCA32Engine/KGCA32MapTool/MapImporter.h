#pragma once
#include "BCore.h"
#include "BAseObj.h"

//====================================================================================//
// ������ ���۵� ������ �ҷ����� ���� Ŭ�����Դϴ�.
// ȣ�� ������ ������ �����ϴ�.
// 1. OpenStream
// 2. LoadMapObj
// 3. LoadMap
//====================================================================================//

class MapImporter
{
public:
	ID3D11DeviceContext* m_pImmediateContext;	// ���� ���ؽ�Ʈ
	ID3D11Device* m_pd3dDevice;					// ���� ����̽�
	BCamera* m_MainCamera;						// ���� ī�޶�
	ID3D11Texture2D* StagingTexTure;			// �� ��ȯ�� �ؽ���

	FILE* m_pStream;					// �ε��� ���� ������
	fpos_t m_FilePosition;				// ���� �������� ��ġ
	TCHAR m_pBuffer[256];				// ���� ���ڿ� ���� (��ü)
	TCHAR m_pString[256];				// ���� ���ڿ� ���� (�κ�)
	TCHAR MapTexture[256];				// ���� ���ڿ� ���� (�� �⺻�ؽ���)
	TCHAR m_pSplatting[4][256];			// ���� ���ڿ� ���� (�� �����̿� �ؽ��ĸ�)
	TCHAR m_WaterMapTex[256];			// ���� ���ڿ� ���� (���͸� �ؽ��ĸ�)
	TCHAR m_WaterNormalMapTex[256];		// ���� ���ڿ� ���� (���� �븻�� �ؽ��ĸ�)

	int iCellCount;				// �� ����
	int iPatchCount;			// �� ��ġ��
	float fCellDistance;		// �� �� �Ÿ���
	
	BAseObj* m_MapObj;			// �� ������Ʈ

public:
	/*�ε� ���� ���� �� ���� �� �����Լ�*/
	bool OpenStream(const TCHAR* pFileName, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, BCamera* MainCamera);
	
	/*���� ��ü�ε� �Լ�*/
	bool LoadMap(vector<BMap*>* m_vMFCMap, vector<BMapDesc*>* m_vMFCMapInfo, vector<BMapTileRender*>* m_vMapQuadTree, vector<TCHAR*>* m_vMapTexture, map<int, TCHAR*>* m_mMapSplattingTexture, vector<BWaterMap*>* m_vWaterQuadTree);
	/*���� �ؽ��� ���� �Լ� (LoadMap ���� ȣ��)*/
	ID3D11Texture2D* LoadAlphaTexture(TCHAR* texture, BMap* Map);
	/*������Ʈ ���� �ؽ��� ���� �Լ� (LoadMap ���� ȣ��)*/
	ID3D11Texture2D* LoadSpawnTexture(TCHAR* texture, BMap* Map);
	/*���̰� ���� �Լ� (LoadMap ���� ȣ��)*/
	void GetHeightFromTex(BNode* Node);

	/*������Ʈ �ε� �Լ�*/
	int LoadMapObj(vector<BAseObj*>* ObjMgr);

	//=========================================//
					/*���̳ʸ���*/
	//=========================================//


public:
	MapImporter();
	~MapImporter();
};

