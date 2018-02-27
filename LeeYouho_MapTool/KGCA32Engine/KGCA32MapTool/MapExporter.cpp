#include "stdafx.h"
#include "MapExporter.h"

//====================================================================================//
//������ ������ ������ �ؽ�Ʈ���Ϸ� �����ϱ� ���� Ŭ�����Դϴ�.
//���� ������ ������ �����ϴ�.
// 1. SetExporter
// 2. ExpMapObjInfo
// 3. ExpAlphaMapTex
// 4. ExpSpawnMapTex
// 5. ExpTexName
// 6. ExpWaterMapInfo (���͸� ����� ȣ��)
// 7. ExpMapInfo
// 8. ExpHeightTotal
//====================================================================================//

#pragma region �ؽ��� ���� ���� �Լ�
/*������Ʈ ���� ���� �ؽ��� ���ϻ��� �Լ�*/
void MapExporter::ExpSpawnMapTex(ID3D11Texture2D* pTextureDest)
{
	D3DX11SaveTextureToFile(m_pContext, pTextureDest, D3DX11_IFF_BMP, L"../../data/MapTool_Spawn.bmp");
}
/*�� ���İ� ���� ���� �ؽ��� ���ϻ��� �Լ�*/
void MapExporter::ExpAlphaMapTex(ID3D11Texture2D* pTextureDest)
{
	D3DX11SaveTextureToFile(m_pContext, pTextureDest, D3DX11_IFF_BMP, L"../../data/MapTool_Alpha.bmp");
}
#pragma endregion

#pragma region �ؽ�Ʈ ���� ���� �Լ�
/*���̺�� ���� ���� �� ���ؽ�Ʈ �ɹ�ȭ �Լ�*/
bool MapExporter::SetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName)
{
	m_pContext = pContext;
	m_pStream = _tfopen(m_szFileName, _T("wt"));
	if (!m_pStream)
	{
		return false;
	}
	return true;
}
/*�ؽ�Ʈ ���� �� �����Լ�*/
// �ؽ��ĸ�
void MapExporter::ExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName)
{
	_ftprintf(m_pStream, _T("%s  "), _T("*AlphaTexture"));
	_ftprintf(m_pStream, _T("%s\n"), _T("../../data/MapTool_Alpha.bmp"));
	_ftprintf(m_pStream, _T("%s  "), _T("*ObjSpawnTexture"));
	_ftprintf(m_pStream, _T("%s\n"), _T("../../data/MapTool_Spawn.bmp"));

	_ftprintf(m_pStream, _T("%s  "), _T("*BaseTexture"));
	_ftprintf(m_pStream, _T("%s\n"), BaseTexName);

	_ftprintf(m_pStream, _T("%s\n"), _T("*SplattingTexture"));
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Red"), pSplattingTexName[0]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Green"), pSplattingTexName[1]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Blue"), pSplattingTexName[2]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Alpha"), pSplattingTexName[3]);
}
// �� ��������
void MapExporter::ExpMapInfo(BMapDesc* pMapInfo)
{
	_ftprintf(m_pStream, _T("%s\t%d\n"), _T("*CellCount"), pMapInfo->iCellCount);
	_ftprintf(m_pStream, _T("%s\t%d\n"), _T("*PatchCount"), pMapInfo->iPatchCount);
	_ftprintf(m_pStream, _T("%s\t%10.4f\n"), _T("*CellDistance"), pMapInfo->fCellDistance);
}
// �� ��������
void MapExporter::ExpHeightTotal(BNode* RootNode)
{
	_ftprintf(m_pStream, _T("%s\t\n"), _T("*Height"));
	ExpHeight(RootNode);
}
// �� �������� (���, ExpHeightTotal ����ȣ��)
void MapExporter::ExpHeight(BNode* Node)
{
	if (Node->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			ExpHeight(Node->m_pChild[iNode]);
		}
	}
	else
	{
		if (Node->m_VertexList.size())
		{
			// m_tBox ��¼��� = Min xyz, Max xyz, Center xyz, fExtent 0, 1, 2
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vMin.x, Node->m_tBox.vMin.y, Node->m_tBox.vMin.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vMax.x, Node->m_tBox.vMax.y, Node->m_tBox.vMax.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vCenter.x, Node->m_tBox.vCenter.y, Node->m_tBox.vCenter.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.fExtent[0], Node->m_tBox.fExtent[1], Node->m_tBox.fExtent[2]);
			for (int iVer = 0; iVer < Node->m_VertexList.size(); iVer++)
			{
				_ftprintf(m_pStream, _T("%f\n"), Node->m_VertexList[iVer].Position.y);
			}
			_ftprintf(m_pStream, _T("\n"));
		}
	}
}
// ���͸� ����
void MapExporter::ExpWaterMapInfo(int iSelectedMapNum, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fWaterHeight, float fWaterWave)
{
	_ftprintf(m_pStream, _T("%s\n"), _T("*WaterMap"));
	_ftprintf(m_pStream, _T("%s %d\n"), _T("*SelectedMapNum"), iSelectedMapNum);
	_ftprintf(m_pStream, _T("%s %s\n"), _T("*WaterTex"), WaterTex);
	_ftprintf(m_pStream, _T("%s %s\n"), _T("*WaterNormalTex"), WaterNormalTex);
	_ftprintf(m_pStream, _T("%s %f\n"), _T("*WaterHeight"), fWaterHeight);
	_ftprintf(m_pStream, _T("%s %f\n"), _T("*WaterWave"), fWaterWave);
}
// �� ������Ʈ ����
void MapExporter::ExpMapObjInfo(vector<BAseObj*>* ObjMgr)
{
	_ftprintf(m_pStream, _T("%s %d\n"), _T("*TotalMpaObjCount"), ObjMgr->size());
	for (int ObjCnt = 0; ObjCnt < ObjMgr->size(); ObjCnt++)
	{
		_ftprintf(m_pStream, _T("%s %d\n"), _T("*MpaObj"), ObjMgr[0][ObjCnt]->m_iInstanceCount);
		_ftprintf(m_pStream, _T("%s%s \n"), (TCHAR*)ObjMgr[0][ObjCnt]->m_szDirName.c_str(), (TCHAR*)ObjMgr[0][ObjCnt]->m_szName.c_str());
		for (int iInstance = 0; iInstance < ObjMgr[0][ObjCnt]->m_InstanceInfo.size(); iInstance++)
		{
			_ftprintf(m_pStream, _T("%d\n"), ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].iInstanceNumber);
			_ftprintf(m_pStream, _T("%f %f %f\n"), ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Angle[0], ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Angle[1], ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Angle[2]);
			_ftprintf(m_pStream, _T("%f %f %f\n"), ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Scale[0], ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Scale[1], ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Scale[2]);
			_ftprintf(m_pStream, _T("%f %f %f\n"), ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Translate.x, ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Translate.y, ObjMgr[0][ObjCnt]->m_InstanceInfo[iInstance].Translate.z);
		}
	}
}
#pragma endregion

#pragma region ���̳ʸ� ���� �����Լ� (�̻��)
/*���̺�� ���̳ʸ����� ���� �� ���ؽ�Ʈ �ɹ�ȭ �Լ�*/
bool MapExporter::BSetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName)
{
	m_pContext = pContext;
	m_pStream = _tfopen(m_szFileName, _T("wb"));
	if (!m_pStream)
	{
		return false;
	}
	return true;
}
// �� ������Ʈ ����
void MapExporter::BExpMapObjInfo(vector<BAseObj*>* ObjMgr)
{
	if (!m_pStream)
	{
		return;
	}
	int iTotalCnt = ObjMgr[0].size();
	fwrite(&iTotalCnt, sizeof(int), 1, m_pStream);
	// �Ž� & ����Ž� ������ �۾�
	for (DWORD dwTotalObjCount = 0; dwTotalObjCount < iTotalCnt; dwTotalObjCount++)
	{
		vector<shared_ptr<BMesh2>> ObjData = ObjMgr[0][dwTotalObjCount]->m_pData;
		for (DWORD dwObjCount = 0; dwObjCount < ObjData.size(); dwObjCount++)
		{
			BMesh2* pMesh = ObjData[dwObjCount].get();
			int iSubCnt = pMesh->m_pSubMesh.size();
			fwrite(&iSubCnt, sizeof(int), 1, m_pStream);
			/*���� �Ž��� ����*/
			if (iSubCnt > 0)
			{
				for (DWORD dwSubObjCount = 0; dwSubObjCount < iSubCnt; dwSubObjCount++)
				{
					BMesh2* pSubMesh = pMesh->m_pSubMesh[dwObjCount].get();
					int iVbCnt = pSubMesh->m_VertexArray.size();
					int iIbCnt = pSubMesh->m_IndexArray.size();
					fwrite(&iVbCnt, sizeof(int), 1, m_pStream);
					fwrite(&iIbCnt, sizeof(int), 1, m_pStream);
					fwrite(pSubMesh->m_TextureName.c_str(), sizeof(TCHAR)*pSubMesh->m_TextureName.length(), 1, m_pStream);
					fwrite(pSubMesh->m_TextureName.c_str(), sizeof(TCHAR)*pSubMesh->m_NormalTextureName.length(), 1, m_pStream);
					if (pSubMesh->m_iNumFace < 1)
					{
						continue;
					}
					else
					{
						for (int iVB = 0; iVB < iVbCnt; iVB++)
						{
							fwrite(&pSubMesh->m_VertexArray[iVB], sizeof(PNCT_Vertex), 1, m_pStream);
						}
						for (int iIB = 0; iIB < iIbCnt; iIB++)
						{
							fwrite(&pSubMesh->m_IndexArray[iIB], sizeof(DWORD), 1, m_pStream);
						}
					}
				}
			}
			/*���� �Ž��� ������� ������*/
			else
			{
				int iVbCnt = pMesh->m_VertexArray.size();
				int iIbCnt = pMesh->m_IndexArray.size();
				fwrite(&iVbCnt, sizeof(int), 1, m_pStream);
				fwrite(&iIbCnt, sizeof(int), 1, m_pStream);
				fwrite(pMesh->m_TextureName.c_str(), sizeof(TCHAR)*pMesh->m_TextureName.length(), 1, m_pStream);
				fwrite(pMesh->m_TextureName.c_str(), sizeof(TCHAR)*pMesh->m_NormalTextureName.length(), 1, m_pStream);
				if (pMesh->m_iNumFace < 1)
				{
					continue;
				}
				else
				{
					for (int iVB = 0; iVB < iVbCnt; iVB++)
					{
						fwrite(&pMesh->m_VertexArray[iVB], sizeof(PNCT_Vertex), 1, m_pStream);
					}
					for (int iIB = 0; iIB < iIbCnt; iIB++)
					{
						fwrite(&pMesh->m_IndexArray[iIB], sizeof(DWORD), 1, m_pStream);
					}
				}
			}
		}
	}
}
void MapExporter::BExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName)
{
	m_TextureInfo.BaseTexure = BaseTexName;
	m_TextureInfo.SplattingTexture[0] = pSplattingTexName[0];
	m_TextureInfo.SplattingTexture[1] = pSplattingTexName[1];
	m_TextureInfo.SplattingTexture[2] = pSplattingTexName[2];
	m_TextureInfo.SplattingTexture[3] = pSplattingTexName[3];
	fwrite(&m_TextureInfo, sizeof(MapTextureName), 1, m_pStream);
}
#pragma endregion



MapExporter::MapExporter()
{
}
MapExporter::~MapExporter()
{
}