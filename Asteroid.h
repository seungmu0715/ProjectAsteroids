#pragma once
#include "Util.h"
#include "Sprite.h"

class Asteroid
{
private:
	Sprite*		m_Sprite;

	int			m_Type;
	int			m_PosX, m_PosY;
	float		m_DeltaX, m_DeltaY;
	float		m_Size;
	float		m_Radius;
	float		m_Speed;
	bool		m_IsDying;
	bool		m_IsDead;

	int			m_Frame;
	float		m_Rotation;

public:
	void Initialize(Sprite * sprite, int type, int fromX, int fromY, int toX, int toY, float size);
	void Update(DWORD delta);
	void Render();

	int GetType() { return m_Type; };
	int GetX() { return m_PosX; };
	int GetY() { return m_PosY; };
	float GetRadius() { return m_Radius; };
	bool IsDying() { return m_IsDying; };
	bool IsDead() { return m_IsDead; };

	void SetDead();
	void ActivateWarpDrive()		{ m_Speed = 0.5f; };
	void DeactivateWarpDrive()		{ m_Speed = 1.0f; };
};

