#include "BNode.h"



BNode::BNode()
{
	m_dwDepth = 0;
	m_isLeaf = FALSE;
	m_tBox.vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tBox.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_tBox.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_tBox.fExtent[0] = 1;
	m_tBox.fExtent[1] = 1;
	m_tBox.fExtent[2] = 1;
}


BNode::~BNode()
{
}
