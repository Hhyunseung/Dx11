#pragma once
#include "CScript.h"

enum class UI_BUTTON_STATE
{
	NORMAL,
	PRESSED,
};

class CUIButtonScript :
    public CScript
{
private:
	Vec2 m_Size;
	
	bool m_IsPressed;

public:
	void SetPressed(bool _Pressed) { m_IsPressed = _Pressed; }

public:
	virtual void Tick() override;

	virtual void OnButtonClick() {}

	virtual void OnButtonDown() {}
	virtual void OnButtonUp() {}

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CUIButtonScript);

public:
	CUIButtonScript();
	virtual ~CUIButtonScript();
};

