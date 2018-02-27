#pragma once
#include "BQuadTree.h"
#include "BNMap.h"

//====================================================================================//
/*�� ��������*/

/*Build �Լ��� ���� ���� �����մϴ�.*/
/*������ ���ÿ� ����Ʈ�� & LOD�� ����Ǹ� ���� ũ�Ⱑ 2�� �������� �ƴϸ� �������� �ʽ��ϴ�.*/
/*�������� �ؽ����� ��� ���м��� ���� 10�� �� ũ�� ����ǰ� �ֽ��ϴ�.*/
/*���� ���� 1000 �̻��� �� ���۽� �������� �ؽ��İ� ������ �Ұ��� �ϹǷ� ���� �� ��� 10�踦 �����ϼž� �մϴ�.*/
/*OMSetState �Լ��� ��� ����̽����� DX���� DSV, DSS, BS�� ���� ������ �ξ��� ������ RTV�� �Ѱ��ָ� �˴ϴ�.*/


/*������Ʈ �������� ����*/ 

//1. ���� - �� ������ ������ ���ɿ���
//2. �׸� - �̵��Ұ�����
//3. ��� - ������ ������ ���ɿ���
//====================================================================================//

#pragma region LOD ���� ����ü
typedef vector<DWORD> DWORD_VECTOR;
class BStaticData
{
public:
	DWORD dwLevel;
	vector<DWORD_VECTOR> IndexList;
	vector<ComPtr<ID3D11Buffer>> IndexBufferList;
public:
	BStaticData();
	~BStaticData();
};
#pragma endregion

class BMapTileRender : public BQuadTree
{
public:

	BNMap* m_pMap;								// ���� ��
	int m_iMinSplitDistance;					// �ּҺ���ũ��
	DWORD m_dwWidth;							// �� ����ũ�� (������ ���� ���ο� ����)
	DWORD m_dwHeight;							// �� ����ũ�� (������ ���� ���ο� ����)
	ID3D11DeviceContext* m_pImmediateContext;	// ���� ���ؽ�Ʈ
	vector<BStaticData*> m_pdwLodIndexArray;	// ����� LOD �ε��� �迭

	//============================ ���� ������ ���� ����============================//
	PNCT_Vertex* m_pVertexList;					// ���ؽ� ����Ʈ (���� ������ ����)
	vector<DWORD> m_IndexList;					// �ε��� ����Ʈ (���� ������ ����)
	int m_iNumFace;								// ���̽� ���� (���� ������ ����)
	bool m_bSharedVertexBuffer;					// �������� ���ۻ�� (���� ������ ����)

	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// �ε��� ���� (���� ������ ����)
	ComPtr<ID3D11Buffer> m_pTileIndexBuffer;	// �������� �ε��� ���� (���� ������ ����)

	size_t m_ibOffset;							// �ε��� ���� �����°� (��ü �ε����� ũ�⸦ �ջ�)
	size_t m_dstibOffset;						// �������� �ε������� �����°� (���� �ε��� ũ��)
	//==============================================================================//

	//============================ �� �������� ���� ���� ============================//
	map<int, TCHAR*> m_pSplattingTexName;	// �� �ؽ��� �̸�
	D3D11_TEXTURE2D_DESC desc;				// �������� �ؽ��� ���� (�� �ؽ���)
	ID3D11Texture2D* StagingTexTure;		// ������ �ؽ��� (�� �ؽ���)
	ID3D11Texture2D* SpreatingTexTure;		// ������ �ؽ��� (�� �ؽ���)

	D3D11_TEXTURE2D_DESC Objdesc;			// �������� �ؽ��� ���� (�� ��������)
	ID3D11Texture2D* StagingObjPosTexture;  // ������ �ؽ��� (�� ��������)
	ID3D11Texture2D* ObjPosTexture;			// ������ �ؽ��� (�� ��������)

	ComPtr<ID3D11ShaderResourceView> SpreatingSRV;				// ������ ���̴� ���ҽ� �� ( �� �ؽ��� )
	ComPtr<ID3D11ShaderResourceView> SpreatingObjSRV;			// ������ ���̴� ���ҽ� �� ( �� �������� )
	map<int, ComPtr<ID3D11ShaderResourceView>> m_pSplattingSRV; // �������� ���ҽ� ��
	//===============================================================================//

public:
	/*�⺻ ���� �Լ�*/
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext*    pContext);
	virtual bool PreFrame();
	virtual bool Frame(BCamera* pCamera);

	/*�� ���� �� Ʈ�� ���һ��� �Լ�*/
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext);
	/*Ʈ�� ��� �����Լ� (Build ���� ȣ��)*/
	BNode* CreateNode(BNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	/*������ # Ʈ�� ���� �Լ� (����Ʈ�� ������ ȣ��)*/
	bool SubDivide(BNode* pNode);
	/*��庰 �ٿ���ڽ� �����Լ� (CreateNode ����ȣ��)*/
	virtual void ComputeBoundingBox(BNode* pNode);
	/*�ٿ���ڽ� ���̰� ��ȯ�Լ� (ComputeBoundingBox ����ȣ��)*/
	D3DXVECTOR2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	/*���ؽ� ����Ʈ ���� �Լ�(Build - SubDevide ���� ȣ��)*/
	bool CreateVertexList(BNode* pNode);
	/*���ؽ� ���� ���� �Լ�(Build - SubDevide ���� ȣ��)*/
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);
	/*�ε��� ����Ʈ ���� �Լ�(Build ���� ȣ��)*/
	int	 CreateIndexList(BNode* pNode);
	/*�ε��� ���� ���� �Լ�(Build ���� ȣ��)*/
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);


	/*����� LOD �� ���� �Լ�*/
	void SetRenderDepth(DWORD dwDepth);
	/*LOD �ε��� ����Լ�*/
	bool ComputeStaticLodIndex(int iMaxCells);
	/*LOD �ε��� ���� �����Լ� */
	HRESULT CreateLODIndexBuffer(BStaticData* pData);
	/*LOD �ε��� ��ȯ�Լ�*/
	DWORD GetIndexCounter(DWORD dwData, DWORD dwNumCell);

	
	/*�������ÿ� �ؽ��� �����Լ�*/
	HRESULT CreateSpreatingTexture();
	/*�ؽ��� ���� �Լ�*/
	void Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius = 1.0f, int iColor = 0); // ��������
	/*�ؽ��� ���� �Լ�*/
	void SaveSpreating(ID3D11DeviceContext* pImmediateContext);
	/*�׶���Ʈ ��� �ؽ��� ���� �Լ�*/
	void GradientSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor);
	/*�ؽ��� �ε� �Լ�*/
	HRESULT SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor);
	/*������Ʈ �������� �ؽ��� ���� �Լ�*/
	void ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor);
	/*������ �ؽ��� ���� �Լ�*/
	HRESULT CreateSRV(ID3D11Texture2D* SpreatingTex, ID3D11Texture2D* StagingTex, D3D11_TEXTURE2D_DESC SpreatingDescV);
	/*���� �ؽ��� ���� ����*/
	bool AlphaRender(ID3D11DeviceContext* pContext);


	/*�ٿ�� �ڽ� �翬�� �Լ�(���)*/
	void UpdateBoundingBox(BNode* pNode);

	/*���� �ε��� ����Ʈ �����Լ�*/
	int UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod);

	/*��� �ε��� ��ȯ�Լ�*/
	int GetIndex(DWORD dwIndex, DWORD dwTL);

	/*�������� ��ü���� �Լ�*/
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);

public:
	BMapTileRender();
	~BMapTileRender();
};

