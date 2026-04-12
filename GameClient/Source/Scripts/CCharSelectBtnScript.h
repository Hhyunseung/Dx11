#pragma once
#include "CUIButtonScript.h"

class CCharSelectBtnScript :
    public CUIButtonScript
{
private:
    ECharacterType m_CharType;

public:
    void SetCharType(ECharacterType _Type) { m_CharType = _Type; }
    ECharacterType GetCharType() const { return m_CharType; }

public:
	virtual void Init() override;

    virtual void OnButtonClick() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCharSelectBtnScript);

public:
    CCharSelectBtnScript();
    CCharSelectBtnScript(ECharacterType _Type);
    virtual ~CCharSelectBtnScript();
};