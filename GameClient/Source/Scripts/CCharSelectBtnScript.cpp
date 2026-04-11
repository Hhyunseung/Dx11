#include "pch.h"
#include "CCharSelectBtnScript.h"

#include "CLobbyUIScript.h"

#include "GamePlayMgr.h"
#include "AssetMgr.h"
#include "GameObject.h"

CCharSelectBtnScript::CCharSelectBtnScript()
	: CUIButtonScript(SCRIPT_TYPE::CHARSELECTBTNSCRIPT)
	, m_CharType(ECharacterType::TimeKeeper)
{
}

CCharSelectBtnScript::~CCharSelectBtnScript()
{
}

void CCharSelectBtnScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_CharType, L"CharType");
}

void CCharSelectBtnScript::OnButtonClick()
{
	// GamePlayMgr 에 선택된 캐릭터 타입 설정
	GamePlayMgr::GetInst()->SetSelectedCharacterType(m_CharType);

	// 게임용 프리팹 키가 지정된 경우 해당 프리팹도 설정
	if (!m_GamePrefabKey.empty())
	{
		Ptr<APrefab> pPrefab = FIND(APrefab, m_GamePrefabKey);
		if (pPrefab != nullptr)
			GamePlayMgr::GetInst()->SetSelectedCharacterPrefab(pPrefab);
	}

	// 부모 오브젝트의 CLobbyUIScript 에 미리보기 교체 요청
	Ptr<GameObject> pParent = GetOwner()->GetParent();
	if (pParent != nullptr)
	{
		Ptr<CLobbyUIScript> pLobby = pParent->GetScript<CLobbyUIScript>();
		if (pLobby != nullptr)
			pLobby->OnCharacterSelected(m_CharType);
	}
}

void CCharSelectBtnScript::SaveToLevelFile(FILE* _File)
{
	int type = (int)m_CharType;
	fwrite(&type, sizeof(int), 1, _File);

	int len = (int)m_GamePrefabKey.length();
	fwrite(&len, sizeof(int), 1, _File);
	if (len > 0)
		fwrite(m_GamePrefabKey.c_str(), sizeof(wchar_t), len, _File);
}

void CCharSelectBtnScript::LoadFromLevelFile(FILE* _File)
{
	int type = 0;
	fread(&type, sizeof(int), 1, _File);
	m_CharType = (ECharacterType)type;

	int len = 0;
	fread(&len, sizeof(int), 1, _File);
	if (len > 0)
	{
		m_GamePrefabKey.resize(len);
		fread(&m_GamePrefabKey[0], sizeof(wchar_t), len, _File);
	}
}
