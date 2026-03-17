#pragma once
#include "AssetUI.h"
class SpriteUI :
    public AssetUI
{
private:
	char m_NameBuffer[256]; // 이름 버퍼
	ASprite* m_PrevSprite;  // 이전 Sprite 추적용

public:
    virtual void Tick_UI() override;

public:
    SpriteUI();
    virtual ~SpriteUI();
};

