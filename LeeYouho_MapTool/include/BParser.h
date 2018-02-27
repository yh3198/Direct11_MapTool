#pragma once
#include "BStd.h"

//====================================================================================//
//BAseObj 클래스에서 사용되는 보조클래스 입니다.
//토큰화 된 파일을 파싱하기 위한 함수들이 있습니다.
//====================================================================================//


#pragma region 파싱 관련 구조체
enum SET_DATA_TYPES
{
	NULL_DATA = 0,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VECTOR_DATA,
	POS_DATA,
	TEX_DATA,
	COLOR_DATA,
	ANIPOS_DATA,
	ANIROT_DATA,
	ANISCL_DATA,
};

struct BAnimTrack
{
	int iTick;
	D3DXQUATERNION qRotate;
	D3DXVECTOR3 vVector;
	BAnimTrack* pNext;
	BAnimTrack* pPrev;
};
#pragma endregion

class BParser
{
public:
	FILE*		m_pStream;		//ASE파일 포인터
	fpos_t		m_FilePosition; //파일 위치 지시자
	TCHAR m_pBuffer[256];		//임시 데이터 버퍼
	TCHAR m_pString[256];		//임시 스트링 버퍼
	int m_iData;				//임시 Int형 버퍼

public:
	/*파일 실행 함수*/
	bool OpenStream(const TCHAR* pFileName);									 
	/*파일 종료 함수*/
	bool CloseStream(TCHAR* pMsg = NULL);

	/*문자열 배열(pObjectType) 검색 함수*/
	int GetSearchStringArray(TCHAR** pObjectType, int iNumObjType, bool bLoop);	  
	/*파일 위치 지시자 정보 저장함수*/
	void SaveFilePosition();
	/*파일 위치 지시자 정보 복구함수*/
	void RestoreFilePosition();
	/*토큰 검색 함수*/
	TCHAR* GetNextTokenString();
	/*라인 검색 함수*/
	TCHAR* GetNextLine();
	/*데이터 정보 저장 함수*/
	bool GetData(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	bool GetData(VOID* pData, INT DataType);

public:
	BParser();
	~BParser();
};

