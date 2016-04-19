#pragma once
#include <time.h>
#include <list>
#include "Util.h"
#include "GameLevel.h"
#include "GUIStatic.h"
#include "Player.h"
#include "Asteroid.h"
class Level1 : public GameLevel
{
private:
	bool InitializeGUI();
	void ShutdownGUI();
	void SpawnAsteroids();
	
	IDWriteTextFormat*			m_LargeTextFormat;

	Player*						m_Player;
	std::list<Asteroid*>		m_Asteroid;
	std::vector<Loop*>::iterator	m_LoopItr, m_LoopEnd;

	GUIStatic*					m_Static[2];
	GUIText*					m_Text[3];
	GUIText*					m_PauseText;
	GUIText*					m_GameOverText;
	GUIButton*					m_Btn;

	int							m_ShieldRecharge, m_Bomb, m_Score;

	int							m_NumberOfAsteroid;
	int							m_Difficulty;
	int							m_MouseX, m_MouseY;

	POINT						m_CursorPos;
	bool						m_GUIInitialized;
	bool						m_Pause;
	bool						m_GameOver;

public:
	Level1();

	virtual bool Load(HWND hWnd, ID2D1HwndRenderTarget * renderTarget, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel, ExitGame exitGame) override;
	virtual void UnLoad() override;
	virtual void Update(DWORD delta) override;
	virtual void Render() override;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) override;
	virtual void OnKeyboardMessage(WPARAM wParam) override;
	virtual void OnTimer() override;
};

