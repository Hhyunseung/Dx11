#pragma once
#include "CScript.h"

class CGamePlaySpawnScript :
	public CScript
{
private:
	Vec4 m_vecPlayerSpawnPos;

private:
	void SpawnPlayer();
	void SpawnStageObjects();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CGamePlaySpawnScript);

public:
	CGamePlaySpawnScript();
	virtual ~CGamePlaySpawnScript();
};