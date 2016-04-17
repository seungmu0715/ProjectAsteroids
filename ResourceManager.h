#pragma once
#include <d2d1_2.h>
#include <vector>
#include <io.h>
#include "Sprite.h"
class ResourceManager
{
private:
	std::vector<Sprite*>	m_Sprites;
	std::vector<wchar_t*>	m_Filenames;

public:
	void Shutdown();
	bool LoadDirectory(ID2D1HwndRenderTarget* renderTarget, wchar_t* directoryName);
	bool LoadFile(ID2D1HwndRenderTarget* renderTarget, wchar_t* filename);
	Sprite* GetSprite(wchar_t* filename);
};

