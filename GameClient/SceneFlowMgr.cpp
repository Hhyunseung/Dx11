#include "pch.h"
#include "SceneFlowMgr.h"

#include "TaskMgr.h"
#include "GamePlayMgr.h"
#include "GameDataMgr.h"

SceneFlowMgr::SceneFlowMgr()
	: m_IsChangingLevel(false)
	, m_bRunResultCommit(false)
{
}

SceneFlowMgr::~SceneFlowMgr()
{
}

void SceneFlowMgr::Init()
{
	m_IsChangingLevel = false;
	m_bRunResultCommit = false;
	m_PendingLevelKey.clear();
}

void SceneFlowMgr::Clear()
{
	m_IsChangingLevel = false;
	m_bRunResultCommit = false;
	m_PendingLevelKey.clear();
}

void SceneFlowMgr::ExecuteAction(SCENE_FLOW_ACTION _Action)
{
	if (m_IsChangingLevel)
		return;

	switch (_Action)
	{
	case SCENE_FLOW_ACTION::GO_LOBBY:
		GoToLobby();
		break;

	case SCENE_FLOW_ACTION::GO_CHARACTER_SELECT:
		GoToCharacterSelect();
		break;

	case SCENE_FLOW_ACTION::START_GAME:
		StartGame();
		break;

	case SCENE_FLOW_ACTION::RESTART_GAME:
		RestartGame();
		break;

	case SCENE_FLOW_ACTION::GAME_OVER_TO_LOBBY:
		CommitRunResult();
		GoToLobby();
		break;

	default:
		break;
	}
}

void SceneFlowMgr::GoToLobby()
{
	RequestLevelChange(L"Level\\LobbyLevel.lv");
	RequestLevelPlay();
}

void SceneFlowMgr::GoToCharacterSelect()
{
	RequestLevelChange(L"Level\\CharacterSelect.lv");
	RequestLevelPlay();
}

void SceneFlowMgr::StartGame()
{
	ResetRunResultCommitFlag();

	// 현재 판 데이터 초기화
	GamePlayMgr::GetInst()->Init();

	const wstring& mapKey = GameDataMgr::GetInst()->GetSelectedStage();

	if (mapKey.empty())
		RequestLevelChange(L"Level\\TestLevel.lv");
	else
		RequestLevelChange(mapKey);

	RequestLevelPlay();
}

void SceneFlowMgr::RestartGame()
{
	ResetRunResultCommitFlag();

	// 재시작 시 현재 판 데이터 초기화
	GamePlayMgr::GetInst()->Init();

	const wstring& mapKey = GameDataMgr::GetInst()->GetSelectedStage();

	if (mapKey.empty())
		RequestLevelChange(L"Level\\TestLevel.lv");
	else
		RequestLevelChange(mapKey);

	RequestLevelPlay();
}

void SceneFlowMgr::CommitRunResult()
{
	if (m_bRunResultCommit)
		return;

	GameDataMgr::GetInst()->UpdateBestScore(GamePlayMgr::GetInst()->GetScore());
	GameDataMgr::GetInst()->AddTotalCoin(GamePlayMgr::GetInst()->GetRunCoin());
	GameDataMgr::GetInst()->Save();

	m_bRunResultCommit = true;
}

void SceneFlowMgr::ResetRunResultCommitFlag()
{
	m_bRunResultCommit = false;
}

void SceneFlowMgr::OnLevelEntered()
{
	m_IsChangingLevel = false;
}

void SceneFlowMgr::RequestLevelChange(const wstring& _LevelKey)
{
	if (m_IsChangingLevel)
		return;

	m_IsChangingLevel = true;
	m_PendingLevelKey = _LevelKey;

	TaskInfo info = {};
	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param_0 = (DWORD_PTR)m_PendingLevelKey.c_str();

	TaskMgr::GetInst()->AddTask(info);
}

void SceneFlowMgr::RequestLevelPlay()
{
	TaskInfo info = {};
	info.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	info.Param_0 = (DWORD_PTR)LEVEL_STATE::PLAY;

	TaskMgr::GetInst()->AddTask(info);
}