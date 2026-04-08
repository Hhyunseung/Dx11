#pragma once

#include "APrefab.h"
#include "AStageData.h"
#include "Source/Scripts/CPlayerScript.h"
#include "Source/Scripts/CGamePlayUIScript.h"

class GamePlayMgr
	: public singleton<GamePlayMgr>
{
	SINGLE(GamePlayMgr);

private:
	CGamePlayUIScript* m_GamePlayUI;

	Ptr<APrefab> m_SeletectedCharacterPrefab;
	GameObject*  m_PlayerObject;
	CPlayerScript* m_PlayerScript;

	ECharacterType m_SelectedCharacterType; // 현재 선택된 캐릭터 타입

	Ptr<AStageData> m_StageData;	// 현재 편집중인 StageData
	
	int m_Score;
	float m_ScrollSpeed;			// 현재 월드 스크롤 속도

	int m_RunCoin;					// 현재 얻은 코인 수 (점수 계산용)

public:
	void Init();
	void Clear();

	void SetGamePlayUIScript(CGamePlayUIScript* _pUI) { m_GamePlayUI = _pUI; }
	CGamePlayUIScript* GetGamePlayUIScript() { return m_GamePlayUI; }

	void SetSelectedCharacterPrefab(Ptr<APrefab> _Prefab) { m_SeletectedCharacterPrefab = _Prefab; }
	Ptr<APrefab> GetSelectedCharacterPrefab() { return m_SeletectedCharacterPrefab; }

	void SetPlayerObject(GameObject* _PlayerObject) { m_PlayerObject = _PlayerObject; }
	GameObject* GetPlayerObject() { return m_PlayerObject; }

	void SetPlayerScript(CPlayerScript* _PlayerScript) { m_PlayerScript = _PlayerScript; }
	CPlayerScript* GetPlayerScript() { return m_PlayerScript; }

	void SetSelectedCharacterType(ECharacterType _Type) { m_SelectedCharacterType = _Type; }
	ECharacterType GetSelectedCharacterType() const { return m_SelectedCharacterType; }


	void AddScore(int _Score) { m_Score += _Score; }
	int GetScore() const { return m_Score; }

	void AddRunCoin(int _Amount) { m_RunCoin += _Amount; }
	int GetRunCoin() const { return m_RunCoin; }

	// StageData
	void SetStageData(Ptr<AStageData> _StageData) { m_StageData = _StageData; }
	Ptr<AStageData> GetStageData() { return m_StageData; }
	void CreateNewStageData();	// 새 StageData 생성

	// 스크롤 속도 (전역 접근용)
	void SetScrollSpeed(float _Speed) { m_ScrollSpeed = _Speed; }
	float GetScrollSpeed() const { return m_ScrollSpeed; }

	void ApplySkillToPlayer();

	// ObjectID -> Prefab Key 변환
	static wstring GetPrefabKey(int _ObjectID);

private:
	CCookieSkillScript* CreateSkillByCharacterType(ECharacterType _Type);
};

