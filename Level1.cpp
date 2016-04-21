#include "pch.h"
#include "Level1.h"


bool Level1::InitializeGUI()
{
	HRESULT result;
	IDWriteFactory2* writeFactory;
	const WCHAR msc_FontName[] = L"Verdana";
	const FLOAT msc_FontSize = 20.0f;
	const int margin = IMAGE_SIZE / 4;

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
	m_Text[0]->SetText(m_ShieldRecharge);

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

void Level1::SpawnAsteroids(bool isObstacle)
{
	int numberOfCurrentAsteroids = m_Asteroid.size();
	
	if (m_NumberOfAsteroid > m_Asteroid.size() || !isObstacle)
	{
		Asteroid* newAsteroid = new Asteroid();
		float size;
		int type;

		wchar_t buffer[64];
		wmemset(buffer, 0, 64);
		if (isObstacle)
		{
			size = (float)(3 + rand() % 2) / 10.0f;
			type = -1;
			swprintf_s(buffer, L"res\\asteroid%d.png", 1 + rand() % 2);
		}
		else
		{
			size = 0.5f;
			type = rand() % 3;
			swprintf_s(buffer, L"res\\item%d.png", 1 + type);
		}

		int fromX, fromY;
		if (rand() % 2 == 0)
		{
			fromX = (rand() % 2) * (m_Rect.right - m_Rect.left);
			if (fromX == 0)
				fromX -= IMAGE_SIZE / 2;
			else
				fromX += IMAGE_SIZE / 2;
			fromY = rand() % (m_Rect.bottom - m_Rect.top);
		}
		else
		{
			fromX = rand() % (m_Rect.right - m_Rect.left);
			fromY = (rand() % 2) * (m_Rect.bottom - m_Rect.top);
			if (fromY == 0)
				fromY -= IMAGE_SIZE / 2;
			else
				fromY += IMAGE_SIZE / 2;
		}
			
		int toX = (m_Rect.right - m_Rect.left) / 2;
		int toY = (m_Rect.bottom - m_Rect.top) / 2;
		
		newAsteroid->Initialize(m_ResourceManager->GetSprite(buffer), type, fromX, fromY, toX, toY, size);
		m_Asteroid.push_back(newAsteroid);
	}
}

Level1::Level1()
{
	m_RenderTarget = nullptr;
	m_ResourceManager = nullptr;

	m_Player = nullptr;
	m_NumberOfAsteroid = 15;
	m_Difficulty = 0;
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
	SpawnAsteroids(false);

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

	m_NumberOfAsteroid = 15;
	m_Difficulty = 0;
	m_Score = 0;
	m_GameOver = false;
	m_Pause = false;
}

void Level1::Update(DWORD delta)
{
	bool doOnce = false;

	if (m_Pause)
		return;

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
	
	int destroyedAsteorid = 0;		// 한 번에 파괴한 소행성의 개수
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
		int type = (*itr)->GetType();

		// 플레이어와 충돌 시 플레이어에게 데미지를 입힌다
		// 아이템인 경우 충돌하지 않는다
		if (type == -1)
		{
			if (Util::GetDistance(playerX, playerY, x, y) <= playerRadius + (*itr)->GetRadius())
			{
				m_Player->SetDamage();
			}
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
				
				int px = (int)((((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - x)) - ((Pn_0.x - Pn_1.x) * (outer * y - outer * x))) / denominator);
				int py = (int)((((Pn_0.x * Pn_1.y - Pn_0.y * Pn_1.x) * (outer - y)) - ((Pn_0.y - Pn_1.y) * (outer * y - outer * x))) / denominator);
				
				// 교점 좌표가 유효한 점이면 flag를 toggle한다
				int minX = (int)Util::GetMax(Util::GetMin(Pn_0.x, Pn_1.x), Util::GetMin((float)outer, (float)x));
				int maxX = (int)Util::GetMin(Util::GetMax(Pn_0.x, Pn_1.x), Util::GetMax((float)outer, (float)x));

				int minY = (int)Util::GetMax(Util::GetMin(Pn_0.y, Pn_1.y), Util::GetMin((float)outer, (float)y));
				int maxY = (int)Util::GetMin(Util::GetMax(Pn_0.y, Pn_1.y), Util::GetMax((float)outer, (float)y));

				if ((minX <= px && px <= maxX) && (minY <= py && py <= maxY))
					flag = !flag;
			}

			// 교점의 수가 홀수면 충돌로 판정해 파괴한다
			if (flag) {
				(*itr)->SetDead();
				destroyedAsteorid++;

				// 아이템에 대한 특수 효과들
				if (type != -1)
				{
					switch (type)
					{
					case 0:								// 폭탄 개수 증가
						m_Player->IncreaseBomb();
						break;
					case 1:								// 보호막 즉시 충전
						m_Player->RechargeShield();
						break;
					case 2:
						m_Player->ActivateWarpDrive();	// 일정 시간 동안 꼬리 길이 증가
						break;
					}
				}
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

	// 파괴한 소행성 개수 2개당 1만큼 최대 소행성 개수를 증가시킨다
	// 파괴한 소행성을 점수로 변환한다. 1개당 총 먹은개수만큼의 점수 부여함. ex) 3개 먹으면 3점씩 3개
	if (destroyedAsteorid > 0)
	{
		if (m_NumberOfAsteroid < 100)
			m_NumberOfAsteroid += destroyedAsteorid / 2;

		m_Score += (int)pow(destroyedAsteorid, 2);
		m_Text[2]->SetText(m_Score);
	}

	// Loop 생성 후 처음 호출된 Update에서만 파괴 동작을 수행하고, 그 이후부터는 파괴 검사를 하지 않는다
	// Loop가 줄어드는 속도보다 소행성의 속도가 더 빠른 경우 안쪽으로 빨려들어가 파괴되는 현상을 방지하기 위함
	if (validLoop)
		m_Player->InvalidateLoop();

	// GUI 업데이트
	if (!m_Player->IsDead())
	{
		m_ShieldRecharge = 100 - m_Player->GetShieldRechargeTimer();
		if (m_ShieldRecharge > 100)
			m_ShieldRecharge = 100;
		m_Bomb = m_Player->GetBomb();
		m_Text[0]->SetText(m_ShieldRecharge);
		m_Text[1]->SetText(m_Bomb);
	}
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
		if (m_Player->GetBomb() > 0)
		{
			m_Player->UseBomb();
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

void Level1::OnTimer()
{
	// 5초에 한번씩 소행성 최대 개수 1 증가
	if (m_NumberOfAsteroid < 100)
	{
		if (m_Difficulty++ > 50)
		{
			m_Difficulty = 0;
			m_NumberOfAsteroid++;
		}
	}

	// 10초에 한번씩 아이템 스폰
	if (m_ItemGenerateTimer++ > 100)
	{
		m_ItemGenerateTimer = 0;
		SpawnAsteroids(false);
	}

	// 소행성 스폰
	SpawnAsteroids(true);

	m_Player->OnTimer();
	if (m_Player->GetWarpDriveTimer() > 0)
	{
		for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end(); ++itr)
		{
			(*itr)->ActivateWarpDrive();
		}
	}
	else if (m_Player->GetWarpDriveTimer() == 0)
	{
		for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end(); ++itr)
		{
			(*itr)->DeactivateWarpDrive();
		}
	}
	else
		;// do nothing
}

