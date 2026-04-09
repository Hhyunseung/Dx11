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
	, m_CollectEffect(nullptr)
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
    AddScriptParam(SCRIPT_PARAM::PREFAB, &m_CollectEffect, L"CollectEffect", true, 0.f);
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
    if (m_CollectEffect == nullptr)
        return;

    Ptr<GameObject> pEffect = m_CollectEffect->Instantiate();
    if (pEffect == nullptr)
        return;

    Vec3 vPos = Transform()->GetRelativePos();
    pEffect->Transform()->SetRelativePos(vPos);

    // 이펙트가 들어갈 레이어 번호는 프로젝트 구조에 맞게 조정
	LevelMgr::GetInst()->GetCurrentLevel()->AddObject(8, pEffect);
}

void CItemScript::ReturnToPool()
{
}

void CItemScript::SaveToLevelFile(FILE* _File)
{
}

void CItemScript::LoadFromLevelFile(FILE* _File)
{
}
