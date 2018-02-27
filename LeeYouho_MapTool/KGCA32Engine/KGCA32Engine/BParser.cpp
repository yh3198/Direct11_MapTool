#define _CRT_SECURE_NO_WARNINGS
#include "BParser.h"



#pragma region ���� ����/���� �Լ�
/*���� ���� �Լ�*/
bool BParser::OpenStream(const TCHAR* pFileName)
{
	m_pStream = _tfopen(pFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		return CloseStream(_T("������ ã�� ���߽��ϴ�."));
	}
	return true;
}
/*���� ���� �Լ�*/
bool BParser::CloseStream(TCHAR* pMsg)
{
	if (!m_pStream) return false;
	fclose(m_pStream);
	if (pMsg != NULL)
	{
		MessageBox(0, pMsg, _T("Fatal error"), MB_OK);
	}
	return true;
}

#pragma endregion

#pragma region ��ū��� �Ľ� ���� �Լ�
/*���ڿ� �迭(pObjectType) �˻� �Լ�*/
int BParser::GetSearchStringArray(TCHAR** pObjectType, int iNumObjType, bool bLoop)
{
	SaveFilePosition();		  // ���� �����͸� �����Ѵ�.
	while (!feof(m_pStream))  // ������ ���� �ƴ϶��
	{
		GetNextTokenString(); // ��ū �˻�
		for (int iType = 0; iType < iNumObjType; iType++)
		{
			if (!_tcsicmp(m_pString, pObjectType[iType])) //���ڿ��� ���� ��ū�� ���� �������
			{
				return iType;							  //���� ���� Ÿ���� ����
			}
		}
		if (!bLoop) break;
	}
	RestoreFilePosition();								  //���������� ����
	return -1;
}
/*���� ��ġ ������ ���� �����Լ�*/
void BParser::SaveFilePosition()
{
	if (fgetpos(m_pStream, &m_FilePosition)) // ���� �����͸� �����ϴ� �Լ�
	{
		CloseStream(_T("�۾����� ���������͸� �Ҿ���Ƚ��ϴ�."));
	}
}
/*���� ��ġ ������ ���� �����Լ�*/
void BParser::RestoreFilePosition()
{
	if (fsetpos(m_pStream, &m_FilePosition)) // ���� �����͸� �ҷ����� �Լ�
	{
		CloseStream(_T("�۾����� ���������͸� �Ҿ���Ƚ��ϴ�."));
	}
}
/*��ū �˻� �Լ�*/
TCHAR* BParser::GetNextTokenString()
{
	_fgetts(m_pBuffer, 256, m_pStream);			//m_pBuffer�� ������ �о�ͼ�
	_stscanf(m_pBuffer, _T("%s"), m_pString);	//m_pString�� ���ڿ����� �ű��.
	return m_pString;							//m_pString (�Ű��� ���ڿ� = ��ū) �� ���
}
/*���� �˻� �Լ�*/
TCHAR* BParser::GetNextLine()
{
	_fgetts(m_pBuffer, 256, m_pStream);
	return m_pBuffer;
}
/*������ ���� ���� �Լ�*/
bool BParser::GetData(TCHAR* pString, VOID* pData, INT DataType)
{
	SaveFilePosition();
	while (!feof(m_pStream))
	{
		TCHAR* pLineString = GetNextTokenString();
		if (pString == NULL || !_tcsicmp(pLineString, pString))
		{
			if (GetData(pData, DataType))
			{
				return true;
			}
		}
	}
	RestoreFilePosition();
	return false;
}
bool BParser::GetData(VOID* pData, INT DataType)
{
	if (pData != NULL)
	{
		switch (DataType)
		{
		case INT_DATA:
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, pData);
			break;
		case FLOAT_DATA:
			_stscanf(m_pBuffer, _T("%s%f"), m_pString, pData);
			break;
		case STRING_DATA:
		{
			TCHAR szTexPath[MAX_PATH] = TEXT("");
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, szTexPath);
			if (szTexPath[0] == '"')
			{
				// ���ڿ��� ���۰� ���� �ִ� (")���ڸ� �����Ѵ�.
				size_t filelen = _tcslen(szTexPath);
				szTexPath[filelen - 1] = 0;
				_tcscpy((TCHAR*)pData, &szTexPath[1]);
			}
			else
			{
				_tcscpy((TCHAR*)pData, &szTexPath[0]);
			}
		}
		break;
		case VECTOR_DATA:
		{
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString, &((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->z, &((D3DXVECTOR3*)pData)->y);
		}
		break;
		case POS_DATA:
		{
			_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
				m_pString,
				&m_iData,
				&((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->z, &((D3DXVECTOR3*)pData)->y);
		}break;
		case TEX_DATA:
		{
			_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
				m_pString,
				&m_iData,
				&((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->y, &((D3DXVECTOR3*)pData)->z);

			D3DXVECTOR3* v = (D3DXVECTOR3*)pData;
			v->y = 1.0f - v->y;
		}break;
		case COLOR_DATA:
		{
			_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
				m_pString,
				&m_iData,
				&((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->y, &((D3DXVECTOR3*)pData)->z);
		}break;
		case ANIPOS_DATA:
		{
			BAnimTrack TempData;
			ZeroMemory((void*)&TempData, sizeof(BAnimTrack));
			_stscanf(m_pBuffer, _T("%s%d%f%f%f"),//%f%f%f
				m_pString,
				&TempData.iTick,
				&TempData.vVector.x,
				&TempData.vVector.z,
				&TempData.vVector.y
			);
			((vector<BAnimTrack>*)pData)->push_back(TempData);
		}break;
		case ANIROT_DATA:
		{
			BAnimTrack TempData;
			ZeroMemory((void*)&TempData, sizeof(BAnimTrack));
			_stscanf(m_pBuffer, _T("%s%d%f%f%f%f"),//%f%f%f
				m_pString,
				&TempData.iTick,
				&TempData.qRotate.x,
				&TempData.qRotate.z,
				&TempData.qRotate.y,
				&TempData.qRotate.w
			);
			((vector<BAnimTrack>*)pData)->push_back(TempData);
		}break;
		case ANISCL_DATA:
		{
			BAnimTrack TempData;
			ZeroMemory((void*)&TempData, sizeof(BAnimTrack));
			_stscanf(m_pBuffer, _T("%s%d%f%f%f %f%f%f%f"),//%f%f%f
				m_pString,
				&TempData.iTick,
				&TempData.vVector.x,
				&TempData.vVector.z,
				&TempData.vVector.y,
				&TempData.qRotate.x,
				&TempData.qRotate.z,
				&TempData.qRotate.y,
				&TempData.qRotate.w
			);
			((vector<BAnimTrack>*)pData)->push_back(TempData);
		}break;
		}
	}
	return true;
}
#pragma endregion



BParser::BParser()
{
}
BParser::~BParser()
{
}