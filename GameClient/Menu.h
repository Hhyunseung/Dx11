#pragma once
#include "EditorUI.h"
class Menu :
    public EditorUI
{
private:

public:
    virtual void Tick() override;
    virtual void Tick_UI() override;

public:
    Menu();
    virtual ~Menu();
};

