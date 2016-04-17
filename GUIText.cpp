#include "pch.h"
#include "GUIText.h"


void GUIText::Initialize(ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat * textFormat, int x, int y, int length, D2D1::ColorF::Enum color)
{
	m_RenderTarget = renderTarget;
	m_TextFormat = textFormat;
	m_PosX = x;
	m_PosY = y;
	m_Length = length;
	m_Text = L"";

	m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &m_Brush);
}

void GUIText::Render()
{
	m_RenderTarget->DrawTextA(m_Text, wcslen(m_Text) + 1, m_TextFormat, D2D1::RectF(m_PosX - m_Length * IMAGE_SIZE / 2, m_PosY - IMAGE_SIZE / 2, m_PosX + m_Length * IMAGE_SIZE / 2, m_PosY + IMAGE_SIZE / 2), m_Brush);
}

void GUIText::SetText(int value)
{
	swprintf_s(m_Buffer, L"%d\0", value);
	m_Text = m_Buffer;
}

void GUIText::SetText(wchar_t * text)
{
	m_Text = text;
}
