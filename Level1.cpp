#include "pch.h"
#include "Level1.h"


bool Level1::InitializeGUI()
{
	HRESULT result;
	IDWriteFactory2* writeFactory;
	static const WCHAR msc_FontName[] = L"Verdana";
	static const FLOAT msc_FontSize = 20.0f;
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

	GUIStatic* newStatic = new GUIStatic();
	newStatic->Initialize(m_ResourceManager->GetSprite(L"res\\spaceship.png"), IMAGE_SIZE + margin, IMAGE_SIZE / 2, 0.5f, true);
	m_Static.push_back(newStatic);

	newStatic = new GUIStatic();
	newStatic->Initialize(m_ResourceManager->GetSprite(L"res\\bomb.png"), (m_Rect.right - m_Rect.left) - IMAGE_SIZE * 2, IMAGE_SIZE / 2, 0.5f, false);
	m_Static.push_back(newStatic);

	GUIText* newText = new GUIText();
	newText->Initialize(m_RenderTarget, m_TextFormat, IMAGE_SIZE * 2, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	newText->SetText(m_Life);
	m_Text.push_back(newText);

	newText = new GUIText();
	newText->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) - IMAGE_SIZE - margin, IMAGE_SIZE / 2, 1, D2D1::ColorF::White);
	newText->SetText(m_Bomb);
	m_Text.push_back(newText);

	newText = new GUIText();
	newText->Initialize(m_RenderTarget, m_TextFormat, (m_Rect.right - m_Rect.left) / 2, IMAGE_SIZE / 2, 2, D2D1::ColorF::White);
	newText->SetText(m_Score);
	m_Text.push_back(newText);

	return true;
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
	m_GameOver = false;
	m_Frame = 0;
	m_Life = 2;
	m_Bomb = 5;
	m_Score = 0;
}

bool Level1::Load(ID2D1HwndRenderTarget * renderTarget, RECT rect, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel)
{
	bool result;

	m_RenderTarget = renderTarget;
	m_Rect = rect;

	m_CallBackMenuLevel = std::move(loadMenuLevel);
	m_CallBackNextLevel = std::move(loadNextLevel);

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

	return true;
}

void Level1::UnLoad()
{
	for (std::list<Asteroid*>::iterator itr = m_Asteroid.begin(); itr != m_Asteroid.end();)
	{
		delete *itr;
		itr = m_Asteroid.erase(itr);
	}

	if (m_Player)
	{
		m_Player->Shutdown();
		delete m_Player;
		m_Player = nullptr;
	}

	for (std::vector<GUIText*>::iterator itr = m_Text.begin(); itr != m_Text.end();)
	{
		delete *itr;
		itr = m_Text.erase(itr);
	}

	for (std::vector<GUIStatic*>::iterator itr = m_Static.begin(); itr != m_Static.end();)
	{
		delete *itr;
		itr = m_Static.erase(itr);
	}

	if (m_ResourceManager)
	{
		m_ResourceManager->Shutdown();
		delete m_ResourceManager;
		m_ResourceManager = nullptr;
	}
}

void Level1::Update(DWORD delta)
{
	m_Frame += delta;

	// ���༺ ����
	SpawnAsteroids();

	// �÷��̾� ������Ʈ �� ���� �˻�
	m_Player->Update(delta);
	if (m_Player->IsDead())
	{
		if (!m_GameOver)
			MessageBeep(0);

		m_GameOver = true;
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
			itr = m_Asteroid.erase(itr);
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

	for (std::vector<GUIText*>::iterator itr = m_Text.begin(); itr != m_Text.end(); itr++)
	{
		(*itr)->Render();
	}

	for (std::vector<GUIStatic*>::iterator itr = m_Static.begin(); itr != m_Static.end(); itr++)
	{
		(*itr)->Render();
	}
}

void Level1::OnMouseMessage(UINT message, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		int mousePosX = GET_X_LPARAM(lParam);
		int mousePosY = GET_Y_LPARAM(lParam);

		m_Player->SetPosition(mousePosX, mousePosY);
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
		if (m_CallBackMenuLevel)
			m_CallBackMenuLevel();
	}
}

