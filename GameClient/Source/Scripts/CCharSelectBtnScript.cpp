#include "pch.h"
#include "CCharSelectBtnScript.h"

#include "GamePlayMgr.h"

CCharSelectBtnScript::CCharSelectBtnScript()
    : CUIButtonScript(SCRIPT_TYPE::CHARSELECTBTNSCRIPT)
    , m_CharType(ECharacterType::TimeKeeper)
{
}

CCharSelectBtnScript::CCharSelectBtnScript(ECharacterType _Type)
    : CUIButtonScript(SCRIPT_TYPE::CHARSELECTBTNSCRIPT)
    , m_CharType(_Type)
{
}

CCharSelectBtnScript::~CCharSelectBtnScript()
{
}

void CCharSelectBtnScript::Init()
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_CharType, L"CharType", true, 0.f);
}

void CCharSelectBtnScript::OnButtonClick()
{
    GamePlayMgr::GetInst()->SetPrefabForCharType(m_CharType);
}

void CCharSelectBtnScript::SaveToLevelFile(FILE* _File)
{
    int action = (int)m_CharType;
    fwrite(&action, sizeof(int), 1, _File);
}

void CCharSelectBtnScript::LoadFromLevelFile(FILE* _File)
{
    int action = 0;
    fread(&action, sizeof(int), 1, _File);
    m_CharType = (ECharacterType)action;
}