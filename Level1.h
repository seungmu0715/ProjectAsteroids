#pragma once
#include <time.h>
#include <list>
#include "Util.h"
#include "GameLevel.h"
#include "Player.h"
#include "Asteroid.h"
class Level1 : public GameLevel
{
private:
	bool InitializeGUI();
	void SpawnAsteroids();
	
	Player*						m_Player;
	std::list<Asteroid*>		m_Asteroid;
	std::vector<Loop*>::iterator	m_LoopItr, m_LoopEnd;
	int							m_NumberOfAsteroid;
	bool						m_GameOver;
	int							m_Frame;
	int							m_MousePosX, m_MousePosY;
	int							m_Life, m_Bomb, m_Score;

public:
	Level1();

	virtual bool Load(ID2D1HwndRenderTarget* renderTarget, RECT rect, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel) override;
	virtual void UnLoad() override;
	virtual void Update(DWORD delta) override;
	virtual void Render() override;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) override;
	virtual void OnKeyboardMessage(WPARAM wParam) override;
};

