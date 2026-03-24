#pragma once

#include "APrefab.h"
#include "AStageData.h"

class GamePlayMgr
	: public singleton<GamePlayMgr>
{
	SINGLE(GamePlayMgr);

private:
	Ptr<APrefab> m_SeletectedCharacterPrefab;

	GameObject*  m_PlayerObject;

	int m_Score;

	Ptr<AStageData> m_StageData;	// 현재 편집중인 StageData

public:
	void Init();
	void Clear();

	void SetSelectedCharacterPrefab(Ptr<APrefab> _Prefab) { m_SeletectedCharacterPrefab = _Prefab; }
	Ptr<APrefab> GetSelectedCharacterPrefab() { return m_SeletectedCharacterPrefab; }

	void SetPlayerObject(GameObject* _PlayerObject) { m_PlayerObject = _PlayerObject; }
	GameObject* GetPlayerObject() { return m_PlayerObject; }

	void AddScore(int _Score) { m_Score += _Score; }
	int GetScore() const { return m_Score; }

	// StageData
	void SetStageData(Ptr<AStageData> _StageData) { m_StageData = _StageData; }
	Ptr<AStageData> GetStageData() { return m_StageData; }
	void CreateNewStageData();	// 새 StageData 생성
};

