#include "pch.h"
#include "CItemScript.h"

#include "CPlayerScript.h"
#include "GamePlayMgr.h"
#include "LevelMgr.h"
#include "ObjectPoolMgr.h"

CItemScript::CItemScript()
	: CScript(SCRIPT_TYPE::ITEMSCRIPT)
	, m_ItemType(EItemType::Giant)
	, m_Duration(5.f)
	, m_HealAmount(1)
	, m_ObjectID(EObjectID::Item_Giant)
	, m_bSpawned(false)
{
}

CItemScript::~CItemScript()
{
}

void CItemScript::Init()
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_ItemType, L"ItemType (0 = Giant, 1 = Boost, 2 = Magnet, 3 = HP)", true, 0.f);
    AddScriptParam(SCRIPT_PARAM::INT, &m_HealAmount, L"HealAmount", true, 0.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, L"Duration", true, 0.f);
    AddScriptParam(SCRIPT_PARAM::EObjectID, &m_ObjectID, L"ObjectID", true, 0.f);
}

void CItemScript::Begin()
{
}

void CItemScript::Tick()
{
}


void CItemScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    CPlayerScript* pPlayer = GamePlayMgr::GetInst()->GetPlayerScript();
    if (pPlayer == nullptr)
        return;

    switch (m_ItemType)
    {
    case EItemType::Giant:
        pPlayer->ActivateGiant(m_Duration);
        break;

    case EItemType::Boost:
        pPlayer->ActivateBoost(m_Duration);
        break;

    case EItemType::Magnet:
        pPlayer->ActivateMagnet(m_Duration);
        break;

    case EItemType::HealHP:
        pPlayer->Heal(m_HealAmount);
        break;
    }

    ObjectPoolMgr::GetInst()->Return(m_ObjectID, GetOwner());
}

void CItemScript::OnSpawn()
{
    if (GetOwner()->FlipbookRender() != nullptr)
    {
        GetOwner()->FlipbookRender()->Play(0, 12.f, -1);
    }

    ADD_DYNAMIC_BEGIN_OVERLAP(CItemScript::BeginOverlap);
}

void CItemScript::SpawnCollectEffect()
{
    const wchar_t* effectPath = nullptr;

    switch (m_ItemType)
    {
    case EItemType::Giant:
        effectPath = L"Prefab\\Effect_TextGiant.pref";
        break;

    case EItemType::Boost:
        effectPath = L"Prefab\\Effect_TextBoost.pref";
        break;

    case EItemType::Magnet:
        effectPath = L"Prefab\\Effect_TextMagnet.pref";
        break;

    case EItemType::HealHP:
        effectPath = L"Prefab\\Effect_TextHeal.pref";
        break;
    }

    if (effectPath == nullptr)
        return;

    Vec3 vPos = Transform()->GetRelativePos();
    ObjectPoolMgr::GetInst()->SpawnEffect(effectPath, vPos, true);
}

void CItemScript::ReturnToPool()
{
}


void CItemScript::SaveToLevelFile(FILE* _File)
{
	int itemType = (int)m_ItemType;
	fwrite(&itemType, sizeof(int), 1, _File);
    fwrite(&m_HealAmount, sizeof(int), 1, _File);
    fwrite(&m_ObjectID, sizeof(EObjectID), 1, _File);
}

void CItemScript::LoadFromLevelFile(FILE* _File)
{
	int itemType = 0;
	fread(&itemType, sizeof(int), 1, _File);
	m_ItemType = (EItemType)itemType;
    fread(&m_HealAmount, sizeof(int), 1, _File);
    fread(&m_ObjectID, sizeof(EObjectID), 1, _File);
}
