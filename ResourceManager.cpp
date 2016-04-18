#include "pch.h"
#include "ResourceManager.h"


bool ResourceManager::LoadDirectory(ID2D1HwndRenderTarget* renderTarget, wchar_t * directoryName)
{
	intptr_t handle;
	int iResult = 0;
	bool bResult;
	_wfinddata_t fd;
	wchar_t buffer[128];

	wcscpy_s(buffer, directoryName);
	wcscat_s(buffer, L"\\*.*");

	handle = _wfindfirst(buffer, &fd);
	if (handle == -1)
		return false;

	while (!wcscmp(fd.name, L".") || !wcscmp(fd.name, L".."))
	{
		_wfindnext(handle, &fd);
	}

	while (iResult != -1)
	{
		wcscpy_s(buffer, directoryName);
		wcscat_s(buffer, L"\\");
		wcscat_s(buffer, fd.name);

		bResult = LoadFile(renderTarget, buffer);
		if (!bResult)
			return false;

		iResult = _wfindnext(handle, &fd);
	}

	_findclose(handle);

	return true;
}

bool ResourceManager::LoadFile(ID2D1HwndRenderTarget* renderTarget, wchar_t * filename)
{
	HRESULT result;
	IWICImagingFactory2* wicFactory;
	IWICBitmapDecoder* wicDecoder;
	IWICBitmapFrameDecode* wicFrame;
	IWICBitmapFlipRotator* wicFlip;
	IWICFormatConverter *wicConverter;
	Sprite*	newSprite;

	// WIC의 각종 인터페이스를 사용하기 위한 factory 생성
	result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if (FAILED(result))
		return false;

	// 파일을 읽고 디코딩 하기 위한 decoder 생성
	result = wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &wicDecoder);
	if (FAILED(result))
		return false;

	// decoder에서 프레임을 얻어옴, // 일반적인 이미지 파일은 single frame만을 지원하므로 0으로 고정
	result = wicDecoder->GetFrame(0, &wicFrame);
	if (FAILED(result))
		return false;

	// 수평으로 뒤집힌 이미지를 얻기 위해 BitmapFlipRotator 생성
	result = wicFactory->CreateBitmapFlipRotator(&wicFlip);
	if (FAILED(result))
		return false;

	// wicFrame를 수평으로 뒤집음
	wicFlip->Initialize(wicFrame, WICBitmapTransformFlipHorizontal);

	// WICBitmap을 D2DBitmap으로 변환시키기 위해 format converter 생성
	result = wicFactory->CreateFormatConverter(&wicConverter);
	if (FAILED(result))
		return false;

	// Converter[0]의 Format을 일반 이미지(wicFrame)에 맞춤
	result = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(result))
		return false;

	// 리소스 정보를 저장 할 Sprite 생성
	newSprite = new Sprite(renderTarget);
	if (!newSprite)
		return false;

	// WICBitmap을 D2DBitmap으로 변환
	//result = renderTarget->CreateBitmapFromWicBitmap(wicConverter, nullptr, &m_Bitmap);
	result = renderTarget->CreateBitmapFromWicBitmap(wicConverter, nullptr, newSprite->GetBitmap());
	if (FAILED(result))
		return false;

	ID2D1Bitmap* bitmap = *(newSprite->GetBitmap());
	int numberOfFrame = bitmap->GetSize().width / IMAGE_SIZE;
	int numberOfAction = bitmap->GetSize().height / IMAGE_SIZE;
	newSprite->Initialize(numberOfFrame, numberOfAction);

	wchar_t* buffer = new wchar_t[128];
	wcscpy_s(buffer, wcslen(filename) + 1, filename);

	// 스프라이트 등록
	m_Sprites.push_back(newSprite);
	m_Filenames.push_back(buffer);

	wicConverter->Release();
	wicConverter = nullptr;

	wicFrame->Release();
	wicFrame = nullptr;

	wicFlip->Release();
	wicFlip = nullptr;

	wicDecoder->Release();
	wicDecoder = nullptr;

	wicFactory->Release();
	wicFactory = nullptr;

	return true;
}

Sprite * ResourceManager::GetSprite(wchar_t * filename)
{
	std::vector<wchar_t*>::iterator itr;
	int index = 0;

	for (itr = m_Filenames.begin(); itr != m_Filenames.end(); ++itr)
	{
		if (!wcscmp(*itr, filename))
			break;

		index++;
	}

	if (index >= m_Filenames.size())
		return nullptr;
	else
		return m_Sprites.at(index);
}

void ResourceManager::Shutdown()
{
	for (std::vector<wchar_t*>::iterator itr = m_Filenames.begin(); itr != m_Filenames.end();)
	{
		delete *itr;
		itr = m_Filenames.erase(itr);
	}
	m_Filenames.clear();

	for (std::vector<Sprite*>::iterator itr = m_Sprites.begin(); itr != m_Sprites.end();)
	{
		(*itr)->Shutdown();
		delete *itr;
		itr = m_Sprites.erase(itr);
	}
	m_Sprites.clear();
}
