#include "pch.h"
#include "CGamePlaySpawnScript.h"

#include "GamePlayMgr.h"

CGamePlaySpawnScript::CGamePlaySpawnScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYSPAWNSCRIPT)
	, m_vecPlayerSpawnPos(-650.f, -95.f, 100.f, 0.f)
{
}

CGamePlaySpawnScript::~CGamePlaySpawnScript()
{
}

void CGamePlaySpawnScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vecPlayerSpawnPos, L"VecSpawnPos", true, 0.f);
}

void CGamePlaySpawnScript::Begin()
{
	GamePlayMgr::GetInst()->Init();

	Ptr<APrefab> pPrefab = GamePlayMgr::GetInst()->GetSelectedCharacterPrefab();
	if (pPrefab == nullptr)
		return;

	GameObject* pPlayer = pPrefab->Instantiate();
	pPlayer->Transform()->SetRelativePos(Vec3(m_vecPlayerSpawnPos.x, m_vecPlayerSpawnPos.y, m_vecPlayerSpawnPos.z));

	CreateObject(pPlayer, 4);
}

void CGamePlaySpawnScript::Tick()
{

}

void CGamePlaySpawnScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vecPlayerSpawnPos, sizeof(Vec3), 1, _File);
}

void CGamePlaySpawnScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vecPlayerSpawnPos, sizeof(Vec3), 1, _File);
}
