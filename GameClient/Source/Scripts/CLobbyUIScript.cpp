#include "pch.h"
#include "CLobbyUIScript.h"

#include "GamePlayMgr.h"
#include "AssetMgr.h"
#include "TaskMgr.h"
#include "APrefab.h"
#include "GameObject.h"

CLobbyUIScript::CLobbyUIScript()
	: CScript(SCRIPT_TYPE::LOBBYUISCRIPT)
	, m_PreviewPrefabTK(nullptr)
	, m_PreviewPrefabLychee(nullptr)
	, m_PreviewLayerIdx(1)
	, m_CurPreviewObj(nullptr)
{
}

CLobbyUIScript::~CLobbyUIScript()
{
}

void CLobbyUIScript::Begin()
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_PreviewPrefabTK,     L"PreviewPrefab_TK");
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_PreviewPrefabLychee, L"PreviewPrefab_Lychee");
	AddScriptParam(SCRIPT_PARAM::INT,    &m_PreviewLayerIdx,      L"PreviewLayerIdx");

	// 현재 선택된 캐릭터로 기본 미리보기 생성
	SpawnPreview(GamePlayMgr::GetInst()->GetSelectedCharacterType());
}

void CLobbyUIScript::OnCharacterSelected(ECharacterType _Type)
{
	DestroyPreview();
	SpawnPreview(_Type);
}

void CLobbyUIScript::SpawnPreview(ECharacterType _Type)
{
	Ptr<APrefab> pPrefab = GetPreviewPrefab(_Type);
	if (pPrefab == nullptr)
		return;

	m_CurPreviewObj = pPrefab->Instantiate();
	CreateObject(m_CurPreviewObj, m_PreviewLayerIdx);
}

void CLobbyUIScript::DestroyPreview()
{
	if (m_CurPreviewObj == nullptr || m_CurPreviewObj->IsDead())
	{
		m_CurPreviewObj = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type    = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_CurPreviewObj;
	TaskMgr::GetInst()->AddTask(info);
	m_CurPreviewObj = nullptr;
}

Ptr<APrefab> CLobbyUIScript::GetPreviewPrefab(ECharacterType _Type) const
{
	switch (_Type)
	{
	case ECharacterType::TimeKeeper: return m_PreviewPrefabTK;
	case ECharacterType::Lychee:     return m_PreviewPrefabLychee;
	default:                         return nullptr;
	}
}

void CLobbyUIScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_PreviewLayerIdx, sizeof(int), 1, _File);
}

void CLobbyUIScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_PreviewLayerIdx, sizeof(int), 1, _File);
}
