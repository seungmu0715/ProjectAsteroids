#pragma once
#include "GUIText.h"

struct ColorSet
{
	D2D1::ColorF::Enum NormalColor;
	D2D1::ColorF::Enum HoverdColor;
	D2D1::ColorF::Enum PressedColor;
	D2D1::ColorF::Enum DisabledColor;
};

class GUIButton
{
private:
	GUIText*	m_Text;

	ColorSet	m_ColorSet;
	bool		m_Pressed;
	bool		m_Disabled;

public:
	GUIButton();

	void Initialize(ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat* textFormat, int x, int y, int length, ColorSet colorSet, wchar_t* text);
	void Initialize(GUIText* guiText);
	void Render();
	void Shutdown();

	void SetNormal();
	void SetHovered();
	void SetPressed();
	void SetEnabled();
	void SetDisabled();

	bool IsIn(int x, int y);
	bool IsPressed() { return m_Pressed; };
	bool IsDisabled() { return m_Disabled; };
};

