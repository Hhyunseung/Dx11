#include "pch.h"
#include "CGamePlaySpawnScript.h"

#include "GamePlayMgr.h"

CGamePlaySpawnScript::CGamePlaySpawnScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYSPAWNSCRIPT)
	, m_vecSpawnPos(-650.f, -95.f, 100.f, 0.f)
{
}

CGamePlaySpawnScript::~CGamePlaySpawnScript()
{
}

void CGamePlaySpawnScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vecSpawnPos, L"VecSpawnPos", true, 0.f);
}

void CGamePlaySpawnScript::Begin()
{
	GamePlayMgr::GetInst()->Init();

	Ptr<APrefab> pPrefab = GamePlayMgr::GetInst()->GetSelectedCharacterPrefab();
	if (pPrefab == nullptr)
		return;

	GameObject* pPlayer = pPrefab->Instantiate();
	pPlayer->Transform()->SetRelativePos(Vec3(m_vecSpawnPos.x, m_vecSpawnPos.y, m_vecSpawnPos.z));

	CreateObject(pPlayer, 4);
}

void CGamePlaySpawnScript::Tick()
{

}

void CGamePlaySpawnScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vecSpawnPos, sizeof(Vec3), 1, _File);
}

void CGamePlaySpawnScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vecSpawnPos, sizeof(Vec3), 1, _File);
}
