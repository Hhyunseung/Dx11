#pragma once

#include "ALevel.h"

class LevelMgr
	:public singleton<LevelMgr>
{
	SINGLE(LevelMgr);

private:
	Ptr<ALevel>		m_CurLevel;

public:
	Ptr<ALevel> GetCurrentLevel() { return m_CurLevel; }
	Ptr<GameObject> FindObjectByName(const wstring& _name);

public:
	void Init();
	void Progress();

};

