#pragma once
#include "CScript.h"
class CPausePanelScript :
    public CScript
{
public:
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CPausePanelScript);

public:
	CPausePanelScript();
	virtual ~CPausePanelScript();
};

