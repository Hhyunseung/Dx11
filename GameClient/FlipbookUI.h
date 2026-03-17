#pragma once
#include "AssetUI.h"

#include "AFlipbook.h"

class FlipbookUI :
    public AssetUI
{
private:
    char  m_NameBuffer[256];   // 이름 편집 버퍼
    AFlipbook* m_PrevFlipbook;  // 이전 Flipbook 추적

    float m_AccTime;
    int   m_CurFrame;
    float m_FPS;
    bool  m_Play;

public:
    virtual void Tick_UI() override;
    void Preview(Ptr<AFlipbook> pFlipbook);
    void SpriteList(Ptr<AFlipbook> pFlipbook);  // Sprite 목록 편집

public:
    FlipbookUI();
    virtual ~FlipbookUI();
};

