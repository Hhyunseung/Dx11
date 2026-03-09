#pragma once
#include "CScript.h"

class CLycheeScript :
    public CScript
{
public:
	void Tick();

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CLycheeScript);

public:
	CLycheeScript();
	CLycheeScript(CScript* _Owner);
	virtual ~CLycheeScript();
};

