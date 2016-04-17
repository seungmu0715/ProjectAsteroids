#include "pch.h"
#include "GUIStatic.h"


void GUIStatic::Initialize(Sprite * sprite, int x, int y, float size, bool animated)
{
	m_Sprite = sprite;
	m_PosX = x;
	m_PosY = y;
	m_Size = size;
	m_Animated = animated;
	m_Frame = 0;
}

void GUIStatic::Render()
{
	if (m_Animated)
	{
		int frame = (m_Frame++ / 4) % 4;
		m_Sprite->Draw(m_PosX, m_PosY, m_Size, 0, frame);
	}
	else
		m_Sprite->Draw(m_PosX, m_PosY, m_Size, 0, 0);
}
