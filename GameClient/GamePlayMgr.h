#pragma once

#include "APrefab.h"
#include "AStageData.h"
#include "Source/Scripts/CPlayerScript.h"

class GamePlayMgr
	: public singleton<GamePlayMgr>
{
	SINGLE(GamePlayMgr);

private:
	Ptr<APrefab> m_SeletectedCharacterPrefab;

	GameObject*  m_PlayerObject;
	CPlayerScript* m_PlayerScript;

	int m_Score;

	Ptr<AStageData> m_StageData;	// 현재 편집중인 StageData

	float m_ScrollSpeed;			// 현재 월드 스크롤 속도

public:
	void Init();
	void Clear();

	void SetSelectedCharacterPrefab(Ptr<APrefab> _Prefab) { m_SeletectedCharacterPrefab = _Prefab; }
	Ptr<APrefab> GetSelectedCharacterPrefab() { return m_SeletectedCharacterPrefab; }

	void SetPlayerObject(GameObject* _PlayerObject) { m_PlayerObject = _PlayerObject; }
	GameObject* GetPlayerObject() { return m_PlayerObject; }

	void SetPlayerScript(CPlayerScript* _PlayerScript) { m_PlayerScript = _PlayerScript; }
	CPlayerScript* GetPlayerScript() { return m_PlayerScript; }

	void AddScore(int _Score) { m_Score += _Score; }
	int GetScore() const { return m_Score; }

	// StageData
	void SetStageData(Ptr<AStageData> _StageData) { m_StageData = _StageData; }
	Ptr<AStageData> GetStageData() { return m_StageData; }
	void CreateNewStageData();	// 새 StageData 생성

	// 스크롤 속도 (전역 접근용)
	void SetScrollSpeed(float _Speed) { m_ScrollSpeed = _Speed; }
	float GetScrollSpeed() const { return m_ScrollSpeed; }

	// ObjectID -> Prefab Key 변환
	static wstring GetPrefabKey(int _ObjectID);
};

