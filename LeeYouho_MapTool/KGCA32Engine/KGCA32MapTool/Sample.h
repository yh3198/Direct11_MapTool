#pragma once
#include "BCore.h"
#include "MapExporter.h"
#include "MapImporter.h"
#include "BMapSetting.h"
#include "BParser.h"
#include "BAseObj.h"

#pragma region MFC ��� ���� ���� ����ü
enum VertexControlerState
{
	vNone = 0,
	vUp,
	vDown,
	vParallel,
};

enum SpreatingState
{
	vSpreatNone = 0,
	vSpreatApply,
};

enum SpreatingColor
{
	vRed = 0,
	vGreen,
	vBlue,
	vAlpha,
};

struct SelectedMapObj
{
	int m_iObjType; // ������Ʈ ����
	int m_iObjNum;  // �ش� ������Ʈ�� vecter����
	float m_fRange; // ������ �������� ������ �Ÿ�
};
#pragma endregion

class Sample : public BCore
{
public:
#pragma region �⺻ ���� ���� ����
	BMapDesc*			m_MFCMapInfo;			// �� ����
	BMap*				m_MFCMap;				// ��
	BMapTileRender*		m_MapQuadTree;			// ���� ��
	
	vector<BMapDesc*>		m_vMFCMapInfo;		// �� ���� ����
	vector<BMap*>			m_vMFCMap;			// �� ����
	vector<BMapTileRender*> m_vMapQuadTree;		// ���� �� ����

	MapExporter m_Exporter;						// �� ��� Ŭ����
	MapImporter m_Importer;						// �� �Է� Ŭ����

	BSelect m_Select;							// ���콺 �浹 ���� Ŭ����
	vector<int>			m_viClosedIndex;		// ���� �浹�� Ȯ�ο� �ε��� (���ý�)

	vector<TCHAR*>		m_vMapTexture;			// �� �ؽ��ĸ�
	map<int, TCHAR*>	m_mMapSplattingTexture;	// �� ���÷��� �ؽ��Ŀ�

	int m_VCtrlState;			// �� �������� ����Ȯ�ο� ���� 
	int m_VSpreatState;			// �� �׸��� ����Ȯ�ο� ����
	int m_VObjSpreatState;		// ������Ʈ �����ؽ��� �׸���� ����

	/*�� ���� ������ ����*/
	float	m_fRadius;			// ���콺 ��ŷ����
	float	m_fHeightValue;		// ���õ� ������ ���� ��
	bool	bParallel;			// �� ��źȭ ���
	float	fParallel;			// ���õ� ������ ���� ��(��źȭ ���� Ȱ��ȭ)

	/*�� �������ÿ� ����*/
	B_BOX	SpreatingBox;		// �������� ���ɿ��� �ִ� �ּҹ���uv��
	float	m_fSpreatingRadius;	// �������� ����
	int		m_iSpreatingColor;	// �������� ����
	bool	m_ModeGradient;		// �׶���Ʈ �������� ����
	int		ColoringCnt;		// ���ӱ׸��� ������ ����

	/*���� �� ���� �ʿ� ����(���� �̻��)*/
	BMapTileRender* m_bSelectedMap; // ���߿� ���� ������ �����Ǿ��� �� �������־���Ѵ�. ������ �⺻ ������ 0������ �־��� ���̴�.
	int m_iSelectedMap;				// ���߿� ���� ������ �����Ǿ��� �� �������־���Ѵ�. ������ �⺻ ������ 0������ �־��� ���̴�.
#pragma endregion

#pragma region ���͸� ���� ����
	/*���͸� ����Ʈ ����*/
	D3DXVECTOR3	m_vInitLightPos;	// ���� ����Ʈ ��ġ
	D3DXMATRIX	m_mLightWorld;		// ��ȯ�� ����Ʈ ��ġ�� ���
	D3DXVECTOR3 m_LightPos;			// ��ȯ�� ����Ʈ ��ġ
	D3DXVECTOR3 m_vDirection;		// ��ȯ�� ����Ʈ ����
	
	/*���͸� ������ ����*/
	BMapDesc* m_MFCWaterMapInfo;			// ���͸� ����
	BNMap* m_MFCWaterMap;					// ���͸� ��� ��
	BWaterMap* m_WaterMapQuadTree;			// ���͸�
	vector<BNMap*> m_vMFCWaterMap;			// ���͸� ��� �� ���� 
	vector<BWaterMap*> m_vWaterQuadTree;	// ���͸� ����
#pragma endregion

#pragma region �������� ���� ����
	BMapSetting m_MapSetting;				// �� ���� �������� ���� Ŭ����
	vector<TCHAR*> Text;					// �������ÿ� �ؽ��ĸ� (�� �ؽ���)
	int m_iSpreatingRange;					// ���γ��� ���� ����
	int m_iObjSpreatingColor;				// ���γ��� ���� ���� 
											// (����� ���� : ���� ���� / ��� : ������ ���� / �׸� : �̵��Ұ� ����)
	bool m_MapSettingRenderMode;			// ȭ�� �������
#pragma endregion

#pragma region ������Ʈ ���� ����
	vector<BAseObj*> m_MapObjMgr;						// �� ������Ʈ ����
	int m_MapObjTotalCnt;								// �� ������Ʈ �� ����
	vector<SelectedMapObj> m_vSelectedObjInfo;			// ���õ� ������Ʈ ������
	vector<SelectedMapObj> m_vBeforeSelectedObjInfo;	// ���� ���� ������Ʈ ������
	vector<int> m_CreateObjType;						// ������Ʈ ������ �ε��� ����
	
	bool ObjSettingMode;		// ������Ʈ �������
	bool ObjCreateOne;			// ������Ʈ ������� (���ϻ����� ����)
	bool ObjSelectingMode;		// ������Ʈ ���ø��
	bool ObjMovingMode;			// ������Ʈ �̵����
	bool ObjRenderMode;			// ������Ʈ �������
	bool BoxRenderMode;			// ������Ʈ MinMax�ڽ� �������
	
	SelectedMapObj m_TempInfo;				// ���� ������Ʈ �ӽ������
	vector<vector<int>> m_DrawObjList;		// ������ ������Ʈ ����

	BAseObj m_RenderSettingASE;				// ASE ������ VS, PS, Layout ������ ���� ����
	BShadow m_Shadow;						// ������ ����
#pragma endregion

#pragma region ������ ���� ����
	// �� ����׿�
	bool bMapDebug;							// ���͸� & ������ �ݻ�, ����, ���� Ȯ�θ��
#pragma endregion

public:
#pragma region �⺻ �����Լ�
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
#pragma endregion

#pragma region �⺻ ���� ���� �Լ�
	/*�� ���� �Լ�*/
	bool CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex = NULL);
	/*���� �� ���� �Լ�*/
	bool CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map);
	/*�� ���� ��Ʈ�� �Լ�*/
	bool MapHeightControl();
	/*�� ���� ���� �Լ�(��, �ٿ� / MapHeightControl ���� ȣ��)*/
	D3DXVECTOR2 ChangeYPosition(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	/*�� ���� ���� �Լ�(���� / MapHeightControl ���� ȣ��)*/
	D3DXVECTOR2 ChangeYParallel(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	/*�� ���� ��ȯ�� �Լ�*/
	D3DXVECTOR4 GetHeight(D3DXVECTOR3 HeroPos);
	
	/*�������� �Լ�*/
	bool MapSpreating();
	/*�������ÿ� �ؽ��� ���� �Լ�*/
	bool GetSplattingTexName(int Color, TCHAR** Name);
	
	/*�� ���� �Լ�*/
	bool MapExport();
	/*�� ȣ�� �Լ�*/
	bool MapImport(TCHAR* ImportFile);
#pragma endregion

#pragma region ���͸� ���� �Լ�
	/*���͸� ���� �Լ�*/
	bool CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave);
#pragma endregion

#pragma region �� ���� ���� �Լ�
	/*�� �������� �ؽ���ȭ ���� �Լ�*/
	bool MapObjInfoSpreating();
	/*���Ҹ� �븻 �����Լ�*/
	void CreateNormal();
	/*���Ҹ� �븻 ����Լ� (���, CreateNormal ���� ȣ��)*/
	void CalcNormal(BNode* pNode, int iRatio);
#pragma endregion

#pragma region �� ������Ʈ ���� �Լ�
	/*������Ʈ �����Լ�*/
	void SelectMapObj();
	/*������Ʈ �����Լ�*/
	void ClearSelected();
	/*������Ʈ �̵��Լ�*/
	bool GetMapObjectMovePosition();
	/*������Ʈ �����Լ�*/
	bool GetMapObjectPosition();
#pragma endregion

#pragma region ������ ���� �Լ�
	/*������ ���� �Լ�*/
	void RenderShadow();
#pragma endregion

public:
	Sample();
	~Sample();
};

