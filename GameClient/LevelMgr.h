#pragma once

#include "ALevel.h"

class LevelMgr
	:public singleton<LevelMgr>
{
	SINGLE(LevelMgr);

private:
	Ptr<ALevel>		m_CurLevel;
	Ptr<ALevel> 	m_SharedLevel; // 에셋매니저를 통해서 관리되는 레벨 에셋 // 원본인 에셋을 가리킴

	/// 가리키고 있는 레벨을 복사하여 플레이 시키고 가리킴
	/// 게임이 끝나면 복사한 레벨을 지우고 다시 원본 레벨을 가리키게함

	LEVEL_STATE 	m_LevelState;

public:
	Ptr<ALevel> GetCurrentLevel() { return m_CurLevel; }
	Ptr<GameObject> FindObjectByName(const wstring& _name);

	LEVEL_STATE GetLevelState() { return m_LevelState; }

private:
	void ChangeLevel(Ptr<ALevel> _NextLevel);
	void ChangeLevelState(LEVEL_STATE _NextState);


public:
	void Init();
	void Progress();

	friend class TaskMgr;
};

