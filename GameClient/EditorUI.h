#pragma once
#include "Entity.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"

class EditorUI :
    public Entity
{
private:
    string                  m_UIName;
    string  			    m_UIKey; 

	bool 				    m_IsModal; /// 모달 창 여부 (모달 창이면, 다른 UI 들이 클릭되지 않도록 막는다)
    bool 	                m_Active;

    EditorUI*               m_Parent;
    vector<Ptr<EditorUI>>   m_ChildUI;

	Vec2 				    m_SizeAsChild; // 자식 UI 인 경우, 담당하는 영역 크기

public:
    GET_SET(Vec2, SizeAsChild);
    GET_SET(string, UIName);

    void SetModal(bool _Modal) { m_IsModal = _Modal; }
    bool IsActive() { return m_Active; }
    void SetActive(bool _Active) 
    { 
        if (m_Active == _Active) 
            return; 
        
        m_Active = _Active; 

        if (m_Active)
            Activate();
        else
            Deactivate();
    }

    void AddChildUI(Ptr<EditorUI> _Child)
    {
        _Child->m_Parent = this;
        m_ChildUI.push_back(_Child);
    }

protected:
    void SetUIKey(const string& _Key) { m_UIKey = _Key; }

public:
    virtual void Tick();
    virtual void Tick_UI() = 0;
    virtual void Activate() {}
    virtual void Deactivate() {}

private:
    void CheckFocus();

public:
    EditorUI(const string& _Name);
    virtual ~EditorUI();

};

typedef void(EditorUI::* DELEGATE_0)(void);
typedef void(EditorUI::* DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);
