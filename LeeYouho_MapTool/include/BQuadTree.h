#pragma once
#include "BNode.h"
#include "BCamera.h"
#include "BMapLOD.h"

//====================================================================================//
/*�� ��������*/

/*Build �Լ��� ���� ���� �����մϴ�.*/
/*������ ���ÿ� ����Ʈ�� & LOD�� ����Ǹ� ���� ũ�Ⱑ 2�� �������� �ƴϸ� �������� �ʽ��ϴ�.*/
//====================================================================================//

class BQuadTree : public BMapLOD
{
public:
	BNode* m_pRootNode;			// ����Ʈ���� ��Ʈ(�ֻ���)���
	float m_fWidth;				// ����Ʈ���� ���ι���
	float m_fHeight;			// ����Ʈ���� ���ι���
	int m_iMaxDepth;			// ��������� �ִ� ����
	int m_iMaxDepthLimit;		// ��� ������ �ִ��ѵ�
	int m_iRenderDepth;			// �ִ� LOD ���� ����
	float m_fMinDivideSize;		// �ּ� ����ũ��

	vector<BObject*> m_DrawObjList;			// ī�޶� ���ν��� �� ������Ʈ ����Ʈ ( �ڽ� ���� )
	vector<MapObjInfo> m_DrawMapObjList;	// ī�޶� ���ν��� �� ������Ʈ ����Ʈ ( �� ������Ʈ ���� )
	int m_iTotalMapObj;						// �� ��ü �ʿ�����Ʈ ��
	queue<BNode*> m_QuadTreeQueue;			// ��� �˻��� ť
	bool m_bUsedIndexList;					// ���� �ε����� �̿��� �������� ��� ����

public:
	/*�⺻ ���� �Լ�*/
	virtual bool Frame(BCamera* pCamera);
	virtual bool Release();

	/*�� ���� �� Ʈ�� ���һ��� �Լ�*/
	bool Build(float fWidth, float fHeight);
	/*Ʈ�� ���� �Լ�*/
	virtual bool BuildTree(BNode* pNode);
	/*��� ���� �Լ�*/
	BNode* CreateNode(BNode* pParentNode, float fLeft, float fRight, float fBottom, float fTop); // ��� �����Լ�
	/*��� ���� �Ǻ� �Լ�*/
	virtual bool SubDivide(BNode* pNode);
	/*���� ���� ������ üũ �Լ�*/
	DWORD CheckSize(DWORD dwSize);

	/*��� �˻��� �Լ�*/
	BNode* FindNode(BNode* pNode, BObject* pObj);
	/*���ν��� & ��� �浹�Ǻ� �Լ�*/
	virtual void DrawFindNode(BNode* pNode, BCamera* pCamera);
	/*���� �� ��� �к� �Լ�*/
	void VisibleNode(BNode* pNode, BCamera* pCamera);
	/*�̿� ��� ���� �Լ�*/
	void SetNeighborNode(BNode* pNode);
	/*�̿� ��� �˻� �Լ�*/
	void FindNeighborNode(BNode* pNode);

	/*����Ʈ�� ���� ������Ʈ �߰� �Լ�*/
	int AddObject(BObject* pObj);
	/*����Ʈ�� �� �� ������Ʈ ���� �Ǻ��� �Լ�*/
	int CheckRect(BNode* pNode, BObject* pObj);
	/*���� �� ������Ʈ �к��Լ�*/
	void VisibleObject(BNode* pNode, BCamera* pCamera);
	/*�� ������Ʈ �߰� �Լ�*/
	void AddMapObject(BNode* pNode, vector<InstancingInfo> MapInfo);
	/*�� ������Ʈ�� ��ġ �Ǻ��� �Լ�*/
	int CheckPosition(BNode* pNode, D3DXVECTOR3 pObj);
	/*��ü ��� �� �� ������Ʈ ���� �Լ�*/
	void ClearObject(BNode* pNode);


	
public:
	BQuadTree();
	virtual ~BQuadTree();
};

