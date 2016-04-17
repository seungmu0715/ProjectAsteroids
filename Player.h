#pragma once
#include <vector>
#include "Util.h"
#include "Sprite.h"
#include "Path.h"
#include "Loop.h"
class Player
{
private:
	ID2D1HwndRenderTarget*	m_RenderTarget;
	Sprite*					m_Sprite;

	std::vector<Path*>		m_Path;
	std::vector<Loop*>		m_Loop;
	ID2D1SolidColorBrush*	m_Brush;
	int						m_PosX, m_PosY;
	int						m_ReservX, m_ReservY;
	int						m_Duration;
	float					m_Radius;
	bool					m_IsDying;
	bool					m_IsDead;
	bool					m_IsLoopValid;

	int						m_Frame;

public:
	void Initialize(ID2D1HwndRenderTarget* renderTarget, Sprite* sprite);
	void Update(DWORD delta);
	void Render();
	void Shutdown();

	void InvalidateLoop() { m_IsLoopValid = false; };

	int GetX() { return m_PosX; };
	int GetY() { return m_PosY; };
	float GetRadius() { return m_Radius; };
	bool IsDead() { return m_IsDead; };
	bool IsLoopValid() { return m_IsLoopValid; };
	std::vector<Loop*>::iterator GetLoopBegin();
	std::vector<Loop*>::iterator GetLoopEnd();

	void SetPosition(int x, int y);
	void SetDead();
};

