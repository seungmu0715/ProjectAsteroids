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

	// WIC�� ���� �������̽��� ����ϱ� ���� factory ����
	result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if (FAILED(result))
		return false;

	// ������ �а� ���ڵ� �ϱ� ���� decoder ����
	result = wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &wicDecoder);
	if (FAILED(result))
		return false;

	// decoder���� �������� ����, // �Ϲ����� �̹��� ������ single frame���� �����ϹǷ� 0���� ����
	result = wicDecoder->GetFrame(0, &wicFrame);
	if (FAILED(result))
		return false;

	// �������� ������ �̹����� ��� ���� BitmapFlipRotator ����
	result = wicFactory->CreateBitmapFlipRotator(&wicFlip);
	if (FAILED(result))
		return false;

	// wicFrame�� �������� ������
	wicFlip->Initialize(wicFrame, WICBitmapTransformFlipHorizontal);

	// WICBitmap�� D2DBitmap���� ��ȯ��Ű�� ���� format converter ����
	result = wicFactory->CreateFormatConverter(&wicConverter);
	if (FAILED(result))
		return false;

	// Converter[0]�� Format�� �Ϲ� �̹���(wicFrame)�� ����
	result = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(result))
		return false;

	// ���ҽ� ������ ���� �� Sprite ����
	newSprite = new Sprite(renderTarget);
	if (!newSprite)
		return false;

	// WICBitmap�� D2DBitmap���� ��ȯ
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

	// ��������Ʈ ���
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
