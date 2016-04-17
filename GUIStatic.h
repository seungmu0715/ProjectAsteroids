#pragma once
#include "Sprite.h"
class GUIStatic
{
private:
	Sprite*		m_Sprite;

	int			m_PosX, m_PosY;
	float		m_Size;
	bool		m_Animated;
	int			m_Frame;

public:
	void Initialize(Sprite* sprite, int x, int y, float size, bool animated);
	void Render();
};

