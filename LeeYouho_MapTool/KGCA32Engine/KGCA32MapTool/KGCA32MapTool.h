
// KGCA32MapTool.h : KGCA32MapTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "Sample.h"

// CKGCA32MapToolApp:
// �� Ŭ������ ������ ���ؼ��� KGCA32MapTool.cpp�� �����Ͻʽÿ�.
//

class CKGCA32MapToolApp : public CWinAppEx
{
public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	Sample Sample;			// ���� Ŭ����

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	
	CKGCA32MapToolApp();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CKGCA32MapToolApp theApp;
