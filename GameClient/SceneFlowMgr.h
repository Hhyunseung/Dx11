#pragma once

#include "Ptr.h"

class ASound;

enum class SCENE_FLOW_ACTION
{
	NONE,
	GO_LOBBY,
	GO_SCORE,
	START_GAME,
	RESTART_GAME,
	GAME_OVER_TO_LOBBY,
};

class SceneFlowMgr
	: public singleton<SceneFlowMgr>
{
	SINGLE(SceneFlowMgr);

private:
	bool m_IsChangingLevel; // 레벨 변경 중인지 여부
	bool m_bRunResultCommit; // 결과 화면에서 점수 커밋이 완료되었는지 여부
	wstring m_PendingLevelKey; // 현재 대기 중인 레벨 키 (레벨 변경 요청 시 저장)

	Ptr<ASound> m_CurBGM; // 현재 재생 중인 배경음악

public:
	void Init();
	void Clear();

	bool IsChangingLevel() const { return m_IsChangingLevel; }

private:
	void RequestLevelChange(const wstring& _LevelKey); // 레벨 변경 요청
	void RequestLevelPlay(); // 레벨 플레이 요청

public:
	void ExecuteAction(SCENE_FLOW_ACTION _Action);

	void GoToLobby();
	void GoToScore();
	void StartGame();
	void RestartGame();

	// 게임 종료 후 결과 반영
	void CommitRunResult();
	void ResetRunResultCommitFlag();

	// 새 레벨 진입 후 호출
	void OnLevelEntered();

	// BGM 관리
	void PlayBGM(const wstring& _SoundKey, float _Volume = 0.5f);
	void StopBGM();
};

