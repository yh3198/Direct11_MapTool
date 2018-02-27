#pragma once
#include "BStd.h"

//====================================================================================//
//BAseObj Ŭ�������� ���Ǵ� ����Ŭ���� �Դϴ�.
//��ūȭ �� ������ �Ľ��ϱ� ���� �Լ����� �ֽ��ϴ�.
//====================================================================================//


#pragma region �Ľ� ���� ����ü
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
	FILE*		m_pStream;		//ASE���� ������
	fpos_t		m_FilePosition; //���� ��ġ ������
	TCHAR m_pBuffer[256];		//�ӽ� ������ ����
	TCHAR m_pString[256];		//�ӽ� ��Ʈ�� ����
	int m_iData;				//�ӽ� Int�� ����

public:
	/*���� ���� �Լ�*/
	bool OpenStream(const TCHAR* pFileName);									 
	/*���� ���� �Լ�*/
	bool CloseStream(TCHAR* pMsg = NULL);

	/*���ڿ� �迭(pObjectType) �˻� �Լ�*/
	int GetSearchStringArray(TCHAR** pObjectType, int iNumObjType, bool bLoop);	  
	/*���� ��ġ ������ ���� �����Լ�*/
	void SaveFilePosition();
	/*���� ��ġ ������ ���� �����Լ�*/
	void RestoreFilePosition();
	/*��ū �˻� �Լ�*/
	TCHAR* GetNextTokenString();
	/*���� �˻� �Լ�*/
	TCHAR* GetNextLine();
	/*������ ���� ���� �Լ�*/
	bool GetData(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	bool GetData(VOID* pData, INT DataType);

public:
	BParser();
	~BParser();
};

