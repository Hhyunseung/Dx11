#pragma once
#include "AssetUI.h"

#include "AFlipbook.h"

class FlipbookUI :
    public AssetUI
{
private:
    float m_AccTime;
    int   m_CurFrame;
    float m_FPS;
    bool  m_Play;

public:
    virtual void Tick_UI() override;
    void Preview(Ptr<AFlipbook> pFlipbook);

public:
    FlipbookUI();
    virtual ~FlipbookUI();
};

