#pragma once

#include "single.h"
#include <string>
#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib")
#endif

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

class FontMgr : public singleton<FontMgr>
{
    SINGLE(FontMgr);

private:
    IFW1Factory* m_FW1Factory;
    IFW1FontWrapper* m_FontWrapper;
    bool m_FontRegistered;

private:
    struct tFontDrawData
    {
        std::wstring Str;
        float PosX;
        float PosY;
        float FontSize;
        UINT Color;
    };

    std::vector<tFontDrawData> m_vecFontDrawData;

private:
    bool RegisterPrivateFont(const wchar_t* _FontPath);


public:
    void Init();
    void Init(const wchar_t* _FontName);
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
    void DrawFontOutline(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize,
        UINT _TextColor, UINT _OutlineColor, float _Thickness);

    void Render();
    void Clear();
};