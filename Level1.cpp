#include "pch.h"
#include "Level1.h"


bool Level1::InitializeGUI()
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

	result = writeFactory->CreateTextFormat(msc_FontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, msc_FontSize * 2.0f, L"", &m_LargeTextFormat);
	if (FAILED(result))
		return false;

	m_LargeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_LargeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	writeFactory->Release();
	writeFactory = nullptr;

	// ���
	m_Static[0] = new GUIStatic();
	m_Static[0]->Initialize(m_ResourceManager->GetSprite(L"res\\spaceship.png"), IMAGE_SIZE + margin, IMAGE_SIZE / 2, 0.5f, true);

	// ��ź
	m_Static[1] = new GUIStatic();
	m_Static[1]->Initialize(m_ResourceManager->GetSprite(L"res\\bomb.png"), (m_Rect.right - m_Rect.left) - IMAGE_SIZE * 2, IMAGE_SIZE / 2, 0.5f, false);

	// ���
	m_Text[0] = new GUIText();
	m_Text[0]->Initialize(m_RenderTarget, m_TextFormat, IMAGE_SIZE * 2, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	m_Text[0]->SetText(m_Life);

	// ��ź
	m_Text[1] = new GUIText();
	m_Text[1]->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) - IMAGE_SIZE - margin, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	m_Text[1]->SetText(m_Bomb);

	// ����
	m_Text[2] = new GUIText();
	m_Text[2]->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) / 2, IMAGE_SIZE / 2, 2, D2D1::ColorF::White);
	m_Text[2]->SetText(m_Score);

	// �Ͻ�����
	m_PauseText = new GUIText();
	m_PauseText->Initialize(m_RenderTarget, m_LargeTextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) / 2, 4, D2D1::ColorF::White, L"�Ͻ� ����");

	// ���� ����
	m_GameOverText = new GUIText();
	m_GameOverText->Initialize(m_RenderTarget, m_LargeTextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) / 2, 4, D2D1::ColorF::White, L"Game Over");

	// ��ư �����
	ColorSet newColorSet;
	newColorSet.NormalColor = D2D1::ColorF::White;
	newColorSet.HoverdColor = D2D1::ColorF::DarkCyan;
	newColorSet.PressedColor = D2D1::ColorF::Cyan;
	newColorSet.DisabledColor = D2D1::ColorF::Gray;

	m_Btn = new GUIButton();
	m_Btn->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) * 3 / 4, 4, newColorSet, L"���� �޴���");
	m_Btn->SetDisabled();

	m_GUIInitialized = true;
	return m_GUIInitialized;
}

void Level1::ShutdownGUI()
{
	if (m_Btn)
	{
		m_Btn->Shutdown();
		delete m_Btn;
		m_Btn = nullptr;
	}

	if (m_GameOverText)
	{
		delete m_GameOverText;
		m_GameOverText = nullptr;
	}

	if (m_PauseText)
	{
		delete m_PauseText;
		m_PauseText = nullptr;
	}

	for (int i = 0; i < ARRAYSIZE(m_Text); i++)
	{
		delete m_Text[i];
		m_Text[i] = nullptr;
	}

	for (int i = 0; i < ARRAYSIZE(m_Static); i++)
	{
		delete m_Static[i];
		m_Static[i] = nullptr;
	}
}

void Level1::SpawnAsteroids()
{
	int numberOfCurrentAsteroids = m_Asteroid.size();
	
	for (int i = 0; i < m_NumberOfAsteroid - numberOfCurrentAsteroids; i++)
	{
		Asteroid* newAsteroid = new Asteroid();

		wchar_t buffer[64];
		wmemset(buffer, 0, 64);
		swprintf_s(buffer, L"res\\asteroid%d.png", 1 + rand() % 2);

		int fromX, fromY;
		if (rand() % 2 == 0)
		{
			fromX = (rand() % 2) * (m_Rect.right - m_Rect.left);
			fromY = rand() % (m_Rect.bottom - m_Rect.top);
		}
		else
		{
			fromX = rand() % (m_Rect.right - m_Rect.left);
			fromY = (rand() % 2) * (m_Rect.bottom - m_Rect.top);
		}
			
		int toX = (m_Rect.right - m_Rect.left) / 2;
		int toY = (m_Rect.bottom - m_Rect.top) / 2;
		float size = (float)(3 + rand() % 3) / 10.0f;

		newAsteroid->Initialize(m_ResourceManager->GetSprite(buffer), fromX, fromY, toX, toY, size);
		m_Asteroid.push_back(newAsteroid);
	}
}

Level1::Level1()
{
	m_RenderTarget = nullptr;
	m_ResourceManager = nullptr;

	m_Player = nullptr;
	m_NumberOfAsteroid = 15;
	m_Frame = 0;
	m_Life = 2;
	m_Bomb = 5;
	m_Score = 0;

	m_GUIInitialized = false;
	m_GameOver = false;
	m_Pause = false;
}

bool Level1::Load(HWND hWnd, ID2D1HwndRenderTarget * renderTarget, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel, ExitGame exitGame)
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

	result = m_ResourceManager->LoadDirectory(renderTarget, L"res");
	if (!result)
		return false;

	m_Player = new Player();
	if (!m_Player)
		return false;

	m_Player->Initialize(renderTarget, m_ResourceManager->GetSprite(L"res\\spaceship.png"));

	result = InitializeGUI();
	if (!result)
		return false;

	srand(time(NULL));
	SpawnAsteroids();

	m_CursorPos.x = (m_Rect.right - m_Rect.left) / 2;
	m_CursorPos.y = (m_Rect.bottom - m_Rect.top) / 2;
	ClientToScreen(m_hWnd, &m_CursorPos);
	SetCursorPos(m_CursorPos.x, m_CursorPos.y);

	return true;
}

void Level1::UnLoad()
{
	ShutdownGUI();

	for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end();)
	{
		delete *itr;
		itr = m_Asteroid.erase(itr);
	}
	m_Asteroid.clear();

	if (m_Player)
	{
		m_Player->Shutdown();
		delete m_Player;
		m_Player = nullptr;
	}

	if (m_ResourceManager)
	{
		m_ResourceManager->Shutdown();
		delete m_ResourceManager;
		m_ResourceManager = nullptr;
	}

	m_Life = 2;
	m_Bomb = 5;
	m_Score = 0;
	m_GameOver = false;
	m_Pause = false;
}

void Level1::Update(DWORD delta)
{
	m_Frame += delta;

	if (m_Pause)
		return;

	// ���༺ ����
	SpawnAsteroids();

	// �÷��̾� ������Ʈ �� ���� �˻�
	m_Player->Update(delta);
	if (m_Player->IsDead()) {
		m_GameOver = true;
		m_Btn->SetEnabled();
	}

	int playerX = m_Player->GetX();
	int playerY = m_Player->GetY();
	float playerRadius = m_Player->GetRadius();

	bool validLoop = m_Player->IsLoopValid();

	if (validLoop)
	{
		m_LoopItr = m_Player->GetLoopBegin();
		m_LoopEnd = m_Player->GetLoopEnd();
	}
	
	for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end();)
	{
		// �̹� �ı� �������� ���༺�� �ǳʶڴ�
		if ((*itr)->IsDying() && !(*itr)->IsDead())
		{
			itr++;
			continue;
		}

		int x = (*itr)->GetX();
		int y = (*itr)->GetY();

		// �÷��̾�� �浹 �� �÷��̾ ���δ�
		if (Util::GetDistance(playerX, playerY, x, y) <= playerRadius + (*itr)->GetRadius())
		{
			m_Player->SetDead();
		}
		
		// ������ �����ϴ� ��� �������� �浹 �˻縦 �߰��� �����Ѵ�
		if (validLoop)
		{
			bool flag = false;

			for (m_LoopItr = m_Player->GetLoopBegin(); m_LoopItr != std::prev(m_LoopEnd); m_LoopItr++)
			{
				// ȭ�� �ܺ��� �� ex) (outer, outer) ���� ���� ���༺ �߽������� ������ �׾�
				// �� ������ Loop�� �� �� �������� ī��Ʈ�Ѵ�
				// Ȧ���� ������ Loop�� ����, ¦���� ������ Loop�� �ܺο� �ִ� ������ �Ǵ��Ѵ�
				const int outer = -1 * IMAGE_SIZE;

				// (Pn_0, Pn_0+1)�� �̷���� ������ ����
				D2D1_POINT_2F Pn_0 = (*m_LoopItr)->GetPosition();				// n��° ��
				D2D1_POINT_2F Pn_1 = (*std::next(m_LoopItr))->GetPosition();	// n+1���� ��

				// ���� px, py��
				float denominator = (Pn_0.x - Pn_1.x) * (outer - y) - (Pn_0.y - Pn_1.y) * (outer - x);
				if (denominator == 0)			// ���� �Ǵ� ��ġ ����
					continue;
				
				int px = (((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - x)) - ((Pn_0.x - Pn_1.x) * (outer * y - outer * x))) / denominator;
				int py = (((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - y)) - ((Pn_0.y - Pn_1.y) * (outer * y - outer * x))) / denominator;
				
				// ���� ��ǥ�� ��ȿ�� ���̸� flag�� toggle�Ѵ�
				int minX = Util::GetMax(Util::GetMin(Pn_0.x, Pn_1.x), Util::GetMin(outer, x));
				int maxX = Util::GetMin(Util::GetMax(Pn_0.x, Pn_1.x), Util::GetMax(outer, x));

				int minY = Util::GetMax(Util::GetMin(Pn_0.y, Pn_1.y), Util::GetMin(outer, y));
				int maxY = Util::GetMin(Util::GetMax(Pn_0.y, Pn_1.y), Util::GetMax(outer, y));

				if ((minX <= px && px <= maxX) && (minY <= py && py <= maxY))
					flag = !flag;
			}

			// ������ ���� Ȧ���� �浹�� ������ �ı��Ѵ�
			if (flag) {
				(*itr)->SetDead();
				m_Text[2]->SetText(++m_Score);
			}
		}

		// ������Ʈ
		(*itr)->Update(delta);

		if ((*itr)->IsDead())
		{
			delete *itr;
			itr = m_Asteroid.erase(itr);
		}
		else
			itr++;
	}

	// Loop ���� �� ó�� ȣ��� Update������ �ı� ������ �����ϰ�, �� ���ĺ��ʹ� �ı� �˻縦 ���� �ʴ´�
	// Loop�� �پ��� �ӵ����� ���༺�� �ӵ��� �� ���� ��� �������� ������ �ı��Ǵ� ������ �����ϱ� ����
	if (validLoop)
		m_Player->InvalidateLoop();
}

void Level1::Render()
{
	for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end(); ++itr)
	{
		(*itr)->Render();
	}

	m_Player->Render();

	if (m_GUIInitialized)
	{
		for (int i = 0; i < ARRAYSIZE(m_Text); i++)
		{
			m_Text[i]->Render();
		}

		for (int i = 0; i < ARRAYSIZE(m_Static); i++)
		{
			m_Static[i]->Render();
		}

		if (m_Pause)
			m_PauseText->Render();
		if (m_GameOver)
			m_GameOverText->Render();
		if (!m_Btn->IsDisabled())
			m_Btn->Render();
	}
}

void Level1::OnMouseMessage(UINT message, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		m_MouseX = GET_X_LPARAM(lParam);
		m_MouseY = GET_Y_LPARAM(lParam);

		m_Player->SetPosition(m_MouseX, m_MouseY);

		if (m_GUIInitialized)
		{
			// hover or pressed ���� ��� ���� �� Hovered ������ ��ư�� ã�� SetHovered
			if (!m_Btn->IsPressed() && !m_Btn->IsDisabled())
			{
				m_Btn->SetNormal();
			}

			if (m_Btn->IsIn(m_MouseX, m_MouseY))
			{
				if (!m_Btn->IsPressed() && !m_Btn->IsDisabled())
				{
					m_Btn->SetHovered();
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (m_GUIInitialized)
		{
			if (m_Btn->IsIn(m_MouseX, m_MouseY))
			{
				if (!m_Btn->IsDisabled())
				{
					m_Btn->SetPressed();
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if (m_GUIInitialized)
		{
			if (m_Btn->IsIn(m_MouseX, m_MouseY))
			{
				if (m_Btn->IsPressed() && !m_Btn->IsDisabled())
				{
					m_CallBackMenuLevel();
					break;
				}
			}

			// Ŭ���� ��ȿ��� ��ư�� ���� ����
			if (!m_Btn->IsDisabled())
			{
				m_Btn->SetNormal();
			}
		}
		break;
	}
}

void Level1::OnKeyboardMessage(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_SPACE:
		// ��ź
		if (m_Bomb > 0)
		{
			m_Text[1]->SetText(--m_Bomb);
		}
		break;
	case VK_ESCAPE:
		if (m_GameOver)
		{
			if (m_CallBackMenuLevel)
				m_CallBackMenuLevel();
		}
		else
		{
			if (!m_Pause)		// Pause
			{
				GetCursorPos(&m_CursorPos);
				m_Pause = true;
				m_Btn->SetEnabled();
			}
			else				// UnPause
			{
				SetCursorPos(m_CursorPos.x, m_CursorPos.y);
				m_Pause = false;
				m_Btn->SetDisabled();
			}
		}
	}
}

