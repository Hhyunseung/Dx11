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

protected:
	bool m_IsPressed;		// 버튼이 눌린 상태인지 여부
	float m_PressDuration;  // 버튼이 눌린 상태로 유지된 시간
	

public:
	bool GetPressed() const { return m_IsPressed; }
	void SetPressed(bool _Pressed);

	bool IsMouseOver();

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

