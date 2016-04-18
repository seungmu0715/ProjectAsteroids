#include "pch.h"
#include "GUIButton.h"


GUIButton::GUIButton()
{
	m_Text = nullptr;

	m_Pressed = false;
	m_Disabled = false;
}

void GUIButton::Initialize(ID2D1HwndRenderTarget * renderTarget, IDWriteTextFormat * textFormat, int x, int y, int length, ColorSet colorSet, wchar_t* text)
{
	m_ColorSet = colorSet;

	m_Text = new GUIText();
	m_Text->Initialize(renderTarget, textFormat, x, y, length, m_ColorSet.NormalColor, text);
}

void GUIButton::Initialize(GUIText* guiText)
{
	m_Text = guiText;
}

void GUIButton::Render()
{
	m_Text->Render();
}

void GUIButton::Shutdown()
{
	if (m_Text)
	{
		delete m_Text;
		m_Text = nullptr;
	}
}

void GUIButton::SetNormal()
{
	m_Pressed = false;
	m_Disabled = false;
	m_Text->SetColor(m_ColorSet.NormalColor);
}

void GUIButton::SetHovered()
{
	m_Text->SetColor(m_ColorSet.HoverdColor);
}

void GUIButton::SetPressed()
{
	m_Pressed = true;
	m_Text->SetColor(m_ColorSet.PressedColor);
}

void GUIButton::SetEnabled()
{
	m_Disabled = false;
}

void GUIButton::SetDisabled()
{
	m_Pressed = false;
	m_Disabled = true;
	m_Text->SetColor(m_ColorSet.DisabledColor);
}

bool GUIButton::IsIn(int x, int y)
{
	RECT rect = m_Text->GetRect();

	if (rect.left <= x && x <= rect.right)
	{
		if (rect.top <= y && y <= rect.bottom)
		{
			return true;
		}
	}

	return false;
}


