#pragma once
#include "Util.h"
#include "Sprite.h"
class Asteroid
{
private:
	Sprite*		m_Sprite;

	int			m_PosX, m_PosY;
	float		m_DeltaX, m_DeltaY;
	float		m_Size;
	float		m_Radius;
	bool		m_IsDying;
	bool		m_IsDead;

	int			m_Frame;

public:
	void Initialize(Sprite * sprite, int fromX, int fromY, int toX, int toY, float size);
	void Update(DWORD delta);
	void Render();

	int GetX() { return m_PosX; };
	int GetY() { return m_PosY; };
	float GetRadius() { return m_Radius; };
	bool IsDying() { return m_IsDying; };
	bool IsDead() { return m_IsDead; };

	void SetDead();
};

