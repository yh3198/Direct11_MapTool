#pragma once
#include "BCore.h"
#include "BAseObj.h"

//====================================================================================//
// ������ ������ ������ �ؽ�Ʈ���Ϸ� �����ϱ� ���� Ŭ�����Դϴ�.
// ���� ������ ������ �����ϴ�.
// 1. SetExporter
// 2. ExpMapObjInfo
// 3. ExpAlphaMapTex
// 4. ExpSpawnMapTex
// 5. ExpTexName
// 6. ExpWaterMapInfo (���͸� ����� ȣ��)
// 7. ExpMapInfo
// 8. ExpHeightTotal
//====================================================================================//

struct MapTextureName
{
	T_STR AlphaTexture = L"../../data/MapTool_Alpha.bmp";
	T_STR ObjSpawnTexture = L"../../data/MapTool_Spawn.bmp";
	T_STR BaseTexure;
	T_STR SplattingTexture[4];
};

class MapExporter
{
public:
	/*����*/
	FILE* m_pStream;					// ������ ���� ������
	ID3D11DeviceContext* m_pContext;	// ���� ���ؽ�Ʈ

	/*���̳ʸ���*/
	MapTextureName m_TextureInfo;
public:
	/*������Ʈ ���� ���� �ؽ��� ���ϻ��� �Լ�*/
	void ExpSpawnMapTex(ID3D11Texture2D* pTextureDest);
	/*�� ���İ� ���� ���� �ؽ��� ���ϻ��� �Լ�*/
	void ExpAlphaMapTex(ID3D11Texture2D* pTextureDest);

	/*���̺�� ���� ���� �� ���ؽ�Ʈ �ɹ�ȭ �Լ�*/
	bool SetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName);

	/*�ؽ�Ʈ ���� �� �����Լ�*/
	// �ؽ��ĸ�
	void ExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName);
	// �� ��������
	void ExpMapInfo(BMapDesc* pMapInfo);
	// �� ��������
	void ExpHeightTotal(BNode* RootNode);
	// �� �������� (���, ExpHeightTotal ����ȣ��)
	void ExpHeight(BNode* Node);
	// ���͸� ����
	void ExpWaterMapInfo(int iSelectedMapNum, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fWaterHeight, float fWaterWave);
	// �� ������Ʈ ����
	void ExpMapObjInfo(vector<BAseObj*>* ObjMgr);

	//=========================================//
					/*���̳ʸ���*/
	//=========================================//
	// �� ������Ʈ ����
	void BExpMapObjInfo(vector<BAseObj*>* ObjMgr);
	bool BSetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName);
	void BExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName);
public:
	MapExporter();
	~MapExporter();
};

