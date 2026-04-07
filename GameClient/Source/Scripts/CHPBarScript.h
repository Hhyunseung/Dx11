#pragma once
#include "CScript.h"

class CHPBarScript :
    public CScript
{
private:
	GameObject* m_HPBarFill;


	float m_OriginWidth;
	Vec3 m_OriginPos;
	Vec3 m_OriginScale;

public:
	void SetHPBarFill(GameObject* _Fill) { m_HPBarFill = _Fill; }
	GameObject* GetHPBarFill() const { return m_HPBarFill; }

public:
	void SetHP(int _CurHP, int _MaxHP);

public:
	virtual void Begin() override;
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}
	
	CLONE(CHPBarScript);

public:
	CHPBarScript();
	virtual ~CHPBarScript();

	friend class CPlayerScript;
};

