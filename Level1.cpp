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

	// 목숨
	m_Static[0] = new GUIStatic();
	m_Static[0]->Initialize(m_ResourceManager->GetSprite(L"res\\spaceship.png"), IMAGE_SIZE + margin, IMAGE_SIZE / 2, 0.5f, true);

	// 폭탄
	m_Static[1] = new GUIStatic();
	m_Static[1]->Initialize(m_ResourceManager->GetSprite(L"res\\bomb.png"), (m_Rect.right - m_Rect.left) - IMAGE_SIZE * 2, IMAGE_SIZE / 2, 0.5f, false);

	// 목숨
	m_Text[0] = new GUIText();
	m_Text[0]->Initialize(m_RenderTarget, m_TextFormat, IMAGE_SIZE * 2, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	m_Text[0]->SetText(m_Life);

	// 폭탄
	m_Text[1] = new GUIText();
	m_Text[1]->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) - IMAGE_SIZE - margin, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	m_Text[1]->SetText(m_Bomb);

	// 점수
	m_Text[2] = new GUIText();
	m_Text[2]->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) / 2, IMAGE_SIZE / 2, 2, D2D1::ColorF::White);
	m_Text[2]->SetText(m_Score);

	// 일시정지
	m_PauseText = new GUIText();
	m_PauseText->Initialize(m_RenderTarget, m_LargeTextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) / 2, 4, D2D1::ColorF::White, L"일시 정지");

	// 게임 오버
	m_GameOverText = new GUIText();
	m_GameOverText->Initialize(m_RenderTarget, m_LargeTextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) / 2, 4, D2D1::ColorF::White, L"Game Over");

	// 버튼 만들기
	ColorSet newColorSet;
	newColorSet.NormalColor = D2D1::ColorF::White;
	newColorSet.HoverdColor = D2D1::ColorF::DarkCyan;
	newColorSet.PressedColor = D2D1::ColorF::Cyan;
	newColorSet.DisabledColor = D2D1::ColorF::Gray;

	m_Btn = new GUIButton();
	m_Btn->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) / 2, (m_Rect.bottom - m_Rect.top) * 3 / 4, 4, newColorSet, L"메인 메뉴로");
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

	// 소행성 스폰
	SpawnAsteroids();

	// 플레이어 업데이트 및 생사 검사
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
		// 이미 파괴 진행중인 소행성은 건너뛴다
		if ((*itr)->IsDying() && !(*itr)->IsDead())
		{
			itr++;
			continue;
		}

		int x = (*itr)->GetX();
		int y = (*itr)->GetY();

		// 플레이어와 충돌 시 플레이어를 죽인다
		if (Util::GetDistance(playerX, playerY, x, y) <= playerRadius + (*itr)->GetRadius())
		{
			m_Player->SetDead();
		}
		
		// 루프가 존재하는 경우 루프와의 충돌 검사를 추가로 수행한다
		if (validLoop)
		{
			bool flag = false;

			for (m_LoopItr = m_Player->GetLoopBegin(); m_LoopItr != std::prev(m_LoopEnd); m_LoopItr++)
			{
				// 화면 외부의 점 ex) (outer, outer) 으로 부터 소행성 중심점까지 선분을 그어
				// 이 선분이 Loop와 몇 번 만나는지 카운트한다
				// 홀수번 만나면 Loop의 내부, 짝수번 만나면 Loop의 외부에 있는 것으로 판단한다
				const int outer = -1 * IMAGE_SIZE;

				// (Pn_0, Pn_0+1)로 이루어진 선분을 구함
				D2D1_POINT_2F Pn_0 = (*m_LoopItr)->GetPosition();				// n번째 점
				D2D1_POINT_2F Pn_1 = (*std::next(m_LoopItr))->GetPosition();	// n+1번재 점

				// 교점 px, py는
				float denominator = (Pn_0.x - Pn_1.x) * (outer - y) - (Pn_0.y - Pn_1.y) * (outer - x);
				if (denominator == 0)			// 평행 또는 일치 상태
					continue;
				
				int px = (((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - x)) - ((Pn_0.x - Pn_1.x) * (outer * y - outer * x))) / denominator;
				int py = (((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - y)) - ((Pn_0.y - Pn_1.y) * (outer * y - outer * x))) / denominator;
				
				// 교점 좌표가 유효한 점이면 flag를 toggle한다
				int minX = Util::GetMax(Util::GetMin(Pn_0.x, Pn_1.x), Util::GetMin(outer, x));
				int maxX = Util::GetMin(Util::GetMax(Pn_0.x, Pn_1.x), Util::GetMax(outer, x));

				int minY = Util::GetMax(Util::GetMin(Pn_0.y, Pn_1.y), Util::GetMin(outer, y));
				int maxY = Util::GetMin(Util::GetMax(Pn_0.y, Pn_1.y), Util::GetMax(outer, y));

				if ((minX <= px && px <= maxX) && (minY <= py && py <= maxY))
					flag = !flag;
			}

			// 교점의 수가 홀수면 충돌로 판정해 파괴한다
			if (flag) {
				(*itr)->SetDead();
				m_Text[2]->SetText(++m_Score);
			}
		}

		// 업데이트
		(*itr)->Update(delta);

		if ((*itr)->IsDead())
		{
			delete *itr;
			itr = m_Asteroid.erase(itr);
		}
		else
			itr++;
	}

	// Loop 생성 후 처음 호출된 Update에서만 파괴 동작을 수행하고, 그 이후부터는 파괴 검사를 하지 않는다
	// Loop가 줄어드는 속도보다 소행성의 속도가 더 빠른 경우 안쪽으로 빨려들어가 파괴되는 현상을 방지하기 위함
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
			// hover or pressed 상태 모두 해제 후 Hovered 상태인 버튼만 찾아 SetHovered
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

			// 클릭이 무효라면 버튼의 상태 해제
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
		// 폭탄
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

