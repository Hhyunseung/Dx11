#pragma once
#include "CUIButtonScript.h"

class CLobbyUIScript;

class CCharSelectBtnScript :
    public CUIButtonScript
{
private:
    ECharacterType  m_CharType;
    wstring         m_GamePrefabKey; // 게임용 캐릭터 프리팹 키 (ex: "Prefab\\DragonCookie.pref")

public:
    void SetCharType(ECharacterType _Type) { m_CharType = _Type; }
    ECharacterType GetCharType() const { return m_CharType; }

    void SetGamePrefabKey(const wstring& _Key) { m_GamePrefabKey = _Key; }
    const wstring& GetGamePrefabKey() const { return m_GamePrefabKey; }

public:
    virtual void Init() override;
    virtual void OnButtonClick() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCharSelectBtnScript);

public:
    CCharSelectBtnScript();
    virtual ~CCharSelectBtnScript();
};
