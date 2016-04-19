#pragma once
#include <functional>
#include <d2d1_2.h>
#include "ResourceManager.h"
#include "GUIButton.h"

typedef std::function<void()> LoadMenuLevel;
typedef std::function<void()> LoadNextLevel;
typedef std::function<void()> ExitGame;

class GameLevel
{
protected:
	HWND					m_hWnd;
	ID2D1HwndRenderTarget*	m_RenderTarget;
	IDWriteTextFormat*		m_TextFormat;

	ResourceManager*		m_ResourceManager;
	
	RECT					m_Rect;
	LoadMenuLevel			m_CallBackMenuLevel;
	LoadNextLevel			m_CallBackNextLevel;
	ExitGame				m_CallBackExitGame;

public:
	virtual bool Load(HWND hWnd, ID2D1HwndRenderTarget * renderTarget, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel, ExitGame exitGame) = 0;
	virtual void UnLoad() = 0;
	virtual void Update(DWORD delta) = 0;
	virtual void Render() = 0;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) = 0;
	virtual void OnKeyboardMessage(WPARAM wParam) = 0;
	virtual void OnTimer() = 0;
};

