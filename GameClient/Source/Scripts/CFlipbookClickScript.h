#pragma once
#include "CUIButtonScript.h"

class CFlipbookClickScript :
    public CUIButtonScript
{
private:
	int m_FlipbookIdx;
	int m_DefaultFlipbookIdx;
    float m_FPS;
    bool m_WaitingForFinish;

public: 
    virtual void Begin() override;
    virtual void Tick() override;
	virtual void OnButtonClick() override;

    CLONE(CFlipbookClickScript);

public:
    CFlipbookClickScript();
    virtual ~CFlipbookClickScript();
};

