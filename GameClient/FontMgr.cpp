#include "pch.h"
#include "FontMgr.h"
#include "Device.h"
#include "PathMgr.h"

FontMgr::FontMgr()
    : m_FW1Factory(nullptr)
    , m_FontWrapper(nullptr)
    , m_FontRegistered(false)
{
}

FontMgr::~FontMgr()
{
    if (m_FontWrapper)
    {
        m_FontWrapper->Release();
        m_FontWrapper = nullptr;
    }

    if (m_FW1Factory)
    {
        m_FW1Factory->Release();
        m_FW1Factory = nullptr;
    }
}

bool FontMgr::RegisterPrivateFont(const wchar_t* _FontPath)
{
    int ret = AddFontResourceExW(_FontPath, FR_PRIVATE, 0);
    return ret > 0;
}

void FontMgr::Init()
{
    if (m_FW1Factory == nullptr)
    {
        HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_FW1Factory);
        assert(SUCCEEDED(hr));
        if (FAILED(hr))
            return;
    }

    if (!m_FontRegistered)
    {
        const std::wstring fontPath = CONTENT_PATH + L"Font\\CookieRunFont_TTF\\CookieRun_Bold.ttf";
        m_FontRegistered = RegisterPrivateFont(fontPath.c_str());
    }

    Init(L"CookieRun");
}

void FontMgr::Init(const wchar_t* _FontName)
{
    if (m_FontWrapper)
    {
        m_FontWrapper->Release();
        m_FontWrapper = nullptr;
    }

    HRESULT hr = m_FW1Factory->CreateFontWrapper(DEVICE, _FontName, &m_FontWrapper);
    assert(SUCCEEDED(hr));

    if (FAILED(hr))
    {
        hr = m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper);
        assert(SUCCEEDED(hr));
    }
}

void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
    if (_pStr == nullptr)
        return;

    tFontDrawData data = {};
    data.Str = _pStr;
    data.PosX = _fPosX;
    data.PosY = _fPosY;
    data.FontSize = _fFontSize;
    data.Color = _Color;

    m_vecFontDrawData.push_back(data);
}

void FontMgr::Render()
{
    if (m_FontWrapper == nullptr)
        return;

    for (size_t i = 0; i < m_vecFontDrawData.size(); ++i)
    {
        const tFontDrawData& data = m_vecFontDrawData[i];

        m_FontWrapper->DrawString(
            CONTEXT,
            data.Str.c_str(),
            data.FontSize,
            data.PosX,
            data.PosY,
            data.Color,
            FW1_RESTORESTATE
        );
    }

    Clear();
}

void FontMgr::Clear()
{
    m_vecFontDrawData.clear();
}