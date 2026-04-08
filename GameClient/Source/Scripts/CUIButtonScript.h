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
	Vec2 m_Size; // 안쓰는중 // 자동으로 transform의 크기를 가져오도록 바꿈

protected:
	bool m_IsPressed;		// 버튼이 눌린 상태인지 여부
	bool m_IsMouseHolding;	// 마우스가 버튼을 누르고 있는지 여부
	

public:
	bool GetPressed() const { return m_IsPressed; }
	void SetPressed(bool _Pressed);

	bool IsMouseHolding() const { return m_IsMouseHolding; }
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
	// 자식 클래스용 생성자
	CUIButtonScript(SCRIPT_TYPE _Type);
	CUIButtonScript();
	virtual ~CUIButtonScript();
};

