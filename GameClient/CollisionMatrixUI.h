#pragma once
#include "EditorUI.h"

class CollisionMatrixUI :
    public EditorUI
{
private:
    char		m_LayerNameBuffer[MAX_LAYER][256];

public:
    virtual void Tick_UI() override;

public:
    CollisionMatrixUI();
    virtual ~CollisionMatrixUI();
};
