#pragma once
#include <map>
#include "Direct2DManager.h"
#include "MenuLevel.h"
#include "Level1.h"

#define NUMBER_OF_LEVELS 2

class GameController
{
private:
	bool CreateLevels();

	HWND				m_hWnd;
	Direct2DManager*	m_Direct2DManager;
	GameLevel*			m_Levels[NUMBER_OF_LEVELS];
	bool				m_IsLevelLoaded[NUMBER_OF_LEVELS];
	RECT				m_Rect;

	bool				m_Loading;
	int					m_CurrentLevel;
	int					m_MouseX, m_MouseY;

public:
	GameController();

	bool Initialize(HWND hWnd);
	void Update(DWORD Delta) { m_Levels[m_CurrentLevel]->Update(Delta); };
	void Render();
	void Shutdown();

	bool LoadMenuLevel();
	bool LoadNextLevel();
	void ExitGame();

	void OnMouseMessage(HWND hWnd, UINT message, LPARAM lParam);
	void OnKeyboardMessage(HWND hWnd, WPARAM wParam);
	void OnTimer();
};

