#pragma once
#include "BObject.h"

//====================================================================================//
/*�⺻ �ڽ� Ŭ�����Դϴ�.*/
/*������Ʈ�� ������ �����մϴ�.*/
//====================================================================================//


class BBox : public BObject
{
public:
	/*������ # ���ؽ� ���� �����Լ�*/
	virtual HRESULT CreateVertexBuffer();
	/*������ # �ε��� ���� �����Լ�*/
	virtual HRESULT CreateIndexBuffer();
	/*������ # MinMax �ڽ� �����Լ�*/
	virtual void UpdateVertexMinMax();
public:
	BBox();
	~BBox();
};

