#pragma once
#include <functional>
#include <d2d1_2.h>
#include "ResourceManager.h"
#include "GUIStatic.h"
#include "GUIText.h"

typedef std::function<void()> LoadMenuLevel;
typedef std::function<void()> LoadNextLevel;

class GameLevel
{
protected:
	ID2D1HwndRenderTarget*	m_RenderTarget;
	IDWriteTextFormat*		m_TextFormat;

	ResourceManager*		m_ResourceManager;
	std::vector<GUIStatic*>	m_Static;
	std::vector<GUIText*>	m_Text;
	
	RECT					m_Rect;
	LoadMenuLevel			m_CallBackMenuLevel;
	LoadNextLevel			m_CallBackNextLevel;

public:
	virtual bool Load(ID2D1HwndRenderTarget* renderTarget, RECT rect, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel) = 0;
	virtual void UnLoad() = 0;
	virtual void Update(DWORD delta) = 0;
	virtual void Render() = 0;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) = 0;
	virtual void OnKeyboardMessage(WPARAM wParam) = 0;
};

