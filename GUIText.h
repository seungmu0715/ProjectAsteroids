#pragma once
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <cstdio>
#pragma comment (lib, "dwrite.lib")
class GUIText
{
private:
	ID2D1HwndRenderTarget*	m_RenderTarget;
	IDWriteTextFormat*		m_TextFormat;

	ID2D1SolidColorBrush*	m_Brush;
	int						m_PosX, m_PosY;
	int						m_Length;
	wchar_t					m_Buffer[8];
	wchar_t*				m_Text;

public:
	void Initialize(ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat* textFormat, int x, int y, int length, D2D1::ColorF::Enum color, wchar_t* text);
	void Initialize(ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat* textFormat, int x, int y, int length, D2D1::ColorF::Enum color);
	void Render();

	RECT GetRect();

	void SetText(int value);
	void SetText(wchar_t* text);
	void SetColor(D2D1::ColorF::Enum color);
};

