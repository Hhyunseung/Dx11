#pragma once
#include "CScript.h"
class CTimeKeeperScript :
    public CScript
{
public:
	void Tick();

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CTimeKeeperScript);

public:
	CTimeKeeperScript();
	CTimeKeeperScript(CScript* _Owner);
	virtual ~CTimeKeeperScript();
};

