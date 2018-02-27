#pragma once
#include "BCore.h"
#include "BAseObj.h"

//====================================================================================//
// 툴에서 제작한 내용을 텍스트파일로 저장하기 위한 클래스입니다.
// 저장 순서는 다음과 같습니다.
// 1. SetExporter
// 2. ExpMapObjInfo
// 3. ExpAlphaMapTex
// 4. ExpSpawnMapTex
// 5. ExpTexName
// 6. ExpWaterMapInfo (워터맵 존재시 호출)
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
	/*공용*/
	FILE* m_pStream;					// 저장할 파일 포인터
	ID3D11DeviceContext* m_pContext;	// 메인 콘텍스트

	/*바이너리용*/
	MapTextureName m_TextureInfo;
public:
	/*오브젝트 관련 정보 텍스쳐 파일생성 함수*/
	void ExpSpawnMapTex(ID3D11Texture2D* pTextureDest);
	/*맵 알파값 관련 정보 텍스쳐 파일생성 함수*/
	void ExpAlphaMapTex(ID3D11Texture2D* pTextureDest);

	/*세이브용 파일 생성 및 콘텍스트 맴버화 함수*/
	bool SetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName);

	/*텍스트 파일 내 저장함수*/
	// 텍스쳐명
	void ExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName);
	// 맵 형태정보
	void ExpMapInfo(BMapDesc* pMapInfo);
	// 맵 높이정보
	void ExpHeightTotal(BNode* RootNode);
	// 맵 높이정보 (재귀, ExpHeightTotal 내부호출)
	void ExpHeight(BNode* Node);
	// 워터맵 정보
	void ExpWaterMapInfo(int iSelectedMapNum, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fWaterHeight, float fWaterWave);
	// 맵 오브젝트 정보
	void ExpMapObjInfo(vector<BAseObj*>* ObjMgr);

	//=========================================//
					/*바이너리식*/
	//=========================================//
	// 맵 오브젝트 정보
	void BExpMapObjInfo(vector<BAseObj*>* ObjMgr);
	bool BSetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName);
	void BExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName);
public:
	MapExporter();
	~MapExporter();
};

