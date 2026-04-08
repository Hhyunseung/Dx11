#pragma once
#include "CScript.h"

enum class TEXT_BIND_TYPE
{
	NONE,
	SCORE,
	COIN,
	CRYSTAL,
	CURRENT_LEVEL,
};

class CTextUIScript :
    public CScript
{
private:
	TEXT_BIND_TYPE m_BindType;		// 텍스트가 어떤 데이터에 바인딩되어 있는지

	wstring m_Text;          
	float m_PosX;
	float m_PosY;
	float m_FontSize;
	float m_OutlineThickness;

public:
	virtual void Init() override;
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CTextUIScript);

public:
	CTextUIScript();
	virtual ~CTextUIScript();
};

