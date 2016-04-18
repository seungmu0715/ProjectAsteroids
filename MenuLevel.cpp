#include "pch.h"
#include "MenuLevel.h"


bool MenuLevel::InitializeGUI()
{
	HRESULT result;
	IDWriteFactory2* writeFactory;
	const WCHAR msc_FontName[] = L"Verdana";
	const FLOAT msc_FontSize = 20.0f;
	const float margin = IMAGE_SIZE / 4;

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), reinterpret_cast<IUnknown **>(&writeFactory));
	if (FAILED(result))
		return false;

	result = writeFactory->CreateTextFormat(msc_FontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, msc_FontSize, L"", &m_TextFormat);
	if (FAILED(result))
		return false;

	m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	writeFactory->Release();
	writeFactory = nullptr;

	// 버튼 만들기
	ColorSet newColorSet;
	newColorSet.NormalColor = D2D1::ColorF::White;
	newColorSet.HoverdColor = D2D1::ColorF::DarkCyan;
	newColorSet.PressedColor = D2D1::ColorF::Cyan;
	newColorSet.DisabledColor = D2D1::ColorF::Gray;

	int x = m_Rect.right - m_Rect.left - IMAGE_SIZE * 2;
	int y = m_Rect.bottom - m_Rect.top - IMAGE_SIZE * 2;

	m_Btn[0] = new GUIButton();
	m_Btn[0]->Initialize(m_RenderTarget, m_TextFormat, x, y, 2, newColorSet, L"S T A R T");

	x = m_Rect.right - m_Rect.left - IMAGE_SIZE * 2;
	y = m_Rect.bottom - m_Rect.top - IMAGE_SIZE;

	m_Btn[1] = new GUIButton();
	m_Btn[1]->Initialize(m_RenderTarget, m_TextFormat, x, y, 2, newColorSet, L"E X I T");

	m_GUIInitialized = true;

	return m_GUIInitialized;
}

MenuLevel::MenuLevel()
{
	m_RenderTarget = nullptr;
	m_ResourceManager = nullptr;

	m_GUIInitialized = false;
}

bool MenuLevel::Load(HWND hWnd, ID2D1HwndRenderTarget * renderTarget, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel, ExitGame exitGame)
{
	bool result;

	m_hWnd = hWnd;
	m_RenderTarget = renderTarget;

	GetClientRect(m_hWnd, &m_Rect);

	m_CallBackMenuLevel = std::move(loadMenuLevel);
	m_CallBackNextLevel = std::move(loadNextLevel);
	m_CallBackExitGame = std::move(exitGame);

	m_ResourceManager = new ResourceManager();
	if (!m_ResourceManager)
		return false;

	/*result = m_ResourceManager->LoadDirectory(renderTarget, L"res");
	if (!result)
		return false;*/

	result = InitializeGUI();
	if (!result)
		return false;

	return true;
}

void MenuLevel::UnLoad()
{
	for (int i = 0; i < ARRAYSIZE(m_Btn); i++)
	{
		m_Btn[i]->Shutdown();
		delete m_Btn[i];
		m_Btn[i] = nullptr;
	}

	if (m_ResourceManager)
	{
		m_ResourceManager->Shutdown();
		delete m_ResourceManager;
		m_ResourceManager = nullptr;
	}
}

void MenuLevel::Update(DWORD delta)
{
}

void MenuLevel::Render()
{
	for (int i = 0; i < ARRAYSIZE(m_Btn); i++)
		m_Btn[i]->Render();
}

void MenuLevel::OnMouseMessage(UINT message, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		m_MouseX = GET_X_LPARAM(lParam);
		m_MouseY = GET_Y_LPARAM(lParam);

		if (m_GUIInitialized)
		{
			for (int i = 0; i < ARRAYSIZE(m_Btn); i++)
			{
				// hover or pressed 상태 모두 해제 후 Hovered 상태인 버튼만 찾아 SetHovered
				if (!m_Btn[i]->IsPressed() && !m_Btn[i]->IsDisabled())
				{
					m_Btn[i]->SetNormal();
				}

				if (m_Btn[i]->IsIn(m_MouseX, m_MouseY))
				{
					if (!m_Btn[i]->IsPressed() && !m_Btn[i]->IsDisabled())
					{
						m_Btn[i]->SetHovered();
					}
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (m_GUIInitialized)
		{
			for (int i = 0; i < ARRAYSIZE(m_Btn); i++)
			{
				if (m_Btn[i]->IsIn(m_MouseX, m_MouseY))
				{
					if (!m_Btn[i]->IsDisabled())
					{
						m_Btn[i]->SetPressed();
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
	{
		int buttonPressed = -1;

		if (m_GUIInitialized)
		{
			// 버튼을 누를 때와 뗄 때 모두 버튼 영역 안에 있어야 유효
			for (int i = 0; i < ARRAYSIZE(m_Btn); i++)
			{
				if (m_Btn[i]->IsIn(m_MouseX, m_MouseY))
				{
					if (m_Btn[i]->IsPressed() && !m_Btn[i]->IsDisabled())
					{
						buttonPressed = i;
					}
				}
			
				// 마우스 버튼이 올라갔으므로 클릭 유효 여부와 관계없이 모든 버튼의 상태 해제
				if (!m_Btn[i]->IsDisabled())
				{
					m_Btn[i]->SetNormal();
				}
			}

			// 유효한 버튼 클릭이 있었을 경우 해당하는 동작 수행
			switch (buttonPressed)
			{
			case 0:
				m_CallBackNextLevel();
				break;
			case 1:
				m_CallBackExitGame();
				break;
			}
		}
		break;
	}
	}
}
