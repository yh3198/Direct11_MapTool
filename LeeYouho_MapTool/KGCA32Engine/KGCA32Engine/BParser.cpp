#define _CRT_SECURE_NO_WARNINGS
#include "BParser.h"



#pragma region 파일 실행/종료 함수
/*파일 실행 함수*/
bool BParser::OpenStream(const TCHAR* pFileName)
{
	m_pStream = _tfopen(pFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		return CloseStream(_T("파일을 찾지 못했습니다."));
	}
	return true;
}
/*파일 종료 함수*/
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

#pragma region 토큰기반 파싱 관련 함수
/*문자열 배열(pObjectType) 검색 함수*/
int BParser::GetSearchStringArray(TCHAR** pObjectType, int iNumObjType, bool bLoop)
{
	SaveFilePosition();		  // 파일 포인터를 저장한다.
	while (!feof(m_pStream))  // 파일의 끝이 아니라면
	{
		GetNextTokenString(); // 토큰 검색
		for (int iType = 0; iType < iNumObjType; iType++)
		{
			if (!_tcsicmp(m_pString, pObjectType[iType])) //문자열을 비교해 토큰과 같지 않을경우
			{
				return iType;							  //같지 않은 타입을 리턴
			}
		}
		if (!bLoop) break;
	}
	RestoreFilePosition();								  //파일포인터 복구
	return -1;
}
/*파일 위치 지시자 정보 저장함수*/
void BParser::SaveFilePosition()
{
	if (fgetpos(m_pStream, &m_FilePosition)) // 파일 포인터를 저장하는 함수
	{
		CloseStream(_T("작업도중 파일포인터를 잃어버렸습니다."));
	}
}
/*파일 위치 지시자 정보 복구함수*/
void BParser::RestoreFilePosition()
{
	if (fsetpos(m_pStream, &m_FilePosition)) // 파일 포인터를 불러오는 함수
	{
		CloseStream(_T("작업도중 파일포인터를 잃어버렸습니다."));
	}
}
/*토큰 검색 함수*/
TCHAR* BParser::GetNextTokenString()
{
	_fgetts(m_pBuffer, 256, m_pStream);			//m_pBuffer로 파일을 읽어와서
	_stscanf(m_pBuffer, _T("%s"), m_pString);	//m_pString에 문자열만을 옮긴다.
	return m_pString;							//m_pString (옮겨진 문자열 = 토큰) 을 출력
}
/*라인 검색 함수*/
TCHAR* BParser::GetNextLine()
{
	_fgetts(m_pBuffer, 256, m_pStream);
	return m_pBuffer;
}
/*데이터 정보 저장 함수*/
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
				// 문자열의 시작과 끝에 있는 (")문자를 삭제한다.
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