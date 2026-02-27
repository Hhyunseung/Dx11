#pragma once
#include "EditorUI.h"
#include "TreeUI.h"

/// 에셋 목록들을 보여주는 UI
class ContentUI :
    public EditorUI
{
private:
	Ptr<TreeUI>    m_Tree;

public:
    virtual void Tick_UI() override;
    void Renew();

private:
    void SelectAsset(DWORD_PTR _Asset);

public:
    ContentUI();
    virtual ~ContentUI();
};

