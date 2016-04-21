#pragma once
#include <vector>
#include "Util.h"
#include "Sprite.h"
#include "Path.h"
#include "Loop.h"
class Player
{
private:
#define NUMBER_OF_VERTEX 60

	template <typename T>
	void ClearVector(std::vector<T*>* vector);
	void EmitShockWave();
	void EmitShockWave(D2D1_POINT_2F * destination);

	ID2D1HwndRenderTarget*	m_RenderTarget;
	Sprite*					m_Sprite;

	std::vector<Path*>		m_Path;
	std::vector<Loop*>		m_Loop, m_ShockWave;
	ID2D1SolidColorBrush	*m_TailBrush, *m_ShockWaveBrush;
	int						m_PosX, m_PosY;
	int						m_ReservX, m_ReservY;
	int						m_Duration;
	int						m_PrevDuration;
	float					m_Radius;
	bool					m_IsDead;
	bool					m_IsInvincible;
	bool					m_IsLoopValid;
	int						m_InvincibleTimer;
	int						m_ShieldRechargeTimer;
	int						m_WarpDriveTimer;
	int						m_Bomb;
	float					m_BombExplosionRadius;

	int						m_Frame;
	int						m_State;	// 0=normal; 1=without shield; 2=dying;

public:
	void Initialize(ID2D1HwndRenderTarget* renderTarget, Sprite* sprite);
	void Update(DWORD delta);
	void Render();
	void Shutdown();

	int GetX()										{ return m_PosX; };
	int GetY()										{ return m_PosY; };
	float GetRadius()								{ return m_Radius; };
	bool IsDead()									{ return m_IsDead; };
	bool IsLoopValid()								{ return m_IsLoopValid; };
	std::vector<Loop*>::iterator GetLoopBegin()		{ return m_Loop.begin(); };
	std::vector<Loop*>::iterator GetLoopEnd()		{ return m_Loop.end(); };
	int GetBomb()									{ return m_Bomb; };
	int GetInvinsibleTimer()						{ return m_InvincibleTimer; };
	int GetShieldRechargeTimer()					{ return m_ShieldRechargeTimer; };
	int GetWarpDriveTimer()							{ return m_WarpDriveTimer; };

	void InvalidateLoop()							{ m_IsLoopValid = false; };
	void SetPosition(int x, int y);
	void SetDamage();
	void UseBomb();
	void IncreaseBomb()								{ m_Bomb++; };
	void RechargeShield()							{ m_ShieldRechargeTimer = 1; };
	void ActivateWarpDrive();

	void OnTimer();
};

