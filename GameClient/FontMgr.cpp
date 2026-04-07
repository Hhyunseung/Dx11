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

        // 이미 등록되어 있어서 false가 나올 수도 있으니 assert는 빼는 게 안전
        // assert(m_FontRegistered);
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
        // 쿠키런 실패 시 확인용 fallback
        hr = m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper);
        assert(SUCCEEDED(hr));
    }
}

void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
    if (m_FontWrapper == nullptr)
        return;

    m_FontWrapper->DrawString(
        CONTEXT,
        _pStr,
        _fFontSize,
        _fPosX,
        _fPosY,
        _Color,
        FW1_RESTORESTATE
    );
}