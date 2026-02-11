#pragma once
#include "Entity.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"

class EditorUI :
    public Entity
{
private:
    string      m_UIName;
    bool 	    m_Active;

public:
    bool IsActive() { return m_Active; }
    void SetActive(bool _Active) { m_Active = _Active; }

public:
	void SetUIName(const string& _ID) { m_UIName = _ID; }
	string GetUIName() { return m_UIName; }

public:
    virtual void Tick();
    virtual void Tick_UI() = 0;

public:
    EditorUI();
    virtual ~EditorUI();

};

